/* Closed entry with simulated transport/body. No owner/SCM/files/intent. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "../apps/server/server_service_admission_bd_win32.h"
#include "../tools/qualification/scm-n0-context-model.h"
static unsigned connects,closes,bodies,frozen,cases;
static DWORD connect_error,close_error;
static jmp_buf trap;
static DWORD WINAPI PaccTransportClient(const WCHAR *name,DWORD pid,ULONGLONG created){(void)name;(void)pid;(void)created;connects++;return connect_error;}
static DWORD PaccTransportClientClose(void){closes++;return close_error;}
static int ig_transport_body(int argc,char **argv){(void)argc;(void)argv;bodies++;return 7;}
__declspec(noreturn) static void ig_transport_write_failed(void){frozen=1;longjmp(trap,1);}
#define main isolated_entry
#include "../tools/qualification/scm-n0-private-stdout-entry.inc"
#undef main
#define T(x) do{cases++;if(!(x)){printf("ENTRY_FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static void reset(void){connects=closes=bodies=frozen=0;connect_error=close_error=0;}
int main(void)
{
 char *a[]={"fixture","--prepare-n0","0b1de869e11493b7cb08fb329578f6ce","123","456","7","124","457","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","--private-stdout","\\\\.\\pipe\\papacc-n0-stdout-00000000000000000000000000000000","123","456"};
 char *base[13];unsigned i;int expected=0;
#ifdef PAPACC_N0START03_PRIVATE_POSITIVE
 a[1]="--cycle-n0";expected=1;
#elif defined(PAPACC_N0START03_READONLY_PINS)
 expected=1;
#endif
 T(ig_n0_inputs_ready()==expected);
 memcpy(base,a,sizeof(a));T(ig_transport_bootstrap(13,a)==expected);
 reset();T(isolated_entry(13,a)==(expected?7:2));T(connects==(unsigned)expected&&bodies==(unsigned)expected&&closes==(unsigned)expected);
 for(i=0;i<13;i++){memcpy(a,base,sizeof(a));a[i]=NULL;reset();T(isolated_entry(13,a)==2);T(!connects&&!bodies&&!closes);}
 memcpy(a,base,sizeof(a));reset();T(isolated_entry(12,a)==2&&!connects);T(!ig_transport_bootstrap(13,NULL));
#define NEG(index,value) do{memcpy(a,base,sizeof(a));a[index]=value;reset();T(isolated_entry(13,a)==2&&!connects&&!bodies&&!closes);}while(0)
 NEG(2,"75330d11cc7e4123ae62eadc9a59c2f5");NEG(3,"0123");NEG(3,"4294967296");NEG(4,"18446744073709551616");NEG(5,"0");NEG(5,"7x");NEG(6,"123");NEG(7,"-1");NEG(8,"");NEG(8,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");NEG(9,"--other");NEG(10,"\\\\remote\\pipe\\x");NEG(10,"\\\\.\\pipe\\papacc-n0-stdout-0000000000000000000000000000000A");NEG(11,"125");NEG(12,"458");
#ifdef PAPACC_N0START03_PRIVATE_POSITIVE
 NEG(1,"--prepare-n0");
#else
 NEG(1,"--cycle-n0");
#endif
 memcpy(a,base,sizeof(a));
 if(expected){reset();connect_error=ERROR_ACCESS_DENIED;T(isolated_entry(13,a)==2);T(connects==1&&!bodies&&closes==1);
  reset();close_error=ERROR_INVALID_HANDLE;if(!setjmp(trap)){(void)isolated_entry(13,a);T(0);}T(frozen&&connects==1&&bodies==1&&closes==1);
  reset();connect_error=ERROR_ACCESS_DENIED;close_error=ERROR_INVALID_HANDLE;if(!setjmp(trap)){(void)isolated_entry(13,a);T(0);}T(frozen&&connects==1&&!bodies&&closes==1);
 }
 T(ig_transport_positive_record(IG_PRIVATE_POSITIVE_APPROVAL_PREFIX,sizeof(IG_PRIVATE_POSITIVE_APPROVAL_PREFIX)-1));T(!ig_transport_positive_record(NULL,999));T(!ig_transport_positive_record(IG_PRIVATE_POSITIVE_APPROVAL_PREFIX,10));
 T(!ig_transport_positive_record("{\n  \"status\": \"EXACT_MANUAL_READONLY_OWNER_APPROVED\",",52));
 printf("PRIVATE_ENTRY_MODEL=PASS cases=%u positive_or_readonly_optin=%d NO_NATIVE_AUTHORITY_NO_EFFECT\n",cases,expected);return 0;
}
