#define _WIN32_WINNT 0x0600
#include "server_issuer_windows_validation.h"
#include <wincrypt.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
static union {ULONGLONG align;BYTE bytes[4096];} scratch;
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL %u at %u\n",checks,(unsigned)__LINE__);return 1;}}while(0)
static int public_csr_negative(const char *path)
{
 static BYTE text[8192],certificate[4096],tbs[2048],csr[4096],spki[1024],signature[256];
 BYTE null_parameters[2]={5,0};FILE *f;size_t read;DWORD n,tbs_size,csr_size,spki_size;
 PCCERT_CONTEXT cert;CERT_REQUEST_INFO request;CERT_SIGNED_CONTENT_INFO signed_info;
 PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_WINDOWS_WORKSPACE w;
 CHECK(fopen_s(&f,path,"rb")==0);CHECK(f!=NULL);read=fread(text,1,sizeof(text),f);CHECK(!ferror(f)&&read>0&&read<sizeof(text));CHECK(fclose(f)==0);
 n=sizeof(certificate);CHECK(CryptStringToBinaryA((const char *)text,(DWORD)read,CRYPT_STRING_BASE64,certificate,&n,NULL,NULL));
 cert=CertCreateCertificateContext(X509_ASN_ENCODING,certificate,n);CHECK(cert!=NULL);
 memset(&request,0,sizeof(request));request.Subject=cert->pCertInfo->Subject;request.SubjectPublicKeyInfo=cert->pCertInfo->SubjectPublicKeyInfo;
 tbs_size=sizeof(tbs);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT_REQUEST_TO_BE_SIGNED,&request,0,NULL,tbs,&tbs_size));
 memset(&signed_info,0,sizeof(signed_info));signed_info.ToBeSigned.pbData=tbs;signed_info.ToBeSigned.cbData=tbs_size;
 signed_info.SignatureAlgorithm.pszObjId=(LPSTR)szOID_RSA_SHA256RSA;
 signed_info.SignatureAlgorithm.Parameters.pbData=null_parameters;signed_info.SignatureAlgorithm.Parameters.cbData=2;
 signed_info.Signature.pbData=signature;signed_info.Signature.cbData=sizeof(signature);
 csr_size=sizeof(csr);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&signed_info,0,NULL,csr,&csr_size));
 memset(&r,0,sizeof(r));r.state=PAPACC_ISSUER_SERIAL_RESERVED;r.request.purpose=PAPACC_ISSUER_CLIENT;
 n=32;CHECK(CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,csr,csr_size,r.request.csr_digest,&n));
 spki_size=sizeof(spki);CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_PUBLIC_KEY_INFO,&request.SubjectPublicKeyInfo,0,NULL,spki,&spki_size));
 n=32;CHECK(CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,spki,spki_size,r.request.key_digest,&n));
 memset(&w,0,sizeof(w));w.bytes=scratch.bytes;w.capacity=sizeof(scratch.bytes);
 /* Structurally valid CSR from an existing PUBLIC fixture key, zero signature:
  * actual native proof-of-possession must fail; no private key operation. */
 CHECK(papacc_issuer_windows_verify_request(&w,&r,csr,csr_size)==PAPACC_ISSUER_INVALID);CHECK(w.native_error!=0);
 r.request.key_digest[0]^=1;CHECK(papacc_issuer_windows_verify_request(&w,&r,csr,csr_size)==PAPACC_ISSUER_CONFLICT);r.request.key_digest[0]^=1;
 csr[csr_size++]=0;n=32;CHECK(CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,csr,csr_size,r.request.csr_digest,&n));
 CHECK(papacc_issuer_windows_verify_request(&w,&r,csr,csr_size)==PAPACC_ISSUER_INVALID);
 CHECK(CertFreeCertificateContext(cert));return 0;
}
int main(int argc,char **argv)
{
 PAPACC_ISSUER_WINDOWS_WORKSPACE w;PAPACC_ISSUER_RECORD r;PAPACC_CA_KEY key,saved;
 PAPACC_ISSUER_BACKEND backend;BYTE invalid[4]={0x30,0x02,0x30,0x00};DWORD n;
 memset(&w,0,sizeof(w));memset(&r,0,sizeof(r));memset(&key,0,sizeof(key));memset(&backend,0,sizeof(backend));
 w.bytes=scratch.bytes;w.capacity=sizeof(scratch.bytes);r.state=PAPACC_ISSUER_SERIAL_RESERVED;r.request.purpose=PAPACC_ISSUER_CLIENT;
 backend.ctx=&w;backend.verify_request=papacc_issuer_windows_verify_request;
 CHECK(backend.verify_request(backend.ctx,&r,NULL,0)==PAPACC_ISSUER_INVALID);
 w.stopped=1;CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))==PAPACC_ISSUER_STOPPED);w.stopped=0;
 CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))==PAPACC_ISSUER_CONFLICT);
 n=32;CHECK(CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,invalid,sizeof(invalid),r.request.csr_digest,&n));
 CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))==PAPACC_ISSUER_INVALID);CHECK(w.native_error!=0);
 w.capacity=0;CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))!=PAPACC_ISSUER_OK);w.capacity=sizeof(scratch.bytes);
 r.state=PAPACC_ISSUER_PREPARED;CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))==PAPACC_ISSUER_DENIED);
 r.state=PAPACC_ISSUER_SERIAL_RESERVED;w.bytes=invalid;w.capacity=sizeof(invalid);
 CHECK(backend.verify_request(backend.ctx,&r,invalid,sizeof(invalid))==PAPACC_ISSUER_INVALID);
 CHECK(papacc_issuer_windows_key_result(NULL,NTE_PERM)==PAPACC_ISSUER_ABSENT);
 CHECK(papacc_issuer_windows_key_result(&key,NTE_PERM)==PAPACC_ISSUER_DENIED);
 CHECK(papacc_issuer_windows_key_result(&key,NTE_BAD_KEYSET)==PAPACC_ISSUER_UNCERTAIN);
 CHECK(papacc_issuer_windows_key_result(&key,NTE_NOT_SUPPORTED)==PAPACC_ISSUER_UNSUPPORTED);
 key.state=PAPACC_CA_KEY_READY;CHECK(papacc_issuer_windows_key_result(&key,ERROR_SUCCESS)==PAPACC_ISSUER_DENIED);
 key.state=PAPACC_CA_KEY_RETAINED;key.first_failure.original_code=NTE_PERM;saved=key;
 CHECK(papacc_issuer_windows_key_result(&key,NTE_PERM)==PAPACC_ISSUER_RETAINED);CHECK(!memcmp(&key,&saved,sizeof(key)));
 if(argc!=2||public_csr_negative(argv[1]))return 1;
 printf("PASS %u WINDOWS_PUBLIC_PREPARATION_NO_KEYS_NO_SIGNING owner=%u workspace=%u\n",checks,(unsigned)sizeof(w),(unsigned)sizeof(scratch));return 0;
}
