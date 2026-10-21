/* Mathematical public checks only. Never a TRUST or installation result. */
#ifndef PAPINHO_PAPACC_PUBLIC_H
#define PAPINHO_PAPACC_PUBLIC_H
#include <stddef.h>
typedef int (*papinho_papacc_stopped)(void *);
enum {
    PAPINHO_PAPACC_STRUCTURE_INVALID=0,
    PAPINHO_PAPACC_SIGNATURE_ONLY=1,
    PAPINHO_PAPACC_STOPPED=-1,
    PAPINHO_PAPACC_SIGNATURE_INVALID=-2,
    PAPINHO_PAPACC_READ_FAILED=-3
};
/* Immutable input, synchronous, no retained aliases or confidential output.
 * Stop is checked around public RSA, not a claim of interruptible RSA/I/O. */
int papinho_papacc_public_check(const unsigned char *,size_t,
    papinho_papacc_stopped,void *);
/* DATA embedded in the EXISTING dialog owner. Not a new authority/owner.
 * Initialize to all zero. No copying aliases; caller ends borrows before discard.
 * Only public ciphertext/digests, no credentials/trust assertion. */
typedef struct papinho_papacc_snapshot {
    unsigned char *bytes;
    size_t size;
    unsigned char digest[32],signer[32];
} papinho_papacc_snapshot;
int papinho_papacc_public_capture(const char *,papinho_papacc_snapshot *,
    papinho_papacc_stopped,void *);
void papinho_papacc_public_discard(papinho_papacc_snapshot *);
/* Recheck same immutable bytes/digests/signature, never a file path or TRUST. */
int papinho_papacc_public_snapshot_check(const papinho_papacc_snapshot *,
    papinho_papacc_stopped,void *);
/* Captures bounded bytes once; never rereads a path after verification. */
int papinho_papacc_public_file(const char *,papinho_papacc_stopped,void *);
#endif
