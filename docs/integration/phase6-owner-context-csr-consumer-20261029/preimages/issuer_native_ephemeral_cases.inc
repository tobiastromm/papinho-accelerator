/* Test translation only. Real ephemeral CNG; simulated authorization/CURRENT.
 * Never linked into the product or used as a source of operational proofs. */
#include "../src/issuer/issuer_codec.h"
static int persistent_note(const char *text)
{DWORD used=0,n=(DWORD)strlen(text);return WriteFile(persistent_receipt,text,n,&used,NULL)&&used==n&&FlushFileBuffers(persistent_receipt);}
static int persistent_fixture_prepare(const char *receipt)
{
 BYTE random[32];unsigned i,z;const wchar_t *prefix=L"Papacc.Qualification.FirstCA.",*hex=L"0123456789abcdef";char line[256];
 if(!receipt||strstr(receipt,"..")||strstr(receipt,"CURRENT"))return 0;
 if(BCryptGenRandom(NULL,random,sizeof(random),BCRYPT_USE_SYSTEM_PREFERRED_RNG))return 0;
 z=0;while(*prefix)qualification_key_name[z++]=*prefix++;
 for(i=0;i<32;i++){qualification_key_name[z++]=hex[random[i]>>4];qualification_key_name[z++]=hex[random[i]&15];}qualification_key_name[z]=0;
 persistent_receipt=CreateFileA(receipt,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
 if(persistent_receipt==INVALID_HANDLE_VALUE)return 0;
 if(sprintf_s(line,sizeof(line),"INTENT TEST_ONLY CURRENTUSER KEY=%ls NO_RETRY NO_DELETE\r\n",qualification_key_name)<0)return 0;
 return persistent_note(line);
}
static int persistent_reacquire(void)
{
 CERT_PUBLIC_KEY_INFO *public_value;union {ULONGLONG alignment;BYTE bytes[2048];} public_buffer;BYTE old_digest[32],new_digest[32];NCRYPT_KEY_HANDLE absent=0;
 /* Retire all first-instance borrows before closing native references. */
 memcpy(old_digest,fixture_view.ca_spki_digest,32);
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);
 CHECK(papacc_issuer_finish(&fixture_core)==0&&papacc_issuer_cng_backend_release_custody(&fixture_backend)==0&&!key.borrower);
 CHECK(persistent_note("CREATED; BORROWS_RETURNED; KEY_RETAINED_IN_KSP\r\n"));
 CHECK(NCryptFreeObject(ephemeral_ca)==0);ephemeral_ca=0;key.key=0;
 CHECK(NCryptFreeObject(ephemeral_client)==0);ephemeral_client=0;
 CHECK(NCryptFreeObject(ephemeral_provider)==0);ephemeral_provider=0;key.provider=0;
 CHECK(NCryptOpenStorageProvider(&ephemeral_provider,MS_KEY_STORAGE_PROVIDER,0)==0);
 CHECK(NCryptOpenKey(ephemeral_provider,&ephemeral_ca,qualification_key_name,0,NCRYPT_SILENT_FLAG)==0);
 CHECK(papacc_issuer_x509_key_public(ephemeral_ca,public_buffer.bytes,sizeof(public_buffer.bytes),&public_value)==0);
 CHECK(xspki(public_value,new_digest,pubder,sizeof(pubder))&&!memcmp(new_digest,old_digest,32));
 CHECK(NCryptOpenKey(ephemeral_provider,&absent,L"Papacc.Qualification.Missing.NoCreate",0,NCRYPT_SILENT_FLAG)!=0&&!absent);
 key.key=ephemeral_ca;key.provider=ephemeral_provider;key.created=0;
 CHECK(model_validate(&key)==0);
 memset(&fixture_core,0,sizeof(fixture_core));memset(&fixture_backend,0,sizeof(fixture_backend));
 memset(&source_owner,0,sizeof(source_owner));source_owner.initialized=source_owner.published=1;source_owner.writer=GetCurrentThreadId();source_owner.store02=6;
 memcpy(source_owner.domain,fixture_view.root,32);source_owner.slots[0].view.revision=fixture_view.revision;source_owner.slots[0].issuer_revision=fixture_view.revision;
 source_owner.slots[0].context_epoch=fixture_view.administrative_epoch;source_owner.slots[0].operational_epoch=fixture_view.epoch;
 CHECK(papacc_issuer_cng_backend_init(&fixture_backend,&root,&administrative,&fixture_core,&key,&fixture_ca,workspace.bytes,sizeof(workspace.bytes),2084039)==0);
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==0&&fixture_backend.source_kind==PAPACC_CA_SOURCE_EXISTING_CURRENT);
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)!=0); /* no duplicate custody */
 CHECK(papacc_configuration_existing_ca_activate_normal(&source_owner,&fixture_backend)==0);
 CHECK(persistent_note("REOPENED NEW_PROVIDER; NAME_SPKI_EXPORT_POLICY_MATCH; EXISTING_CURRENT_BOUND; PUBLICATION_STILL_SIMULATED\r\n"));
 return 0;
}
#include "c2_operation_win32.h"
#include "server_credential_revocation_win32.h"
#include "../apps/server/server_credential_revocation_win32.inc"
static PAPACC_C2_OPERATION_MANAGER ephemeral_manager;
static PAPACC_C2_SESSION ephemeral_sessions[4],ephemeral_unrelated;
static BYTE ephemeral_credentials[2][32];
static PAPACC_CREDENTIAL_REVOCATION_RECEIPT ephemeral_receipt;
static PAPACC_RESULT ephemeral_deny_policy(void *ctx,const PAPACC_C2_SESSION *session,const BYTE *data,PAPACC_SIZE bytes,PAPACC_BOOL *permit)
{(void)ctx;(void)session;(void)data;(void)bytes;*permit=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static PAPACC_RESULT ephemeral_deny_capacity(void *ctx,PAPACC_SIZE bytes,PAPACC_BOOL *permit)
{(void)ctx;(void)bytes;*permit=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static BYTE ephemeral_image[PAPACC_ISSUER_IMAGE_BYTES],ephemeral_copy[2048];
static PAPACC_ISSUER_PUBLICATION_FACTS ephemeral_ack;
static int ephemeral_hash(void *ctx,const BYTE *b,PAPACC_SIZE n,BYTE d[32])
{DWORD z=32;(void)ctx;return n<=MAXDWORD&&CryptHashCertificate2(BCRYPT_SHA256_ALGORITHM,0,NULL,b,(DWORD)n,d,&z)&&z==32;}
static PAPACC_ISSUER_RESULT ephemeral_revoke(void *ctx,const PAPACC_ISSUER_VIEW *view,const PAPACC_ISSUER_RECORD *revoked)
{
 PAPACC_ISSUER_RECORD original;unsigned i,mask=0;(void)ctx;
 if(view->revision!=fixture_view.revision||view->administrative_epoch!=fixture_view.administrative_epoch||
  papacc_issuer_image_credential(ephemeral_image,revoked->certificate_digest,&original,ephemeral_hash,NULL)!=0||
  original.state!=PAPACC_ISSUER_REVOKED||memcmp(original.request.id,revoked->request.id,16))return PAPACC_ISSUER_CONFLICT;
 for(i=0;i<2;i++)if(!memcmp(ephemeral_credentials[i],original.certificate_digest,32))mask|=1U<<i;
 memset(&ephemeral_receipt,0,sizeof(ephemeral_receipt));memcpy(ephemeral_receipt.credential,original.certificate_digest,32);
 return credential_stop_cohort(&ephemeral_manager,ephemeral_sessions,mask,&ephemeral_receipt)==PAPACC_RESULT_OK?PAPACC_ISSUER_OK:PAPACC_ISSUER_UNCERTAIN;
}
static PAPACC_ISSUER_RESULT ephemeral_copy_cert(void *ctx,const BYTE id[16],const BYTE digest[32],BYTE *out_der,PAPACC_SIZE cap,PAPACC_SIZE *used)
{BYTE d[32];PAPACC_ISSUER_RESULT r;(void)ctx;r=papacc_issuer_image_certificate(ephemeral_image,id,out_der,cap,used,ephemeral_hash,NULL);if(r)return r;return ephemeral_hash(NULL,out_der,*used,d)&&!memcmp(d,digest,32)?PAPACC_ISSUER_OK:PAPACC_ISSUER_CONFLICT;}
static PAPACC_ISSUER_RESULT ephemeral_confirm(void *ctx,const PAPACC_ISSUER_VIEW *before,const PAPACC_ISSUER_RECORD *next,const PAPACC_ISSUER_VIEW *after,const PAPACC_ISSUER_RECORD *actual)
{PAPACC_ISSUER_PUBLICATION_FACTS f;(void)ctx;
 if(papacc_issuer_image_publication(ephemeral_image,next->request.id,&f,ephemeral_hash,NULL))return PAPACC_ISSUER_CONFLICT;
 return after->epoch==before->epoch&&after->revision==f.revision+1&&after->administrative_epoch==f.administrative_epoch+1&&actual->published_epoch==f.administrative_epoch&&f.policy_epoch==after->epoch&&f.revision==f.issuer_revision?PAPACC_ISSUER_OK:PAPACC_ISSUER_CONFLICT;}
static PAPACC_ISSUER_RESULT ephemeral_cas(void *ctx,const PAPACC_ISSUER_VIEW *before,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const BYTE *der,PAPACC_SIZE bytes,int publish)
{
 PAPACC_ISSUER_VIEW decoded;PAPACC_ISSUER_RESULT r;BYTE correlation[32]={1};(void)ctx;(void)old;
 if(before->revision!=fixture_view.revision||before->high_water!=fixture_view.high_water)return PAPACC_ISSUER_CONFLICT;
 if(!publish)r=papacc_issuer_image_append(ephemeral_image,next,der,bytes,ephemeral_hash,NULL);
 else{
  r=papacc_issuer_image_intent(ephemeral_image,next,correlation,before->epoch,ephemeral_hash,NULL);if(r)return r;
  r=papacc_issuer_image_validate(ephemeral_image,&decoded,ephemeral_hash,NULL);if(r)return r;
  memset(&ephemeral_ack,0,sizeof(ephemeral_ack));ephemeral_ack.policy_epoch=before->epoch;
  ephemeral_ack.revision=ephemeral_ack.issuer_revision=decoded.revision;ephemeral_ack.administrative_epoch=before->administrative_epoch+1;
  /* Header event count is an explicit BE32 wire field, not a native struct. */
  ephemeral_ack.intent_ordinal=((PAPACC_U64)ephemeral_image[16]<<24)|((PAPACC_U64)ephemeral_image[17]<<16)|((PAPACC_U64)ephemeral_image[18]<<8)|(PAPACC_U64)ephemeral_image[19];
  memcpy(ephemeral_ack.intent_digest,ephemeral_image+224,32);memcpy(ephemeral_ack.correlation,correlation,32);
  r=papacc_issuer_image_ack(ephemeral_image,next,&ephemeral_ack,ephemeral_hash,NULL);if(r)printf("NATIVE_TEST ack=%u ordinal=%llu P=%llu approved=%llu rev=%llu\n",r,ephemeral_ack.intent_ordinal,ephemeral_ack.policy_epoch,next->approved_epoch,ephemeral_ack.revision);
 }
 if(r)return r;r=papacc_issuer_image_validate(ephemeral_image,&decoded,ephemeral_hash,NULL);if(r)return r;
 record=*next;if(publish)record.published_epoch=ephemeral_ack.administrative_epoch;
 fixture_view.revision=decoded.revision;fixture_view.high_water=high;fixture_view.administrative_epoch+=publish?2:1;return PAPACC_ISSUER_OK;
}
static int native_ephemeral_journey(void)
{
 DWORD n;PAPACC_ISSUER_VIEW metadata,after;PAPACC_ISSUER_RECORD prepared,actual;PAPACC_SIZE bytes;
 BYTE base[32]={1},digest[32];CRYPT_DATA_BLOB blob;unsigned before;CERT_PUBLIC_KEY_INFO saved_public;
 native_crypto=1;
 CHECK(NCryptOpenStorageProvider(&ephemeral_provider,MS_KEY_STORAGE_PROVIDER,0)==ERROR_SUCCESS);
 CHECK(ephemeral_new(&ephemeral_client)==ERROR_SUCCESS);
 CHECK(ephemeral_public(ephemeral_client,ephemeral_client_public.bytes,sizeof(ephemeral_client_public.bytes)));
 ca_public=*(CERT_PUBLIC_KEY_INFO *)ephemeral_client_public.bytes;
 CHECK(!source_setup((CERT_PUBLIC_KEY_INFO *)ephemeral_client_public.bytes));
 CHECK(fixture_ca_creates==1&&key.key==ephemeral_ca&&key.borrower==&fixture_backend);
 {BYTE tiny_buffer[16];CERT_PUBLIC_KEY_INFO *public_value=(CERT_PUBLIC_KEY_INFO *)(ULONG_PTR)1;
  CHECK(papacc_issuer_x509_key_public(ephemeral_ca,tiny_buffer,sizeof(tiny_buffer),&public_value)==PAPACC_ISSUER_LIMIT&&!public_value);
  CHECK(papacc_issuer_x509_key_public(ephemeral_ca,ephemeral_ca_public.bytes,sizeof(ephemeral_ca_public.bytes),(CERT_PUBLIC_KEY_INFO **)ephemeral_ca_public.bytes)==PAPACC_ISSUER_INVALID);
 }
 n=initial_ca_der_bytes;
 blob.pbData=initial_ca_der;blob.cbData=n;
 CHECK(CryptVerifyCertificateSignatureEx(0,X509_ASN_ENCODING,CRYPT_VERIFY_CERT_SIGN_SUBJECT_BLOB,&blob,CRYPT_VERIFY_CERT_SIGN_ISSUER_PUBKEY,&ca_public,0,NULL));
 CHECK(papacc_issuer_x509_ca_matches(&fixture_view,&fixture_ca,digest)==0);
 initial_ca_der[n-1]^=1;CHECK(!CryptVerifyCertificateSignatureEx(0,X509_ASN_ENCODING,CRYPT_VERIFY_CERT_SIGN_SUBJECT_BLOB,&blob,CRYPT_VERIFY_CERT_SIGN_ISSUER_PUBKEY,&ca_public,0,NULL));initial_ca_der[n-1]^=1;
 saved_public=fixture_ca.SubjectPublicKeyInfo;fixture_ca.SubjectPublicKeyInfo=*(CERT_PUBLIC_KEY_INFO *)ephemeral_client_public.bytes;
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)!=0&&!fixture_core.initialized);
 fixture_ca.SubjectPublicKeyInfo=saved_public;
 metadata=fixture_view;metadata.revision=1;metadata.high_water=1;metadata.epoch=metadata.administrative_epoch=metadata.proofs=0;
 CHECK(papacc_issuer_image_init_v2(ephemeral_image,&metadata,base,ephemeral_hash,NULL)==0);
 prepared=record;prepared.state=PAPACC_ISSUER_PREPARED;prepared.counter=0;memset(prepared.serial,0,16);
 CHECK(papacc_issuer_image_append(ephemeral_image,&prepared,NULL,0,ephemeral_hash,NULL)==0);
 CHECK(papacc_issuer_image_append(ephemeral_image,&record,NULL,0,ephemeral_hash,NULL)==0);
 CHECK(papacc_issuer_image_validate(ephemeral_image,&metadata,ephemeral_hash,NULL)==0);fixture_view.revision=metadata.revision;
 root.cas=ephemeral_cas;root.copy_certificate=ephemeral_copy_cert;root.confirm_publication=ephemeral_confirm;
 root.revoke_dependents=ephemeral_revoke;
 fixture_backend.root=root;
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==0);
 {PAPACC_ISSUER_RESULT results[2]={PAPACC_ISSUER_OK,PAPACC_ISSUER_OK};HANDLE thread=CreateThread(NULL,0,source_other_thread,results,0,NULL);
  CHECK(thread&&WaitForSingleObject(thread,10000)==WAIT_OBJECT_0&&CloseHandle(thread));CHECK(results[0]!=0&&results[1]!=0);}
 if(persistent_fixture)CHECK(!persistent_reacquire());
 before=native_signs;
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==0&&native_signs==before+1);
 CHECK(record.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED);
 CHECK(ephemeral_copy_cert(NULL,record.request.id,record.certificate_digest,ephemeral_copy,sizeof(ephemeral_copy),&bytes)==0);
 blob.pbData=ephemeral_copy;blob.cbData=(DWORD)bytes;
 CHECK(CryptVerifyCertificateSignatureEx(0,X509_ASN_ENCODING,CRYPT_VERIFY_CERT_SIGN_SUBJECT_BLOB,&blob,CRYPT_VERIFY_CERT_SIGN_ISSUER_PUBKEY,&ca_public,0,NULL));
 {PAPACC_ISSUER_RESULT r=papacc_issuer_transition(&fixture_core,record.request.id,PAPACC_ISSUER_PUBLISHED);if(r)printf("NATIVE_TEST publish=%u phase=%u rev=%llu A=%llu factrev=%llu factA=%llu\n",r,record.state,fixture_view.revision,fixture_view.administrative_epoch,ephemeral_ack.revision,ephemeral_ack.administrative_epoch);CHECK(r==0&&native_signs==before+1);}
 CHECK(papacc_issuer_query(&fixture_core,record.request.id,&actual)==0&&actual.published_epoch==ephemeral_ack.administrative_epoch);
 after=fixture_view;after.epoch++;CHECK(ephemeral_confirm(NULL,&fixture_view,&record,&after,&record)!=0);
 ephemeral_image[400]^=1;CHECK(papacc_issuer_image_validate(ephemeral_image,&metadata,ephemeral_hash,NULL)!=0);ephemeral_image[400]^=1;
 CHECK(papacc_issuer_image_ack(ephemeral_image,&record,&ephemeral_ack,ephemeral_hash,NULL)!=0); /* replay denied */
 /* Real effect-only cohort APIs; Session authentication remains a fixture.
  * Selection uses the actual issued DER digest, not a principal-wide alias. */
 CHECK(papacc_c2_operation_manager_init(&ephemeral_manager,GetCurrentThreadId(),ephemeral_deny_policy,ephemeral_deny_capacity,NULL)==0);
 {unsigned i;for(i=0;i<2;i++){
  CHECK(papacc_c2_session_init(&ephemeral_sessions[i],ephemeral_manager.owner,i+1,42,77,1,PAPACC_TRUE,PAPACC_TRUE,PAPACC_FALSE)==0);
  ephemeral_manager.sessions[i].session=&ephemeral_sessions[i];
  ephemeral_manager.operations[i].state=PAPACC_C2_OP_APPLICATION;ephemeral_manager.operations[i].session=&ephemeral_sessions[i];
  ephemeral_manager.operations[i].tokens.generated=PAPACC_TRUE;ephemeral_manager.operations[i].tokens.owner=ephemeral_manager.owner;ephemeral_manager.operations[i].tokens.borrowers=1;
 }}
 memcpy(ephemeral_credentials[0],record.certificate_digest,32);memcpy(ephemeral_credentials[1],record.certificate_digest,32);ephemeral_credentials[1][0]^=1;
 ephemeral_unrelated=ephemeral_sessions[1];
 CHECK(papacc_issuer_transition(&fixture_core,record.request.id,PAPACC_ISSUER_REVOKED)==0&&native_signs==before+1);
 CHECK(ephemeral_receipt.sessions_invalidated==1&&ephemeral_receipt.operations_stop_requested==1&&!ephemeral_receipt.close_proven&&!ephemeral_receipt.quiescence_proven);
 CHECK(ephemeral_manager.operations[0].tokens.borrowers==1&&!memcmp(&ephemeral_unrelated,&ephemeral_sessions[1],sizeof(ephemeral_unrelated)));
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))!=0&&native_signs==before+1);
 CHECK(papacc_issuer_finish(&fixture_core)==0&&papacc_issuer_cng_backend_release_custody(&fixture_backend)==0&&!key.borrower);
 CHECK(NCryptFreeObject(ephemeral_ca)==0);ephemeral_ca=0;key.key=0;
 if(ephemeral_client){CHECK(NCryptFreeObject(ephemeral_client)==0);ephemeral_client=0;}
 CHECK(NCryptFreeObject(ephemeral_provider)==0);ephemeral_provider=0;key.provider=0;
 if(persistent_fixture){CHECK(persistent_note("PASS REOPEN_AND_ISSUE; SIGNATURE_AND_PUBLISHED_V2_VERIFIED; NATIVE_REFS_CLOSED; KEY_RETAINED; NO_DELETE; NO_PRODUCT_AUTHORITY\r\n"));CHECK(CloseHandle(persistent_receipt));persistent_receipt=INVALID_HANDLE_VALUE;printf("PASS NATIVE_PERSISTENT_FIXTURE checks=%u CNG_CURRENTUSER_REAL REOPEN=PASS EXPORT_POLICY=ZERO KEY_RETAINED=TRUE PUBLICATION=SIMULATED PRODUCT_AUTHORITY=DENY\n",checks);return 0;}
 printf("PASS NATIVE_EPHEMERAL_COMPOSITION checks=%u CNG_RSA2048_REAL signatures=%u verifications=%u PUBLISHED_V2_SIMULATED PRIVATE_EXPORT=NONE PERSISTENT_KEYS=NONE PRODUCT_AUTHORITY=DENY\n",checks,native_signs,native_verifications);return 0;
}
