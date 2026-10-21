#ifndef PAPACC_MANUAL_PACKAGE_PDG_INTERNAL_H
#define PAPACC_MANUAL_PACKAGE_PDG_INTERNAL_H
#include "manual_package_validation_internal.h"
/* Reject/match candidate DATA after SIGNATURE; even a match is not authority.
 * Fixed native factual-source gate still denies before KDF. No source factory. */
int mp_auth_pdg_candidate(MP_AUTH_OPERATION *,const MP_ENVELOPE *);
#endif
