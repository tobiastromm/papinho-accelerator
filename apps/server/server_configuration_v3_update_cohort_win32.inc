/* Same canonical owner and existing G7 slots. Unresolved lifetime only. */
PAPACC_RESULT papacc_configuration_v3_update_hold_data(PAPACC_CONFIGURATION_COMMIT *o,
 PAPACC_CONFIG_PIN *pin,const PAPACC_U8 *image,PAPACC_SIZE n,PAPACC_SIZE reserved,PAPACC_U64 invocation)
{
 PAPACC_CONFIG_V3_BORROW base;PAPACC_CONFIG_V3_BACKING *candidate;PAPACC_CONFIG_SLOT *slot;
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_U8 digest[32];PAPACC_SIZE q,off;unsigned index,j;
 if(!owner(o)||!pin||!v3b_range(image,n)||n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX||!invocation||
 issuer06_overlap(o,sizeof(*o),image,n)||issuer06_overlap(o,sizeof(*o),pin,sizeof(*pin))||
 issuer06_overlap(pin,sizeof(*pin),image,n))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(reserved<sizeof(*o)||reserved>PAPACC_ED11_PROCESS_LIMIT||
 n+sizeof(base)+sizeof(digest)>PAPACC_ED11_PROCESS_LIMIT-reserved)return PAPACC_RESULT_LIMIT_EXCEEDED;
 if(o->store02!=8||PAPACC_V3_UPDATE_BUSY(o)||o->prepared||o->committed_pending||o->stopped||o->unresolved||
 o->authority_lost||o->issuer_stage_prepared||o->recovery_selected||o->recovery_guard||pending_blocks(o)||
 o->pending_commit.thread||o->pending_commit.transaction||(o->pending_commit.file&&o->pending_commit.file!=INVALID_HANDLE_VALUE)||o->pending_commit.close_error||
 o->pending_commit.packet_format||o->pending_commit.result!=PAPACC_COMMIT_IDLE||o->pending_commit.previous_bytes||o->pending_commit.next_bytes)
 return PAPACC_RESULT_INVALID_STATE;
 if(papacc_configuration_v3_checkpoint_match_data(o,pin)!=PAPACC_RESULT_OK||
 papacc_configuration_v3_borrow_data(o,pin,&base)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 /* Bounds and correlation only, not the component/source validators. */
 q=v3b32(image+56);off=v3b32(image+48);
 if(memcmp(image,"PACROOT8",8)||image[8]!=3U||image[9]||v3b32(image+12)!=n||q!=n-544U||
 off>q||q-off!=16384U||memcmp(image+72,base.packet+72,32)||
 base.revision==(PAPACC_U64)-1||v3b64(image+104)!=base.revision+1U||
 v3b64(image+112)!=base.revision||v3b64(image+136)!=base.revision||
 v3b64(image+144)!=o->slots[pin->index].context_epoch||v3b64(image+152)!=o->slots[pin->index].operational_epoch||
 v3b64(image+160)!=base.issuer_revision||v3b64(image+120)!=base.issuer_revision||
 memcmp(image+176,base.packet+v3b32(base.packet+56)+88,32)||
 memcmp(image+208,base.packet+base.packet_bytes-32U,32)||
 memcmp(image+off,base.packet+v3b32(base.packet+48),16384U)||
 !store03_zero(image+q,512U)||!papacc_configuration_issuer_hash06(o,image,n-32U,digest)||memcmp(digest,image+n-32U,32))
 return PAPACC_RESULT_INVALID_ARGUMENT;
 /* Hash/native work has returned; no pending child borrows the inputs. */
 EnterCriticalSection(&o->lock);u=&o->v3_update;
 if(PAPACC_V3_UPDATE_BUSY(o)||o->prepared||o->stopped||o->unresolved||o->authority_lost||
 pending_blocks(o)||pin->index!=o->current||o->next_serial==(PAPACC_U64)-1||
 o->committed_bytes!=base.packet_bytes||memcmp(o->committed,base.packet,base.packet_bytes)){
 LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 for(j=0;j<32U;j++)if(o->tickets[j].ticket==pin->ticket&&o->tickets[j].serial==pin->serial&&o->tickets[j].index==pin->index)break;
 if(j==32U||!pin->ticket||o->slots[pin->index].serial!=pin->serial||!o->slots[pin->index].readers){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 for(index=0;index<PAPACC_CONFIG_SLOTS;index++)if(index!=o->current&&!o->slots[index].readers&&!o->slots[index].retained)break;
 if(index==PAPACC_CONFIG_SLOTS){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_LIMIT_EXCEEDED;}
 slot=&o->slots[index];candidate=&o->v3_backings[index];
 memset(candidate,0,sizeof(*candidate));memcpy(candidate->packet,image,n);candidate->packet_bytes=n;
 memcpy(candidate->ca,base.ca,base.ca_bytes);candidate->ca_bytes=base.ca_bytes;
 /* serial stays ZERO: unresolved candidate cannot be a G7 exported borrow. */
 memset(&slot->view,0,sizeof(slot->view));memcpy(slot->view.domain,image+72,32);
 slot->view.revision=base.revision+1U;slot->view.predecessor=base.revision;
 slot->context_epoch=0;slot->operational_epoch=0;slot->issuer_revision=0;
 slot->serial=++o->next_serial;o->issuer_snapshot_valid[index]=0;
 u->predecessor_pin=*pin;memset(pin,0,sizeof(*pin));u->candidate_serial=slot->serial;
 u->candidate_index=index;u->prior_pending=o->pending;u->invocation=invocation;u->reserved=reserved;
 memcpy(o->pending_commit.previous,base.packet,base.packet_bytes);o->pending_commit.previous_bytes=(DWORD)base.packet_bytes;
 o->pending=index;o->prepared=1;u->phase=PAPACC_V3_UPDATE_HELD;
 LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_v3_update_abort_data(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_CONFIG_SLOT *s;PAPACC_RESULT result;
 if(!owner(o)||!PAPACC_V3_UPDATE_BUSY(o))return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&o->lock);u=&o->v3_update;
 /* This phase starts no child work and exports no candidate borrow. Still
  * prove quiescence/slot identity; absence of a lease or timeout is not proof. */
 if(u->source_ticket||o->pending_commit.active||o->pending_commit.thread||
 o->pending_commit.transaction||(o->pending_commit.file&&o->pending_commit.file!=INVALID_HANDLE_VALUE)||
 o->pending_commit.close_error||o->unresolved||o->committed_pending||o->authority_lost||
 o->pending_commit.result!=PAPACC_COMMIT_IDLE||o->pending_commit.packet_format){
 u->phase=PAPACC_V3_UPDATE_RETAINED;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 if(u->candidate_serial){
  if(u->candidate_index>=PAPACC_CONFIG_SLOTS||u->candidate_index==o->current){u->phase=PAPACC_V3_UPDATE_RETAINED;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
  s=&o->slots[u->candidate_index];
  if(!o->prepared||o->pending!=u->candidate_index||s->serial!=u->candidate_serial||s->readers||s->retained||o->v3_backings[u->candidate_index].serial){
   u->phase=PAPACC_V3_UPDATE_RETAINED;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 }
 result=papacc_configuration_unpin(o,&u->predecessor_pin,0);
 if(result!=PAPACC_RESULT_OK){u->phase=PAPACC_V3_UPDATE_RETAINED;LeaveCriticalSection(&o->lock);return result;}
 if(u->candidate_serial){
  memset(&o->v3_backings[u->candidate_index],0,sizeof(o->v3_backings[0]));
  memset(&o->slots[u->candidate_index],0,sizeof(o->slots[0]));
  memset(o->pending_commit.previous,0,o->pending_commit.previous_bytes);o->pending_commit.previous_bytes=0;
  o->pending=u->prior_pending;o->prepared=0;
 }
 memset(u,0,sizeof(*u));LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
