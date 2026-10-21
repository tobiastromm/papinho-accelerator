/* Real public RSA -> factual-source absence -> zero KDF/output regression.
 * Public upstream TEST key/signature only, no delegated trust is invented. */
{
 static unsigned char aq[4096],astage[4096],aout[4096];
 MP_AUTH_INPUT ai;MP_AUTH_OPERATION ao;
 memset(&ai,0,sizeof(ai));memset(&ao,0,sizeof(ao));memset(aout,0x5a,sizeof(aout));
 ai.crypto=c;ai.package.data=raw;ai.package.size=rn;ai.password.data=pw;ai.password.size=32;
 ai.quarantine=aq;ai.quarantine_capacity=sizeof(aq);ai.staging=astage;ai.staging_capacity=sizeof(astage);ai.output=aout;ai.output_capacity=sizeof(aout);
 /* Isolated test-only pricing, NOT runtime admission credit or host maxima. */
 ai.provider_reserved=1500000;ai.other_reserved=100000;
 CHECK(mp_crypto_verify_signature(c,raw,rn)==MP_CRYPTO_ONLY);
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED);
 CHECK(ao.flow.first_failure_stage==MP_AUTH_TRUST&&ao.flow.kdf_invocations==0&&ao.flow.aead_invocations==0&&ao.output_size==0);
 CHECK(ao.flow.first_failure_code==-102&&aout[0]==0x5a&&aout[sizeof(aout)-1]==0x5a&&aq[0]==0&&astage[0]==0);
 memset(&ao,0,sizeof(ao));raw[rn-1]^=1;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_stage==MP_AUTH_SIGNATURE&&ao.flow.kdf_invocations==0);
 raw[rn-1]^=1;memset(&ao,0,sizeof(ao));ai.provider_reserved=MP_AUTH_UNKNOWN_BYTES;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_LIMIT&&ao.flow.kdf_invocations==0&&aout[0]==0x5a);
 memset(&ao,0,sizeof(ao));ai.other_reserved=2103847;ai.provider_reserved=1290354;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_LIMIT&&ao.flow.first_failure_stage==MP_AUTH_BUDGET&&ao.flow.kdf_invocations==0&&aout[0]==0x5a);
 CHECK(peak<=1500000); /* actual provider test peak, not provider/kernel qualification */
 /* Valid public RSA and consumed MODEL PDG data must STILL fail factual TRUST.
  * No caller callback/row/checkpoint is a CURRENT pin or independent ceremony. */
 {
 MP_PDG_INPUT pdg;unsigned char spki_hash[32],*row;MP_ENVELOPE pdg_env;unsigned k;
 memset(&pdg,0,sizeof(pdg));CHECK(mp_envelope_inspect(raw,rn,&pdg_env,0)==MP_DATA_ONLY);
 CHECK(mp_crypto_public_sha256(c,pdg_env.spki.data,pdg_env.spki.size,spki_hash)==MP_CRYPTO_ONLY);
 CHECK(pdg_fixture_make(c,spki_hash,1));pdg.root.data=pdg_root;pdg.root.size=pdg_bytes;pdg.data_component=pdg_model_component;
 CHECK(mp_crypto_public_sha256(c,raw,rn,pdg.package_digest)==MP_CRYPTO_ONLY);
 ai.pdg=&pdg;ai.provider_reserved=1500000;ai.other_reserved=100000;memset(&ao,0,sizeof(ao));
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_stage==MP_AUTH_TRUST&&ao.flow.first_failure_code==-102);
 CHECK(ao.pdg_data.revision==2&&ao.pdg_data.row[96]==15&&ao.pdg_data.certificate_state==PAPACC_ISSUER_PUBLISHED);
 CHECK(!ao.pdg_data.administrative_epoch&&!ao.pdg_data.policy_epoch&&ao.flow.kdf_invocations==0&&ao.flow.aead_invocations==0&&ao.output_size==0&&aout[0]==0x5a);
 row=pdg_fields+pr32(pdg_fields+260)+64;
 pl16(row+16,2);CHECK(pdg_fixture_rehash(c));memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_code==-104&&ao.flow.kdf_invocations==0);
 for(k=0;k<3;k++){
 CHECK(pdg_fixture_make(c,spki_hash,1));row=pdg_fields+pr32(pdg_fields+260)+64;if(k<2)pl16(row+(k==0?18:16),k==0?2:99);if(k==2)row[80]^=1;
 CHECK(pdg_fixture_rehash(c));memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.kdf_invocations==0&&aout[0]==0x5a);
 }
 CHECK(pdg_fixture_make(c,spki_hash,0));pdg.root.size=pdg_bytes;memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_code==-102&&ao.flow.kdf_invocations==0);
 CHECK(pdg_fixture_make(c,spki_hash,1));pdg.root.size=pdg_bytes;pdg.package_digest[0]^=1;memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_code==-101&&ao.flow.kdf_invocations==0);pdg.package_digest[0]^=1;
 pdg.data_component=NULL;memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.flow.first_failure_code==-102&&ao.flow.kdf_invocations==0);pdg.data_component=pdg_model_component;
 pdg_stop_after_data=1;ai.stopped=pdg_stop;memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_STOPPED&&ao.flow.kdf_invocations==0&&aout[0]==0x5a);ai.stopped=NULL;pdg_stop_after_data=0;
 pdg.root.data=aq;memset(&ao,0,sizeof(ao));CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_INVALID&&ao.flow.kdf_invocations==0);pdg.root.data=pdg_root;
 memset(&ao,0,sizeof(ao));ai.other_reserved=mp_package_validation_process_limit();CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_LIMIT&&ao.flow.kdf_invocations==0);
 ai.pdg=NULL;
 printf("PDG_M1_NATIVE_DENY_WITH_REAL_RSA_AND_SYNTHETIC_SOURCE no_CURRENT_no_pin_no_independent_confirmation op=%zu input=%zu pdg=%zu selection=%zu\n",sizeof(ao),sizeof(ai),sizeof(pdg),sizeof(ao.pdg_data));
 }
}
