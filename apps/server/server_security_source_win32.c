#include "../../src/core/ed11_process_profile.h"
#include "server_security_source_win32.h"

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pst_provider_bootstrap_win32.h"

typedef struct PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE {
    PAPACC_SECURITY_SOURCE_TRANSACTION *transaction;
    PAPACC_SIZE capacities[3];
    PAPACC_U8 *certificate;
    PAPACC_SIZE certificate_size;
    PAPACC_U8 *private_key;
    PAPACC_SIZE private_key_size;
    PAPACC_U8 *client_ca;
    PAPACC_SIZE client_ca_size;
    PAPACC_SECURITY_DER_ITEM certificate_item;
    PAPACC_SECURITY_DER_ITEM ca_item;
    PAPACC_U8 allowed_client_certificate_sha256[
        PAPACC_CREDENTIAL_SHA256_SIZE];
} PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE;

static void papacc_server_security_zero(void *data, PAPACC_SIZE size)
{
    volatile PAPACC_U8 *bytes = (volatile PAPACC_U8 *)data;
    while (bytes != NULL && size != 0U) {
        *bytes++ = 0U;
        --size;
    }
}

static int source_owned(const PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    return t!=NULL && t->owner_thread==GetCurrentThreadId() &&
        t->state!=PAPACC_SECURITY_SOURCE_EMPTY &&
        t->state!=PAPACC_SECURITY_SOURCE_CLOSED;
}
static void *source_allocate(PAPACC_SECURITY_SOURCE_TRANSACTION *t,PAPACC_SIZE size)
{
    void *p;
    if(t!=NULL){
        if(!source_owned(t) || t->close_uncertain || t->used>t->credit ||
            size>t->credit-t->used){t->failure=PAPACC_RESULT_LIMIT_EXCEEDED;return NULL;}
#ifdef PAPACC_SECURITY_SOURCE_OFFLINE_TEST
        ++t->allocation_attempts;
        if(t->fail_allocation_at!=0U && t->allocation_attempts==t->fail_allocation_at){
            t->failure=PAPACC_RESULT_OUT_OF_MEMORY;return NULL;}
#endif
        t->used+=size;if(t->used>t->peak)t->peak=t->used;
    }
    p=malloc(size);
    if(p==NULL && t!=NULL){t->used-=size;t->failure=PAPACC_RESULT_OUT_OF_MEMORY;}
    return p;
}
static void source_free(PAPACC_SECURITY_SOURCE_TRANSACTION *t,void *p,PAPACC_SIZE size)
{
    if(p==NULL)return;
    papacc_server_security_zero(p,size);free(p);
    if(t!=NULL){
        if(t->used>=size)t->used-=size;
        else {t->close_uncertain=PAPACC_TRUE;t->failure=PAPACC_RESULT_INTERNAL_ERROR;}
    }
}

static void papacc_server_security_environment_release(void *context)
{
    PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *storage =
        (PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *)context;
    PAPACC_SECURITY_SOURCE_TRANSACTION *t;
    if (storage == NULL) return;
    t=storage->transaction;
    if(t!=NULL && (t->state==PAPACC_SECURITY_SOURCE_RETAINED || t->borrowers!=0U))
        return;
    source_free(t,storage->certificate,storage->capacities[0]);
    source_free(t,storage->private_key,storage->capacities[1]);
    source_free(t,storage->client_ca,storage->capacities[2]);
    if(t!=NULL)t->held_storage=NULL;
    source_free(t,storage,sizeof(*storage));
}

/* One existing loader, with optional transaction credit. The historical
 * 16MiB input bound is unchanged. Decoded capacity and encoded staging overlap
 * and are charged simultaneously. No data/content hashes are exported. */
