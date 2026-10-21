#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include "server_configuration_commit_win32.h"
#include <ktmw32.h>
#include <bcrypt.h>
#include <string.h>
#include <limits.h>

static int owner(PAPACC_CONFIGURATION_COMMIT *o)
{return o!=NULL && o->initialized && o->writer==GetCurrentThreadId();}
static int pending_blocks(PAPACC_CONFIGURATION_COMMIT *o)
{
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 return o->pending_commit.active||o->authority_lost||o->pending_commit.bootstrap_parent||o->pending_commit.bootstrap_container;
#else
 (void)o;return 0;
#endif
}
static void put64(unsigned char *p,PAPACC_U64 v)
{unsigned i;for(i=0U;i<8U;++i){p[7U-i]=(unsigned char)(v&255U);v>>=8;}}
static PAPACC_U64 get64(const unsigned char *p)
{unsigned i;PAPACC_U64 v=0U;for(i=0U;i<8U;++i)v=(v<<8)|p[i];return v;}
static int digest(unsigned char *record,int verify)
{
    BCRYPT_ALG_HANDLE alg=NULL;
    BCRYPT_HASH_HANDLE hash=NULL;
    unsigned char value[32];
    NTSTATUS status;
    int ok=0;
    status=BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,NULL,0U);
    if(status<0)return 0;
    status=BCryptCreateHash(alg,&hash,NULL,0U,NULL,0U,0U);
    if(status>=0)status=BCryptHashData(hash,record,480U,0U);
    if(status>=0)status=BCryptFinishHash(hash,value,32U,0U);
    if(status>=0){ok=!verify || memcmp(value,record+480U,32U)==0;
        if(!verify)memcpy(record+480U,value,32U);}
    if(hash!=NULL && BCryptDestroyHash(hash)<0)ok=0;
    if(BCryptCloseAlgorithmProvider(alg,0U)<0)ok=0;
    return ok;
}
/* Parent chain and final leaf are checked without following a reparse point.
 * Supported diagnostic fault model excludes concurrent privileged path mutation. */
static int path_ok(const char *path)
{
    char copy[MAX_PATH];size_t len,i;DWORD a;
    if(path==NULL)return 0;
    len=strlen(path);
    if(len<4U||len>=MAX_PATH||path[1]!=':'||path[2]!='\\')return 0;
    if(strstr(path,"..")!=NULL||strchr(path+2,':')!=NULL)return 0;
    memcpy(copy,path,len+1U);
    for(i=3U;i<len;++i)if(copy[i]=='\\'){
        copy[i]=0;a=GetFileAttributesA(copy);copy[i]='\\';
        if(a==INVALID_FILE_ATTRIBUTES||!(a&FILE_ATTRIBUTE_DIRECTORY)||
            (a&FILE_ATTRIBUTE_REPARSE_POINT))return 0;
    }
    a=GetFileAttributesA(copy);
    return a==INVALID_FILE_ATTRIBUTES ? GetLastError()==ERROR_FILE_NOT_FOUND :
        !(a&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY));
}
static void fix(PAPACC_CONFIG_VIEW *v)
{v->config.bind_selection.interface_persistent_ids=
 v->config.bind_selection.interface_persistent_id_count!=0U?v->ids:NULL;}
static void encode(const PAPACC_CONFIG_VIEW *v,unsigned char *b)
{
    unsigned i,j,pos=70U;const PAPACC_SERVER_CONFIG *c=&v->config;
    const PAPACC_U8 *refs[4];PAPACC_SIZE sizes[4];
    refs[0]=c->security_configuration_ref;sizes[0]=c->security_configuration_ref_length;
    refs[1]=c->egress_policy_ref;sizes[1]=c->egress_policy_ref_length;
    refs[2]=c->egress_profile_ref;sizes[2]=c->egress_profile_ref_length;
    refs[3]=c->outbound_tls_profile_ref;sizes[3]=c->outbound_tls_profile_ref_length;
    memset(b,0,PAPACC_CONFIG_RECORD_BYTES);memcpy(b,"PACCFC01",8U);
    memcpy(b+8U,v->domain,32U);put64(b+40U,v->revision);
    put64(b+48U,v->predecessor);put64(b+56U,v->policy_epoch);
    b[64]=(unsigned char)(c->control_port>>8);b[65]=(unsigned char)c->control_port;
    b[66]=(unsigned char)c->listener_profile;b[67]=(unsigned char)c->allow_network_egress;
    b[68]=(unsigned char)c->bind_selection.mode;
    b[69]=(unsigned char)c->bind_selection.interface_persistent_id_count;
    for(i=0U;i<4U;++i){b[pos++]=(unsigned char)sizes[i];
        for(j=0U;j<sizes[i];++j)b[pos+j]=refs[i][j];pos+=64U;}
    for(i=0U;i<c->bind_selection.interface_persistent_id_count;++i)
        put64(b+330U+8U*i,v->ids[i].value);
}
static int record_valid(unsigned char *b)
{
    PAPACC_U64 rev=get64(b+40U),pred=get64(b+48U);
    unsigned i;
    if(memcmp(b,"PACCFC01",8U)!=0||rev==0U||pred==((PAPACC_U64)-1)||
        rev!=pred+1U||get64(b+56U)==0U||b[69]>PAPACC_CONFIG_IDS)return 0;
    for(i=0U;i<4U;++i)if(b[70U+i*65U]>64U)return 0;
    return digest(b,1);
}
#include "server_configuration_store02_win32.inc"
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "server_configuration_store03_win32.inc"
#include "server_configuration_body04_win32.h"
#include "server_configuration_body04_win32.inc"
#include "server_configuration_store04_win32.inc"
#include "server_configuration_body05_win32.h"
#include "server_configuration_body05_win32.inc"
#include "server_configuration_store05_win32.inc"
#include "server_configuration_issuer06_win32.inc"
#endif

