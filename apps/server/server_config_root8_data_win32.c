#include "server_config_root8_data_win32.h"
#include "server_configuration_body05_win32.h"
#include <string.h>
static unsigned le16(const PAPACC_U8 *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static PAPACC_U64 le64(const PAPACC_U8 *p)
{PAPACC_U64 v=0;unsigned i;for(i=0;i<8U;++i)v|=(PAPACC_U64)p[i]<<(8U*i);return v;}
PC_ROOT8_RESULT papacc_root8_body05_data(void *ctx,unsigned kind,const PAPACC_U8 *p,
 PAPACC_SIZE n,const PAPACC_U8 *desc,const PAPACC_U8 *header)
{
 PAPACC_CONFIG_VIEW v;PAPACC_SIZE i;const PAPACC_SERVER_CONFIG *c;PAPACC_RESULT result;
 (void)ctx;
 if(kind==6U)return PC_ROOT8_UNSUPPORTED;
 if(kind!=5U||!desc||!header||!p||n!=1024U)return PC_ROOT8_INVALID;
 result=papacc_configuration_body05_decode(p,n,&v);
 if(result!=PAPACC_RESULT_OK)return PC_ROOT8_INVALID;
 c=&v.config;
 if(memcmp(v.domain,header+72,32)||v.revision!=le64(header+104)||v.predecessor!=le64(header+112)||v.policy_epoch||
  c->control_port!=le16(desc+8)||(unsigned)c->listener_profile!=desc[4]||
  c->policy.admission+1U!=desc[5]||(unsigned)c->bind_selection.mode!=desc[6]||
  c->bind_selection.interface_persistent_id_count!=le16(desc+18)||
  c->security_configuration_ref_length!=le16(desc+12)||
  memcmp(c->security_configuration_ref,desc+88,c->security_configuration_ref_length)||
  memcmp(c->policy.identity,desc+280,32))return PC_ROOT8_INVALID;
 for(i=0;i<c->bind_selection.interface_persistent_id_count;++i)
  if(!v.ids[i].is_valid||v.ids[i].value!=le64(desc+392U+16U*i))return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}

#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(PAPACC_OFFLINE_D3_WORKER)
#include "server_config_root8_signer_win32.h"
#include <wincrypt.h>
static int pinned_range(const void *p,PAPACC_SIZE n)
{return p&&n&&n-1U<=MAXULONG_PTR-(ULONG_PTR)p;}
static int pinned_overlap(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x<=y?y-x<n:x-y<m;}
static PAPACC_U32 pinned32(const PAPACC_U8 *p)
{return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
/* Decode only the public CA DER retained by the same canonical pin. NOCOPY
 * references stay in that immutable DER or the caller's CA arena until return.
 * No certificate context/heap/handle, private operation or authority factory. */
static const CERT_INFO *pinned_ca(const PAPACC_CONFIG_V3_BORROW *borrow,
 PAPACC_ISSUER_WINDOWS_WORKSPACE *workspace)
{
 BYTE *cursor=workspace->bytes;DWORD left=(DWORD)workspace->capacity,n=0,rounded;
 CERT_SIGNED_CONTENT_INFO *signed_info;CERT_INFO *info;
 workspace->native_error=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,borrow->ca,(DWORD)borrow->ca_bytes,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))goto error;
 if(n>0xfffffff8U||(rounded=(n+7U)&~7U)>left)return NULL;
 signed_info=(CERT_SIGNED_CONTENT_INFO *)cursor;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,borrow->ca,(DWORD)borrow->ca_bytes,CRYPT_DECODE_NOCOPY_FLAG,NULL,signed_info,&n))goto error;
 cursor+=rounded;left-=rounded;n=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))goto error;
 if(n>left)return NULL;info=(CERT_INFO *)cursor;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,info,&n))goto error;
 return info;
