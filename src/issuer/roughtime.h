#ifndef PAPACC_ROUGHTIME_H
#define PAPACC_ROUGHTIME_H
#include "qualified_utc.h"
#define PAPACC_RT_PACKET 2048U
#define PAPACC_RT_QUERIES 6U
/* Private nonserialized DATA only. No source identity, CURRENT, status,
 * calibration, entropy or clock is qualified by any result/field here.
 * Existing administrative caller owns immutable buffers, backend and refs;
 * serialize calls and retain the entire graph until factual quiescence.
 * No allocation, network, thread, pin, borrow acquisition or release. */
typedef struct PAPACC_RT_BYTES { const PAPACC_U8 *p; PAPACC_SIZE n; } PAPACC_RT_BYTES;
typedef struct PAPACC_RT_CRYPTO {
 void *user;
 /* SHA512 truncated to32, over concatenated slices; Ed25519 exact context
  * including terminating zero + message. Crypto success is not trust. */
 int (*hash)(void *,const PAPACC_RT_BYTES *,unsigned,PAPACC_U8 *);
 int (*verify)(void *,const PAPACC_U8 *,const PAPACC_U8 *,PAPACC_SIZE,
               const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *);
} PAPACC_RT_CRYPTO;
typedef enum PAPACC_RT_RESULT {
 PAPACC_RT_DATA_OK=0,PAPACC_RT_INVALID,PAPACC_RT_LIMIT,
 PAPACC_RT_CRYPTO_UNAVAILABLE,PAPACC_RT_EXPIRED,PAPACC_RT_STOPPED,
 PAPACC_RT_RETAINED,PAPACC_RT_NOT_QUALIFIED
} PAPACC_RT_RESULT;
typedef struct PAPACC_RT_REPLY { PAPACC_U64 midpoint; PAPACC_U32 radius; } PAPACC_RT_REPLY;
/* Output must be disjoint from all inputs/backend state; output untouched
 * on rejection. Success proves public wire/crypto properties only. */
PAPACC_RT_RESULT papacc_rt_merkle_data(const PAPACC_RT_CRYPTO *,PAPACC_RT_BYTES,PAPACC_RT_BYTES,PAPACC_U32,const PAPACC_U8 *);
PAPACC_RT_RESULT papacc_rt_packet_data(PAPACC_RT_BYTES);
PAPACC_RT_RESULT papacc_rt_request_data(const PAPACC_RT_CRYPTO *,const PAPACC_U8 *,const PAPACC_U8 *,PAPACC_U8 *);
PAPACC_RT_RESULT papacc_rt_reply_data(const PAPACC_RT_CRYPTO *,PAPACC_RT_BYTES,PAPACC_RT_BYTES,const PAPACC_U8 *,PAPACC_RT_REPLY *);
PAPACC_RT_RESULT papacc_rt_campaign_data(const PAPACC_RT_CRYPTO *,const PAPACC_RT_BYTES *,const PAPACC_RT_BYTES *,const PAPACC_U8 *,const PAPACC_U8 *,PAPACC_RT_REPLY *);
/* Mathematical bound for one reply at final campaign cut, with qualified
 * inputs supplied as DATA. Caller must independently prove every premise.
 * Never calls papacc_issuer_qualified_utc_read or publishes an observation. */
PAPACC_RT_RESULT papacc_rt_interval_data(const PAPACC_RT_REPLY *,const PAPACC_UTC_OBSERVATION *,PAPACC_U64,PAPACC_U64 *,PAPACC_U64 *);
PAPACC_RT_RESULT papacc_rt_hull_data(const PAPACC_RT_REPLY *,const PAPACC_UTC_OBSERVATION *,PAPACC_U64,PAPACC_U64 *,PAPACC_U64 *);
/* Offline transport-event accounting, not a resource owner or I/O producer.
 * Zero initialize once. Pending is retained after Stop/deadline; native
 * completion/closure belongs to existing owner, never inferred by this API. */
typedef struct PAPACC_RT_EXCHANGE {
 PAPACC_U64 campaign,start,sent,last;
 unsigned phase,stopped,invalid;
} PAPACC_RT_EXCHANGE;
PAPACC_RT_RESULT papacc_rt_exchange_start_data(PAPACC_RT_EXCHANGE *,PAPACC_U64,PAPACC_U64);
PAPACC_RT_RESULT papacc_rt_exchange_sent_data(PAPACC_RT_EXCHANGE *,PAPACC_U64);
PAPACC_RT_RESULT papacc_rt_exchange_received_data(PAPACC_RT_EXCHANGE *,PAPACC_U64);
PAPACC_RT_RESULT papacc_rt_exchange_verified_data(PAPACC_RT_EXCHANGE *,PAPACC_U64);
void papacc_rt_exchange_stop_data(PAPACC_RT_EXCHANGE *);
PAPACC_U32 papacc_rt_backoff_data(unsigned);
#endif
