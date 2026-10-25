/* Positive pre-metric census only: borrow stable content, never volatile facts.
 * No content observation or lifetime clock is renewed during enumeration. */
static DWORD ig_project_census(void)
{
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 IG_N0_CENSUS_CONTENT scope;DWORD e,final_check=0;ULONGLONG began;
 if(ig_n0_census_content)return ERROR_INVALID_STATE;
 if(ig_n0_positive&&native.admission.bd.bound&&native.admission.bd.binding.n0&&!ig_n0.metric_window){
  if((e=lab_aux_gate()))return e;
  began=GetTickCount64();
  e=papacc_server_service_admission_bd_revalidate(native.admission.bd.check);if(e)return e;
  scope.descriptor=&native.admission.bd.binding;scope.began=ig_n0.began;
  scope.validated=ig_n0.validated_at;scope.challenge=native.admission.bd.binding.challenge;
  scope.thread=GetCurrentThreadId();scope.calls=0;
  ig_n0_census_content=&scope;
  e=ig_project_census_body();
  ig_n0_census_content=NULL; /* no scope survives error, close uncertainty or return */
  if(!e){final_check=1;e=papacc_server_service_admission_bd_revalidate(native.admission.bd.check);}
#ifdef PAPACC_N0_CENSUS_TIMING
  printf("IG_CENSUS_CONTENT_WINDOW calls=%lu full_boundary_checks=%lu elapsed_ms=%llu oldest_age_ms=%llu first=%lu; VOLATILE_CHECKS_PER_ENTRY; NO_FRESHNESS_RESET_NO_CREDIT\n",
   (unsigned long)scope.calls,(unsigned long)(1U+final_check),GetTickCount64()-began,GetTickCount64()-scope.began,(unsigned long)e);
#else
  (void)began;(void)final_check;
#endif
  return e;
 }
#endif
 return ig_project_census_body();
}
