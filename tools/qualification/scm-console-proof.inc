/* Read-only rejected-process evidence, same qualifier owner and PSS lifecycle. */
#include <wintrust.h>
#include <softpub.h>
#include "scm-console-proof-model.h"
typedef struct IG_CONSOLE_PROOF {
 HANDLE process,image;FILETIME creation;DWORD pid,parent,session,elevated_session;
 HWND window;DWORD window_owner;DWORD members[32];WCHAR path[1024],after_path[1024];
 BY_HANDLE_FILE_INFORMATION file;IG_PROCESS metrics;IG_CP_FACTS facts;
 WINTRUST_DATA trust;WINTRUST_FILE_INFO trust_file;GUID action;
 PACC_STORAGE_SHA256 sha;BYTE block[4096],digest[32];unsigned invoked;
} IG_CONSOLE_PROOF;
static IG_CONSOLE_PROOF ig_console_proof;
static int ig_cp_close_ref(HANDLE *ref)
{if(native.retained||ig_observer.retained)return 0;if(!*ref)return 1;if(!CloseHandle(*ref)){native.retained=1;return 0;}*ref=NULL;return 1;}
static int ig_cp_members(void)
{
 DWORD n,i;int self=0,elevated=0;n=GetConsoleProcessList(ig_console_proof.members,32);
 printf("IG_CONSOLE_MEMBERS count=%lu error=%lu; BOUND=32; NO_REALLOCATION_NO_RETRY\n",(unsigned long)n,(unsigned long)(n?0:GetLastError()));
 if(!n||n>32)return 0;
 for(i=0;i<n;i++){printf("IG_CONSOLE_MEMBER pid=%lu; CLIENT_PID_NOT_HOST_IDENTITY\n",(unsigned long)ig_console_proof.members[i]);if(ig_console_proof.members[i]==GetCurrentProcessId())self=1;if(ig_console_proof.members[i]==ig_observer.pid[1])elevated=1;}
 return self&&elevated;
}
static DWORD ig_cp_image_hash(void)
{
 DWORD n;ULONGLONG total=0;unsigned i;int result;
 result=pacc_storage_sha_init(&ig_console_proof.sha);if(result!=PACC_HASH_OK)return ERROR_INVALID_DATA;
 for(;;){if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;
 if(!ReadFile(ig_console_proof.image,ig_console_proof.block,sizeof(ig_console_proof.block),&n,NULL))return lab_error();if(!n)break;
 total+=n;if(total>16777216ULL)return ERROR_FILE_TOO_LARGE;
 result=pacc_storage_sha_update(&ig_console_proof.sha,ig_console_proof.block,n,NULL,NULL);if(result!=PACC_HASH_OK)return ERROR_INVALID_DATA;}
 if(pacc_storage_sha_final(&ig_console_proof.sha,ig_console_proof.digest,32,NULL,NULL)!=PACC_HASH_OK)return ERROR_INVALID_DATA;
 printf("IG_CONSOLE_IMAGE_SHA256=");for(i=0;i<32;i++)printf("%02x",(unsigned)ig_console_proof.digest[i]);printf("; BYTES=%llu; CURRENT_OPEN_FILE_NOT_MAPPED_IMAGE_PROOF\n",total);return 0;
}
static void ig_cp_capture(DWORD pid,DWORD parent)
{
 DWORD saved=GetLastError(),e=0,n,j,owner=0;unsigned role;FILETIME ct,et,kt,ut;HWND window;LONG trust_result;BY_HANDLE_FILE_INFORMATION after;
 if(ig_console_proof.invoked){printf("IG_CONSOLE_CAPTURE=ALREADY_ATTEMPTED_NO_RETRY\n");SetLastError(saved);return;}
 ig_console_proof.invoked=1;ig_console_proof.pid=pid;ig_console_proof.parent=parent;
 printf("IG_CONSOLE_CAPTURE attempt=" IG_ATTEMPT " pid=%lu parent=%lu tick=%llu bytes=%llu; DIAGNOSTIC_ONLY_CENSUS_DENY\n",(unsigned long)pid,(unsigned long)parent,GetTickCount64(),(ULONGLONG)sizeof(ig_console_proof));
 for(role=0;role<3;role++)printf("IG_ACTOR role=%u pid=%lu creation=%lu:%lu; VALIDATED_ACTOR_NOT_PARENT_TRUST\n",role,(unsigned long)ig_observer.pid[role],(unsigned long)ig_observer.creation[role].dwHighDateTime,(unsigned long)ig_observer.creation[role].dwLowDateTime);
 if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 ig_console_proof.process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,pid);if(!ig_console_proof.process){e=lab_error();goto done;}
 if(!GetProcessTimes(ig_console_proof.process,&ig_console_proof.creation,&et,&kt,&ut)){e=lab_error();goto done;}
 printf("IG_CONSOLE_INSTANCE pid=%lu creation=%lu:%lu wait=%lu; ORIGINAL_HANDLE_HELD\n",(unsigned long)pid,(unsigned long)ig_console_proof.creation.dwHighDateTime,(unsigned long)ig_console_proof.creation.dwLowDateTime,(unsigned long)WaitForSingleObject(ig_console_proof.process,0));
 if(!ProcessIdToSessionId(pid,&ig_console_proof.session)||!ProcessIdToSessionId(ig_observer.pid[1],&ig_console_proof.elevated_session)){e=lab_error();goto done;}
 ig_console_proof.facts.session_equal=ig_console_proof.session==ig_console_proof.elevated_session;
 printf("IG_CONSOLE_SESSION host=%lu elevated=%lu; SAME_SESSION_NOT_OWNERSHIP\n",(unsigned long)ig_console_proof.session,(unsigned long)ig_console_proof.elevated_session);
 n=1024;if(!QueryFullProcessImageNameW(ig_console_proof.process,0,ig_console_proof.path,&n)){e=lab_error();goto done;}
 printf("IG_CONSOLE_IMAGE_UTF16=");for(j=0;j<n&&j<768U;j++)printf("%04x",(unsigned)(unsigned short)ig_console_proof.path[j]);printf("; TRUNCATED=%u; PROCESS_PATH_ONLY\n",(unsigned)(n>768U));
 ig_console_proof.window=GetConsoleWindow();if(ig_console_proof.window)GetWindowThreadProcessId(ig_console_proof.window,&ig_console_proof.window_owner);
 printf("IG_CONSOLE_WINDOW hwnd=%llu creator_pid=%lu; NULL_OR_UNSUPPORTED_IS_UNKNOWN\n",(ULONGLONG)(ULONG_PTR)ig_console_proof.window,(unsigned long)ig_console_proof.window_owner);
 ig_console_proof.facts.members_before=ig_cp_members();
 /* I/O scope guard ONLY, never a trusted-name or census exception. No UNC,
  * network drive or arbitrary child image is opened for provenance. */
 if(_wcsicmp(ig_console_proof.path,L"C:\\Windows\\System32\\conhost.exe")||GetDriveTypeW(L"C:\\")!=DRIVE_FIXED){e=ERROR_NOT_SUPPORTED;puts("IG_CONSOLE_PROVENANCE=UNKNOWN_OUTSIDE_LOCAL_OS_FILE_SCOPE");goto measure;}
 {DWORD a=GetFileAttributesW(L"C:\\Windows"),b=GetFileAttributesW(L"C:\\Windows\\System32");if(a==INVALID_FILE_ATTRIBUTES||b==INVALID_FILE_ATTRIBUTES||((a|b)&FILE_ATTRIBUTE_REPARSE_POINT)){e=ERROR_NOT_SUPPORTED;puts("IG_CONSOLE_PROVENANCE=UNKNOWN_OS_PATH_REPARSE_OR_UNAVAILABLE");goto measure;}}
 ig_console_proof.image=CreateFileW(ig_console_proof.path,GENERIC_READ|FILE_READ_ATTRIBUTES,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(ig_console_proof.image==INVALID_HANDLE_VALUE){ig_console_proof.image=NULL;e=lab_error();goto measure;}
 if(!GetFileInformationByHandle(ig_console_proof.image,&ig_console_proof.file)){e=lab_error();goto measure;}
 if(ig_console_proof.file.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)){e=ERROR_INVALID_DATA;goto measure;}
 printf("IG_CONSOLE_IMAGE_FILE native=%lu:%lu:%lu size=%lu:%lu; NATIVE_FILE_NOT_PROCESS_MAPPING_ID\n",(unsigned long)ig_console_proof.file.dwVolumeSerialNumber,(unsigned long)ig_console_proof.file.nFileIndexHigh,(unsigned long)ig_console_proof.file.nFileIndexLow,(unsigned long)ig_console_proof.file.nFileSizeHigh,(unsigned long)ig_console_proof.file.nFileSizeLow);
 if(ig_console_proof.file.nFileSizeHigh||ig_console_proof.file.nFileSizeLow>16777216UL){e=ERROR_FILE_TOO_LARGE;goto measure;}
 e=ig_cp_image_hash();if(e)goto measure;
 ig_console_proof.action=(GUID)WINTRUST_ACTION_GENERIC_VERIFY_V2;
 ig_console_proof.trust_file.cbStruct=sizeof(ig_console_proof.trust_file);ig_console_proof.trust_file.pcwszFilePath=ig_console_proof.path;ig_console_proof.trust_file.hFile=ig_console_proof.image;
 ig_console_proof.trust.cbStruct=sizeof(ig_console_proof.trust);ig_console_proof.trust.dwUIChoice=WTD_UI_NONE;ig_console_proof.trust.dwUnionChoice=WTD_CHOICE_FILE;ig_console_proof.trust.pFile=&ig_console_proof.trust_file;ig_console_proof.trust.dwStateAction=WTD_STATEACTION_VERIFY;ig_console_proof.trust.dwProvFlags=WTD_CACHE_ONLY_URL_RETRIEVAL|WTD_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT;
 trust_result=WinVerifyTrust(INVALID_HANDLE_VALUE,&ig_console_proof.action,&ig_console_proof.trust);
 printf("IG_CONSOLE_FILE_TRUST native=%ld; OFFLINE_CACHE_ONLY; PUBLISHER_IDENTITY_AND_MAPPED_IMAGE_BINDING=UNKNOWN; NOT_AUTHORITY\n",trust_result);
 if(ig_console_proof.trust.hWVTStateData){ig_console_proof.trust.dwStateAction=WTD_STATEACTION_CLOSE;trust_result=WinVerifyTrust(INVALID_HANDLE_VALUE,&ig_console_proof.action,&ig_console_proof.trust);if(trust_result){native.retained=1;e=ERROR_INVALID_STATE;goto done;}ig_console_proof.trust.hWVTStateData=NULL;}
 n=1024;if(!QueryFullProcessImageNameW(ig_console_proof.process,0,ig_console_proof.after_path,&n)||!GetFileInformationByHandle(ig_console_proof.image,&after)){e=lab_error();goto measure;}
 ig_console_proof.facts.image_stable=!wcscmp(ig_console_proof.path,ig_console_proof.after_path)&&!memcmp(&after,&ig_console_proof.file,sizeof(after));
