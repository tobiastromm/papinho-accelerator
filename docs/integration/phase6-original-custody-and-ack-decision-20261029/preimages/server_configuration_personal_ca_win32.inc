/* Same writer/packet protects one borrow from the existing CA owner.
 * No initialization, private export, signature, publication or new manager. */
PAPACC_RESULT papacc_configuration_personal_ca_match_data(const PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CA_KEY *ca,const unsigned char id[32],const unsigned char a[16],const unsigned char g[16])
{
 const PAPACC_CONFIG_PENDING *p;const PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC v;
 unsigned char parent[12];DWORD values[3];unsigned i,j,k;wchar_t name[96];const wchar_t *prefix=L"PapinhoAccelerator.CA.",*hex=L"0123456789abcdef";
 const unsigned char *source;size_t bytes;
 if(!o||!ca||!id||!a||!g||o->writer!=GetCurrentThreadId()||ca->thread!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(o->stopped||o->unresolved||p->stop||p->active||p->thread||p->close_error||
 !p->personal_data.effectful||!p->personal_data.consumed||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONSUMED||
 memcmp(id,p->personal_data.attempt,32U)||memcmp(id,n->handoff_attempt,32U)||n->phase!=1U||n->handoff_state!=PAPACC_PERSONAL_HANDOFF_VERIFIED||
 !p->bootstrap_created||!p->bootstrap_parent||!p->bootstrap_container||p->result!=PAPACC_CAS_SUCCESS||
 !n->plan_leaf[0]||!memchr(n->plan_leaf,0,sizeof(n->plan_leaf))||memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))||
 ca->state!=PAPACC_CA_KEY_READY||!ca->created||ca->diagnostic_finalization!=2U||!ca->key||!ca->provider||ca->close_attempted||
 ca->retained_token||ca->retained_local[0]||ca->retained_local[1]||ca->retained_local[2]||
 ca->operator_size!=n->sid_bytes||!n->sid_bytes||n->sid_bytes>sizeof(n->sid)||memcmp(ca->operator_sid,n->sid,n->sid_bytes)||
 !ca->service_size||ca->service_size>sizeof(ca->service_sid)||!wmemchr(ca->name,0,sizeof(ca->name)/sizeof(ca->name[0])))return PAPACC_RESULT_INVALID_STATE;
 /* Explicitly exclude both retained LAB identities from productive binding. */
 if(!wcscmp(ca->name,L"PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0")||
 !wcscmp(ca->name,L"PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2"))return PAPACC_RESULT_NOT_SUPPORTED;
 k=0;while(*prefix)name[k++]=*prefix++;
 for(i=0;i<16U;i++){name[k++]=hex[a[i]>>4];name[k++]=hex[a[i]&15U];}name[k++]=L'.';
 for(i=0;i<16U;i++){name[k++]=hex[g[i]>>4];name[k++]=hex[g[i]&15U];}name[k]=0;
 if(wcsncmp(ca->name,name,sizeof(ca->name)/sizeof(ca->name[0])))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&source,&bytes))return PAPACC_RESULT_INVALID_ARGUMENT;
 values[0]=n->parent.dwVolumeSerialNumber;values[1]=n->parent.nFileIndexHigh;values[2]=n->parent.nFileIndexLow;
 for(i=0;i<3U;i++)for(j=0;j<4U;j++)parent[i*4U+j]=(unsigned char)(values[i]>>(j*8U));
 memset(&v,0,sizeof(v));v.operator_id=n->sid;v.operator_bytes=n->sid_bytes;v.parent_id=parent;
 v.leaf=(const unsigned char *)n->plan_leaf;v.leaf_bytes=strlen(n->plan_leaf);v.authority=a;v.generation=g;
 v.service_id=ca->service_sid;v.service_bytes=ca->service_size;v.configuration=p->next;v.configuration_bytes=bytes;
 return papacc_personal_effect_matches(p->personal_data.plan,p->personal_data.bytes,&v)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_ARGUMENT;
}
static int personal_ca_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p=&o->pending_commit;BY_HANDLE_FILE_INFORMATION f;
 return personal_native_ceremony_ready(p)&&personal_packet_current(p)==PAPACC_RESULT_OK&&
 personal_packet_source(p)==PAPACC_RESULT_OK&&GetFileInformationByHandle(p->bootstrap_container,&f)&&
 !(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&
 f.dwVolumeSerialNumber==p->bootstrap_volume&&f.nFileIndexHigh==p->bootstrap_high&&f.nFileIndexLow==p->bootstrap_low&&!p->stop;
}
PAPACC_RESULT papacc_configuration_personal_ca_bind(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *ca,
 const unsigned char id[32],const unsigned char a[16],const unsigned char g[16])
{
 PAPACC_RESULT r;PAPACC_CONFIG_PENDING *p;
 if(!o||!ca||o->writer!=GetCurrentThreadId()||ca->thread!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(o->published||o->prepared||o->committed_pending||o->recovery_selected||o->pending_commit.personal_ca||o->pending_commit.personal_ca_used||ca->borrower)return PAPACC_RESULT_INVALID_STATE;
 r=papacc_configuration_personal_ca_match_data(o,ca,id,a,g);if(r!=PAPACC_RESULT_OK)return r;
 p=&o->pending_commit;
 if(!personal_ca_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 if(papacc_ca_key_validate(ca)!=ERROR_SUCCESS||!personal_ca_native_current(o))return PAPACC_RESULT_INVALID_STATE;
 p->personal_ca_used=1U; /* never rearm after return or failed acquisition */
 if(papacc_ca_key_borrow(ca,p)!=ERROR_SUCCESS)return PAPACC_RESULT_INVALID_STATE;
 p->personal_ca=ca;return PAPACC_RESULT_OK; /* borrow only, never authority */
}
PAPACC_RESULT papacc_configuration_personal_ca_current(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char id[32],const unsigned char a[16],const unsigned char g[16])
{
 PAPACC_CA_KEY *ca;PAPACC_RESULT r;
 if(!o||o->writer!=GetCurrentThreadId()||!(ca=o->pending_commit.personal_ca)||ca->thread!=GetCurrentThreadId()||ca->borrower!=&o->pending_commit)return PAPACC_RESULT_INVALID_STATE;
 r=papacc_configuration_personal_ca_match_data(o,ca,id,a,g);if(r!=PAPACC_RESULT_OK)return r;
 if(!personal_ca_native_current(o)||papacc_ca_key_validate(ca)!=ERROR_SUCCESS||!personal_ca_native_current(o))return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK; /* revalidation for future preparation only */
}
PAPACC_RESULT papacc_configuration_personal_ca_release(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_CA_KEY *ca;
 if(!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;ca=p->personal_ca;
 if(!ca||p->active||p->thread)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_ca_key_return(ca,p)!=ERROR_SUCCESS){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 p->personal_ca=NULL;return PAPACC_RESULT_OK; /* caller still owns CA resources */
}
