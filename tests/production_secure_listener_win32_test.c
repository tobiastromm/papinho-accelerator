#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

static int launch(const char *command, DWORD flags, PROCESS_INFORMATION *pi)
{
    STARTUPINFOA si;
    char mutable_command[4096];
    memset(&si, 0, sizeof(si));
    memset(pi, 0, sizeof(*pi));
    si.cb = sizeof(si);
    if (sprintf_s(mutable_command, sizeof(mutable_command), "%s", command) < 0)
        return 0;
    return CreateProcessA(NULL, mutable_command, NULL, NULL, FALSE, flags,
        NULL, NULL, &si, pi) != 0;
}

static void close_process(PROCESS_INFORMATION *pi)
{
    if (pi->hThread != NULL) CloseHandle(pi->hThread);
    if (pi->hProcess != NULL) CloseHandle(pi->hProcess);
    memset(pi, 0, sizeof(*pi));
}

static int run_client(const char *client, unsigned short port,
    const char *shutdown_name, const char *ack_name, const char *scenario,
    unsigned short target_port)
{
    char command[4096];
    PROCESS_INFORMATION pi;
    DWORD exit_code = 1U;
    if ((target_port==0U?sprintf_s(command,sizeof(command),
            "\"%s\" %u \"%s\" \"%s\" %s",client,(unsigned int)port,
            shutdown_name,ack_name,scenario):sprintf_s(command,sizeof(command),
            "\"%s\" %u \"%s\" \"%s\" %s %u",client,(unsigned int)port,
            shutdown_name,ack_name,scenario,(unsigned int)target_port)) < 0 ||
        !launch(command, CREATE_NO_WINDOW, &pi))
        return 0;
    if (WaitForSingleObject(pi.hProcess, 30000U) != WAIT_OBJECT_0 ||
        !GetExitCodeProcess(pi.hProcess, &exit_code) || exit_code != 0U) {
        fprintf(stderr, "production client scenario failed: %s exit=%lu\n",
            scenario, (unsigned long)exit_code);
        TerminateProcess(pi.hProcess, 99U);
        close_process(&pi);
        return 0;
    }
    close_process(&pi);
    return 1;
}

static int read_target_ports(const char *path,unsigned short *tls_port,
    unsigned short *plain_port)
{
    FILE *file=NULL;char text[4096];size_t size;char *tls,*plain;
    if(fopen_s(&file,path,"rb")!=0||file==NULL)return 0;
    size=fread(text,1U,sizeof(text)-1U,file);fclose(file);text[size]='\0';
    tls=strstr(text,"tls_target_port=");plain=strstr(text,"plain_target_port=");
    if(tls==NULL||plain==NULL)return 0;
    *tls_port=(unsigned short)strtoul(tls+16,NULL,10);
    *plain_port=(unsigned short)strtoul(plain+18,NULL,10);
    return *tls_port!=0U&&*plain_port!=0U;
}

