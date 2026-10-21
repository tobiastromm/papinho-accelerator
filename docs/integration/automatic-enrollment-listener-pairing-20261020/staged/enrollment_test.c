/* DATA and synthetic control tests; no CURRENT/policy/approval authority. */
#include "enrollment.h"
#include "../src/core/ed11_process_profile.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define CHECK(x) do {checks++;if(!(x)){printf("FAIL line=%u checks=%u\n",(unsigned)__LINE__,checks);return 1;}} while(0)
static PAPACC_U8 body[16384],wire[16400],rx[16400];
static void be16(PAPACC_U8 *p,unsigned n){p[0]=(PAPACC_U8)(n>>8);p[1]=(PAPACC_U8)n;}
static void be32(PAPACC_U8 *p,unsigned n){p[0]=(PAPACC_U8)(n>>24);p[1]=(PAPACC_U8)(n>>16);p[2]=(PAPACC_U8)(n>>8);p[3]=(PAPACC_U8)n;}
static PAPACC_SIZE make(unsigned k){
 memset(body,0,sizeof(body));
 if(k==1){memset(body,1,32);memset(body+32,2,32);be16(body+64,1);body[66]='a';return 67;}
 if(k==2){memset(body,3,16);memset(body+16,1,32);memset(body+48,4,32);memset(body+80,2,32);be32(body+116,300000);return 120;}
 memset(body,3,16);if(k==3||k==5||k==7)memset(body+16,4,32);
 if(k==3){memcpy(body+48,"0000000000",10);return 62;}
 if(k==4){body[16]=1;return 20;}
 if(k==5){be32(body+48,1);body[52]=0x30;return 53;}
 if(k==6){be32(body+16,1);be32(body+20,1);body[24]=body[25]=0x30;return 26;}
 if(k==7)return 48;be16(body+16,1);return 20;
}
static int framing(void){
 EP_FRAME f;EP_READER reader;PAPACC_SIZE n,used,i,taken,offset,step;unsigned k,d;
 for(k=1;k<=8;k++){
 n=make(k);d=(k==1||k==3||k==5||k==7)?EP_C2S:EP_S2C;used=0;
 CHECK(ep_frame_encode(k,1,d,body,n,wire,sizeof(wire),&used)==EP_DATA_ONLY&&used==16U+n);
 CHECK(ep_frame_decode(wire,used,d,&f)==EP_DATA_ONLY&&f.kind==k&&f.sequence==1&&f.payload.size==n);
 for(i=1;i<used;i++)CHECK(ep_frame_decode(wire,i,d,&f)==EP_MORE);
 CHECK(ep_frame_decode(wire,used,1U-d,&f)==EP_INVALID);
 wire[used]=0;CHECK(ep_frame_decode(wire,used+1,d,&f)==EP_INVALID);
 for(i=4;i<8;i++){PAPACC_U8 saved=wire[i];wire[i]=255;CHECK(ep_frame_decode(wire,used,d,&f)==EP_INVALID);wire[i]=saved;}
 be32(wire+8,16385);CHECK(ep_frame_decode(wire,used,d,&f)==EP_INVALID);be32(wire+8,(unsigned)n);
 be32(wire+12,9);CHECK(ep_frame_decode(wire,used,d,&f)==EP_INVALID);be32(wire+12,1);
 CHECK(ep_frame_encode(k,1,d,body,n,body,sizeof(body),&used)==EP_INVALID);
 }
 n=make(1);CHECK(ep_frame_encode(1,1,EP_C2S,body,n,wire,sizeof(wire),&used)==EP_DATA_ONLY);
 for(step=1;step<=17;step++){
 memset(&reader,0,sizeof(reader));reader.rx=rx;reader.capacity=sizeof(rx);offset=0;
 while(offset<used){i=used-offset;if(i>step)i=step;taken=0;CHECK(ep_reader_push(&reader,wire+offset,i,100+offset,EP_C2S,&taken,&f)==(offset+i==used?EP_DATA_ONLY:EP_MORE));CHECK(taken==i);offset+=taken;}
 CHECK(reader.ready&&f.kind==1&&f.payload.size==67);
 CHECK(ep_reader_push(&reader,wire,1,200,EP_C2S,&taken,&f)==EP_DENIED);
 }
 memset(&reader,0,sizeof(reader));reader.rx=rx;reader.capacity=sizeof(rx);CHECK(ep_reader_push(&reader,wire,1,100,EP_C2S,&taken,&f)==EP_MORE);
 CHECK(ep_reader_push(&reader,NULL,0,5100,EP_C2S,&taken,&f)==EP_EXPIRED&&reader.have==1);
 memset(&reader,0,sizeof(reader));reader.rx=rx;reader.capacity=sizeof(rx);reader.stopped=1;CHECK(ep_reader_push(&reader,wire,1,0,EP_C2S,&taken,&f)==EP_STOPPED&&reader.have==0);
 n=make(3);be16(body+58,1);be16(body+60,1);body[62]='u';body[63]='p';CHECK(ep_frame_encode(3,1,EP_C2S,body,64,wire,sizeof(wire),&used)==EP_DATA_ONLY);
 body[62]=0xc0;CHECK(ep_frame_encode(3,1,EP_C2S,body,64,wire,sizeof(wire),&used)==EP_INVALID);body[62]=0;CHECK(ep_frame_encode(3,1,EP_C2S,body,64,wire,sizeof(wire),&used)==EP_INVALID);
 (void)make(5);be32(body+48,4096);memset(body+52,0x30,4096);CHECK(ep_frame_encode(5,8,EP_C2S,body,4148,wire,sizeof(wire),&used)==EP_DATA_ONLY);
 (void)make(6);be32(body+16,4096);be32(body+20,4096);CHECK(ep_frame_encode(6,8,EP_S2C,body,8216,wire,sizeof(wire),&used)==EP_DATA_ONLY);
 CHECK(ep_frame_decode(wire,used,EP_S2C,&f)==EP_DATA_ONLY);CHECK(ep_frame_encode(6,8,EP_S2C,body,8217,wire,sizeof(wire),&used)==EP_INVALID);
 return 0;
}
static int trace(void){
 EP_TRACE t,saved;EP_FRAME f;PAPACC_SIZE n;unsigned mode,approval;
 for(mode=0;mode<2;mode++)for(approval=0;approval<2;approval++){
 memset(&t,0,sizeof(t));f.kind=1;f.sequence=1;f.payload.data=body;f.payload.size=make(1);CHECK(ep_trace_observe(&t,&f,EP_C2S)==EP_DATA_ONLY);
 f.kind=2;f.payload.size=make(2);body[112]=(PAPACC_U8)mode;body[113]=(PAPACC_U8)approval;CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DATA_ONLY);
 f.kind=3;f.sequence=2;n=make(3);if(mode){be16(body+58,1);be16(body+60,1);body[62]='u';body[63]='p';n=64;}f.payload.size=n;CHECK(ep_trace_observe(&t,&f,EP_C2S)==EP_DATA_ONLY);
 saved=t;CHECK(ep_trace_observe(&saved,&f,EP_C2S)==EP_DENIED&&saved.terminal);
 saved=t;f.kind=5;f.sequence=3;f.payload.size=make(5);CHECK(ep_trace_observe(&saved,&f,EP_C2S)==EP_DENIED); /* CSR before declared approval */
 f.kind=4;f.sequence=2;f.payload.size=make(4);CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DATA_ONLY);body[16]=2;f.sequence=3;CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DATA_ONLY);
 f.kind=5;f.sequence=3;f.payload.size=make(5);CHECK(ep_trace_observe(&t,&f,EP_C2S)==EP_DATA_ONLY&&t.csr_size==1&&t.csr[0]==0x30);body[52]=0;CHECK(t.csr[0]==0x30);
 saved=t;f.kind=4;f.sequence=4;f.payload.size=make(4);body[16]=3;CHECK(ep_trace_observe(&saved,&f,EP_S2C)==EP_DATA_ONLY);f.kind=6;f.sequence=5;f.payload.size=make(6);CHECK(ep_trace_observe(&saved,&f,EP_S2C)==EP_DENIED);
 f.kind=6;f.sequence=4;f.payload.size=make(6);CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DATA_ONLY&&t.terminal);CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DENIED);ep_trace_stop(&t);CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_STOPPED&&t.csr[0]==0x30);
 }
 memset(&t,0,sizeof(t));f.kind=1;f.sequence=1;f.payload.size=make(1);CHECK(ep_trace_observe(&t,&f,EP_C2S)==EP_DATA_ONLY);f.kind=2;f.payload.size=make(2);body[16]^=1;CHECK(ep_trace_observe(&t,&f,EP_S2C)==EP_DENIED);
 memset(&t,0,sizeof(t));t.bytes[0]=32768;f.kind=1;f.payload.size=make(1);CHECK(ep_trace_observe(&t,&f,EP_C2S)==EP_DENIED);
 return 0;
}
static int pairing(void){
 EP_PAIR p,copy,live;PAPACC_U8 random[55],client[32],code[10];unsigned i;
 memset(random,0,sizeof(random));memset(client,7,sizeof(client));for(i=7;i<55;i++)random[i]=(PAPACC_U8)i;
 CHECK(ep_code_format(random,code)==EP_DATA_ONLY&&!memcmp(code,"0000000000",10));memset(random,255,7);CHECK(ep_code_format(random,code)==EP_DATA_ONLY&&!memcmp(code,"ZZZZZZZZZZ",10));
 CHECK(ep_code_parse_local((const PAPACC_U8 *)"01234-56789",11,code)==EP_DATA_ONLY&&!memcmp(code,"0123456789",10));CHECK(ep_code_parse_local((const PAPACC_U8 *)"01234-5678I",11,code)==EP_INVALID);CHECK(ep_code_parse_local((const PAPACC_U8 *)"012345678z",10,code)==EP_INVALID);
 memset(&p,0,sizeof(p));CHECK(ep_pair_prepare_data(&p,random,client,11,12,100,NULL,0)==EP_DATA_ONLY);copy=p;live=p;
 CHECK(ep_pair_claim_data(&p,p.code,10,11,12,299999)==EP_DATA_ONLY&&p.claimed&&p.submitted);CHECK(ep_pair_claim_data(&p,p.code,10,11,12,300000)==EP_REPLAY);
 p=copy;CHECK(ep_pair_claim_data(&p,p.code,10,11,12,300100)==EP_EXPIRED);p=copy;CHECK(ep_pair_claim_data(&p,p.code,10,11,13,101)==EP_DENIED);CHECK(ep_pair_claim_data(&p,p.code,10,11,12,102)==EP_REPLAY);
 p=copy;CHECK(ep_pair_claim_data(&p,(const PAPACC_U8 *)"0000000000",10,11,12,101)==EP_DENIED);p=copy;CHECK(ep_pair_claim_data(&p,p.code,10,11,12,99)==EP_EXPIRED);
 p=copy;ep_pair_stop(&p);CHECK(ep_pair_claim_data(&p,p.code,10,11,12,101)==EP_STOPPED&&!memcmp(p.code,copy.code,10));
 memset(&p,0,sizeof(p));CHECK(ep_pair_prepare_data(&p,random,client,11,12,100,&live,1)==EP_DENIED&&!p.initialized);CHECK(ep_pair_prepare_data(&p,random,client,11,12,(PAPACC_U64)-1,NULL,0)==EP_LIMIT);
#ifdef _WIN32
 memset(&p,0,sizeof(p));CHECK(ep_pair_generate_windows(&p,client,11,12,100,NULL,0)==EP_DATA_ONLY);CHECK(ep_pair_claim_data(&p,p.code,10,11,12,101)==EP_DATA_ONLY);CHECK(ep_pair_generate_windows(&p,client,11,12,102,NULL,0)==EP_INVALID);
#endif
 return 0;
}
static int limits(void){
 EP_LIMITER l;EP_ORIGIN o,other;EP_TICKET a,b;PAPACC_U8 ip[16]={10,1,2,3},mapped[16]={0,0,0,0,0,0,0,0,0,0,255,255,10,1,2,3};PAPACC_SIZE q=0;unsigned i;
 memset(&l,0,sizeof(l));CHECK(ep_origin_set(&o,4,ip,0)==EP_DATA_ONLY);CHECK(ep_origin_set(&other,6,mapped,0)==EP_DATA_ONLY&&!memcmp(&o,&other,sizeof(o)));
 CHECK(ep_limiter_admit(&l,&o,0,&a)==EP_DATA_ONLY);CHECK(ep_limiter_admit(&l,&o,1,&b)==EP_DATA_ONLY);CHECK(ep_limiter_admit(&l,&o,2,&b)==EP_LIMIT);
 CHECK(ep_limiter_release_data(&l,a)==EP_DATA_ONLY);CHECK(ep_limiter_release_data(&l,a)==EP_REPLAY);CHECK(ep_limiter_admit(&l,&o,2,&a)==EP_DATA_ONLY);CHECK(ep_limiter_release_data(&l,a)==EP_DATA_ONLY);CHECK(ep_limiter_admit(&l,&o,3,&a)==EP_LIMIT);
 CHECK(ep_limiter_admit(&l,&o,60000,&a)==EP_DATA_ONLY);ep_limiter_stop(&l);CHECK(ep_limiter_admit(&l,&o,120001,&a)==EP_STOPPED);CHECK(l.held[0]||l.held[1]);
 memset(&l,0,sizeof(l));for(i=0;i<6;i++){ip[3]=(PAPACC_U8)i;CHECK(ep_origin_set(&o,4,ip,0)==EP_DATA_ONLY);CHECK(ep_limiter_admit(&l,&o,100+i,&a)==EP_DATA_ONLY);CHECK(ep_limiter_release_data(&l,a)==EP_DATA_ONLY);}CHECK(ep_limiter_admit(&l,&o,107,&a)==EP_LIMIT);CHECK(ep_limiter_admit(&l,&o,99,&a)==EP_EXPIRED);
 CHECK(ep_enrollment_quote(0,262144,0,&q)==EP_DATA_ONLY&&q==262144);CHECK(ep_enrollment_quote(0,262145,0,&q)==EP_LIMIT);CHECK(ep_enrollment_quote(0,100,EP_UNKNOWN_BYTES,&q)==EP_LIMIT);CHECK(ep_enrollment_quote(PAPACC_ED11_PROCESS_LIMIT,1,0,&q)==EP_LIMIT);
 CHECK(ep_enrollment_quote(2103847,262144,0,&q)==(PAPACC_ED11_PROCESS_LIMIT==3145728U?EP_DATA_ONLY:EP_LIMIT));
 return 0;
}
int main(void){if(framing()||trace()||pairing()||limits())return 1;printf("PASS %u DATA_MODEL assertions; native_BCrypt_CSPRNG_pairing_secret_only; NO_CURRENT_APPROVAL_SIGNATURE_TLS_NETWORK\n",checks);printf("SIZES reader=%zu trace=%zu pair=%zu limiter=%zu RX_TX_two=%u process_limit=%u\n",sizeof(EP_READER),sizeof(EP_TRACE),sizeof(EP_PAIR),sizeof(EP_LIMITER),65600U,(unsigned)PAPACC_ED11_PROCESS_LIMIT);return 0;}
