#include "../src/core/ed11_process_profile.h"
/* Default: synthetic signatures/root authority. Explicit native-ephemeral
 * route: real ephemeral CNG crypto; no persistent key, authority or network. */
#define _WIN32_WINNT 0x0600
#include "server_issuer_x509_win32.h"
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static int native_crypto;
static NCRYPT_PROV_HANDLE ephemeral_provider;
static NCRYPT_KEY_HANDLE ephemeral_ca,ephemeral_client;
static union {ULONGLONG alignment;BYTE bytes[2048];} ephemeral_ca_public,ephemeral_client_public;
static unsigned native_signs,native_verifications;
static SECURITY_STATUS ephemeral_new(NCRYPT_KEY_HANDLE *k)
{
 DWORD bits=2048;SECURITY_STATUS r;
 r=NCryptCreatePersistedKey(ephemeral_provider,k,NCRYPT_RSA_ALGORITHM,NULL,0,0);if(r)return r;
 r=NCryptSetProperty(*k,NCRYPT_LENGTH_PROPERTY,(PBYTE)&bits,sizeof(bits),0);if(r)return r;
 return NCryptFinalizeKey(*k,0);
}
static int ephemeral_public(NCRYPT_KEY_HANDLE k,void *out,DWORD cap)
{CERT_PUBLIC_KEY_INFO *value;return papacc_issuer_x509_key_public(k,out,cap,&value)==PAPACC_ISSUER_OK;}
static int ephemeral_signature(NCRYPT_KEY_HANDLE k,const BYTE *input,DWORD bytes,BYTE sig[256])
{
 BYTE digest[32];DWORD n=32;BCRYPT_PKCS1_PADDING_INFO pad={BCRYPT_SHA256_ALGORITHM};
 if(!CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,input,bytes,digest,&n))return 0;
 native_signs++;return NCryptSignHash(k,&pad,digest,32,sig,256,&n,NCRYPT_PAD_PKCS1_FLAG|NCRYPT_SILENT_FLAG)==0&&n==256;
}
static BOOL ephemeral_verify(HCRYPTPROV_LEGACY p,DWORD e,DWORD s,void *a,DWORD i,void *b,DWORD f,void *x)
{native_verifications++;return CryptVerifyCertificateSignatureEx(p,e,s,a,i,b,f,x);}
static SECURITY_STATUS ephemeral_sign(NCRYPT_KEY_HANDLE k,void *p,PBYTE h,DWORD n,PBYTE out,DWORD cap,DWORD *used,DWORD f)
{native_signs++;return NCryptSignHash(k,p,h,n,out,cap,used,f);}
static BOOL ephemeral_export(HCRYPTPROV_OR_NCRYPT_KEY_HANDLE k,DWORD spec,DWORD enc,LPSTR oid,DWORD f,void *aux,PCERT_PUBLIC_KEY_INFO out,DWORD *n)
{(void)spec;return CryptExportPublicKeyInfoEx(k,CERT_NCRYPT_KEY_SPEC,enc,oid,f,aux,out,n);}
static CERT_PUBLIC_KEY_INFO ca_public;
static unsigned checks,private_calls,validation_calls;static SECURITY_STATUS fail_sign;
static int bad_signature,deny_admin,stop_on_sign,fail_return,normal_profile,defer_core_init;
static unsigned source_reads,source_fault_at,source_fault;
static PAPACC_ISSUER_CORE fixture_core;
static BOOL WINAPI model_verify(HCRYPTPROV_LEGACY p,DWORD e,DWORD s,void *a,DWORD i,void *b,DWORD f,void *x)
{if(native_crypto)return ephemeral_verify(p,e,s,a,i,b,f,x);(void)p;(void)e;(void)s;(void)a;(void)i;(void)b;(void)f;(void)x;return !bad_signature;}
static SECURITY_STATUS model_validate(PAPACC_CA_KEY *key){DWORD bits=0,policy=1,n=0;validation_calls++;
 if(!native_crypto)return 0;
 if(!key||key->key!=ephemeral_ca||key->provider!=ephemeral_provider||key->thread!=GetCurrentThreadId())return NTE_BAD_KEY_STATE;
 if(NCryptGetProperty(key->key,NCRYPT_LENGTH_PROPERTY,(PBYTE)&bits,sizeof(bits),&n,0)||n!=sizeof(bits)||bits!=2048)return NTE_BAD_KEY_STATE;
 if(NCryptGetProperty(key->key,NCRYPT_EXPORT_POLICY_PROPERTY,(PBYTE)&policy,sizeof(policy),&n,0)||n!=sizeof(policy)||policy)return NTE_BAD_KEY_STATE;
 return 0;}
