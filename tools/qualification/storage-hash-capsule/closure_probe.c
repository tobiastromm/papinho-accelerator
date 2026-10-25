/* No CRT/imports probe: this is the exact capsule closure, not oracle/harness. */
#include "storage_sha256.h"
static PACC_STORAGE_SHA256 a,b;
static unsigned char x[32],y[32];
static const unsigned char expected[32]={0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
int probe_main(void){unsigned i;
 if(pacc_storage_sha_init(&a)||pacc_storage_sha_init(&b))return 1;
 if(pacc_storage_sha_update(&a,"a",1,0,0)||pacc_storage_sha_update(&b,"abc",3,0,0)||pacc_storage_sha_update(&a,"bc",2,0,0))return 2;
 if(pacc_storage_sha_final(&a,x,32,0,0)||pacc_storage_sha_final(&b,y,32,0,0))return 3;
 for(i=0;i<32;i++)if(x[i]!=expected[i]||y[i]!=expected[i])return 4;
 if(!pacc_storage_hash_callback(0,(const unsigned char *)"abc",3,x))return 5;
 for(i=0;i<32;i++)if(x[i]!=expected[i])return 6;
 return 0;}
