/* M3 DATA validation only, never factual issuer/trust/CONTENT/RELEASE.
 * Input is an already authenticated EXCLUSIVE PRIVATE quarantine of the
 * existing owner. Views borrow it; end all views before wipe/free/reuse.
 * No allocation/I/O, store, owner, callback borrower or installation authority.
 * Failure/Stop leaves output unchanged. All outputs disjoint from inputs.
 */
#ifndef PAPINHO_PAPACC_CONTENT_H
#define PAPINHO_PAPACC_CONTENT_H
#include <stddef.h>
#include "papinho_papacc_public.h"
typedef struct papinho_papacc_slice {const unsigned char *data;size_t size;} papinho_papacc_slice;
typedef struct papinho_papacc_content_view {papinho_papacc_slice member[5];} papinho_papacc_content_view;
enum {PAPINHO_PAPACC_CONTENT_INVALID=0,PAPINHO_PAPACC_CONTENT_DATA_ONLY=1,
      PAPINHO_PAPACC_CONTENT_STOPPED=-1};
int papinho_papacc_content_inspect(const unsigned char *,size_t,
    papinho_papacc_content_view *,papinho_papacc_stopped,void *);
/* present-empty issuance tag5: nonNULL data,size0; absent/invalid: NULL,size0 */
papinho_papacc_slice papinho_papacc_content_field(const papinho_papacc_content_view *,int,unsigned);
/* Maps ONLY derivable fields at accepted provenance record offsets0..319.
 * Does not create a record/generation/admin/consent, or grant trust. Missing
 * fields320..1535 remain the caller's responsibility; never invent their facts.
 * Verifies CA/client/issuance hashes, not signer certificate/issuer status.
 */
int papinho_papacc_content_provenance_prefix(const papinho_papacc_content_view *,
    const papinho_papacc_snapshot *,unsigned char [320]);
#endif
