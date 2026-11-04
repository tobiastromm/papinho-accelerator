/* Exact adapter body, owner dispatch modeled. Never invokes the native owner,
 * filesystem/SCM/RNG, child process, endpoint or installed executable. The
 * owner itself is covered by staged_binary_update_test's API-mocked suite.
 */
#include <stdio.h>
#include <string.h>
static unsigned calls,checks;
static int owner_result,expected_argc;
static const char *const *expected_argv;
static FILE *expected_out,*expected_err;
static int update_model_owner(int argc,const char *const *argv,FILE *out,FILE *err)
{
 calls++;
 if(argc!=expected_argc||argv!=expected_argv||out!=expected_out||err!=expected_err)return 99;
 return owner_result;
}
#define papacc_server_launcher_binary_update_cli update_model_owner
#define main papacc_update_program_main
#include "../apps/server/update_main.c"
#undef main

#define CHECK(x) do {checks++;if(!(x)){printf("FAIL update entry line=%u\n",(unsigned)__LINE__);return 1;}} while(0)
int main(void)
{
 const char *args[17]={"papacc_update.exe","--owner-manual-binary-update","DATA_ONLY",
  "42","0","3","42","0","4","old","new","ssl","crypto","42","0","18","executor"};
 const char *bad[]={"service","install","host","start","stop","--owner-manual-selected-staging", "--owner-manual-prepare-update-utility", "--owner-manual-update-utility", "--scq-original", "--help", ""};
 unsigned i;expected_out=stdout;expected_err=stderr;expected_argc=17;expected_argv=args;
 CHECK(papacc_update_entry(0,NULL,stdout,stderr)==2&&!calls);
 CHECK(papacc_update_entry(1,args,stdout,stderr)==2&&!calls);
 args[1]=NULL;CHECK(papacc_update_entry(17,args,stdout,stderr)==2&&!calls);
 for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++){args[1]=bad[i];CHECK(papacc_update_entry(17,args,stdout,stderr)==2&&!calls);}
 args[1]="--owner-manual-binary-update";
 CHECK(papacc_update_entry(17,args,NULL,stderr)==2&&!calls);
 CHECK(papacc_update_entry(17,args,stdout,NULL)==2&&!calls);
 owner_result=0;CHECK(papacc_update_entry(17,args,stdout,stderr)==0&&calls==1U);
 owner_result=2;CHECK(papacc_update_entry(17,args,stdout,stderr)==2&&calls==2U);
 owner_result=-1;CHECK(papacc_update_entry(17,args,stdout,stderr)==2&&calls==3U);
 owner_result=99;CHECK(papacc_update_entry(17,args,stdout,stderr)==2&&calls==4U);
 expected_argc=2;owner_result=2;CHECK(papacc_update_entry(2,args,stdout,stderr)==2&&calls==5U);
 printf("PASS UPDATE_ENTRY checks=%u EXACT_ARGV_AND_STREAM_BORROW UPDATE_ONLY OWNER_DISPATCH_MODELED NO_NATIVE_EFFECTS\n",checks);
 return 0;
}
