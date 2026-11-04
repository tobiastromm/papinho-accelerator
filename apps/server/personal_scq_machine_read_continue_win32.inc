/* Bound to native originals by transport_continue before entry. One pipe,
 * one P, original fixture identity and existing receipt/close continuation. */
static DWORD scqkm_admin_control(PAPACC_MACHINE_FIXTURE_CONTROL *f,PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PSID administrator,PSID runtime,volatile LONG *stop)
{
 BYTE a[16],g[16];unsigned i,j;DWORD e;SECURITY_STATUS s;
 if(scqk_current(k,owner,stop)||scqkr_current(c,owner,&k->source->value,stop)||k->phase!=3U||!k->consumed)return ERROR_ACCESS_DENIED;
 if(f->phase==2U)return scqkf_identity_current(f,owner,stop);
 if(f->phase||f->self||f->key.diagnostic_only||f->key.state)return ERROR_INVALID_STATE;
 for(i=0;i<2U;i++)for(j=0;j<16U;j++){
  BYTE value=0;unsigned z;for(z=0;z<2U;z++){WCHAR ch=k->key[22U+i*33U+2U*j+z];if(!((ch>=L'0'&&ch<=L'9')||(ch>=L'a'&&ch<=L'f')))return ERROR_INVALID_NAME;value=(BYTE)((value<<4)|(ch<=L'9'?ch-L'0':ch-L'a'+10));}
  (i?g:a)[j]=value;
 }
 e=papacc_ca_key_machine_read_bind(&f->key,c,k,owner,stop);if(e)return e;
 s=papacc_ca_key_fixture_read_open(&f->key,a,g,administrator,runtime);if(s)return (DWORD)s;
 if(wcscmp(f->key.name,k->key))return ERROR_REVISION_MISMATCH;
 e=scqkf_read_identity(f,owner,k,stop);if(e)return e;
 if(memcmp(f->public_id,k->key_public,32)||scqk_current(k,owner,stop)||scqkr_current(c,owner,&k->source->value,stop))return ERROR_CRC;
 return 0;
}
/* Snapshot original return and decoded result only. No query, I/O, clock,
 * handle, identity, key name or public/private blob is acquired or printed. */
static DWORD scqkm_close_result(DWORD e,unsigned role,const PAPACC_PERSONAL_CNG_PROBE *p,unsigned control)
{
 DWORD result=0;unsigned checked=0;
 if(!e&&role==1U){checked=1U;result=scqk_result(p,control);if(result!=1U)e=control&&papacc_personal_cng_inconclusive(p)?ERROR_NOT_FOUND:ERROR_ACCESS_DENIED;}
 if(e&&e!=ERROR_IO_PENDING&&e!=ERROR_MORE_DATA)
  (void)fprintf(stderr,"SCQ_READ_CLOSE_RESULT_FIRST_CAUSE origin=%u error=%lu result_checked=%u result=%lu control=%u probe_present=%u phase=%lu classification=%lu provider_calls=%u provider_status=0x%08lx open_calls=%u open_status=0x%08lx first_stage=%lu first_origin=%lu first_error=0x%08lx close_attempted=%u close_calls=%u close_error=0x%08lx provider_present=%u key_present=%u thread_present=%u; ORIGINAL_SNAPSHOTS_ONLY_NO_NEW_QUERY_NO_AUTHORITY\n",
   checked?2U:1U,(unsigned long)e,checked,(unsigned long)result,control,p?1U:0U,
   p?(unsigned long)p->phase:0UL,p?(unsigned long)p->classification:0UL,p?p->provider_calls:0U,p?(unsigned long)p->provider_result:0UL,p?p->open_calls:0U,p?(unsigned long)p->open_result:0UL,
   p?(unsigned long)p->first_stage:0UL,p?(unsigned long)p->first_origin:0UL,p?(unsigned long)p->first_error:0UL,p?p->close_attempted:0U,p?p->close_calls:0U,p?(unsigned long)p->close_error:0UL,p&&p->provider?1U:0U,p&&p->key?1U:0U,p&&p->unexpected_thread?1U:0U);
 return e;
}
/* Query the SAME retained originals after submission; no PID reopen and no
 * replacement of the live-peer gate for any new effect. Exit is not identity. */
