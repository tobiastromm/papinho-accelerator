#define _WIN32_WINNT 0x0600
/* Isolated existing synthetic-signature X509 test extended for kind8 DATA.
 * No keygen/private signature, fixture files or production trust result. */
#include "server_issuer_x509_win32.h"
#ifdef PAPACC_KIND8_NATIVE_NEGATIVE
#define papacc_issuer_x509_tbs kind8_model_papacc_issuer_x509_tbs
#define papacc_issuer_x509_verify kind8_model_papacc_issuer_x509_verify
#define papacc_issuer_x509_server_tbs kind8_model_papacc_issuer_x509_server_tbs
#define papacc_issuer_x509_server_verify kind8_model_papacc_issuer_x509_server_verify
#endif
#define main existing_x509_model_main
#include "issuer_cng_x509_test.c"
#undef main
#ifdef PAPACC_KIND8_NATIVE_NEGATIVE
#undef papacc_issuer_x509_tbs
#undef papacc_issuer_x509_verify
#undef papacc_issuer_x509_server_tbs
#undef papacc_issuer_x509_server_verify
#define EXPECT_KIND8 PC_ROOT8_INVALID
#else
#define EXPECT_KIND8 PC_ROOT8_OK
#endif
#include "../apps/server/server_config_root8_signer_win32.c"
static void put16(BYTE *p,unsigned n){p[0]=(BYTE)n;p[1]=(BYTE)(n>>8);}
static void put64(BYTE *p,PAPACC_U64 n){unsigned j;for(j=0;j<8;j++){p[j]=(BYTE)n;n>>=8;}}
static int hash_data(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 d[32]){(void)ctx;return n<=0xffffffffU&&xhash(p,(DWORD)n,d);}
static BYTE carrier[16384],row[384],header[512],kind8_der[8192];
int main(int argc,char **argv){
 PCCERT_CONTEXT caf,client;PAPACC_ISSUER_VIEW initial,observed;PAPACC_ISSUER_RECORD event,issued;
 PAPACC_ROOT8_SIGNER_DATA data;BYTE anchor[32]={1};unsigned start,i;PAPACC_SIZE oldcap;
 CHECK(argc==3);caf=fixture(argv[1]);client=fixture(argv[2]);CHECK(caf&&client);ca_public=caf->pCertInfo->SubjectPublicKeyInfo;
 CHECK(!initialize(&client->pCertInfo->SubjectPublicKeyInfo));record.request.purpose=PAPACC_ISSUER_PACKAGE;
 CHECK(papacc_issuer_issue(&core,record.request.id,csr,csr_size,out,sizeof(out))==PAPACC_ISSUER_OK);
 issued=record;initial=v;initial.high_water=1;initial.revision=1;
 CHECK(papacc_issuer_image_init(carrier+8192,&initial,anchor,hash_data,NULL)==PAPACC_ISSUER_OK);
 event=issued;event.state=PAPACC_ISSUER_PREPARED;event.counter=0;memset(event.serial,0,16);event.certificate_size=0;memset(event.certificate_digest,0,32);
 CHECK(papacc_issuer_image_append(carrier+8192,&event,NULL,0,hash_data,NULL)==PAPACC_ISSUER_OK);
 event.counter=issued.counter;memcpy(event.serial,issued.serial,16);event.state=PAPACC_ISSUER_SERIAL_RESERVED;
 CHECK(papacc_issuer_image_append(carrier+8192,&event,NULL,0,hash_data,NULL)==PAPACC_ISSUER_OK);
 event.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(papacc_issuer_image_append(carrier+8192,&event,NULL,0,hash_data,NULL)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_image_append(carrier+8192,&issued,out,issued.certificate_size,hash_data,NULL)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_image_validate(carrier+8192,&observed,hash_data,NULL)==PAPACC_ISSUER_OK);
 memcpy(header,"PACROOT8",8);put16(header+8,3);put16(header+10,512);memcpy(header+72,observed.root,32);put64(header+120,observed.revision);
 memcpy(row,"PACPDL01",8);put16(row+8,1);put16(row+10,384);put16(row+16,1);put16(row+18,1);
 memcpy(row+64,observed.authority,16);memcpy(row+80,observed.generation,16);memcpy(row+96,issued.request.id,16);
 memcpy(row+112,issued.certificate_digest,32);memcpy(row+144,issued.request.key_digest,32);memcpy(row+260,observed.ca_spki_digest,32);
 put64(row+176,issued.request.not_before);put64(row+184,issued.request.not_after);
 memset(&data,0,sizeof(data));data.ca=&ca;data.workspace=&backend.workspace;data.certificate=kind8_der;data.certificate_capacity=sizeof(kind8_der);data.hash=hash_data;
 data.already_reserved=sizeof(carrier)+sizeof(row)+sizeof(header)+sizeof(kind8_der)+sizeof(workspace)+sizeof(data)+sizeof(ca)+sizeof(backend)+sizeof(core)+sizeof(record)+sizeof(v)+sizeof(csr)+sizeof(out)+sizeof(tbs)+sizeof(pubder)+sizeof(subject)+sizeof(root_serial)+sizeof(ski_bytes)+sizeof(extensions)+sizeof(ext)+sizeof(key)+sizeof(initial)+sizeof(observed)+sizeof(event)+sizeof(issued)+sizeof(anchor);
 start=private_calls;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==EXPECT_KIND8);CHECK(private_calls==start);
 data.ca=NULL;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==PC_ROOT8_INCOMPLETE);data.ca=&ca;
 data.workspace->stopped=1;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==PC_ROOT8_INCOMPLETE);data.workspace->stopped=0;
 oldcap=data.already_reserved;data.already_reserved=PAPACC_ED11_PROCESS_LIMIT;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==PC_ROOT8_INCOMPLETE);data.already_reserved=oldcap;
 CHECK(papacc_root8_signer_data(&data,7,carrier,sizeof(carrier),row,header)==PC_ROOT8_UNSUPPORTED);
 CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier)-1,row,header)==PC_ROOT8_INVALID);
 for(i=0;i<384;i++)if((i>=64&&i<176)||(i>=260&&i<292)||i==18){row[i]^=1;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)!=PC_ROOT8_OK);row[i]^=1;}
 carrier[8200]^=1;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==PC_ROOT8_INVALID);carrier[8200]^=1;
 bad_signature=1;CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==PC_ROOT8_INVALID);bad_signature=0;
 put16(row+16,2);CHECK(papacc_root8_signer_data(&data,8,carrier,sizeof(carrier),row,header)==EXPECT_KIND8); /* historical revocation remains DATA, never approval */
 CHECK(private_calls==start);CHECK(CertFreeCertificateContext(client));CHECK(CertFreeCertificateContext(caf));
 printf("PASS %u KIND8_DATA_MODEL_OR_NATIVE_NEGATIVE_ONLY context=%u workspace=%u caller_named=%Iu NO_CURRENT_NO_TRUST_NO_NATIVE_PRIVATE_SIGNATURE\n",checks,(unsigned)sizeof(data),(unsigned)sizeof(*data.workspace),data.already_reserved);return 0;
}
