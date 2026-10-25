/* Extracted verbatim after newline normalization; the caller models prior success and goto done. */
static DWORD test_after_initial_success(int positive,int n0)
{DWORD e=0;
 if(positive&&n0){
  cycle_stage="OPERATOR_REFERENCE_FINISH";
  if(!lab_token){e=ERROR_INVALID_STATE;goto done;}
  if(!CloseHandle(lab_token)){e=lab_error();a1_retain(&lab_aux_owner,e);(void)lab_aux_sync(e);goto done;}
  lab_token=NULL;
 }
 done:return e;
}
