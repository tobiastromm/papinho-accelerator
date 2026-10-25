#include <windows.h>
#include <stdio.h>
#include "../tools/qualification/scm-console-proof-model.h"
#define T(v) do{if(!(v)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void){IG_CP_FACTS f={1,1,1,1,1,1,1,0,0};unsigned n=0;T(ig_cp_window_candidate(&f));++n;
f.window_owner_matches=0;T(!ig_cp_window_candidate(&f));++n;f.window_owner_matches=1;
f.identity_stable=0;T(!ig_cp_window_candidate(&f));++n;f.identity_stable=1;
f.image_stable=0;T(!ig_cp_window_candidate(&f));++n;f.image_stable=1;
f.session_equal=0;T(!ig_cp_window_candidate(&f));++n;f.session_equal=1;
f.window_stable=0;T(!ig_cp_window_candidate(&f));++n;f.window_stable=1;
f.members_before=0;T(!ig_cp_window_candidate(&f));++n;f.members_before=1;
f.members_after=0;T(!ig_cp_window_candidate(&f));++n;f.members_after=1;
f.stopped=1;T(!ig_cp_window_candidate(&f));++n;f.stopped=0;
f.retained=1;T(!ig_cp_window_candidate(&f));++n;
printf("CONSOLE_PROOF_MODEL=PASS cases=%u; WINDOW_CANDIDATE_ONLY; AUTHENTICATED_ASSOCIATION_UNKNOWN; NO_ADMISSION_NO_SCM\n",n);return 0;}
