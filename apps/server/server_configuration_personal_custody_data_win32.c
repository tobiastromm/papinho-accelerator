#include "server_configuration_commit_win32.h"
/* Projection of existing owners only: no owner copy, acquisition, handle
 * dereference, validation, close, retry, authority or native creation. */
PAPACC_PERSONAL_CUSTODY_DATA papacc_configuration_personal_custody_data(
 const PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CA_KEY *ca)
{
 PAPACC_PERSONAL_CUSTODY_DATA q;
 const PAPACC_CONFIG_PENDING *p;
 q.target=PAPACC_PERSONAL_CUSTODY_UNAVAILABLE;
 q.ca=PAPACC_PERSONAL_CUSTODY_UNAVAILABLE;q.ceremony_phase=0U;q.effect_denied=1U;
 if(!o||o->writer!=GetCurrentThreadId())return q;
 p=&o->pending_commit;q.ceremony_phase=p->personal_data.phase;
 q.target=PAPACC_PERSONAL_CUSTODY_ABSENT;
 if(p->bootstrap_created||p->bootstrap_container||p->bootstrap_ready)
  q.target=PAPACC_PERSONAL_CUSTODY_METADATA;
 if(p->personal_ca||p->active||p->thread||p->close_error||o->unresolved||
    p->result==PAPACC_CAS_UNKNOWN||p->personal_native.phase==2U)
  q.target=PAPACC_PERSONAL_CUSTODY_RETAINED;
 /* A stopped owner is still inspected for retention; Stop is not release.
  * A foreign-thread key owner is not inspected further or borrowed. */
 q.ca=PAPACC_PERSONAL_CUSTODY_ABSENT;
 if(ca){
  if(ca->thread&&ca->thread!=GetCurrentThreadId())q.ca=PAPACC_PERSONAL_CUSTODY_UNAVAILABLE;
  else if(ca->state==PAPACC_CA_KEY_RETAINED||ca->retained_token||
    ca->retained_local[0]||ca->retained_local[1]||ca->retained_local[2]||
    (ca->close_attempted&&!ca->close_confirmed)||ca->borrower)
   q.ca=PAPACC_PERSONAL_CUSTODY_RETAINED;
  else if(ca->state!=PAPACC_CA_KEY_EMPTY||ca->provider||ca->key||ca->created)
   q.ca=PAPACC_PERSONAL_CUSTODY_METADATA;
 }
 return q; /* No combination of metadata permits an effect. */
}
