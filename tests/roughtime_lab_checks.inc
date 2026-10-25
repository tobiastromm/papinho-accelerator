/* LAB_ONLY integration checks. No key generation/signing or trust source. */
static PAPACC_U32 lab_u32(const PAPACC_U8 *p){return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U8 *lab_field(PAPACC_U8 *map,PAPACC_SIZE size,const char *tag,PAPACC_SIZE *bytes){
 PAPACC_U32 count;PAPACC_SIZE i,a,b;
 if(size<8)return NULL;count=lab_u32(map);if(!count||count>32||size<8U*count)return NULL;
 for(i=0;i<count;i++)if(!memcmp(map+4U*count+4U*i,tag,4)){
  a=i?lab_u32(map+4U*i):0;b=i+1<count?lab_u32(map+4U*(i+1)):size-8U*count;
  if(a>b||b>size-8U*count)return NULL;*bytes=b-a;return map+8U*count+a;
 }return NULL;
}
static void lab_bad_reply(const PAPACC_RT_CRYPTO *crypto,PAPACC_RT_BYTES request,PAPACC_RT_BYTES response,const PAPACC_U8 *key){
 PAPACC_RT_REPLY out,old;memset(&out,0x55,sizeof(out));old=out;
 CK(papacc_rt_reply_data(crypto,request,response,key,&out)==PAPACC_RT_INVALID);CK(!memcmp(&out,&old,sizeof(out)));
}
static void lab_mutation(const PAPACC_RT_CRYPTO *crypto,PAPACC_RT_BYTES req,PAPACC_RT_BYTES resp,const PAPACC_U8 *key,PAPACC_U8 *byte){
 PAPACC_U8 save;CK(byte!=NULL);if(!byte)return;save=*byte;*byte^=1;lab_bad_reply(crypto,req,resp,key);*byte=save;
}
static void lab_checks(RT_TEST_GRAPH *g,const PAPACC_RT_CRYPTO *crypto){
 PAPACC_RT_BYTES req[6],resp[6];PAPACC_RT_REPLY out[6],saved[6];PAPACC_UTC_OBSERVATION q[6];
 PAPACC_RT_EXCHANGE event,retained;PAPACC_U64 l=0,u=0;PAPACC_SIZE n,cn,sn,i,j;PAPACC_U8 *p,*cert,*srep,*dele,*v,save;
 for(i=0;i<6;i++){req[i].p=g->requests[i];req[i].n=2048;resp[i].p=g->responses[i];resp[i].n=lab_response_bytes[i];}
 if(papacc_rt_campaign_data(crypto,req,resp,g->sources,&g->randoms[0][0],out)!=PAPACC_RT_DATA_OK){CK(0);return;}CK(1);
 for(i=0;i<6;i++){
  CK(out[i].midpoint==(PAPACC_U64)2000000000UL&&out[i].radius==3);
  CK(papacc_rt_reply_data(crypto,req[i],resp[i],g->sources+32*(i%3),&out[i])==PAPACC_RT_DATA_OK);
  memset(&q[i],0,sizeof(q[i]));q[i].version=1;q[i].present=PAPACC_UTC_DATA_PRESENT;q[i].rho_den=1000000;q[i].rho_num=10;q[i].pal_error=100;
  q[i].m0=(PAPACC_U64)i*1000000U;q[i].m1=q[i].m0+1000U;q[i].deadline=1000000000UL;
  memset(&event,0,sizeof(event));CK(papacc_rt_exchange_start_data(&event,0,q[i].m0)==PAPACC_RT_DATA_OK);
  CK(papacc_rt_exchange_sent_data(&event,q[i].m0+100)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_received_data(&event,q[i].m0+200)==PAPACC_RT_DATA_OK);
  CK(papacc_rt_exchange_verified_data(&event,q[i].m0+300)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_received_data(&event,q[i].m0+400)==PAPACC_RT_INVALID);
 }
 CK(papacc_rt_hull_data(out,q,10000000UL,&l,&u)==PAPACC_RT_DATA_OK);CK(l==(PAPACC_U64)1999999997UL*1000000000UL);CK(u>(PAPACC_U64)2000000003UL*1000000000UL&&u-l<=30U*(PAPACC_U64)1000000000UL);
 /* Every mutation begins with a cryptographically valid complete baseline. */
 memcpy(saved,out,sizeof(out));save=g->randoms[1][0];g->randoms[1][0]^=1;memset(out,0x55,sizeof(out));memcpy(saved,out,sizeof(out));
 CK(papacc_rt_campaign_data(crypto,req,resp,g->sources,&g->randoms[0][0],out)==PAPACC_RT_INVALID);CK(!memcmp(saved,out,sizeof(out)));g->randoms[1][0]=save;
 p=g->responses[0]+12;n=resp[0].n-12;
 v=lab_field(p,n,"NONC",&cn);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 v=lab_field(p,n,"SIG\0",&cn);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 cert=lab_field(p,n,"CERT",&cn);CK(cert!=NULL);if(!cert)return;
 v=lab_field(cert,cn,"SIG\0",&sn);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 dele=lab_field(cert,cn,"DELE",&sn);CK(dele!=NULL);if(!dele)return;
 v=lab_field(dele,sn,"PUBK",&j);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 srep=lab_field(p,n,"SREP",&sn);CK(srep!=NULL);if(!srep)return;
 v=lab_field(srep,sn,"VER\0",&j);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 v=lab_field(p,n,"PATH",&cn);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 v=lab_field(p,n,"INDX",&cn);lab_mutation(crypto,req[0],resp[0],g->sources,v);
 save=g->sources[0];g->sources[0]^=1;lab_bad_reply(crypto,req[0],resp[0],g->sources);g->sources[0]=save;
 for(i=0;i<6;i++)for(j=0;j<lab_response_bytes[i];j++){resp[i].n=j;lab_bad_reply(crypto,req[i],resp[i],g->sources+32*(i%3));}
 for(i=0;i<6;i++)resp[i].n=lab_response_bytes[i];
 resp[0].n=2049;lab_bad_reply(crypto,req[0],resp[0],g->sources);resp[0].n=lab_response_bytes[0];req[0].n=2047;lab_bad_reply(crypto,req[0],resp[0],g->sources);req[0].n=2048;
 memset(&event,0,sizeof(event));CK(papacc_rt_exchange_start_data(&event,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&event,1)==PAPACC_RT_DATA_OK);retained=event;
 papacc_rt_exchange_stop_data(&event);CK(event.phase==retained.phase&&event.sent==retained.sent);CK(papacc_rt_exchange_received_data(&event,2)==PAPACC_RT_RETAINED);
 memset(&event,0,sizeof(event));CK(papacc_rt_exchange_start_data(&event,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&event,1000000000UL)==PAPACC_RT_EXPIRED);
 CK(papacc_issuer_qualified_utc_read(NULL,&q[0])==PAPACC_RESULT_NOT_SUPPORTED);
 CK(papacc_rt_campaign_data(crypto,req,resp,g->sources,&g->randoms[0][0],out)==PAPACC_RT_DATA_OK);
 printf("ROUGHTIME_TRANSCRIPT=LAB_ONLY RFC10049_CONFORMANCE=OFFLINE_SCOPE UTC_SOURCE=NOT_QUALIFIED complete_pairs=6\n");
}
