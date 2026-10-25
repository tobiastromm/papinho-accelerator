/* Bounded, read-only-only attribution in SAME observer/owner. No new authority. */
#ifdef PAPACC_READONLY_HANDLE_TABLE_TRACE
#if !defined(PAPACC_N0START03_READONLY_PINS) || defined(PAPACC_N0_POSITIVE_PINNED_LAB)
#error Handle_table_requires_exact_successor_readonly_pins_and_forbids_positive
#endif
static unsigned ig_table_owner(HANDLE h)
{
 HANDLE v[]={ig_approval_file,lab_parent,lab_config,lab_token,native.image,cycle_source_file,cycle_dependencies[0],cycle_dependencies[1],native.admission.mutex,native.admission.launcher,native.admission.token,native.admission.bd.thread,native.admission.bd.self,native.admission.bd.auxiliary,native.admission.bd.file,native.admission.bd.aux_token,ig_n0.dossier,ig_n0.evidence,ig_n0.policy[0],ig_n0.policy[1],ig_n0.policy[2],ig_n0.policy[3],ig_n0.input_root[0],ig_n0.input_root[1],ig_n0.input_root[2],ig_n0.input_root[3],ig_n0.input_root[4]};unsigned i;
 for(i=0;i<sizeof(v)/sizeof(v[0]);i++)if(h&&h==v[i])return i+1;return 0;
}
static DWORD ig_table_trace(unsigned phase)
{
 PSS_HANDLE_ENTRY entry;FILETIME creation,exit,kernel,user;DWORD before=0,captured=0,marker=0,after=0,e=0,z,rows=0,flags;BYTE digest[32];unsigned j;BOOL live;
 if(native.retained||ig_observer.retained||ig_observer.snapshot||ig_observer.marker||ig_observer.thread)return ERROR_INVALID_STATE;
 if((e=lab_aux_gate()))return e;
 ig_diagnostic.current_role=PS_BD_QUALIFIER;ig_diagnostic.current_pid=GetCurrentProcessId();
 if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;
 if(!GetProcessTimes(GetCurrentProcess(),&creation,&exit,&kernel,&user))return GetLastError();
 if(!GetProcessHandleCount(GetCurrentProcess(),&before))return GetLastError();
 /* No change of guard: deny this optional acquisition if its known original
  * reference cannot fit; unknown internal peaks remain explicitly unknown. */
 if(before>=128U)return ERROR_NOT_ENOUGH_QUOTA;
 printf("IG_HT_BEGIN p=%u pid=%lu creation=%lu:%lu pre=%lu stdin=%llu stdout=%llu stderr=%llu; CURRENT_PROCESS_TABLE_ONLY; AFTER_CRT_NOT_BIRTH\n",phase,(unsigned long)GetCurrentProcessId(),(unsigned long)creation.dwHighDateTime,(unsigned long)creation.dwLowDateTime,(unsigned long)before,(ULONGLONG)(ULONG_PTR)GetStdHandle(STD_INPUT_HANDLE),(ULONGLONG)(ULONG_PTR)GetStdHandle(STD_OUTPUT_HANDLE),(ULONGLONG)(ULONG_PTR)GetStdHandle(STD_ERROR_HANDLE));
 e=PssCaptureSnapshot(GetCurrentProcess(),(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_HANDLES|PSS_CAPTURE_HANDLE_BASIC_INFORMATION|PSS_CAPTURE_HANDLE_NAME_INFORMATION|PSS_CAPTURE_HANDLE_TYPE_SPECIFIC_INFORMATION),0,&ig_observer.snapshot);
 if(e){if(ig_observer.snapshot){ig_observer.retained=1;native.retained=1;ig_diag_note(cycle_stage,"PssCaptureSnapshot","contradictory_table_output",e,e,GetCurrentProcessId(),4);return e;}goto done;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&captured)){e=GetLastError();goto done;}if(captured>128U){e=ERROR_NOT_ENOUGH_QUOTA;goto done;}
 e=PssWalkMarkerCreate(NULL,&ig_observer.marker);if(e){if(ig_observer.marker){ig_observer.retained=1;native.retained=1;ig_diag_note(cycle_stage,"PssWalkMarkerCreate","contradictory_table_marker",e,e,GetCurrentProcessId(),4);return e;}goto done;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&marker)){e=GetLastError();goto done;}if(marker>128U){e=ERROR_NOT_ENOUGH_QUOTA;goto done;}
 for(;;){
  if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;break;}
  ZeroMemory(&entry,sizeof(entry));e=PssWalkSnapshot(ig_observer.snapshot,PSS_WALK_HANDLES,ig_observer.marker,&entry,sizeof(entry));
  if(e==ERROR_NO_MORE_ITEMS){e=0;break;}if(e)break;
  if(++rows>128U){e=ERROR_NOT_ENOUGH_QUOTA;break;}
  flags=0;live=GetHandleInformation(entry.Handle,&flags);
  printf("IG_HT_ROW p=%u h=%llu t=%u k=%u a=%lu access=%lu live=%u f=%lu owner=%u type=",phase,(ULONGLONG)(ULONG_PTR)entry.Handle,(unsigned)entry.ObjectType,(unsigned)entry.Flags,(unsigned long)entry.Attributes,(unsigned long)entry.GrantedAccess,(unsigned)live,(unsigned long)flags,ig_table_owner(entry.Handle));
  for(j=0;(entry.Flags&PSS_HANDLE_HAVE_TYPE)&&entry.TypeName&&j<entry.TypeNameLength/sizeof(WCHAR)&&j<16;j++)printf("%04x",(unsigned)(unsigned short)entry.TypeName[j]);
  printf(" name_sha=");if((entry.Flags&PSS_HANDLE_HAVE_NAME)&&entry.ObjectName&&entry.ObjectNameLength){if(!pacc_storage_hash_callback(NULL,(const BYTE *)entry.ObjectName,entry.ObjectNameLength,digest)){e=ERROR_INVALID_DATA;break;}for(j=0;j<32;j++)printf("%02x",(unsigned)digest[j]);}else printf("UNKNOWN");
  printf("; IDENTITY=UNPROVEN\n");
 }
 done:
 if(e)ig_diag_note(cycle_stage,"PssHandleTable","readonly_attribution",e,e,GetCurrentProcessId(),4);
 z=ig_observer_close_snapshot();if(z){native.retained=1;return e?e:z;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&after))return e?e:GetLastError();
 printf("IG_HT_FINAL p=%u first=%lu pre=%lu capture=%lu marker=%lu after=%lu rows=%lu entry_bytes=%llu; UNSAMPLED_PEAK=UNKNOWN; NO_CREDIT\n",phase,(unsigned long)e,(unsigned long)before,(unsigned long)captured,(unsigned long)marker,(unsigned long)after,(unsigned long)rows,(ULONGLONG)sizeof(entry));
 if(after>128U&&!e)e=ERROR_NOT_ENOUGH_QUOTA;
 return e;
}
#endif
