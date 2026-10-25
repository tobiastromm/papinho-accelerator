/* Offline capacity/lifetime test, not a transcript/source generator.
 * Uses only existing public verification vectors; never signs or creates keys.
 * Explicit storage is charged separately from OpenSSL allocation hooks.
 * No transport, syscall clock, root, pin, authority or production reservation. */
#include "roughtime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct RT_TEST_GRAPH {
 PAPACC_U8 requests[6][2048],responses[6][2048],randoms[6][32];
 PAPACC_U8 parser[4096],crypto[16384],metadata[1536],sources[1536],control[4096],history[488];
} RT_TEST_GRAPH;
static unsigned checks,failures;
static void ck(int value,int line){++checks;if(!value){++failures;printf("FAIL line=%d\n",line);}}
#define CK(x) ck((x),__LINE__)

#ifdef RT_LAB_TRANSCRIPT
static PAPACC_SIZE lab_response_bytes[6];
static int lab_read(const char *dir,const char *name,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE expected){
 char path[2048];FILE *f;size_t n;int tail;if(strlen(dir)+strlen(name)+2>=sizeof(path))return 0;
 sprintf(path,"%s/%s",dir,name);f=fopen(path,"rb");if(!f)return 0;n=fread(out,1,cap,f);tail=fgetc(f);fclose(f);return n==expected&&tail==EOF;
}
static int lab_load(RT_TEST_GRAPH *g,const char *dir){char name[64];unsigned i;
 for(i=0;i<6;i++){sprintf(name,"request-%u.bin",i);if(!lab_read(dir,name,g->requests[i],2048,2048))return 0;
  sprintf(name,"response-%u.bin",i);if(!lab_read(dir,name,g->responses[i],2048,480))return 0;lab_response_bytes[i]=480;}
 return lab_read(dir,"long-term-keys.bin",g->sources,96,96)&&lab_read(dir,"nonce-randoms.bin",&g->randoms[0][0],192,192);
}
#endif

#ifdef RT_OPENSSL_TEST
#include "roughtime_openssl.h"
#include <openssl/provider.h>
#include <openssl/crypto.h>
#include "roughtime_public_vector.inc"
typedef union RT_MEM_HEADER { size_t n; long double alignment; void *p; } RT_MEM_HEADER;
static size_t live,peak,phase_peak,allocations;
static void *measured_alloc(size_t n,const char *f,int line){RT_MEM_HEADER *p;(void)f;(void)line;
 if(n>(size_t)-1-sizeof(*p)||live>(size_t)-1-n-sizeof(*p))return NULL;
 p=(RT_MEM_HEADER *)malloc(n+sizeof(*p));if(!p)return NULL;
 p->n=n+sizeof(*p);live+=p->n;++allocations;if(live>peak)peak=live;if(live>phase_peak)phase_peak=live;return p+1;}
static void measured_free(void *v,const char *f,int line){RT_MEM_HEADER *p;(void)f;(void)line;if(!v)return;p=(RT_MEM_HEADER *)v-1;live-=p->n;free(p);}
static void *measured_resize(void *v,size_t n,const char *f,int line){RT_MEM_HEADER *p;void *q;size_t old;
 if(!v)return measured_alloc(n,f,line);if(!n){measured_free(v,f,line);return NULL;}
 p=(RT_MEM_HEADER *)v-1;old=p->n-sizeof(*p);q=measured_alloc(n,f,line);if(!q)return NULL;memcpy(q,v,old<n?old:n);measured_free(v,f,line);return q;}
