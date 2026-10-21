#include "egress_runtime_config.h"
PAPACC_RESULT papacc_egress_runtime_resolve(const PAPACC_EGRESS_RUNTIME_CONFIG*c,
 const PAPACC_NETWORK_ENDPOINT*endpoint,PAPACC_NETWORK_TRANSPORT_MODE mode,
 const PAPACC_EGRESS_PROFILE**out){PAPACC_U32 id=0U;PAPACC_SIZE i;PAPACC_RESULT r;
 if(out!=NULL)*out=NULL;if(c==NULL||endpoint==NULL||out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(c->allow_network_egress!=PAPACC_TRUE||c->policy==NULL||c->profiles==NULL||
    c->profile_count==0U||c->select_profile==NULL)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_egress_endpoint_allowed(c->policy,endpoint)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 r=c->select_profile(c->select_profile_context,endpoint,mode,&id);
 if(r!=PAPACC_RESULT_OK||id==0U)return r==PAPACC_RESULT_OK?PAPACC_RESULT_INVALID_STATE:r;
 for(i=0U;i<c->profile_count;++i)if(c->profiles[i].profile_id==id){
   if(papacc_egress_profile_validate(&c->profiles[i])!=PAPACC_RESULT_OK||
      c->profiles[i].available!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
   *out=&c->profiles[i];return PAPACC_RESULT_OK;}
 return PAPACC_RESULT_INVALID_STATE;}