error:workspace->native_error=GetLastError();return NULL;
}
static PC_ROOT8_RESULT pinned_signer(PAPACC_ROOT8_CARRIER_DATA *context,
 const PAPACC_U8 *carrier,PAPACC_SIZE bytes,const PAPACC_U8 *row,const PAPACC_U8 *header)
{
 PAPACC_CONFIG_V3_BORROW borrow,check;PAPACC_ROOT8_SIGNER_DATA signer;
 PAPACC_ISSUER_WINDOWS_WORKSPACE *ca_workspace,*signer_workspace;PAPACC_RESULT result;
 PC_ROOT8_RESULT data_result;const CERT_INFO *ca;const void *areas[6];PAPACC_SIZE sizes[6],known;
 unsigned j,k;int found=0;
 if(!context||!context->owner||context->v3_version!=1U)return PC_ROOT8_INCOMPLETE;
 ca_workspace=context->v3_ca_workspace;signer_workspace=context->v3_signer_workspace;
 if(!ca_workspace||!signer_workspace||!context->v3_certificate)return PC_ROOT8_INCOMPLETE;
 if(!pinned_range(ca_workspace,sizeof(*ca_workspace))||!pinned_range(signer_workspace,sizeof(*signer_workspace))||
 !pinned_range(ca_workspace->bytes,ca_workspace->capacity)||!pinned_range(signer_workspace->bytes,signer_workspace->capacity)||
 !pinned_range(context->v3_certificate,context->v3_certificate_capacity)||
 ((ULONG_PTR)ca_workspace->bytes&7U)||ca_workspace->capacity>0xffffffffU||
 ca_workspace->capacity>PAPACC_ED11_PROCESS_LIMIT||signer_workspace->capacity>PAPACC_ED11_PROCESS_LIMIT||
 context->v3_certificate_capacity>8192U)return PC_ROOT8_INVALID;
 areas[0]=ca_workspace;sizes[0]=sizeof(*ca_workspace);areas[1]=signer_workspace;sizes[1]=sizeof(*signer_workspace);
 areas[2]=ca_workspace->bytes;sizes[2]=ca_workspace->capacity;areas[3]=signer_workspace->bytes;sizes[3]=signer_workspace->capacity;
 areas[4]=context->v3_certificate;sizes[4]=context->v3_certificate_capacity;areas[5]=context;sizes[5]=sizeof(*context);
 known=sizeof(*context->owner)+sizeof(borrow)+sizeof(check)+sizeof(signer);
 for(j=0;j<6U;j++){
  if(sizes[j]>PAPACC_ED11_PROCESS_LIMIT-known||pinned_overlap(areas[j],sizes[j],context->owner,sizeof(*context->owner)))return PC_ROOT8_INVALID;
  known+=sizes[j];for(k=0;k<j;k++)if(pinned_overlap(areas[j],sizes[j],areas[k],sizes[k]))return PC_ROOT8_INVALID;
 }
 if(context->v3_reserved<known||context->v3_reserved>PAPACC_ED11_PROCESS_LIMIT||ca_workspace->stopped||signer_workspace->stopped)return PC_ROOT8_INCOMPLETE;
 result=papacc_configuration_v3_borrow_data(context->owner,&context->v3_pin,&borrow);
 if(result!=PAPACC_RESULT_OK)return PC_ROOT8_INCOMPLETE;
 /* Only exact protected ranges: never accept unpinned clones by digest alone. */
 if(header!=borrow.packet||bytes!=16384U||carrier!=borrow.packet+pinned32(borrow.packet+48))return PC_ROOT8_INVALID;
 for(j=0;j<(unsigned)(borrow.packet[268]|((unsigned)borrow.packet[269]<<8));j++)
  if(row==borrow.pdg+64U+384U*j){found=1;break;}
 if(!found)return PC_ROOT8_INVALID;
 ca=pinned_ca(&borrow,ca_workspace);if(!ca)return PC_ROOT8_INCOMPLETE;
 memset(&signer,0,sizeof(signer));signer.ca=ca;signer.workspace=signer_workspace;
 signer.certificate=context->v3_certificate;signer.certificate_capacity=context->v3_certificate_capacity;
 signer.hash=papacc_configuration_issuer_hash06;signer.hash_context=context->owner;signer.already_reserved=context->v3_reserved;
 data_result=papacc_root8_signer_data(&signer,8U,carrier,bytes,row,header);
 if(data_result!=PC_ROOT8_OK)return data_result; /* no historical fallback */
 if(ca_workspace->stopped||signer_workspace->stopped||
 papacc_configuration_v3_borrow_data(context->owner,&context->v3_pin,&check)!=PAPACC_RESULT_OK)return PC_ROOT8_INCOMPLETE;
 return PC_ROOT8_OK; /* PUBLIC DATA ONLY, never CURRENT/delegation/TRUST */
}
#endif

