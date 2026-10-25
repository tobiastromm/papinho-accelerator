static DWORD cycle_live_close_last(HANDLE *owned)
{
 DWORD e;if(lab_aux_owner.state&A1_FROZEN)return lab_aux_sync(ERROR_INVALID_STATE);
 if(!owned||!*owned)return ERROR_INVALID_HANDLE;
 if(!CloseHandle(*owned)){e=lab_error();a1_retain(&lab_aux_owner,e);return lab_aux_sync(e);}
 *owned=NULL;return 0;
}
