/* Bounded private plan bytes only. No allocator, native API or authority. */
static const char effect_prefix[]="NEW_INSTALL\nCREATE_EXCLUSIVE_TARGET\nCA_INITIALIZE\nINITIAL_ROOT_PREPARE\nCOMMIT=DENY\nCURRENT=DENY\n";
static int effect_nonzero(const unsigned char *p,size_t n)
{size_t i;unsigned v=0;if(!p)return 0;for(i=0;i<n;i++)v|=p[i];return v!=0;}
static int effect_alias(const void *left,size_t a,const void *right,size_t b)
{const unsigned char *x=(const unsigned char *)left,*y=(const unsigned char *)right;size_t i;
 for(i=0;i<a;i++)if(x+i==y||x+i==y+b-1U)return 1;
 for(i=0;i<b;i++)if(y+i==x||y+i==x+a-1U)return 1;return 0;}
static void effect_hex(unsigned char **out,const unsigned char *p,size_t n)
{static const char hex[]="0123456789abcdef";size_t i;for(i=0;i<n;i++){*(*out)++=(unsigned char)hex[p[i]>>4];*(*out)++=(unsigned char)hex[p[i]&15U];}}
size_t papacc_personal_effect_build(const PAPACC_PERSONAL_EFFECT_SPEC *v,unsigned char *out,size_t cap)
{
 const unsigned char *fields[6];size_t sizes[6],need,i,j;unsigned char *p;
 if(!v||!out||!v->configuration||!v->configuration_bytes||v->configuration_bytes>PAPACC_PERSONAL_PLAN_MAX||
 !v->operator_bytes||v->operator_bytes>68U||!v->service_bytes||v->service_bytes>68U||!v->leaf||!v->leaf_bytes||v->leaf_bytes>64U)return 0;
 fields[0]=v->operator_id;sizes[0]=v->operator_bytes;fields[1]=v->parent_id;sizes[1]=12U;
 fields[2]=v->leaf;sizes[2]=v->leaf_bytes;fields[3]=v->authority;sizes[3]=16U;
 fields[4]=v->generation;sizes[4]=16U;fields[5]=v->service_id;sizes[5]=v->service_bytes;
 need=sizeof(effect_prefix)-1U+sizeof("CONFIGURATION\n")-1U+v->configuration_bytes;
 for(i=0;i<6U;i++){if(!effect_nonzero(fields[i],sizes[i]))return 0;need+=sizes[i]*2U+1U;}
 if(v->operator_bytes==v->service_bytes&&!memcmp(v->operator_id,v->service_id,v->operator_bytes))return 0;
 for(i=0;i<v->leaf_bytes;i++){unsigned ch=v->leaf[i];if(!((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='-'||ch=='.'))return 0;}
 if(v->leaf[v->leaf_bytes-1U]=='.'||need>cap||need>PAPACC_PERSONAL_PLAN_MAX)return 0;
 /* Reject every alias before touching output; no partial plan on error. */
 if(effect_alias(out,need,v,sizeof(*v))||effect_alias(out,need,v->configuration,v->configuration_bytes))return 0;
 for(j=0;j<6U;j++)if(effect_alias(out,need,fields[j],sizes[j]))return 0;
 p=out;memcpy(p,effect_prefix,sizeof(effect_prefix)-1U);p+=sizeof(effect_prefix)-1U;
 for(i=0;i<6U;i++){effect_hex(&p,fields[i],sizes[i]);*p++='\n';}
 memcpy(p,"CONFIGURATION\n",sizeof("CONFIGURATION\n")-1U);p+=sizeof("CONFIGURATION\n")-1U;
 memcpy(p,v->configuration,v->configuration_bytes);return need;
}
int papacc_personal_effect_source(const unsigned char *p,size_t n,const unsigned char **source,size_t *bytes)
{
 size_t i,k,offset=sizeof(effect_prefix)-1U,min,max;unsigned any;
 if(!p||!source||!bytes||n>PAPACC_PERSONAL_PLAN_MAX||n<=offset||memcmp(p,effect_prefix,offset))return 0;
 for(i=0;i<6U;i++){
  k=0;any=0;min=i==1U?24U:(i==3U||i==4U?32U:2U);max=i==0U||i==5U?136U:(i==1U?24U:(i==2U?128U:32U));
  while(offset<n&&p[offset]!='\n'){
   unsigned ch=p[offset++];if(!((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f'))||++k>max)return 0;any|=ch!='0';}
  if(k<min||(k&1U)||!any||offset>=n)return 0;offset++;
 }
 if(n-offset<=sizeof("CONFIGURATION\n")-1U||memcmp(p+offset,"CONFIGURATION\n",sizeof("CONFIGURATION\n")-1U))return 0;
 offset+=sizeof("CONFIGURATION\n")-1U;*source=p+offset;*bytes=n-offset;return 1;
}
int papacc_personal_effect_upgrade(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *plan,size_t n)
{
 const unsigned char *source;size_t bytes;
 if(!p||p->phase!=PAPACC_PERSONAL_DATA_CAPTURED||p->consumed||p->effectful||!plan||!n||n>PAPACC_PERSONAL_PLAN_MAX||
 overlaps(p,plan,n)||!papacc_personal_effect_source(plan,n,&source,&bytes)||bytes!=p->bytes||memcmp(source,p->plan,bytes))return 0;
 memcpy(p->plan,plan,n);p->bytes=n;p->effectful=1U;return 1;
}
int papacc_personal_effect_confirm(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n,const char *reply,size_t reply_bytes)
{
 static const char prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ";static const char hex[]="0123456789abcdef";
 const unsigned char *source;size_t bytes,i,z=sizeof(prefix)-1U;
 if(!p||!p->effectful||p->phase!=PAPACC_PERSONAL_DATA_PRESENTED)return 0;
 if(!reply||reply_bytes!=z+64U||!match(p,id,plan,n)||!papacc_personal_effect_source(plan,n,&source,&bytes)||memcmp(reply,prefix,z))goto invalid;
 for(i=0;i<32U;i++)if(reply[z+2U*i]!=hex[id[i]>>4]||reply[z+2U*i+1U]!=hex[id[i]&15U])goto invalid;
 p->phase=PAPACC_PERSONAL_DATA_CONFIRMED;return 1;
invalid:p->phase=PAPACC_PERSONAL_DATA_INVALID;return 0;
}
int papacc_personal_effect_consume(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n)
{
 if(!p||!p->effectful||p->phase!=PAPACC_PERSONAL_DATA_CONFIRMED||p->consumed)return 0;
 if(!match(p,id,plan,n)){p->phase=PAPACC_PERSONAL_DATA_INVALID;return 0;}
 p->consumed=1U;p->phase=PAPACC_PERSONAL_DATA_CONSUMED;return 1;
}

/* Compare bounded immutable presentation against requested bindings, no copy. */
int papacc_personal_effect_matches(const unsigned char *plan,size_t n,const PAPACC_PERSONAL_EFFECT_SPEC *v)
{
 const unsigned char *source,*fields[6];size_t bytes,sizes[6],offset,i,j;static const char hex[]="0123456789abcdef";
 if(!v||!papacc_personal_effect_source(plan,n,&source,&bytes)||!v->configuration||bytes!=v->configuration_bytes||
 memcmp(source,v->configuration,bytes)||!v->operator_bytes||v->operator_bytes>68U||!v->service_bytes||v->service_bytes>68U||!v->leaf_bytes||v->leaf_bytes>64U)return 0;
 fields[0]=v->operator_id;sizes[0]=v->operator_bytes;fields[1]=v->parent_id;sizes[1]=12U;
 fields[2]=v->leaf;sizes[2]=v->leaf_bytes;fields[3]=v->authority;sizes[3]=16U;
 fields[4]=v->generation;sizes[4]=16U;fields[5]=v->service_id;sizes[5]=v->service_bytes;
 offset=sizeof(effect_prefix)-1U;
 for(i=0;i<6U;i++){
  if(!fields[i])return 0;
  for(j=0;j<sizes[i];j++){
   if(offset+2U>n||plan[offset++]!=(unsigned char)hex[fields[i][j]>>4]||plan[offset++]!=(unsigned char)hex[fields[i][j]&15U])return 0;
  }
  if(offset>=n||plan[offset++]!='\n')return 0;
 }
 return offset+(sizeof("CONFIGURATION\n")-1U)==(size_t)(source-plan);
}
