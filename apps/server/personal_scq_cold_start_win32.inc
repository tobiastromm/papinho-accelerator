/* Native acquisition for the cold controller. No file/argv grant is loaded.
 * The original files remain locked; the existing source and three native
 * confirmations supply attribution. All effectful APIs remain downstream. */
#include "personal_scq_cold_close_win32.inc"
static DWORD scqc_controller_variant(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *input,const PAPACC_SCQ_READ_COLD_INPUT *read)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA plan;DWORD e;
 if(!o||!input||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.stop)return ERROR_ACCESS_DENIED;
 n=&o->pending_commit.personal_native;
 if(n->phase||n->scq_profile_data.phase||n->scq_inputs.phase||n->scq_admin.phase||n->scq_session.phase||n->scq_cold_raw[0]||n->scq_cold_raw[1]||n->scq_cold_raw[2])return ERROR_INVALID_STATE;
 n->phase=1U;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)return ERROR_CANCELLED;
 e=read?scqc_plan_variant(o,input,&plan,read):scqc_plan(o,input,&plan);if(e)goto failed;
 if((read?papacc_configuration_personal_scq_profile_prepare_machine_read_data(o,1U,&plan,&plan.administrator):papacc_configuration_personal_scq_profile_prepare_transport_data(o,1U,&plan,&plan.administrator))!=PAPACC_RESULT_OK||
  papacc_configuration_personal_scq_session_acquire_originals(o,n->scq_cold_raw)!=PAPACC_RESULT_OK){e=ERROR_ACCESS_DENIED;goto failed;}
 e=scqc_close_raw(o);if(e)return e;
 if(papacc_configuration_personal_scq_start_plan_capture(o,&input->limits)!=PAPACC_RESULT_OK)return ERROR_ACCESS_DENIED;
 return papacc_configuration_personal_scq_first_controller(o);
failed:
 /* Known synchronous originals can close once even when preparation failed.
  * Pending acquisition/hash/descriptor work stays owned and UNKNOWN. Never
  * retry a failed close or dispose backing for an unresolved native call. */
 if(n->scq_inputs.checking||n->scq_inputs.content_hash.retained||n->scq_inputs.content_hash.active||n->scq_inputs.image_chain.phase==4U)o->unresolved=1;
 if(!o->unresolved){DWORD close_error=scqc_close_raw(o);if(close_error)return close_error;}
 return e;
}

DWORD papacc_configuration_personal_scq_cold_controller(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *input)
{return scqc_controller_variant(o,input,NULL);}
DWORD papacc_configuration_personal_scq_cold_machine_read_controller(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_READ_COLD_INPUT *input)
{
 static const char prefix[]="OWNER_SCQ_MACHINE_READ ",hex[]="0123456789abcdef";
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_READ_DIAGNOSTIC *k;HANDLE in,out;DWORD mode,e;unsigned i;char reply[sizeof(prefix)+64U];
 if(!input)return ERROR_INVALID_PARAMETER;e=scqc_controller_variant(o,&input->cold,input);if(e)return e;
 n=&o->pending_commit.personal_native;k=&n->scq_read_diagnostic;
 if(!personal_scq_admin_native_current(o))return ERROR_ACCESS_DENIED;
 e=scqk_prepare(k,o,&n->scq_admin,n->install_paths.read_public,&o->pending_commit.stop);if(e)return e;
 in=GetStdHandle(STD_INPUT_HANDLE);out=GetStdHandle(STD_OUTPUT_HANDLE);
 if(!GetConsoleMode(in,&mode)||!GetConsoleMode(out,&mode)||
 !personal_console_write(out,"ONE MACHINE_READ P ONLY: declare original legitimate administration and THIS TEST_ONLY fixture's K1 provenance, exact key/public identity in the approved original request. No create/adopt/sign/private-export/CA/CURRENT. E/Q/S do not approve P; actual bilateral/source/resource and positive administrative control remain pending gates.\r\n")||
 !personal_console_write(out,"KEY=")||!personal_console_write(out,input->key)||!personal_console_write(out,"\r\n")||
 !scqa_console_hex(out,"PUBLIC_SHA256=",k->key_public,32))return ERROR_ACCESS_DENIED;
 memcpy(reply,prefix,sizeof(prefix)-1U);for(i=0;i<32U;i++){reply[sizeof(prefix)-1U+2U*i]=hex[k->attempt[i]>>4];reply[sizeof(prefix)+2U*i]=hex[k->attempt[i]&15U];}reply[sizeof(reply)-1U]=0;
 if(!personal_console_write(out,reply)||!personal_console_write(out,"\r\n")||!personal_console_confirm(in,out,reply)||!personal_scq_admin_native_current(o))return ERROR_CANCELLED;
 e=scqk_confirm(k,o,reply,sizeof(reply)-1U,&o->pending_commit.stop);if(!e)n->scq_session.read_act=k;return e; /* not consumed until actual peer gates */
}
