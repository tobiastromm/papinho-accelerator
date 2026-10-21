#include "capability_negotiation.h"
PAPACC_RESULT papacc_capability_negotiator_init(PAPACC_CAPABILITY_NEGOTIATOR *n,
    PAPACC_U64 sid,PAPACC_CAPABILITY_KEY *ss,PAPACC_SIZE sc,
    PAPACC_CAPABILITY_KEY *ps,PAPACC_SIZE pc,PAPACC_SESSION_CAPABILITY_SNAPSHOT *snap)
{PAPACC_RESULT r;if(n==NULL||sid==0U||snap==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;if(n->initialized==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;r=papacc_capability_set_init(&n->client_supported,ss,sc);if(r!=PAPACC_RESULT_OK)return r;r=papacc_capability_set_init(&n->client_preference,ps,pc);if(r!=PAPACC_RESULT_OK)return r;n->session_instance_id=sid;n->snapshot=snap;n->initialized=PAPACC_TRUE;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_capability_negotiator_process(PAPACC_CAPABILITY_NEGOTIATOR *n,
    const PAPACC_U8 *in,PAPACC_SIZE len,const PAPACC_CAPABILITY_EVALUATION_INPUTS *base,
    PAPACC_CAPABILITY_KEY *effective_storage,PAPACC_SIZE effective_capacity,
    PAPACC_U8 *out,PAPACC_SIZE out_capacity,PAPACC_SIZE *written)
{PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;PAPACC_RESULT r;if(written!=NULL)*written=0U;if(n==NULL||n->initialized!=PAPACC_TRUE||base==NULL||out==NULL||written==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;if(n->snapshot->published==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 r=papacc_capability_negotiate_decode(in,len,&n->client_supported,&n->client_preference);if(r!=PAPACC_RESULT_OK)return r;inputs=*base;inputs.client_supported=&n->client_supported;inputs.client_preference=&n->client_preference;r=papacc_session_capability_snapshot_publish(n->snapshot,n->session_instance_id,effective_storage,effective_capacity,&inputs);if(r!=PAPACC_RESULT_OK)return r;r=papacc_capability_negotiate_result_encode(&n->snapshot->effective,out,out_capacity,written);if(r!=PAPACC_RESULT_OK){papacc_session_capability_snapshot_release(n->snapshot);return r;}return PAPACC_RESULT_OK;}
