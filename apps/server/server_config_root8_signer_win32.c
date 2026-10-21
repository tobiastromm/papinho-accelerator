#include "server_config_root8_signer_win32.h"
#include "../../src/core/ed11_process_profile.h"
#include <stdint.h>
#include <string.h>
static unsigned k16(const PAPACC_U8 *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static PAPACC_U64 k64(const PAPACC_U8 *p){PAPACC_U64 kind8_view=0;unsigned j;for(j=0;j<8;j++)kind8_view|=(PAPACC_U64)p[j]<<(8*j);return kind8_view;}
static int krange(const void *p,PAPACC_SIZE n){return p&&n&&n-1U<=UINTPTR_MAX-(uintptr_t)p;}
static int kover(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m){uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;return x<=y?y-x<n:x-y<m;}
PC_ROOT8_RESULT papacc_root8_signer_data(void *ctx,unsigned kind,const PAPACC_U8 *p,
 PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{
 PAPACC_ROOT8_SIGNER_DATA *c=ctx;PAPACC_ISSUER_VIEW kind8_view;PAPACC_ISSUER_RECORD r;
 PAPACC_ISSUER_RESULT result;PAPACC_SIZE used,known;BYTE digest[32];
 const void *input[5];PAPACC_SIZE size[5];unsigned j;
 if(kind!=8U)return PC_ROOT8_UNSUPPORTED;
 if(!krange(c,sizeof(*c))||!krange(p,n)||n!=16384U||!krange(row,384)||!krange(header,512))return PC_ROOT8_INVALID;
 if(!c->ca||!c->workspace||!c->certificate||!c->hash)return PC_ROOT8_INCOMPLETE;
 if(!krange(c->ca,sizeof(*c->ca))||!krange(c->workspace,sizeof(*c->workspace))||
    !krange(c->workspace->bytes,c->workspace->capacity)||!krange(c->certificate,c->certificate_capacity)||
    c->certificate_capacity>8192U||c->workspace->capacity>PAPACC_ED11_PROCESS_LIMIT)return PC_ROOT8_INVALID;
 input[0]=p;size[0]=n;input[1]=row;size[1]=384;input[2]=header;size[2]=512;
 input[3]=c;size[3]=sizeof(*c);input[4]=c->ca;size[4]=sizeof(*c->ca);
 for(j=0;j<5;j++)if(kover(input[j],size[j],c->workspace->bytes,c->workspace->capacity)||
    kover(input[j],size[j],c->certificate,c->certificate_capacity)||
    kover(input[j],size[j],c->workspace,sizeof(*c->workspace)))return PC_ROOT8_INVALID;
 if(kover(c->workspace->bytes,c->workspace->capacity,c->certificate,c->certificate_capacity)||
    kover(c->workspace->bytes,c->workspace->capacity,c->workspace,sizeof(*c->workspace))||
    kover(c->certificate,c->certificate_capacity,c->workspace,sizeof(*c->workspace)))return PC_ROOT8_INVALID;
 known=n+384U+512U+sizeof(*c)+sizeof(*c->ca)+sizeof(*c->workspace)+c->certificate_capacity;
 if(known>PAPACC_ED11_PROCESS_LIMIT||c->workspace->capacity>PAPACC_ED11_PROCESS_LIMIT-known)return PC_ROOT8_INCOMPLETE;
 known+=c->workspace->capacity;
 if(c->already_reserved<known||c->already_reserved>PAPACC_ED11_PROCESS_LIMIT||
    sizeof(kind8_view)+sizeof(r)+sizeof(digest)>PAPACC_ED11_PROCESS_LIMIT-c->already_reserved)return PC_ROOT8_INCOMPLETE;
 if(c->workspace->stopped)return PC_ROOT8_INCOMPLETE;
 if(memcmp(header,"PACROOT8",8)||k16(header+8)!=3U||k16(header+10)!=512U||
    memcmp(row,"PACPDL01",8)||k16(row+8)!=1U||k16(row+10)!=384U||
    (k16(row+16)!=1U&&k16(row+16)!=2U)||k16(row+18)!=1U)return PC_ROOT8_INVALID;
 result=papacc_issuer_image_validate(p+8192,&kind8_view,c->hash,c->hash_context);
 if(result==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(result!=PAPACC_ISSUER_OK||memcmp(kind8_view.root,header+72,32)||kind8_view.revision!=k64(header+120))return PC_ROOT8_INVALID;
 result=papacc_issuer_image_find(p+8192,row+96,&r,c->hash,c->hash_context);
 if(result==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(result!=PAPACC_ISSUER_OK||r.request.purpose!=PAPACC_ISSUER_PACKAGE||
    (r.state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r.state!=PAPACC_ISSUER_PUBLISHED&&r.state!=PAPACC_ISSUER_REVOKED)||
    memcmp(row+64,kind8_view.authority,16)||memcmp(row+80,kind8_view.generation,16)||memcmp(row+260,kind8_view.ca_spki_digest,32)||
    memcmp(row+112,r.certificate_digest,32)||memcmp(row+144,r.request.key_digest,32)||
    k64(row+176)<r.request.not_before||k64(row+184)>r.request.not_after||k64(row+184)<=k64(row+176))return PC_ROOT8_INVALID;
 used=0;result=papacc_issuer_image_certificate(p+8192,row+96,c->certificate,c->certificate_capacity,&used,c->hash,c->hash_context);
 if(result==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(result==PAPACC_ISSUER_LIMIT)return PC_ROOT8_INCOMPLETE;
 if(result!=PAPACC_ISSUER_OK||used!=r.certificate_size||!used||used>8192U)return PC_ROOT8_INVALID;
 result=papacc_issuer_x509_verify(&kind8_view,&r,c->ca,c->certificate,(DWORD)used,c->workspace,digest);
 if(result==PAPACC_ISSUER_STOPPED||result==PAPACC_ISSUER_LIMIT)return PC_ROOT8_INCOMPLETE;
 if(result!=PAPACC_ISSUER_OK||memcmp(digest,row+112,32))return PC_ROOT8_INVALID;
 return c->workspace->stopped?PC_ROOT8_INCOMPLETE:PC_ROOT8_OK; /* DATA, including historical revoked rows */
}
