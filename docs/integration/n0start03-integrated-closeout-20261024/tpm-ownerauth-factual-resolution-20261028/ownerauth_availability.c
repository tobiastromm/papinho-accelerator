#define _WIN32_WINNT 0x0602
#include <windows.h>
#include <tbs.h>
#include <stdio.h>
typedef TBS_RESULT (WINAPI *OPEN)(PCTBS_CONTEXT_PARAMS,PTBS_HCONTEXT);
typedef TBS_RESULT (WINAPI *QUERY)(TBS_HCONTEXT,TBS_OWNERAUTH_TYPE,PBYTE,PUINT32);
typedef TBS_RESULT (WINAPI *CLOSE)(TBS_HCONTEXT);
int main(void){HMODULE module;OPEN open;QUERY query;CLOSE close;TBS_CONTEXT_PARAMS2 params;TBS_HCONTEXT ctx=NULL;TBS_RESULT a,b=0,c=0;UINT32 length=0;DWORD before=0,after=0;const char *state;
 if(!GetProcessHandleCount(GetCurrentProcess(),&before)||before>124){printf("{\"first\":\"HANDLE_GUARD\",\"queried\":false,\"value_read\":false}\n");return 2;}
 module=LoadLibraryExW(L"tbs.dll",NULL,LOAD_LIBRARY_SEARCH_SYSTEM32);if(!module){printf("{\"first\":\"LOAD_TBS\",\"value_read\":false}\n");return 2;}
 open=(OPEN)(void *)GetProcAddress(module,"Tbsi_Context_Create");query=(QUERY)(void *)GetProcAddress(module,"Tbsi_Get_OwnerAuth");close=(CLOSE)(void *)GetProcAddress(module,"Tbsip_Context_Close");
 if(!open||!query||!close){printf("{\"first\":\"API_UNAVAILABLE\",\"value_read\":false}\n");return FreeLibrary(module)?2:3;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&after)||after>128){printf("{\"first\":\"HANDLE_GUARD_AFTER_LOAD\",\"value_read\":false}\n");return FreeLibrary(module)?2:3;}
 ZeroMemory(&params,sizeof(params));params.version=TBS_CONTEXT_VERSION_TWO;params.includeTpm20=1;
 a=open((PCTBS_CONTEXT_PARAMS)&params,&ctx);
 if(a==TBS_SUCCESS&&ctx){
  /* Official optional NULL output. Never allocate, request or receive bytes.
   * STORAGE_20=13 is distinct from TPM1.2 FULL and TPM2.0 LockoutAuth. */
  b=query(ctx,TBS_OWNERAUTH_TYPE_STORAGE_20,NULL,&length);
 }
 if(ctx)c=close(ctx);
 state=a?"CONTEXT_UNAVAILABLE":(b==0x80284015UL?"STORAGE20_NOT_FOUND_BY_SUPPORTED_API":((b==TBS_SUCCESS||b==0x80284005UL)&&length?"METADATA_INDICATES_SOURCE_PRESENT_NOT_VALUE_OR_CUSTODY_PROOF":"QUERY_NOT_CONCLUSIVE"));
 printf("{\"open_rc\":\"%08lx\",\"query_rc\":\"%08lx\",\"query_type\":13,\"query_attempted\":%s,\"output_buffer\":\"NULL\",\"value_read\":false,\"state\":\"%s\",\"close_rc\":\"%08lx\",\"context_close\":%s,\"own_before\":%lu,\"own_after_load\":%lu,\"TPM_Submits\":0,\"NV_mutations\":\"NONE\",\"authority\":\"DENY\"}\n",a,b,a==0&&ctx?"true":"false",state,c,ctx&&c==0?"true":"false",before,after);
 if(c){fflush(stdout);for(;;)Sleep(1000);} /* original context/module retained */
 if(!FreeLibrary(module))return 3;return a?2:0;
}
