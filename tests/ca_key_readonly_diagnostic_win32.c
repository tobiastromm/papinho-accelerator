/* Read-only diagnostic harness, never linked into product startup.
 * Build only with PAPACC_CA_READ_ONLY_DIAGNOSTIC for BOTH translation units. */
#ifndef PAPACC_CA_READ_ONLY_DIAGNOSTIC
#error Diagnostic harness requires read-only backend variant
#endif
#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static int diag_hex(const char *s,BYTE out[16]) {
 unsigned i,v=0;if(!s||strlen(s)!=32)return 0;
 for(i=0;i<16;i++){
  unsigned a,b;char x=s[2*i],y=s[2*i+1];
  if(!((x>='0'&&x<='9')||(x>='a'&&x<='f'))||!((y>='0'&&y<='9')||(y>='a'&&y<='f')))return 0;
  a=(unsigned)(x<='9'?x-'0':x-'a'+10);b=(unsigned)(y<='9'?y-'0':y-'a'+10);
  out[i]=(BYTE)((a<<4)|b);v|=out[i];
 }return v!=0;
}
static int diag_identity(int argc,char **argv,BYTE a[16],BYTE g[16]) {
 char expected[96];int n;
 if(argc!=7||strcmp(argv[1],"--observe-existing")||!diag_hex(argv[2],a)||!diag_hex(argv[3],g)||!argv[5][0]||!argv[6][0])return 0;
 n=_snprintf_s(expected,sizeof(expected),_TRUNCATE,"PapinhoAccelerator.CA.%s.%s",argv[2],argv[3]);
 return n>0&&!strcmp(expected,argv[4]);
}
/* Stop is a local diagnostic cancellation request, never product Stop authority.
 * No polling/retry; an already-running OS query has no bounded cancel claim. */
static SECURITY_STATUS diag_observe(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service,volatile LONG *stop) {
 SECURITY_STATUS s,c;
 if(InterlockedCompareExchange(stop,0,0))return NTE_USER_CANCELLED;
 s=papacc_ca_key_open(o,a,g,admin,service);
 if(o->state==PAPACC_CA_KEY_READY){c=papacc_ca_key_close(o);if(c!=ERROR_SUCCESS)return c;}
 if(s==ERROR_SUCCESS&&InterlockedCompareExchange(stop,0,0))return NTE_USER_CANCELLED;
 return s;
}
#ifndef PAPACC_CA_DIAGNOSTIC_NO_MAIN
static volatile LONG diagnostic_stop;
static BOOL WINAPI diag_control(DWORD event) {
 if(event!=CTRL_C_EVENT&&event!=CTRL_BREAK_EVENT)return FALSE;
 InterlockedExchange(&diagnostic_stop,1);return TRUE;
}
int main(int argc,char **argv) {
 static PAPACC_CA_KEY owner;BYTE a[16],g[16];PSID admin=NULL,service=NULL;SECURITY_STATUS s;int result=0;
 if(!diag_identity(argc,argv,a,g)){puts("DENY incomplete or divergent exact diagnostic identity; no key query");return 2;}
 if(!ConvertStringSidToSidA(argv[5],&admin)){puts("DENY invalid operator SID");return 2;}
 if(!ConvertStringSidToSidA(argv[6],&service)){result=2;goto done;}
 if(!SetConsoleCtrlHandler(diag_control,TRUE)){result=3;goto done;}
 printf("purpose=DIAGNOSTIC_OBSERVE_EXISTING; provider=Microsoft Software Key Storage Provider; scope=MACHINE; name=%s; operator=%s; service=%s\n",argv[4],argv[5],argv[6]);
 s=diag_observe(&owner,a,g,admin,service,&diagnostic_stop);
 printf("status=0x%08lx; first_present=%lu; function=%lu; stage=%lu; origin=%lu; api=%lu; original=0x%08lx; backend=0x%08lx; win32=%lu\n",
  (unsigned long)s,(unsigned long)owner.first_failure.present,(unsigned long)owner.first_failure.function,
  (unsigned long)owner.first_failure.stage,(unsigned long)owner.first_failure.origin,(unsigned long)owner.first_failure.native_api,
  (unsigned long)owner.first_failure.original_code,(unsigned long)owner.first_failure.backend_code,(unsigned long)owner.first_failure.win32_error);
 printf("first_snapshot_state=%lu; created=%lu; finalized=%lu; key_owned=%lu; provider_owned=%lu; ownership=%lu\n",
  (unsigned long)owner.first_failure.state,(unsigned long)owner.first_failure.created,(unsigned long)owner.first_failure.finalization,
  (unsigned long)owner.first_failure.key_owned,(unsigned long)owner.first_failure.provider_owned,(unsigned long)owner.first_failure.ownership);
 printf("state=%d; created=%d; finalized=%lu; close_attempted=%d; close_confirmed=%d; key_owned=%d; provider_owned=%d; persisted_key_state=UNKNOWN\n",
  (int)owner.state,owner.created,(unsigned long)owner.diagnostic_finalization,owner.close_attempted,owner.close_confirmed,owner.key!=0,owner.provider!=0);
 result=s==ERROR_SUCCESS?0:4;
 if(!SetConsoleCtrlHandler(diag_control,FALSE))result=5;
done:
 if(service&&LocalFree(service))result=6;
 if(admin&&LocalFree(admin))result=6;
 return result;
}
#endif