static int papacc_server_security_load_base64(const char *path,
    PAPACC_U8 **data,PAPACC_SIZE *size,PAPACC_SIZE *capacity,
    PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    FILE *file=NULL;char *encoded=NULL;PAPACC_U8 *decoded=NULL;
    long length=0;DWORD needed=0U,written;PAPACC_SIZE encoded_capacity=0U;
    int close_result;
    if(data!=NULL)*data=NULL;if(size!=NULL)*size=0U;if(capacity!=NULL)*capacity=0U;
    if(path==NULL || path[0]=='\0' || data==NULL || size==NULL || capacity==NULL ||
        fopen_s(&file,path,"rb")!=0 || file==NULL)return 0;
    if(fseek(file,0,SEEK_END)!=0 || (length=ftell(file))<=0 ||
        length>16L*1024L*1024L || fseek(file,0,SEEK_SET)!=0)goto fail;
    encoded_capacity=(PAPACC_SIZE)length+1U;
    encoded=source_allocate(t,encoded_capacity);
    if(encoded==NULL || fread(encoded,1U,(size_t)length,file)!=(size_t)length)goto fail;
    close_result=fclose(file);file=NULL;
    if(close_result!=0){if(t!=NULL)t->close_uncertain=PAPACC_TRUE;goto fail;}
    encoded[length]='\0';
    if(!CryptStringToBinaryA(encoded,(DWORD)length,CRYPT_STRING_BASE64,
        NULL,&needed,NULL,NULL) || needed==0U)goto fail;
    decoded=source_allocate(t,(PAPACC_SIZE)needed);
    if(decoded==NULL)goto fail;
    written=needed;
    if(!CryptStringToBinaryA(encoded,(DWORD)length,CRYPT_STRING_BASE64,
        decoded,&written,NULL,NULL) || written==0U || written>needed)goto fail;
    source_free(t,encoded,encoded_capacity);
    *data=decoded;*size=(PAPACC_SIZE)written;*capacity=(PAPACC_SIZE)needed;return 1;
fail:
    if(file!=NULL){close_result=fclose(file);file=NULL;
        if(close_result!=0 && t!=NULL)t->close_uncertain=PAPACC_TRUE;}
    /* fclose consumes the CRT stream even on failure: never retry it. An
     * uncertain native close keeps the transaction/owned material contained. */
    if(t!=NULL && t->close_uncertain){
        t->retained_staging=encoded;t->retained_staging_capacity=encoded_capacity;
        if(decoded!=NULL){*data=decoded;*size=(PAPACC_SIZE)needed;*capacity=(PAPACC_SIZE)needed;}
        t->failure=PAPACC_RESULT_INTERNAL_ERROR;return 0;
    }
    source_free(t,decoded,(PAPACC_SIZE)needed);
    source_free(t,encoded,encoded_capacity);return 0;
}

