#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int wait_for_ready(const char *path)
{
    unsigned int attempt;
    for (attempt = 0U; attempt < 400U; ++attempt) {
        if (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) return 1;
        Sleep(25U);
    }
    return 0;
}

static int ready_value(const char *path, const char *name, char *value,
    size_t capacity)
{
    FILE *file = NULL;
    char line[1024];
    size_t name_size = strlen(name);
    if (fopen_s(&file, path, "rb") != 0 || file == NULL) return 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t size;
        if (strncmp(line, name, name_size) != 0 || line[name_size] != '=')
            continue;
        size = strcspn(line + name_size + 1U, "\r\n");
        if (size + 1U > capacity) break;
        memcpy(value, line + name_size + 1U, size);
        value[size] = '\0';
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}

static int launch(char *command, PROCESS_INFORMATION *process)
{
    STARTUPINFOA startup;
    memset(&startup, 0, sizeof(startup));
    memset(process, 0, sizeof(*process));
    startup.cb = sizeof(startup);
    return CreateProcessA(NULL, command, NULL, NULL, FALSE, 0U,
        NULL, NULL, &startup, process) != 0;
}

static void close_process(PROCESS_INFORMATION *process)
{
    if (process->hThread != NULL) CloseHandle(process->hThread);
    if (process->hProcess != NULL) CloseHandle(process->hProcess);
}

