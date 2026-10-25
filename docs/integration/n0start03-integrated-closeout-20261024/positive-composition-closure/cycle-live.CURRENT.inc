static DWORD cycle_live(DWORD pid)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 union {TOKEN_USER align;BYTE b[256];}u;
 union {TOKEN_GROUPS align;BYTE b[8192];}g;
 union {TOKEN_PRIVILEGES align;BYTE b[4096];}p;
 TOKEN_STATISTICS stats;DWORD n,i,e=0,handles,threads=0;LPWSTR sid=NULL;THREADENTRY32 te;PROCESS_MEMORY_COUNTERS_EX mem;WCHAR image[1024];DWORD len=1024;TOKEN_ELEVATION elev;
 if(!cycle_process)cycle_process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,pid);if(!cycle_process)return lab_error();
 if(!QueryFullProcessImageNameW(cycle_process,0,image,&len)||_wcsicmp(image,cycle_path)||!OpenProcessToken(cycle_process,TOKEN_QUERY|TOKEN_DUPLICATE,&cycle_token))return ERROR_ACCESS_DENIED;
 {MODULEENTRY32W me;unsigned seen=0;WCHAR expected[1024];
 cycle_threads=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);if(cycle_threads==INVALID_HANDLE_VALUE){cycle_threads=NULL;return lab_error();}
 ZeroMemory(&me,sizeof(me));me.dwSize=sizeof(me);if(!Module32FirstW(cycle_threads,&me))return lab_error();
 do{
 printf("LAB_MODULE=%ls; PATH=%ls\n",me.szModule,me.szExePath);
 if(!_wcsicmp(me.szModule,L"libcrypto-3-x64.dll")||!_wcsicmp(me.szModule,L"libssl-3-x64.dll")){
 expected[0]=0;if(!lab_append(expected,1024,lab_parent_path)||!lab_append(expected,1024,L"\\")||!lab_append(expected,1024,me.szModule)||_wcsicmp(expected,me.szExePath))return ERROR_INVALID_NAME;
 seen|=!_wcsicmp(me.szModule,L"libcrypto-3-x64.dll")?1U:2U;
 }
 }while(Module32NextW(cycle_threads,&me));if(GetLastError()!=ERROR_NO_MORE_FILES||seen!=3)return ERROR_INVALID_DATA;
 if(!CloseHandle(cycle_threads)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_threads=NULL;
 puts("LAB_IMPORTED_DLL_PATHS=MATCH_PROTECTED_INSTALLED_FILES; MAPPED_PAGE_ATTESTATION=NOT_CLAIMED");
 }

 if(!GetTokenInformation(cycle_token,TokenStatistics,&stats,sizeof(stats),&n)||stats.TokenType!=TokenPrimary||
 !GetTokenInformation(cycle_token,TokenUser,&u,sizeof(u),&n)||!cycle_sid_inside(u.b,n,u.align.User.Sid)||!EqualSid(u.align.User.Sid,native.service_sid)||
 !ConvertSidToStringSidW(u.align.User.Sid,&sid))return ERROR_ACCESS_DENIED;
 printf("LAB_TOKEN_USER=%ls; PRIMARY=1; PID=%lu\n",sid,(unsigned long)pid);if(LocalFree(sid)){cycle_sid_retained=sid;native.retained=1;return ERROR_INVALID_HANDLE;}sid=NULL;
 if(!GetTokenInformation(cycle_token,TokenGroups,&g,sizeof(g),&n)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||g.align.GroupCount>(n-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return ERROR_INVALID_DATA;
 for(i=0;i<g.align.GroupCount;i++){
 if(!cycle_sid_inside(g.b,n,g.align.Groups[i].Sid)||!ConvertSidToStringSidW(g.align.Groups[i].Sid,&sid))return ERROR_INVALID_SID;
 printf("LAB_TOKEN_GROUP=%ls; ATTR=%08lx\n",sid,(unsigned long)g.align.Groups[i].Attributes);if(LocalFree(sid)){cycle_sid_retained=sid;native.retained=1;return ERROR_INVALID_HANDLE;}sid=NULL;}
 if(!GetTokenInformation(cycle_token,TokenPrivileges,&p,sizeof(p),&n)||n<(DWORD)FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges)||p.align.PrivilegeCount>(n-FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges))/sizeof(LUID_AND_ATTRIBUTES))return ERROR_INVALID_DATA;
 for(i=0;i<p.align.PrivilegeCount;i++)printf("LAB_TOKEN_PRIVILEGE=%ld:%lu; ATTR=%08lx\n",(long)p.align.Privileges[i].Luid.HighPart,(unsigned long)p.align.Privileges[i].Luid.LowPart,(unsigned long)p.align.Privileges[i].Attributes);
 if(!GetTokenInformation(cycle_token,TokenElevation,&elev,sizeof(elev),&n))return lab_error();printf("LAB_TOKEN_ELEVATED=%lu; TOKEN_ID=%ld:%lu; AUTH_ID=%ld:%lu; MODIFIED=%ld:%lu\n",(unsigned long)elev.TokenIsElevated,(long)stats.TokenId.HighPart,(unsigned long)stats.TokenId.LowPart,(long)stats.AuthenticationId.HighPart,(unsigned long)stats.AuthenticationId.LowPart,(long)stats.ModifiedId.HighPart,(unsigned long)stats.ModifiedId.LowPart);
 if(!DuplicateToken(cycle_token,SecurityImpersonation,&cycle_access))return lab_error();
 if((e=cycle_access_file(lab_parent,0))||(e=cycle_access_file(lab_config,1))||(e=cycle_access_file(cycle_file,2))||(e=cycle_access_file(cycle_dependencies[0],2))||(e=cycle_access_file(cycle_dependencies[1],2))||(e=cycle_no_endpoints(pid)))return e;
 cycle_threads=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);if(cycle_threads==INVALID_HANDLE_VALUE){cycle_threads=NULL;return lab_error();}
 ZeroMemory(&te,sizeof(te));te.dwSize=sizeof(te);if(!Thread32First(cycle_threads,&te))return lab_error();
 do{if(te.th32OwnerProcessID==pid){A1_LEASE token;DWORD token_error;
 cycle_thread=OpenThread(THREAD_QUERY_INFORMATION,FALSE,te.th32ThreadID);if(!cycle_thread)return lab_error();
 token_error=lab_aux_token(&token,cycle_thread,1,TOKEN_QUERY);if(!token_error)return lab_aux_close_preserving(token,ERROR_ACCESS_DENIED);
 if(token_error!=ERROR_NO_TOKEN)return token_error;threads++;if(!CloseHandle(cycle_thread))return ERROR_INVALID_HANDLE;cycle_thread=NULL;}
 }while(Thread32Next(cycle_threads,&te));if(GetLastError()!=ERROR_NO_MORE_FILES)return lab_error();
 ZeroMemory(&mem,sizeof(mem));mem.cb=sizeof(mem);
 if(!GetProcessHandleCount(cycle_process,&handles)||!GetProcessMemoryInfo(cycle_process,(PROCESS_MEMORY_COUNTERS *)&mem,sizeof(mem)))return lab_error();
 printf("LAB_PROCESS_OBSERVATION threads=%lu handles=%lu working_set=%llu private_bytes=%llu peak_working_set=%llu; IMPERSONATION=NONE_OBSERVED_SNAPSHOT; STACK_KERNEL_PROVIDER=UNKNOWN; ED11=PARTIAL\n",(unsigned long)threads,(unsigned long)handles,(unsigned long long)mem.WorkingSetSize,(unsigned long long)mem.PrivateUsage,(unsigned long long)mem.PeakWorkingSetSize);
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 /* All successful last uses above have completed; process/image remain owned. */
 if(ig_n0_positive){
  if((e=cycle_live_close_last(&cycle_threads))||(e=cycle_live_close_last(&cycle_access))||(e=cycle_live_close_last(&cycle_token)))return e;
 }
#endif
 return ERROR_SUCCESS;
}
