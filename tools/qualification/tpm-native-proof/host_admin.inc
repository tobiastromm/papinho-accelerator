/* C0-C9 accepted standalone profile; no TPM or product authority here. */
typedef LONG (NTAPI *HP_NATIVE_QUERY)(HANDLE,ULONG,PVOID,ULONG,PULONG);
typedef struct HP_ROLE_FILE_PIN {const wchar_t *relative;DWORD volume,high,low,bytes;const char *sha;} HP_ROLE_FILE_PIN;
static const HP_ROLE_FILE_PIN hp_role_pins[3]={
 {L"WindowsPowerShell\\v1.0\\powershell.exe",417614100UL,327680UL,882077UL,455680UL,"9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3"},
 {L"conhost.exe",417614100UL,327680UL,1591236UL,867840UL,"b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963"},
 {L"ntdll.dll",417614100UL,65536UL,1875457UL,2029496UL,"36b1a62e4bf6bf7d68458fff6e8a33b39f6527ec6a46e55ecdd12cde83894c8d"},
};
static int hp_forward(TP_PLATFORM *p,HP_NATIVE_QUERY query,HANDLE process,ULONGLONG *raw,DWORD *pid){ULONG used=0;LONG status;
 if(p->proof.frozen)return 0;status=query(process,49,raw,sizeof(*raw),&used);
 if(p->proof.frozen)return 0;if(status||used!=8)return tp_fail(&p->proof,"CONSOLE_NATIVE_QUERY",(DWORD)status);
 if((*raw&3ULL)!=1||!(*raw&~3ULL)||(*raw&~3ULL)>MAXDWORD)return tp_fail(&p->proof,"CONSOLE_NATIVE_PROFILE",0);
 *pid=(DWORD)(*raw&~3ULL);return 1;
}
static int hp_role_file(TP_PLATFORM *p,HANDLE process,unsigned role){const HP_ROLE_FILE_PIN *pin=&hp_role_pins[role];TP_OWNER *o=&p->proof;wchar_t expected[1024],actual[1024];DWORD n,used=0,got=0,total=0;BY_HANDLE_FILE_INFORMATION id,after;LARGE_INTEGER bytes;unsigned char block[4096],digest[32];char hex[65];NTSTATUS status;int ok=0;const char *previous=o->stage;
 if(o->frozen||p->role_image||o->work_hash||p->token)return 0;o->stage=role==0?"ROLE_PARENT_FILE":role==1?"ROLE_AUXILIARY_FILE":"ROLE_NATIVE_ABI_FILE";
 n=GetSystemDirectoryW(expected,1024);if(!n||n>=1024||wcscat_s(expected,1024,L"\\")||wcscat_s(expected,1024,pin->relative))return tp_fail(o,"ROLE_SYSTEM_PATH",0);
 if(process){n=1024;if(!QueryFullProcessImageNameW(process,0,actual,&n))return host_fail(p,"ROLE_PROCESS_IMAGE_QUERY");if(_wcsicmp(actual,expected))return tp_fail(o,"ROLE_PROCESS_IMAGE_PATH",0);}
 p->role_image=CreateFileW(expected,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(p->role_image==INVALID_HANDLE_VALUE){p->role_image=NULL;return host_fail(p,"ROLE_IMAGE_OPEN");}o->controlled_refs++;
 if(!GetFileInformationByHandle(p->role_image,&id)||!GetFileSizeEx(p->role_image,&bytes)){host_fail(p,"ROLE_IMAGE_INFO");goto done;}
 if(GetFileType(p->role_image)!=FILE_TYPE_DISK||(id.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||id.dwVolumeSerialNumber!=pin->volume||id.nFileIndexHigh!=pin->high||id.nFileIndexLow!=pin->low||bytes.QuadPart!=pin->bytes){tp_fail(o,"ROLE_IMAGE_NATIVE_ID",0);goto done;}
 if(!GetKernelObjectSecurity(p->role_image,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,p->token_buffer,sizeof(p->token_buffer),&used)){host_fail(p,"ROLE_IMAGE_SECURITY_QUERY");goto done;}
 if(!used||used>sizeof(p->token_buffer)||!tp_hash(o,p->token_buffer,used,digest))goto done;hp_hex(digest,32,hex);
 if(strcmp(hex,"7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0")){tp_fail(o,"ROLE_IMAGE_DESCRIPTOR",0);goto done;}
 status=BCryptCreateHash(o->sha,&o->work_hash,o->hash_object,o->sha_object,NULL,0,0);if(o->work_hash)o->controlled_refs++;
 if(status!=0||!o->work_hash){if(o->work_hash)o->retained=1;tp_fail(o,"ROLE_HASH_CREATE",(DWORD)status);goto done;}
 if(!tp_sample(o,"ROLE_FILE_HASH_HELD"))goto hash_done;
 while(total<pin->bytes){DWORD want=pin->bytes-total;if(want>sizeof(block))want=sizeof(block);
 if(InterlockedCompareExchange(&o->stop,0,0)){tp_fail(o,"OWNER_STOP",0);goto hash_done;}
 if(!ReadFile(p->role_image,block,want,&got,NULL)){host_fail(p,"ROLE_IMAGE_READ");goto hash_done;}if(got!=want){tp_fail(o,"ROLE_IMAGE_SHORT_READ",0);goto hash_done;}
 status=BCryptHashData(o->work_hash,block,got,0);if(status!=0){tp_fail(o,"ROLE_HASH_DATA",(DWORD)status);goto hash_done;}total+=got;}
 status=BCryptFinishHash(o->work_hash,digest,32,0);if(status!=0){tp_fail(o,"ROLE_HASH_FINISH",(DWORD)status);goto hash_done;}hp_hex(digest,32,hex);
 if(strcmp(hex,pin->sha)){tp_fail(o,"ROLE_IMAGE_HASH",0);goto hash_done;}
 if(!GetFileInformationByHandle(p->role_image,&after)){host_fail(p,"ROLE_IMAGE_RECHECK");goto hash_done;}
 if(after.dwVolumeSerialNumber!=id.dwVolumeSerialNumber||after.nFileIndexHigh!=id.nFileIndexHigh||after.nFileIndexLow!=id.nFileIndexLow||after.nFileSizeHigh!=id.nFileSizeHigh||after.nFileSizeLow!=id.nFileSizeLow){tp_fail(o,"ROLE_IMAGE_CHANGED",0);goto hash_done;}ok=1;
 hash_done:status=BCryptDestroyHash(o->work_hash);if(status!=0){o->retained=1;return tp_fail(o,"ROLE_HASH_CLOSE_UNKNOWN",(DWORD)status);}o->work_hash=NULL;o->controlled_refs--;SecureZeroMemory(o->hash_object,sizeof(o->hash_object));
 done:if(o->retained)return 0;if(!close_handle(p,&p->role_image))return 0;if(ok&&!o->frozen){p->role_file_verified_mask|=1U<<role;o->stage=previous;}return ok&&!o->frozen;
}
static int hp_role_token(TP_PLATFORM *p,HANDLE process,unsigned role){TP_OWNER *o=&p->proof;TOKEN_STATISTICS a,b;TOKEN_ELEVATION elevation;TOKEN_TYPE type;TOKEN_USER *user=(TOKEN_USER *)p->token_buffer;DWORD used=0,session=0,self_session=0;unsigned char ids[24];int ok=0;const char *previous=o->stage;
 if(o->frozen||p->token||role>1)return 0;o->stage=role?"ROLE_AUXILIARY_TOKEN":"ROLE_PARENT_TOKEN";
 if(!OpenProcessToken(process,TOKEN_QUERY,&p->token)){if(p->token){o->controlled_refs++;o->retained=1;}return host_fail(p,"ROLE_TOKEN_OPEN");}o->controlled_refs++;
 if(!tp_sample(o,"ROLE_TOKEN_HELD"))goto done;
 if(!GetTokenInformation(p->token,TokenStatistics,&a,sizeof(a),&used)){host_fail(p,"ROLE_TOKEN_STATS");goto done;}if(used!=sizeof(a)){tp_fail(o,"ROLE_TOKEN_STATS_SIZE",0);goto done;}
 if(!GetTokenInformation(p->token,TokenUser,p->token_buffer,sizeof(p->token_buffer),&used)){host_fail(p,"ROLE_TOKEN_USER_QUERY");goto done;}
 if(used<sizeof(TOKEN_USER)||used>sizeof(p->token_buffer)||(ULONG_PTR)user->User.Sid<(ULONG_PTR)p->token_buffer||(ULONG_PTR)user->User.Sid>(ULONG_PTR)p->token_buffer+used-8||!IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>used-(DWORD)((ULONG_PTR)user->User.Sid-(ULONG_PTR)p->token_buffer)||!EqualSid(user->User.Sid,(PSID)operator_sid)){tp_fail(o,"ROLE_TOKEN_USER",0);goto done;}
 if(!GetTokenInformation(p->token,TokenElevation,&elevation,sizeof(elevation),&used)){host_fail(p,"ROLE_TOKEN_ELEVATION_QUERY");goto done;}if(used!=sizeof(elevation)||!elevation.TokenIsElevated){tp_fail(o,"ROLE_TOKEN_NOT_ELEVATED",0);goto done;}
 if(!GetTokenInformation(p->token,TokenType,&type,sizeof(type),&used)){host_fail(p,"ROLE_TOKEN_TYPE_QUERY");goto done;}if(used!=sizeof(type)||type!=TokenPrimary){tp_fail(o,"ROLE_TOKEN_TYPE",0);goto done;}
 if(!GetTokenInformation(p->token,TokenSessionId,&session,sizeof(session),&used)||!ProcessIdToSessionId(GetCurrentProcessId(),&self_session)){host_fail(p,"ROLE_TOKEN_SESSION_QUERY");goto done;}if(used!=sizeof(session)||session!=self_session){tp_fail(o,"ROLE_TOKEN_SESSION",0);goto done;}
 if(!GetTokenInformation(p->token,TokenStatistics,&b,sizeof(b),&used)){host_fail(p,"ROLE_TOKEN_STATS");goto done;}
 if(used!=sizeof(b)||memcmp(&a.TokenId,&b.TokenId,8)||memcmp(&a.AuthenticationId,&b.AuthenticationId,8)||memcmp(&a.ModifiedId,&b.ModifiedId,8)){tp_fail(o,"ROLE_TOKEN_CHANGED",0);goto done;}
 memcpy(ids,&b.TokenId,8);memcpy(ids+8,&b.AuthenticationId,8);memcpy(ids+16,&b.ModifiedId,8);
 if(!p->token_bound||!tp_eq(ids+8,p->token_ids+8,8)){tp_fail(o,"ROLE_LOGON_MISMATCH",0);goto done;}
 if(p->role_token_bound[role]&&!tp_eq(ids,p->role_token_ids[role],24)){tp_fail(o,"ROLE_TOKEN_CONTINUITY",0);goto done;}
 if(!p->role_token_bound[role]){memcpy(p->role_token_ids[role],ids,24);p->role_token_bound[role]=1;}ok=1;
 done:if(!close_handle(p,&p->token))ok=0;if(ok&&!o->frozen)o->stage=previous;return ok&&!o->frozen;
}
static int hp_admin_evidence(TP_PLATFORM *p,HANDLE process,unsigned role){return hp_role_token(p,process,role)&&hp_role_file(p,process,role);}
#ifndef HP_ADMIN_EVIDENCE
#define HP_ADMIN_EVIDENCE hp_admin_evidence
#endif
#ifndef HP_ABI_EVIDENCE
#define HP_ABI_EVIDENCE(p) hp_role_file((p),NULL,2)
#endif
static int hp_alive(TP_PLATFORM *p,HANDLE h,DWORD pid,FILETIME *creation){FILETIME e,k,u;
 if(p->proof.frozen)return 0;if(GetProcessId(h)!=pid)return tp_fail(&p->proof,"ROLE_PROCESS_ID",0);
 if(WaitForSingleObject(h,0)!=WAIT_TIMEOUT)return tp_fail(&p->proof,"ROLE_PROCESS_NOT_LIVE",0);
 if(!GetProcessTimes(h,creation,&e,&k,&u))return host_fail(p,"ROLE_CREATION_QUERY");
 return ft(creation)?1:tp_fail(&p->proof,"ROLE_CREATION_UNKNOWN",0);
}
static int hp_admin_current(TP_PLATFORM *p){TP_OWNER *o=&p->proof;unsigned char basic[48];ULONG used=0;ULONGLONG parent64,raw_self=0,raw_parent=0,after_self=0,after_parent=0;DWORD parent,console=0,other=0,count,i,self=GetCurrentProcessId();BOOL seenSelf=FALSE,seenParent=FALSE;FILETIME pc,cc,after_pc,after_cc;TP_ADMIN_SAMPLE *s;int ok=0;HP_NATIVE_QUERY query;
 if(o->frozen)return 0;if(InterlockedCompareExchange(&o->stop,0,0))return tp_fail(o,"OWNER_STOP",0);
 p->role_file_verified_mask=0;if(p->parent||p->console||p->role_image||p->token)return tp_fail(o,"ADMIN_REFERENCES_BUSY",0);
 p->observer_attempted=1;p->observer_known=GetProcessHandleCount(GetCurrentProcess(),&p->observer_count)!=0;p->observer_error=p->observer_known?0:GetLastError();
 if(!p->observer_known)return tp_fail(o,"NATIVE_OBSERVER_QUERY_FAILED",p->observer_error);if(p->observer_count>TP_OBSERVER_LIMIT)return tp_fail(o,"NATIVE_OBSERVER_HEADROOM",0);
 query=(HP_NATIVE_QUERY)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryInformationProcess");if(!query||query(GetCurrentProcess(),0,basic,sizeof(basic),&used)!=0||used!=sizeof(basic))return tp_fail(o,"PARENT_QUERY_UNKNOWN",0);
 memcpy(&parent64,basic+40,8);if(!parent64||parent64>MAXDWORD||parent64==self)return tp_fail(o,"PARENT_QUERY_UNKNOWN",0);parent=(DWORD)parent64;
 p->window_owner=0;if(GetConsoleWindow())GetWindowThreadProcessId(GetConsoleWindow(),&p->window_owner);
 count=GetConsoleProcessList(p->topology_ids,8);p->topology_parent=parent;p->topology_count=count;
 for(i=0;count<=8&&i<count;i++){if(p->topology_ids[i]==self)seenSelf=TRUE;if(p->topology_ids[i]==parent)seenParent=TRUE;}
 if(count!=2||!seenSelf||!seenParent)return tp_fail(o,"PARTICIPANT_TOPOLOGY_DENY",0);
 if(!HP_ABI_EVIDENCE(p)||!hp_forward(p,query,GetCurrentProcess(),&raw_self,&console))return 0;
 p->topology_console=console;p->self_console_raw=raw_self;
 if(console==self||console==parent)return tp_fail(o,"AUXILIARY_NOT_DISTINCT",0);
 if(p->parent_pid&&(p->parent_pid!=parent||p->console_pid!=console))return tp_fail(o,"ADMIN_PID_CHANGED",0);
 p->parent=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,parent);if(!p->parent)return host_fail(p,"PARENT_OPEN_QUERY");o->controlled_refs++;
 if(!hp_alive(p,p->parent,parent,&pc)||!hp_forward(p,query,p->parent,&raw_parent,&other))goto done;
 p->console_raw=raw_parent;if(raw_parent!=raw_self||other!=console){tp_fail(o,"ADMIN_CONSOLE_MISMATCH",0);goto done;}
 p->console=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,console);if(!p->console){host_fail(p,"CONSOLE_OPEN_QUERY");goto done;}o->controlled_refs++;
 if(!hp_alive(p,p->console,console,&cc))goto done;
 if(p->parent_creation&&(p->parent_creation!=ft(&pc)||p->console_creation!=ft(&cc))){tp_fail(o,"ADMIN_PID_REUSE",0);goto done;}
 if(p->creation&&ft(&pc)>p->creation){tp_fail(o,"PARENT_CREATION_ORDER",0);goto done;}
 if(!HP_ADMIN_EVIDENCE(p,p->parent,0)||!HP_ADMIN_EVIDENCE(p,p->console,1))goto done;
 if(p->admin_count==TP_ADMIN_SAMPLES){tp_fail(o,"ADMIN_SAMPLE_CAP",0);goto done;}s=&p->admins[p->admin_count];memset(s,0,sizeof(*s));
 if(!GetProcessHandleCount(p->parent,&s->parent_handles)||!GetProcessHandleCount(p->console,&s->console_handles)){host_fail(p,"ADMIN_METRIC_UNKNOWN");goto done;}
 if(!s->parent_handles||!s->console_handles){tp_fail(o,"ADMIN_METRIC_UNKNOWN",0);goto done;}
 if((ULONGLONG)s->parent_handles+s->console_handles>TP_ADMIN_LIMIT){tp_fail(o,"ADMIN_BUDGET_DENY",0);goto done;}
 if(!hp_alive(p,p->parent,parent,&after_pc)||!hp_alive(p,p->console,console,&after_cc)||ft(&pc)!=ft(&after_pc)||ft(&cc)!=ft(&after_cc)){if(!o->frozen)tp_fail(o,"ADMIN_CREATION_CHANGED",0);goto done;}
 if(!hp_forward(p,query,GetCurrentProcess(),&after_self,&other)||!hp_forward(p,query,p->parent,&after_parent,&other))goto done;
 if(raw_self!=after_self||raw_parent!=after_parent){tp_fail(o,"ADMIN_CONSOLE_CHANGED",0);goto done;}
 count=GetConsoleProcessList(p->topology_ids,8);p->topology_count=count;if(count!=2){tp_fail(o,"ADMIN_CLIENTS_CHANGED",0);goto done;}seenSelf=seenParent=FALSE;for(i=0;i<2;i++){if(p->topology_ids[i]==self)seenSelf=TRUE;if(p->topology_ids[i]==parent)seenParent=TRUE;}
 if(!seenSelf||!seenParent){tp_fail(o,"ADMIN_CLIENTS_CHANGED",0);goto done;}
 p->parent_pid=parent;p->console_pid=console;p->parent_creation=ft(&pc);p->console_creation=ft(&cc);p->console_raw=raw_parent;
 s->parent_pid=parent;s->console_pid=console;s->parent_creation=ft(&pc);s->console_creation=ft(&cc);s->tick=GetTickCount64();p->admin_count++;
 ok=tp_sample(o,"OBSERVER_REFERENCES_HELD");
 done:if(o->retained)return 0;if(!close_handle(p,&p->console)||!close_handle(p,&p->parent)){p->observer_close_error=GetLastError();return 0;}return ok&&!o->frozen;
}

int hp_admin(TP_PLATFORM *p){int result;
 if(p->proof.owner_thread!=GetCurrentThreadId())return 0;
 if(p->proof.frozen)return 0;
 if(InterlockedCompareExchange(&p->admin_busy,1,0))return tp_fail(&p->proof,"ADMIN_REENTRANT",0);
 result=hp_admin_current(p);InterlockedExchange(&p->admin_busy,0);return result;
}
