/* Accepted00d0cd8d: same packet/primitives, independent utility maintenance.
 * No server target, new owner, retry, rollback or historical-cohort reset. */
size_t papacc_server_launcher_utility_update_known_bytes(void)
{
 /* Applicable shared bootstrap-frame census, plus maintenance coexistence:
  * SCM queries/rename/readback structs and ephemeral verification generation.
  * Own charge, not imported UPDATE K or prior bootstrap capacity. */
 return papacc_server_launcher_utility_prep_known_bytes()+sizeof(QUERY_SERVICE_CONFIGW)+
  sizeof(SERVICE_STATUS_PROCESS)+sizeof(SERVICE_SID_INFO)+sizeof(PAPACC_UPDATE_RENAME_INFO)+
  sizeof(BY_HANDLE_FILE_INFORMATION)+2U*MAX_PATH*sizeof(WCHAR)+32U*sizeof(void *)+32U*sizeof(DWORD);
}
static DWORD utility_update_apply(void)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_QUALIFICATION_STAGING *s=staging_act();
 if(!staged_update_owned()||u->preparation_mode!=2U||u->act_state!=2U||!u->confirmed||u->consumed||u->first)return ERROR_ACCESS_DENIED;
 if(utility_prep_current()||staged_update_resources(u->new_bytes))return u->first;
 if(InterlockedCompareExchange((volatile LONG *)&u->consumed,1,0)!=0)return ERROR_ACCESS_DENIED;u->act_state=3U;
 if(staged_update_create()||staged_binary_copy_and_flush()||staged_update_rename(1U,13U,u->previous)||
  staged_update_readback(15U,u->previous,13U,u->old_hash))return u->first;
 /* Slot15 is an ephemeral verification borrow; original old slot13 stays
  * retained. Retire ONLY this known-closed generation before reusing the
  * bounded slot for canonical readback. No reset or reuse after uncertainty. */
 if(s->close_attempts&(1U<<15U))return staged_update_retain(ERROR_INVALID_HANDLE);
 s->close_attempts|=1U<<15U;
 if(!CloseHandle(s->objects[15].handle))return staged_update_retain(ERROR_INVALID_HANDLE);
 ZeroMemory(&s->objects[15],sizeof(s->objects[15]));s->close_attempts&=~(1U<<15U);
 if(staged_update_rename(2U,14U,L"papacc_update.exe")||staged_update_readback(15U,L"papacc_update.exe",14U,u->new_hash)||utility_prep_current())return u->first;
 return 0;
}