static DWORD scqkm_retained_peer(PAPACC_SCQ_SESSION_DRIVER *c,const PAPACC_SCQ_PLAN_DATA *p)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY *r=c->runtime;TOKEN_STATISTICS observed;DWORD logon=0,e;
 FILETIME created,ended,kernel,user;
 if(!r||r->thread!=GetCurrentThreadId()||r->phase!=1U||r->first_error||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||
  r->closed_preparation!=1U||r->read_diagnostic!=1U||r->transport_only||!r->process||!r->token||!c->transport.delivery_process||
  r->pid!=p->runtime.pid||memcmp(&r->creation,p->runtime.creation,sizeof(FILETIME))||
  memcmp(&r->statistics.TokenId,p->runtime.token_id,sizeof(LUID))||memcmp(&r->statistics.AuthenticationId,p->runtime.authentication,sizeof(LUID))||
  memcmp(&r->statistics.ModifiedId,p->runtime.modified,sizeof(LUID)))return ERROR_ACCESS_DENIED;
 if(GetProcessId(r->process)!=r->pid||!GetProcessTimes(r->process,&created,&ended,&kernel,&user)||memcmp(&created,&r->creation,sizeof(created))||
  GetProcessId(c->transport.delivery_process)!=r->pid||!GetProcessTimes(c->transport.delivery_process,&created,&ended,&kernel,&user)||memcmp(&created,&r->creation,sizeof(created)))return ERROR_ACCESS_DENIED;
 e=pri_query(r,r->token,&observed,&logon);if(e)return e;
 if(!logon||memcmp(&observed.TokenId,&r->statistics.TokenId,sizeof(LUID))||memcmp(&observed.AuthenticationId,&r->statistics.AuthenticationId,sizeof(LUID))||
  memcmp(&observed.ModifiedId,&r->statistics.ModifiedId,sizeof(LUID)))return ERROR_ACCESS_DENIED;
 return 0; /* continuity of held originals, not peer liveness/readiness */
}
/* I2/I4: observe ONLY the final write already authorized under scqt_current.
 * No peer acquisition, new authority, reconnect or post-exit write is allowed.
 * A new submission below still goes through the unchanged live native gate. */
