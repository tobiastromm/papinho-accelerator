/* Actual native take-image body, original source/consumption state machine.
 * Token/file/resource/console edges are models; no SCM/endpoint/RNG/Start. */
#define PAPACC_FIRST_CONTROLLER_ENTRY preserved_take_first_main
#include "scq_first_controller_test.c"
#include <stdarg.h>
static unsigned tk_case,tk_guards,tk_resources,tk_files;static DWORD tk_step;
static PAPACC_SERVICE_IMAGE_VIEW tk_view;static BYTE tk_digest[32];static WCHAR tk_path[MAX_PATH];
static char tk_trace[4096];
static int tk_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 tk_guards++;
 if(!o||tk_case==2U||(tk_case==11U&&tk_guards==2U)||o->pending_commit.stop)return 0;
 return !scqa_current(&o->pending_commit.personal_native.scq_admin,o,&o->pending_commit.stop);
}
static DWORD tk_resource(PAPACC_CONFIGURATION_COMMIT *o,int creating)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;tk_resources++;
 if(creating!=1)return ERROR_INVALID_PARAMETER;
 if(tk_case==3U)return ERROR_NOT_ENOUGH_MEMORY;
 if(tk_case==4U)n->scq_profile_data.record[0]^=1U;
 if(tk_case==14U)o->pending_commit.stop=1;
 if(tk_case==15U)return ERROR_INVALID_DATA;
 if(tk_case==16U)return ERROR_NOT_ENOUGH_QUOTA;
 if(tk_case==17U)return ERROR_TIMEOUT;
 if(tk_case==18U)return ERROR_NOT_SUPPORTED;
 return 0;
}
static BOOL WINAPI tk_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION out)
{
 tk_files++;if(h!=(HANDLE)43)return FALSE;
 if(tk_case==5U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 *out=capture_selection_fixture.original.image_id;if(tk_case==6U)out->nFileIndexLow++;return TRUE;
}
static int tk_print(FILE *f,const char *format,...)
{
 char line[2048];va_list args;int n;unsigned long step;(void)f;
 va_start(args,format);n=_vsnprintf_s(line,sizeof(line),_TRUNCATE,format,args);va_end(args);
 if(sscanf_s(line,"SCQ_START_TAKE_FIRST_CAUSE step=%lu",&step)==1)tk_step=(DWORD)step;
 if(n>0&&strlen(tk_trace)+(size_t)n<sizeof(tk_trace))strcat_s(tk_trace,sizeof(tk_trace),line);
 return n;
}
#define personal_scq_admin_native_current tk_current
#define papacc_configuration_personal_scq_start_resource_current tk_resource
#undef GetFileInformationByHandle
#define GetFileInformationByHandle tk_file
#define fprintf tk_print
#include "../apps/server/personal_scq_start_take_win32.inc"
#undef fprintf
static int tk_setup(void)
{
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 SESSION_FIXTURE *f=&capture_selection_fixture;char reply[96];unsigned j;PAPACC_SCQ_PLAN_DATA plan;
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
 C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
 f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"";f->policy.data=L"";
 C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));
 memcpy(reply,prefix,sizeof(prefix)-1U);for(j=0;j<32U;j++){reply[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[sizeof(prefix)+2U*j]="0123456789abcdef"[f->plan.attempt[j]&15U];}reply[sizeof(prefix)-1U+64U]=0;
 C(!scqa_start_confirm(&f->source,&f->administrator_owner,reply,sizeof(prefix)-1U+64U,&f->stop));
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
 memset(o,0,sizeof(*o));o->writer=GetCurrentThreadId();n->scq_profile_data=f->controller_profile;n->scq_profile_data.self=&n->scq_profile_data;n->scq_profile_data.owner=o;
 n->scq_selection=f->policy;n->scq_selection.controller=o;n->scq_selection.source=&n->scq_admin;
 n->scq_admin=f->source;n->scq_admin.self=&n->scq_admin;n->scq_admin.controller=o;n->scq_admin.registration=&n->scq_selection;n->scq_admin.plan=&n->scq_profile_data;n->scq_admin.value.original=&n->scq_admin.value;n->scq_admin.value.controller=o;
 wcscpy_s(n->install_paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
 C(!scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&plan));
 memset(&tk_view,0,sizeof(tk_view));tk_view.handle=(HANDLE)43;tk_view.identity=f->original.image_id;tk_view.owner_thread=GetCurrentThreadId();
 wcscpy_s(tk_path,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator\\papacc_server.exe");memcpy(tk_digest,plan.refs[2].digest,32);tk_view.path=tk_path;tk_view.sha256=tk_digest;
 tk_guards=tk_resources=tk_files=0;tk_step=0;tk_trace[0]=0;return 0;
}
int main(void)
{
 unsigned i,before=checks;PAPACC_RESULT r;DWORD expected;
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<25U;i++){
  tk_case=0;C(!tk_setup());tk_case=i;expected=0;
  if(i==1U||i==19U||i==20U||i==21U)expected=2;
  if(i==2U||i==24U)expected=3;
  if(i==3U||i>=15U&&i<=18U)expected=4;
  if(i>=4U&&i<=10U)expected=i+1U;
  if(i==4U)expected=12U; /* decoder projects fields; source revalidation rejects changed version */
  if(i==11U||i==14U)expected=12;
  if(i==12U||i==13U||i==23U)expected=13;
  if(i==7U)tk_view.identity.nFileIndexLow++;
  if(i==8U)tk_digest[0]^=1U;
  if(i==9U){unsigned j;for(j=0;j<MAX_PATH-1U;j++)n->install_paths.binary[j]=L'x';n->install_paths.binary[MAX_PATH-1U]=0;}
  if(i==10U)tk_path[3]=L'X';
  if(i==12U){n->scq_admin.start_phase=3U;n->scq_admin.start_consumed=1;}
  if(i==13U)n->scq_admin.value.controller_consumed=0;
  if(i==19U)tk_view.sha256=NULL;
  if(i==20U)tk_view.owner_thread++;
  if(i==21U)tk_view.handle=NULL;
  if(i==23U){n->scq_admin.start_phase=0;n->scq_admin.start_consumed=0;}
  if(i==24U)receiver_now=n->scq_selection.valid_until[0];
  r=papacc_configuration_personal_scq_start_take_image(o,i==1U?NULL:&tk_view);
  if((expected&&tk_step!=expected)||(!expected&&r!=PAPACC_RESULT_OK))printf("MODEL_TAKE_DIAGNOSTIC case=%u result=%u step=%lu guards=%u resources=%u files=%u trace=%s\n",i,(unsigned)r,(unsigned long)tk_step,tk_guards,tk_resources,tk_files,tk_trace);
  if(expected){C(r!=PAPACC_RESULT_OK&&tk_step==expected);if(i!=12U)C(!n->scq_admin.start_consumed);}
  else {
   C(r==PAPACC_RESULT_OK&&!tk_step&&n->scq_admin.start_consumed&&tk_guards==2U&&tk_resources==1U&&tk_files==1U);
   if(i==22U){C(papacc_configuration_personal_scq_start_take_image(o,&tk_view)!=PAPACC_RESULT_OK&&tk_step==13U&&n->scq_admin.start_consumed);}
  }
  if(expected==2U)C(!tk_guards&&!tk_resources&&!tk_files);
  if(expected==3U)C(tk_guards==1U&&!tk_resources&&!tk_files);
  if(expected==4U||expected==5U)C(tk_resources==1U&&!tk_files);
  if(i==3U)C(strstr(tk_trace,"cause=8")!=NULL);
  C(!mcreated&&!mopened&&!mevents&&!cs_running);
 }
 printf("PASS START_TAKE_NATIVE_BODY cases=25 checks=%u ORIGINAL_SOURCE_CONSUMPTION_PRESERVED ALL_FILE_RESOURCE_SOURCE_BOUNDARIES_MODELED NO_ENDPOINT_NO_SCM_NO_START\n",checks-before);
 return 0;
}
