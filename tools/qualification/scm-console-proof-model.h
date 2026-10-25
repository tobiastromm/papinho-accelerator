#ifndef PAPACC_CONSOLE_PROOF_MODEL_H
#define PAPACC_CONSOLE_PROOF_MODEL_H
/* Evidence classification ONLY: never an admission or a fifth role. */
typedef struct IG_CP_FACTS {
 unsigned identity_stable,image_stable,session_equal,members_before,members_after;
 unsigned window_stable,window_owner_matches,stopped,retained;
} IG_CP_FACTS;
static __inline int ig_cp_window_candidate(const IG_CP_FACTS *f)
{return f->identity_stable&&f->image_stable&&f->session_equal&&f->members_before&&f->members_after&&f->window_stable&&f->window_owner_matches&&!f->stopped&&!f->retained;}
#endif
