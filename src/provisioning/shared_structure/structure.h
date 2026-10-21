/* Complementary provisioning DATA API, private extraction slice, no release name/version.
 * No crypto/trust/root/issuer/PST/UI/I/O/allocation or installation authority.
 * Caller serializes synchronous calls; inputs immutable; writable outputs disjoint.
 * Returned views BORROW caller input. Hold backing and its owner/lease until all
 * users finish, including Stop/retention. No callback may free/reenter/acquire borrows.
 * Stop is observed only at bounded checkpoints; not native cancellation/closure.
 * No internal retained resources. Caller wipes only quiescent owned secrets.
 */
#ifndef SP_STRUCTURE_H
#define SP_STRUCTURE_H
#include <stddef.h>
#include <limits.h>
#if CHAR_BIT != 8
#error Eight-bit bytes required
#endif
#if ULONG_MAX == 0xffffffffUL
typedef unsigned long SP_U32;
#elif UINT_MAX == 0xffffffffU
typedef unsigned int SP_U32;
#else
#error An exact unsigned 32-bit type is required
#endif
typedef char sp_u32_size[(sizeof(SP_U32)==4)?1:-1];
/* Reviewed flat-address targets only; pointer conversions are local, never wire. */
typedef char sp_flat_size[(sizeof(size_t)==sizeof(void *))?1:-1];
#define SP_INPUT_MAX 131072UL
#define SP_ARCHIVE_MAX 49654UL
#define SP_MEMBERS 5U
typedef enum SP_RESULT {SP_DATA_ONLY=1,SP_INVALID=-1,SP_LIMIT=-2,SP_STOPPED=-3} SP_RESULT;
typedef int (*SP_STOP)(void *);
typedef struct SP_SLICE {const unsigned char *data;size_t size;} SP_SLICE;
typedef struct SP_PAYLOAD {SP_SLICE member[5];} SP_PAYLOAD;
typedef struct SP_ENVELOPE {SP_SLICE spki,ciphertext,tag,signature;} SP_ENVELOPE;
/* Success is STRUCTURAL DATA only. No SPKI/RSA/AEAD/X509/profile validation.
 * On every error/Stop all output structs stay unchanged. Invalid output/alias
 * preconditions do not modify caller storage. No partial usable result. */
SP_RESULT sp_envelope_inspect(const unsigned char *,size_t,SP_ENVELOPE *,SP_STOP,void *);
SP_RESULT sp_payload_inspect(const unsigned char *,size_t,SP_PAYLOAD *,SP_STOP,void *);
/* kind=0 config,1 issuance; validates syntax only, not factual identities. */
SP_RESULT sp_metadata_validate(SP_SLICE,unsigned);
/* A field view borrows the already owned metadata; revalidates syntax.
 * Invalid/absent -> NULL,0. Issuance tag5 present-empty -> nonNULL,0. */
SP_SLICE sp_payload_field(const SP_PAYLOAD *,unsigned,unsigned);
/* Normalization is DATA, not KDF or authentication. <=128 input bytes ->32
 * Base32 ASCII. No locale or CSPRNG claim. After valid disjoint output preflight,
 * failure clears output32; aliases/precondition failure leave input/output alone.
 * Copy/wipe policy reused from existing Browser adapter, no new primitive. */
SP_RESULT sp_password_normalize(const unsigned char *,size_t,unsigned char [32]);
#endif