static int run_case(const char *fixture_path, const char *client_path,
    const char *scenario, const char *client_scenario, unsigned int ordinal,
    unsigned int repetitions)
{
    char temporary[MAX_PATH], ready[MAX_PATH * 2], stop_name[128];
    char shutdown_name[128], ack_name[128], fixture_command[MAX_PATH * 5];
    char client_command[MAX_PATH * 5], control_port[16], target_port[16];
    char plain_target_port[16];
    char target_host[64], expected_body[64], tls_required[8];
    char network_mode[32], target_scheme[16], request_count[8];
    char tls_target_host[64], tls_target_port[16], tls_target_scheme[16];
    char tls_target_mode[32], tls_target_body[64];
    char plain_target_host[64], plain_target_scheme[16];
    char plain_target_mode[32], plain_target_body[64];
    char expected_capabilities[64], operation_count[8];
    char network_major[8], tls_granted[8], framing[32], declared[16];
    char delivered[16], http_complete[8], close_notify[8];
    char expected_body_length[16], expected_body_trailing_lf[8];
    char expected_terminal[32], expected_reason[32], fragmentation[32];
    PROCESS_INFORMATION fixture_process, client_process;
    HANDLE stop_event = NULL, shutdown_event = NULL, ack_event = NULL;
    DWORD exit_code = 1U;
    unsigned int repetition = 0U;
    int ok = 0;
    const char *failure_stage = "setup";
    memset(&fixture_process, 0, sizeof(fixture_process));
    memset(&client_process, 0, sizeof(client_process));
    if (GetTempPathA(sizeof(temporary), temporary) == 0U ||
        sprintf_s(ready, sizeof(ready), "%spapacc-ib-%lu-%u.ready", temporary,
            (unsigned long)GetCurrentProcessId(), ordinal) < 0 ||
        sprintf_s(stop_name, sizeof(stop_name), "Local\\papacc-ib-stop-%lu-%u",
            (unsigned long)GetCurrentProcessId(), ordinal) < 0 ||
        sprintf_s(shutdown_name, sizeof(shutdown_name),
            "Local\\papacc-ib-shutdown-%lu-%u",
            (unsigned long)GetCurrentProcessId(), ordinal) < 0 ||
        sprintf_s(ack_name, sizeof(ack_name), "Local\\papacc-ib-ack-%lu-%u",
            (unsigned long)GetCurrentProcessId(), ordinal) < 0)
        return 0;
    DeleteFileA(ready);
    stop_event = CreateEventA(NULL, TRUE, FALSE, stop_name);
    shutdown_event = CreateEventA(NULL, TRUE, FALSE, shutdown_name);
    ack_event = CreateEventA(NULL, TRUE, FALSE, ack_name);
    if (stop_event == NULL || shutdown_event == NULL || ack_event == NULL ||
        sprintf_s(fixture_command, sizeof(fixture_command),
            "\"%s\" --browser-fixture %s \"%s\" \"%s\"", fixture_path,
            scenario, ready, stop_name) < 0 ||
        !launch(fixture_command, &fixture_process) || !wait_for_ready(ready) ||
        !ready_value(ready, "CONTROL_port", control_port,
            sizeof(control_port)) ||
        !ready_value(ready, "target_port", target_port,
            sizeof(target_port)) ||
        !ready_value(ready, "target_host", target_host,
            sizeof(target_host)) ||
        !ready_value(ready, "TLS_REQUIRED", tls_required,
            sizeof(tls_required)) ||
        !ready_value(ready, "network_mode", network_mode,
            sizeof(network_mode)) ||
        !ready_value(ready, "target_scheme", target_scheme,
            sizeof(target_scheme)) ||
        !ready_value(ready, "expected_HTTP_body", expected_body,
            sizeof(expected_body)) ||
        !ready_value(ready, "sequential_request_count", request_count,
            sizeof(request_count)))
        goto cleanup;
    if (strncmp(scenario, "tls-", 4U) == 0 &&
        (!ready_value(ready, "NETWORK_EGRESS_major", network_major,
                sizeof(network_major)) ||
         !ready_value(ready, "TLS_OFFLOAD_granted", tls_granted,
                sizeof(tls_granted)) ||
         !ready_value(ready, "HTTP_framing", framing, sizeof(framing)) ||
         !ready_value(ready, "declared_content_length", declared,
                sizeof(declared)) ||
         !ready_value(ready, "delivered_body_length", delivered,
                sizeof(delivered)) ||
         !ready_value(ready, "expected_HTTP_body_length",
                expected_body_length, sizeof(expected_body_length)) ||
         !ready_value(ready, "expected_HTTP_body_trailing_lf",
                expected_body_trailing_lf, sizeof(expected_body_trailing_lf)) ||
         !ready_value(ready, "HTTP_complete", http_complete,
                sizeof(http_complete)) ||
         !ready_value(ready, "clean_close_notify", close_notify,
                sizeof(close_notify)) ||
         !ready_value(ready, "expected_terminal", expected_terminal,
                sizeof(expected_terminal)) ||
         !ready_value(ready, "expected_terminal_reason", expected_reason,
                sizeof(expected_reason)) ||
         !ready_value(ready, "pacc_fragmentation", fragmentation,
                sizeof(fragmentation)) ||
         strcmp(network_major, "2") != 0 ||
         strcmp(tls_granted, "YES") != 0 ||
         strcmp(expected_body_trailing_lf, "YES") != 0 ||
         strtoul(expected_body_length, NULL, 10) !=
            strtoul(delivered, NULL, 10) ||
         strlen(expected_body) + 1U !=
            strtoul(delivered, NULL, 10))) goto cleanup;
    if (strcmp(scenario, "mixed-egress") == 0) {
        if (!ready_value(ready, "plain_target_port", plain_target_port,
                sizeof(plain_target_port)) ||
            !ready_value(ready, "tls_target_host", tls_target_host,
                sizeof(tls_target_host)) ||
            !ready_value(ready, "tls_target_port", tls_target_port,
                sizeof(tls_target_port)) ||
            !ready_value(ready, "tls_target_scheme", tls_target_scheme,
                sizeof(tls_target_scheme)) ||
            !ready_value(ready, "tls_target_network_mode", tls_target_mode,
                sizeof(tls_target_mode)) ||
            !ready_value(ready, "tls_expected_HTTP_body", tls_target_body,
                sizeof(tls_target_body)) ||
            !ready_value(ready, "plain_target_host", plain_target_host,
                sizeof(plain_target_host)) ||
            !ready_value(ready, "plain_target_scheme", plain_target_scheme,
                sizeof(plain_target_scheme)) ||
            !ready_value(ready, "plain_target_network_mode",
                plain_target_mode, sizeof(plain_target_mode)) ||
            !ready_value(ready, "plain_expected_HTTP_body",
                plain_target_body, sizeof(plain_target_body)) ||
            !ready_value(ready, "expected_capabilities",
                expected_capabilities, sizeof(expected_capabilities)) ||
            !ready_value(ready, "sequential_operation_count",
                operation_count, sizeof(operation_count)) ||
            strcmp(tls_target_host, "papacc-test-server") != 0 ||
            strcmp(tls_target_port, target_port) != 0 ||
            strcmp(tls_target_scheme, "https") != 0 ||
            strcmp(tls_target_mode, "TLS_REQUIRED") != 0 ||
            strcmp(tls_target_body, "papacc-mode-b-ok") != 0 ||
            strcmp(plain_target_host, "papacc-plain-test-server") != 0 ||
            strcmp(plain_target_scheme, "http") != 0 ||
            strcmp(plain_target_mode, "TCP_PLAIN") != 0 ||
            strcmp(plain_target_body, "papacc-tcp-plain-ok") != 0 ||
            strcmp(expected_capabilities,
                "NETWORK_EGRESS,TLS_OFFLOAD") != 0 ||
            strcmp(operation_count, "3") != 0 ||
            sprintf_s(client_command, sizeof(client_command),
                "\"%s\" %s \"%s\" \"%s\" %s %s %s", client_path,
                control_port, shutdown_name, ack_name, client_scenario,
                target_port, plain_target_port) < 0) goto cleanup;
    } else if (sprintf_s(client_command, sizeof(client_command),
            "\"%s\" %s \"%s\" \"%s\" %s %s", client_path,
            control_port, shutdown_name, ack_name, client_scenario,
            target_port) < 0) goto cleanup;
    failure_stage = "client";
    Sleep(1000U);
    for (repetition = 0U; repetition < repetitions; ++repetition) {
        if (!launch(client_command, &client_process) ||
            WaitForSingleObject(client_process.hProcess, 30000U) !=
                WAIT_OBJECT_0 ||
            !GetExitCodeProcess(client_process.hProcess, &exit_code) ||
            exit_code != 0U) goto cleanup;
        close_process(&client_process);
        memset(&client_process, 0, sizeof(client_process));
        if (repetition + 1U < repetitions && strcmp(scenario,
                "secure-v2-protocol-error-no-fallback") != 0)
            Sleep(1000U);
    }
    failure_stage = "fixture-stop";
    if (!SetEvent(stop_event) ||
        WaitForSingleObject(fixture_process.hProcess, 15000U) != WAIT_OBJECT_0 ||
        !GetExitCodeProcess(fixture_process.hProcess, &exit_code) ||
        exit_code != 0U || GetFileAttributesA(ready) !=
            INVALID_FILE_ATTRIBUTES)
        goto cleanup;
    if ((strcmp(scenario, "tcp-plain-egress") == 0 &&
            (strcmp(target_host, "papacc-plain-test-server") != 0 ||
             strcmp(expected_body, "papacc-tcp-plain-ok") != 0 ||
             strcmp(tls_required, "0") != 0 ||
             strcmp(network_mode, "TCP_PLAIN") != 0 ||
             strcmp(target_scheme, "http") != 0 ||
             strcmp(request_count, "3") != 0)) ||
        (strcmp(scenario, "mixed-egress") == 0 &&
            (strcmp(network_mode, "MIXED") != 0 ||
             strcmp(request_count, "3") != 0)) ||
        (strcmp(scenario, "mode-b-egress") == 0 &&
            (strcmp(tls_required, "1") != 0 ||
             strcmp(expected_body, "papacc-mode-b-ok") != 0 ||
             strcmp(target_host, "papacc-test-server") != 0)) ||
        (strcmp(scenario, "mode-b-tls-name-failure") == 0 &&
            strcmp(target_host, "wrong.example") != 0)) goto cleanup;
    if (strstr(scenario, "tls-truncated-content-length-complete") == scenario &&
        (strcmp(framing, "CONTENT_LENGTH") != 0 ||
         strcmp(http_complete, "YES") != 0 ||
         strcmp(close_notify, "NO") != 0 ||
         strcmp(expected_terminal, "NETWORK_ABORT_REASON") != 0 ||
         strcmp(expected_reason, "TLS_TRUNCATED") != 0)) goto cleanup;
    if (strstr(scenario, "tls-truncated-content-length-incomplete") == scenario &&
        (strcmp(framing, "CONTENT_LENGTH") != 0 ||
         strcmp(http_complete, "NO") != 0 ||
         strtoul(delivered, NULL, 10) >= strtoul(declared, NULL, 10) ||
         strcmp(expected_reason, "TLS_TRUNCATED") != 0)) goto cleanup;
    if (strcmp(scenario, "tls-truncated-chunked-complete") == 0 &&
        (strcmp(framing, "CHUNKED") != 0 ||
         strcmp(http_complete, "YES") != 0 ||
         strcmp(expected_body, "papacc-chunked!") != 0 ||
         strcmp(delivered, "16") != 0)) goto cleanup;
    if (strcmp(scenario, "tls-truncated-chunked-incomplete") == 0 &&
        (strcmp(framing, "CHUNKED") != 0 ||
         strcmp(http_complete, "NO") != 0 ||
         strcmp(expected_body, "papacc-chunked!") != 0 ||
         strcmp(delivered, "16") != 0)) goto cleanup;
    if (strcmp(scenario, "tls-truncated-close-delimited") == 0 &&
        (strcmp(framing, "CLOSE_DELIMITED") != 0 ||
         strcmp(http_complete, "NO") != 0 ||
         strcmp(expected_body, "papacc-close-delimited") != 0 ||
         strcmp(delivered, "23") != 0)) goto cleanup;
    if (strcmp(scenario, "tls-clean-close") == 0 &&
        (strcmp(close_notify, "YES") != 0 ||
         strcmp(expected_terminal, "NETWORK_CLOSE") != 0 ||
         strcmp(expected_reason, "NONE") != 0)) goto cleanup;
    if (strstr(scenario, "multi-frame") != NULL &&
        strcmp(fragmentation, "MULTI_FRAME") != 0) goto cleanup;
    if (strstr(scenario, "content-length-complete") != NULL &&
        (strcmp(expected_body, "papacc-complete!") != 0 ||
         strcmp(declared, "17") != 0 || strcmp(delivered, "17") != 0))
        goto cleanup;
    if (strstr(scenario, "content-length-incomplete") != NULL &&
        (strcmp(expected_body, "papacc-incomplete") != 0 ||
         strcmp(declared, "23") != 0 || strcmp(delivered, "18") != 0))
        goto cleanup;
    ok = 1;
cleanup:
    if (!ok) {
        DWORD fixture_exit = 0U, client_exit = 0U;
        if (fixture_process.hProcess != NULL)
            (void)GetExitCodeProcess(fixture_process.hProcess, &fixture_exit);
        if (client_process.hProcess != NULL)
            (void)GetExitCodeProcess(client_process.hProcess, &client_exit);
        fprintf(stderr,
            "MODE_B_FIXTURE_FAILURE scenario=%s stage=%s repetition=%u fixture_exit=%lu client_exit=%lu\n",
            scenario, failure_stage, repetition, (unsigned long)fixture_exit,
            (unsigned long)client_exit);
    }
    if (client_process.hProcess != NULL &&
        WaitForSingleObject(client_process.hProcess, 0U) == WAIT_TIMEOUT)
        TerminateProcess(client_process.hProcess, 1U);
    if (fixture_process.hProcess != NULL &&
        WaitForSingleObject(fixture_process.hProcess, 0U) == WAIT_TIMEOUT) {
        if (stop_event != NULL) SetEvent(stop_event);
        if (WaitForSingleObject(fixture_process.hProcess, 5000U) == WAIT_TIMEOUT)
            TerminateProcess(fixture_process.hProcess, 1U);
    }
    close_process(&client_process);
    close_process(&fixture_process);
    if (ack_event != NULL) CloseHandle(ack_event);
    if (shutdown_event != NULL) CloseHandle(shutdown_event);
    if (stop_event != NULL) CloseHandle(stop_event);
    DeleteFileA(ready);
    return ok;
}