static void phase(const char *name){printf("phase=%s provider_live=%u provider_phase_peak=%u provider_cumulative_peak=%u explicit_graph=%u\n",name,(unsigned)live,(unsigned)phase_peak,(unsigned)peak,(unsigned)sizeof(RT_TEST_GRAPH));phase_peak=live;}
#ifdef RT_LAB_TRANSCRIPT
#include "roughtime_lab_checks.inc"
#endif
static void capacity(RT_TEST_GRAPH *g){
 OSSL_LIB_CTX *lib;OSSL_PROVIDER *provider;PAPACC_RT_OPENSSL backend;PAPACC_RT_CRYPTO crypto;
 PAPACC_RT_BYTES requests[6],responses[6],slice;PAPACC_RT_REPLY replies[6],prior[6];
 PAPACC_RT_EXCHANGE exchange,saved;PAPACC_U8 before[12][32],after[32],keys[96];
 size_t retained;unsigned i,j;int valid;PAPACC_U64 lower=11,upper=12;PAPACC_UTC_OBSERVATION absent;
 static const PAPACC_U8 dc[]="Roughtime v1 delegation signature",rc[]="Roughtime v1 response signature";
 /* Install hooks before any native primitive; explicit graph is malloc-owned
  * outside provider counter and is charged as its own whole allocation. */
 CK(CRYPTO_set_mem_functions(measured_alloc,measured_resize,measured_free));
 CK(OPENSSL_init_crypto(OPENSSL_INIT_NO_LOAD_CONFIG,NULL));
 lib=OSSL_LIB_CTX_new();CK(lib!=NULL);if(!lib)return;
 provider=OSSL_PROVIDER_load(lib,"default");CK(provider!=NULL);if(!provider){OSSL_LIB_CTX_free(lib);return;}
 backend.libctx=lib;backend.scratch=g->crypto;backend.bytes=4096;
 crypto.user=&backend;crypto.hash=papacc_rt_openssl_hash;crypto.verify=papacc_rt_openssl_verify;
 phase("context_open_all_slots_reserved");
 /* Existing AppendixB signatures are negative vectors, not positive source
  * evidence. Every packet slot remains disjoint and physically touched. */
 #ifndef RT_LAB_TRANSCRIPT
 for(i=0;i<6;i++){
  memset(g->randoms[i],(int)(i+1U),32);memcpy(keys+32*(i%3),vector_key,32);
  CK(papacc_rt_request_data(&crypto,vector_key,g->randoms[i],g->requests[i])==PAPACC_RT_DATA_OK);
  memcpy(g->responses[i],vector_response,sizeof(vector_response));
  requests[i].p=g->requests[i];requests[i].n=2048;responses[i].p=g->responses[i];responses[i].n=sizeof(vector_response);
  CK(papacc_rt_packet_data(requests[i])==PAPACC_RT_DATA_OK);CK(papacc_rt_packet_data(responses[i])==PAPACC_RT_DATA_OK);
 }
 #else
 for(i=0;i<6;i++){
  requests[i].p=g->requests[i];requests[i].n=2048;responses[i].p=g->responses[i];responses[i].n=lab_response_bytes[i];
  CK(papacc_rt_packet_data(requests[i])==PAPACC_RT_DATA_OK);CK(papacc_rt_packet_data(responses[i])==PAPACC_RT_DATA_OK);
 }
#endif
 phase("six_requests_and_six_responses_all_slots_reserved");
#ifdef RT_LAB_TRANSCRIPT
 lab_checks(g,&crypto);phase("positive_lab_campaign_and_negative_regressions_complete");
#endif
 for(j=0;j<6;j++){
  valid=crypto.verify(crypto.user,vector_key,dc,sizeof(dc),vector_dele,sizeof(vector_dele),vector_dele_sig);CK(!valid);
  valid=crypto.verify(crypto.user,vector_pub,rc,sizeof(rc),vector_srep,sizeof(vector_srep),vector_srep_sig);CK(!valid);
 }
 memset(replies,0x55,sizeof(replies));memcpy(prior,replies,sizeof(replies));
#ifdef RT_LAB_TRANSCRIPT
 memcpy(keys,g->sources,96);g->randoms[0][0]^=1;
#endif
 CK(papacc_rt_campaign_data(&crypto,requests,responses,keys,&g->randoms[0][0],replies)==PAPACC_RT_INVALID);CK(!memcmp(prior,replies,sizeof(replies)));
#ifdef RT_LAB_TRANSCRIPT
 g->randoms[0][0]^=1;
#endif
 memset(&absent,0,sizeof(absent));CK(papacc_rt_interval_data(&replies[0],&absent,1,&lower,&upper)!=PAPACC_RT_DATA_OK);CK(lower==11&&upper==12);
 CK(papacc_issuer_qualified_utc_read(NULL,&absent)==PAPACC_RESULT_NOT_SUPPORTED);
 phase("negative_validation_output_preserved");
 /* Hash every full slot, including unused capacity. No slot reuse or
  * sharing deduction is assumed by this measurement. */
 for(i=0;i<12;i++){slice.p=i<6?g->requests[i]:g->responses[i-6];slice.n=2048;CK(crypto.hash(crypto.user,&slice,1,before[i]));}
 memset(&exchange,0,sizeof(exchange));CK(papacc_rt_exchange_start_data(&exchange,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&exchange,1)==PAPACC_RT_DATA_OK);saved=exchange;
 retained=live;papacc_rt_exchange_stop_data(&exchange);CK(live==retained);CK(exchange.phase==saved.phase&&exchange.start==saved.start&&exchange.sent==saved.sent);
 CK(papacc_rt_exchange_received_data(&exchange,2)==PAPACC_RT_RETAINED);
 for(i=0;i<12;i++){slice.p=i<6?g->requests[i]:g->responses[i-6];slice.n=2048;CK(crypto.hash(crypto.user,&slice,1,after));CK(!memcmp(after,before[i],32));}
 phase("stop_retained_all_transcripts_unchanged");
 OSSL_PROVIDER_unload(provider);OSSL_LIB_CTX_free(lib);phase("synchronous_calls_returned_context_closed_graph_still_owned");
 printf("test_known_heap_charge=%u provider_peak=%u graph=%u allocations=%u\n",(unsigned)(peak+sizeof(*g)),(unsigned)peak,(unsigned)sizeof(*g),(unsigned)allocations);
 /* All calls are synchronous and complete, no I/O or borrowers were started.
  * This test cleanup is not proof of future native transport quiescence. */
 OPENSSL_cleanup();printf("provider_live_after_test_cleanup=%u\n",(unsigned)live);CK(live==0);
}
#endif
int main(int argc,char **argv){
 RT_TEST_GRAPH *graph;PAPACC_RT_EXCHANGE exchange;
#ifdef RT_LAB_TRANSCRIPT
 PAPACC_RT_BYTES packet;unsigned i;
#endif
 PAPACC_U64 n=(PAPACC_U64)1000000000UL;
 CK(sizeof(RT_TEST_GRAPH)==52904U);graph=(RT_TEST_GRAPH *)malloc(sizeof(*graph));CK(graph!=NULL);if(!graph)return 1;
 (void)argc;(void)argv;memset(graph,0,sizeof(*graph));memset(&exchange,0,sizeof(exchange));
#ifdef RT_LAB_TRANSCRIPT
 CK(argc==2);if(argc!=2){free(graph);return 2;}if(!lab_load(graph,argv[1])){CK(0);free(graph);return 2;}CK(1);
 for(i=0;i<6;i++){packet.p=graph->requests[i];packet.n=2048;CK(papacc_rt_packet_data(packet)==PAPACC_RT_DATA_OK);packet.p=graph->responses[i];packet.n=lab_response_bytes[i];CK(papacc_rt_packet_data(packet)==PAPACC_RT_DATA_OK);}
#endif
 CK(papacc_rt_exchange_start_data(&exchange,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&exchange,n)==PAPACC_RT_EXPIRED);
 papacc_rt_exchange_stop_data(&exchange);CK(papacc_rt_exchange_received_data(&exchange,n)==PAPACC_RT_RETAINED);
#ifdef RT_OPENSSL_TEST
 capacity(graph);
#endif
 free(graph);printf("capacity_checks=%u failures=%u graph=%u exchange=%u history=%u observation=%u\n",checks,failures,(unsigned)sizeof(RT_TEST_GRAPH),(unsigned)sizeof(PAPACC_RT_EXCHANGE),(unsigned)sizeof(PAPACC_UTC_HISTORY),(unsigned)sizeof(PAPACC_UTC_OBSERVATION));return failures?1:0;
}
