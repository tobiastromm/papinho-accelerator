#ifndef PAPACC_INTERNAL_C2_SESSION_H
#define PAPACC_INTERNAL_C2_SESSION_H
#include <papacc/pacc/capability_negotiation_protocol.h>
typedef enum PAPACC_C2_SESSION_STATE { PAPACC_C2_SESSION_EMPTY=0,PAPACC_C2_SESSION_CONFIGURED,PAPACC_C2_SESSION_RESULT_PENDING,PAPACC_C2_SESSION_ELIGIBLE,PAPACC_C2_SESSION_UNAVAILABLE,PAPACC_C2_SESSION_REVOKED,PAPACC_C2_SESSION_RETAINED } PAPACC_C2_SESSION_STATE;
typedef struct PAPACC_C2_SESSION {
 PAPACC_U64 incarnation,principal,policy_epoch;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 /* Exact existing Resolver identity; scalar principal is not C8 authority. */
 PAPACC_U8 authenticated_principal[16],root_domain[32];
 PAPACC_U64 root_invocation,root_revision,control_connection,control_channel;
 PAPACC_BOOL root_bound;
#endif
 PAPACC_U32 owner;
 PAPACC_U8 assurance;
 PAPACC_BOOL tls_allowed,c3_supported,legacy_lab,initialized;
 PAPACC_C2_SESSION_STATE state;
 PAPACC_CAPABILITY_KEY grants[2];PAPACC_SIZE grant_count;
} PAPACC_C2_SESSION;
/* Internal immutable Session gate; no new public package API. Authentication/policy
 * arguments come only from the real owner's verified context, never peer declarations.
 * Server full result write and Browser full result receipt are separate barriers. */
PAPACC_RESULT papacc_c2_session_init(PAPACC_C2_SESSION*,PAPACC_U32,PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U8,PAPACC_BOOL,PAPACC_BOOL,PAPACC_BOOL);
PAPACC_RESULT papacc_c2_server_negotiate(PAPACC_C2_SESSION*,PAPACC_U32,const PAPACC_U8*,PAPACC_SIZE,PAPACC_U8*,PAPACC_SIZE,PAPACC_SIZE*);
PAPACC_RESULT papacc_c2_server_result_written(PAPACC_C2_SESSION*,PAPACC_U32,PAPACC_BOOL,PAPACC_U64);
PAPACC_RESULT papacc_c2_browser_result_received(PAPACC_C2_SESSION*,PAPACC_U32,const PAPACC_U8*,PAPACC_SIZE,PAPACC_U64);
PAPACC_BOOL papacc_c2_session_authorized(const PAPACC_C2_SESSION*,PAPACC_U32,PAPACC_U64,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_c2_session_revoke(PAPACC_C2_SESSION*,PAPACC_U32,PAPACC_BOOL);
#endif
