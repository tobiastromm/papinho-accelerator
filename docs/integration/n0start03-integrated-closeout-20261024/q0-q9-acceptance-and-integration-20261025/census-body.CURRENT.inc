static DWORD ig_project_census_body(void)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 PROCESSENTRY32W pe;DWORD e=0;unsigned j,n0_seen=0;
 ULONGLONG snapshot_begin=0,snapshot_end=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.used){
  if(!native.admission.bd.bound)return ERROR_ACCESS_DENIED;
  e=papacc_server_service_admission_bd_revalidate(native.admission.bd.check);if(e)return e;
  e=ps_bd_inventory(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64());if(e)return e;
 }
#endif
if(ig_guard_snapshot||native.retained)return ERROR_INVALID_STATE;snapshot_begin=GetTickCount64();ig_guard_snapshot=ig_diag_handle(cycle_stage,"CreateToolhelp32Snapshot","process_snapshot",CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0));if(ig_guard_snapshot==INVALID_HANDLE_VALUE){ig_guard_snapshot=NULL;return lab_error();}
 ZeroMemory(&pe,sizeof(pe));pe.dwSize=sizeof(pe);if(!ig_diag_bool(cycle_stage,"Process32FirstW","process_snapshot_entry",Process32FirstW(ig_guard_snapshot,&pe)))return lab_error();
 do{int participant=0,child=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.bound){
  if(native.admission.bd.binding.n0){for(j=0;j<PS_BD_ROLES;j++)if(native.admission.bd.inventory.present[j]&&pe.th32ProcessID==native.admission.bd.inventory.role[j].pid)n0_seen|=1U<<j;}
  e=ps_bd_census(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64(),pe.th32ProcessID,pe.th32ParentProcessID,(unsigned)(!_wcsnicmp(pe.szExeFile,L"papacc",6)));
  if(e){ig_diag_note(cycle_stage,"bd_census_predicate","process_snapshot",e,e,pe.th32ProcessID,4);break;}
  continue;
 }
#endif

 for(j=0;j<4;j++){if(ig_observer.pid[j]&&pe.th32ProcessID==ig_observer.pid[j])participant=1;if(ig_observer.pid[j]&&pe.th32ParentProcessID==ig_observer.pid[j])child=1;}
 if(!participant&&(child||!_wcsnicmp(pe.szExeFile,L"papacc",6))){unsigned k;ig_diag_note(cycle_stage,"process_census_predicate","process_snapshot",ERROR_BUSY,ERROR_BUSY,pe.th32ProcessID,4);printf("IG_CENSUS_REJECT pid=%lu parent=%lu child=%u project_prefix=%u; NO_ADMISSION\n",(unsigned long)pe.th32ProcessID,(unsigned long)pe.th32ParentProcessID,(unsigned)child,(unsigned)(!_wcsnicmp(pe.szExeFile,L"papacc",6)));printf("IG_CENSUS_NAME_UTF16=");for(k=0;k<64U&&pe.szExeFile[k];k++)printf("%04x",(unsigned)(unsigned short)pe.szExeFile[k]);printf("; TRUNCATED=%u; SNAPSHOT_NAME_ONLY_NO_CREATION_PROOF\n",(unsigned)(k==64U&&pe.szExeFile[k]!=0));e=ERROR_BUSY;break;}}while(ig_diag_next(cycle_stage,"Process32NextW","process_snapshot_entry",Process32NextW(ig_guard_snapshot,&pe)));
 if(!e&&GetLastError()!=ERROR_NO_MORE_FILES)e=lab_error();
 snapshot_end=GetTickCount64();
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 if(!e&&native.admission.bd.bound&&native.admission.bd.binding.n0){unsigned expected=0;
 for(j=0;j<PS_BD_ROLES;j++)if(native.admission.bd.inventory.present[j])expected|=1U<<j;
 if(n0_seen!=expected){e=ERROR_INVALID_DATA;ig_diag_note(cycle_stage,"n0_original_roles_missing","process_snapshot",e,e,0,4);
 printf("IG_CENSUS_ROLE_MASK expected=%u seen=%u missing=%u; ORIGINAL_INVENTORY; SNAPSHOT_ONLY_NOT_EXIT_PROOF\n",expected,n0_seen,expected&~n0_seen);
 /* Original borrowed inventory handles only. No PID reopen or census exception. */
 for(j=0;j<PS_BD_ROLES;j++)if((expected&~n0_seen)&(1U<<j)){
  DWORD saved=GetLastError();PT_ROLE q;
  if(lab_aux_gate()){ZeroMemory(&q,sizeof(q));q.result=PT_STOPPED;q.error=ERROR_CANCELLED;}
  else q=pt_role(native.admission.bd.inventory.role[j].handle,native.admission.bd.inventory.role[j].pid,native.admission.bd.inventory.role[j].creation,j,expected,n0_seen,0,snapshot_begin,snapshot_end,GetTickCount64());
  printf("IG_CENSUS_ORIGINAL role=%u expected_pid=%lu expected_creation=%llu observed_pid=%lu observed_creation=%llu result=%lu query_error=%lu wait=%lu begin=%llu end=%llu observed=%llu; AFTER_SNAPSHOT_ONLY; NO_ADMISSION\n",j,(unsigned long)native.admission.bd.inventory.role[j].pid,native.admission.bd.inventory.role[j].creation,(unsigned long)q.pid,q.creation,(unsigned long)q.result,(unsigned long)q.error,(unsigned long)q.wait_result,q.snapshot_begin,q.snapshot_end,q.observed_at);
  SetLastError(saved);
 }
 }}
#else
 (void)n0_seen;(void)snapshot_begin;(void)snapshot_end;
#endif
 if(!ig_diag_bool(cycle_stage,"CloseHandle","process_snapshot_handle",CloseHandle(ig_guard_snapshot))){native.retained=1;return ERROR_INVALID_HANDLE;}ig_guard_snapshot=NULL;
 /* One self query at the post-census/pre-PSS boundary, not admission authority.
  * No real handle acquisition, snapshot, external process or recorder. */
 if(e==ERROR_BUSY){DWORD saved=GetLastError(),handles=0,metric_error=0;BOOL observed;
  observed=GetProcessHandleCount(GetCurrentProcess(),&handles);if(!observed){metric_error=GetLastError();if(!metric_error)metric_error=ERROR_GEN_FAILURE;}
  printf("IG_HANDLE_REQUALIFICATION observed=%u handles=%lu limit=128 metric_error=%lu pid=%lu tick=%llu; POINT=POST_CENSUS_CLOSE_PRE_METRICS_PSS; HANDLE_GATE=NOT_REACHED; FIRST_ERROR_UNCHANGED; NO_ADMISSION\n",(unsigned)observed,(unsigned long)handles,(unsigned long)metric_error,(unsigned long)GetCurrentProcessId(),GetTickCount64());
  SetLastError(saved);
 }
 return e;
}
