/* Same-owner synchronous DATA window; no published candidate pin/lease. */
PAPACC_RESULT papacc_configuration_candidate_borrow_data(PAPACC_CONFIGURATION_COMMIT *o,
 PAPACC_U64 invocation,PAPACC_U64 serial,PAPACC_CONFIG_V3_BORROW *out)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_CONFIG_V3_BACKING *b;PAPACC_CONFIG_V3_BORROW v,base;PAPACC_U8 digest[32];
 PAPACC_RESULT result=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!out||!v3b_range(out,sizeof(*out))||issuer06_overlap(o,sizeof(*o),out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);u=&o->v3_update;
 if(!u->data_active||!invocation||invocation!=u->invocation||!serial||serial!=u->candidate_serial||!v3act_ready(o))goto done;
 if(backing_borrow_data_version(o,&u->predecessor_pin,&base,o->v3_backings[o->current].packet[8])!=PAPACC_RESULT_OK)goto done;
 b=&o->v3_backings[u->candidate_index];
 if(!b->ca_bytes||b->ca_bytes>PAPACC_CONFIG_V3_CA_BYTES||
 !papacc_configuration_storage_hash(o,b->packet,b->packet_bytes,digest)||memcmp(digest,b->packet_hash,32)||
 !papacc_configuration_storage_hash(o,b->ca,b->ca_bytes,digest)||memcmp(digest,b->ca_hash,32))goto done;
 /* Hold bounds are rechecked before any derived pointer. Root codec validates
  * these offsets before invoking a component; this function also bounds them. */
 if(v3b32(b->packet+48)>b->packet_bytes||b->packet_bytes-v3b32(b->packet+48)<16384U||
 v3b32(b->packet+244)>b->packet_bytes||v3b32(b->packet+248)>b->packet_bytes-v3b32(b->packet+244)||
 v3b32(b->packet+260)>b->packet_bytes||v3b32(b->packet+264)>b->packet_bytes-v3b32(b->packet+260))goto done;
 memset(&v,0,sizeof(v));v.packet=b->packet;v.packet_bytes=b->packet_bytes;v.ca=b->ca;v.ca_bytes=b->ca_bytes;
 v.snt=b->packet+v3b32(b->packet+244);v.snt_bytes=v3b32(b->packet+248);
 v.pdg=b->packet+v3b32(b->packet+260);v.pdg_bytes=v3b32(b->packet+264);
 v.issuer_image=b->packet+v3b32(b->packet+48)+8192U;
 v.revision=v3b64(b->packet+104);v.issuer_revision=v3b64(b->packet+120);*out=v;result=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return result;
}
PAPACC_RESULT papacc_configuration_prepared_storage_validate_data(PAPACC_CONFIGURATION_COMMIT *o,
 PC_ROOT8_COMPONENT component,void *context)
{
 PAPACC_CONFIG_V3_BORROW b,check;PAPACC_V3_UPDATE_COHORT *u;PAPACC_RESULT result;PC_ROOT8_RESULT data;
 if(!owner(o)||!component||!v3act_ready(o))return PAPACC_RESULT_INVALID_STATE;
 u=&o->v3_update;if(u->data_active)return PAPACC_RESULT_INVALID_STATE;u->data_active=1;
 result=papacc_configuration_candidate_borrow_data(o,u->invocation,u->candidate_serial,&b);
 if(result==PAPACC_RESULT_OK){
  data=b.packet[8]==3U?pc_root8_v3_validate(b.packet,b.packet_bytes,papacc_configuration_storage_hash,o,component,context):
   pc_root8_v4_validate(b.packet,b.packet_bytes,papacc_configuration_storage_hash,o,component,context);
  if(data!=PC_ROOT8_OK)result=PAPACC_RESULT_INVALID_STATE;
  if(result==PAPACC_RESULT_OK)result=papacc_configuration_candidate_borrow_data(o,u->invocation,u->candidate_serial,&check);
 }
 u->data_active=0;return result; /* DATA only: phase HELD, status SOURCE_ABSENT. */
}
