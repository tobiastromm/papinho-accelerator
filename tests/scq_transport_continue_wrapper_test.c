/* Actual transport_continue wrapper and original-return projection; ALL effects mocked. */
#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT own;
static PAPACC_SCQ_OWNER_DECISION source;
static PAPACC_PERSONAL_SCQ_ORIGINAL model_proof;
static unsigned fault,logs,model_branch,checks,tail_calls,proof_calls;static DWORD logged,returned;
static int projection(FILE *stream,const char *format,...){va_list ap;(void)stream;(void)format;va_start(ap,format);model_branch=(unsigned)va_arg(ap,unsigned long);logged=(DWORD)va_arg(ap,unsigned long);va_end(ap);logs++;return 0;}
static const PAPACC_SCQ_OWNER_DECISION *model_registered(void){return fault==1?NULL:&source;}
static DWORD model_resource(void){return fault==2||fault==3?5:0;}
static DWORD bootstrap(void){return fault==7?5:returned;}
static DWORD read_continue(void){return fault==9?5:returned;}
static DWORD to_continue(void){return fault==10?5:returned;}
#define fprintf projection
#define personal_scq_registered_endpoint_decision(o) model_registered()
#define personal_scq_registered_owner_decision(o) model_registered()
#define personal_scq_query_resources(o,c) model_resource()
#define papacc_configuration_personal_scq_start_resource_current(o,c) model_resource()
#define papacc_server_console_win32_stop_requested() 0
#define personal_scq_session_native_current(o) (fault!=5)
#define personal_scq_native_original(o) (proof_calls++,fault==6?NULL:&model_proof)
#define scqt_owned(c,o) 1
#define scqt_stop(c,o) 0
#define scqb_write(c,o,s) bootstrap()
#define personal_scq_admin_native_current(o) (fault!=8)
#define scqkm_continue_session(...) read_continue()
#define scqr_continue_transport(...) to_continue()
#define papacc_configuration_personal_scq_transport_continue actual_continue
static DWORD model_final_read(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_MACHINE_FIXTURE_CONTROL *f,PAPACC_PERSONAL_CNG_PROBE *p,volatile LONG *stop)
{(void)c;(void)owner;(void)k;(void)f;(void)p;(void)stop;tail_calls++;return returned;}
#define scqkm_return_inconclusive model_final_read
#include "../apps/server/personal_scq_transport_continue_win32.inc"
#undef fprintf
#define C(x) do{checks++;if(!(x)){printf("FAIL line%u checks%u\n",(unsigned)__LINE__,checks);return 1;}}while(0)
static void reset(void){PAPACC_PERSONAL_NATIVE_CONTEXT *n;memset(&own,0,sizeof(own));memset(&source,0,sizeof(source));fault=logs=model_branch=logged=tail_calls=proof_calls=0;returned=ERROR_MORE_DATA;own.writer=GetCurrentThreadId();source.read_diagnostic=1;source.purpose=PAPACC_SCQ_PURPOSE_READ;n=&own.pending_commit.personal_native;n->scq_session.role=1;n->scq_session.phase=3;n->scq_session.transport.phase=1;n->scq_admin.start_consumed=1;n->scq_profile_data.read_diagnostic=1;n->scq_session.transport.read_diagnostic=1;n->scq_session.transport.query_refs3=1;}
int main(void){unsigned i;DWORD e;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&own.pending_commit.personal_native;
 for(i=1;i<=10;i++){reset();fault=i;if(i==2)n->scq_session.role=2;if(i==4)own.pending_commit.stop=1;if(i>=8)n->scq_session.transport.legs=1;if(i==10){source.read_diagnostic=0;source.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;}e=actual_continue(&own);C(e!=0&&e!=ERROR_IO_PENDING&&e!=ERROR_MORE_DATA);C(logs==1&&model_branch==i&&logged==e);}
 reset();C(actual_continue(&own)==ERROR_MORE_DATA&&logs==0);
 reset();returned=ERROR_IO_PENDING;C(actual_continue(&own)==ERROR_IO_PENDING&&logs==0);
 reset();returned=0;C(actual_continue(&own)==ERROR_MORE_DATA&&logs==0);
 reset();n->scq_session.transport.legs=1;returned=0;C(actual_continue(&own)==0&&logs==0);
 reset();n->scq_session.transport.legs=1;returned=ERROR_IO_PENDING;C(actual_continue(&own)==ERROR_IO_PENDING&&logs==0);
 reset();C(scqtc_result(&own,9U,ERROR_MORE_DATA)==ERROR_MORE_DATA&&logs==0);
 for(i=0;i<6U;i++){
  reset();n->scq_session.phase=4;n->scq_session.transport.operation=16;n->scq_session.transport.pending=1;
  n->runtime_cng_probe.phase=3;n->runtime_cng_probe.classification=PAPACC_CNG_PROBE_NOT_FOUND;n->runtime_cng_probe.provider_calls=n->runtime_cng_probe.open_calls=n->runtime_cng_probe.close_attempted=n->runtime_cng_probe.close_calls=1;
  n->runtime_cng_probe.open_result=n->runtime_cng_probe.first_error=NTE_BAD_KEYSET;n->runtime_cng_probe.first_stage=4;n->runtime_cng_probe.first_origin=3;
  returned=i==1U?ERROR_IO_PENDING:i==2U?ERROR_BROKEN_PIPE:ERROR_NOT_FOUND;
  if(i==3U)fault=8U;if(i==4U)own.pending_commit.stop=1;if(i==5U){source.read_diagnostic=0;source.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;}
  e=actual_continue(&own);
  if(i<3U){C(e==returned&&tail_calls==1U&&!proof_calls);C(i==1U?logs==0:model_branch==11U);}
  if(i==3U)C(e==ERROR_ACCESS_DENIED&&!tail_calls&&!proof_calls&&model_branch==11U);
  if(i==4U)C(e==ERROR_CANCELLED&&!tail_calls&&!proof_calls&&model_branch==4U);
  if(i==5U)C(!tail_calls&&proof_calls==1U);
 }
 printf("PASS TRANSPORT_WRAPPER_FIRST_RETURN checks=%u ALL_TEN_BRANCHES_ORIGINAL_ERROR_BUSY_SUCCESS_PRESERVED NO_NEW_QUERY_NO_EFFECTS\n",checks);return 0;}