/* Separate offline callback: historical body-only dispatch above is unchanged. */
PC_ROOT8_RESULT papacc_root8_issuer_carrier_data(void *ctx,unsigned kind,
 const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{
 PAPACC_ROOT8_CARRIER_DATA *c=ctx;PAPACC_ISSUER_VIEW v;
 PAPACC_ISSUER_CHECKPOINT_REPORT report;PAPACC_ISSUER_RECORD record;
 PAPACC_ISSUER_RESULT s;
 if(kind==5U)return papacc_root8_body05_data(NULL,kind,p,n,row,header);
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(PAPACC_OFFLINE_D3_WORKER)
 if(kind==8U)return pinned_signer(c,p,n,row,header);
#endif
 if(kind!=6U&&kind!=7U)return PC_ROOT8_UNSUPPORTED;
 if(!c||!c->owner||!c->workspace||!header||!p||n!=16384U)return PC_ROOT8_INVALID;
 if(c->owner->stopped||c->owner->unresolved||c->owner->authority_lost)return PC_ROOT8_INCOMPLETE;
 if(!c->floor||c->floor_bytes!=16384U)return PC_ROOT8_INCOMPLETE;
 if(papacc_configuration_issuer_carrier_inspect06(c->owner,c->floor,c->floor_bytes,c->workspace,&v)!=PAPACC_RESULT_OK||
    papacc_configuration_issuer_carrier_inspect06(c->owner,p,n,c->workspace,&v)!=PAPACC_RESULT_OK)return PC_ROOT8_INVALID;
 if(memcmp(v.root,header+72,32)||v.revision!=le64(header+120)||memcmp(p,c->floor,8192))return PC_ROOT8_INVALID;
 s=papacc_issuer_checkpoint_assess(p+8192,8192,c->floor+8192,8192,
    papacc_configuration_issuer_hash06,c->owner,0,&report);
 if(s!=PAPACC_ISSUER_DENIED||report.state!=PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED)return PC_ROOT8_INVALID;
 if(kind==6U)return PC_ROOT8_OK; /* relative DATA continuity; never recovery */
 if(!row||memcmp(row+40,v.authority,16)||memcmp(row+56,v.generation,16)||memcmp(row+568,v.ca_spki_digest,32))return PC_ROOT8_INVALID;
 if(row[22]==1U)return PC_ROOT8_OK;
 if(row[22]!=2U&&row[22]!=3U)return PC_ROOT8_INVALID;
 s=papacc_issuer_image_find(p+8192,row+72,&record,papacc_configuration_issuer_hash06,c->owner);
 if(s!=PAPACC_ISSUER_OK||record.request.purpose!=PAPACC_ISSUER_SERVER||
    memcmp(row+88,record.request.csr_digest,32)||memcmp(row+120,record.request.key_digest,32))return PC_ROOT8_INVALID;
 if(row[22]==3U)return PC_ROOT8_INCOMPLETE; /* DER/profile/SAN authority gate */
 if(record.state!=PAPACC_ISSUER_SERIAL_RESERVED&&record.state!=PAPACC_ISSUER_SIGN_REQUESTED)return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}
