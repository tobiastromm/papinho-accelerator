/* Native public DER/hash and actual canonical ticket code; publication below
 * is an explicit in-memory MODEL, never factual root CURRENT or authority. */
#define _WIN32_WINNT 0x0600
#include "config_root8_v3_test.c"
#include <wincrypt.h>
static PAPACC_U8 ca_der[PAPACC_CONFIG_V3_CA_BYTES],text_ca[16384],spki[2048];
static unsigned __int64 arena[4096];
static int load_ca(const char *path,PAPACC_SIZE *bytes,PAPACC_U8 digest_key[32])
{
 FILE *f;size_t n;DWORD dn=PAPACC_CONFIG_V3_CA_BYTES,kn=sizeof(spki);PCCERT_CONTEXT cert;
 CHECK(fopen_s(&f,path,"rb")==0&&f!=NULL);n=fread(text_ca,1,sizeof(text_ca),f);CHECK(!ferror(f)&&fclose(f)==0&&n&&n<sizeof(text_ca));
 CHECK(CryptStringToBinaryA((char *)text_ca,(DWORD)n,CRYPT_STRING_BASE64,ca_der,&dn,NULL,NULL));
 cert=CertCreateCertificateContext(X509_ASN_ENCODING,ca_der,dn);CHECK(cert!=NULL);
 CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_PUBLIC_KEY_INFO,&cert->pCertInfo->SubjectPublicKeyInfo,0,NULL,spki,&kn));
 CHECK(hash(&owner,spki,kn,digest_key));CHECK(CertFreeCertificateContext(cert));*bytes=dn;return 1;
}
static int bind_ca_data(const PAPACC_U8 key[32])
{
 PAPACC_U8 *image=f3+r32(f3+48)+8192,*sn=f3+r32(f3+244)+64,*pg=f3+r32(f3+260)+64;unsigned j;
 memcpy(image+136,key,32);CHECK(hash(&owner,image,8160,image+8160));
 for(j=0;j<(unsigned)f3[252];j++)memcpy(sn+768U*j+568,key,32);
 for(j=0;j<(unsigned)f3[268];j++)memcpy(pg+384U*j+260,key,32);
 return seal3();
}
static int backing_cases(const char *ca_path)
{
 PAPACC_CONFIG_PIN pin={0},stale;PAPACC_CONFIG_V3_BORROW borrow,unchanged,second;
 PAPACC_ISSUER_WINDOWS_WORKSPACE workspace;PAPACC_U8 key[32];PAPACC_SIZE cn;PAPACC_U64 serial;
 char path[MAX_PATH];DWORD len;PAPACC_CONFIG_V3_BACKING *slot_data;
 len=GetFullPathNameA("v3-backing-model-NO-STORE",MAX_PATH,path,NULL);CHECK(len&&len<MAX_PATH);
 CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 CHECK(load_ca(ca_path,&cn,key));CHECK(setup3(1,1,1));CHECK(bind_ca_data(key));
 memset(&workspace,0,sizeof(workspace));workspace.bytes=arena;workspace.capacity=sizeof(arena);
 /* MODEL prepared slot only. No call to commit/publish or root projection. */
 config_init(0,2);owner.slots[1].view=view;owner.slots[1].serial=++owner.next_serial;
 owner.pending=1;owner.prepared=1;memcpy(owner.domain,f3+72,32);slot_data=&owner.v3_backings[1];
 memset(&borrow,0xa5,sizeof(borrow));unchanged=borrow;
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)!=PAPACC_RESULT_OK&&!memcmp(&borrow,&unchanged,sizeof(borrow)));
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,NULL,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK&&!slot_data->serial);
 workspace.stopped=1;CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK);workspace.stopped=0;
 owner.slots[1].view.revision++;
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK&&!slot_data->serial);owner.slots[1].view.revision--;
 e3[600]^=1;CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK);e3[600]^=1;
 key[0]^=1;CHECK(bind_ca_data(key));CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK&&!slot_data->serial);key[0]^=1;CHECK(bind_ca_data(key));
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)==PAPACC_RESULT_OK);
 CHECK(slot_data->serial==owner.slots[1].serial&&!memcmp(slot_data->packet,e3,z3)&&!memcmp(slot_data->ca,ca_der,cn));
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK);
 /* Diagnostic publication bit grants a real storage ticket only. Root CURRENT
  * remains absent: no normative commit, checkpoint/publication or E0 source. */
 owner.prepared=0;owner.published=1;owner.current=1;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)==PAPACC_RESULT_OK);
 CHECK(borrow.packet==slot_data->packet&&borrow.ca==slot_data->ca&&borrow.revision==2&&borrow.issuer_revision==r64(e3+120));
 CHECK(borrow.snt==borrow.packet+r32(e3+244)&&borrow.pdg==borrow.packet+r32(e3+260));
 CHECK(borrow.issuer_image==borrow.packet+r32(e3+48)+8192);
 e3[600]^=1;ca_der[10]^=1;CHECK(borrow.packet[600]!=e3[600]&&borrow.ca[10]!=ca_der[10]);e3[600]^=1;ca_der[10]^=1;
 memset(arena,0,sizeof(arena));CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)==PAPACC_RESULT_OK&&!memcmp(second.ca,ca_der,cn));
 stale=pin;stale.ticket++;CHECK(papacc_configuration_v3_borrow_data(&owner,&stale,&second)!=PAPACC_RESULT_OK);
 slot_data->packet[600]^=1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);slot_data->packet[600]^=1;
 slot_data->ca[10]^=1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);slot_data->ca[10]^=1;
 owner.slots[1].issuer_revision=borrow.issuer_revision+1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);owner.slots[1].issuer_revision=0;
 owner.issuer_snapshot_valid[1]=1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);
 memcpy(owner.issuer_snapshots[1],borrow.issuer_image,8192);CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)==PAPACC_RESULT_OK);
 owner.prepared=1;CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=PAPACC_RESULT_OK);owner.prepared=0;
 owner.authority_lost=1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);CHECK(!memcmp(borrow.ca,ca_der,cn));owner.authority_lost=0;
 owner.unresolved=1;CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);CHECK(!memcmp(borrow.packet,e3,z3));owner.unresolved=0;
 /* End ALL borrows before factual unpin. A stale ticket cannot be resurrected. */
 stale=pin;CHECK(papacc_configuration_unpin(&owner,&pin,0)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&stale,&second)!=PAPACC_RESULT_OK);
 owner.slots[1].serial=++owner.next_serial;serial=owner.slots[1].serial;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK); /* old backing serial */
 CHECK(papacc_configuration_unpin(&owner,&pin,0)==PAPACC_RESULT_OK);
 /* Free non-current slot may be reused only under a NEW generation. */
 owner.current=0;owner.pending=1;owner.prepared=1;
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)==PAPACC_RESULT_OK&&slot_data->serial==serial);
 owner.prepared=0;owner.current=1;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&second)!=PAPACC_RESULT_OK);
 CHECK(!memcmp(borrow.ca,ca_der,cn)&&!memcmp(borrow.packet,e3,z3));
 CHECK(papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK); /* live pin */
 CHECK(papacc_configuration_unpin(&owner,&pin,0)==PAPACC_RESULT_OK); /* children quiescent */
 CHECK(papacc_configuration_finish(&owner)==PAPACC_RESULT_OK);
 /* Separate diagnostic cycle: retained disposition must NOT be cleared by
  * finish/timeout. This test intentionally leaves only owner memory retained. */
 memset(&owner,0,sizeof(owner));CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 CHECK(setup3(4,4,4));CHECK(bind_ca_data(key));
 w16(f3+r32(f3+260)+64+16,2);CHECK(seal3()); /* REVOKED is DATA, never trust */
 CHECK(z3==PC_ROOT8_V3_MAX);
 config_init(0,2);owner.slots[1].view=view;owner.slots[1].serial=++owner.next_serial;
 owner.pending=1;owner.prepared=1;memcpy(owner.domain,e3+72,32);
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)==PAPACC_RESULT_OK);
 owner.prepared=0;owner.published=1;owner.current=1;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)==PAPACC_RESULT_OK&&borrow.packet_bytes==PC_ROOT8_V3_MAX&&borrow.pdg[64+16]==2);
 CHECK(papacc_configuration_pin(&owner,&stale,&view)==PAPACC_RESULT_OK&&owner.slots[1].readers==2);
 CHECK(papacc_configuration_unpin(&owner,&stale,0)==PAPACC_RESULT_OK&&owner.slots[1].readers==1);
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_unpin(&owner,&pin,1)==PAPACC_RESULT_OK);
 CHECK(owner.slots[1].retained&&papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);
 CHECK(!memcmp(owner.v3_backings[1].packet,e3,z3));
 printf("PASS %u V3_BACKING_DATA_MODEL_NATIVE_PUBLIC_CA_HASH_PIN_LIFETIME NO_CURRENT_NO_TRUST retained_not_clean=1 slot=%Iu all=%Iu caller_arena=%Iu\n",checks,sizeof(PAPACC_CONFIG_V3_BACKING),sizeof(owner.v3_backings),sizeof(arena));return 1;
}
#ifdef PAPACC_KIND8_DISPATCH_FIXTURE
int backing_fixture_entry_not_run(int argc,char **argv){return argc==2&&backing_cases(argv[1])?0:1;}
#else
int main(int argc,char **argv){return argc==2&&backing_cases(argv[1])?0:1;}
#endif