static SECURITY_STATUS model_borrow(PAPACC_CA_KEY *k,const void *h){if(native_crypto)return papacc_ca_key_borrow(k,h);if(!k||!h||k->borrower||k->state!=PAPACC_CA_KEY_READY)return NTE_BAD_KEY_STATE;k->borrower=h;return 0;}
static SECURITY_STATUS model_return(PAPACC_CA_KEY *k,const void *h){if(native_crypto)return papacc_ca_key_return(k,h);if(!k||k->borrower!=h||k->state!=PAPACC_CA_KEY_READY||fail_return)return NTE_BAD_KEY_STATE;k->borrower=NULL;return 0;}
static BOOL WINAPI model_export(HCRYPTPROV_OR_NCRYPT_KEY_HANDLE k,DWORD spec,DWORD enc,LPSTR oid,DWORD flags,void *aux,PCERT_PUBLIC_KEY_INFO info,DWORD *n)
{DWORD bytes,oidbytes;BYTE *cursor;
 if(native_crypto)return ephemeral_export(k,spec,enc,oid,flags,aux,info,n);
 (void)k;(void)spec;(void)enc;(void)oid;(void)flags;(void)aux;
 oidbytes=(DWORD)strlen(ca_public.Algorithm.pszObjId)+1;
 bytes=(DWORD)sizeof(*info)+oidbytes+ca_public.Algorithm.Parameters.cbData+ca_public.PublicKey.cbData;
 if(!info){*n=bytes;return TRUE;}if(*n<bytes)return FALSE;
 *info=ca_public;cursor=(BYTE *)(info+1);info->Algorithm.pszObjId=(LPSTR)cursor;memcpy(cursor,ca_public.Algorithm.pszObjId,oidbytes);cursor+=oidbytes;
 info->Algorithm.Parameters.pbData=cursor;memcpy(cursor,ca_public.Algorithm.Parameters.pbData,ca_public.Algorithm.Parameters.cbData);cursor+=ca_public.Algorithm.Parameters.cbData;
 info->PublicKey.pbData=cursor;memcpy(cursor,ca_public.PublicKey.pbData,ca_public.PublicKey.cbData);*n=bytes;return TRUE;}
