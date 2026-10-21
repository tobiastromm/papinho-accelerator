#include <windows.h>

#include <stdio.h>
#include <string.h>

static int wait_for_file(const char *path)
{
    unsigned int attempt;
    for (attempt = 0U; attempt < 200U; ++attempt) {
        DWORD attributes = GetFileAttributesA(path);
        if (attributes != INVALID_FILE_ATTRIBUTES &&
            (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0U)
            return 1;
        Sleep(25U);
    }
    return 0;
}

static int file_has_scenario(const char *path, const char *scenario)
{
    char expected[80];
    char content[4096];
    FILE *file = NULL;
    size_t size;
    if (sprintf_s(expected, sizeof(expected), "fixture_scenario=%s\n",
            scenario) < 0 || fopen_s(&file, path, "rb") != 0 || file == NULL)
        return 0;
    size = fread(content, 1U, sizeof(content) - 1U, file);
    fclose(file);
    content[size] = '\0';
    if (strstr(content, "fixture_version=2\n") == NULL ||
        strstr(content, expected) == NULL ||
        strstr(content, "CONTROL_host=127.0.0.1\n") == NULL ||
        strstr(content, "DATA_host=127.0.0.1\n") == NULL ||
        strstr(content, "ALPN=papacc/1\n") == NULL ||
        strstr(content, "TLS_version=1.3\n") == NULL ||
        strstr(content, "mTLS=required\n") == NULL) return 0;
    if (strncmp(scenario, "mode-b-", 7U) == 0)
        return strstr(content, "target_port=0\n") == NULL &&
            strstr(content, "target_scheme=https\n") != NULL &&
            strstr(content, "TLS_REQUIRED=1\n") != NULL &&
            strstr(content, "resolver_policy=controlled\n") != NULL &&
            strstr(content, "site_tls_verification=required\n") != NULL &&
            strstr(content, "expected_HTTP_status=200\n") != NULL &&
            strstr(content, "expected_HTTP_body=papacc-mode-b-ok\n") != NULL;
    return 1;
}

static int run_scenario(const char *fixture_path, const char *scenario,
    unsigned int ordinal)
{
    char ready_path[MAX_PATH * 2];
    char event_name[128];
    char command[MAX_PATH * 4];
    char temporary[MAX_PATH];
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    HANDLE stop_event = NULL;
    DWORD exit_code = 1U;
    int ok = 0;
    if (GetTempPathA(sizeof(temporary), temporary) == 0U ||
        sprintf_s(ready_path, sizeof(ready_path),
            "%spapacc-ia6-fixture-%lu-%u.ready", temporary,
            (unsigned long)GetCurrentProcessId(), ordinal) < 0 ||
        sprintf_s(event_name, sizeof(event_name),
            "Local\\papacc-ia6-fixture-stop-%lu-%u",
            (unsigned long)GetCurrentProcessId(), ordinal) < 0 ||
        sprintf_s(command, sizeof(command),
            "\"%s\" --browser-fixture %s \"%s\" \"%s\"",
            fixture_path, scenario, ready_path, event_name) < 0)
        return 0;
    DeleteFileA(ready_path);
    stop_event = CreateEventA(NULL, TRUE, FALSE, event_name);
    if (stop_event == NULL) return 0;
    memset(&startup, 0, sizeof(startup));
    memset(&process, 0, sizeof(process));
    startup.cb = sizeof(startup);
    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &startup, &process))
        goto cleanup;
    if (!wait_for_file(ready_path) ||
        !file_has_scenario(ready_path, scenario) || !SetEvent(stop_event) ||
        WaitForSingleObject(process.hProcess, 10000U) != WAIT_OBJECT_0 ||
        !GetExitCodeProcess(process.hProcess, &exit_code) || exit_code != 0U ||
        GetFileAttributesA(ready_path) != INVALID_FILE_ATTRIBUTES)
        goto cleanup_process;
    ok = 1;

cleanup_process:
    if (!ok && WaitForSingleObject(process.hProcess, 0U) == WAIT_TIMEOUT) {
        (void)SetEvent(stop_event);
        if (WaitForSingleObject(process.hProcess, 5000U) == WAIT_TIMEOUT)
            TerminateProcess(process.hProcess, 1U);
    }
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
cleanup:
    CloseHandle(stop_event);
    DeleteFileA(ready_path);
    return ok;
}

/* Egress fixtures require application work before a successful Stop.
   Reuse the real client harness, including ready metadata and clean Stop. */
static int run_egress_probe(const char *fixture, const char *integration,
    const char *client)
{
    char command[MAX_PATH * 4];
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    DWORD code = 1U;
    int ok;
    if (sprintf_s(command, sizeof(command), "\"%s\" \"%s\" \"%s\" --launcher-egress-only",
            integration, fixture, client) < 0) return 0;
    memset(&startup, 0, sizeof(startup)); startup.cb = sizeof(startup);
    memset(&process, 0, sizeof(process));
    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW,
            NULL, NULL, &startup, &process)) return 0;
    ok = WaitForSingleObject(process.hProcess, 60000U) == WAIT_OBJECT_0 &&
        GetExitCodeProcess(process.hProcess, &code) && code == 0U;
    if (!ok && WaitForSingleObject(process.hProcess, 0U) == WAIT_TIMEOUT)
        TerminateProcess(process.hProcess, 1U);
    CloseHandle(process.hThread); CloseHandle(process.hProcess);
    return ok;
}

int main(int argc, char **argv)
{
    if (argc != 4) return 2;
    if (!run_scenario(argv[1], "positive", 1U)) return 10;
    if (!run_scenario(argv[1], "stall", 2U)) return 20;
    if (!run_scenario(argv[1], "data-failure", 3U)) return 30;
    if (!run_egress_probe(argv[1], argv[2], argv[3])) return 40;
    if (!run_scenario(argv[1], "mode-b-tls-name-failure", 5U)) return 50;
    puts("FIXTURE_SCENARIO_POSITIVE=YES");
    puts("FIXTURE_SCENARIO_STALL=YES");
    puts("FIXTURE_SCENARIO_DATA_FAILURE=YES");
    puts("FIXTURE_SCENARIO_MODE_B_EGRESS=YES");
    puts("FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE=YES");
    puts("FIXTURE_READY_AND_CLEAN_STOP=PASS");
    return 0;
}
