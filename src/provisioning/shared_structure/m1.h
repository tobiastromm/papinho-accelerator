/* Private portable sequencing metadata, never a trust receipt or public ABI. */
#ifndef PAPACC_SHARED_M1_H
#define PAPACC_SHARED_M1_H
typedef enum SP_M1_STAGE {
 SP_M1_NONE=0,SP_M1_STRUCTURE,SP_M1_BUDGET,SP_M1_SIGNATURE,
 SP_M1_TRUST,SP_M1_KDF,SP_M1_AEAD,SP_M1_CONTENT,SP_M1_RECHECK_TRUST,SP_M1_RELEASE
} SP_M1_STAGE;
typedef struct SP_M1_STATE {
 unsigned state; /*0 unused,1 running,2 retained,3 terminal; never reinitialize live */
 SP_M1_STAGE stage,first_failure_stage;
 int first_failure_code;
 unsigned kdf_invocations,aead_invocations,content_invocations;
 const void *owner_identity,*binding_identity; /* retained provenance of this integration lifetime */
} SP_M1_STATE;
/* Results describe sequence execution, not authority, installation or CURRENT. */
enum {SP_M1_COMPLETE=1,SP_M1_DENIED=-10,SP_M1_STOPPED=-11,
 SP_M1_RETAINED=-12,SP_M1_INVALID=-13,SP_M1_LIMIT=-14,SP_M1_ABORTED=-15};
#endif