static SECURITY_STATUS WINAPI model_sign(NCRYPT_KEY_HANDLE key,void *pad,PBYTE hash,DWORD hn,PBYTE out,DWORD cap,DWORD *used,DWORD flags)
{
 BCRYPT_PKCS1_PADDING_INFO *p=pad;(void)key;(void)hash;private_calls++;
 if(hn!=32||cap!=256||wcscmp(p->pszAlgId,BCRYPT_SHA256_ALGORITHM)||flags!=(NCRYPT_PAD_PKCS1_FLAG|NCRYPT_SILENT_FLAG))return NTE_INVALID_PARAMETER;
 if(fail_sign)return fail_sign;if(native_crypto)return ephemeral_sign(key,pad,hash,hn,out,cap,used,flags);memset(out,0x5a,256);*used=256;if(stop_on_sign)papacc_issuer_stop(&fixture_core);return 0;
}
#define CryptVerifyCertificateSignatureEx model_verify
#define papacc_ca_key_validate model_validate
#define papacc_ca_key_borrow model_borrow
#define papacc_ca_key_return model_return
#define CryptExportPublicKeyInfoEx model_export
#define NCryptSignHash model_sign
#define PAPACC_ISSUER_CNG_OFFLINE_TEST 1
#include "../apps/server/server_issuer_windows_validation.c"
#include "../apps/server/server_issuer_x509_win32.c"
#include "../apps/server/server_issuer_cng_backend_win32.c"
#undef CryptVerifyCertificateSignatureEx
#undef papacc_ca_key_validate
#undef papacc_ca_key_borrow
#undef papacc_ca_key_return
#undef CryptExportPublicKeyInfoEx
#undef NCryptSignHash
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL %u line %u\n",checks,(unsigned)__LINE__);return 1;}}while(0)
static PAPACC_ISSUER_VIEW fixture_view;
static PAPACC_ISSUER_RECORD record;
static PAPACC_ISSUER_CNG_BACKEND fixture_backend;
static PAPACC_CA_KEY key;
static PAPACC_ISSUER_BACKEND administrative;
static PAPACC_ISSUER_ROOT_PORT root;
static union {ULONGLONG alignment;BYTE bytes[4096];} workspace;
static BYTE csr[2048],out[2048],tbs[2048],pubder[512],subject[128],root_serial[16],ski_bytes[32],extensions[3][128];
static CERT_INFO fixture_ca;
static CERT_EXTENSION ext[3];
static DWORD csr_size;
static PAPACC_ISSUER_RESULT root_read(void *ctx,const PAPACC_U8 id[16],PAPACC_ISSUER_VIEW *view,PAPACC_ISSUER_RECORD *r,int *found)
{(void)ctx;source_reads++;
 if(source_fault_at&&source_reads==source_fault_at){
  if(source_fault==1U)fixture_view.epoch++;if(source_fault==2U)fixture_view.generation[0]^=1U;if(source_fault==3U)fixture_view.revision++;
  if(source_fault==4U)return PAPACC_ISSUER_UNCERTAIN;if(source_fault==5U)papacc_issuer_cng_backend_stop(&fixture_backend);
  if(source_fault==6U)fixture_view.administrative_epoch++;if(source_fault==7U)fixture_view.high_water++;if(source_fault==8U)fixture_view.policy[0]^=1U;
 }
 *view=fixture_view;*found=!memcmp(id,record.request.id,16);if(*found)*r=record;return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT root_cas(void *ctx,const PAPACC_ISSUER_VIEW *view,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const PAPACC_U8 *der,PAPACC_SIZE n,int publish)
{(void)ctx;(void)old;(void)der;(void)n;if(publish)return PAPACC_ISSUER_DENIED;if(view->revision!=fixture_view.revision)return PAPACC_ISSUER_CONFLICT;record=*next;fixture_view.high_water=high;fixture_view.revision++;fixture_view.administrative_epoch++;return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT admin(void *ctx,const PAPACC_ISSUER_VIEW *view,const PAPACC_ISSUER_REQUEST *r,unsigned op)
{(void)ctx;(void)view;(void)r;(void)op;return deny_admin?PAPACC_ISSUER_DENIED:PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT clock_value(void *ctx,PAPACC_U64 *now){(void)ctx;*now=1700000000ULL;return PAPACC_ISSUER_OK;}
static PCCERT_CONTEXT fixture(const char *path)
{BYTE text[8192],der[4096];FILE *f;size_t n;DWORD count=sizeof(der);PCCERT_CONTEXT c;if(fopen_s(&f,path,"rb"))return NULL;n=fread(text,1,sizeof(text),f);if(ferror(f)||fclose(f)||!n||n==sizeof(text))return NULL;if(!CryptStringToBinaryA((char *)text,(DWORD)n,CRYPT_STRING_BASE64,der,&count,NULL,NULL))return NULL;c=CertCreateCertificateContext(X509_ASN_ENCODING,der,count);return c;}
static int initialize(const CERT_PUBLIC_KEY_INFO *client)
{
 CERT_REQUEST_INFO q;CERT_SIGNED_CONTENT_INFO signed_request;CERT_BASIC_CONSTRAINTS2_INFO bc={TRUE,TRUE,0};CRYPT_BIT_BLOB ku;CRYPT_DATA_BLOB ski;
 DWORD n,z=32;BYTE usage=6,signature[256]={0};unsigned i,k;static const wchar_t hex[]=L"0123456789abcdef";
 memset(&fixture_core,0,sizeof(fixture_core));memset(&fixture_backend,0,sizeof(fixture_backend));memset(&record,0,sizeof(record));memset(&fixture_view,0,sizeof(fixture_view));memset(&fixture_ca,0,sizeof(fixture_ca));memset(&key,0,sizeof(key));memset(&administrative,0,sizeof(administrative));memset(&root,0,sizeof(root));
 private_calls=validation_calls=0;source_reads=source_fault_at=source_fault=0;fail_sign=0;bad_signature=deny_admin=stop_on_sign=fail_return=0;
 fixture_view.root[0]=1;fixture_view.authority[0]=2;fixture_view.generation[0]=3;fixture_view.prefix[0]=4;fixture_view.policy[0]=5;fixture_view.revision=1;fixture_view.epoch=9;fixture_view.administrative_epoch=101;fixture_view.high_water=2;fixture_view.ca_not_after=1690000000ULL+1825ULL*86400ULL;fixture_view.proofs=31;
 if(normal_profile){fixture_view.continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;fixture_view.proofs=PAPACC_ISSUER_NORMAL_PROOFS;}
 record.request.id[0]=1;record.request.device[0]=7;record.request.purpose=PAPACC_ISSUER_CLIENT;record.request.not_before=1700000000ULL;record.request.not_after=1700003600ULL;
 memcpy(record.authority,fixture_view.authority,16);memcpy(record.generation,fixture_view.generation,16);memcpy(record.policy,fixture_view.policy,32);memcpy(record.serial,fixture_view.prefix,8);record.serial[15]=2;record.counter=2;record.approved_epoch=9;record.state=PAPACC_ISSUER_SERIAL_RESERVED;
 CHECK(xspki(&ca_public,fixture_view.ca_spki_digest,pubder,sizeof(pubder)));CHECK(xspki(client,record.request.key_digest,pubder,sizeof(pubder)));
 memset(&q,0,sizeof(q));q.SubjectPublicKeyInfo=*client;n=128;CHECK(xname("CN=Ignored CSR Name ",record.request.device,subject,&n));q.Subject.pbData=subject;q.Subject.cbData=n;
 n=sizeof(tbs);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT_REQUEST_TO_BE_SIGNED,&q,0,NULL,tbs,&n));
 memset(&signed_request,0,sizeof(signed_request));signed_request.ToBeSigned.pbData=tbs;signed_request.ToBeSigned.cbData=n;signed_request.SignatureAlgorithm.pszObjId=(LPSTR)szOID_RSA_SHA256RSA;
 signed_request.SignatureAlgorithm.Parameters.pbData=(BYTE *)"\x05\x00";signed_request.SignatureAlgorithm.Parameters.cbData=2;if(native_crypto){CHECK(ephemeral_signature(ephemeral_client,tbs,n,signature));}
 signed_request.Signature.pbData=signature;signed_request.Signature.cbData=256;csr_size=sizeof(csr);
 CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&signed_request,native_crypto?CRYPT_ENCODE_NO_SIGNATURE_BYTE_REVERSAL_FLAG:0,NULL,csr,&csr_size));CHECK(CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,csr,csr_size,record.request.csr_digest,&z));
 fixture_ca.dwVersion=CERT_V3;fixture_ca.SubjectPublicKeyInfo=ca_public;CHECK(xname("CN=Papinho CA ",fixture_view.authority,subject,&n));fixture_ca.Subject.pbData=subject;fixture_ca.Subject.cbData=n;fixture_ca.Issuer=fixture_ca.Subject;
 root_serial[0]=1;memset(root_serial+1,0,15);for(i=0;i<8;i++)root_serial[8+i]=fixture_view.prefix[7-i];n=16;while(n>1&&!root_serial[n-1])n--;fixture_ca.SerialNumber.pbData=root_serial;fixture_ca.SerialNumber.cbData=n;
 fixture_ca.SignatureAlgorithm.pszObjId=(LPSTR)szOID_RSA_SHA256RSA;fixture_ca.SignatureAlgorithm.Parameters.pbData=(BYTE *)"\x05\x00";fixture_ca.SignatureAlgorithm.Parameters.cbData=2;
 CHECK(xtime(1690000000ULL,&fixture_ca.NotBefore));CHECK(xtime(fixture_view.ca_not_after,&fixture_ca.NotAfter));memset(ext,0,sizeof(ext));
 ext[0].pszObjId=(LPSTR)szOID_BASIC_CONSTRAINTS2;ext[0].fCritical=TRUE;ext[1].pszObjId=(LPSTR)szOID_KEY_USAGE;ext[1].fCritical=TRUE;ext[2].pszObjId=(LPSTR)szOID_SUBJECT_KEY_IDENTIFIER;
 for(i=0;i<3;i++){ext[i].Value.pbData=extensions[i];ext[i].Value.cbData=128;}
 CHECK(xencode(X509_BASIC_CONSTRAINTS2,&bc,extensions[0],128,&ext[0].Value.cbData));ku.cbData=1;ku.pbData=&usage;ku.cUnusedBits=1;
 CHECK(xencode(X509_KEY_USAGE,&ku,extensions[1],128,&ext[1].Value.cbData));z=32;CHECK(CryptHashPublicKeyInfo(0,CALG_SHA_256,0,X509_ASN_ENCODING,&ca_public,ski_bytes,&z));ski.pbData=ski_bytes;ski.cbData=32;
 CHECK(xencode(X509_OCTET_STRING,&ski,extensions[2],128,&ext[2].Value.cbData));fixture_ca.cExtension=3;fixture_ca.rgExtension=ext;
 root.ctx=&fixture_view;root.read=root_read;root.cas=root_cas;root.test_only=1;administrative.authorize=admin;administrative.utc=clock_value;administrative.test_only=1;
 key.state=PAPACC_CA_KEY_READY;key.key=101;key.provider=202;key.thread=GetCurrentThreadId();k=0;{const wchar_t *prefix=L"PapinhoAccelerator.CA.";while(*prefix)key.name[k++]=*prefix++;}
 for(i=0;i<16;i++){key.name[k++]=hex[fixture_view.authority[i]>>4];key.name[k++]=hex[fixture_view.authority[i]&15];}key.name[k++]=L'.';for(i=0;i<16;i++){key.name[k++]=hex[fixture_view.generation[i]>>4];key.name[k++]=hex[fixture_view.generation[i]&15];}key.name[k]=0;
 CHECK(papacc_issuer_cng_backend_init(&fixture_backend,&root,&administrative,&fixture_core,&key,&fixture_ca,workspace.bytes,sizeof(workspace.bytes),2084039)==PAPACC_ISSUER_OK);
 if(defer_core_init)return 0;
 CHECK((normal_profile?papacc_issuer_init_normal(&fixture_core,&root,&fixture_backend.ports):papacc_issuer_init(&fixture_core,&root,&fixture_backend.ports))==PAPACC_ISSUER_OK);return 0;
}
#ifdef PAPACC_OFFLINE_D3_WORKER
#include "issuer_normal_source_composition_cases.inc"
#include "issuer_first_existing_ca_cases.inc"
#include "issuer_native_ephemeral_cases.inc"
#endif
static int inspect_leaf(int package)
{
 PCCERT_CONTEXT leaf;BYTE usage=0,storage[128];DWORD n=sizeof(storage);char name[128];CERT_ENHKEY_USAGE *eku=(CERT_ENHKEY_USAGE *)storage;
 PCERT_EXTENSION extension;CERT_BASIC_CONSTRAINTS2_INFO bc;
 leaf=CertCreateCertificateContext(X509_ASN_ENCODING,out,(DWORD)record.certificate_size);CHECK(leaf!=NULL);
 CHECK(leaf->pCertInfo->dwVersion==CERT_V3&&leaf->pCertInfo->cExtension==5);
 CHECK(CertGetNameStringA(leaf,CERT_NAME_SIMPLE_DISPLAY_TYPE,0,NULL,name,sizeof(name))>1);
 CHECK(!strcmp(name,package?"Papinho Package 07000000000000000000000000000000":"Papinho Client 07000000000000000000000000000000"));
 CHECK(CertGetIntendedKeyUsage(X509_ASN_ENCODING,leaf->pCertInfo,&usage,1)&&usage==CERT_DIGITAL_SIGNATURE_KEY_USAGE);
 extension=CertFindExtension(szOID_BASIC_CONSTRAINTS2,5,leaf->pCertInfo->rgExtension);CHECK(extension&&extension->fCritical);n=sizeof(bc);
 CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_BASIC_CONSTRAINTS2,extension->Value.pbData,extension->Value.cbData,0,NULL,&bc,&n)&&!bc.fCA&&!bc.fPathLenConstraint);
 extension=CertFindExtension(szOID_ENHANCED_KEY_USAGE,5,leaf->pCertInfo->rgExtension);CHECK(extension&&!extension->fCritical);n=sizeof(storage);
 CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_ENHANCED_KEY_USAGE,extension->Value.pbData,extension->Value.cbData,0,NULL,storage,&n));
 CHECK(eku->cUsageIdentifier==1&&!strcmp(eku->rgpszUsageIdentifier[0],package?szOID_PKIX_KP_CODE_SIGNING:szOID_PKIX_KP_CLIENT_AUTH));
 CHECK(!CertFindExtension(szOID_SUBJECT_ALT_NAME2,5,leaf->pCertInfo->rgExtension));CHECK(CertFreeCertificateContext(leaf));return 0;
}
static int server_profile(const CERT_PUBLIC_KEY_INFO *public_key)
{
 DWORD n,z;BYTE digest[32],signature[256]={0},storage[2048],usage;CERT_INFO *info=(CERT_INFO *)storage;
 CERT_SIGNED_CONTENT_INFO signed_cert;PCERT_EXTENSION extension;CERT_ALT_NAME_INFO *alt;CERT_ENHKEY_USAGE *eku;
 union {ULONGLONG alignment;BYTE bytes[1024];} decoded;
 const char *name="accel.example";char maximum[254];unsigned i;
 CHECK(!initialize(public_key));record.request.purpose=PAPACC_ISSUER_SERVER;
 CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_OK);
 z=sizeof(storage);CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,tbs,n,0,NULL,storage,&z));
 CHECK(info->cExtension==6&&info->dwVersion==CERT_V3);CHECK(info->Subject.cbData==2&&info->Subject.pbData[0]==0x30&&info->Subject.pbData[1]==0);usage=0;
 CHECK(CertGetIntendedKeyUsage(X509_ASN_ENCODING,info,&usage,1)&&usage==CERT_DIGITAL_SIGNATURE_KEY_USAGE);
 extension=CertFindExtension(szOID_ENHANCED_KEY_USAGE,info->cExtension,info->rgExtension);CHECK(extension&&!extension->fCritical);z=sizeof(decoded);
 CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_ENHANCED_KEY_USAGE,extension->Value.pbData,extension->Value.cbData,0,NULL,decoded.bytes,&z));
 eku=(CERT_ENHKEY_USAGE *)decoded.bytes;CHECK(eku->cUsageIdentifier==1&&!strcmp(eku->rgpszUsageIdentifier[0],szOID_PKIX_KP_SERVER_AUTH));
 extension=CertFindExtension(szOID_SUBJECT_ALT_NAME2,info->cExtension,info->rgExtension);CHECK(extension&&extension->fCritical);z=sizeof(decoded);
 CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_ALTERNATE_NAME,extension->Value.pbData,extension->Value.cbData,0,NULL,decoded.bytes,&z));
 alt=(CERT_ALT_NAME_INFO *)decoded.bytes;CHECK(alt->cAltEntry==1&&alt->rgAltEntry[0].dwAltNameChoice==CERT_ALT_NAME_DNS_NAME&&!wcscmp(alt->rgAltEntry[0].pwszDNSName,L"accel.example"));
 memset(&signed_cert,0,sizeof(signed_cert));signed_cert.ToBeSigned.pbData=tbs;signed_cert.ToBeSigned.cbData=n;
 signed_cert.SignatureAlgorithm=info->SignatureAlgorithm;signed_cert.Signature.pbData=signature;signed_cert.Signature.cbData=256;z=sizeof(out);
 CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&signed_cert,0,NULL,out,&z));
 CHECK(papacc_issuer_x509_server_verify(&fixture_view,&record,&fixture_ca,name,13,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_x509_server_verify(&fixture_view,&record,&fixture_ca,"other.example",13,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_INVALID);
 CHECK(papacc_issuer_x509_verify(&fixture_view,&record,&fixture_ca,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_UNSUPPORTED);
 bad_signature=1;CHECK(papacc_issuer_x509_server_verify(&fixture_view,&record,&fixture_ca,name,13,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_INVALID);bad_signature=0;
 CHECK(papacc_issuer_windows_verify_request(&fixture_backend.workspace,&record,csr,csr_size)==PAPACC_ISSUER_OK);
 bad_signature=1;CHECK(papacc_issuer_windows_verify_request(&fixture_backend.workspace,&record,csr,csr_size)==PAPACC_ISSUER_INVALID);bad_signature=0;
 /* Independently corrupt role fields without adding a CN fallback.
  * Synthetic outer signatures never stand in for a native private operation. */
 for(i=0;i<4;i++){
  CERT_ENHKEY_USAGE wrong_eku;LPSTR wrong_oid=(LPSTR)szOID_PKIX_KP_CLIENT_AUTH;BYTE wrong_usage=0xa0;
  CRYPT_BIT_BLOB bits;CERT_BASIC_CONSTRAINTS2_INFO wrong_bc={TRUE,FALSE,0};DWORD changed;
  CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_OK);
  changed=sizeof(storage);CHECK(CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,tbs,n,0,NULL,storage,&changed));
  if(i==0)info->cExtension=5; /* Remove the only identity-bearing extension. */
  else{
   extension=CertFindExtension(i==1?szOID_KEY_USAGE:(i==2?szOID_ENHANCED_KEY_USAGE:szOID_BASIC_CONSTRAINTS2),info->cExtension,info->rgExtension);CHECK(extension!=NULL);changed=sizeof(decoded);
   if(i==1){bits.pbData=&wrong_usage;bits.cbData=1;bits.cUnusedBits=5;CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_KEY_USAGE,&bits,0,NULL,decoded.bytes,&changed));}
   else if(i==2){wrong_eku.cUsageIdentifier=1;wrong_eku.rgpszUsageIdentifier=&wrong_oid;CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_ENHANCED_KEY_USAGE,&wrong_eku,0,NULL,decoded.bytes,&changed));}
   else CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_BASIC_CONSTRAINTS2,&wrong_bc,0,NULL,decoded.bytes,&changed));
   extension->Value.pbData=decoded.bytes;extension->Value.cbData=changed;
  }
  n=sizeof(tbs);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,info,0,NULL,tbs,&n));
  signed_cert.ToBeSigned.cbData=n;z=sizeof(out);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&signed_cert,0,NULL,out,&z));
  CHECK(papacc_issuer_x509_server_verify(&fixture_view,&record,&fixture_ca,name,13,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_INVALID);
 }

 CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_OK);
 signed_cert.ToBeSigned.cbData=n;z=sizeof(out);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&signed_cert,0,NULL,out,&z));
 fixture_backend.workspace.stopped=1;CHECK(papacc_issuer_x509_server_verify(&fixture_view,&record,&fixture_ca,name,13,out,z,&fixture_backend.workspace,digest)==PAPACC_ISSUER_STOPPED);fixture_backend.workspace.stopped=0;
 {const char *invalid[]={"","*.example","127.0.0.1","-bad.example","bad-.example","a..b","a.example.","a;CN=bad","a_b.example"};
 for(i=0;i<sizeof(invalid)/sizeof(invalid[0]);i++)CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,invalid[i],(DWORD)strlen(invalid[i]),tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID);}
 CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,"a\0b",3,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID);
 for(i=0;i<253;i++)maximum[i]=((i==63)||(i==127)||(i==191))?'.':'a';maximum[253]=0;
 CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,maximum,253,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_OK);
 maximum[63]='a';CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,maximum,253,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID);
 record.serial[15]=3;CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_DENIED);record.serial[15]=2;
 CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,8,&n)==PAPACC_ISSUER_LIMIT);
 record.request.purpose=PAPACC_ISSUER_CLIENT;CHECK(papacc_issuer_x509_server_tbs(&fixture_view,&record,&fixture_ca,public_key,name,13,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID);
 CHECK(!private_calls);papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==PAPACC_ISSUER_OK);return 0;
}
int main(int argc,char **argv)
{
 PCCERT_CONTEXT caf,client;PAPACC_ISSUER_RESULT result;DWORD n;PAPACC_SIZE saved;unsigned before;
 if(argc==4&&!strcmp(argv[3],"--native-ephemeral"))return native_ephemeral_journey();if(argc!=3)return 2;caf=fixture(argv[1]);client=fixture(argv[2]);CHECK(caf&&client);ca_public=caf->pCertInfo->SubjectPublicKeyInfo;
 normal_profile=1;CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_OK&&private_calls==1);
 CHECK(papacc_issuer_recover(&fixture_core,record.request.id)==PAPACC_ISSUER_DENIED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_view.proofs|=PAPACC_ISSUER_PROOF_COMPLETE_CHAIN;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_UNCERTAIN&&!private_calls);
 normal_profile=0;
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));
 result=papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out));CHECK(result==PAPACC_ISSUER_OK);CHECK(private_calls==1&&record.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED&&fixture_backend.active==0&&!key.borrower);
 CHECK(!inspect_leaf(0));printf("MODEL_PUBLIC_DER_BYTES=%Iu CSR_BYTES=%lu NO_VALID_NATIVE_SIGNATURE\n",record.certificate_size,csr_size);
 before=private_calls;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_UNSUPPORTED);CHECK(private_calls==before);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.state=PAPACC_ISSUER_SIGN_REQUESTED;
 {void *lease=NULL;PAPACC_SIZE z=0;CHECK(fixture_backend.ports.acquire(&fixture_backend,&fixture_view,&record,&lease)==PAPACC_ISSUER_DENIED&&!lease);
 CHECK(fixture_backend.ports.sign(&fixture_backend,&fixture_backend,&record,csr,csr_size,out,sizeof(out),&z)==PAPACC_ISSUER_DENIED&&!private_calls);}

 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,16)==PAPACC_ISSUER_LIMIT&&!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fail_return=1;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_RETAINED);CHECK(private_calls==1&&key.borrower==&fixture_backend&&fixture_core.lease==&fixture_backend&&record.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_OK);
 {BYTE digest[32];bad_signature=1;CHECK(papacc_issuer_x509_verify(&fixture_view,&record,&fixture_ca,out,(DWORD)record.certificate_size,&fixture_backend.workspace,digest)==PAPACC_ISSUER_INVALID);
 bad_signature=0;out[10]^=1;CHECK(papacc_issuer_x509_verify(&fixture_view,&record,&fixture_ca,out,(DWORD)record.certificate_size,&fixture_backend.workspace,digest)!=PAPACC_ISSUER_OK);}
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_ca.cExtension=4;CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&client->pCertInfo->SubjectPublicKeyInfo,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_UNSUPPORTED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));memcpy(record.request.key_digest,fixture_view.ca_spki_digest,32);
 CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&ca_public,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.request.purpose=PAPACC_ISSUER_PACKAGE;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_OK);CHECK(!inspect_leaf(1));
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.counter=0;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))!=0);CHECK(!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));bad_signature=1;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_INVALID);CHECK(!private_calls&&record.state==PAPACC_ISSUER_SERIAL_RESERVED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));deny_admin=1;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_DENIED);CHECK(!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_backend.key=NULL;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_ABSENT);CHECK(!private_calls&&record.state==PAPACC_ISSUER_SIGN_REQUESTED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fail_sign=NTE_PERM;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_RETAINED);CHECK(private_calls==1&&fixture_backend.native_error==NTE_PERM&&fixture_backend.native_stage==4&&fixture_core.retained_output==out&&key.borrower==&fixture_backend);
 CHECK(papacc_issuer_finish(&fixture_core)==PAPACC_ISSUER_RETAINED);CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));stop_on_sign=1;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_RETAINED);CHECK(private_calls==1&&fixture_core.stopped&&fixture_backend.retained);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));papacc_issuer_stop(&fixture_core);CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_STOPPED);CHECK(!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_view.proofs=0;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_UNCERTAIN);CHECK(!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.serial[15]=3;CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&client->pCertInfo->SubjectPublicKeyInfo,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_DENIED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_ca.SignatureAlgorithm.pszObjId=(LPSTR)szOID_RSA_SHA1RSA;
 CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&client->pCertInfo->SubjectPublicKeyInfo,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_INVALID&&!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.request.purpose=PAPACC_ISSUER_SERVER;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_UNSUPPORTED&&!private_calls&&record.state==PAPACC_ISSUER_SERIAL_RESERVED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.request.purpose=PAPACC_ISSUER_SERVER;CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&client->pCertInfo->SubjectPublicKeyInfo,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_UNSUPPORTED);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));saved=fixture_backend.reserved;fixture_backend.reserved=PAPACC_ED11_PROCESS_LIMIT;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_LIMIT);CHECK(!private_calls);fixture_backend.reserved=saved;
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));fixture_view.prefix[0]=0;fixture_view.prefix[1]=128;memcpy(record.serial,fixture_view.prefix,8);
 {unsigned i,at;for(i=0;i<8;i++)root_serial[8+i]=fixture_view.prefix[7-i];fixture_ca.SerialNumber.cbData=16;
 CHECK(papacc_issuer_x509_tbs(&fixture_view,&record,&fixture_ca,&client->pCertInfo->SubjectPublicKeyInfo,tbs,sizeof(tbs),&n)==PAPACC_ISSUER_OK);
 at=(tbs[1]&128U)?2U+(tbs[1]&127U):2U;CHECK(tbs[at]==0xa0);at+=2U+tbs[at+1U];
 CHECK(tbs[at]==2U&&tbs[at+1U]==16U&&tbs[at+2U]==0U&&tbs[at+3U]==128U);}
 CHECK(!server_profile(&client->pCertInfo->SubjectPublicKeyInfo));
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==PAPACC_ISSUER_OK);
 normal_profile=1;CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));key.borrower=&fixture_backend;fixture_backend.custody=1;
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==PAPACC_ISSUER_DENIED&&key.borrower==&fixture_backend);
 fail_return=1;CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==0&&key.borrower==&fixture_backend&&!fixture_backend.active&&fixture_backend.custody&&private_calls==1);fail_return=0;
 papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==PAPACC_ISSUER_DENIED);
 papacc_issuer_stop(&fixture_core);CHECK(papacc_issuer_finish(&fixture_core)==0);CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0&&!key.borrower&&!fixture_backend.custody);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==PAPACC_ISSUER_DENIED);
 normal_profile=0;CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));key.borrower=&fixture_backend;fixture_backend.custody=1;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_DENIED&&!private_calls&&key.borrower==&fixture_backend);
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==0);fail_return=1;
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==PAPACC_ISSUER_RETAINED&&fixture_backend.custody&&key.borrower==&fixture_backend);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==PAPACC_ISSUER_RETAINED&&key.borrower==&fixture_backend);
 normal_profile=1;defer_core_init=1;CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));defer_core_init=0;
 key.borrower=&fixture_backend;fixture_backend.custody=1;papacc_issuer_cng_backend_stop(&fixture_backend);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0&&!key.borrower&&!fixture_core.initialized&&!private_calls);
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));key.borrower=&fixture_core;fixture_backend.custody=1;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_RETAINED&&fixture_backend.retained&&!private_calls&&key.borrower==&fixture_core);
 #ifdef PAPACC_OFFLINE_D3_WORKER
 CHECK(!normal_source_composition_cases(&client->pCertInfo->SubjectPublicKeyInfo));
 CHECK(!first_existing_ca_cases(&client->pCertInfo->SubjectPublicKeyInfo));
#endif
 CHECK(CertFreeCertificateContext(client));CHECK(CertFreeCertificateContext(caf));
 printf("PASS %u SYNTHETIC_SIGNATURES_AND_ROOT_ONLY native_private_operations=NOT_RUN backend=%u workspace=%u core=%u key=%u\n",checks,(unsigned)sizeof(fixture_backend),(unsigned)sizeof(workspace),(unsigned)sizeof(fixture_core),(unsigned)sizeof(key));return 0;
}
