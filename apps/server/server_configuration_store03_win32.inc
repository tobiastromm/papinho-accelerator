/* Private BODY0.3 profile: same custody/selection algorithm, four1024-byte bodies.
 * Old0.2 codec stays unchanged; no migration or positive policy epoch. */
#define STORE03_C 128U
#define STORE03_W 4736U
#define STORE03_PENDING 5248U
#define STORE03_FOOTER 8160U
#define STORE03_CAP 4U
static int store03_zero(const unsigned char*p,unsigned n)
{unsigned i;for(i=0;i<n;++i)if(p[i])return 0;return 1;}
static int store03_hash(const unsigned char*p,unsigned n,unsigned char out[32])
{
 BCRYPT_ALG_HANDLE a=NULL;BCRYPT_HASH_HANDLE h=NULL;NTSTATUS s;int ok;
 s=BCryptOpenAlgorithmProvider(&a,BCRYPT_SHA256_ALGORITHM,NULL,0U);if(s<0)return 0;
 s=BCryptCreateHash(a,&h,NULL,0U,NULL,0U,0U);
 if(s>=0)s=BCryptHashData(h,(PUCHAR)p,n,0U);
 if(s>=0)s=BCryptFinishHash(h,out,32U,0U);ok=s>=0;
 if(h!=NULL&&BCryptDestroyHash(h)<0)ok=0;
 if(BCryptCloseAlgorithmProvider(a,0U)<0)ok=0;return ok;
}
static void p03_put16(unsigned char *b,unsigned n){b[0]=(unsigned char)(n>>8);b[1]=(unsigned char)n;}
static unsigned p03_get16(const unsigned char *b){return ((unsigned)b[0]<<8)|b[1];}
static void p03_put32(unsigned char *b,PAPACC_U32 n){b[0]=(unsigned char)(n>>24);b[1]=(unsigned char)(n>>16);b[2]=(unsigned char)(n>>8);b[3]=(unsigned char)n;}
static PAPACC_U32 p03_get32(const unsigned char *b){return ((PAPACC_U32)b[0]<<24)|((PAPACC_U32)b[1]<<16)|((PAPACC_U32)b[2]<<8)|b[3];}
static int p03_body_encode(const PAPACC_CONFIG_VIEW *v,unsigned char *b)
{
 unsigned i,pos;unsigned char hash[32];const PAPACC_ADMIN_POLICY *p=&v->config.policy;
 if(p->representation!=0U||!p->present||v->policy_epoch!=0U||papacc_server_config_validate(&v->config)!=PAPACC_RESULT_OK)return 0;
 memset(b,0,1024U);encode(v,b);if(!digest(b,0))return 0;
 memcpy(b+512U,"ADMP0301",8U);b[520U]=1U;b[521U]=(unsigned char)p->profile;
 b[522U]=(unsigned char)p->admission;p03_put16(b+524U,p->listener_port);
 b[526U]=(unsigned char)p->enabled_count;b[527U]=(unsigned char)p->rule_count;memcpy(b+528U,p->identity,32U);
 for(i=0U;i<p->enabled_count;++i){pos=560U+i*6U;p03_put32(b+pos,p->enabled[i].id);p03_put16(b+pos+4U,p->enabled[i].major);}
 for(i=0U;i<p->rule_count;++i){const PAPACC_ADMIN_POLICY_RULE *rule=&p->rules[i];pos=584U+i*40U;
  p03_put32(b+pos,rule->key.id);p03_put16(b+pos+4U,rule->key.major);
  b[pos+6U]=(unsigned char)rule->action;b[pos+7U]=(unsigned char)rule->conditions;memcpy(b+pos+8U,rule->principal,32U);}
 if(!store03_hash(b,992U,hash))return 0;memcpy(b+992U,hash,32U);return 1;
}
static int p03_body_valid(unsigned char *b)
{
 unsigned i;unsigned char hash[32];PAPACC_U64 rev=get64(b+40U),pred=get64(b+48U);
 if(memcmp(b,"PACCFC01",8U)||rev==0U||pred==((PAPACC_U64)-1)||rev!=pred+1U||
    get64(b+56U)!=0U||b[69U]>PAPACC_CONFIG_IDS||!digest(b,1))return 0;
 for(i=0U;i<4U;++i)if(b[70U+i*65U]>64U)return 0;
 return store03_hash(b,992U,hash)&&memcmp(hash,b+992U,32U)==0;
}
static int p03_policy_decode(const unsigned char *b,PAPACC_ADMIN_POLICY *p)
{
 unsigned i,pos;
 if(memcmp(b+512U,"ADMP0301",8U)||b[520U]!=1U||b[523U]!=0U||
    b[526U]>PAPACC_ADMIN_POLICY_KEYS||b[527U]>PAPACC_ADMIN_POLICY_RULES ||
    !store03_zero(b+578U,6U)||!store03_zero(b+744U,248U))return 0;
 p->present=PAPACC_TRUE;p->profile=b[521U];p->admission=b[522U];p->listener_port=(PAPACC_U16)p03_get16(b+524U);
 p->enabled_count=b[526U];p->rule_count=b[527U];memcpy(p->identity,b+528U,32U);
 for(i=0U;i<PAPACC_ADMIN_POLICY_KEYS;++i){pos=560U+i*6U;
  if(i<p->enabled_count){p->enabled[i].id=p03_get32(b+pos);p->enabled[i].major=(PAPACC_U16)p03_get16(b+pos+4U);}
  else if(!store03_zero(b+pos,6U))return 0;}
 for(i=0U;i<PAPACC_ADMIN_POLICY_RULES;++i){pos=584U+i*40U;
  if(i<p->rule_count){PAPACC_ADMIN_POLICY_RULE *rule=&p->rules[i];rule->key.id=p03_get32(b+pos);
   rule->key.major=(PAPACC_U16)p03_get16(b+pos+4U);rule->action=b[pos+6U];rule->conditions=b[pos+7U];memcpy(rule->principal,b+pos+8U,32U);}
  else if(!store03_zero(b+pos,40U))return 0;}
 return 1;
}
static int store03_decode(unsigned char*b,PAPACC_CONFIG_VIEW*v)
{
 unsigned i,j,pos=70U;PAPACC_SIZE*lengths[4];PAPACC_U8*refs[4];PAPACC_BOOL*valids[4];
 if(!p03_body_valid(b)||store03_zero(b+8U,32U))return 0;
 memset(v,0,sizeof(*v));memcpy(v->domain,b+8U,32U);v->revision=get64(b+40U);
 v->predecessor=get64(b+48U);v->policy_epoch=get64(b+56U);
 v->config.control_port=(PAPACC_U16)(((unsigned)b[64U]<<8)|b[65U]);
 v->config.listener_profile=(PAPACC_SERVER_LISTENER_PROFILE)b[66U];
 if(b[67U]>1U)return 0;v->config.allow_network_egress=(PAPACC_BOOL)b[67U];
 v->config.bind_selection.mode=(PAPACC_BIND_SELECTION_MODE)b[68U];
 v->config.bind_selection.interface_persistent_id_count=b[69U];
 refs[0]=v->config.security_configuration_ref;lengths[0]=&v->config.security_configuration_ref_length;valids[0]=&v->config.security_configuration_ref_valid;
 refs[1]=v->config.egress_policy_ref;lengths[1]=&v->config.egress_policy_ref_length;valids[1]=&v->config.egress_policy_ref_valid;
 refs[2]=v->config.egress_profile_ref;lengths[2]=&v->config.egress_profile_ref_length;valids[2]=&v->config.egress_profile_ref_valid;
 refs[3]=v->config.outbound_tls_profile_ref;lengths[3]=&v->config.outbound_tls_profile_ref_length;valids[3]=&v->config.outbound_tls_profile_ref_valid;
 for(i=0U;i<4U;++i){unsigned n=b[pos++];*lengths[i]=n;*valids[i]=n?PAPACC_TRUE:PAPACC_FALSE;
  memcpy(refs[i],b+pos,n);for(j=n;j<64U;++j)if(b[pos+j])return 0;pos+=64U;}
 for(i=0U;i<b[69U];++i){v->ids[i].is_valid=PAPACC_TRUE;v->ids[i].value=get64(b+330U+8U*i);}
 if(!store03_zero(b+330U+8U*b[69U],480U-330U-8U*b[69U]))return 0;
 fix(v);return p03_policy_decode(b,&v->config.policy)&&papacc_server_config_validate(&v->config)==PAPACC_RESULT_OK;
}
static int store03_select(unsigned char*b,const BY_HANDLE_FILE_INFORMATION*f,PAPACC_CONFIG_VIEW*out)
{
 unsigned i,j,n;unsigned char d[32],prev[32];unsigned char*c,*w;PAPACC_CONFIG_VIEW candidate;PAPACC_U64 last_epoch=0U;
 if(memcmp(b,"PACL0301",8U)||get64(b+8U)!=3U||store03_zero(b+16U,32U))return 0;
 if(!store03_hash(b,STORE03_FOOTER,d)||memcmp(d,b+STORE03_FOOTER,32U))return 0;
 if(get64(b+104U)!=f->dwVolumeSerialNumber||get64(b+112U)!=(((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow))return 0;
 if(get64(b+56U)==0U||get64(b+56U)>STORE03_CAP||get64(b+64U)!=get64(b+56U))return 0;
 n=(unsigned)get64(b+56U);memset(prev,0,32U);memset(&candidate,0,sizeof(candidate));
 if(!store03_zero(b+STORE03_PENDING,1280U)||!store03_zero(b+120U,8U))return 0;
 for(i=0U;i<n;++i){c=b+STORE03_C+i*1152U;w=b+STORE03_W+i*128U;
  if(memcmp(c,b+16U,32U)||memcmp(w,b+16U,32U))return 0;
  for(j=0U;j<i;++j)if(get64(w+32U)==get64(b+STORE03_W+j*128U+32U)||get64(w+48U)==get64(b+STORE03_W+j*128U+48U))return 0;
  if(get64(c+32U)!=i+1U||get64(w+32U)!=i+1U||get64(c+40U)!=i||get64(w+40U)!=i||
    get64(c+48U)==0U||get64(c+48U)!=get64(w+48U)||get64(c+56U)!=1024U)return 0;
  if(memcmp(c+64U,prev,32U)||memcmp(w+88U,prev,32U)||memcmp(c+96U,"CONF0301",8U)||!store03_zero(c+104U,24U)||!store03_zero(w+120U,8U))return 0;
  if(!store03_hash(c,1152U,d)||memcmp(d,w+56U,32U)||!store03_decode(c+128U,&candidate))return 0;
  if(memcmp(candidate.domain,b+16U,32U)||candidate.revision!=i+1U||candidate.predecessor!=i)return 0;
  if(candidate.policy_epoch<last_epoch)return 0;last_epoch=candidate.policy_epoch;
  if(!store03_hash(w,128U,prev))return 0;
 }
 if(get64(b+48U)!=n||memcmp(b+72U,prev,32U)||!store03_zero(b+STORE03_C+n*1152U,(STORE03_CAP-n)*1152U)||
   !store03_zero(b+STORE03_W+n*128U,(STORE03_CAP-n)*128U)||!store03_zero(b+6528U,STORE03_FOOTER-6528U))return 0;
 *out=candidate;fix(out);return 1;
}
static int store03_append(PAPACC_CONFIGURATION_COMMIT*o,const PAPACC_CONFIG_VIEW*v,
 const BY_HANDLE_FILE_INFORMATION*f,unsigned char*b)
{
 unsigned char*c,*w,d[32];unsigned index;PAPACC_CONFIG_VIEW prior;
 if(v->revision==0U||v->revision>STORE03_CAP)return 0;
 if(o->published){memcpy(b,o->committed,PAPACC_CONFIG_STORE_BYTES);if(!store03_select(b,f,&prior)||prior.revision!=v->predecessor)return 0;}
 else{memset(b,0,PAPACC_CONFIG_STORE_BYTES);memcpy(b,"PACL0301",8U);put64(b+8U,3U);memcpy(b+16U,v->domain,32U);
  put64(b+104U,f->dwVolumeSerialNumber);put64(b+112U,((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow);}
 index=(unsigned)v->revision-1U;c=b+STORE03_C+index*1152U;w=b+STORE03_W+index*128U;
 memcpy(c,v->domain,32U);put64(c+32U,v->revision);put64(c+40U,v->predecessor);put64(c+48U,1000U+v->revision);
 put64(c+56U,1024U);memcpy(c+64U,b+72U,32U);memcpy(c+96U,"CONF0301",8U);if(!p03_body_encode(v,c+128U))return 0;
 memcpy(w,v->domain,32U);put64(w+32U,v->revision);put64(w+40U,v->predecessor);put64(w+48U,1000U+v->revision);
 if(!store03_hash(c,1152U,d))return 0;memcpy(w+56U,d,32U);memcpy(w+88U,b+72U,32U);
 if(!store03_hash(w,128U,d))return 0;memcpy(b+72U,d,32U);put64(b+48U,v->revision);put64(b+56U,v->revision);put64(b+64U,v->revision);
 return store03_hash(b,STORE03_FOOTER,b+STORE03_FOOTER);
}
