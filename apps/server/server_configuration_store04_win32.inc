/* Explicit private BODY0.4 custody. No auto-detect or migration. */
static int p04_body_encode(const PAPACC_CONFIG_VIEW *v,unsigned char *b)
{
 PAPACC_CONFIG_BODY04_VIEW typed;typed.base=*v;fix(&typed.base);
 return papacc_configuration_body04_encode(&typed,b,1024U)==PAPACC_RESULT_OK;
}
static int store04_decode(unsigned char *b,PAPACC_CONFIG_VIEW *v)
{
 PAPACC_CONFIG_BODY04_VIEW typed;
 if(papacc_configuration_body04_decode(b,1024U,&typed)!=PAPACC_RESULT_OK)return 0;
 *v=typed.base;fix(v);return 1;
}
static int store04_select(unsigned char*b,const BY_HANDLE_FILE_INFORMATION*f,PAPACC_CONFIG_VIEW*out)
{
 unsigned i,j,n;unsigned char d[32],prev[32];unsigned char*c,*w;PAPACC_CONFIG_VIEW candidate;PAPACC_U64 last_epoch=0U;
 if(memcmp(b,"PACL0401",8U)||get64(b+8U)!=4U||store03_zero(b+16U,32U))return 0;
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
  if(memcmp(c+64U,prev,32U)||memcmp(w+88U,prev,32U)||memcmp(c+96U,"CONF0401",8U)||!store03_zero(c+104U,24U)||!store03_zero(w+120U,8U))return 0;
  if(!store03_hash(c,1152U,d)||memcmp(d,w+56U,32U)||!store04_decode(c+128U,&candidate))return 0;
  if(memcmp(candidate.domain,b+16U,32U)||candidate.revision!=i+1U||candidate.predecessor!=i)return 0;
  if(candidate.policy_epoch<last_epoch)return 0;last_epoch=candidate.policy_epoch;
  if(!store03_hash(w,128U,prev))return 0;
 }
 if(get64(b+48U)!=n||memcmp(b+72U,prev,32U)||!store03_zero(b+STORE03_C+n*1152U,(STORE03_CAP-n)*1152U)||
   !store03_zero(b+STORE03_W+n*128U,(STORE03_CAP-n)*128U)||!store03_zero(b+6528U,STORE03_FOOTER-6528U))return 0;
 *out=candidate;fix(out);return 1;
}
static int store04_append(PAPACC_CONFIGURATION_COMMIT*o,const PAPACC_CONFIG_VIEW*v,
 const BY_HANDLE_FILE_INFORMATION*f,unsigned char*b)
{
 unsigned char*c,*w,d[32];unsigned index;PAPACC_CONFIG_VIEW prior;
 if(v->revision==0U||v->revision>STORE03_CAP)return 0;
 if(o->published){memcpy(b,o->committed,PAPACC_CONFIG_STORE_BYTES);if(!store04_select(b,f,&prior)||prior.revision!=v->predecessor)return 0;}
 else{memset(b,0,PAPACC_CONFIG_STORE_BYTES);memcpy(b,"PACL0401",8U);put64(b+8U,4U);memcpy(b+16U,v->domain,32U);
  put64(b+104U,f->dwVolumeSerialNumber);put64(b+112U,((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow);}
 index=(unsigned)v->revision-1U;c=b+STORE03_C+index*1152U;w=b+STORE03_W+index*128U;
 memcpy(c,v->domain,32U);put64(c+32U,v->revision);put64(c+40U,v->predecessor);put64(c+48U,1000U+v->revision);
 put64(c+56U,1024U);memcpy(c+64U,b+72U,32U);memcpy(c+96U,"CONF0401",8U);if(!p04_body_encode(v,c+128U))return 0;
 memcpy(w,v->domain,32U);put64(w+32U,v->revision);put64(w+40U,v->predecessor);put64(w+48U,1000U+v->revision);
 if(!store03_hash(c,1152U,d))return 0;memcpy(w+56U,d,32U);memcpy(w+88U,b+72U,32U);
 if(!store03_hash(w,128U,d))return 0;memcpy(b+72U,d,32U);put64(b+48U,v->revision);put64(b+56U,v->revision);put64(b+64U,v->revision);
 return store03_hash(b,STORE03_FOOTER,b+STORE03_FOOTER);
}
