#ifndef PAPACC_INTERNAL_C2_TOKENS_WIN32_H
#define PAPACC_INTERNAL_C2_TOKENS_WIN32_H
#include "c2_session.h"
#include "c2_codec.h"
typedef struct PAPACC_C2_TOKENS {
 const PAPACC_C2_SESSION *session;
 PAPACC_U8 identity[32],bytes[2][32];
 PAPACC_U64 channels[2],incarnation,principal,policy_epoch,expires_ms;
 PAPACC_U32 owner;
 PAPACC_BOOL generated,grant_written,claimed[2],ack_written[2],revoked,retained;
 PAPACC_SIZE borrowers;
} PAPACC_C2_TOKENS;
/* No wire input supplies the authenticated Principal/Session context. */
PAPACC_RESULT papacc_c2_tokens_generate(PAPACC_C2_TOKENS*,const PAPACC_C2_SESSION*,PAPACC_U32,const PAPACC_U8*,const PAPACC_C2_TOKENS*,PAPACC_SIZE);
PAPACC_RESULT papacc_c2_tokens_grant_written(PAPACC_C2_TOKENS*,PAPACC_U32);
PAPACC_RESULT papacc_c2_tokens_claim(PAPACC_C2_TOKENS*,PAPACC_U32,PAPACC_U64,PAPACC_U64,PAPACC_U64,const PAPACC_U8*,PAPACC_SIZE);
PAPACC_RESULT papacc_c2_tokens_ack_written(PAPACC_C2_TOKENS*,PAPACC_U32,unsigned,PAPACC_U64);
PAPACC_RESULT papacc_c2_tokens_borrow(PAPACC_C2_TOKENS*,PAPACC_U32);
PAPACC_RESULT papacc_c2_tokens_quiescent(PAPACC_C2_TOKENS*,PAPACC_U32);
PAPACC_RESULT papacc_c2_tokens_revoke(PAPACC_C2_TOKENS*,PAPACC_U32,PAPACC_BOOL);
#ifdef PAPACC_C2_TEST_ONLY_FAULTS
typedef PAPACC_RESULT (*PAPACC_C2_TEST_RNG)(PAPACC_U8*,PAPACC_SIZE);
void papacc_c2_tokens_test_rng(PAPACC_C2_TEST_RNG);
#endif
#endif
