#ifndef PAPACC_ENROLLMENT_ISSUER_H
#define PAPACC_ENROLLMENT_ISSUER_H
#include "enrollment.h"
#include "../issuer/issuer_core.h"
/* Existing serialized owner only. No owner, approval, listener or authority
 * factory. Requires a pre-existing authorized CLIENT record in NORMAL root.
 * The trace/pair and quarantine stay immutable/alive through PENDING/RETAINED.
 * No recipient output is exposed until explicit publication and revalidation. */
PAPACC_ISSUER_RESULT ep_issuer_submit(PAPACC_ISSUER_CORE *,const EP_TRACE *,const EP_PAIR *,PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_ISSUER_RESULT ep_issuer_poll(PAPACC_ISSUER_CORE *,const EP_TRACE *,const EP_PAIR *,PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_ISSUER_RESULT ep_issuer_publish(PAPACC_ISSUER_CORE *,const EP_TRACE *,const EP_PAIR *,PAPACC_U64,PAPACC_U64,PAPACC_U64);
PAPACC_ISSUER_RESULT ep_issuer_deliver(PAPACC_ISSUER_CORE *,EP_TRACE *,const EP_PAIR *,PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *);
#endif
