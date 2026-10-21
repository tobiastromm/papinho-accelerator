/* Protocol0.2 full server-model BODY profile; same C/W/H selection algorithm.
 * No capabilities, runtime policy or leases are invented by body decoding. */
#define STORE02_C 128U
#define STORE02_W 5248U
#define STORE02_PENDING 6272U
#define STORE02_FOOTER 8160U
#define STORE02_CAP 8U
static int store02_zero(const unsigned char*p,unsigned n)
{unsigned i;for(i=0;i<n;++i)if(p[i])return 0;return 1;}
static int store02_hash(const unsigned char*p,unsigned n,unsigned char out[32])
{
 BCRYPT_ALG_HANDLE a=NULL;BCRYPT_HASH_HANDLE h=NULL;NTSTATUS s;int ok;
 s=BCryptOpenAlgorithmProvider(&a,BCRYPT_SHA256_ALGORITHM,NULL,0U);if(s<0)return 0;
 s=BCryptCreateHash(a,&h,NULL,0U,NULL,0U,0U);
 if(s>=0)s=BCryptHashData(h,(PUCHAR)p,n,0U);
 if(s>=0)s=BCryptFinishHash(h,out,32U,0U);ok=s>=0;
 if(h!=NULL&&BCryptDestroyHash(h)<0)ok=0;
 if(BCryptCloseAlgorithmProvider(a,0U)<0)ok=0;return ok;
}
static int store02_decode(unsigned char*b,PAPACC_CONFIG_VIEW*v)
{
 unsigned i,j,pos=70U;PAPACC_SIZE*lengths[4];PAPACC_U8*refs[4];PAPACC_BOOL*valids[4];
 if(!record_valid(b)||store02_zero(b+8U,32U))return 0;
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
 if(!store02_zero(b+330U+8U*b[69U],480U-330U-8U*b[69U]))return 0;
 fix(v);return papacc_server_config_validate(&v->config)==PAPACC_RESULT_OK;
}
static int store02_select(unsigned char*b,const BY_HANDLE_FILE_INFORMATION*f,PAPACC_CONFIG_VIEW*out)
{
 unsigned i,j,n;unsigned char d[32],prev[32];unsigned char*c,*w;PAPACC_CONFIG_VIEW candidate;PAPACC_U64 last_epoch=0U;
 if(memcmp(b,"PACL0202",8U)||get64(b+8U)!=2U||store02_zero(b+16U,32U))return 0;
 if(!store02_hash(b,STORE02_FOOTER,d)||memcmp(d,b+STORE02_FOOTER,32U))return 0;
 if(get64(b+104U)!=f->dwVolumeSerialNumber||get64(b+112U)!=(((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow))return 0;
 if(get64(b+56U)==0U||get64(b+56U)>STORE02_CAP||get64(b+64U)!=get64(b+56U))return 0;
 n=(unsigned)get64(b+56U);memset(prev,0,32U);memset(&candidate,0,sizeof(candidate));
 if(!store02_zero(b+STORE02_PENDING,1280U)||!store02_zero(b+120U,8U))return 0;
 for(i=0U;i<n;++i){c=b+STORE02_C+i*640U;w=b+STORE02_W+i*128U;
  if(memcmp(c,b+16U,32U)||memcmp(w,b+16U,32U))return 0;
  for(j=0U;j<i;++j)if(get64(w+32U)==get64(b+STORE02_W+j*128U+32U)||get64(w+48U)==get64(b+STORE02_W+j*128U+48U))return 0;
  if(get64(c+32U)!=i+1U||get64(w+32U)!=i+1U||get64(c+40U)!=i||get64(w+40U)!=i||
    get64(c+48U)==0U||get64(c+48U)!=get64(w+48U)||get64(c+56U)!=512U)return 0;
  if(memcmp(c+64U,prev,32U)||memcmp(w+88U,prev,32U)||memcmp(c+96U,"CONF0101",8U)||!store02_zero(c+104U,24U)||!store02_zero(w+120U,8U))return 0;
  if(!store02_hash(c,640U,d)||memcmp(d,w+56U,32U)||!store02_decode(c+128U,&candidate))return 0;
  if(memcmp(candidate.domain,b+16U,32U)||candidate.revision!=i+1U||candidate.predecessor!=i)return 0;
  if(candidate.policy_epoch<last_epoch)return 0;last_epoch=candidate.policy_epoch;
  if(!store02_hash(w,128U,prev))return 0;
 }
 if(get64(b+48U)!=n||memcmp(b+72U,prev,32U)||!store02_zero(b+STORE02_C+n*640U,(STORE02_CAP-n)*640U)||
   !store02_zero(b+STORE02_W+n*128U,(STORE02_CAP-n)*128U)||!store02_zero(b+7552U,STORE02_FOOTER-7552U))return 0;
 *out=candidate;fix(out);return 1;
}
static int store02_append(PAPACC_CONFIGURATION_COMMIT*o,const PAPACC_CONFIG_VIEW*v,
 const BY_HANDLE_FILE_INFORMATION*f,unsigned char*b)
{
 unsigned char*c,*w,d[32];unsigned index;PAPACC_CONFIG_VIEW prior;
 if(v->revision==0U||v->revision>STORE02_CAP)return 0;
 if(o->published){memcpy(b,o->committed,PAPACC_CONFIG_STORE_BYTES);if(!store02_select(b,f,&prior)||prior.revision!=v->predecessor)return 0;}
 else{memset(b,0,PAPACC_CONFIG_STORE_BYTES);memcpy(b,"PACL0202",8U);put64(b+8U,2U);memcpy(b+16U,v->domain,32U);
  put64(b+104U,f->dwVolumeSerialNumber);put64(b+112U,((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow);}
 index=(unsigned)v->revision-1U;c=b+STORE02_C+index*640U;w=b+STORE02_W+index*128U;
 memcpy(c,v->domain,32U);put64(c+32U,v->revision);put64(c+40U,v->predecessor);put64(c+48U,1000U+v->revision);
 put64(c+56U,512U);memcpy(c+64U,b+72U,32U);memcpy(c+96U,"CONF0101",8U);encode(v,c+128U);
 if(!digest(c+128U,0))return 0;
 memcpy(w,v->domain,32U);put64(w+32U,v->revision);put64(w+40U,v->predecessor);put64(w+48U,1000U+v->revision);
 if(!store02_hash(c,640U,d))return 0;memcpy(w+56U,d,32U);memcpy(w+88U,b+72U,32U);
 if(!store02_hash(w,128U,d))return 0;memcpy(b+72U,d,32U);put64(b+48U,v->revision);put64(b+56U,v->revision);put64(b+64U,v->revision);
 return store02_hash(b,STORE02_FOOTER,b+STORE02_FOOTER);
}
