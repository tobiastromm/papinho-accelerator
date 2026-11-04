static PAPACC_U32 publication_u32(const PAPACC_U8 *b){return ((PAPACC_U32)b[0]<<24)|((PAPACC_U32)b[1]<<16)|((PAPACC_U32)b[2]<<8)|b[3];}
static PAPACC_U64 publication_u64(const PAPACC_U8 *b){return ((PAPACC_U64)publication_u32(b)<<32)|publication_u32(b+4);}
/* SAME root owner/worker. No synchronous waits, signature, new authority or
 * optimistic acknowledgement. Phases 1 intention, 2 ack, 3 checkpoint. */
static PAPACC_ISSUER_RESULT issuer_publication_fail_v2(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,int stopped)
{o->issuer_port_pending.active=2U;o->configuration_commit.authority_lost=1;return stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_UNCERTAIN;}
static PAPACC_ISSUER_RESULT issuer_publication_poll_v2(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,int stopped)
{
 PAPACC_ISSUER_PENDING_STORE *p=&o->issuer_port_pending;PAPACC_CONFIGURATION_COMMIT *c=&o->configuration_commit;
 PAPACC_ISSUER_PUBLICATION_FACTS *f=&o->issuer_port_publication;
 PAPACC_ISSUER_CAS_RESULT outcome;PAPACC_RESULT result;PAPACC_ISSUER_LINK link;
 PAPACC_U8 image[8192];PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD record,wanted;
 PAPACC_SIZE pos,last=0;PAPACC_U64 revision;
 if(stopped)(void)papacc_configuration_stop(c);
 if(p->continuation==3U){
  result=papacc_server_launcher_configuration_checkpoint_poll(o,&outcome);
  if(c->pending_commit.active)return PAPACC_ISSUER_PENDING;
  if(stopped||result!=PAPACC_RESULT_OK||outcome!=PAPACC_CAS_SUCCESS||!issuer_root_live_ready(o)||
   c->committed_bytes!=16896U||c->pending_commit.next_bytes!=16896U||memcmp(c->committed,c->pending_commit.next,16896U)||
   papacc_configuration_checkpoint_data07(c,&link)!=PAPACC_RESULT_NOT_SUPPORTED||
   link.revision!=p->before.revision+(p->publish?2U:1U)||link.policy_epoch!=p->before.epoch||
   !link.administrative_epoch||link.administrative_epoch!=c->slots[c->current].context_epoch)
    return issuer_publication_fail_v2(o,stopped);
  if(p->publish){PAPACC_ISSUER_PUBLICATION_FACTS check;
   if(papacc_issuer_image_publication(c->committed+8192,p->next.request.id,&check,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||
    check.revision!=f->revision||check.issuer_revision!=f->issuer_revision||check.administrative_epoch!=f->administrative_epoch||
    check.policy_epoch!=f->policy_epoch||check.intent_ordinal!=f->intent_ordinal||memcmp(check.intent_digest,f->intent_digest,32)||memcmp(check.correlation,f->correlation,32))
     return issuer_publication_fail_v2(o,0);
  }
  p->active=0;return PAPACC_ISSUER_OK;
 }
 result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);
 if(c->pending_commit.active)return PAPACC_ISSUER_PENDING;
 revision=p->before.revision+(p->continuation==2U?2U:1U);
 if(stopped||result!=PAPACC_RESULT_OK||outcome!=PAPACC_CAS_SUCCESS||!issuer_root_live_ready(o)||
  o->issuer_projection_validated||c->committed_pending||c->prepared||c->unresolved||c->authority_lost||
  c->slots[c->current].view.revision!=revision||c->slots[c->current].issuer_revision!=revision||
  !c->slots[c->current].context_epoch||c->slots[c->current].context_epoch==o->issuer_port_previous_administrative_epoch||
  c->slots[c->current].operational_epoch!=p->before.epoch||!c->issuer_snapshot_valid[c->current]||
  c->pending_commit.next_bytes!=16384U||memcmp(c->committed,c->pending_commit.next,16384U)||
  papacc_configuration_issuer_copy06(c,image)!=PAPACC_RESULT_OK||memcmp(image,c->issuer_snapshots[c->current],8192)||
  papacc_issuer_image_validate(image,&v,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||v.revision!=revision||v.high_water!=p->high||
  papacc_issuer_image_find(image,p->next.request.id,&record,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK)
   return issuer_publication_fail_v2(o,stopped);
 wanted=p->next;
 if(p->publish&&p->continuation==1U)wanted.state=PAPACC_ISSUER_PUBLICATION_INTENT;
 else if(p->publish)wanted.published_epoch=f->administrative_epoch;
 if(!issuer_port_same_record(&wanted,&record))return issuer_publication_fail_v2(o,0);
 if(p->publish&&p->continuation==1U){
  for(pos=256;pos<publication_u32(image+12);pos+=publication_u32(image+pos+8))last=pos;
  if(!last||publication_u32(image+last+324)!=1U)return issuer_publication_fail_v2(o,0);
  f->revision=c->slots[c->current].view.revision;f->issuer_revision=v.revision;
  f->administrative_epoch=c->slots[c->current].context_epoch;f->policy_epoch=c->slots[c->current].operational_epoch;
  f->intent_ordinal=publication_u64(image+last);memcpy(f->intent_digest,image+last+publication_u32(image+last+8)-32,32);memcpy(f->correlation,image+last+404,32);
  if(papacc_issuer_image_ack(image,&p->next,f,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||
   papacc_server_launcher_configuration_issuer_prepare_live(o,image,&o->config,revision,p->high)!=PAPACC_RESULT_OK)
    return issuer_publication_fail_v2(o,0);
  o->issuer_port_previous_administrative_epoch=f->administrative_epoch;p->continuation=2U;
  result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);
  if(result!=PAPACC_RESULT_OK||outcome!=PAPACC_COMMIT_PENDING)return issuer_publication_fail_v2(o,0);
  return PAPACC_ISSUER_PENDING;
 }
 if(papacc_server_launcher_configuration_checkpoint_begin(o)!=PAPACC_RESULT_OK)return issuer_publication_fail_v2(o,0);
 p->continuation=3U;return PAPACC_ISSUER_PENDING;
}
static PAPACC_ISSUER_RESULT issuer_publication_confirm_v2(void *ctx,const PAPACC_ISSUER_VIEW *before,const PAPACC_ISSUER_RECORD *next,const PAPACC_ISSUER_VIEW *after,const PAPACC_ISSUER_RECORD *actual)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ISSUER_LINK link;PAPACC_ISSUER_PUBLICATION_FACTS f;
 PAPACC_ISSUER_RECORD expected;
 if(!preparation_owned(o)||!before||!next||!after||!actual)return PAPACC_ISSUER_DENIED;c=&o->configuration_commit;
 if(!issuer_root_live_ready(o)||o->issuer_port_pending.active||!o->issuer_port_pending.publish||o->issuer_port_pending.continuation!=3U||
  !issuer_port_same_view(before,&o->issuer_port_pending.before)||!issuer_port_same_record(next,&o->issuer_port_pending.next)||
  c->committed_bytes!=16896U||papacc_configuration_checkpoint_data07(c,&link)!=PAPACC_RESULT_NOT_SUPPORTED||
  link.revision!=after->revision||link.administrative_epoch!=after->administrative_epoch||link.policy_epoch!=after->epoch||after->epoch!=before->epoch||
  papacc_issuer_image_publication(c->committed+8192,next->request.id,&f,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||
  f.revision!=before->revision+1U||f.issuer_revision!=f.revision||f.policy_epoch!=before->epoch||after->revision!=f.revision+1U||
  f.administrative_epoch!=o->issuer_port_publication.administrative_epoch||f.administrative_epoch!=actual->published_epoch)return PAPACC_ISSUER_UNCERTAIN;
 expected=*next;expected.published_epoch=f.administrative_epoch;
 return issuer_port_same_record(&expected,actual)?PAPACC_ISSUER_OK:PAPACC_ISSUER_UNCERTAIN;
}
