#define _WIN32_WINNT 0x0600
#include "server_issuer_windows_validation.h"
#include <wincrypt.h>
#include <bcrypt.h>
#include <string.h>
#include <stdint.h>
static int overlap(const void *a,PAPACC_SIZE an,const void *b,PAPACC_SIZE bn)
{
 uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
 if(an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return 1;
 return x<y+bn&&y<x+an;
}
static PAPACC_ISSUER_RESULT native_failure(PAPACC_ISSUER_WINDOWS_WORKSPACE *w)
{
 w->native_error=GetLastError();return w->native_error==ERROR_MORE_DATA?PAPACC_ISSUER_LIMIT:PAPACC_ISSUER_INVALID;
}
static int take(BYTE **p,DWORD *left,DWORD n,void **out)
{
 DWORD rounded;
 if(n>0xfffffff8U)return 0;rounded=(n+7U)&~7U;
 if(rounded>*left)return 0;*out=*p;*p+=rounded;*left-=rounded;return 1;
}
static int parameters(const CRYPT_OBJID_BLOB *p)
{
 return p->cbData==2U&&p->pbData&&p->pbData[0]==5U&&p->pbData[1]==0U;
}
PAPACC_ISSUER_RESULT papacc_issuer_windows_verify_request(void *context,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *der,PAPACC_SIZE size)
{
 PAPACC_ISSUER_WINDOWS_WORKSPACE *w=context;BYTE *cursor,*temporary,digest[32];
 DWORD left,n,amount;void *space;CERT_SIGNED_CONTENT_INFO *signed_info;
 CERT_REQUEST_INFO *request;PUBLICKEYSTRUC *blob;RSAPUBKEY *rsa;
 CRYPT_DATA_BLOB subject;
 if(!w||!r||!der||!size||size>0xffffffffU||!w->bytes||w->capacity>0xffffffffU||
    ((uintptr_t)w->bytes&7U)||overlap(w->bytes,w->capacity,der,size)||
    overlap(w->bytes,w->capacity,r,sizeof(*r))||overlap(w->bytes,w->capacity,w,sizeof(*w))||
    overlap(w,sizeof(*w),der,size)||overlap(w,sizeof(*w),r,sizeof(*r)))return PAPACC_ISSUER_INVALID;
 w->native_error=0;if(w->stopped)return PAPACC_ISSUER_STOPPED;
 if(r->request.purpose<PAPACC_ISSUER_CLIENT||r->request.purpose>PAPACC_ISSUER_PACKAGE||
   (r->state!=PAPACC_ISSUER_SERIAL_RESERVED&&r->state!=PAPACC_ISSUER_SIGN_REQUESTED))return PAPACC_ISSUER_DENIED;
 n=sizeof(digest);
 if(!CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,der,(DWORD)size,digest,&n))return native_failure(w);
 if(n!=sizeof(digest)||memcmp(digest,r->request.csr_digest,32U))return PAPACC_ISSUER_CONFLICT;
 cursor=w->bytes;left=(DWORD)w->capacity;n=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,der,(DWORD)size,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))return native_failure(w);
 if(!take(&cursor,&left,n,&space))return PAPACC_ISSUER_LIMIT;signed_info=space;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,der,(DWORD)size,CRYPT_DECODE_NOCOPY_FLAG,NULL,signed_info,&n))return native_failure(w);
 if(!signed_info->SignatureAlgorithm.pszObjId||strcmp(signed_info->SignatureAlgorithm.pszObjId,szOID_RSA_SHA256RSA)||
    !parameters(&signed_info->SignatureAlgorithm.Parameters)||signed_info->Signature.cUnusedBits||signed_info->Signature.cbData!=256U)return PAPACC_ISSUER_UNSUPPORTED;
 n=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_REQUEST_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))return native_failure(w);
 if(!take(&cursor,&left,n,&space))return PAPACC_ISSUER_LIMIT;request=space;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_REQUEST_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,request,&n))return native_failure(w);
 if(request->dwVersion!=0U||request->cAttribute||!request->SubjectPublicKeyInfo.Algorithm.pszObjId||
    strcmp(request->SubjectPublicKeyInfo.Algorithm.pszObjId,szOID_RSA_RSA)||
    !parameters(&request->SubjectPublicKeyInfo.Algorithm.Parameters)||request->SubjectPublicKeyInfo.PublicKey.cUnusedBits)return PAPACC_ISSUER_UNSUPPORTED;
 /* Decode structures remain valid in caller scratch. Tail is reused only for
  * completed synchronous encode/compare/hash operations, no borrowed escape. */
 temporary=cursor;
 n=left;
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,signed_info,0,NULL,temporary,&n))return native_failure(w);
 if(n!=size||memcmp(temporary,der,n))return PAPACC_ISSUER_INVALID;
 n=left;
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT_REQUEST_TO_BE_SIGNED,request,0,NULL,temporary,&n))return native_failure(w);
 if(n!=signed_info->ToBeSigned.cbData||memcmp(temporary,signed_info->ToBeSigned.pbData,n))return PAPACC_ISSUER_INVALID;
 n=left;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,RSA_CSP_PUBLICKEYBLOB,request->SubjectPublicKeyInfo.PublicKey.pbData,
   request->SubjectPublicKeyInfo.PublicKey.cbData,0,NULL,temporary,&n))return native_failure(w);
 if(n!=sizeof(PUBLICKEYSTRUC)+sizeof(RSAPUBKEY)+256U)return PAPACC_ISSUER_UNSUPPORTED;
 blob=(PUBLICKEYSTRUC *)temporary;rsa=(RSAPUBKEY *)(temporary+sizeof(*blob));
 if(blob->bType!=PUBLICKEYBLOB||blob->bVersion!=CUR_BLOB_VERSION||blob->reserved||
    rsa->magic!=0x31415352U||rsa->bitlen!=2048U||rsa->pubexp!=65537U)return PAPACC_ISSUER_UNSUPPORTED;
 n=left;
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_PUBLIC_KEY_INFO,&request->SubjectPublicKeyInfo,0,NULL,temporary,&n))return native_failure(w);
 amount=sizeof(digest);
 if(!CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,temporary,n,digest,&amount))return native_failure(w);
 if(amount!=sizeof(digest)||memcmp(digest,r->request.key_digest,32U))return PAPACC_ISSUER_CONFLICT;
 subject.pbData=(BYTE *)der;subject.cbData=(DWORD)size;
 if(w->stopped)return PAPACC_ISSUER_STOPPED;
 if(!CryptVerifyCertificateSignatureEx(0,X509_ASN_ENCODING,CRYPT_VERIFY_CERT_SIGN_SUBJECT_BLOB,&subject,
   CRYPT_VERIFY_CERT_SIGN_ISSUER_PUBKEY,&request->SubjectPublicKeyInfo,0,NULL))return native_failure(w);
 return w->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT papacc_issuer_windows_key_result(const PAPACC_CA_KEY *key,SECURITY_STATUS code)
{
 if(!key)return PAPACC_ISSUER_ABSENT;
 if(key->state==PAPACC_CA_KEY_RETAINED||key->retained_token||key->retained_local[0]||key->retained_local[1]||key->retained_local[2])return PAPACC_ISSUER_RETAINED;
 if(code==NTE_PERM)return PAPACC_ISSUER_DENIED;
 if(code==NTE_NOT_SUPPORTED)return PAPACC_ISSUER_UNSUPPORTED;
 if(code!=ERROR_SUCCESS)return PAPACC_ISSUER_UNCERTAIN;
 /* A raw SUCCESS or READY flag cannot establish root reservation/authority. */
 return PAPACC_ISSUER_DENIED;
}
