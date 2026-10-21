#ifndef PAPACC_MANUAL_PACKAGE_VALIDATION_H
#define PAPACC_MANUAL_PACKAGE_VALIDATION_H
#include "manual_package_crypto.h"
/* Synchronous OFFLINE component, not linked to runtime/CLI/Browser.
 * No supplied boolean/callback/fingerprint can create productive trust.
 * Existing policy-delegation/independent-attestation source is UNBOUND: DENY.
 * Caller existing owner serializes and holds immutable input/password/struct,
 * crypto context, all disjoint buffers and operation through factual finish.
 * Never call synchronous KDF on owner Stop path. Numeric sizes below are
 * accounting inputs, NOT grants of credit or provider qualification. */
#define MP_AUTH_UNKNOWN_BYTES ((size_t)-1)
typedef enum MP_AUTH_STAGE {
 MP_AUTH_NONE=0,MP_AUTH_STRUCTURE,MP_AUTH_BUDGET,MP_AUTH_SIGNATURE,
 MP_AUTH_TRUST,MP_AUTH_KDF,MP_AUTH_AEAD,MP_AUTH_CONTENT,
 MP_AUTH_RECHECK_TRUST,MP_AUTH_RELEASE
} MP_AUTH_STAGE;
typedef enum MP_AUTH_RESULT {
 MP_AUTH_VALIDATED=1, /* unreachable until factual source/content adapters exist */
 MP_AUTH_SYNTHETIC_ONLY=2, /* only separately compiled test implementation */
 MP_AUTH_DENIED=-10,MP_AUTH_STOPPED=-11,MP_AUTH_RETAINED=-12,
 MP_AUTH_INVALID=-13,MP_AUTH_LIMIT=-14,MP_AUTH_ABORTED=-15
} MP_AUTH_RESULT;
typedef struct MP_AUTH_INPUT {
 MP_CRYPTO *crypto;
 MP_SLICE package,password;
 unsigned char *quarantine,*staging,*output;
 size_t quarantine_capacity,staging_capacity,output_capacity;
 size_t other_reserved,provider_reserved;
 MP_CANCEL stopped;void *stop_context;
} MP_AUTH_INPUT;
typedef struct MP_AUTH_OPERATION {
 /* Zero-init once. No retry/reinitialization while retained or borrowed.
  * Internal fields are not a serialized format or authority evidence. */
 unsigned state; /*0 unused,1 running,2 retained,3 complete/aborted */
 MP_AUTH_STAGE stage,first_failure_stage;
 int first_failure_code;
 size_t staging_size,output_size,managed_estimate;
 unsigned kdf_invocations,aead_invocations,content_invocations;
 unsigned char key[32];
 MP_ENVELOPE envelope;MP_PAYLOAD payload; /* durable metadata for retained borrowers */
 const MP_AUTH_INPUT *input;
 const void *internal_port;void *internal_context;
} MP_AUTH_OPERATION;
/* Output remains untouched on every denial/Stop/unknown. Private quarantine
 * and staging are never exposed by API; views during CONTENT expire at return.
 * Current productive binding always denies at TRUST (or earlier).
 * Argument/budget rejection acquires no workspace; caller owns prior contents. */
MP_AUTH_RESULT mp_package_validate_authenticated(MP_AUTH_OPERATION *,const MP_AUTH_INPUT *);
/* Query factual quiescence only; never reissue step/close, decrypt or release.
 * RETAINED holds input/password/key/staging/context/credit until confirmed.
 * Finish discards uncertain result; never publishes a late plaintext result. */
MP_AUTH_RESULT mp_package_validation_finish(MP_AUTH_OPERATION *);
size_t mp_package_validation_process_limit(void);
#endif