static unsigned short reserve_port(void)
{
    SOCKET socket_value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in address;
    int size = sizeof(address);
    unsigned short port = 0U;
    if (socket_value == INVALID_SOCKET) return 0U;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(socket_value, (const struct sockaddr *)&address,
            sizeof(address)) == 0 &&
        getsockname(socket_value, (struct sockaddr *)&address, &size) == 0)
        port = ntohs(address.sin_port);
    closesocket(socket_value);
    return port;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PROCESS_INFORMATION server,target_process;
    HANDLE shutdown_event = NULL, ack_event = NULL;
    char server_command[4096], path[MAX_PATH * 2];
    const char *shutdown_name = "Local\\PapaccProductionSecureShutdown";
    const char *ack_name = "Local\\PapaccProductionSecureAck";
    unsigned short port;
    unsigned short tls_target_port=0U,plain_target_port=0U;
    unsigned short requested_tls_target_port=0U,requested_plain_target_port=0U;
    HANDLE target_stop_event=NULL;
    const char *target_stop_name="Local\\PapaccProductionEgressTargetStop";
    char target_command[4096],ready_path[MAX_PATH*2];
    DWORD exit_code = 1U;
    int result = 1;
    ready_path[0]='\0';
    memset(&target_process,0,sizeof(target_process));
    if (argc != 5 || WSAStartup(MAKEWORD(2, 2), &winsock) != 0) return 2;
    port = reserve_port();
    requested_tls_target_port=reserve_port();
    requested_plain_target_port=reserve_port();
    if (port == 0U || requested_tls_target_port == 0U ||
        requested_plain_target_port == 0U ||
        requested_tls_target_port == requested_plain_target_port) goto cleanup;
    shutdown_event = CreateEventA(NULL, TRUE, FALSE, shutdown_name);
    ack_event = CreateEventA(NULL, TRUE, FALSE, ack_name);
    if (shutdown_event == NULL || ack_event == NULL) goto cleanup;
    if (sprintf_s(path, sizeof(path), "%s\\server.der.b64", argv[3]) < 0 ||
        !SetEnvironmentVariableA("PAPACC_SECURITY_CONFIGURATION_REF",
            "production-test") ||
        !SetEnvironmentVariableA("PAPACC_SERVER_CERTIFICATE_DER_B64", path))
        goto cleanup;
    if (sprintf_s(path, sizeof(path), "%s\\server.pk8.b64", argv[3]) < 0 ||
        !SetEnvironmentVariableA("PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64",
            path)) goto cleanup;
    if (sprintf_s(path, sizeof(path), "%s\\ca.der.b64", argv[3]) < 0 ||
        !SetEnvironmentVariableA("PAPACC_CLIENT_CA_DER_B64", path))
        goto cleanup;
    if (!SetEnvironmentVariableA("PAPACC_ALLOWED_CLIENT_CERT_SHA256",
            "35c5e98e701d8b18e5fb85eaf1eb5d2157204ce735ab14bf7d9e2d25b8302ac2"))
        goto cleanup;
    if(GetTempPathA(sizeof(ready_path),ready_path)==0U||
        strcat_s(ready_path,sizeof(ready_path),"papacc-production-egress.ready")!=0)
        goto cleanup;
    if(ready_path[0]!='\0')DeleteFileA(ready_path);
    target_stop_event=CreateEventA(NULL,TRUE,FALSE,target_stop_name);
    if(target_stop_event==NULL||sprintf_s(target_command,sizeof(target_command),
        "\"%s\" --http-port %u --https-port %u --ready-file \"%s\" "
        "--stop-event \"%s\"",argv[4],(unsigned)requested_plain_target_port,
        (unsigned)requested_tls_target_port,ready_path,target_stop_name)<0||
        !launch(target_command,CREATE_NO_WINDOW,&target_process))goto cleanup;
    {DWORD attempt;for(attempt=0U;attempt<200U;++attempt){
        if(read_target_ports(ready_path,&tls_target_port,&plain_target_port))break;
        Sleep(25U);}if(tls_target_port==0U||plain_target_port==0U)goto cleanup;}
    if(!SetEnvironmentVariableA("PAPACC_EGRESS_POLICY_REF","nt4-lab-controlled")||
        !SetEnvironmentVariableA("PAPACC_EGRESS_PROFILE_REF","default-route")||
        !SetEnvironmentVariableA("PAPACC_OUTBOUND_TLS_PROFILE_REF","lab-private-ca")||
        sprintf_s(path,sizeof(path),"%s\\ca.der.b64",argv[3])<0||
        !SetEnvironmentVariableA("PAPACC_OUTBOUND_TLS_CA_DER_B64",path))goto cleanup;
    {char value[16];
     sprintf_s(value,sizeof(value),"%u",(unsigned)tls_target_port);
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_HOST","papacc-test-server");
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_IPV4","127.0.0.1");
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_PORT",value);
     sprintf_s(value,sizeof(value),"%u",(unsigned)plain_target_port);
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_HOST","papacc-plain-test-server");
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_IPV4","127.0.0.1");
     SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_PORT",value);}
    if (sprintf_s(server_command, sizeof(server_command),
            "\"%s\" --port %u --all-interfaces --listener-profile "
            "secure-principal --security-configuration-ref production-test "
            "--allow-network-egress --egress-policy-ref nt4-lab-controlled "
            "--egress-profile-ref default-route --outbound-tls-profile-ref "
            "lab-private-ca --log-level off", argv[1], (unsigned int)port) < 0 ||
        !SetConsoleCtrlHandler(NULL, TRUE) ||
        !launch(server_command, CREATE_NEW_PROCESS_GROUP, &server))
        goto cleanup;
    Sleep(1500U);
    if (WaitForSingleObject(server.hProcess, 0U) != WAIT_TIMEOUT ||
        !run_client(argv[2], port, shutdown_name, ack_name,
            "production-control-data",0U) ||
        !run_client(argv[2],port,shutdown_name,ack_name,"network-http",
            tls_target_port) ||
        !run_client(argv[2],port,shutdown_name,ack_name,
            "network-tcp-plain-http-sequence",plain_target_port) ||
        !run_client(argv[2], port, shutdown_name, ack_name, "wrong-alpn",0U) ||
        !run_client(argv[2], port, shutdown_name, ack_name, "missing-alpn",0U) ||
        !run_client(argv[2], port, shutdown_name, ack_name,
            "missing-credential",0U) ||
        !run_client(argv[2], port, shutdown_name, ack_name,
            "untrusted-client",0U) ||
        !run_client(argv[2], port, shutdown_name, ack_name, "not-enrolled",0U) ||
        !run_client(argv[2], port, shutdown_name, ack_name, "tls12",0U))
        goto stop;
    if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, server.dwProcessId) ||
        WaitForSingleObject(server.hProcess, 15000U) != WAIT_OBJECT_0 ||
        !GetExitCodeProcess(server.hProcess, &exit_code) || exit_code != 0U)
        goto stop;
    puts("PAPACC_PRODUCTION_SECURE_PRINCIPAL_LISTENER=PASS");
    puts("PAPACC_PRODUCTION_SECURE_NEGATIVE_MATRIX=PASS");
    puts("PAPACC_PRODUCTION_SECURE_CLEAN_STOP=PASS");
    puts("CONTROLLED_REAL_SERVER_HTTP=PASS");
    puts("CONTROLLED_REAL_SERVER_HTTPS=PASS");
    puts("CONTROLLED_EXTERNAL_FIXTURE_SERVER_PROCESS_COUNT=0");
    result = 0;
    goto cleanup_server;
