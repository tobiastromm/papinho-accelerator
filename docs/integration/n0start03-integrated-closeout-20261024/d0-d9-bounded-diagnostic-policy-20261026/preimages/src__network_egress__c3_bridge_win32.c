#include "c3_bridge_win32.h"
#include "pst_socket_attachment_win32.h"
#include "papinho_secure_transport_win32.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* Optional isolated diagnostic observation. Numeric identities are never
 * usable PST references and no secret or native error is put on PACC wire. */
static void c3_trace(PAPACC_C3_BRIDGE *b,const char *event,PST_RESULT result,pst_u32 disposition)
{
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
 const char *enabled=getenv("PAPACC_C2_PIPELINE_TRACE");
 if(enabled==NULL||enabled[0]!='1'||enabled[1]!=0)return;
 printf("C3_LIFETIME pid=%lu thread=%lu sequence=%llu operation=%llu generation=%llu pst_identity=%llu peer_lifetime=%llu event=%s result=%lu disposition=%lu ba_received=%llu ba_accepted=%llu ab_received=%llu ab_accepted=%llu\n",(unsigned long)GetCurrentProcessId(),(unsigned long)b->owner_thread,(unsigned long long)++b->trace_sequence,(unsigned long long)b->operation,(unsigned long long)b->generation,(unsigned long long)b->child_identity_value,(unsigned long long)b->peer_identity_value,event,(unsigned long)result,(unsigned long)disposition,(unsigned long long)b->ba.received,(unsigned long long)b->ba.accepted,(unsigned long long)b->ab.received,(unsigned long long)b->ab.accepted);
 fflush(stdout);
 if(strcmp(event,"BA_NATIVE_SEND_WOULD_BLOCK")==0){char name[128];DWORD count=GetEnvironmentVariableA("PAPINHO_ED11_RECORD_PRESSURE_RELEASE_EVENT",name,sizeof(name));
  if(count>0U&&count<sizeof(name)&&strncmp(name,"Local\\PapinhoS6AppReadRelease-",30)==0){HANDLE gate=OpenEventA(EVENT_MODIFY_STATE,FALSE,name);if(gate){(void)SetEvent(gate);(void)CloseHandle(gate);}}
 }
#else
 (void)b;(void)event;(void)result;(void)disposition;
#endif
}
static int owner(const PAPACC_C3_BRIDGE*b){return b!=NULL&&b->initialized&&b->owner_thread==GetCurrentThreadId();}
static PAPACC_RESULT failure(PAPACC_C3_BRIDGE*b,PST_RESULT cause){if(b->first_failure==PST_RESULT_OK)b->first_failure=cause;b->source_result=cause;b->revoked=PAPACC_TRUE;return PAPACC_RESULT_INTERNAL_ERROR;}
static int queue_add_ok(PAPACC_U64 n,PAPACC_SIZE amount){return n<=(PAPACC_U64)-1-(PAPACC_U64)amount;}
static int tuple_equal(const struct sockaddr_in*a,const struct sockaddr_in*b){return a->sin_family==AF_INET&&b->sin_family==AF_INET&&a->sin_addr.s_addr==b->sin_addr.s_addr&&a->sin_port==b->sin_port;}
static PAPACC_RESULT reconcile_peer(PAPACC_C3_BRIDGE*b){
 pst_u32 interest=0;pst_wait_token token;PAPACC_RESULT result;
 if(b->peer_source==NULL)return PAPACC_RESULT_OK;
 if(b->ab.size>=49152U)b->ab_read_paused=PAPACC_TRUE;
 if(b->ab_read_paused&&b->ab.size<=24576U)b->ab_read_paused=PAPACC_FALSE;
 if(!b->revoked&&!b->retained&&!b->ab_sealed){if(!b->ab_eof&&!b->ab_read_paused)interest|=PST_INTEREST_READ;if(b->ba.size&&!b->child_destroyed&&!b->c7_hold_ba)interest|=PST_INTEREST_WRITE;}
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
 {char enabled[8];if(GetEnvironmentVariableA("PAPACC_C7_METRICS",enabled,sizeof(enabled))==1U&&enabled[0]=='1'&&((interest!=b->peer_interest)||(b->ab_read_paused&&(b->peer_interest&PST_INTEREST_READ))))
  fprintf(stderr,"E3_C3_READ_POLICY operation=%llu generation=%llu occupancy=%llu paused=%d read_interest=%d high=49152 low=24576 cap=65536\n",(unsigned long long)b->operation,(unsigned long long)b->generation,(unsigned long long)b->ab.size,b->ab_read_paused,(interest&PST_INTEREST_READ)!=0);}
#endif
 if(interest==b->peer_interest&&((interest==0U&&!b->peer_registered)||(interest!=0U&&b->peer_registered)))return PAPACC_RESULT_OK;
 if(b->peer_registered){result=papacc_pst_secure_scheduler_remove_external(b->scheduler,b->peer_source);if(result!=0)return result;b->peer_registered=PAPACC_FALSE;b->peer_interest=0;}
 if(interest!=0U){result=papacc_pst_secure_scheduler_add_external(b->scheduler,b->peer_source,(interest&PST_INTEREST_READ)?PAPACC_TRUE:PAPACC_FALSE,(interest&PST_INTEREST_WRITE)?PAPACC_TRUE:PAPACC_FALSE,&token);if(result!=0)return result;b->peer_registered=PAPACC_TRUE;b->peer_interest=interest;}
 return PAPACC_RESULT_OK;
}
static PAPACC_RESULT close_owned_once(PAPACC_C3_BRIDGE*b,SOCKET*s,PAPACC_BOOL*attempted){
 if(*s==INVALID_SOCKET)return PAPACC_RESULT_OK;
 if(*attempted)return PAPACC_RESULT_INVALID_STATE;
 *attempted=PAPACC_TRUE;
 if(closesocket(*s)!=0){b->retained=PAPACC_TRUE;return failure(b,PST_RESULT_RESOURCE_FAILURE);}
 *s=INVALID_SOCKET;return PAPACC_RESULT_OK;
}
static PAPACC_RESULT private_pair(PAPACC_C3_BRIDGE*b){
 struct sockaddr_in address,local_a,remote_a,local_b,remote_b;int n=(int)sizeof(address),error=0;u_long nonblocking=1;ULONGLONG deadline=GetTickCount64()+5000U;
 b->setup_listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(b->setup_listener==INVALID_SOCKET)return failure(b,PST_RESULT_RESOURCE_FAILURE);
 memset(&address,0,sizeof(address));address.sin_family=AF_INET;address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
 if(ioctlsocket(b->setup_listener,FIONBIO,&nonblocking)||bind(b->setup_listener,(struct sockaddr*)&address,n)||listen(b->setup_listener,1)||getsockname(b->setup_listener,(struct sockaddr*)&address,&n))return failure(b,PST_RESULT_RESOURCE_FAILURE);

#ifdef PAPACC_C2_C3_TEST_BARRIERS
 {char ready_name[128],continue_name[128];DWORD ready_size=GetEnvironmentVariableA("PAPACC_C3_ATTACH_READY_EVENT",ready_name,sizeof(ready_name));
  if(ready_size){HANDLE ready_event=NULL,continue_event=NULL;DWORD waited=WAIT_FAILED;PAPACC_BOOL signalled=PAPACC_FALSE;
   if(ready_size>=sizeof(ready_name)||GetEnvironmentVariableA("PAPACC_C3_ATTACH_CONTINUE_EVENT",continue_name,sizeof(continue_name))==0U)return failure(b,PST_RESULT_RESOURCE_FAILURE);
   ready_event=OpenEventA(EVENT_MODIFY_STATE,FALSE,ready_name);continue_event=OpenEventA(SYNCHRONIZE,FALSE,continue_name);
   if(ready_event&&continue_event){signalled=SetEvent(ready_event)?PAPACC_TRUE:PAPACC_FALSE;if(signalled)waited=WaitForSingleObject(continue_event,5000U);}
   if(ready_event)CloseHandle(ready_event);if(continue_event)CloseHandle(continue_event);
   if(!signalled||waited!=WAIT_OBJECT_0)return failure(b,PST_RESULT_RESOURCE_FAILURE);
   c3_trace(b,"TEST_ONLY_PRIVATE_LISTENER_BARRIER_RELEASED",PST_RESULT_OK,0U);
  }
 }
#endif
 b->setup_connector=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(b->setup_connector==INVALID_SOCKET||ioctlsocket(b->setup_connector,FIONBIO,&nonblocking))return failure(b,PST_RESULT_RESOURCE_FAILURE);
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
 {char record_gate[128];if(GetEnvironmentVariableA("PAPINHO_ED11_RECORD_PRESSURE_RELEASE_EVENT",record_gate,sizeof(record_gate))>0U){int requested=16384;
  if(setsockopt(b->setup_connector,SOL_SOCKET,SO_RCVBUF,(const char*)&requested,sizeof(requested)))return failure(b,PST_RESULT_RESOURCE_FAILURE);
 }}
#endif
 if(connect(b->setup_connector,(struct sockaddr*)&address,n)==SOCKET_ERROR&&WSAGetLastError()!=WSAEWOULDBLOCK)return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 while(b->peer==INVALID_SOCKET){
  fd_set readset;struct timeval wait;
  b->peer=accept(b->setup_listener,NULL,NULL);if(b->peer!=INVALID_SOCKET)break;
  if(WSAGetLastError()!=WSAEWOULDBLOCK||GetTickCount64()>=deadline)return failure(b,PST_RESULT_TRANSPORT_FAILURE);
  FD_ZERO(&readset);FD_SET(b->setup_listener,&readset);wait.tv_sec=0;wait.tv_usec=10000;
  if(select(0,&readset,NULL,NULL,&wait)==SOCKET_ERROR)return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 }
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
 {char record_gate[128];if(GetEnvironmentVariableA("PAPINHO_ED11_RECORD_PRESSURE_RELEASE_EVENT",record_gate,sizeof(record_gate))>0U){int requested=16384;
  if(setsockopt(b->peer,SOL_SOCKET,SO_SNDBUF,(const char*)&requested,sizeof(requested)))return failure(b,PST_RESULT_RESOURCE_FAILURE);
  c3_trace(b,"ED11_DIAGNOSTIC_C3_SOCKET_HINTS_16K_NOT_KERNEL_BOUND",PST_RESULT_OK,0U);
 }}
#endif
 if(close_owned_once(b,&b->setup_listener,&b->setup_listener_close_attempted)!=0)return PAPACC_RESULT_INTERNAL_ERROR;
 n=(int)sizeof(error);if(getsockopt(b->setup_connector,SOL_SOCKET,SO_ERROR,(char*)&error,&n)||error!=0||ioctlsocket(b->peer,FIONBIO,&nonblocking))return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 n=(int)sizeof(local_a);if(getsockname(b->setup_connector,(struct sockaddr*)&local_a,&n))return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 n=(int)sizeof(remote_a);if(getpeername(b->setup_connector,(struct sockaddr*)&remote_a,&n))return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 n=(int)sizeof(local_b);if(getsockname(b->peer,(struct sockaddr*)&local_b,&n))return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 n=(int)sizeof(remote_b);if(getpeername(b->peer,(struct sockaddr*)&remote_b,&n))return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 if(local_a.sin_addr.s_addr!=htonl(INADDR_LOOPBACK)||!tuple_equal(&remote_a,&local_b)||!tuple_equal(&remote_b,&local_a)||!tuple_equal(&address,&local_b))return failure(b,PST_RESULT_PROTOCOL_FAILURE);
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c3_bridge_init(PAPACC_C3_BRIDGE*b,pst_runtime*runtime,const PST_CONNECTION_CONFIG*config,PAPACC_PST_SECURE_SCHEDULER*scheduler,PAPACC_CONNECTION_MANAGER*manager,PAPACC_U64 operation,PAPACC_U64 generation,PAPACC_U64 deadline_ns){
 PAPACC_RESULT result;PST_RESULT source;PST_PROVIDER_INFO provider;PAPACC_BOOL accepted=PAPACC_FALSE;pst_wait_token token;pst_size index;
 if(b==NULL||runtime==NULL||config==NULL||scheduler==NULL||manager==NULL||operation==0U||generation==0U||deadline_ns==0U)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(b->initialized||!scheduler->initialized||!manager->initialized||config->role!=PST_CONNECTION_ROLE_CLIENT||config->tls.minimum_version!=PST_TLS_VERSION_1_3||config->tls.maximum_version!=PST_TLS_VERSION_1_3||config->tls.resumption!=PST_FEATURE_DISABLED||config->tls.early_data!=PST_FEATURE_DISABLED||config->peer_authentication.certificate_mode!=PST_PEER_CERTIFICATE_REQUIRED||config->tls.require_graceful_shutdown!=PST_FEATURE_REQUIRED||config->peer_authentication.expected_peer_name==NULL||config->peer_authentication.expected_peer_name_size==0U||config->peer_authentication.expected_peer_name_size>253U||config->server_name_indication==NULL||config->server_name_indication_mode!=PST_SNI_MODE_EXPLICIT||config->server_name_indication_size!=config->peer_authentication.expected_peer_name_size||memcmp(config->server_name_indication,config->peer_authentication.expected_peer_name,config->server_name_indication_size))return PAPACC_RESULT_INVALID_STATE;
 memset(b,0,sizeof(*b));b->peer=b->setup_listener=b->setup_connector=INVALID_SOCKET;b->owner_thread=GetCurrentThreadId();b->expected_name_size=(PAPACC_SIZE)config->peer_authentication.expected_peer_name_size;memcpy(b->expected_name,config->peer_authentication.expected_peer_name,b->expected_name_size);b->operation=operation;b->generation=generation;b->scheduler=scheduler;b->manager=manager;b->initialized=PAPACC_TRUE;
 b->require_alpn=config->alpn.mode==PST_FEATURE_REQUIRED?PAPACC_TRUE:PAPACC_FALSE;b->allow_empty_alpn=(config->alpn.mode==PST_FEATURE_OPTIONAL||config->alpn.mode==PST_FEATURE_DISABLED)?PAPACC_TRUE:PAPACC_FALSE;
 if(config->alpn.protocol_count>8U||(config->alpn.protocol_count!=0U&&config->alpn.protocols==NULL)){b->revoked=PAPACC_TRUE;return PAPACC_RESULT_INVALID_ARGUMENT;}
 for(index=0;index<config->alpn.protocol_count;++index){pst_size size=config->alpn.protocols[index].size;
  if(size==0U||size>255U||config->alpn.protocols[index].data==NULL||b->alpn_offer_bytes>=256U||size>255U-b->alpn_offer_bytes){b->revoked=PAPACC_TRUE;return PAPACC_RESULT_INVALID_ARGUMENT;}
  b->alpn_offers[b->alpn_offer_bytes++]=(PAPACC_U8)size;memcpy(b->alpn_offers+b->alpn_offer_bytes,config->alpn.protocols[index].data,size);b->alpn_offer_bytes+=size;
 }
 result=papacc_pst_secure_processor_init(&b->processor,runtime,config,deadline_ns);if(result!=0){b->revoked=PAPACC_TRUE;(void)reconcile_peer(b);return result;}b->child_acquired=PAPACC_TRUE;b->child_identity_value=(PAPACC_SIZE)b->processor.connection;c3_trace(b,"PST_CHILD_ACQUIRED",PST_RESULT_OK,0U);
 memset(&provider,0,sizeof(provider));provider.struct_size=sizeof(provider);provider.api_version=PST_API_VERSION;
 source=pst_connection_get_provider_info(b->processor.connection,&provider);if(source!=0||strcmp(provider.provider_id,"openssl"))return failure(b,source!=0?source:PST_RESULT_UNSUPPORTED);
 result=private_pair(b);if(result!=0)return result;b->peer_identity_value=(PAPACC_SIZE)b->peer;c3_trace(b,"PRIVATE_PEER_ACQUIRED",PST_RESULT_OK,0U);
 /* Socket wrapper/attach is handed off once below; no native alias cleanup. */

#ifdef PAPACC_C2_C3_TEST_BARRIERS
 if(GetEnvironmentVariableA("PAPACC_C6_SOCKET_IDENTITIES",NULL,0)>0U)
  printf("C6_NATIVE_CHILD_TRANSFER operation=%llu generation=%llu pst_identity=%llu socket=%llu peer=%llu waitset_registered_before_release=REQUIRED\n",(unsigned long long)b->operation,(unsigned long long)b->generation,(unsigned long long)b->child_identity_value,(unsigned long long)b->setup_connector,(unsigned long long)b->peer);
#endif
 result=papacc_pst_socket_attach(&b->processor,&b->setup_connector,&b->unconsumed_transport,&accepted);
 if(accepted)b->bound=PAPACC_TRUE;
 if(b->unconsumed_transport!=NULL){b->retained=PAPACC_TRUE;return failure(b,PST_RESULT_RESOURCE_FAILURE);}
 if(result!=0)return failure(b,b->processor.source_result);
 result=papacc_pst_secure_scheduler_add_connection(scheduler,b->processor.connection,&token);
 if(result!=0)return failure(b,PST_RESULT_RESOURCE_FAILURE);b->registered=PAPACC_TRUE;source=pst_win32_socket_external_source_create((pst_size)b->peer,&b->peer_source);if(source!=0)return failure(b,source);return reconcile_peer(b);
}
PAPACC_RESULT papacc_c3_bridge_publish(PAPACC_C3_BRIDGE*b){
 PAPACC_NETWORK_ENDPOINT endpoint=PAPACC_NETWORK_ENDPOINT_INITIALIZER;PAPACC_RESULT result;
 if(!owner(b)||b->revoked||b->retained||b->published||b->processor.state!=PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED)return PAPACC_RESULT_INVALID_STATE;
 {pst_peer_info *peer=NULL;PST_RESULT source;
  source=pst_connection_get_peer_info(b->processor.connection,&peer);
  if(source!=PST_RESULT_OK)return failure(b,source);
  memset(&b->tls_facts,0,sizeof(b->tls_facts));b->tls_facts.struct_size=sizeof(b->tls_facts);b->tls_facts.api_version=PST_API_VERSION;
  source=pst_peer_info_get_summary(peer,&b->tls_facts);pst_peer_info_release(peer);
  if(source!=PST_RESULT_OK||b->tls_facts.local_role!=PST_CONNECTION_ROLE_CLIENT||b->tls_facts.certificate_present!=PST_KNOWN_TRUE||b->tls_facts.chain_validated!=PST_KNOWN_TRUE||b->tls_facts.peer_name_validated!=PST_KNOWN_TRUE||b->tls_facts.peer_authenticated!=PST_KNOWN_TRUE||b->tls_facts.tls_version!=PST_TLS_VERSION_1_3)return failure(b,source!=0?source:PST_RESULT_AUTH_FAILURE);
  {pst_size selected=0;PAPACC_SIZE cursor=0;PAPACC_BOOL match=PAPACC_FALSE;
   source=pst_connection_get_negotiated_alpn(b->processor.connection,b->selected_alpn,sizeof(b->selected_alpn),&selected);
   if(source==PST_RESULT_UNAVAILABLE&&b->allow_empty_alpn&&!b->require_alpn&&b->tls_facts.alpn_available==PST_KNOWN_FALSE)selected=0;
   else if(source!=PST_RESULT_OK||selected==0U||selected>255U)return failure(b,source!=0?source:PST_RESULT_PROTOCOL_FAILURE);
   if(selected){while(cursor<b->alpn_offer_bytes){PAPACC_SIZE size=b->alpn_offers[cursor++];if(size==selected&&memcmp(b->alpn_offers+cursor,b->selected_alpn,size)==0)match=PAPACC_TRUE;cursor+=size;}if(!match)return failure(b,PST_RESULT_PROTOCOL_FAILURE);}
   b->selected_alpn_size=selected;
  }
  b->tls_verified=PAPACC_TRUE;
 }
 if(!papacc_transport_connection_is_valid(&b->transport)){result=papacc_pst_secure_transport_adapter_init(&b->adapter,&b->processor,b->scheduler,&b->transport);if(result!=0)return result;}
 (void)papacc_ip_address_set_ipv4(&endpoint.address,127,0,0,1);
 result=papacc_connection_manager_publish(b->manager,&b->transport,&endpoint,&endpoint,&b->connection);
 if(result!=0)return result;b->published=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c3_bridge_accept_ba(PAPACC_C3_BRIDGE*b,PAPACC_U64 operation,PAPACC_U64 generation,PAPACC_U64 offset,const PAPACC_U8*p,PAPACC_SIZE n){
 if(!owner(b)||b->revoked||b->retained||b->ba_frozen||b->ba_input_ended||b->child_destroyed||operation!=b->operation||generation!=b->generation)return PAPACC_RESULT_INVALID_STATE;
 if(p==NULL||n==0U||n>PAPACC_C3_SLICE||offset!=b->ba.received||!queue_add_ok(offset,n))return PAPACC_RESULT_PROTOCOL_ERROR;
 if(n>PAPACC_C3_QUEUE_CAPACITY-b->ba.size)return PAPACC_RESULT_LIMIT_EXCEEDED;
 memcpy(b->ba.bytes+b->ba.size,p,n);b->ba.size+=n;b->ba.received+=n;return reconcile_peer(b);
}
PAPACC_RESULT papacc_c3_bridge_step(PAPACC_C3_BRIDGE*b){
 int n,error;PAPACC_SIZE amount;
 if(!owner(b)||b->revoked||b->retained||b->peer==INVALID_SOCKET||b->ab_sealed)return PAPACC_RESULT_INVALID_STATE;
 if(b->ba.size&&!b->c7_hold_ba){amount=b->ba.size<PAPACC_C3_SLICE?b->ba.size:PAPACC_C3_SLICE;n=send(b->peer,(const char*)b->ba.bytes,(int)amount,0);
  if(n>0){b->ba.accepted+=(PAPACC_U64)n;b->ba.size-=(PAPACC_SIZE)n;if(b->ba.size)memmove(b->ba.bytes,b->ba.bytes+n,b->ba.size);}
  else {error=WSAGetLastError();if(n==0||error!=WSAEWOULDBLOCK)return failure(b,PST_RESULT_TRANSPORT_FAILURE);c3_trace(b,"BA_NATIVE_SEND_WOULD_BLOCK",PST_RESULT_OK,0U);}
 }
 if(!b->ab_eof&&!b->ab_read_paused&&b->ab.size<49152U){amount=PAPACC_C3_QUEUE_CAPACITY-b->ab.size;if(amount>PAPACC_C3_SLICE)amount=PAPACC_C3_SLICE;
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
{char enabled[8];if(GetEnvironmentVariableA("PAPACC_C7_METRICS",enabled,sizeof(enabled))==1U&&enabled[0]=='1')fprintf(stderr,"E3_C3_NATIVE_READ operation=%llu generation=%llu occupancy=%llu paused=%d offered=%llu\n",(unsigned long long)b->operation,(unsigned long long)b->generation,(unsigned long long)b->ab.size,b->ab_read_paused,(unsigned long long)amount);}
#endif
n=recv(b->peer,(char*)b->ab.bytes+b->ab.size,(int)amount,0);
  if(n>0){if(!queue_add_ok(b->ab.received,(PAPACC_SIZE)n))return failure(b,PST_RESULT_PROTOCOL_FAILURE);b->ab.size+=(PAPACC_SIZE)n;b->ab.received+=(PAPACC_U64)n;}
  else if(n==0){b->ab_eof=PAPACC_TRUE;c3_trace(b,b->child_destroyed?"C3_ORDERED_EOF":"C3_EARLY_EOF",PST_RESULT_OK,0U);if(!b->child_destroyed)return failure(b,PST_RESULT_TRUNCATED);}
  else if(WSAGetLastError()!=WSAEWOULDBLOCK)return failure(b,PST_RESULT_TRANSPORT_FAILURE);
 }
 return reconcile_peer(b);
}
/* Factual Site producer EOF ends only the private C3 input direction.
 * This is not BA seal, TLS completion, AB EOF, release, or native close.
 * Caller must prove the native RECORD_EVENT and exact admitted prefix. */
PAPACC_RESULT papacc_c3_bridge_end_ba_input(PAPACC_C3_BRIDGE*b,PAPACC_U64 operation,PAPACC_U64 generation){
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed||b->peer==INVALID_SOCKET||operation!=b->operation||generation!=b->generation)return PAPACC_RESULT_INVALID_STATE;
 if(b->ba_input_ended)return PAPACC_RESULT_OK;
 if(b->ba.size!=0U||b->ba.received!=b->ba.accepted)return PAPACC_RESULT_INVALID_STATE;
 if(shutdown(b->peer,SD_SEND)==SOCKET_ERROR){c3_trace(b,"BA_INPUT_EOF_FAILED",PST_RESULT_TRANSPORT_FAILURE,0U);return failure(b,PST_RESULT_TRANSPORT_FAILURE);}
 b->ba_input_ended=PAPACC_TRUE;c3_trace(b,"BA_INPUT_ORDERED_EOF_SUBMITTED",PST_RESULT_OK,0U);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c3_bridge_commit_ab(PAPACC_C3_BRIDGE*b,PAPACC_U64 operation,PAPACC_U64 generation,PAPACC_SIZE n){
 if(!owner(b)||b->revoked||b->retained||b->ab_sealed||operation!=b->operation||generation!=b->generation)return PAPACC_RESULT_INVALID_STATE;
 if(n>b->ab.size||!queue_add_ok(b->ab.accepted,n))return PAPACC_RESULT_INVALID_ARGUMENT;
 b->ab.accepted+=n;b->ab.size-=n;if(b->ab.size)memmove(b->ab.bytes,b->ab.bytes+n,b->ab.size);return reconcile_peer(b);
}
PAPACC_RESULT papacc_c3_bridge_shutdown_once(PAPACC_C3_BRIDGE*b){
 PAPACC_PST_SECURE_STEP step;PAPACC_RESULT result;
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed)return PAPACC_RESULT_INVALID_STATE;
 if(b->tls_complete)return PAPACC_RESULT_OK;
 result=papacc_pst_secure_processor_shutdown_once(&b->processor,&step);if(result!=0)return failure(b,b->processor.source_result);
 if(step==PAPACC_PST_SECURE_STEP_COMPLETE){b->tls_complete=PAPACC_TRUE;c3_trace(b,"PUBLIC_RECIPROCAL_COMPLETE",b->processor.source_result,0U);}return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c3_bridge_freeze_ba(PAPACC_C3_BRIDGE*b){if(!owner(b)||b->revoked||b->retained||!b->tls_complete)return PAPACC_RESULT_INVALID_STATE;b->ba_frozen=PAPACC_TRUE;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_c3_bridge_ba_ack_written(PAPACC_C3_BRIDGE*b){if(!owner(b)||b->revoked||b->retained||!b->ba_frozen||b->ba.size||b->ba.received!=b->ba.accepted)return PAPACC_RESULT_INVALID_STATE;if(!b->ba_ack_written)c3_trace(b,"BA_ACK_FULL_WRITE_OBSERVED",PST_RESULT_OK,0U);b->ba_ack_written=PAPACC_TRUE;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_c3_bridge_release_child(PAPACC_C3_BRIDGE*b){
 PAPACC_BOOL removed=PAPACC_FALSE;PAPACC_RESULT result;
 if(!owner(b)||!b->child_acquired||b->retained||b->child_destroyed)return PAPACC_RESULT_INVALID_STATE;
 if(!b->revoked&&(!b->tls_complete||!b->ba_ack_written))return PAPACC_RESULT_INVALID_STATE;
 if(b->published){
  result=papacc_pst_secure_transport_adapter_prepare_close(&b->adapter);b->registered=b->adapter.registered;
  if(result!=PAPACC_RESULT_OK)return result;
  c3_trace(b,"WAITSET_REMOVE_CONFIRMED",PST_RESULT_OK,0U);
  c3_trace(b,"PST_RELEASE_ATTEMPT",PST_RESULT_OK,0U);
  result=papacc_connection_manager_try_remove(b->manager,b->connection->connection_instance_id,&removed);b->registered=b->adapter.registered;
 }
 else {
  if(b->registered){result=papacc_pst_secure_scheduler_remove_connection(b->scheduler,b->processor.connection);if(result!=0)return result;b->registered=PAPACC_FALSE;}
  c3_trace(b,"WAITSET_REMOVE_CONFIRMED",PST_RESULT_OK,0U);c3_trace(b,"PST_RELEASE_ATTEMPT",PST_RESULT_OK,0U);
  result=papacc_pst_secure_processor_release(&b->processor);removed=result==0&&b->processor.connection==NULL?PAPACC_TRUE:PAPACC_FALSE;
 }
 b->release_info=b->processor.release_info;b->source_result=b->processor.source_result;
 c3_trace(b,"PST_RELEASE_RESULT",b->source_result,0U);c3_trace(b,"PST_RELEASE_DISPOSITION",b->source_result,b->release_info.disposition);
 if(b->processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL){b->retained=PAPACC_TRUE;b->revoked=PAPACC_TRUE;(void)reconcile_peer(b);return result;}
 if(result!=0||!removed)return result==0?PAPACC_RESULT_INTERNAL_ERROR:result;
 b->child_destroyed=PAPACC_TRUE;b->registered=PAPACC_FALSE;b->connection=NULL;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c3_bridge_seal_ab(PAPACC_C3_BRIDGE*b,PAPACC_U64 operation,PAPACC_U64 generation){
 if(!owner(b)||b->revoked||b->retained||operation!=b->operation||generation!=b->generation||!b->child_destroyed||!b->ab_eof||b->ab.size||b->ab.received!=b->ab.accepted)return PAPACC_RESULT_INVALID_STATE;
 b->ab_sealed=PAPACC_TRUE;return reconcile_peer(b);
}
PAPACC_RESULT papacc_c3_bridge_revoke(PAPACC_C3_BRIDGE*b){if(!owner(b))return PAPACC_RESULT_INVALID_STATE;b->revoked=PAPACC_TRUE;return reconcile_peer(b);}
PAPACC_RESULT papacc_c3_bridge_close_peer(PAPACC_C3_BRIDGE*b){
 if(!owner(b)||b->retained||!b->child_destroyed||(!b->revoked&&!b->ab_sealed))return PAPACC_RESULT_INVALID_STATE;
 if(b->peer_registered){PAPACC_RESULT result=papacc_pst_secure_scheduler_remove_external(b->scheduler,b->peer_source);if(result!=0)return result;b->peer_registered=PAPACC_FALSE;b->peer_interest=0;}
 if(b->peer_source!=NULL){PST_RESULT result=pst_external_source_try_release(b->peer_source);if(result!=0)return PAPACC_RESULT_INTERNAL_ERROR;b->peer_source=NULL;}
 if(close_owned_once(b,&b->setup_listener,&b->setup_listener_close_attempted)!=0)return PAPACC_RESULT_INTERNAL_ERROR;
 if(close_owned_once(b,&b->setup_connector,&b->setup_connector_close_attempted)!=0)return PAPACC_RESULT_INTERNAL_ERROR;
 if(b->peer==INVALID_SOCKET)return PAPACC_RESULT_OK;
 if(b->peer_close_attempted)return PAPACC_RESULT_INVALID_STATE;
 b->peer_close_attempted=PAPACC_TRUE;
 if(closesocket(b->peer)!=0){b->retained=PAPACC_TRUE;return failure(b,PST_RESULT_RESOURCE_FAILURE);}
 c3_trace(b,"NATIVE_PEER_CLOSE_CONFIRMED",PST_RESULT_OK,0U);b->peer=INVALID_SOCKET;return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_c3_bridge_reset_destroyed(PAPACC_C3_BRIDGE *b)
{
 if(!owner(b)||b->retained||!b->child_acquired||!b->child_destroyed||
  b->release_info.disposition!=PST_RELEASE_DISPOSITION_DESTROYED||
  b->processor.connection!=NULL||b->connection!=NULL||b->registered||
  b->adapter.registered||b->peer_source!=NULL||b->peer!=INVALID_SOCKET||
  b->setup_connector!=INVALID_SOCKET||b->setup_listener!=INVALID_SOCKET)
  return PAPACC_RESULT_INVALID_STATE;
 SecureZeroMemory(b,sizeof(*b));b->peer=b->setup_connector=b->setup_listener=INVALID_SOCKET;
 return PAPACC_RESULT_OK;
}
