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
 CHECK(ao.first_failure_stage==MP_AUTH_TRUST&&ao.kdf_invocations==0&&ao.aead_invocations==0&&ao.output_size==0);
 CHECK(ao.first_failure_code==-102&&aout[0]==0x5a&&aout[sizeof(aout)-1]==0x5a&&aq[0]==0&&astage[0]==0);
 memset(&ao,0,sizeof(ao));raw[rn-1]^=1;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_DENIED&&ao.first_failure_stage==MP_AUTH_SIGNATURE&&ao.kdf_invocations==0);
 raw[rn-1]^=1;memset(&ao,0,sizeof(ao));ai.provider_reserved=MP_AUTH_UNKNOWN_BYTES;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_LIMIT&&ao.kdf_invocations==0&&aout[0]==0x5a);
 memset(&ao,0,sizeof(ao));ai.other_reserved=2103847;ai.provider_reserved=1290354;
 CHECK(mp_package_validate_authenticated(&ao,&ai)==MP_AUTH_LIMIT&&ao.first_failure_stage==MP_AUTH_BUDGET&&ao.kdf_invocations==0&&aout[0]==0x5a);
 CHECK(peak<=1500000); /* actual provider test peak, not provider/kernel qualification */
}
