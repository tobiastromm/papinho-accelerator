/* Two independent MODEL journeys; no native persisted key or signature.
 * Original snapshot belongs to this fixture, not a product root/restore grant. */
static int existing_fixture(const CERT_PUBLIC_KEY_INFO *client,int present)
{
 PAPACC_ISSUER_VIEW original;PAPACC_ISSUER_RECORD old;CERT_INFO original_ca;unsigned creates;
 CHECK(!source_setup(client));original=fixture_view;old=record;original_ca=fixture_ca;creates=fixture_ca_creates;
 CHECK(fixture_backend.source_kind==PAPACC_CA_SOURCE_NEW_INSTALL&&creates==1U);
 papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0);
 /* A fresh process/backend has no old live confirmation or attempt. */
 normal_profile=1;defer_core_init=1;CHECK(!initialize(client));defer_core_init=0;
 fixture_view=original;record=old;fixture_ca=original_ca;
 memset(&source_owner,0,sizeof(source_owner));source_owner.initialized=source_owner.published=1;
 source_owner.writer=GetCurrentThreadId();source_owner.store02=6;memcpy(source_owner.domain,original.root,32);
 source_owner.slots[0].view.revision=original.revision;source_owner.slots[0].issuer_revision=original.revision;
 source_owner.slots[0].context_epoch=original.administrative_epoch;source_owner.slots[0].operational_epoch=original.epoch;
 key.state=PAPACC_CA_KEY_EMPTY;key.key=key.provider=0;key.created=0;fixture_ca_opens=0;
 if(!present)return 0; /* absence is NOT permission to create */
 fixture_ca_opens++;key.state=PAPACC_CA_KEY_READY;key.key=101;key.provider=202;
 CHECK(fixture_ca_creates==creates&&fixture_ca_opens==1U&&!key.created&&!key.borrower);
 return 0;
}
static int first_existing_ca_cases(const CERT_PUBLIC_KEY_INFO *client)
{
 PAPACC_ISSUER_VIEW genesis;BYTE public_tbs[1024],kid[32],empty[32]={0};DWORD size;unsigned i;
 CHECK(!source_setup(client));CHECK(fixture_ca_creates==1U&&fixture_ca_opens==0U);
 CHECK(fixture_backend.source_kind==PAPACC_CA_SOURCE_NEW_INSTALL);
 genesis=fixture_view;genesis.revision=genesis.epoch=genesis.administrative_epoch=0;genesis.proofs=0;genesis.high_water=1;
 CHECK(papacc_issuer_x509_ca_tbs(&genesis,1690000000ULL,&ca_public,public_tbs,sizeof(public_tbs),&size)==0&&size);
 CHECK(papacc_issuer_x509_ca_matches(&genesis,&fixture_ca,kid)==0);
 genesis.high_water=2;CHECK(papacc_issuer_x509_ca_tbs(&genesis,1690000000ULL,&ca_public,public_tbs,sizeof(public_tbs),&size)!=0&&!size);
 genesis.high_water=1;genesis.ca_not_after++;CHECK(papacc_issuer_x509_ca_tbs(&genesis,1690000000ULL,&ca_public,public_tbs,sizeof(public_tbs),&size)!=0);
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_DENIED);
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==0);
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==0&&private_calls==1);
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==0);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0);

 CHECK(!existing_fixture(client,0));CHECK(!key.key&&!key.created);
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_DENIED&&fixture_ca_opens==0U);
 CHECK(!existing_fixture(client,1));CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==0);
 CHECK(fixture_backend.custody&&fixture_backend.source_kind==PAPACC_CA_SOURCE_EXISTING_CURRENT&&!memcmp(fixture_backend.original_attempt,empty,32));
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==PAPACC_ISSUER_DENIED); /* no reused NEW_INSTALL */
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_DENIED);
 CHECK(papacc_configuration_existing_ca_activate_normal(&source_owner,&fixture_backend)==0);
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==0&&private_calls==1&&fixture_ca_opens==1U&&fixture_ca_creates==1U);
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==0);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0);
 for(i=0;i<12U;i++){
  CHECK(!existing_fixture(client,1));
  if(i==0U)source_owner.published=0;if(i==1U)key.created=1;if(i==2U)fixture_ca.SubjectPublicKeyInfo=*client;
  if(i==3U)source_owner.recovery_selected=1;if(i==4U)fixture_view.proofs=0;if(i==5U)fixture_view.generation[0]^=1U;
  if(i==6U)source_owner.slots[0].context_epoch++;if(i==7U)fixture_backend.key=NULL;if(i==8U)key.borrower=&fixture_core;
  if(i==9U)source_owner.pending_commit.personal_data.phase=PAPACC_PERSONAL_DATA_CONSUMED;
  if(i==10U)source_owner.pending_commit.active=1;if(i==11U)source_owner.stopped=1;
  CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)!=0&&!fixture_backend.custody&&!fixture_core.initialized&&!private_calls&&fixture_ca_opens==1U&&fixture_ca_creates==1U);
 }
 CHECK(!existing_fixture(client,1));source_reads=0;source_fault_at=2;source_fault=1;
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_CONFLICT&&!key.borrower);
 CHECK(!existing_fixture(client,1));source_reads=0;source_fault_at=3;source_fault=4;
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_UNCERTAIN&&key.borrower==&fixture_backend&&fixture_backend.retained&&source_owner.unresolved&&!private_calls);
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==PAPACC_ISSUER_DENIED&&key.borrower==&fixture_backend);
 CHECK(!existing_fixture(client,1));source_owner.pending_commit.personal_data.phase=PAPACC_PERSONAL_DATA_FINISHED;
 CHECK(papacc_configuration_existing_ca_bind(&source_owner,&fixture_backend)==0); /* retired DATA is not live authority */
 source_owner.slots[0].operational_epoch++;
 CHECK(papacc_configuration_existing_ca_activate_normal(&source_owner,&fixture_backend)==PAPACC_ISSUER_CONFLICT&&!fixture_core.initialized&&key.borrower==&fixture_backend);
 puts("PASS FIRST_INSTANCE_AND_EXISTING_CA_MODEL NO_PREVIOUS_CA_REQUIRED NO_RECREATION_ON_RESTART NO_NATIVE_KEY_CREATION");return 0;
}
