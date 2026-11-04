/* Same writer/packet protects one borrow from the existing CA owner.
 * Explicit initialization has separate one-shot/native gates. No private
 * export, signature, publication or new manager. */
#include "server_issuer_x509_win32.h"
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
 if(n->runtime_identity.phase&&(n->runtime_identity.phase!=1U||ca->service_size!=n->runtime_identity.sid_bytes||
 memcmp(ca->service_sid,n->runtime_identity.sid,ca->service_size)))return PAPACC_RESULT_INVALID_STATE;
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
 if(p->personal_native.runtime_identity.phase&&papacc_configuration_personal_runtime_current(o)!=PAPACC_RESULT_OK)return 0;
 return personal_native_ceremony_ready(p)&&personal_packet_current(p)==PAPACC_RESULT_OK&&
 personal_packet_source(p)==PAPACC_RESULT_OK&&GetFileInformationByHandle(p->bootstrap_container,&f)&&
 !(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&
 f.dwVolumeSerialNumber==p->bootstrap_volume&&f.nFileIndexHigh==p->bootstrap_high&&f.nFileIndexLow==p->bootstrap_low&&!p->stop;
}
static int ca_initialize_overlap(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE z)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;if(n>MAXULONG_PTR-x||z>MAXULONG_PTR-y)return 1;return x<y+z&&y<x+n;}
static SECURITY_STATUS ca_initialize_fail(PAPACC_CONFIGURATION_COMMIT *o,SECURITY_STATUS error)
{
 PAPACC_CONFIG_PENDING *p=&o->pending_commit;
 if(!p->ca_initialize_first)p->ca_initialize_first=error;
 p->ca_initialize_phase=4U;o->unresolved=1;return p->ca_initialize_first;
}
SECURITY_STATUS papacc_configuration_personal_ca_initialize(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *ca,
 const unsigned char id[32],const unsigned char a[16],const unsigned char g[16],const unsigned char *service,DWORD service_bytes,PAPACC_SIZE reserved)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC spec;
 const unsigned char *source;size_t bytes;BYTE parent[12];DWORD values[3];unsigned i,j;SECURITY_STATUS status;
 PAPACC_SIZE local=sizeof(spec)+sizeof(parent)+sizeof(values)+sizeof(source)+sizeof(bytes)+sizeof(status)+sizeof(i)+sizeof(j)+sizeof(p)+sizeof(n);
 if(!o||!ca||!id||!a||!g||!service||!service_bytes||service_bytes>SECURITY_MAX_SID_SIZE||o->writer!=GetCurrentThreadId())return NTE_INVALID_PARAMETER;
 if(ca_initialize_overlap(ca,sizeof(*ca),o,sizeof(*o))||ca_initialize_overlap(ca,sizeof(*ca),id,32)||
  ca_initialize_overlap(ca,sizeof(*ca),a,16)||ca_initialize_overlap(ca,sizeof(*ca),g,16)||ca_initialize_overlap(ca,sizeof(*ca),service,service_bytes))return NTE_INVALID_PARAMETER;
 p=&o->pending_commit;n=&p->personal_native;
 if(reserved<sizeof(*o)+sizeof(*ca)||reserved>PAPACC_ED11_PROCESS_LIMIT||local>PAPACC_ED11_PROCESS_LIMIT-reserved)return NTE_NO_MEMORY;
 if(o->published||o->prepared||o->committed_pending||o->recovery_selected||o->recovery_guard||o->stopped||o->unresolved||o->authority_lost||
  p->personal_ca||p->personal_ca_candidate||p->personal_ca_used||p->ca_initialize_phase||p->active||p->thread||p->stop||p->close_error||
  ca->state!=PAPACC_CA_KEY_EMPTY||ca->key||ca->provider||ca->thread||ca->borrower||ca->first_failure.present||ca->close_attempted||
  !p->personal_data.effectful||!p->personal_data.consumed||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONSUMED||
  memcmp(id,p->personal_data.attempt,32)||memcmp(id,n->handoff_attempt,32)||n->phase!=1U||n->handoff_state!=PAPACC_PERSONAL_HANDOFF_VERIFIED||
  !p->bootstrap_created||!p->bootstrap_parent||!p->bootstrap_container||p->result!=PAPACC_CAS_SUCCESS||
  !n->plan_leaf[0]||!memchr(n->plan_leaf,0,sizeof(n->plan_leaf))||memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))||
  !personal_ca_native_current(o))return NTE_BAD_KEY_STATE;
 if(!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&source,&bytes))return NTE_INVALID_PARAMETER;
 values[0]=n->parent.dwVolumeSerialNumber;values[1]=n->parent.nFileIndexHigh;values[2]=n->parent.nFileIndexLow;
 for(i=0;i<3U;i++)for(j=0;j<4U;j++)parent[i*4U+j]=(BYTE)(values[i]>>(j*8U));
 memset(&spec,0,sizeof(spec));spec.operator_id=n->sid;spec.operator_bytes=n->sid_bytes;spec.parent_id=parent;
 spec.leaf=(const BYTE *)n->plan_leaf;spec.leaf_bytes=strlen(n->plan_leaf);spec.authority=a;spec.generation=g;
 spec.service_id=service;spec.service_bytes=service_bytes;spec.configuration=p->next;spec.configuration_bytes=bytes;
 if(!papacc_personal_effect_matches(p->personal_data.plan,p->personal_data.bytes,&spec)||!personal_ca_native_current(o))return NTE_INVALID_PARAMETER;
 /* No callback/worker turn between taking the attempt and the one native
  * create. Even failure before finalize never rearms this consumed act. */
 p->ca_initialize_phase=1U;p->personal_ca_candidate=ca;
 status=papacc_ca_key_create(ca,a,g,(PSID)n->sid,(PSID)service);
 if(status!=ERROR_SUCCESS)return ca_initialize_fail(o,status);
 if(papacc_configuration_personal_ca_match_data(o,ca,id,a,g)!=PAPACC_RESULT_OK||!personal_ca_native_current(o))return ca_initialize_fail(o,NTE_BAD_KEY_STATE);
 p->ca_initialize_phase=2U;return ERROR_SUCCESS;
}
PAPACC_RESULT papacc_configuration_personal_ca_bind(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *ca,
 const unsigned char id[32],const unsigned char a[16],const unsigned char g[16])
{
 PAPACC_RESULT r;PAPACC_CONFIG_PENDING *p;
 if(!o||!ca||o->writer!=GetCurrentThreadId()||ca->thread!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(o->published||o->prepared||o->committed_pending||o->recovery_selected||o->pending_commit.personal_ca||o->pending_commit.personal_ca_used||ca->borrower||
  (o->pending_commit.personal_ca_candidate&&(o->pending_commit.personal_ca_candidate!=ca||o->pending_commit.ca_initialize_phase!=2U)))return PAPACC_RESULT_INVALID_STATE;
 r=papacc_configuration_personal_ca_match_data(o,ca,id,a,g);if(r!=PAPACC_RESULT_OK)return r;
 p=&o->pending_commit;
 if(!personal_ca_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 if(papacc_ca_key_validate(ca)!=ERROR_SUCCESS||!personal_ca_native_current(o))return PAPACC_RESULT_INVALID_STATE;
 p->personal_ca_used=1U; /* never rearm after return or failed acquisition */
 if(papacc_ca_key_borrow(ca,p)!=ERROR_SUCCESS){if(p->personal_ca_candidate)(void)ca_initialize_fail(o,NTE_BAD_KEY_STATE);return PAPACC_RESULT_INVALID_STATE;}
 p->personal_ca=ca;if(p->personal_ca_candidate){p->personal_ca_candidate=NULL;p->ca_initialize_phase=3U;}
 return PAPACC_RESULT_OK; /* borrow only, never authority */
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
PAPACC_RESULT papacc_configuration_personal_ca_handoff(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CNG_BACKEND *backend,
 const unsigned char id[32],const unsigned char a[16],const unsigned char g[16])
{
 PAPACC_CONFIG_PENDING *p;PAPACC_CA_KEY *ca;PAPACC_RESULT result;
 if(!o||!backend||o->writer!=GetCurrentThreadId()||backend->thread!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;ca=p->personal_ca;
 if(!ca||!backend->initialized||backend->stopped||backend->retained||backend->active||backend->custody||
  backend->key!=ca||backend->ports.ctx!=backend||!backend->owner||backend->owner->busy||backend->owner->lease||
  backend->owner->retained||backend->owner->retained_output||backend->owner->pending.active||backend->owner->initialized==2)
  return PAPACC_RESULT_INVALID_STATE;
 if(backend->owner->initialized&&backend->owner->continuity_profile!=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)return PAPACC_RESULT_INVALID_STATE;
 if(backend->owner->initialized&&(backend->owner->backend!=&backend->ports||
  backend->owner->root.ctx!=backend->root.ctx||backend->owner->root.read!=backend->root.read||backend->owner->root.cas!=backend->root.cas))
  return PAPACC_RESULT_INVALID_STATE;
 result=papacc_configuration_personal_ca_current(o,id,a,g);if(result!=PAPACC_RESULT_OK)return result;
 /* No native callback between transfer and recipient metadata publication. */
 if(papacc_ca_key_transfer_borrow(ca,p,backend)!=ERROR_SUCCESS){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 backend->original_source_root=o;memcpy(backend->original_attempt,id,32);memcpy(backend->original_authority,a,16);memcpy(backend->original_generation,g,16);
 backend->source_kind=PAPACC_CA_SOURCE_NEW_INSTALL;backend->custody=1;p->personal_ca=NULL;return PAPACC_RESULT_OK;
}
