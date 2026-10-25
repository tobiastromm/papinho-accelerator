/* SAME PS_ADMISSION owner. No entrypoint, registry, mutex or activation here. */
static int psa_bd_file(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow&&a->nFileSizeHigh==b->nFileSizeHigh&&a->nFileSizeLow==b->nFileSizeLow&&a->nNumberOfLinks==1&&!(a->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT));}
/* N2 opened Windows file: exact reviewed link count, not a global exception. */
static int psa_bd_aux_file(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return b->nNumberOfLinks&&a->nNumberOfLinks==b->nNumberOfLinks&&a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow&&a->nFileSizeHigh==b->nFileSizeHigh&&a->nFileSizeLow==b->nFileSizeLow&&!(a->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT));}
static DWORD psa_bd_no_impersonation(HANDLE thread)
{
 if(PSA.bd.unexpected)return ERROR_INVALID_STATE;
 if(OpenThreadToken(thread,TOKEN_QUERY,TRUE,&PSA.bd.unexpected))return ERROR_ACCESS_DENIED;
 return GetLastError()==ERROR_NO_TOKEN?0:psa_error();
}
/* Native N2 producer in SAME owner. Context check MUST validate protected
 * hashes/SD/approved OPENED-file Windows provenance and exact plan, including
 * auxiliary tuple. No raw flags or argv constitute that trusted adapter. */
