/* Real adapter; process/token/SCM edges modeled, no protected effects. */
#include <stdio.h>
#include <stdarg.h>
static unsigned rd_site,rd_lines,rd_returns;static unsigned long rd_error;
static int rd_print(FILE *stream,const char *format,...)
{
 char line[1024];va_list args;int n;unsigned site;unsigned long error;(void)stream;
 va_start(args,format);n=_vsnprintf_s(line,sizeof(line),_TRUNCATE,format,args);va_end(args);
 if(sscanf_s(line,"SCQ_START_RUNTIME_FIRST_CAUSE site=%u error=%lu",&site,&error)==2){rd_site=site;rd_error=error;rd_lines++;}
 if(sscanf_s(line,"SCQ_START_RUNTIME_CAPTURE_RETURN site=%u error=%lu",&site,&error)==2)rd_returns++;
 return n;
}
#define fprintf rd_print
#define main preserved_runtime_diagnostics_main
#include "personal_runtime_identity_test.c"
#undef main
#undef fprintf
int main(void)
{
 static const unsigned failures[]={1,2,3,5,7,8,13,17,18,19,20,21,22,23,24,25};
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;DWORD result;unsigned i,sites[16];
 C(ConvertStringSidToSidA("S-1-5-6",&service_sid));C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 runtime_service_state=SERVICE_START_PENDING;
 for(i=0;i<sizeof(failures)/sizeof(failures[0]);i++){
  reset(&r,failures[i],PAPACC_PERSONAL_RUNTIME_VIRTUAL);rd_site=rd_lines=rd_returns=0;rd_error=0;
  result=papacc_personal_runtime_capture_closed(&r,(HANDLE)1,virtual_sid,1U);
  C(result!=0&&rd_lines==1U&&rd_site!=0&&rd_error==r.first_error&&rd_returns==1U);sites[i]=rd_site;
  C(papacc_personal_runtime_capture_closed(&r,(HANDLE)1,virtual_sid,1U)!=0&&rd_lines==1U);
  fault=0;C(!papacc_personal_runtime_close(&r));
 }
 C(sites[1]!=sites[2]&&sites[1]!=sites[13]&&sites[2]!=sites[13]);
 reset(&r,0,2);rd_site=rd_lines=rd_returns=0;C(!papacc_personal_runtime_capture_closed(&r,(HANDLE)1,virtual_sid,1U));C(!rd_lines&&!rd_returns);C(!papacc_personal_runtime_close(&r));
 runtime_service_state=SERVICE_RUNNING;reset(&r,2,2);rd_lines=rd_returns=0;C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)!=0&&!rd_lines&&!rd_returns);fault=0;C(!papacc_personal_runtime_close(&r));
 C(!LocalFree(service_sid)&&!LocalFree(virtual_sid)&&!LocalFree(sam_sid));
 puts("PASS START_RUNTIME_DIAGNOSTICS cases=18 FIRST_SITE_AND_ERROR_PRESERVED OTHER_PROFILES_UNCHANGED ALL_NATIVE_EDGES_MODELED");return 0;
}
