/* Public M2 framing only. This is NOT authenticated package validation.
 * No crypto, trust, password, plaintext, persistence or installation authority.
 * Caller owns immutable input for the synchronous call. Output is disjoint.
 */
#ifndef PAPINHO_PAPACC_FRAME_H
#define PAPINHO_PAPACC_FRAME_H
#include <stddef.h>
typedef struct papinho_papacc_frame {
    size_t spki_offset,spki_length,ciphertext_offset,ciphertext_length;
    size_t tag_offset,signature_offset;
} papinho_papacc_frame;
/* 1 means only outer framing matches M2; SPKI/hash/signature remain UNVERIFIED.
 * Success supplies offsets, not a borrowed view or usable confidential result.
 * Failure leaves output unchanged. No allocation, I/O or ownership transfer.
 */
int papinho_papacc_frame_read(const unsigned char *,size_t,papinho_papacc_frame *);
#endif