static DWORD psa_bd_aux_current(const PS_BD_DESCRIPTOR *d,ULONGLONG now)
{
 FILETIME c,e,k,u;DWORD n=1024,session,used=0,flags,ids[32],count,i;WCHAR path[1024];BY_HANDLE_FILE_INFORMATION id;
 TOKEN_STATISTICS stats;union {TOKEN_USER aligned;BYTE bytes[256];} user;
 if(!PSA.bd.bound||!PSA.bd.aux_bound||PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId()||PSA.stop||
 d->challenge!=PSA.bd.binding.challenge||strcmp(d->plan,PSA.bd.binding.plan)||strcmp(d->policy,PS_BD_N0_POLICY)||
 !ps_bd_id(&d->auxiliary.instance,&PSA.bd.binding.auxiliary.instance)||d->auxiliary.file!=PSA.bd.file||
 !PSA.bd.check||!PSA.bd.auxiliary||!PSA.bd.file||!PSA.bd.aux_token||now<PSA.bd.inventory.console.observed||
 now-PSA.bd.inventory.console.observed>60000ULL)return ERROR_INVALID_STATE;
 if(GetProcessId(PSA.bd.auxiliary)!=d->auxiliary.instance.pid||WaitForSingleObject(PSA.bd.auxiliary,0)!=WAIT_TIMEOUT||
 !GetHandleInformation(PSA.bd.auxiliary,&flags)||(flags&HANDLE_FLAG_INHERIT)||
 !GetProcessTimes(PSA.bd.auxiliary,&c,&e,&k,&u)||(((ULONGLONG)c.dwHighDateTime<<32)|c.dwLowDateTime)!=d->auxiliary.instance.creation||
 !ProcessIdToSessionId(d->auxiliary.instance.pid,&session)||session!=d->actor.session)return ERROR_INVALID_DATA;
 if(!QueryFullProcessImageNameW(PSA.bd.auxiliary,0,path,&n)||_wcsicmp(path,L"C:\\Windows\\System32\\conhost.exe"))return ERROR_INVALID_DATA;
 if(!GetFileInformationByHandle(PSA.bd.file,&id)||!psa_bd_aux_file(&id,&d->auxiliary.file_id))return ERROR_INVALID_DATA;
 n=GetFinalPathNameByHandleW(PSA.bd.file,path,1024,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
 if(!n||n>=1024||_wcsicmp(path,L"\\\\?\\C:\\Windows\\System32\\conhost.exe"))return ERROR_INVALID_DATA;
 if(PSA.bd.aux_check_token)return ERROR_INVALID_STATE;
 if(!OpenProcessToken(PSA.bd.auxiliary,TOKEN_QUERY,&PSA.bd.aux_check_token))return psa_error();
 if(!GetTokenInformation(PSA.bd.aux_check_token,TokenUser,&user,sizeof(user),&used)||used<sizeof(TOKEN_USER)||used>sizeof(user)||
 (BYTE *)user.aligned.User.Sid<user.bytes||(BYTE *)user.aligned.User.Sid>user.bytes+used-8U||
 ((BYTE *)user.aligned.User.Sid)[1]>SID_MAX_SUB_AUTHORITIES||8U+4U*((BYTE *)user.aligned.User.Sid)[1]>used-(DWORD)((BYTE *)user.aligned.User.Sid-user.bytes)||
 !IsValidSid(user.aligned.User.Sid)||!EqualSid(user.aligned.User.Sid,(PSID)d->sid))return ERROR_INVALID_OWNER;
 if(!GetTokenInformation(PSA.bd.aux_check_token,TokenSessionId,&session,sizeof(session),&used)||used!=sizeof(session)||session!=d->actor.session||
 !GetTokenInformation(PSA.bd.aux_check_token,TokenStatistics,&stats,sizeof(stats),&used)||used!=sizeof(stats)||stats.TokenType!=TokenPrimary||
 memcmp(&stats.TokenId,&PSA.bd.aux_statistics.TokenId,sizeof(LUID))||memcmp(&stats.AuthenticationId,&PSA.bd.aux_statistics.AuthenticationId,sizeof(LUID))||
 memcmp(&stats.ModifiedId,&PSA.bd.aux_statistics.ModifiedId,sizeof(LUID)))return ERROR_ACCESS_DENIED;
 if(!CloseHandle(PSA.bd.aux_check_token)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.bd.aux_check_token=NULL;
 count=GetConsoleProcessList(ids,32);if(count!=2U)return ERROR_ACCESS_DENIED;
 for(i=0;i<count;i++)if(ids[i]!=d->actor.pid&&ids[i]!=d->owner_pid)return ERROR_ACCESS_DENIED;
 if(ids[0]==ids[1])return ERROR_INVALID_DATA;
 return PSA.bd.check(d); /* hashes, SD and provenance record remain mandatory */
}
static DWORD psa_bd_aux_bind(void)
{
 PS_BD_DESCRIPTOR *d=&PSA.bd.binding;PS_BD_PROOF *p=&PSA.bd.inventory.console;DWORD n;
 if(!d->n0)return 0;
 if(!DuplicateHandle(GetCurrentProcess(),d->auxiliary.instance.handle,GetCurrentProcess(),&PSA.bd.auxiliary,0,FALSE,DUPLICATE_SAME_ACCESS)||
 !DuplicateHandle(GetCurrentProcess(),d->auxiliary.file,GetCurrentProcess(),&PSA.bd.file,0,FALSE,DUPLICATE_SAME_ACCESS))return psa_error();
 d->auxiliary.instance.handle=PSA.bd.auxiliary;d->auxiliary.file=PSA.bd.file;
 if(!OpenProcessToken(PSA.bd.auxiliary,TOKEN_QUERY,&PSA.bd.aux_token)||
 !GetTokenInformation(PSA.bd.aux_token,TokenStatistics,&PSA.bd.aux_statistics,sizeof(PSA.bd.aux_statistics),&n)||n!=sizeof(PSA.bd.aux_statistics))return psa_error();
 p->actor=d->actor;p->console=d->auxiliary.instance;p->source=PS_BD_N0_SOURCE;p->known=PS_BD_N0_KNOWN;
 p->boot=d->boot;p->generation=d->challenge;p->observed=GetTickCount64();p->validate=psa_bd_aux_current;
 PSA.bd.aux_bound=1;return 0;
}
static DWORD psa_bd_current(void)
{
 const PS_BD_DESCRIPTOR *d=&PSA.bd.binding;FILETIME ct,et,kt,ut;DWORD n,session;WCHAR path[1024];TOKEN_STATISTICS s;TOKEN_ELEVATION elevation;
 union {TOKEN_USER aligned;BYTE bytes[256];} u;BY_HANDLE_FILE_INFORMATION id;DWORD e;
 if(PSA.stop||PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if((e=psa_bd_no_impersonation(GetCurrentThread()))||(e=psa_bd_no_impersonation(PSA.bd.thread)))return e;
 if(GetProcessId(PSA.launcher)!=d->actor.pid||GetProcessIdOfThread(PSA.bd.thread)!=d->actor.pid||GetThreadId(PSA.bd.thread)!=d->control_tid||
 WaitForSingleObject(PSA.launcher,0)!=WAIT_TIMEOUT||WaitForSingleObject(PSA.bd.thread,0)!=WAIT_TIMEOUT||
 !GetProcessTimes(PSA.launcher,&ct,&et,&kt,&ut)||(((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime)!=d->actor.creation||
 !ProcessIdToSessionId(d->actor.pid,&session)||session!=d->actor.session||!ProcessIdToSessionId(d->owner_pid,&session)||session!=d->actor.session)return ERROR_INVALID_DATA;
 n=1024;if(!QueryFullProcessImageNameW(PSA.launcher,0,path,&n)||_wcsicmp(path,L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"))return ERROR_INVALID_DATA;
 if(PSA.bd.actor_check_token)return ERROR_INVALID_STATE;
 if(!OpenProcessToken(PSA.launcher,TOKEN_QUERY,&PSA.bd.actor_check_token))return psa_error();
 ZeroMemory(&u,sizeof(u));n=0;
 if(!GetTokenInformation(PSA.bd.actor_check_token,TokenUser,&u,sizeof(u),&n)||n<sizeof(TOKEN_USER)||n>sizeof(u)||
 (BYTE *)u.aligned.User.Sid<u.bytes||(BYTE *)u.aligned.User.Sid>u.bytes+n-8U||
 ((BYTE *)u.aligned.User.Sid)[1]>SID_MAX_SUB_AUTHORITIES||8U+4U*((BYTE *)u.aligned.User.Sid)[1]>n-(DWORD)((BYTE *)u.aligned.User.Sid-u.bytes)||
 !IsValidSid(u.aligned.User.Sid)||GetLengthSid(u.aligned.User.Sid)>sizeof(u)-(size_t)((BYTE *)u.aligned.User.Sid-u.bytes)||!EqualSid(u.aligned.User.Sid,(PSID)d->sid))return ERROR_INVALID_OWNER;
 if(!GetTokenInformation(PSA.bd.actor_check_token,TokenElevation,&elevation,sizeof(elevation),&n)||!elevation.TokenIsElevated||
 !GetTokenInformation(PSA.bd.actor_check_token,TokenStatistics,&s,sizeof(s),&n)||s.TokenType!=TokenPrimary||
 memcmp(&s.TokenId,&d->statistics.TokenId,sizeof(LUID))||memcmp(&s.AuthenticationId,&d->statistics.AuthenticationId,sizeof(LUID))||memcmp(&s.ModifiedId,&d->statistics.ModifiedId,sizeof(LUID)))return ERROR_ACCESS_DENIED;
 if(!CloseHandle(PSA.bd.actor_check_token)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.bd.actor_check_token=NULL;
 if(!GetFileInformationByHandle(d->configuration,&id)||!psa_bd_file(&id,&d->config_id)||!GetFileInformationByHandle(d->image,&id)||!psa_bd_file(&id,&d->image_id))return ERROR_INVALID_DATA;
 if(d->n0){DWORD z=psa_bd_aux_current(d,GetTickCount64());if(z)return z;}
 return PSA.bd.check?PSA.bd.check(d):ERROR_NOT_SUPPORTED;
}
static DWORD psa_bd_confirm(void)
{
 const PS_BD_DESCRIPTOR *d=&PSA.bd.binding;WCHAR text[1024],reply[192],expected[192];DWORD n=0,w=0,im,om;int length;
 PSA.bd.input=CreateFileW(L"CONIN$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
 if(PSA.bd.input==INVALID_HANDLE_VALUE){PSA.bd.input=NULL;return psa_error();}
 PSA.bd.output=CreateFileW(L"CONOUT$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
 if(PSA.bd.output==INVALID_HANDLE_VALUE){PSA.bd.output=NULL;return psa_error();}
 if(!GetConsoleMode(PSA.bd.input,&im)||!GetConsoleMode(PSA.bd.output,&om)||(im&(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT))!=(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT))return ERROR_ACCESS_DENIED;
 if(d->n0){
  length=swprintf_s(text,1024,L"N0-N9: IDENTIFIED_AUXILIARY_CANDIDATE pid=%lu creation=%llu session=%lu file=%lu:%lu:%lu links=%lu\r\nOpenedFileHash=%hs\r\nSecurityHash=%hs\r\nProvenanceRecord=%hs\r\nCONSOLE_ASSOCIATION=UNPROVEN; MAPPED_IMAGE_PROVENANCE=UNPROVEN; N7_SINGLE_LAB_ONLY; NO_START_AUTHORITY\r\n",(unsigned long)d->auxiliary.instance.pid,d->auxiliary.instance.creation,(unsigned long)d->auxiliary.instance.session,(unsigned long)d->auxiliary.file_id.dwVolumeSerialNumber,(unsigned long)d->auxiliary.file_id.nFileIndexHigh,(unsigned long)d->auxiliary.file_id.nFileIndexLow,(unsigned long)d->auxiliary.file_id.nNumberOfLinks,d->auxiliary.file_hash,d->auxiliary.security_hash,d->auxiliary.provenance_hash);
  if(length<0||!WriteConsoleW(PSA.bd.output,text,(DWORD)length,&w,NULL)||w!=(DWORD)length)return ERROR_WRITE_FAULT;
 }
 if(swprintf_s(expected,192,L"BIND_BD_READ_ONLY %hs %lu %llu %llu",d->attempt,(unsigned long)d->owner_pid,d->owner_creation,d->challenge)<0)return ERROR_INVALID_DATA;
 length=swprintf_s(text,1024,L"B+D owner binding ONLY; no operational authority. Actor=%lu creation=%llu; boot=%llu; attempt=%hs\r\nPlan=%hs\r\nConfiguration=%hs\r\nBinary=%hs\r\nPolicy=%hs\r\nHost=%hs\r\nAuxiliary is NOT authenticated console infrastructure. Type exactly:\r\n%ls\r\n> ",(unsigned long)d->actor.pid,d->actor.creation,d->boot,d->attempt,d->plan,d->config_hash,d->image_hash,d->policy,d->host,expected);
 if(length<0||!WriteConsoleW(PSA.bd.output,text,(DWORD)length,&w,NULL)||w!=(DWORD)length||!ReadConsoleW(PSA.bd.input,reply,191,&n,NULL)||n>=192)return ERROR_ACCESS_DENIED;
 reply[n]=0;while(n&&(reply[n-1]=='\r'||reply[n-1]=='\n'))reply[--n]=0;
 return wcscmp(reply,expected)?ERROR_CANCELLED:0;
}
DWORD papacc_server_service_admission_bd_bind(const PS_BD_DESCRIPTOR *d,PS_BD_CONTEXT_CHECK check)
{
 FILETIME ct,et,kt,ut;DWORD e;
 if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId()||PSA.stop||PSA.consumed||PSA.bd.used||PSA.launcher||PSA.token)return ERROR_INVALID_STATE;
 PSA.bd.used=1; /* No confirmation replay/rebinding, including failed binding. */
 if((e=ps_bd_descriptor(d)))return e;if(!check)return ERROR_NOT_SUPPORTED;
 if(d->owner_pid!=GetCurrentProcessId()||!GetProcessTimes(GetCurrentProcess(),&ct,&et,&kt,&ut)||(((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime)!=d->owner_creation)return ERROR_INVALID_DATA;
 PSA.bd.binding=*d;PSA.bd.check=check;
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&PSA.bd.approved_sid))return psa_error();
 if(!EqualSid(PSA.bd.approved_sid,(PSID)d->sid))return ERROR_INVALID_OWNER;
 if(!DuplicateHandle(GetCurrentProcess(),d->actor.handle,GetCurrentProcess(),&PSA.launcher,0,FALSE,DUPLICATE_SAME_ACCESS)||
 !DuplicateHandle(GetCurrentProcess(),d->control_thread,GetCurrentProcess(),&PSA.bd.thread,0,FALSE,DUPLICATE_SAME_ACCESS))return psa_error();
 PSA.bd.binding.actor.handle=PSA.launcher;PSA.bd.binding.control_thread=PSA.bd.thread;
 if(!DuplicateHandle(GetCurrentProcess(),GetCurrentProcess(),GetCurrentProcess(),&PSA.bd.self,PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,0)||!OpenProcessToken(PSA.launcher,TOKEN_QUERY,&PSA.token))return psa_error();
 /* Stage native references before confirmation; never grant authority here. */
 PSA.bd.bound=1;
 if((e=psa_bd_aux_bind())||(e=psa_bd_current())||(e=psa_bd_confirm())){PSA.bd.bound=0;return e;}
 if(!CloseHandle(PSA.bd.input)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.bd.input=NULL;
 if(!CloseHandle(PSA.bd.output)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.bd.output=NULL;
 if((e=psa_bd_current())){PSA.bd.bound=0;return e;}
 PSA.bd.bound=1;PSA.bd.inventory.count=2;
 PSA.bd.inventory.present[PS_BD_ADMIN]=PSA.bd.inventory.present[PS_BD_QUALIFIER]=1;
 PSA.bd.inventory.role[PS_BD_ADMIN]=PSA.bd.binding.actor;
 PSA.bd.inventory.role[PS_BD_QUALIFIER].handle=PSA.bd.self;PSA.bd.inventory.role[PS_BD_QUALIFIER].pid=d->owner_pid;
 PSA.bd.inventory.role[PS_BD_QUALIFIER].creation=d->owner_creation;PSA.bd.inventory.role[PS_BD_QUALIFIER].session=d->actor.session;
 if(d->n0){PSA.bd.inventory.present[PS_BD_CONSOLE]=1;PSA.bd.inventory.role[PS_BD_CONSOLE]=PSA.bd.binding.auxiliary.instance;PSA.bd.inventory.count++;}
 /* No fabricated service PID; original auxiliary refs owned until checked close. */
 return 0;
}
DWORD papacc_server_service_admission_bd_revalidate(PS_BD_CONTEXT_CHECK check)
{if(!PSA.bd.bound||!check||check!=PSA.bd.check)return ERROR_INVALID_STATE;return psa_bd_current();}
DWORD papacc_server_service_admission_bd_close(int quiescent)
{
 HANDLE *refs[12];unsigned i;if(!PSA.bd.used)return 0;
 if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(!quiescent){PSA.phase=3;return ERROR_INVALID_STATE;}
 refs[0]=&PSA.bd.input;refs[1]=&PSA.bd.output;refs[2]=&PSA.bd.unexpected;refs[3]=&PSA.token;refs[4]=&PSA.bd.thread;refs[5]=&PSA.launcher;refs[6]=&PSA.bd.aux_token;refs[7]=&PSA.bd.file;refs[8]=&PSA.bd.auxiliary;refs[9]=&PSA.bd.aux_check_token;refs[10]=&PSA.bd.actor_check_token;refs[11]=&PSA.bd.self;
 for(i=0;i<12;i++)if(*refs[i]){if(!CloseHandle(*refs[i])){PSA.phase=3;return ERROR_INVALID_HANDLE;}*refs[i]=NULL;}
 if(PSA.bd.approved_sid){if(LocalFree(PSA.bd.approved_sid)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.bd.approved_sid=NULL;}
 PSA.bd.bound=0;PSA.bd.aux_bound=0;PSA.bd.metrics_valid=0;PSA.bd.binding.auxiliary.instance.handle=NULL;PSA.bd.binding.auxiliary.file=NULL;PSA.bd.check=NULL;PSA.bd.binding.actor.handle=NULL;PSA.bd.binding.control_thread=NULL;PSA.bd.binding.configuration=PSA.bd.binding.image=NULL;ZeroMemory(&PSA.bd.inventory,sizeof(PSA.bd.inventory));return 0;
}


DWORD papacc_server_service_admission_bd_metrics(const PS_BD_COST *cost,ULONGLONG observed)
{
 DWORD e;ULONGLONG now=GetTickCount64();PSA.bd.metrics_valid=0;if(!cost||!PSA.bd.bound||!PSA.bd.binding.n0||observed>now||now-observed>60000ULL)return ERROR_INVALID_STATE;
 if((e=psa_bd_current())||(e=ps_bd_inventory(&PSA.bd.binding,&PSA.bd.inventory,now))||(e=ps_bd_costs(&PSA.bd.inventory,cost)))return e;
 memcpy(PSA.bd.costs,cost,sizeof(PSA.bd.costs));PSA.bd.metrics_at=observed;PSA.bd.metrics_valid=1;return 0;
}
