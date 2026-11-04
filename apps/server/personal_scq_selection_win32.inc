/* Binary-only is private TRANSPORT_ONLY DATA, not a full installation target.
 * Keep full-layout callers intact and reject unused-field smuggling through
 * the existing binary_current guard. No cached/fixed path bypass. */
static DWORD personal_scq_paths_current(const PAPACC_PERSONAL_INSTALL_PATHS *paths,DWORD purpose,volatile LONG *stop)
{
 if(!paths)return ERROR_INVALID_PARAMETER;
 if(paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){
  PAPACC_SCQ_IMAGE_CHAIN *g=(PAPACC_SCQ_IMAGE_CHAIN *)paths->original_chain;
  if(!PAPACC_SCQ_EXISTING_PATH_PURPOSE(paths,purpose)||!g||g->paths!=paths||g->phase!=2U)return ERROR_ACCESS_DENIED;
  return scqi_chain_current(g,(PSID)g->plan.administrator.sid,stop);
 }
 if(paths->mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY)
  return purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_personal_paths_binary_current(paths,stop):ERROR_ACCESS_DENIED;
 return paths->mode==PAPACC_PERSONAL_PATHS_NEW?papacc_personal_paths_current(paths,stop):ERROR_INVALID_STATE;
}
static const PAPACC_SCQ_OWNER_DECISION *personal_scq_registered_owner_decision(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||
  o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return NULL;
 n=&o->pending_commit.personal_native;
 /* This lookup can only borrow an ALREADY acquired original. It neither
  * bootstraps scqa_capture from DATA nor restores a source after restart.
  * Start permission is further gated by scqa_start_selection. */
 if(n->scq_admin.self==&n->scq_admin&&n->scq_admin.controller==o&&
  n->scq_admin.registration&&!scqa_current(&n->scq_admin,o,&o->pending_commit.stop))return n->scq_admin.registration;
 if(n->scq_recipient.self==&n->scq_recipient&&n->scq_recipient.owner==o&&
  n->scq_recipient.registration&&!scqr_current(&n->scq_recipient,o,&n->scq_profile_data,
   &n->scq_inputs,&n->runtime_identity,&n->scq_dispatch,&o->pending_commit.stop))return n->scq_recipient.registration;
 return NULL;
}
/* Capture a query-only candidate from originals ALREADY acquired by this
 * native owner. This is neither registration by DATA nor operational approval.
 * The existing attributed SCQ and distinct Start confirmations remain required.
 * No public setter, argv/file grant loader or receiver reconstruction exists. */
static DWORD personal_scq_selection_capture(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *pending;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_OWNER_DECISION *r;FILETIME created,exited,kernel,user;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||
  o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return ERROR_INVALID_STATE;
 pending=&o->pending_commit;n=&pending->personal_native;r=&n->scq_selection;
 if(pending->stop||pending->active||pending->thread||pending->personal_ca||pending->personal_ca_candidate||
  n->scq_selection_captured||n->scq_admin.phase||!scqp_owned(&n->scq_profile_data,o)||
  n->scq_profile_data.role!=1U||n->scq_profile_data.phase!=1U||!n->scq_profile_data.pre_readiness||
  !n->scq_profile_data.waiting_runtime||scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p)||
  !PAPACC_SCQ_EXISTING_PATH_PURPOSE(&n->install_paths,p.purpose)||n->scq_profile_data.read_diagnostic!=n->install_paths.read_diagnostic||!scqp_unbound(&p.runtime)||!scqp_static(&p)||!scqp_window(&p)||
  !scqr_inputs_scope(&n->scq_inputs,&p)||!personal_token(pending,0)||
  n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||
  personal_scq_paths_current(&n->install_paths,p.purpose,&pending->stop)||personal_scq_inputs_check(&n->scq_inputs,n->sid,&pending->stop,0)||
  p.administrator.pid!=GetCurrentProcessId()||p.administrator.sid_bytes!=n->sid_bytes||memcmp(p.administrator.sid,n->sid,n->sid_bytes)||
  memcmp(p.administrator.token_id,&n->token.TokenId,sizeof(LUID))||memcmp(p.administrator.authentication,&n->token.AuthenticationId,sizeof(LUID))||
  memcmp(p.administrator.modified,&n->token.ModifiedId,sizeof(LUID))||
  !GetProcessTimes(GetCurrentProcess(),&created,&exited,&kernel,&user)||memcmp(&created,p.administrator.creation,sizeof(created)))return ERROR_ACCESS_DENIED;
 memset(r,0,sizeof(*r));r->binary=n->install_paths.binary;r->parent=n->install_paths.parent;r->data=n->install_paths.data;
 r->controller=o;r->source=&n->scq_admin;r->read_diagnostic=n->scq_profile_data.read_diagnostic;r->purpose=p.purpose;r->controller_pid=p.administrator.pid;
 memcpy(r->controller_creation,p.administrator.creation,sizeof(r->controller_creation));
 memcpy(r->controller_token,p.administrator.token_id,sizeof(r->controller_token));
 memcpy(r->controller_authentication,p.administrator.authentication,sizeof(r->controller_authentication));
 memcpy(r->controller_modified,p.administrator.modified,sizeof(r->controller_modified));
 memcpy(r->valid_from,p.valid_from,sizeof(r->valid_from));memcpy(r->valid_until,p.valid_until,sizeof(r->valid_until));
 memcpy(r->request,p.request,32);memcpy(r->decision,p.decision,32);memcpy(r->attempt,p.attempt,32);
 memcpy(r->policy,p.policy,32);memcpy(r->target,p.target,32);memcpy(r->image,p.refs[2].digest,32);
 memcpy(r->originals,p.refs,sizeof(r->originals));memcpy(r->administrator,p.administrator.sid,68);memcpy(r->runtime,p.runtime.sid,68);
 r->administrator_bytes=p.administrator.sid_bytes;r->runtime_bytes=p.runtime.sid_bytes;memcpy(r->key,p.key,sizeof(r->key));
 if(!scqa_scope(r,&p))return ERROR_ACCESS_DENIED;
 n->scq_selection_captured=1U;return 0; /* candidate only; no confirmation or consume */
}