static DWORD scqkm_return_inconclusive(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_READ_DIAGNOSTIC *k,
 PAPACC_MACHINE_FIXTURE_CONTROL *fixture,PAPACC_PERSONAL_CNG_PROBE *probe,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;PAPACC_SCQ_PLAN_DATA p;DWORD e,flags;unsigned i,j;ULONGLONG value,ids[2];
 if(!c||!stop||!fixture||!k||!scqt_owned(c,owner)||c->role!=1U||c->phase!=4U||c->first||c->peer_lost||
  !c->profile||c->profile->read_diagnostic!=1U||!c->profile->pre_readiness||!c->dispatch||c->dispatch->purpose!=PAPACC_SCQ_PURPOSE_READ||
  c->source!=k->source||c->read_act!=k||k->phase!=3U||!k->consumed||fixture->phase!=2U||!fixture->key.diagnostic_control_confirmed||
  !papacc_personal_cng_inconclusive(probe)||!scqt_read_result_closed(c))return ERROR_ACCESS_DENIED;
 t=&c->transport;
 if(t->phase!=1U||t->first||t->cancelled||!t->read_diagnostic||!t->query_refs3||t->query_only||
  !t->close_accepted||!(t->legs&8U)||t->original!=&k->source->value||!t->original||t->original->original!=t->original||
  t->original->controller!=owner||!t->original->controller_consumed||t->original->retained)return ERROR_ACCESS_DENIED;
 e=scqk_current(k,owner,stop);if(e)return e;
 e=scqt_resource_effect(c,owner,stop,0);if(e)return e;
 if(scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||p.purpose!=PAPACC_SCQ_PURPOSE_READ)return ERROR_ACCESS_DENIED;
 e=scqkm_retained_peer(c,&p);if(e)return e;
 if(!GetHandleInformation(t->pipe,&flags)||(flags&HANDLE_FLAG_INHERIT)||!GetHandleInformation(t->event,&flags)||(flags&HANDLE_FLAG_INHERIT))return ERROR_INVALID_HANDLE;
 if(t->legs&16U)return t->pending||t->operation||t->total?ERROR_INVALID_STATE:ERROR_NOT_FOUND;
 if(!t->operation){
  if(t->pending||t->total||t->offset)return ERROR_INVALID_STATE;
  /* This is the ONLY submission branch; scqt_start_io authenticates the live
   * original runtime before WriteFile. Never entered from a pending tail. */
  e=scqt_start_io(c,owner,16U,&p.administrator,&p.runtime,p.refs,t->original,stop);
  if(e)return e;
 }else{
  if(t->operation!=16U||t->total!=scqt_close_size(t)||t->offset>t->total||t->ov.hEvent!=t->event)return ERROR_INVALID_STATE;
  if(t->pending){e=scqt_poll(c,owner);if(e)return e;}
 }
 /* Local completion proves neither peer liveness nor global close. It only
  * completes this original write; terminal exit/SCM/owner close are separate. */
 if(t->pending||t->operation!=16U||t->total!=scqt_close_size(t)||t->offset!=t->total)return scqt_fail(c,ERROR_IO_INCOMPLETE);
 if(memcmp(c->buffer,"SQC3",4)||memcmp(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES))return scqt_fail(c,ERROR_INVALID_DATA);
 ids[0]=(ULONGLONG)(ULONG_PTR)t->remote_query[0];ids[1]=(ULONGLONG)(ULONG_PTR)t->remote_query[1];
 if(!scqt_query_ids(c->buffer,PAPACC_SCQ_CLOSE_BYTES,ids))return scqt_fail(c,ERROR_INVALID_DATA);
 for(i=0;i<3U;i++){
  value=0;for(j=0;j<8U;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]<<(8U*j);
  if(!t->remote_files[i]||value!=(ULONGLONG)(ULONG_PTR)t->remote_files[i])return scqt_fail(c,ERROR_INVALID_DATA);
 }
 t->legs|=16U;t->operation=t->total=t->offset=0;
 return ERROR_NOT_FOUND; /* local final write complete; NEVER READ PASS */
}
static DWORD scqkm_continue_session(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_SCQ_READ_RECIPIENT *r,PAPACC_SCQ_RECIPIENT_ORIGIN *origin,PAPACC_PERSONAL_SCQ_INPUTS *held_inputs,PAPACC_MACHINE_FIXTURE_CONTROL *fixture,PAPACC_PERSONAL_CNG_PROBE *probe,const BYTE public_id[32],PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e;
 if(!c->profile->read_diagnostic||!t->read_diagnostic||!t->query_refs3)return ERROR_ACCESS_DENIED;
 if(c->role==1U){
  if(!(t->legs&SCQK_READY_READ)){e=scqkr_ready_io(c,owner,k,NULL,proof,stop);return e?e:ERROR_MORE_DATA;}
  if(k->phase==2U){e=scqkr_current(c,owner,proof,stop);if(e)return e;e=scqk_take(k,owner,stop);if(e)return e;}
  if(!(t->legs&SCQK_P_SEND)){
   e=scqkm_admin_control(fixture,k,c,owner,(PSID)k->source->value.operator_sid,(PSID)k->source->registration->runtime,stop);if(e)return e;
   if(!t->operation){e=scqkr_projection(k,c,owner,stop);if(e)return e;}
   e=scqkr_io(c,owner,SCQK_P_SEND,proof,k,NULL,stop);if(e)return e;
   e=scqkr_finish_send(c,owner,SCQK_P_SEND,proof,stop);return e?e:ERROR_MORE_DATA;
  }
  if(!(t->legs&SCQK_RESULT_READ)){
   e=scqkf_identity_current(fixture,owner,stop);if(e)return e;
   e=scqkr_io(c,owner,SCQK_RESULT_READ,proof,k,NULL,stop);if(e)return e;
   e=scqkr_result_accept(k,c,owner,probe,stop);return e?e:ERROR_MORE_DATA;
  }
 }else{
  if(!r->phase){e=scqkr_prepare(r,owner,origin,c,held_inputs,public_id,stop);return e?e:ERROR_MORE_DATA;}
  if(!(t->legs&SCQK_READY_SEND)){e=scqkr_ready_io(c,owner,NULL,r,proof,stop);return e?e:ERROR_MORE_DATA;}
  if(r->phase==1U){e=scqkr_io(c,owner,SCQK_P_READ,proof,NULL,r,stop);if(e)return e;e=scqkr_accept(r,owner,stop);return e?e:ERROR_MORE_DATA;}
  if(r->phase==2U){e=scqkr_probe(r,owner,probe,stop);return e?e:ERROR_MORE_DATA;}
  if(!(t->legs&SCQK_RESULT_SEND)){
   if(!t->operation){e=scqkr_result_ready(r,owner,stop);if(e)return e;}
   e=scqkr_io(c,owner,SCQK_RESULT_SEND,proof,NULL,r,stop);if(e)return e;
   e=scqkr_finish_send(c,owner,SCQK_RESULT_SEND,proof,stop);return e?e:ERROR_MORE_DATA;
  }
 }
 if(c->role==1U&&c->phase==4U&&t->close_accepted&&(t->legs&8U)&&fixture->key.diagnostic_control_confirmed&&papacc_personal_cng_inconclusive(probe))
  return scqkm_return_inconclusive(c,owner,k,fixture,probe,stop);
 e=scqr_continue_transport(c,owner,c->role==2U?origin:NULL,c->role==2U?held_inputs:NULL,proof,stop);
 return scqkm_close_result(e,c->role,probe,fixture->key.diagnostic_control_confirmed);
}