PAPACC_RESULT papacc_configuration_commit_init(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{
    DWORD a;
    if(o==NULL||o->initialized||o->writer!=0U||!path_ok(path))return PAPACC_RESULT_INVALID_ARGUMENT;
    a=GetFileAttributesA(path);
    if(a!=INVALID_FILE_ATTRIBUTES||GetLastError()!=ERROR_FILE_NOT_FOUND)
        return PAPACC_RESULT_INVALID_STATE;
    if(BCryptGenRandom(NULL,o->domain,32U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)
        return PAPACC_RESULT_INTERNAL_ERROR;
    if(!InitializeCriticalSectionAndSpinCount(&o->lock,0U))
        return PAPACC_RESULT_OUT_OF_MEMORY;
    memcpy(o->path,path,strlen(path)+1U);o->writer=GetCurrentThreadId();
    o->retained_file=INVALID_HANDLE_VALUE;o->retained_transaction=INVALID_HANDLE_VALUE;
    o->initialized=1;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_prepare(PAPACC_CONFIGURATION_COMMIT *o,
    const PAPACC_SERVER_CONFIG *config,PAPACC_U64 expected,PAPACC_U64 epoch)
{
    unsigned i;PAPACC_CONFIG_SLOT *s;PAPACC_U64 current;
    if(!owner(o)||config==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(papacc_server_config_validate(config)!=PAPACC_RESULT_OK||
        ((o->store02==3||o->store02==4||o->store02==5)?epoch!=0U:epoch==0U)||
        config->bind_selection.interface_persistent_id_count>PAPACC_CONFIG_IDS)
        return PAPACC_RESULT_INVALID_ARGUMENT;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(((o->store02==3||o->store02==4||o->store02==5) && !config->policy.present) ||
       ((o->store02!=3&&o->store02!=4&&o->store02!=5) && config->policy.present))return PAPACC_RESULT_NOT_SUPPORTED;
    if(o->store02!=5&&config->site_tls.present)return PAPACC_RESULT_NOT_SUPPORTED;
    if((o->store02==4||o->store02==5)!=(config->policy.representation==PAPACC_ADMIN_REPRESENTATION_04))return PAPACC_RESULT_NOT_SUPPORTED;
#endif
    EnterCriticalSection(&o->lock);
    if(o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    current=o->published?o->slots[o->current].view.revision:0U;
    if(o->store02 && current>=((o->store02==3||o->store02==4||o->store02==5)?4U:STORE02_CAP)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_LIMIT_EXCEEDED;}
    if((o->published && epoch<o->slots[o->current].view.policy_epoch)||
        expected!=current||expected==(PAPACC_U64)-1||o->next_serial==(PAPACC_U64)-1){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    /* Profile changes require full participant rebootstrap, not this component. */
    if(o->published && config->listener_profile!=o->slots[o->current].view.config.listener_profile){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
    for(i=0U;i<PAPACC_CONFIG_SLOTS;++i)if((!o->published||i!=o->current)&&
        o->slots[i].readers==0U&&!o->slots[i].retained)break;
    if(i==PAPACC_CONFIG_SLOTS){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_LIMIT_EXCEEDED;}
    s=&o->slots[i];memset(&s->view,0,sizeof(s->view));s->view.config=*config;
    if(config->bind_selection.interface_persistent_id_count!=0U)
        memcpy(s->view.ids,config->bind_selection.interface_persistent_ids,
            config->bind_selection.interface_persistent_id_count*sizeof(s->view.ids[0]));
    fix(&s->view);memcpy(s->view.domain,o->domain,32U);
    s->view.predecessor=expected;s->view.revision=expected+1U;s->view.policy_epoch=epoch;
    s->serial=++o->next_serial;o->pending=i;o->prepared=1;
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_commit(PAPACC_CONFIGURATION_COMMIT *o)
{
    unsigned char next[PAPACC_CONFIG_STORAGE_BYTES],previous[PAPACC_CONFIG_STORAGE_BYTES];
    HANDLE tx,file;DWORD count,bytes,previous_bytes;int ok,closed,rolled;BY_HANDLE_FILE_INFORMATION info;
    if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
    if(!o->prepared||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)||!path_ok(o->path)){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    bytes=o->store02?PAPACC_CONFIG_STORE_BYTES:PAPACC_CONFIG_RECORD_BYTES;
    previous_bytes=bytes;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->issuer_stage_prepared){bytes=PAPACC_ISSUER_ROOT_IMAGE_BYTES;previous_bytes=o->committed_bytes;}
    else if(o->store02==6){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
#endif
    if(!o->store02){encode(&o->slots[o->pending].view,next);
        if(!digest(next,0)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}}
    tx=CreateTransaction(NULL,NULL,0U,0U,0U,0U,NULL);
    if(tx==INVALID_HANDLE_VALUE){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
    file=CreateFileTransactedA(o->path,GENERIC_READ|GENERIC_WRITE,0U,NULL,
        o->published?OPEN_EXISTING:CREATE_NEW,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,
        NULL,tx,NULL,NULL);
    ok=file!=INVALID_HANDLE_VALUE;
    if(ok&&o->published){
        LARGE_INTEGER size;
        ok=GetFileSizeEx(file,&size)&&size.QuadPart==previous_bytes;
        if(ok)ok=ReadFile(file,previous,previous_bytes,&count,NULL)&&count==previous_bytes&&memcmp(previous,o->committed,previous_bytes)==0;
        if(ok)ok=SetFilePointer(file,0,NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER;
    }
    if(ok&&o->store02){
        ok=GetFileInformationByHandle(file,&info)!=0;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
        if(ok&&o->issuer_stage_prepared)ok=issuer06_append(o,&o->slots[o->pending].view,&info,next);
        else if(ok&&o->store02==5)ok=store05_append(o,&o->slots[o->pending].view,&info,next);
        else if(ok&&o->store02==4)ok=store04_append(o,&o->slots[o->pending].view,&info,next);
        else if(ok&&o->store02==3)ok=store03_append(o,&o->slots[o->pending].view,&info,next);
        else
#endif
        if(ok)ok=store02_append(o,&o->slots[o->pending].view,&info,next);
    }
    if(ok)ok=WriteFile(file,next,bytes,&count,NULL)&&count==bytes&&SetEndOfFile(file)&&FlushFileBuffers(file);
    closed=file==INVALID_HANDLE_VALUE||CloseHandle(file);
    if(!closed){o->retained_file=file;o->retained_transaction=tx;o->unresolved=1;
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}
    if(!ok){rolled=RollbackTransaction(tx)!=0;
        if(!rolled){o->retained_transaction=tx;o->unresolved=1;}
        else if(!CloseHandle(tx)){o->retained_transaction=tx;o->unresolved=1;}
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}
    if(!CommitTransaction(tx)){o->retained_transaction=tx;o->unresolved=1;
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}
    memcpy(o->committed,next,bytes);o->prepared=0;o->committed_pending=1;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    o->committed_bytes=bytes;
#endif
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->issuer_stage_prepared){
        memcpy(o->issuer_snapshots[o->pending],o->issuer_staged,PAPACC_ISSUER_IMAGE_BYTES);
        o->issuer_snapshot_valid[o->pending]=1;
        o->store02=6;o->issuer_stage_prepared=0;
    }
#endif
    if(!CloseHandle(tx)){o->retained_transaction=tx;o->unresolved=1;
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_publish(PAPACC_CONFIGURATION_COMMIT *o)
{
    if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
#ifndef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->store02==6){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
#endif
#endif
    if(o->recovery_selected){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
    if(!o->committed_pending||o->stopped||o->unresolved||pending_blocks(o)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    o->current=o->pending;o->committed_pending=0;o->published=1;
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_abort(PAPACC_CONFIGURATION_COMMIT *o)
{
    if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
    if(!o->prepared||o->committed_pending||o->unresolved||pending_blocks(o)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    o->issuer_stage_prepared=0;
#endif
    o->prepared=0;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_pin(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CONFIG_PIN *p,PAPACC_CONFIG_VIEW *v)
{
    PAPACC_CONFIG_SLOT *s;unsigned i;
    if(o==NULL||!o->initialized||p==NULL||v==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    EnterCriticalSection(&o->lock);
    if(!o->published||o->committed_pending||o->stopped||o->unresolved||pending_blocks(o)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    s=&o->slots[o->current];if(s->readers==ULONG_MAX){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_LIMIT_EXCEEDED;}
    for(i=0U;i<32U;++i)if(o->tickets[i].ticket==0U)break;
    if(i==32U||o->next_ticket==(PAPACC_U64)-1){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_LIMIT_EXCEEDED;}
    ++s->readers;p->issuer=o;p->index=o->current;p->serial=s->serial;p->ticket=++o->next_ticket;
    o->tickets[i]=*p;*v=s->view;fix(v);
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_unpin(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,int retained)
{
    PAPACC_CONFIG_SLOT *s;unsigned i;
    if(o==NULL||!o->initialized||p==NULL||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS)return PAPACC_RESULT_INVALID_ARGUMENT;
    EnterCriticalSection(&o->lock);s=&o->slots[p->index];
    for(i=0U;i<32U;++i)if(p->ticket!=0U&&o->tickets[i].ticket==p->ticket&&
        o->tickets[i].serial==p->serial&&o->tickets[i].index==p->index)break;
    if(i==32U||s->serial!=p->serial||s->readers==0U){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    o->tickets[i].ticket=0U;--s->readers;if(retained)s->retained=1;
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_BOOL papacc_configuration_current(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_VIEW *v)
{
    int ok;PAPACC_CONFIG_VIEW *c;
    if(o==NULL||!o->initialized||v==NULL)return PAPACC_FALSE;
    EnterCriticalSection(&o->lock);c=&o->slots[o->current].view;
    ok=o->published&&!o->committed_pending&&!o->stopped&&!o->unresolved&&!pending_blocks(o)&&
        memcmp(c->domain,v->domain,32U)==0&&c->revision==v->revision&&c->policy_epoch==v->policy_epoch;
    LeaveCriticalSection(&o->lock);return ok?PAPACC_TRUE:PAPACC_FALSE;
}
PAPACC_RESULT papacc_configuration_stop(PAPACC_CONFIGURATION_COMMIT *o)
{if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 (void)InterlockedExchange(&o->pending_commit.stop,1);
#endif
 EnterCriticalSection(&o->lock);o->stopped=1;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_configuration_finish(PAPACC_CONFIGURATION_COMMIT *o)
{
    unsigned i;if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
    if(!o->stopped||o->prepared||o->committed_pending||o->unresolved||pending_blocks(o)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    for(i=0U;i<PAPACC_CONFIG_SLOTS;++i)if(o->slots[i].readers||o->slots[i].retained){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    /* Caller must establish reader/callback quiescence before finish. */
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->recovery_guard){
        if(!CloseHandle(o->retained_file)){o->unresolved=1;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INTERNAL_ERROR;}
        o->retained_file=INVALID_HANDLE_VALUE;o->recovery_guard=0;
    }
#endif
    o->initialized=0;LeaveCriticalSection(&o->lock);DeleteCriticalSection(&o->lock);
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_commit_init02(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{
    PAPACC_RESULT result=papacc_configuration_commit_init(o,path);
    if(result==PAPACC_RESULT_OK)o->store02=1;
    return result;
}
PAPACC_RESULT papacc_configuration_recovery_init02(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{
    DWORD a;
    if(o==NULL||o->initialized||o->writer!=0U||!path_ok(path))return PAPACC_RESULT_INVALID_ARGUMENT;
    a=GetFileAttributesA(path);if(a==INVALID_FILE_ATTRIBUTES)return PAPACC_RESULT_INVALID_STATE;
    if(!InitializeCriticalSectionAndSpinCount(&o->lock,0U))return PAPACC_RESULT_OUT_OF_MEMORY;
    memcpy(o->path,path,strlen(path)+1U);o->writer=GetCurrentThreadId();
    o->retained_file=INVALID_HANDLE_VALUE;o->retained_transaction=INVALID_HANDLE_VALUE;
    o->store02=1;o->initialized=1;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_recovery_inspect(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{
    HANDLE f;unsigned char b[PAPACC_CONFIG_STORE_BYTES];DWORD n,bytes;LARGE_INTEGER size;int ok;
    BY_HANDLE_FILE_INFORMATION info;PAPACC_CONFIG_VIEW selected;unsigned i;
    if(!owner(o)||o->unresolved||pending_blocks(o)||!path_ok(path))return PAPACC_RESULT_INVALID_ARGUMENT;
    if(o->store02 && strcmp(path,o->path)!=0)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(o->store02 && o->recovery_selected)return PAPACC_RESULT_NOT_SUPPORTED;
    bytes=o->store02?PAPACC_CONFIG_STORE_BYTES:PAPACC_CONFIG_RECORD_BYTES;
    f=CreateFileA(path,GENERIC_READ,0U,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
    if(f==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
    ok=GetFileInformationByHandle(f,&info)&&GetFileSizeEx(f,&size)&&size.QuadPart==bytes&&ReadFile(f,b,bytes,&n,NULL)&&n==bytes;
    if(!CloseHandle(f)){o->retained_file=f;o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;}
    if(!ok)return PAPACC_RESULT_INVALID_STATE;
    if(!o->store02)return record_valid(b)?PAPACC_RESULT_NOT_SUPPORTED:PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->store02==5){if(!store05_select(b,&info,&selected))return PAPACC_RESULT_INVALID_STATE;}
    else if(o->store02==4){if(!store04_select(b,&info,&selected))return PAPACC_RESULT_INVALID_STATE;}
    else if(o->store02==3){if(!store03_select(b,&info,&selected))return PAPACC_RESULT_INVALID_STATE;}
    else
#endif
    if(!store02_select(b,&info,&selected))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
    if(o->published||o->prepared||o->committed_pending||o->stopped||o->recovery_selected||o->next_serial!=0U){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;}
    for(i=0U;i<PAPACC_CONFIG_SLOTS;++i)if(o->slots[i].readers||o->slots[i].retained){
        LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    /* Selection is factual; missing real leases/current-policy/capability
     * provenance prevents operational publication. No default authority. */
    memcpy(o->domain,selected.domain,32U);memcpy(o->committed,b,bytes);
    o->slots[0].view=selected;fix(&o->slots[0].view);o->slots[0].serial=++o->next_serial;
    o->current=0U;o->recovery_selected=1;
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_NOT_SUPPORTED;
}
PAPACC_RESULT papacc_configuration_recovered_copy(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CONFIG_VIEW *v)
{
    if(v==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    memset(v,0,sizeof(*v));
    if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
    EnterCriticalSection(&o->lock);
    if(!o->recovery_selected||o->unresolved){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
    *v=o->slots[o->current].view;fix(v);
    LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}

#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_RESULT papacc_configuration_commit_init05(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{PAPACC_RESULT r=papacc_configuration_commit_init(o,path);if(r==PAPACC_RESULT_OK)o->store02=5;return r;}
PAPACC_RESULT papacc_configuration_prepare05(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SERVER_CONFIG *c,PAPACC_U64 expected)
{if(o==NULL||o->store02!=5||!c||!c->site_tls.present)return PAPACC_RESULT_INVALID_STATE;return papacc_configuration_prepare(o,c,expected,0U);}
PAPACC_RESULT papacc_configuration_commit_init04(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{PAPACC_RESULT r=papacc_configuration_commit_init(o,path);if(r==PAPACC_RESULT_OK)o->store02=4;return r;}
PAPACC_RESULT papacc_configuration_recovery_init04(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{PAPACC_RESULT r=papacc_configuration_recovery_init02(o,path);if(r==PAPACC_RESULT_OK)o->store02=4;return r;}
PAPACC_RESULT papacc_configuration_prepare04(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SERVER_CONFIG *c,PAPACC_U64 expected)
{if(o==NULL||o->store02!=4)return PAPACC_RESULT_INVALID_STATE;return papacc_configuration_prepare(o,c,expected,0U);}
PAPACC_RESULT papacc_configuration_commit_init03(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{PAPACC_RESULT r=papacc_configuration_commit_init(o,path);if(r==PAPACC_RESULT_OK)o->store02=3;return r;}
PAPACC_RESULT papacc_configuration_recovery_init03(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{PAPACC_RESULT r=papacc_configuration_recovery_init02(o,path);if(r==PAPACC_RESULT_OK)o->store02=3;return r;}
PAPACC_RESULT papacc_configuration_prepare03(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SERVER_CONFIG *c,PAPACC_U64 expected)
{if(o==NULL||o->store02!=3)return PAPACC_RESULT_INVALID_STATE;return papacc_configuration_prepare(o,c,expected,0U);}
#include "server_configuration_restart_win32.inc"
#include "server_configuration_restart04_win32.inc"
#endif


#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "server_configuration_pending_win32.inc"
#endif
