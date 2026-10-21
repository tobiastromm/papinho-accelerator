/* Private normal-custody restart. Never writes or provisions a store. */
static int restart03_read(PAPACC_CONFIGURATION_COMMIT *o,HANDLE f)
{
 unsigned char b[PAPACC_CONFIG_STORE_BYTES];DWORD n;LARGE_INTEGER size;
 BY_HANDLE_FILE_INFORMATION info;PAPACC_CONFIG_VIEW v;
 if(!GetFileInformationByHandle(f,&info)||!GetFileSizeEx(f,&size)||size.QuadPart!=sizeof(b)||
    SetFilePointer(f,0,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER||
    !ReadFile(f,b,sizeof(b),&n,NULL)||n!=sizeof(b)||memcmp(b,o->committed,sizeof(b))||
    !store03_select(b,&info,&v))return 0;
 return v.revision==o->slots[o->current].view.revision&&
  v.predecessor==o->slots[o->current].view.predecessor&&memcmp(v.domain,o->domain,32U)==0;
}
PAPACC_RESULT papacc_configuration_recovery_guard03(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_NORMAL_RESTART_WITNESS *w)
{
 HANDLE f;BY_HANDLE_FILE_INFORMATION info;unsigned char hash[32];int ok;
 if(!owner(o)||!w||o->store02!=3||!o->recovery_selected||o->recovery_guard||
    o->published||o->stopped||o->unresolved||o->prepared||o->committed_pending||
    o->retained_file!=INVALID_HANDLE_VALUE||!path_ok(o->path))return PAPACC_RESULT_INVALID_STATE;
 if(!w->revision||w->revision!=o->slots[o->current].view.revision||
    w->predecessor!=o->slots[o->current].view.predecessor||memcmp(w->domain,o->domain,32U)||
    !store03_hash(o->committed,PAPACC_CONFIG_STORE_BYTES,hash)||memcmp(hash,w->image_sha256,32U))
  return PAPACC_RESULT_INVALID_STATE;
 f=CreateFileA(o->path,GENERIC_READ,0U,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(f==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
 ok=GetFileInformationByHandle(f,&info)&&!(info.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&
  w->volume==info.dwVolumeSerialNumber&&w->file_index==(((PAPACC_U64)info.nFileIndexHigh<<32)|info.nFileIndexLow)&&
  restart03_read(o,f);
 if(!ok){
  if(!CloseHandle(f)){o->retained_file=f;o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;}
  return PAPACC_RESULT_INVALID_STATE;
 }
 o->retained_file=f;o->recovery_guard=1;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_recovery_validate03(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!owner(o)||o->store02!=3||!o->recovery_guard||!o->recovery_selected||o->published||
    o->stopped||o->unresolved||o->retained_file==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
 return restart03_read(o,o->retained_file)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_recovery_publish03(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_RESULT result;
 if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&o->lock);result=papacc_configuration_recovery_validate03(o);
 if(result==PAPACC_RESULT_OK){o->published=1;o->recovery_selected=0;}
 LeaveCriticalSection(&o->lock);return result;
}