static PAPACC_RESULT papacc_server_lab_principal_resolve(void *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLUTION *resolution, PAPACC_PRINCIPAL *principal)
{
    PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *storage =
        (PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *)context;
    if (evidence == NULL || resolution == NULL || principal == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *principal = (PAPACC_PRINCIPAL)PAPACC_PRINCIPAL_INITIALIZER;
    if (storage == NULL || evidence->certificate_present != PAPACC_TRUE ||
        evidence->chain_validated != PAPACC_TRUE ||
        evidence->peer_authenticated != PAPACC_TRUE ||
        evidence->certificate_sha256_valid != PAPACC_TRUE) {
        *resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
        return PAPACC_RESULT_OK;
    }
    if (memcmp(evidence->certificate_sha256,
            storage->allowed_client_certificate_sha256,
            PAPACC_CREDENTIAL_SHA256_SIZE) != 0) {
        *resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
        return PAPACC_RESULT_OK;
    }
    memcpy(principal->value, evidence->certificate_sha256,
        PAPACC_PRINCIPAL_VALUE_SIZE);
    principal->valid = PAPACC_TRUE;
    *resolution = PAPACC_PRINCIPAL_RESOLVED;
    return PAPACC_RESULT_OK;
}

static int papacc_server_security_parse_hex_sha256(const char *text,
    PAPACC_U8 output[PAPACC_CREDENTIAL_SHA256_SIZE])
{
    PAPACC_SIZE index;
    if (text == NULL || strlen(text) != PAPACC_CREDENTIAL_SHA256_SIZE * 2U)
        return 0;
    for (index = 0U; index < PAPACC_CREDENTIAL_SHA256_SIZE; ++index) {
        int high = text[index * 2U];
        int low = text[index * 2U + 1U];
        high = high >= '0' && high <= '9' ? high - '0' :
            high >= 'a' && high <= 'f' ? high - 'a' + 10 :
            high >= 'A' && high <= 'F' ? high - 'A' + 10 : -1;
        low = low >= '0' && low <= '9' ? low - '0' :
            low >= 'a' && low <= 'f' ? low - 'a' + 10 :
            low >= 'A' && low <= 'F' ? low - 'A' + 10 : -1;
        if (high < 0 || low < 0) return 0;
        output[index] = (PAPACC_U8)((high << 4) | low);
    }
    return 1;
}

static PAPACC_RESULT papacc_server_lab_authorize(void *context,
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_ACTION action,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    (void)context;
    if (principal == NULL || decision == NULL || principal->valid !=
            PAPACC_TRUE || action < PAPACC_AUTHORIZATION_ACCESS_ACCELERATOR ||
        action > PAPACC_AUTHORIZATION_ASSOCIATE_DATA)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision = PAPACC_AUTHORIZATION_ALLOW;
    return PAPACC_RESULT_OK;
}

/* 1=value, 0=missing, -1=empty/overlong/error. Paths are not secrets,
 * but never exported by this source. No truncation is accepted. */
static int papacc_server_security_environment_value(const char *name,
    char *value,DWORD capacity)
{
    DWORD count,error;
    value[0]='\0';SetLastError(ERROR_SUCCESS);
    count=GetEnvironmentVariableA(name,value,capacity);error=GetLastError();
    if(count==0U)return error==ERROR_ENVVAR_NOT_FOUND?0:-1;
    if(count>=capacity){value[0]='\0';return -1;}
    return 1;
}

static PAPACC_RESULT source_resolve(
    void *context, const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration,
    PAPACC_SECURITY_SOURCE_TRANSACTION *transaction)
{
    PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *storage;
    char expected[PAPACC_SECURITY_CONFIGURATION_REF_CAPACITY+1U];
    char certificate[MAX_PATH];
    char private_key[MAX_PATH];
    char client_ca[MAX_PATH];
    char allowed_client[65U];
    PAPACC_SIZE expected_size;
    int environment_status;
    (void)context;
    if (reference == NULL || resolution == NULL || configuration == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *resolution = PAPACC_SECURITY_CONFIGURATION_RESOLVER_ERROR;
    /* Reject malformed reference before environment lookup or byte comparison. */
    if (reference->valid != PAPACC_TRUE || reference->length == 0U ||
        reference->length > PAPACC_SECURITY_CONFIGURATION_REF_CAPACITY)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    environment_status=papacc_server_security_environment_value(
        "PAPACC_SECURITY_CONFIGURATION_REF",expected,(DWORD)sizeof(expected));
    if (environment_status == 0) {
        *resolution = PAPACC_SECURITY_CONFIGURATION_UNAVAILABLE;
        return PAPACC_RESULT_OK;
    }
    if (environment_status != 1) {
        *resolution = PAPACC_SECURITY_CONFIGURATION_INVALID;
        return PAPACC_RESULT_OK;
    }
    expected_size = strlen(expected);
    if (reference->valid != PAPACC_TRUE || expected_size != reference->length ||
        memcmp(reference->value, expected, expected_size) != 0) {
        *resolution = PAPACC_SECURITY_CONFIGURATION_NOT_FOUND;
        return PAPACC_RESULT_OK;
    }
    if (papacc_server_security_environment_value("PAPACC_SERVER_CERTIFICATE_DER_B64",certificate,
            (DWORD)sizeof(certificate)) != 1 ||
        papacc_server_security_environment_value("PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64",private_key,
            (DWORD)sizeof(private_key)) != 1 ||
        papacc_server_security_environment_value("PAPACC_CLIENT_CA_DER_B64",client_ca,
            (DWORD)sizeof(client_ca)) != 1 ||
        papacc_server_security_environment_value("PAPACC_ALLOWED_CLIENT_CERT_SHA256",allowed_client,
            (DWORD)sizeof(allowed_client)) != 1) {
        *resolution = PAPACC_SECURITY_CONFIGURATION_INVALID;
        return PAPACC_RESULT_OK;
    }
    storage = (PAPACC_SERVER_SECURITY_ENVIRONMENT_STORAGE *)source_allocate(
        transaction,sizeof(*storage));
    if (storage == NULL) return transaction!=NULL?transaction->failure:PAPACC_RESULT_OUT_OF_MEMORY;
    memset(storage,0,sizeof(*storage));storage->transaction=transaction;
    if(transaction!=NULL)transaction->held_storage=storage;
    if (!papacc_server_security_parse_hex_sha256(allowed_client,
            storage->allowed_client_certificate_sha256) ||
        !papacc_server_security_load_base64(certificate,
            &storage->certificate, &storage->certificate_size,&storage->capacities[0],transaction) ||
        !papacc_server_security_load_base64(private_key,
            &storage->private_key, &storage->private_key_size,&storage->capacities[1],transaction) ||
        !papacc_server_security_load_base64(client_ca,
            &storage->client_ca, &storage->client_ca_size,&storage->capacities[2],transaction)) {
        if(transaction!=NULL && transaction->close_uncertain){
            configuration->release=papacc_server_security_environment_release;
            configuration->release_context=storage;
        }else papacc_server_security_environment_release(storage);
        *resolution = PAPACC_SECURITY_CONFIGURATION_INVALID;
        return transaction!=NULL && transaction->failure!=PAPACC_RESULT_OK?
            transaction->failure:PAPACC_RESULT_OK;
    }
    storage->certificate_item.data = storage->certificate;
    storage->certificate_item.size = storage->certificate_size;
    storage->ca_item.data = storage->client_ca;
    storage->ca_item.size = storage->client_ca_size;
    configuration->composition_inputs.local_certificate_chain =
        &storage->certificate_item;
    configuration->composition_inputs.local_certificate_count = 1U;
    configuration->composition_inputs.local_private_key_pkcs8_der =
        storage->private_key;
    configuration->composition_inputs.local_private_key_pkcs8_der_size =
        storage->private_key_size;
    configuration->composition_inputs.peer_trust_anchors = &storage->ca_item;
    configuration->composition_inputs.peer_trust_anchor_count = 1U;
    configuration->composition_inputs.secure_principal_alpn =
        (const PAPACC_U8 *)PAPACC_SECURITY_ALPN_PAPACC_1;
    configuration->composition_inputs.secure_principal_alpn_size =
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE;
    configuration->composition_inputs.provider_id = "openssl";
    configuration->composition_inputs.provider_bootstrap =
        papacc_pst_provider_bootstrap_win32;
    configuration->composition_inputs.logger = (const PAPACC_LOGGER *)context;
    configuration->principal_resolve = papacc_server_lab_principal_resolve;
    configuration->principal_resolve_context = storage;
    configuration->authorize = papacc_server_lab_authorize;
    configuration->release = papacc_server_security_environment_release;
    configuration->release_context = storage;
    configuration->valid = PAPACC_TRUE;
    *resolution = PAPACC_SECURITY_CONFIGURATION_RESOLVED;
    return PAPACC_RESULT_OK;
}


/* Existing public-to-the-application resolver contract and callback identities
 * remain unchanged. Transactional callers use the same source_resolve path. */
PAPACC_RESULT papacc_server_security_environment_resolve(
    void *context,const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{return source_resolve(context,reference,resolution,configuration,NULL);}

PAPACC_RESULT papacc_server_security_source_prepare(PAPACC_SECURITY_SOURCE_TRANSACTION *t,
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,const PAPACC_LOGGER *logger,
    PAPACC_SIZE other_reserved,PAPACC_SIZE credit)
{
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION resolution;
    PAPACC_RESULT result;
    if(t==NULL || reference==NULL || logger==NULL || credit==0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(t->state!=PAPACC_SECURITY_SOURCE_EMPTY || t->owner_thread!=0U ||
        t->held_storage!=NULL || t->used!=0U || t->configuration.release!=NULL)
        return PAPACC_RESULT_INVALID_STATE;
    if(sizeof(*t)>PAPACC_ED11_PROCESS_LIMIT || other_reserved>PAPACC_ED11_PROCESS_LIMIT-sizeof(*t) ||
        credit>PAPACC_ED11_PROCESS_LIMIT-sizeof(*t)-other_reserved)return PAPACC_RESULT_LIMIT_EXCEEDED;
    t->owner_thread=GetCurrentThreadId();t->other_reserved=other_reserved;
    t->credit=credit;t->reference=*reference;t->logger=*logger;
    t->state=PAPACC_SECURITY_SOURCE_PREPARING;t->failure=PAPACC_RESULT_OK;
    result=source_resolve(&t->logger,reference,&resolution,&t->configuration,t);
    if(result==PAPACC_RESULT_OK && resolution==PAPACC_SECURITY_CONFIGURATION_RESOLVED){
        t->state=PAPACC_SECURITY_SOURCE_PREPARED;
        result=papacc_server_security_source_validate(t);
    }
    else if(result==PAPACC_RESULT_OK)result=PAPACC_RESULT_INVALID_STATE;
    if(result!=PAPACC_RESULT_OK){
        t->failure=result;
        if(t->close_uncertain){t->state=PAPACC_SECURITY_SOURCE_RETAINED;return result;}
        papacc_resolved_server_security_configuration_release(&t->configuration);
        t->state=PAPACC_SECURITY_SOURCE_REJECTED;
    }
    return result;
}
PAPACC_RESULT papacc_server_security_source_validate(const PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    if(!source_owned(t) || t->close_uncertain || t->used==0U ||
        t->used>t->credit || t->held_storage==NULL ||
        (t->state!=PAPACC_SECURITY_SOURCE_PREPARED &&
         t->state!=PAPACC_SECURITY_SOURCE_COMMITTED && t->state!=PAPACC_SECURITY_SOURCE_PUBLISHED))
        return PAPACC_RESULT_INVALID_STATE;
    return papacc_resolved_server_security_configuration_validate(&t->configuration);
}
PAPACC_RESULT papacc_server_security_source_commit(PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    if(!source_owned(t) || t->state!=PAPACC_SECURITY_SOURCE_PREPARED ||
        papacc_server_security_source_validate(t)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
    t->state=PAPACC_SECURITY_SOURCE_COMMITTED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_publish(PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    if(!source_owned(t) || t->state!=PAPACC_SECURITY_SOURCE_COMMITTED ||
        papacc_server_security_source_validate(t)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
    t->state=PAPACC_SECURITY_SOURCE_PUBLISHED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_borrow(PAPACC_SECURITY_SOURCE_TRANSACTION *t,
    PAPACC_U64 *ticket,const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION **view)
{
    unsigned i;if(ticket!=NULL)*ticket=0U;if(view!=NULL)*view=NULL;
    if(ticket==NULL || view==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(!source_owned(t) || t->state!=PAPACC_SECURITY_SOURCE_PUBLISHED ||
        papacc_server_security_source_validate(t)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
    if(t->next_ticket==(PAPACC_U64)-1)return PAPACC_RESULT_LIMIT_EXCEEDED;
    for(i=0U;i<8U;++i)if(t->tickets[i]==0U)break;
    if(i==8U)return PAPACC_RESULT_LIMIT_EXCEEDED;
    t->tickets[i]=++t->next_ticket;++t->borrowers;*ticket=t->tickets[i];
    *view=&t->configuration;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_unborrow(PAPACC_SECURITY_SOURCE_TRANSACTION *t,PAPACC_U64 ticket)
{
    unsigned i;
    if(!source_owned(t) || ticket==0U)return PAPACC_RESULT_INVALID_STATE;
    for(i=0U;i<8U;++i)if(t->tickets[i]==ticket)break;
    if(i==8U || t->borrowers==0U)return PAPACC_RESULT_INVALID_STATE;
    t->tickets[i]=0U;--t->borrowers;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_rollback(PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    if(!source_owned(t) || t->state!=PAPACC_SECURITY_SOURCE_PREPARED ||
        t->borrowers!=0U || t->close_uncertain)return PAPACC_RESULT_INVALID_STATE;
    papacc_resolved_server_security_configuration_release(&t->configuration);
    if(t->used || t->held_storage)return PAPACC_RESULT_INTERNAL_ERROR;
    t->state=PAPACC_SECURITY_SOURCE_CLOSED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_finish(PAPACC_SECURITY_SOURCE_TRANSACTION *t,PAPACC_BOOL quiescent)
{
    if(!source_owned(t) || quiescent!=PAPACC_TRUE || t->close_uncertain ||
        t->borrowers!=0U || t->state==PAPACC_SECURITY_SOURCE_RETAINED ||
        (t->state!=PAPACC_SECURITY_SOURCE_COMMITTED && t->state!=PAPACC_SECURITY_SOURCE_PUBLISHED &&
         t->state!=PAPACC_SECURITY_SOURCE_REJECTED))return PAPACC_RESULT_INVALID_STATE;
    papacc_resolved_server_security_configuration_release(&t->configuration);
    if(t->used || t->held_storage)return PAPACC_RESULT_INTERNAL_ERROR;
    t->state=PAPACC_SECURITY_SOURCE_CLOSED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_security_source_retain(PAPACC_SECURITY_SOURCE_TRANSACTION *t)
{
    if(!source_owned(t) || t->state==PAPACC_SECURITY_SOURCE_REJECTED)
        return PAPACC_RESULT_INVALID_STATE;
    t->state=PAPACC_SECURITY_SOURCE_RETAINED;return PAPACC_RESULT_OK;
}