static int run_terminal_latency_trace_case(const char *fixture_path,
    const char *client_path)
{
    char temporary[MAX_PATH],trace_path[MAX_PATH * 2],line[1024];
    FILE *file=NULL;unsigned int expected=0U;unsigned long long previous=0U;
    int ok=0;
    if(GetTempPathA(sizeof(temporary),temporary)==0U||
        sprintf_s(trace_path,sizeof(trace_path),
            "%spapacc-terminal-latency-%lu.trace",temporary,
            (unsigned long)GetCurrentProcessId())<0)return 0;
    DeleteFileA(trace_path);
    if(!SetEnvironmentVariableA("PAPACC_TERMINAL_LATENCY_TRACE_FILE",
            trace_path))return 0;
    if(!run_case(fixture_path,client_path,
            "tls-truncated-content-length-complete",
            "network-http-terminal-truncated",80U,1U))goto cleanup;
    if(fopen_s(&file,trace_path,"rb")!=0||file==NULL)goto cleanup;
    while(fgets(line,sizeof(line),file)!=NULL){
        unsigned int event_index;unsigned long long elapsed;
        if(sscanf_s(line,"event=A%u name=%*s elapsed_us=%llu",&event_index,
                &elapsed)!=2)continue;
        if(event_index!=expected||(expected!=0U&&elapsed<previous))
            goto cleanup;
        if(event_index==4U&&
            (strstr(line,"pst_result=")==NULL||
             strstr(line,"classification=TLS_TRUNCATED")==NULL))goto cleanup;
        previous=elapsed;++expected;
    }
    ok=expected==10U;
cleanup:
    if(file!=NULL)fclose(file);
    (void)SetEnvironmentVariableA("PAPACC_TERMINAL_LATENCY_TRACE_FILE",NULL);
    DeleteFileA(trace_path);
    return ok;
}

