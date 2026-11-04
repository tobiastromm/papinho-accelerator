/* Real progress/wait bodies; all process/token/SCM/pipe/wait edges modeled. */
#define PAPACC_FIRST_CONTROLLER_ENTRY preserved_progress_first_main
#include "scq_first_controller_test.c"
#include <stdarg.h>
static unsigned pg_case,pg_calls,pg_step,pg_lines,pg_bind,pg_offer,pg_enter,pg_refs,pg_continue,pg_poll;
static PAPACC_CONFIGURATION_COMMIT pg_owner;
static PAPACC_SCQ_OWNER_DECISION pg_decision,pg_other;
static unsigned pg_wait_step,pg_carrier_calls,pg_clock_calls,pg_wait_calls;
static char pg_last_line[2048];
static int pg_print(FILE *stream,const char *format,...)
{
 char line[2048];va_list args;int count;unsigned long step;(void)stream;
 va_start(args,format);count=_vsnprintf_s(line,sizeof(line),_TRUNCATE,format,args);va_end(args);
 if(sscanf_s(line,"SCQ_START_PROGRESS_FIRST_CAUSE step=%lu",&step)==1){pg_step=(unsigned)step;pg_lines++;}
 strcpy_s(pg_last_line,sizeof(pg_last_line),line);
 if(sscanf_s(line,"SCQ_START_WAIT_FIRST_CAUSE step=%lu",&step)==1){pg_wait_step=(unsigned)step;pg_lines++;}
 return count;
}
static const PAPACC_SCQ_OWNER_DECISION *pg_registered(PAPACC_CONFIGURATION_COMMIT *o){(void)o;pg_calls++;return pg_case==1U?NULL:&pg_decision;}
static const PAPACC_SCQ_OWNER_DECISION *pg_endpoint(PAPACC_CONFIGURATION_COMMIT *o){(void)o;pg_calls++;return pg_case==2U?&pg_other:&pg_decision;}
static int pg_current(PAPACC_CONFIGURATION_COMMIT *o){(void)o;pg_calls++;return pg_case!=3U;}
static DWORD pg_resource(PAPACC_CONFIGURATION_COMMIT *o,int creating){(void)o;C(creating==1);pg_calls++;return pg_case==48U?ERROR_NOT_FOUND:pg_case==4U?ERROR_NOT_ENOUGH_QUOTA:0;}
static PAPACC_RESULT pg_runtime(PAPACC_CONFIGURATION_COMMIT *o,HANDLE h,PSID sid,unsigned profile,DWORD type)
{(void)h;(void)sid;C(profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL&&type==SERVICE_SID_TYPE_UNRESTRICTED);pg_bind++;if(pg_case==9U){o->pending_commit.personal_native.runtime_identity.first_error=ERROR_ACCESS_DENIED;return PAPACC_RESULT_INVALID_STATE;}o->pending_commit.personal_native.runtime_identity.phase=1;return PAPACC_RESULT_OK;}
static PAPACC_RESULT pg_offer_data(PAPACC_CONFIGURATION_COMMIT *o){(void)o;pg_offer++;return pg_case==10U?PAPACC_RESULT_INVALID_ARGUMENT:PAPACC_RESULT_OK;}
static int pg_stop_requested(void){return pg_case==11U;}
static DWORD pg_stop(PAPACC_SCQ_SESSION_DRIVER *c,PAPACC_CONFIGURATION_COMMIT *o){(void)c;(void)o;return ERROR_CANCELLED;}
static DWORD pg_enter_data(PAPACC_CONFIGURATION_COMMIT *o,unsigned operation,const void *unused)
{(void)o;(void)unused;pg_enter++;if(operation==1U&&pg_case==12U)return ERROR_ACCESS_DENIED;if(operation==2U&&pg_case==14U)return ERROR_INVALID_DATA;if(pg_case==18U)return ERROR_IO_PENDING;return 0;}
static DWORD pg_poll_data(PAPACC_SCQ_SESSION_DRIVER *c,PAPACC_CONFIGURATION_COMMIT *o){(void)c;(void)o;pg_poll++;return pg_case==13U?ERROR_BROKEN_PIPE:pg_case==19U?ERROR_IO_PENDING:0;}
static DWORD pg_references(PAPACC_CONFIGURATION_COMMIT *o,const void *unused){(void)o;(void)unused;pg_refs++;return pg_case==15U?ERROR_ACCESS_DENIED:pg_case==20U?ERROR_MORE_DATA:0;}
static DWORD pg_continue_data(PAPACC_CONFIGURATION_COMMIT *o){(void)o;pg_continue++;return pg_case==46U||pg_case==47U?ERROR_NOT_FOUND:pg_case==16U?ERROR_ACCESS_DENIED:pg_case==21U?ERROR_IO_PENDING:pg_case==22U?ERROR_MORE_DATA:0;}
#define personal_scq_registered_owner_decision pg_registered
#define personal_scq_registered_endpoint_decision pg_endpoint
#define personal_scq_admin_native_current pg_current
#define papacc_configuration_personal_scq_start_resource_current pg_resource
#define papacc_configuration_personal_runtime_bind pg_runtime
#define papacc_configuration_personal_scq_session_offer_data pg_offer_data
#undef papacc_server_console_win32_stop_requested
#define papacc_server_console_win32_stop_requested pg_stop_requested
#define scqt_stop pg_stop
#define scqt_poll pg_poll_data
#define papacc_configuration_personal_scq_transport_enter pg_enter_data
#define papacc_configuration_personal_scq_transport_references pg_references
#define papacc_configuration_personal_scq_transport_continue pg_continue_data
static PAPACC_RESULT pg_inconclusive(PAPACC_CONFIGURATION_COMMIT *o){(void)o;return pg_case==46U?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;}
#define papacc_configuration_personal_scq_controller_inconclusive pg_inconclusive
#define fprintf pg_print
#include "../apps/server/personal_scq_start_progress_win32.inc"
static struct {PAPACC_CONFIGURATION_COMMIT configuration_commit;} pg_root;
static DWORD pg_carrier(PS_SCQ_CARRIER *scope){(void)scope;pg_carrier_calls++;return pg_case==32U?ERROR_NOT_FOUND:0;}
static ULONGLONG pg_clock(void){pg_clock_calls++;return 10;}
static int pg_remaining(const PS_SCQ_CARRIER *scope,unsigned low,unsigned high,unsigned *out){(void)scope;(void)low;(void)high;*out=10;return pg_case!=33U;}
static PAPACC_RESULT pg_wait(HANDLE event,HANDLE process,unsigned ms,DWORD *observed)
{(void)event;(void)process;(void)ms;pg_wait_calls++;*observed=(pg_case==35U||pg_case>=40U)?WAIT_TIMEOUT:pg_case==36U?WAIT_OBJECT_0+1U:WAIT_OBJECT_0;return pg_case==34U?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
#define PAPACC_OFFLINE_D3_WORKER 1
#define root pg_root
#undef papacc_server_launcher_scq_start_carrier
#define papacc_server_launcher_scq_start_carrier pg_carrier
#undef GetTickCount64
#define GetTickCount64 pg_clock
#define ps_scq_remaining_ms pg_remaining
#define papacc_server_console_win32_wait_original pg_wait
#undef papacc_server_launcher_scq_start_wait
#include "../apps/server/personal_scq_start_wait_win32.inc"
#undef fprintf
int main(void)
{
 unsigned i,expected;DWORD result;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&pg_owner.pending_commit.personal_native;
 for(i=0;i<25U;i++){
  memset(&pg_owner,0,sizeof(pg_owner));pg_case=i;pg_step=pg_lines=pg_calls=pg_bind=pg_offer=pg_enter=pg_refs=pg_continue=pg_poll=0;
  n->scq_admin.start_consumed=1;n->scq_session.transport.phase=2;n->scq_session.transport.pipe=(HANDLE)1;n->scq_session.transport.original=&n->scq_admin.value;n->scq_profile_data.waiting_runtime=1;
  if(i==5U)n->scq_admin.start_consumed=0;if(i==6U)n->scq_session.transport.phase=0;if(i==7U)n->scq_session.transport.pipe=NULL;if(i==8U)n->scq_session.transport.original=NULL;
  if(i==13U||i==19U)n->scq_session.transport.pending=1;
  if(i==23U){n->runtime_identity.phase=1;n->scq_profile_data.waiting_runtime=0;n->scq_session.transport.phase=3;n->scq_session.transport.delivery_process=(HANDLE)2;}
  result=papacc_configuration_personal_scq_start_progress(&pg_owner,i==24U?NULL:(HANDLE)2);
  expected=0;if(i>=1U&&i<=3U||i==24U)expected=1;if(i==4U)expected=2;if(i>=5U&&i<=8U)expected=3;if(i==9U)expected=4;if(i==10U)expected=5;if(i==12U)expected=6;if(i==13U)expected=7;if(i==14U)expected=8;if(i==15U)expected=9;if(i==16U)expected=10;
  C(pg_step==expected);C(pg_lines==(expected?1U:0U));
  if(expected)C(result!=0);else if(i==11U)C(result==ERROR_CANCELLED&&pg_owner.pending_commit.stop==1);else if(i==18U||i==19U||i==21U)C(result==ERROR_IO_PENDING);else if(i==20U||i==22U)C(result==ERROR_MORE_DATA);else C(result==0);
  if(i==4U)C(result==ERROR_NOT_ENOUGH_QUOTA&&!pg_bind&&!pg_enter);
  if(i==9U)C(result==ERROR_ACCESS_DENIED&&!pg_offer&&!pg_enter);
  if(i==10U)C(!pg_enter&&!pg_refs&&!pg_continue);
  if(i==23U)C(!pg_bind&&!pg_offer&&!pg_enter&&!pg_refs&&pg_continue==1U);
  C(n->scq_admin.start_consumed==(i==5U?0:1));
 }
 for(i=46U;i<=48U;i++){
  memset(&pg_owner,0,sizeof(pg_owner));pg_case=i;pg_step=pg_lines=0;n->scq_admin.start_consumed=1;
  n->scq_session.transport.phase=1;n->scq_session.transport.pipe=(HANDLE)1;n->scq_session.transport.original=&n->scq_admin.value;
  n->runtime_identity.phase=1;n->scq_session.transport.delivery_process=(HANDLE)2;
  result=papacc_configuration_personal_scq_start_progress(&pg_owner,(HANDLE)2);
  C(result==(i==46U?PAPACC_SCQ_PROGRESS_CLOSED_INCONCLUSIVE:ERROR_NOT_FOUND));
  C(pg_step==(i==46U?0U:i==47U?10U:2U));
 }
 for(i=30U;i<=37U;i++){
  pg_case=i;pg_wait_step=pg_lines=pg_carrier_calls=pg_clock_calls=pg_wait_calls=0;
  pg_root.configuration_commit.pending_commit.personal_native.scq_session.transport.event=i==31U?NULL:(HANDLE)1;
  result=papacc_server_launcher_scq_start_wait(i==30U?NULL:(HANDLE)2);
  expected=i<=31U?1U:i==32U?2U:i==33U?3U:i==34U?4U:i==35U?5U:i==36U?6U:0U;
  C(pg_wait_step==expected&&pg_lines==(expected?1U:0U));
  if(i<=31U)C(!pg_carrier_calls&&!pg_clock_calls&&!pg_wait_calls&&result==ERROR_ACCESS_DENIED);
  if(i==32U)C(pg_carrier_calls==1U&&!pg_clock_calls&&!pg_wait_calls&&result==ERROR_ACCESS_DENIED);
  if(i==33U)C(!pg_wait_calls&&result==ERROR_TIMEOUT);
  if(i==34U)C(result==ERROR_INVALID_STATE);if(i==35U)C(result==ERROR_TIMEOUT);if(i==36U)C(result==ERROR_PROCESS_ABORTED);if(i==37U)C(result==0);
 }
 for(i=40U;i<46U;i++){
  PAPACC_SCQ_TRANSPORT *t;PAPACC_SCQ_TRANSPORT before;
  static const DWORD operations[6]={3U,64U,2U,8U,16U,0U};char expected_op[48];
  memset(&pg_root,0,sizeof(pg_root));pg_case=i;pg_wait_step=pg_lines=pg_carrier_calls=pg_clock_calls=pg_wait_calls=0;
  t=&pg_root.configuration_commit.pending_commit.personal_native.scq_session.transport;
  t->event=(HANDLE)1;t->phase=i==40U?2U:1U;t->operation=operations[i-40U];t->pending=i==45U?0U:1U;
  t->offset=17U;t->total=i==41U?1100U:i==43U||i==44U?1092U:1052U;t->legs=i;
  t->connect_started=1U;t->query_refs3=1U;t->delivery_process=(HANDLE)(ULONG_PTR)0x12345678U;
  t->remote_files[0]=(HANDLE)(ULONG_PTR)0x23456789U;t->remote_files[2]=(HANDLE)(ULONG_PTR)0x34567890U;t->remote_query[1]=(HANDLE)(ULONG_PTR)0x45678901U;
  before=*t;result=papacc_server_launcher_scq_start_wait((HANDLE)2);
  C(result==ERROR_TIMEOUT&&pg_wait_step==5U&&pg_lines==1U);
  C(pg_carrier_calls==1U&&pg_clock_calls==1U&&pg_wait_calls==1U&&!memcmp(t,&before,sizeof(before)));
  sprintf_s(expected_op,sizeof(expected_op),"operation=%lu ",(unsigned long)operations[i-40U]);C(strstr(pg_last_line,expected_op));
  C(strstr(pg_last_line,"offset=17 ")&&strstr(pg_last_line,"delivery_bound=1 remote_files_mask=5 remote_query_mask=2 query_refs3=1"));
  C(!strstr(pg_last_line,"12345678")&&!strstr(pg_last_line,"23456789")&&!strstr(pg_last_line,"34567890")&&!strstr(pg_last_line,"45678901"));
  C(strstr(pg_last_line,"ZERO_NOT_PROOF; METADATA_NOT_AUTHORITY"));
 }
 puts("PASS START_PROGRESS_WAIT_NATIVE_BODIES cases=39 PACKET_SCALARS_NOT_HANDLE_VALUES NO_ADDITIONAL_QUERY_OR_MUTATION NO_SCM_NO_ENDPOINT_NO_START_NO_NATIVE_WAIT");return 0;
}
