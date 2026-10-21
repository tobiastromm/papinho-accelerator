/* Browser-private CRYPTO ONLY, not TRUST/CONTENT/RELEASE/installation authority.
 * Operational callers MUST first establish M1 SIGNATURE + factual D3 TRUST.
 * UI currently denies before password/KDF; only isolated tests invoke these
 * building blocks until the separate D3 binding is implemented/reviewed.
 * Synchronous calls: caller exclusively owns all writable memory and keeps
 * immutable inputs alive; stop callbacks cannot reenter or acquire borrows.
 */
#ifndef PAPINHO_PAPACC_CRYPTO_H
#define PAPINHO_PAPACC_CRYPTO_H
#include <stddef.h>
#include "papinho_papacc_public.h"
enum {
    PAPINHO_PAPACC_CRYPTO_INVALID=0,
    PAPINHO_PAPACC_CRYPTO_ONLY=1,
    PAPINHO_PAPACC_CRYPTO_STOPPED=-1,
    PAPINHO_PAPACC_CRYPTO_LOCKED=-2
};
/* Existing non-elidable C89 erasure. Only quiescent, owned valid bytes. */
void papinho_c2_wipe(void *,size_t);
/* <=128 ASCII input bytes -> exactly32 uppercase Base32 bytes (no NUL).
 * Only ASCII space/hyphen stripped, lowercase folded. No locale/ENV/CLI.
 * Disjoint valid output is wiped on normalization failure. */
int papinho_papacc_crypto_password(const unsigned char *,size_t,unsigned char [32]);
/* Exact suite1: normalized32, salt16, iterations600000, output32.
 * Valid disjoint key[32] is zero on failure/Stop. Wrong output shape or aliases
 * are precondition failures: no write, no borrowed input destroyed.
 * No alternate suite, cost tuning, provider fallback or dynamic allocation. */
int papinho_papacc_crypto_derive(const unsigned char *,size_t,
    const unsigned char *,size_t,unsigned long,unsigned char *,size_t,
    papinho_papacc_stopped,void *);
/* RFC8439 nonce12/tag16; ciphertext1..65536, AAD0..1116. Actual package caller
 * must bind exact M2 header+SPKI AAD, same immutable bytes and authority context.
 * q is EXCLUSIVE PRIVATE QUARANTINE, never a public plaintext destination.
 * Output q/used must be disjoint from every input and each other. Bad writable
 * shape/aliases: untouched. Otherwise used=0/q cleared on failure or Stop.
 * CRYPTO_ONLY means tag authenticated, not CONTENT or RELEASE. Keep q private
 * through full content/final authority validation; then existing owner controls
 * release. Wipe q only when its borrows have factually ended, including cancel.
 * No encryption/nonce generation, heap, thread, handle or store ownership here.
 */
int papinho_papacc_crypto_unseal(const unsigned char [32],
    const unsigned char *,size_t,const unsigned char *,size_t,
    const unsigned char *,size_t,const unsigned char *,size_t,
    unsigned char *,size_t,size_t *,papinho_papacc_stopped,void *);
/* CRYPTO ONLY M2 binding for the future already-TRUSTed caller. No authority
 * source/boolean is accepted. NOT called from UI while D3 source is absent.
 * Same immutable captured object; AAD/nonce/tag/salt/iterations come only from
 * its verified M2 bytes, never arbitrary AAD or a replaceable path.
 * failures belongs to the existing UI session, survives file changes and
 * successful crypto, saturates at3. Stop/invalid snapshot are not unlock fails.
 * CRYPTO_ONLY retains q PRIVATE for CONTENT; never calls store/writer/RELEASE. */
int papinho_papacc_crypto_unlock_m2(const papinho_papacc_snapshot *,
    unsigned long *,const unsigned char *,size_t,unsigned char *,size_t,size_t *,
    papinho_papacc_stopped,void *);
#endif