measure:
 printf("IG_CONSOLE_IDENTITY_STAGE error=%lu; FIRST_CENSUS_ERROR_UNCHANGED\n",(unsigned long)e);
 if(!papacc_server_console_win32_stop_requested()){
 DWORD metric=ig_measure_process(4,ig_console_proof.process,pid,&ig_console_proof.creation,&ig_console_proof.metrics);
 printf("IG_CONSOLE_RESOURCE_RESULT error=%lu; UNOBSERVED_FIELDS=UNKNOWN; NO_FIFTH_SLOT\n",(unsigned long)metric);
 if(metric&&!e)e=metric;
 for(role=0;role<3&&!ig_observer.retained;role++){DWORD actor=ig_observe_process(role);printf("IG_CONSOLE_SIMULTANEOUS_ACTOR role=%u error=%lu tick=%llu; SEQUENTIAL_SAMPLES_LIVE_OVERLAP_NOT_ATOMIC\n",role,(unsigned long)actor,GetTickCount64());if(actor){if(!e)e=actor;break;}}
 }
 if(!ig_observer.retained){DWORD close=ig_observer_close_snapshot();if(close){native.retained=1;if(!e)e=close;}}
 if(ig_observer.retained||native.retained){native.retained=1;goto done;}
 if(GetProcessTimes(ig_console_proof.process,&ct,&et,&kt,&ut))ig_console_proof.facts.identity_stable=!memcmp(&ct,&ig_console_proof.creation,sizeof(ct))&&WaitForSingleObject(ig_console_proof.process,0)==WAIT_TIMEOUT;
 ig_console_proof.facts.members_after=ig_cp_members();window=GetConsoleWindow();if(window)GetWindowThreadProcessId(window,&owner);ig_console_proof.facts.window_stable=window&&window==ig_console_proof.window&&owner==ig_console_proof.window_owner;ig_console_proof.facts.window_owner_matches=owner==pid;
