/* Private C89 E0-E8 revision 2 interface, not wire or a binary ABI.
 * Objects may only originate in reviewed factual existing-owner bindings.
 * No constructors, layouts, decoded-data conversion or caller callbacks.
 * Current build has NO producer: trust_unavailable.c always denies acquire.
 * No positive acquire/revalidate/release qualification is implied.
 */
#ifndef PAPACC_SHARED_TRUST_H
#define PAPACC_SHARED_TRUST_H
#define SP_TRUST_CONTRACT_REVISION 2
#include <stddef.h>
typedef struct SP_TRUST_PACKAGE SP_TRUST_PACKAGE;
typedef struct SP_ADMIN_OBSERVATION SP_ADMIN_OBSERVATION;
typedef struct SP_INDEPENDENT_CONFIRMATION SP_INDEPENDENT_CONFIRMATION;
typedef struct SP_TRUST_LEASE SP_TRUST_LEASE;
typedef enum SP_TRUST_PHASE {
 SP_TRUST_BEFORE_KDF=1, SP_TRUST_BEFORE_RELEASE=2
} SP_TRUST_PHASE;
typedef enum SP_TRUST_RESULT {
 SP_TRUST_APPLICABLE=1, SP_TRUST_DENIED=-1,
 SP_TRUST_RETAINED=-2, SP_TRUST_INVALID=-3
} SP_TRUST_RESULT;
/* Synchronous borrow-only; no native acquisition, allocation or pending work.
 * Success requires complete prior factual protection by the original owners.
 * Denied/invalid: output untouched, no ownership effects. Never RETAINED.
 * Output must be valid/disjoint; no lease is issued by the unavailable binding.
 */
SP_TRUST_RESULT sp_trust_acquire(
 const SP_TRUST_PACKAGE *,const SP_ADMIN_OBSERVATION *,
 const SP_INDEPENDENT_CONFIRMATION *,const SP_TRUST_LEASE **);
/* Known producer borrows only; never pass forged/stale/arbitrary pointers.
 * Revalidation does not release dependencies. RETAINED prohibits M1 advance.
 */
SP_TRUST_RESULT sp_trust_revalidate(const SP_TRUST_LEASE *,SP_TRUST_PHASE);
typedef enum SP_TRUST_RELEASE_RESULT {
 SP_TRUST_RELEASED=1, SP_TRUST_RELEASE_RETAINED=-1,
 SP_TRUST_RELEASE_INVALID=-2
} SP_TRUST_RELEASE_RESULT;
/* INVALID = no attempt/effect, ownership and known borrow preserved.
 * After an effective attempt: RELEASED or RETAINED only. RETAINED permits
 * query only, never a second release; no timeout/Stop implies closure.
 * After RELEASED invalidate alias once and do not query again.
 */
SP_TRUST_RELEASE_RESULT sp_trust_release(const SP_TRUST_LEASE *);
SP_TRUST_RELEASE_RESULT sp_trust_release_query(const SP_TRUST_LEASE *);
#endif
