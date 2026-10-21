#include "server_outbound_tls_source_win32.h"

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ref_equal(const PAPACC_U8 *value,PAPACC_SIZE size,
    const char *expected)
{
    return expected!=NULL&&strlen(expected)==size&&
        memcmp(value,expected,size)==0;
}

static int load_base64(const char *path,PAPACC_U8 **data,PAPACC_SIZE *size)
{
    FILE *file=NULL;char *encoded=NULL;long length;DWORD needed=0U,written;
    if(path==NULL||path[0]=='\0'||data==NULL||size==NULL||
        fopen_s(&file,path,"rb")!=0||file==NULL)return 0;
    if(fseek(file,0,SEEK_END)!=0||(length=ftell(file))<=0||
        length>16L*1024L*1024L||fseek(file,0,SEEK_SET)!=0)goto fail;
    encoded=(char *)malloc((size_t)length+1U);
    if(encoded==NULL||fread(encoded,1U,(size_t)length,file)!=(size_t)length)
        goto fail;
    fclose(file);file=NULL;encoded[length]='\0';
    if(!CryptStringToBinaryA(encoded,(DWORD)length,CRYPT_STRING_BASE64,NULL,
        &needed,NULL,NULL)||needed==0U)goto fail;
    *data=(PAPACC_U8 *)malloc((size_t)needed);if(*data==NULL)goto fail;
    written=needed;
    if(!CryptStringToBinaryA(encoded,(DWORD)length,CRYPT_STRING_BASE64,*data,
        &written,NULL,NULL)){free(*data);*data=NULL;goto fail;}
    free(encoded);*size=(PAPACC_SIZE)written;return 1;
fail:
    if(file!=NULL)fclose(file);free(encoded);return 0;
}

PAPACC_RESULT papacc_server_outbound_tls_environment_resolve(
    const PAPACC_SERVER_CONFIG *config,
    PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE *profile)
{
    char expected[65],ca_path[MAX_PATH];size_t required=0U,ca_required=0U;
    if(config==NULL||profile==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *profile=(PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE)
        PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE_INITIALIZER;
    if(getenv_s(&required,expected,sizeof(expected),
            "PAPACC_OUTBOUND_TLS_PROFILE_REF")!=0||required==0U||
        required>sizeof(expected)||
        !ref_equal(config->outbound_tls_profile_ref,
            config->outbound_tls_profile_ref_length,expected))
        return PAPACC_RESULT_INVALID_STATE;
    if(strcmp(expected,"system-tls13")==0){
        profile->trust_kind=PAPACC_OUTBOUND_TLS_TRUST_SYSTEM;
    }else{
        if(getenv_s(&ca_required,ca_path,sizeof(ca_path),
                "PAPACC_OUTBOUND_TLS_CA_DER_B64")!=0||ca_required==0U||
            ca_required>sizeof(ca_path)||!load_base64(ca_path,
                &profile->custom_ca_der,&profile->custom_ca_der_size))
            return PAPACC_RESULT_INVALID_STATE;
        profile->trust_kind=PAPACC_OUTBOUND_TLS_TRUST_CUSTOM_CA_DER;
    }
    memcpy(profile->provider_id,"openssl",8U);
    profile->valid=PAPACC_TRUE;return PAPACC_RESULT_OK;
}

void papacc_resolved_outbound_tls_profile_release(
    PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE *profile)
{
    if(profile==NULL)return;free(profile->custom_ca_der);
    *profile=(PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE)
        PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE_INITIALIZER;
}
