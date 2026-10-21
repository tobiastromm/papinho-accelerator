/* PRIVATE integration TCB, not a caller-selectable evidence/backend API.
 * Product adapter binding is fixed by reviewed code/build. The application
 * cannot supply this table. Test ports are isolated; completion proves only
 * sequencing. No bool/callback/string/decoded row becomes factual trust.
 * A TRUST adapter must deny without the separately governed factual source.
 * Owner retains state/context/inputs/workspaces/evidence/provider/credits until
 * factual completion. Callbacks synchronous, serialized, no reentry, no state
 * mutation or publication except publish after all checks; stopped observes
 * only. quiescent must cover all children/borrows, not elapsed time or Stop.
 * Retained finish only queries quiescence then wipes; never reissues a step.
 */
#ifndef PAPACC_SHARED_M1_INTERNAL_H
#define PAPACC_SHARED_M1_INTERNAL_H
#include "m1.h"
typedef struct SP_M1_BINDING {
 /* COMPLETE means stage completed by fixed adapter, not generic trust. */
 int (*perform)(void *,SP_M1_STAGE,int *);
 int (*stopped)(void *);
 int (*quiescent)(void *);
 void (*wipe)(void *);
 void (*publish)(void *);
} SP_M1_BINDING;
/* Extracted existing detail codes, not wire or a new evidence format. */
#define SP_M1_CONTENT_PARSE ((SP_M1_STAGE)10) /* internal preflight, not wire or an authority stage */
#define SP_M1_UNCERTAIN_DETAIL (-103)
#define SP_M1_INVALID_DETAIL (-101)
#define SP_M1_CONTENT_STOP_DETAIL (-3)
int sp_m1_execute(SP_M1_STATE *,const SP_M1_BINDING *,void *);
int sp_m1_finish(SP_M1_STATE *,const SP_M1_BINDING *,void *);
#endif
