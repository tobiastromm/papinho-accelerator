#include "server_run_win32.h"
#include "server_secure_io_win32.h"
#include "server_egress_source_win32.h"
#include "server_outbound_tls_source_win32.h"
#include "server_security_source_win32.h"
#include "server_c2_participant_win32.h"
#include "server_application_lifetime_win32.h"
#include <stdio.h>
#include "c3_bridge_win32.h"
int main(void)
{
 const PAPACC_SIZE n=PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY,l=1U,w=n+l+PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
 PAPACC_SIZE root,arrays,secure_arrays,participants;
 root=sizeof(PAPACC_SERVER_NETWORK)+sizeof(PAPACC_SERVER_CONSOLE_WIN32)+sizeof(PAPACC_LOGGER)+sizeof(PAPACC_SERVER_CONFIG)+
 sizeof(PAPACC_SERVER_ACCEPTOR_WIN32)+sizeof(PAPACC_SERVER_IO_LOOP_WIN32)+sizeof(PAPACC_SERVER_EGRESS_SOURCE_WIN32)+
 sizeof(PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE)+sizeof(PAPACC_PST_LOG_ADAPTER)+sizeof(pst_runtime *)+sizeof(pst_trust *)+
 sizeof(PAPACC_OUTBOUND_TLS_POLICY)+sizeof(PAPACC_SERVER_LISTENER_CONFIGURATION)+sizeof(PAPACC_SERVER_LISTENER_CONFIGURATION_SET)+
 sizeof(PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION)+sizeof(PAPACC_SECURITY_COMPOSITION)+sizeof(PAPACC_SERVER_SECURE_IO_WIN32)+
 11U*sizeof(void *)+papacc_application_lifetime_storage_size()+sizeof(long);
 arrays=n*(sizeof(PAPACC_CONNECTION)+sizeof(PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)+sizeof(PAPACC_SESSION)+
 sizeof(PAPACC_CHANNEL)+sizeof(PAPACC_DATA_ASSOCIATION_ENTRY)+sizeof(PAPACC_SERVER_PROTOCOL_SLOT_WIN32));
 secure_arrays=n*sizeof(PAPACC_SERVER_SECURE_CANDIDATE_WIN32)+w*(sizeof(PAPACC_PST_SCHEDULER_MEMBER)+sizeof(PST_WAIT_EVENT))+
 l*(sizeof(pst_external_source *)+sizeof(pst_wait_token)+sizeof(PAPACC_TCP_LISTENER_ENTRY_WIN32));
 participants=sizeof(PAPACC_SERVER_C2_PARTICIPANT)+sizeof(PAPACC_SECURITY_SOURCE_TRANSACTION);
 printf("LAYOUT root_static_requested=%llu base_arrays=%llu secure_arrays_one_listener=%llu dormant_participants=%llu material_credit=32768 lifetime_metadata=%llu config=%llu loop=%llu acceptor=%llu secure=%llu slot=%llu candidate=%llu\n",
 (unsigned long long)root,(unsigned long long)arrays,(unsigned long long)secure_arrays,(unsigned long long)participants,
 (unsigned long long)papacc_application_lifetime_storage_size(),(unsigned long long)sizeof(PAPACC_SERVER_CONFIG),
 (unsigned long long)sizeof(PAPACC_SERVER_IO_LOOP_WIN32),(unsigned long long)sizeof(PAPACC_SERVER_ACCEPTOR_WIN32),
 (unsigned long long)sizeof(PAPACC_SERVER_SECURE_IO_WIN32),(unsigned long long)sizeof(PAPACC_SERVER_PROTOCOL_SLOT_WIN32),
 (unsigned long long)sizeof(PAPACC_SERVER_SECURE_CANDIDATE_WIN32));
 printf("REQUESTED_PROFILE_SUM=%llu LIMIT=2097152 PROVIDER_KERNEL=UNKNOWN STACK_ALLOCATOR=NOT_MEASURED\n",
 (unsigned long long)(root+arrays+secure_arrays+participants+32768U));
 printf("COMPONENT_C3_BRIDGE_SIZE=%llu FUTURE_TWO_BRIDGES=%llu NOT_ALLOCATED_BY_CURRENT_ROOT=YES\n",
 (unsigned long long)sizeof(PAPACC_C3_BRIDGE),(unsigned long long)(2U*sizeof(PAPACC_C3_BRIDGE)));
 return 0;
}