/* FCR0-FCR7 DATA only, disjoint from creation/CA/READ approvals. */
static const char prepared_prefix[]="PREPARED_INSTANCE_VALIDATE\nREAD_ONLY\nNO_CREATE_NO_SIGN_NO_BORROW_NO_CURRENT\n";
static int prepared_fields(const PAPACC_PERSONAL_PREPARED_SPEC *s,const unsigned char *fields[10],size_t sizes[10])
{
 const PAPACC_PERSONAL_EFFECT_SPEC *v;if(!s)return 0;v=&s->selection;
 if(!v->operator_bytes||v->operator_bytes>68U||!v->service_bytes||v->service_bytes>68U||
  v->leaf_bytes!=11U||!v->leaf||memcmp(v->leaf,"Accelerator",11U)||!v->configuration||
  !v->configuration_bytes||v->configuration_bytes>PAPACC_PERSONAL_PLAN_MAX)return 0;
 fields[0]=v->operator_id;sizes[0]=v->operator_bytes;fields[1]=v->parent_id;sizes[1]=12U;
 fields[2]=v->leaf;sizes[2]=11U;fields[3]=v->authority;sizes[3]=16U;
 fields[4]=v->generation;sizes[4]=16U;fields[5]=v->service_id;sizes[5]=v->service_bytes;
 fields[6]=s->target_id;sizes[6]=12U;fields[7]=s->source_id;sizes[7]=12U;
 fields[8]=s->image_id;sizes[8]=12U;fields[9]=s->image_digest;sizes[9]=32U;return 1;
}
int papacc_personal_prepared_source(const unsigned char *p,size_t n,const unsigned char **source,size_t *bytes)
{
 size_t at=sizeof(prepared_prefix)-1U,i,k,min,max;unsigned any;
 if(!p||!source||!bytes||n<=at||n>PAPACC_PERSONAL_PLAN_MAX||memcmp(p,prepared_prefix,at))return 0;
 for(i=0;i<10U;i++){
  min=max=i==1U||i==6U||i==7U||i==8U?24U:i==2U?22U:i==3U||i==4U?32U:i==9U?64U:2U;
  if(i==0U||i==5U)max=136U;
  k=any=0;
  while(at<n&&p[at]!='\n'){unsigned c=p[at++];if(!((c>='0'&&c<='9')||(c>='a'&&c<='f'))||++k>max)return 0;any|=c!='0';}
  if(k<min||(k&1U)||!any||at>=n)return 0;at++;
 }
 if(n-at<=sizeof("CONFIGURATION\n")-1U||memcmp(p+at,"CONFIGURATION\n",sizeof("CONFIGURATION\n")-1U))return 0;
 at+=sizeof("CONFIGURATION\n")-1U;*source=p+at;*bytes=n-at;return 1;
}
size_t papacc_personal_prepared_build(const PAPACC_PERSONAL_PREPARED_SPEC *v,unsigned char *out,size_t cap)
{
 const unsigned char *f[10];size_t z[10],n,i;unsigned char *at;
 if(!out||!prepared_fields(v,f,z))return 0;
 n=sizeof(prepared_prefix)-1U+sizeof("CONFIGURATION\n")-1U+v->selection.configuration_bytes;
 for(i=0;i<10U;i++){if(!effect_nonzero(f[i],z[i]))return 0;n+=2U*z[i]+1U;}
 if(n>cap||n>PAPACC_PERSONAL_PLAN_MAX||effect_alias(out,n,v,sizeof(*v))||
  effect_alias(out,n,v->selection.configuration,v->selection.configuration_bytes))return 0;
 for(i=0;i<10U;i++)if(effect_alias(out,n,f[i],z[i]))return 0;
 at=out;memcpy(at,prepared_prefix,sizeof(prepared_prefix)-1U);at+=sizeof(prepared_prefix)-1U;
 for(i=0;i<10U;i++){effect_hex(&at,f[i],z[i]);*at++='\n';}
 memcpy(at,"CONFIGURATION\n",sizeof("CONFIGURATION\n")-1U);at+=sizeof("CONFIGURATION\n")-1U;
 memcpy(at,v->selection.configuration,v->selection.configuration_bytes);return n;
}
int papacc_personal_prepared_matches(const unsigned char *p,size_t n,const PAPACC_PERSONAL_PREPARED_SPEC *v)
{
 const unsigned char *f[10],*source;size_t z[10],bytes,i,j,at=sizeof(prepared_prefix)-1U;static const char hex[]="0123456789abcdef";
 if(!prepared_fields(v,f,z)||!papacc_personal_prepared_source(p,n,&source,&bytes)||
  bytes!=v->selection.configuration_bytes||memcmp(source,v->selection.configuration,bytes))return 0;
 for(i=0;i<10U;i++){if(!f[i])return 0;for(j=0;j<z[i];j++){
  if(at+2U>n||p[at++]!=hex[f[i][j]>>4]||p[at++]!=hex[f[i][j]&15U])return 0;
 }if(at>=n||p[at++]!='\n')return 0;}
 return at+sizeof("CONFIGURATION\n")-1U==(size_t)(source-p);
}
int papacc_personal_prepared_upgrade(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *plan,size_t n)
{
 const unsigned char *source;size_t bytes;
 if(!p||p->phase!=PAPACC_PERSONAL_DATA_CAPTURED||p->consumed||p->effectful||!plan||!n||
  n>PAPACC_PERSONAL_PLAN_MAX||overlaps(p,plan,n)||!papacc_personal_prepared_source(plan,n,&source,&bytes)||
  bytes!=p->bytes||memcmp(source,p->plan,bytes))return 0;
 memcpy(p->plan,plan,n);p->bytes=n;p->effectful=3U;return 1;
}
int papacc_personal_prepared_confirm(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n,const char *reply,size_t bytes)
{
 static const char prefix[]="OWNER_PREPARED_INSTANCE_VALIDATE ",hex[]="0123456789abcdef";size_t i,z=sizeof(prefix)-1U;
 const unsigned char *source;size_t source_bytes;
 if(!p||p->effectful!=3U||p->phase!=PAPACC_PERSONAL_DATA_PRESENTED)return 0;
 if(!reply||bytes!=z+64U||!match(p,id,plan,n)||!papacc_personal_prepared_source(plan,n,&source,&source_bytes)||memcmp(reply,prefix,z))goto invalid;
 for(i=0;i<32U;i++)if(reply[z+2U*i]!=hex[id[i]>>4]||reply[z+2U*i+1U]!=hex[id[i]&15U])goto invalid;
 p->phase=PAPACC_PERSONAL_DATA_CONFIRMED;return 1;
invalid:p->phase=PAPACC_PERSONAL_DATA_INVALID;return 0;
}