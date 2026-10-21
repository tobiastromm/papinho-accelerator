/* Explicit typed BODY0.4 codec. Native selection is a separate strict04
 * profile in the existing configuration owner; no migration or autodetection. */
static int body04_overlap(const void *a,PAPACC_SIZE an,const void *b,PAPACC_SIZE bn)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x<=y?y-x<an:x-y<bn;}
static int body04_model(const PAPACC_CONFIG_BODY04_VIEW *v)
{
 const PAPACC_ADMIN_POLICY *p;unsigned i;
 if(!v||!v->base.config.policy.present||v->base.config.policy.representation!=PAPACC_ADMIN_REPRESENTATION_04||v->base.policy_epoch||
    v->base.config.site_tls.present||!v->base.revision||v->base.predecessor==(PAPACC_U64)-1||
    v->base.revision!=v->base.predecessor+1U||
    store03_zero(v->base.domain,32U)||
    v->base.config.bind_selection.interface_persistent_id_count>PAPACC_CONFIG_IDS||
    (v->base.config.bind_selection.interface_persistent_id_count&&v->base.config.bind_selection.interface_persistent_ids!=v->base.ids)||
    papacc_server_config_validate(&v->base.config)!=PAPACC_RESULT_OK)return 0;
 p=&v->base.config.policy;
 for(i=(unsigned)p->enabled_count;i<PAPACC_ADMIN_POLICY_KEYS;++i)
  if(p->enabled[i].id||p->enabled[i].major)return 0;
 for(i=0U;i<PAPACC_ADMIN_POLICY_RULES;++i){
  const PAPACC_BODY04_PRINCIPAL *id=&p->rules[i].principal16;
  if(!store03_zero(p->rules[i].principal,32U))return 0;
  if(i>=p->rule_count&&(p->rules[i].key.id||p->rules[i].key.major||p->rules[i].action||p->rules[i].conditions))return 0;
  if(i<p->rule_count&&(p->rules[i].conditions&PAPACC_ADMIN_POLICY_PRINCIPAL)){
   if(id->type!=PAPACC_BODY04_PRINCIPAL_RESOLVER16||id->length!=PAPACC_PRINCIPAL_VALUE_SIZE)return 0;
  }else if(id->type||id->length||!store03_zero(id->value,PAPACC_PRINCIPAL_VALUE_SIZE))return 0;
 }
 return 1;
}
PAPACC_RESULT papacc_configuration_body04_encode(const PAPACC_CONFIG_BODY04_VIEW *v,PAPACC_U8 *b,PAPACC_SIZE capacity)
{
 const PAPACC_ADMIN_POLICY *p;unsigned i,pos;PAPACC_U8 hash[32];
 if(!v||!b||capacity!=PAPACC_CONFIG_BODY04_BYTES||body04_overlap(v,sizeof(*v),b,capacity))return PAPACC_RESULT_INVALID_ARGUMENT;
 /* All semantic rejection occurs before touching the caller's destination. */
 if(!body04_model(v))return PAPACC_RESULT_INVALID_STATE;
 p=&v->base.config.policy;memset(b,0,PAPACC_CONFIG_BODY04_BYTES);
 encode(&v->base,b);if(!digest(b,0))return PAPACC_RESULT_INTERNAL_ERROR;
 memcpy(b+512U,"ADMP0401",8U);b[520U]=1U;b[521U]=(PAPACC_U8)p->profile;
 b[522U]=(PAPACC_U8)p->admission;p03_put16(b+524U,p->listener_port);
 b[526U]=(PAPACC_U8)p->enabled_count;b[527U]=(PAPACC_U8)p->rule_count;
 memcpy(b+528U,p->identity,32U);
 for(i=0U;i<p->enabled_count;++i){pos=560U+i*6U;p03_put32(b+pos,p->enabled[i].id);p03_put16(b+pos+4U,p->enabled[i].major);}
 for(i=0U;i<p->rule_count;++i){const PAPACC_ADMIN_POLICY_RULE *rule=&p->rules[i];const PAPACC_BODY04_PRINCIPAL *id=&p->rules[i].principal16;pos=584U+i*40U;
  p03_put32(b+pos,rule->key.id);p03_put16(b+pos+4U,rule->key.major);b[pos+6U]=(PAPACC_U8)rule->action;b[pos+7U]=(PAPACC_U8)rule->conditions;
  b[pos+8U]=id->type;b[pos+9U]=id->length;
  if(id->length)memcpy(b+pos+10U,id->value,id->length);
 }
 if(!store03_hash(b,992U,hash))return PAPACC_RESULT_INTERNAL_ERROR;
 memcpy(b+992U,hash,32U);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_body04_decode(const PAPACC_U8 *input,PAPACC_SIZE length,PAPACC_CONFIG_BODY04_VIEW *out)
{
 PAPACC_CONFIG_BODY04_VIEW v;PAPACC_U8 b[PAPACC_CONFIG_BODY04_BYTES];
 unsigned i,j,pos=70U;PAPACC_SIZE *lengths[4];PAPACC_U8 *refs[4];PAPACC_BOOL *valids[4];PAPACC_ADMIN_POLICY *p;
 if(!out)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(input&&body04_overlap(input,length,out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(!input||length!=PAPACC_CONFIG_BODY04_BYTES)return PAPACC_RESULT_INVALID_ARGUMENT;
 memcpy(b,input,sizeof(b));memset(&v,0,sizeof(v));
 if(memcmp(b+512U,"ADMP0401",8U)||!p03_body_valid(b)||store03_zero(b+8U,32U)||
    b[520U]!=1U||b[523U]||b[526U]>PAPACC_ADMIN_POLICY_KEYS||b[527U]>PAPACC_ADMIN_POLICY_RULES||
    !store03_zero(b+578U,6U)||!store03_zero(b+744U,248U))return PAPACC_RESULT_INVALID_STATE;
 memcpy(v.base.domain,b+8U,32U);v.base.revision=get64(b+40U);v.base.predecessor=get64(b+48U);
 v.base.policy_epoch=get64(b+56U);v.base.config.control_port=(PAPACC_U16)p03_get16(b+64U);
 v.base.config.listener_profile=(PAPACC_SERVER_LISTENER_PROFILE)b[66U];
 if(b[67U]>1U)return PAPACC_RESULT_INVALID_STATE;
 v.base.config.allow_network_egress=(PAPACC_BOOL)b[67U];v.base.config.bind_selection.mode=(PAPACC_BIND_SELECTION_MODE)b[68U];
 v.base.config.bind_selection.interface_persistent_id_count=b[69U];
 refs[0]=v.base.config.security_configuration_ref;lengths[0]=&v.base.config.security_configuration_ref_length;valids[0]=&v.base.config.security_configuration_ref_valid;
 refs[1]=v.base.config.egress_policy_ref;lengths[1]=&v.base.config.egress_policy_ref_length;valids[1]=&v.base.config.egress_policy_ref_valid;
 refs[2]=v.base.config.egress_profile_ref;lengths[2]=&v.base.config.egress_profile_ref_length;valids[2]=&v.base.config.egress_profile_ref_valid;
 refs[3]=v.base.config.outbound_tls_profile_ref;lengths[3]=&v.base.config.outbound_tls_profile_ref_length;valids[3]=&v.base.config.outbound_tls_profile_ref_valid;
 for(i=0U;i<4U;++i){unsigned n=b[pos++];*lengths[i]=n;*valids[i]=n?PAPACC_TRUE:PAPACC_FALSE;
  memcpy(refs[i],b+pos,n);for(j=n;j<64U;++j)if(b[pos+j])return PAPACC_RESULT_INVALID_STATE;pos+=64U;}
 for(i=0U;i<b[69U];++i){v.base.ids[i].is_valid=PAPACC_TRUE;v.base.ids[i].value=get64(b+330U+8U*i);}
 if(!store03_zero(b+330U+8U*b[69U],480U-330U-8U*b[69U]))return PAPACC_RESULT_INVALID_STATE;
 p=&v.base.config.policy;p->representation=PAPACC_ADMIN_REPRESENTATION_04;p->present=PAPACC_TRUE;p->profile=b[521U];p->admission=b[522U];p->listener_port=(PAPACC_U16)p03_get16(b+524U);
 p->enabled_count=b[526U];p->rule_count=b[527U];memcpy(p->identity,b+528U,32U);
 for(i=0U;i<PAPACC_ADMIN_POLICY_KEYS;++i){pos=560U+i*6U;
  if(i<p->enabled_count){p->enabled[i].id=p03_get32(b+pos);p->enabled[i].major=(PAPACC_U16)p03_get16(b+pos+4U);}
  else if(!store03_zero(b+pos,6U))return PAPACC_RESULT_INVALID_STATE;}
 for(i=0U;i<PAPACC_ADMIN_POLICY_RULES;++i){pos=584U+i*40U;
  if(i<p->rule_count){PAPACC_ADMIN_POLICY_RULE *rule=&p->rules[i];PAPACC_BODY04_PRINCIPAL *id=&rule->principal16;
   rule->key.id=p03_get32(b+pos);rule->key.major=(PAPACC_U16)p03_get16(b+pos+4U);rule->action=b[pos+6U];rule->conditions=b[pos+7U];
   id->type=b[pos+8U];id->length=b[pos+9U];
   if(id->length!=0U&&id->length!=PAPACC_PRINCIPAL_VALUE_SIZE)return PAPACC_RESULT_INVALID_STATE;
   if(id->length)memcpy(id->value,b+pos+10U,id->length);
   if(!store03_zero(b+pos+10U+id->length,30U-id->length))return PAPACC_RESULT_INVALID_STATE;
  }else if(!store03_zero(b+pos,40U))return PAPACC_RESULT_INVALID_STATE;}
 fix(&v.base);if(!body04_model(&v))return PAPACC_RESULT_INVALID_STATE;
 *out=v;fix(&out->base);return PAPACC_RESULT_OK;
}
