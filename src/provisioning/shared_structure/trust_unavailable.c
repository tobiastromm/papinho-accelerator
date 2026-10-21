/* Fixed SOURCE_ABSENT binding only. E8 forbids inventing a positive producer.
 * This translation unit defines no opaque layout or lease factory. Replace
 * only through separately reviewed real-owner integration, not a runtime
 * provider/callback registry. It cannot issue APPLICABLE, RETAINED or RELEASED.
 * It neither dereferences opaque inputs nor touches caller/external storage.
 * Resources prepared earlier remain reachable and owned by their real owners;
 * no SDK lease does NOT establish that these resources have been released.
 */
#include "trust.h"
SP_TRUST_RESULT sp_trust_acquire(const SP_TRUST_PACKAGE *p,
 const SP_ADMIN_OBSERVATION *a,const SP_INDEPENDENT_CONFIRMATION *c,
 const SP_TRUST_LEASE **out){
 (void)p;(void)a;(void)c;
 if(!out)return SP_TRUST_INVALID;
 return SP_TRUST_DENIED;
}
SP_TRUST_RESULT sp_trust_revalidate(const SP_TRUST_LEASE *l,SP_TRUST_PHASE phase){
 (void)l;(void)phase;
 /* This binding has never returned a known lease. No discovery by pointer. */
 return SP_TRUST_INVALID;
}
SP_TRUST_RELEASE_RESULT sp_trust_release(const SP_TRUST_LEASE *l){
 (void)l;
 /* No attempt and no effect; no native ownership can be released here. */
 return SP_TRUST_RELEASE_INVALID;
}
SP_TRUST_RELEASE_RESULT sp_trust_release_query(const SP_TRUST_LEASE *l){
 (void)l;
 return SP_TRUST_RELEASE_INVALID;
}
