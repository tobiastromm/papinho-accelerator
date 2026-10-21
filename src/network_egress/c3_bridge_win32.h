#ifndef PAPACC_INTERNAL_C3_BRIDGE_WIN32_H
#define PAPACC_INTERNAL_C3_BRIDGE_WIN32_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include "connection.h"
#include "pst_secure_transport_adapter.h"
#define PAPACC_C3_QUEUE_CAPACITY 65536U
#define PAPACC_C3_SLICE 16384U
typedef struct PAPACC_C3_QUEUE { PAPACC_U8 bytes[PAPACC_C3_QUEUE_CAPACITY];PAPACC_SIZE size;PAPACC_U64 received,accepted; } PAPACC_C3_QUEUE;
typedef struct PAPACC_C3_BRIDGE {
 DWORD owner_thread;PAPACC_U64 operation,generation,trace_sequence;
 PAPACC_SIZE child_identity_value,peer_identity_value;
 SOCKET peer,setup_connector,setup_listener;
 pst_transport *unconsumed_transport; pst_external_source *peer_source;PAPACC_BOOL peer_registered,ab_read_paused,c7_hold_ba;pst_u32 peer_interest;
 PAPACC_BOOL setup_connector_close_attempted,setup_listener_close_attempted;
 PAPACC_TRANSPORT_CONNECTION transport;
 PAPACC_PST_SECURE_PROCESSOR processor;
 PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter;
 PAPACC_PST_SECURE_SCHEDULER *scheduler;
 PAPACC_CONNECTION_MANAGER *manager;PAPACC_CONNECTION *connection;
 PAPACC_C3_QUEUE ba,ab;
 PAPACC_U8 expected_name[253];PAPACC_SIZE expected_name_size;
 PAPACC_U8 alpn_offers[256],selected_alpn[255];PAPACC_SIZE alpn_offer_bytes,selected_alpn_size;
 PAPACC_BOOL require_alpn,allow_empty_alpn;
 PAPACC_BOOL tls_verified,child_acquired,ba_input_ended;
 PST_PEER_INFO_SUMMARY tls_facts;
 PAPACC_BOOL initialized,bound,registered,published,tls_complete,ba_frozen,ba_ack_written,child_destroyed,ab_eof,ab_sealed,revoked,retained,peer_close_attempted;
 PST_RESULT source_result,first_failure;PST_RELEASE_INFO release_info;
} PAPACC_C3_BRIDGE;
/* Runtime/credentials/WinSock and manager/scheduler are borrowed from one durable
 * owner. Bridge must remain alive until close confirms success; retention never
 * permits destroying dependencies. No socket value crosses the PACC wire. */
PAPACC_RESULT papacc_c3_bridge_init(PAPACC_C3_BRIDGE*,pst_runtime*,const PST_CONNECTION_CONFIG*,PAPACC_PST_SECURE_SCHEDULER*,PAPACC_CONNECTION_MANAGER*,PAPACC_U64,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_c3_bridge_publish(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_accept_ba(PAPACC_C3_BRIDGE*,PAPACC_U64,PAPACC_U64,PAPACC_U64,const PAPACC_U8*,PAPACC_SIZE);
PAPACC_RESULT papacc_c3_bridge_step(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_end_ba_input(PAPACC_C3_BRIDGE*,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_c3_bridge_commit_ab(PAPACC_C3_BRIDGE*,PAPACC_U64,PAPACC_U64,PAPACC_SIZE);
PAPACC_RESULT papacc_c3_bridge_shutdown_once(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_freeze_ba(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_ba_ack_written(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_release_child(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_seal_ab(PAPACC_C3_BRIDGE*,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_c3_bridge_revoke(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_close_peer(PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_c3_bridge_reset_destroyed(PAPACC_C3_BRIDGE*);
#endif