stop:
    GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, server.dwProcessId);
    if (WaitForSingleObject(server.hProcess, 5000U) != WAIT_OBJECT_0)
        TerminateProcess(server.hProcess, 98U);
cleanup_server:
    close_process(&server);
cleanup:
    if(target_stop_event!=NULL)SetEvent(target_stop_event);
    if(target_process.hProcess!=NULL){
        if(WaitForSingleObject(target_process.hProcess,15000U)!=WAIT_OBJECT_0)
            TerminateProcess(target_process.hProcess,97U);
        close_process(&target_process);}
    if(target_stop_event!=NULL)CloseHandle(target_stop_event);
    DeleteFileA(ready_path);
    SetConsoleCtrlHandler(NULL, FALSE);
    SetEnvironmentVariableA("PAPACC_SECURITY_CONFIGURATION_REF", NULL);
    SetEnvironmentVariableA("PAPACC_SERVER_CERTIFICATE_DER_B64", NULL);
    SetEnvironmentVariableA("PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64", NULL);
    SetEnvironmentVariableA("PAPACC_CLIENT_CA_DER_B64", NULL);
    SetEnvironmentVariableA("PAPACC_ALLOWED_CLIENT_CERT_SHA256", NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_POLICY_REF",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_PROFILE_REF",NULL);
    SetEnvironmentVariableA("PAPACC_OUTBOUND_TLS_PROFILE_REF",NULL);
    SetEnvironmentVariableA("PAPACC_OUTBOUND_TLS_CA_DER_B64",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_HOST",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_IPV4",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_0_PORT",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_HOST",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_IPV4",NULL);
    SetEnvironmentVariableA("PAPACC_EGRESS_RULE_1_PORT",NULL);
    if (ack_event != NULL) CloseHandle(ack_event);
    if (shutdown_event != NULL) CloseHandle(shutdown_event);
    WSACleanup();
    return result;
}
