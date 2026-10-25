#include "../../tools/qualification/scm-lab-attempt-binding.h"
#ifndef PAPACC_SERVICE_ADMISSION_BD_WIN32_H
#define PAPACC_SERVICE_ADMISSION_BD_WIN32_H
/* Private bounded data/validators inside PS_ADMISSION, not another owner.
 * MODEL evidence exists ONLY in the isolated test translation unit. */
#include <windows.h>
#include <string.h>
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL) || defined(PS_BD_SOURCE_ONLY)
#include "../../tools/qualification/scm-successor-image-pin.h"
#else
#define PS_BD_N0_IMAGE_SHA ""
#endif
#define PS_BD_ROLES 4U
enum { PS_BD_NATIVE_ID=1U,PS_BD_IMAGE_PROVENANCE=2U,PS_BD_FACTUAL_RELATION=4U,PS_BD_SESSION=8U,
 PS_BD_LIVE=16U,PS_BD_METRICS=32U,PS_BD_REVALIDATED=64U,PS_BD_GENERATION=128U };
#define PS_BD_PROOF_ALL 255U
#define PS_BD_MIB 1048576ULL
enum { PS_BD_ADMIN=0,PS_BD_QUALIFIER=1,PS_BD_SERVICE=2,PS_BD_CONSOLE=3 };
typedef struct PS_BD_ID {HANDLE handle;DWORD pid,session;ULONGLONG creation;} PS_BD_ID;
#define PS_BD_N0_POLICY "bec9d0c275d5ee8bd68de4d42fe173a08f124a9900c119e7a08d23757d9d7941"
#define PS_BD_N0_SOURCE 0x4e304e39UL
/* Metrics are checked separately; factual console association stays UNPROVEN. */
#define PS_BD_N0_KNOWN (PS_BD_PROOF_ALL & ~(PS_BD_METRICS | PS_BD_FACTUAL_RELATION))
/* Provenance applies to this OPENED file. No mapped-image/console assertion. */
typedef struct PS_BD_AUX_INPUT {
 PS_BD_ID instance;HANDLE file;BY_HANDLE_FILE_INFORMATION file_id;
 char file_hash[65],security_hash[65],provenance_hash[65];DWORD parent;
} PS_BD_AUX_INPUT;
typedef struct PS_BD_DESCRIPTOR {
 PS_BD_ID actor;HANDLE control_thread,configuration,image;DWORD control_tid;
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD sid_size;TOKEN_STATISTICS statistics;
 BY_HANDLE_FILE_INFORMATION config_id,image_id;
 char attempt[33],plan[65],config_hash[65],image_hash[65],policy[65],host[65];
 DWORD owner_pid;ULONGLONG owner_creation,boot,challenge;unsigned n0;PS_BD_AUX_INPUT auxiliary;
} PS_BD_DESCRIPTOR;
typedef struct PS_BD_PROOF {
 PS_BD_ID actor,console;DWORD source,known;ULONGLONG boot,observed,generation;
 DWORD (*validate)(const PS_BD_DESCRIPTOR *,ULONGLONG);
} PS_BD_PROOF;
typedef struct PS_BD_INVENTORY {
 PS_BD_ID role[PS_BD_ROLES];unsigned present[PS_BD_ROLES],count;PS_BD_PROOF console;
} PS_BD_INVENTORY;
typedef struct PS_BD_COST {
 ULONGLONG private_bytes,commit,working_set,stack;DWORD handles,threads,error;
} PS_BD_COST;
typedef DWORD (*PS_BD_CONTEXT_CHECK)(const PS_BD_DESCRIPTOR *);
/* Protected preparation context belongs to the SAME admission owner. */
typedef struct PS_BD_FILE_RECORD {unsigned long volume,high,low,size,links;char hash[65],security[65],evidence[65];} PS_BD_FILE_RECORD;
typedef struct PS_BD_CONTEXT_STORAGE {
 HANDLE dossier,evidence,osfile,auxiliary,control,actor,actor_token,policy[4],input_root[5];
 PSECURITY_DESCRIPTOR sd;BY_HANDLE_FILE_INFORMATION dossier_id,evidence_id,os_id;
 PS_BD_FILE_RECORD record;char plan[65];PS_BD_DESCRIPTOR descriptor;
 ULONGLONG began,validated_at;unsigned prepared,metric_window;
} PS_BD_CONTEXT_STORAGE;
static __inline int ps_bd_context_pending(const PS_BD_CONTEXT_STORAGE *c)
{return c->prepared||c->metric_window||c->dossier||c->evidence||c->osfile||c->auxiliary||c->control||c->actor||c->actor_token||c->sd||c->policy[0]||c->policy[1]||c->policy[2]||c->policy[3]||c->input_root[0]||c->input_root[1]||c->input_root[2]||c->input_root[3]||c->input_root[4];}
typedef struct PS_BD_STATE {
 PS_BD_DESCRIPTOR binding;PS_BD_INVENTORY inventory;
 HANDLE self,thread,unexpected,input,output,auxiliary,file,aux_token,aux_check_token,actor_check_token;TOKEN_STATISTICS aux_statistics;
 PS_BD_COST costs[PS_BD_ROLES];ULONGLONG metrics_at;unsigned aux_bound,metrics_valid;PSID approved_sid;PS_BD_CONTEXT_CHECK check;unsigned used,bound;PS_BD_CONTEXT_STORAGE context;
} PS_BD_STATE;
static __inline int ps_bd_id(const PS_BD_ID *a,const PS_BD_ID *b)
{return a->pid&&a->creation&&a->handle&&a->handle!=INVALID_HANDLE_VALUE&&b->handle&&b->handle!=INVALID_HANDLE_VALUE&&a->pid==b->pid&&a->creation==b->creation&&a->session==b->session;}
static __inline int ps_bd_hex(const char s[65])
{unsigned i;for(i=0;i<64;i++)if(!((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')))return 0;return !s[64];}
static __inline DWORD ps_bd_descriptor(const PS_BD_DESCRIPTOR *d)
{
 if(!d||!ps_bd_id(&d->actor,&d->actor)||!d->control_thread||d->control_thread==INVALID_HANDLE_VALUE||!d->control_tid||
 !d->configuration||d->configuration==INVALID_HANDLE_VALUE||!d->image||d->image==INVALID_HANDLE_VALUE||
 !d->owner_pid||d->actor.pid==d->owner_pid||!d->owner_creation||!d->boot||!d->challenge||
 d->attempt[32]||strcmp(d->attempt,"" PACC_LAB_ATTEMPT "")||!ps_bd_hex(d->plan)||!ps_bd_hex(d->config_hash)||!ps_bd_hex(d->image_hash)||!ps_bd_hex(d->policy)||!ps_bd_hex(d->host))return ERROR_INVALID_DATA;
 if(d->n0){const PS_BD_AUX_INPUT *a=&d->auxiliary;
  if(d->n0!=1U||d->boot!=236441ULL||strcmp(d->policy,PS_BD_N0_POLICY)||
  strcmp(d->config_hash,"a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44")||
  strcmp(d->image_hash,PS_BD_N0_IMAGE_SHA)||
  strcmp(d->host,"2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff")||
  !ps_bd_id(&a->instance,&a->instance)||a->instance.pid==d->actor.pid||a->instance.pid==d->owner_pid||a->parent!=d->actor.pid||
  a->instance.creation<=d->actor.creation||a->instance.session!=d->actor.session||!a->file||a->file==INVALID_HANDLE_VALUE||
  !a->file_id.dwVolumeSerialNumber||(!a->file_id.nFileIndexHigh&&!a->file_id.nFileIndexLow)||!a->file_id.nNumberOfLinks||
  (a->file_id.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||!ps_bd_hex(a->file_hash)||!ps_bd_hex(a->security_hash)||!ps_bd_hex(a->provenance_hash))return ERROR_INVALID_DATA;
 }
 if(d->sid_size<8||d->sid_size>sizeof(d->sid)||d->sid[0]!=SID_REVISION||d->sid[1]>SID_MAX_SUB_AUTHORITIES||d->sid_size!=8U+4U*d->sid[1])return ERROR_INVALID_SID;
 return 0;
}
static __inline DWORD ps_bd_console(const PS_BD_DESCRIPTOR *d,const PS_BD_INVENTORY *v,ULONGLONG now)
{
 const PS_BD_PROOF *p=&v->console;
 if(!v->present[PS_BD_CONSOLE]||p->known!=(d->n0?PS_BD_N0_KNOWN:PS_BD_PROOF_ALL)||!ps_bd_id(&d->actor,&p->actor)||
 !ps_bd_id(&v->role[PS_BD_CONSOLE],&p->console)||p->boot!=d->boot||p->generation!=d->challenge||
 p->console.session!=d->actor.session||now<p->observed||now-p->observed>60000U)return ERROR_ACCESS_DENIED;
 if(d->n0){
  if(!ps_bd_id(&d->auxiliary.instance,&p->console))return ERROR_INVALID_DATA;
#ifdef PAPACC_N0_N9_NATIVE_OWNER
  if(p->source==PS_BD_N0_SOURCE&&p->validate)return p->validate(d,now);
#endif
#ifdef PAPACC_SERVICE_BD_MODEL
  if(p->source==0x4d4f444cUL)return 0;
#endif
  return ERROR_NOT_SUPPORTED;
 }
 /* No native source qualified yet. A mask/source supplied by a caller cannot
  * assert factual association. Positive cases are synthetic tests ONLY. */
#ifdef PAPACC_SERVICE_BD_MODEL
 if(p->source==0x4d4f444cUL)return 0;
#endif
 return ERROR_NOT_SUPPORTED;
}
static __inline DWORD ps_bd_inventory(const PS_BD_DESCRIPTOR *d,const PS_BD_INVENTORY *v,ULONGLONG now)
{
 unsigned i,j,n=0;DWORD e=ps_bd_descriptor(d);if(e)return e;
 if(!v||v->count>PS_BD_ROLES||!v->present[PS_BD_ADMIN]||!v->present[PS_BD_QUALIFIER])return ERROR_INVALID_DATA;
 for(i=0;i<PS_BD_ROLES;i++){
  if(v->present[i]>1U)return ERROR_INVALID_DATA;
  if(!v->present[i]){if(v->role[i].pid||v->role[i].handle||v->role[i].creation)return ERROR_INVALID_DATA;continue;}
  n++;if(!ps_bd_id(&v->role[i],&v->role[i])||(i==PS_BD_SERVICE&&d->n0?v->role[i].session!=0U:v->role[i].session!=d->actor.session))return ERROR_INVALID_DATA;
  for(j=0;j<i;j++)if(v->present[j]&&(v->role[i].pid==v->role[j].pid||v->role[i].handle==v->role[j].handle))return ERROR_INVALID_DATA;
 }
 if(n!=v->count||!ps_bd_id(&d->actor,&v->role[PS_BD_ADMIN])||v->role[PS_BD_QUALIFIER].pid!=d->owner_pid||v->role[PS_BD_QUALIFIER].creation!=d->owner_creation)return ERROR_INVALID_DATA;
 return ps_bd_console(d,v,now);
}
static __inline DWORD ps_bd_census(const PS_BD_DESCRIPTOR *d,const PS_BD_INVENTORY *v,ULONGLONG now,DWORD pid,DWORD parent,unsigned project)
{
 unsigned i;int child=0;DWORD e=ps_bd_inventory(d,v,now);if(e)return e;
 for(i=0;i<PS_BD_ROLES;i++)if(v->present[i]){if(v->role[i].pid==pid){if(d->n0&&i==PS_BD_CONSOLE&&parent!=d->auxiliary.parent)return ERROR_INVALID_DATA;return 0;}if(v->role[i].pid==parent)child=1;}
 return child||project?ERROR_BUSY:0; /* unchanged host baseline, never a name exemption */
}
static __inline DWORD ps_bd_costs(const PS_BD_INVENTORY *v,const PS_BD_COST c[PS_BD_ROLES])
{
 unsigned i;ULONGLONG a=0,b=0,w=0,s=0;DWORD h=0,t=0;
 for(i=0;i<PS_BD_ROLES;i++)if(v->present[i]){
  const PS_BD_COST *p=&c[i];ULONGLONG cap=(i==PS_BD_ADMIN||i==PS_BD_CONSOLE?256:64)*PS_BD_MIB;
  if(p->error)return p->error;
  if(p->private_bytes>cap||p->commit>cap||p->working_set>cap||p->stack>(i==PS_BD_ADMIN||i==PS_BD_CONSOLE?32:8)*PS_BD_MIB||
  p->handles>(i==PS_BD_ADMIN||i==PS_BD_CONSOLE?1024U:128U)||!p->threads||p->threads>(i==PS_BD_ADMIN||i==PS_BD_CONSOLE?64U:16U))return ERROR_NOT_ENOUGH_QUOTA;
  if(i==PS_BD_ADMIN||i==PS_BD_CONSOLE){a+=p->private_bytes;b+=p->commit;w+=p->working_set;s+=p->stack;h+=p->handles;t+=p->threads;}
 }
 /* Values are bounded above before summation: no unchecked unbounded input. */
 return a<=256*PS_BD_MIB&&b<=256*PS_BD_MIB&&w<=256*PS_BD_MIB&&s<=32*PS_BD_MIB&&h<=1024U&&t<=64U?0:ERROR_NOT_ENOUGH_QUOTA;
}
#endif
