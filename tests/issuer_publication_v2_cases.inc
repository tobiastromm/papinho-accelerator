/* Real SHA256, synthetic DATA only, no signer or root authority. */
static int publication_v2_cases(void){
 PAPACC_ISSUER_PUBLICATION_FACTS f,read;PAPACC_U8 correlation[32]={1},out[3];PAPACC_SIZE n;unsigned i;
 metadata_init();memset(hash,1,32);CHECK(papacc_issuer_image_init_v2(image,&metadata,hash,papacc_configuration_issuer_hash06,&root)==0);
 request_init(1);CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=2;memcpy(record.serial,metadata.prefix,8);record.serial[15]=2;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;record.certificate_size=3;memcpy(record.certificate_digest,cert_hash,32);
 CHECK(papacc_issuer_image_append(image,&record,(const PAPACC_U8 *)"DER",3,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_PUBLISHED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT);
 CHECK(papacc_issuer_image_publication(image,record.request.id,&read,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_ABSENT);
 CHECK(papacc_issuer_image_intent(image,&record,correlation,8,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT);
 CHECK(papacc_issuer_image_intent(image,&record,correlation,9,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_find(image,record.request.id,&found,papacc_configuration_issuer_hash06,&root)==0&&found.state==PAPACC_ISSUER_PUBLICATION_INTENT&&!found.published_epoch);
 CHECK(papacc_issuer_image_intent(image,&record,correlation,9,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT);
 memset(&f,0,sizeof(f));f.policy_epoch=9;f.revision=f.issuer_revision=7;f.administrative_epoch=101;f.intent_ordinal=5;memcpy(f.intent_digest,image+224,32);memcpy(f.correlation,correlation,32);
 memcpy(saved,image,8192);
 for(i=0;i<7;i++){
  PAPACC_ISSUER_PUBLICATION_FACTS bad=f;
  if(i==0)bad.policy_epoch++;if(i==1)bad.revision++;if(i==2)bad.issuer_revision++;if(i==3)bad.administrative_epoch=0;if(i==4)bad.intent_ordinal++;if(i==5)bad.intent_digest[0]^=1;if(i==6)bad.correlation[0]^=1;
  CHECK(papacc_issuer_image_ack(image,&record,&bad,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT&&!memcmp(image,saved,8192));
 }
 CHECK(papacc_issuer_image_ack(image,&record,&f,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_ack(image,&record,&f,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT);
 CHECK(papacc_issuer_image_publication(image,record.request.id,&read,papacc_configuration_issuer_hash06,&root)==0&&read.administrative_epoch==101&&read.revision==7);
 CHECK(papacc_issuer_image_certificate(image,record.request.id,out,sizeof(out),&n,papacc_configuration_issuer_hash06,&root)==0&&n==3&&!memcmp(out,"DER",3));
 CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)==0&&!decoded.administrative_epoch&&!decoded.epoch&&!decoded.proofs&&decoded.high_water==2);
 record.published_epoch=101;record.state=PAPACC_ISSUER_REVOKED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_publication(image,record.request.id,&read,papacc_configuration_issuer_hash06,&root)==0&&read.administrative_epoch==101);
 memcpy(saved,image,8192);for(i=0;i<8192;i+=257){image[i]^=1;CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)!=0);memcpy(image,saved,8192);}
 image[7]='1';CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_INVALID);memcpy(image,saved,8192);
 return 0;
}
