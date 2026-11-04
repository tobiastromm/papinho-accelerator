static int personal_native_ceremony_ready(const PAPACC_CONFIG_PENDING *p)
{(void)p;return 0; /* DATA sequencing never establishes native admissibility. */}
/* Same root/packet, DATA only. Never establishes native eligibility, parent
 * identity, CSPRNG provenance or permission for a worker/custody operation. */
PAPACC_RESULT papacc_configuration_personal_data_capture(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char id[32],const unsigned char *plan,PAPACC_SIZE bytes)
{
 unsigned i;
 if(!owner(o)||o->stopped||o->unresolved||o->published||o->prepared||o->committed_pending||
 o->committed_bytes||o->recovery_selected||o->recovery_guard||pending_blocks(o)||o->pending_commit.thread)
 return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<PAPACC_CONFIG_SLOTS;i++)if(o->slots[i].readers||o->slots[i].retained||o->issuer_snapshot_valid[i])return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_data_capture(&o->pending_commit.personal_data,id,plan,bytes)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_ARGUMENT;
}
PAPACC_RESULT papacc_configuration_personal_data_retire(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;
 if(!owner(o))return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;
 /* The DATA object never acquires a borrow. Still deny erasure while any
  * canonical worker/native owner can be pending or retained. */
 if((p->personal_native.scq_recipient.phase&&p->personal_native.scq_recipient.phase!=3U)||(p->personal_native.scq_session.phase&&p->personal_native.scq_session.phase!=5U)||(p->personal_native.scq_admin.phase&&p->personal_native.scq_admin.phase!=5U)||(p->personal_native.scq_profile_data.phase&&p->personal_native.scq_profile_data.phase!=3U)||(p->personal_native.scq_inputs.phase&&p->personal_native.scq_inputs.phase!=3U)||(p->personal_native.scq_receiver.original||p->personal_native.scq_receiver.phase==4U)||(p->personal_native.scq_dispatch.phase&&p->personal_native.scq_dispatch.phase!=3U)||(p->personal_native.runtime_cng_probe.phase&&p->personal_native.runtime_cng_probe.phase!=3U)||(p->personal_native.runtime_identity.phase&&p->personal_native.runtime_identity.phase!=3U)||p->personal_ca||p->personal_ca_candidate||(p->personal_native.phase&&p->personal_native.phase!=3U)||papacc_store_hash_blocked(&o->store_hash)||papacc_store_hash_blocked(&p->store_hash)||p->active||p->thread||p->bootstrap_parent||p->bootstrap_container||p->bootstrap_token||
 p->transaction||(p->file&&p->file!=INVALID_HANDLE_VALUE)||o->unresolved||
 o->retained_transaction||(o->retained_file&&o->retained_file!=INVALID_HANDLE_VALUE))return PAPACC_RESULT_INVALID_STATE;
 papacc_personal_data_invalidate(&p->personal_data);
 return papacc_personal_data_retire(&p->personal_data)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}


/* Prepare requested effects BEFORE presentation, in the SAME owned packet.
 * CA IDs/service bytes remain requested DATA, never factual CA custody. */
PAPACC_RESULT papacc_configuration_personal_effect_plan_data(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char authority[16],const unsigned char generation[16],const unsigned char *service,PAPACC_SIZE service_bytes)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC v;
 unsigned char parent[12];DWORD values[3];unsigned i,j;size_t bytes;
 if(!owner(o)||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(p->active||p->thread||p->bootstrap_created||p->bootstrap_container||n->phase!=1U||
    p->personal_data.phase!=PAPACC_PERSONAL_DATA_CAPTURED||p->personal_data.effectful||p->personal_data.consumed||
    !n->plan_leaf[0]||!memchr(n->plan_leaf,0,sizeof(n->plan_leaf))||
    memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))||
    !authority||!generation||!service||!service_bytes||service_bytes>68U)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_configuration_personal_native_current(o)!=PAPACC_RESULT_OK||
    papacc_configuration_personal_source_current(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(n->runtime_identity.phase&&(n->runtime_identity.phase!=1U||service_bytes!=n->runtime_identity.sid_bytes||
    memcmp(service,n->runtime_identity.sid,service_bytes)||papacc_configuration_personal_runtime_current(o)!=PAPACC_RESULT_OK))return PAPACC_RESULT_INVALID_STATE;
 values[0]=n->parent.dwVolumeSerialNumber;values[1]=n->parent.nFileIndexHigh;values[2]=n->parent.nFileIndexLow;
 for(i=0;i<3U;i++)for(j=0;j<4U;j++)parent[i*4U+j]=(unsigned char)(values[i]>>(j*8U));
 memset(&v,0,sizeof(v));v.operator_id=n->sid;v.operator_bytes=n->sid_bytes;v.parent_id=parent;
 v.leaf=(const unsigned char *)n->plan_leaf;v.leaf_bytes=strlen(n->plan_leaf);
 v.authority=authority;v.generation=generation;v.service_id=service;v.service_bytes=service_bytes;
 v.configuration=p->next;v.configuration_bytes=p->personal_data.bytes;
 bytes=papacc_personal_effect_build(&v,p->previous+1024U,PAPACC_PERSONAL_PLAN_MAX);
 if(!bytes||!papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024U,bytes))return PAPACC_RESULT_INVALID_ARGUMENT;
 return PAPACC_RESULT_OK; /* no CA acquisition, authorization or effect */
}