int main(int argc, char **argv)
{
    if(argc==4&&strcmp(argv[3],"--terminal-latency-trace")==0){
        if(!run_terminal_latency_trace_case(argv[1],argv[2]))return 120;
        puts("TERMINAL_LATENCY_TRACE_A0_A9=PASS");
        puts("TERMINAL_LATENCY_TRACE_MONOTONIC=PASS");
        puts("TERMINAL_LATENCY_TRACE_TLS_CLASSIFICATION=PASS");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--protocol-error-repetition") == 0) {
        if (!run_case(argv[1], argv[2],
                "secure-v2-protocol-error-no-fallback",
                "network-secure-v2-protocol-error", 70U, 100U))
            return 110;
        puts("PROTOCOL_ERROR_REALIZATION_REPETITION=100/100_PASS");
        puts("NORMAL_CAPABILITY_RESULT_COUNT=0");
        puts("CLIENT_SUCCESS_BEFORE_FAULT_COUNT=0");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--negative-fallback-matrix") == 0) {
        static const char *fixture_scenarios[] = {
            "secure-v2-protocol-error-no-fallback",
            "secure-v2-auth-failure-no-fallback",
            "secure-v2-transport-failure-no-fallback",
            "secure-v2-egress-started-no-fallback"
        };
        static const char *client_scenarios[] = {
            "network-secure-v2-protocol-error",
            "network-secure-auth-failure",
            "network-secure-transport-failure",
            "network-secure-post-egress-failure"
        };
        unsigned int index;
        for (index = 0U; index < 4U; ++index)
            if (!run_case(argv[1], argv[2], fixture_scenarios[index],
                    client_scenarios[index], 50U + index, 1U))
                return 90 + (int)index;
        puts("NEGATIVE_FIXTURE_REALIZATION_MATRIX=PASS");
        puts("READY_FILE_EXPECTATION_MATCHES_REALIZATION=YES");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--fallback-matrix") == 0) {
        if (!run_case(argv[1], argv[2],
                "secure-v2-unavailable-v1-fallback",
                "network-secure-v2-to-v1-fallback", 40U, 1U))
            return 80;
        puts("SECURE_V2_TO_V1_FALLBACK_FIXTURE=PASS");
        puts("V1_FALLBACK_USES_NEW_SESSION=YES");
        puts("IN_PLACE_MAJOR_DOWNGRADE=NO");
        puts("LEGACY_CONNECTION_COUNT=0");
        puts("V1_SESSION_OPERATION=PASS");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--tls-clean-close-only") == 0) {
        if (!run_case(argv[1], argv[2], "tls-clean-close",
                "network-http-terminal-clean", 19U, 1U))
            return 59;
        puts("TLS_REQUIRED_PEER_CLEAN_CLOSE=PASS");
        puts("TLS_REQUIRED_TERMINAL=NETWORK_CLOSE");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--tls-terminal-matrix") == 0) {
        static const char *fixture_scenarios[] = {
            "tls-truncated-content-length-complete",
            "tls-truncated-content-length-incomplete",
            "tls-truncated-chunked-complete",
            "tls-truncated-chunked-incomplete",
            "tls-truncated-close-delimited",
            "tls-clean-close",
            "tls-truncated-content-length-complete-multi-frame",
            "tls-truncated-content-length-incomplete-multi-frame"
        };
        static const char *client_scenarios[] = {
            "network-http-terminal-truncated",
            "network-http-terminal-truncated",
            "network-http-terminal-truncated",
            "network-http-terminal-truncated",
            "network-http-terminal-truncated",
            "network-http-terminal-clean",
            "network-http-terminal-truncated-multi",
            "network-http-terminal-truncated-multi"
        };
        unsigned int index;
        for (index = 0U; index < sizeof(fixture_scenarios) /
                sizeof(fixture_scenarios[0]); ++index)
            if (!run_case(argv[1], argv[2], fixture_scenarios[index],
                    client_scenarios[index], 20U + index, 1U))
                return 60 + (int)index;
        puts("TLS_TERMINAL_BROWSER_FIXTURE_MATRIX=PASS");
        puts("TLS_TERMINAL_FRAGMENTATION_VARIANTS=PASS");
        puts("READY_FILE_BODY_METADATA_MATCH=PASS");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--mixed-only") == 0) {
        if (!run_case(argv[1], argv[2], "mixed-egress",
                "network-mixed-egress-sequence", 5U, 1U)) return 50;
        puts("MIXED_EGRESS_PERSISTENT_CONTROL=PASS");
        puts("MIXED_EGRESS_SEQUENCE_TLS_PLAIN_TLS=PASS");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--tcp-plain-only") == 0) {
        if (!run_case(argv[1], argv[2], "tcp-plain-egress",
                "network-tcp-plain-http-sequence", 4U, 1U)) return 40;
        puts("TCP_PLAIN_EXTERNAL_FIXTURE=PASS");
        puts("TCP_PLAIN_THREE_SEQUENTIAL_REQUESTS=PASS");
        puts("TCP_PLAIN_SITE_TLS_PST_USED=NO");
        return 0;
    }
    if (argc == 4 && strcmp(argv[3], "--launcher-egress-only") == 0) {
        if (!run_case(argv[1], argv[2], "mode-b-egress", "network-http", 40U, 1U))
            return 40;
        puts("LAUNCHER_EGRESS_REAL_APPLICATION_AND_STOP=PASS");
        return 0;
    }
    if (argc != 3) return 2;
    if (!run_case(argv[1], argv[2], "mode-b-egress", "network-tls", 1U,
            1U))
        return 10;
    if (!run_case(argv[1], argv[2], "mode-b-egress", "network-http", 2U,
            2U))
        return 20;
    if (!run_case(argv[1], argv[2], "mode-b-tls-name-failure",
            "network-negative-tls-name", 3U, 1U)) return 30;
    if (!run_case(argv[1], argv[2], "tcp-plain-egress",
            "network-tcp-plain-http-sequence", 4U, 1U)) return 40;
    if (!run_case(argv[1], argv[2], "mixed-egress",
            "network-mixed-egress-sequence", 5U, 1U)) return 50;
    puts("MODE_B_SECURE_PRINCIPAL=PASS");
    puts("MODE_B_TLS_REQUIRED_ROUNDTRIP=PASS");
    puts("MODE_B_HTTP_ROUNDTRIP=PASS");
    puts("MODE_B_TWO_FRESH_SEQUENTIAL_HTTP_SESSIONS=PASS");
    puts("MODE_B_TLS_NAME_FAILURE=PASS");
    puts("MODE_B_CLEAN_STOP=PASS");
    puts("TCP_PLAIN_EXTERNAL_FIXTURE=PASS");
    puts("TCP_PLAIN_THREE_SEQUENTIAL_REQUESTS=PASS");
    puts("TCP_PLAIN_SITE_TLS_PST_USED=NO");
    puts("MIXED_EGRESS_SEQUENCE_TLS_PLAIN_TLS=PASS");
    return 0;
}
