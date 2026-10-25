/* Pure pin/profile fixture: no native admission/SCM/files. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../apps/server/server_service_admission_bd_win32.h"
#include "../tools/qualification/scm-n0-context-model.h"
#define T(x) do{if(!(x)){printf("PIN_PROFILE_FAIL line=%d\n",__LINE__);return 1;}cases++;}while(0)
int main(void)
{
 unsigned cases=0;BY_HANDLE_FILE_INFORMATION id;char *argv[]={"fixture","--prepare-n0","attempt","actor","birth","thread","aux","auxbirth","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
 {IG_N0_FILE_RECORD r;T(!ig_n0_parse("",0,&r));}
 ZeroMemory(&id,sizeof(id));id.dwVolumeSerialNumber=417614100;id.nNumberOfLinks=1;id.nFileIndexHigh=1966080;id.nFileIndexLow=2672522;id.nFileSizeLow=397;
#ifdef PAPACC_N0START03_READONLY_PINS
 T(ig_n0_inputs_ready());T(ig_n0_input_identity(&id,0));T(!ig_n0_input_identity(&id,1));
 T(!strcmp(IG_N0_DOSSIER_SHA,"5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050"));
 T(!strcmp(IG_N0_EVIDENCE_SHA,"15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7"));
 T(ig_n0_readonly_command(9,argv));T(!ig_n0_readonly_command(8,argv));argv[8]="";T(!ig_n0_readonly_command(9,argv));argv[8]="missing-N7-and-approval";T(!ig_n0_readonly_command(9,argv));
#else
 T(!ig_n0_inputs_ready());T(!ig_n0_input_identity(&id,0));T(!IG_N0_DOSSIER_SHA[0]&&!IG_N0_EVIDENCE_SHA[0]);T(ig_n0_readonly_command(8,argv));T(!ig_n0_readonly_command(9,argv));
#endif
 id.nFileIndexLow++;T(!ig_n0_input_identity(&id,0));id.nFileIndexHigh=2883584;id.nFileIndexLow=2416818;id.nFileSizeLow=7573;
#ifdef PAPACC_N0START03_READONLY_PINS
 T(ig_n0_input_identity(&id,1));
#endif
 id.nNumberOfLinks=2;T(!ig_n0_input_identity(&id,1));id.nNumberOfLinks=1;id.dwFileAttributes=FILE_ATTRIBUTE_REPARSE_POINT;T(!ig_n0_input_identity(&id,1));
 id.dwFileAttributes=0;id.nFileIndexHigh=196608;id.nFileIndexLow=2841429;id.nFileSizeLow=5358;T(!ig_n0_input_identity(&id,1));
 T(ig_n0_policy_ready(IG_N0_N7_ED11_SHA));T(!ig_n0_policy_ready(NULL));T(!ig_n0_policy_ready(""));T(!ig_n0_policy_ready("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));T(!ig_n0_fresh(1002,1,1000));T(!ig_n0_fresh(60002,1,60000));
 printf("N0START03_PIN_PROFILE=PASS cases=%u; SYNTHETIC_ONLY; NO_AUTHORITY_NO_BUDGET_NO_NATIVE_ENTRY\n",cases);return 0;
}