done:
 ig_console_proof.facts.stopped=papacc_server_console_win32_stop_requested()!=0;ig_console_proof.facts.retained=native.retained||ig_observer.retained;
 printf("IG_CONSOLE_RELATION window_owner_candidate=%d identity_stable=%u image_stable=%u session_equal=%u members_before=%u members_after=%u; AUTHENTICATED_ASSOCIATION=UNKNOWN; CENSUS_RETURN=170; METRIC_ERROR=%lu; TICK=%llu\n",ig_cp_window_candidate(&ig_console_proof.facts),ig_console_proof.facts.identity_stable,ig_console_proof.facts.image_stable,ig_console_proof.facts.session_equal,ig_console_proof.facts.members_before,ig_console_proof.facts.members_after,(unsigned long)e,GetTickCount64());
 if(!native.retained&&!ig_observer.retained){if(ig_cp_close_ref(&ig_console_proof.image)&&ig_cp_close_ref(&ig_console_proof.process))puts("IG_CONSOLE_OWNED_REFERENCES=CLOSED; HOST_PROCESS_TERMINATION=NOT_CLAIMED");}
 else puts("IG_CONSOLE_OWNED_REFERENCES=RETAINED_UNKNOWN; NO_SECOND_CLOSE_NO_RETRY");
 SetLastError(saved);
}
#ifndef IG_CP_CAPTURE_REJECTION
#define IG_CP_CAPTURE_REJECTION(pid,parent) ig_cp_capture(pid,parent)
#endif
