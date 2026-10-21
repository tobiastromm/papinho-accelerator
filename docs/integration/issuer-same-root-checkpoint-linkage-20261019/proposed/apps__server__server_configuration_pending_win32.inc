static int pending_overlap(const void *a,PAPACC_SIZE na,const void *b,PAPACC_SIZE nb)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return na&&nb&&(x<=y?y-x<na:x-y<nb);}
/* Only native persistence runs off-owner. No root pointer reaches worker. */
static void pending_error(PAPACC_CONFIG_PENDING *p)
{if(!p->native_error)p->native_error=GetLastError();}
static DWORD WINAPI pending_worker(void *arg)
{
 PAPACC_CONFIG_PENDING *p=arg;DWORD n;LARGE_INTEGER size;BY_HANDLE_FILE_INFORMATION f;
 int ok=0,conflict=0,rolled=0,verified=0;
 p->result=PAPACC_COMMIT_FAILED;p->file=INVALID_HANDLE_VALUE;p->transaction=NULL;
 if(InterlockedCompareExchange(&p->stop,0,0))return 0;
 p->transaction=CreateTransaction(NULL,NULL,0,0,0,0,NULL);
 if(p->transaction==INVALID_HANDLE_VALUE){p->transaction=NULL;pending_error(p);return 0;}
 p->file=CreateFileTransactedA(p->path,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,
 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL,p->transaction,NULL,NULL);
 if(p->file==INVALID_HANDLE_VALUE){pending_error(p);goto cleanup;}
 if(!GetFileInformationByHandle(p->file,&f)||!GetFileSizeEx(p->file,&size)){pending_error(p);goto cleanup;}
 if((f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||size.QuadPart!=p->previous_bytes||
    get64(p->previous+104)!=f.dwVolumeSerialNumber||
    get64(p->previous+112)!=(((PAPACC_U64)f.nFileIndexHigh<<32)|f.nFileIndexLow)){conflict=1;goto cleanup;}
 if(!ReadFile(p->file,p->observed,p->previous_bytes,&n,NULL)||n!=p->previous_bytes){pending_error(p);goto cleanup;}
 if(memcmp(p->observed,p->previous,p->previous_bytes)){conflict=1;goto cleanup;}
 verified=1;
 if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;
 if(SetFilePointer(p->file,0,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER){pending_error(p);goto cleanup;}
 if(!WriteFile(p->file,p->next,p->next_bytes,&n,NULL)||n!=p->next_bytes||!SetEndOfFile(p->file)||!FlushFileBuffers(p->file)){pending_error(p);goto cleanup;}
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 if(p->test_entered){
  if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0){pending_error(p);goto cleanup;}
 }
 if(p->test_unknown){SetLastError(ERROR_WRITE_FAULT);pending_error(p);p->result=PAPACC_CAS_UNKNOWN;return 0;}
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;
 if(!CloseHandle(p->file)){p->close_error=GetLastError();p->result=PAPACC_CAS_UNKNOWN;return 0;}
 p->file=INVALID_HANDLE_VALUE;
 if(!CommitTransaction(p->transaction)){pending_error(p);p->result=PAPACC_CAS_UNKNOWN;return 0;}
 p->committed=1;p->result=PAPACC_CAS_SUCCESS;
 if(!CloseHandle(p->transaction)){p->close_error=GetLastError();return 0;}
 p->transaction=NULL;return 0;
cleanup:
 if(p->file!=INVALID_HANDLE_VALUE){
  if(!CloseHandle(p->file)){p->close_error=GetLastError();p->result=PAPACC_CAS_UNKNOWN;return 0;}
  p->file=INVALID_HANDLE_VALUE;
 }
 rolled=RollbackTransaction(p->transaction)!=0;
 if(!rolled){pending_error(p);p->result=PAPACC_CAS_UNKNOWN;return 0;}
 ok=CloseHandle(p->transaction)!=0;
 if(!ok){p->close_error=GetLastError();p->result=PAPACC_CAS_UNKNOWN;return 0;}
 p->transaction=NULL;
 p->result=conflict?PAPACC_CAS_CONFLICT:(verified?PAPACC_COMMIT_FAILED:PAPACC_CAS_UNKNOWN);
 return 0;
}
static PAPACC_RESULT pending_begin06(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_ISSUER_VIEW v;BY_HANDLE_FILE_INFORMATION f;
 if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;
 if(p->active||p->thread||o->authority_lost||o->unresolved||o->stopped||o->recovery_selected||
    !o->published||!o->prepared||!o->issuer_stage_prepared||o->committed_pending||
    o->slots[o->pending].readers||o->slots[o->pending].retained||
    (o->store02!=5&&o->store02!=6)||!path_ok(o->path))return PAPACC_RESULT_INVALID_STATE;
 /* Validate existing signed-free ROOT06 codec and immutable base identity.
  * Actual file identity and full CAS are checked again by the native worker. */
 memset(&f,0,sizeof(f));f.dwVolumeSerialNumber=(DWORD)get64(o->committed+104);
 f.nFileIndexHigh=(DWORD)(get64(o->committed+112)>>32);f.nFileIndexLow=(DWORD)get64(o->committed+112);
 if(!issuer06_binding(o,o->committed,o->issuer_staged,&f,&v)||
    v.revision!=o->slots[o->pending].view.revision)return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 {HANDLE a=p->test_entered,b=p->test_release;int fault=p->test_unknown;
 memset(p,0,sizeof(*p));p->test_entered=a;p->test_release=b;p->test_unknown=fault;}
#else
 memset(p,0,sizeof(*p));
#endif
 p->file=INVALID_HANDLE_VALUE;
 memcpy(p->previous,o->committed,o->committed_bytes);memcpy(p->next,o->committed,8192);
 memcpy(p->next+8192,o->issuer_staged,8192);memcpy(p->path,o->path,sizeof(p->path));
 p->previous_bytes=o->committed_bytes;p->next_bytes=PAPACC_ISSUER_ROOT_IMAGE_BYTES;p->result=PAPACC_COMMIT_PENDING;p->active=1;
 p->thread=CreateThread(NULL,PAPACC_PENDING_STACK_BYTES,pending_worker,p,STACK_SIZE_PARAM_IS_A_RESERVATION,NULL);
 if(!p->thread){p->native_error=GetLastError();p->result=PAPACC_COMMIT_FAILED;p->active=0;return PAPACC_RESULT_INTERNAL_ERROR;}
 return PAPACC_RESULT_OK;
}
static PAPACC_RESULT pending_poll06(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 PAPACC_CONFIG_PENDING *p;DWORD w;
 if(!owner(o)||!out||pending_overlap(o,sizeof(*o),out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;p=&o->pending_commit;
 if(!p->active){*out=p->result;return PAPACC_RESULT_INVALID_STATE;}
 if(p->thread_close_attempted){*out=PAPACC_CAS_UNKNOWN;return PAPACC_RESULT_INVALID_STATE;}
 w=WaitForSingleObject(p->thread,0);
 if(w==WAIT_TIMEOUT){*out=PAPACC_COMMIT_PENDING;return PAPACC_RESULT_OK;}
 if(w!=WAIT_OBJECT_0){o->authority_lost=1;*out=PAPACC_CAS_UNKNOWN;return PAPACC_RESULT_INTERNAL_ERROR;}
 /* A signaled thread proves no remaining access to embedded buffers. Closing
  * the handle is separate from commit and has its own factual result. */
 p->thread_close_attempted=1;
 if(!CloseHandle(p->thread)){p->close_error=GetLastError();o->authority_lost=1;*out=p->result;return PAPACC_RESULT_INTERNAL_ERROR;}
 p->thread=NULL;p->active=0;*out=p->result;
 if(p->result==PAPACC_CAS_SUCCESS){
  memcpy(o->committed,p->next,p->next_bytes);o->committed_bytes=p->next_bytes;
  if(!p->checkpoint_only){memcpy(o->issuer_snapshots[o->pending],p->next+8192,8192);
   o->issuer_snapshot_valid[o->pending]=1;o->store02=6;o->prepared=0;o->issuer_stage_prepared=0;o->committed_pending=1;}
 }else if(p->result==PAPACC_CAS_CONFLICT||p->result==PAPACC_CAS_UNKNOWN)o->authority_lost=1;
 if(p->file!=INVALID_HANDLE_VALUE||p->transaction!=NULL||p->close_error)o->unresolved=1;
 return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_configuration_issuer_begin06(PAPACC_CONFIGURATION_COMMIT *o)
{PAPACC_RESULT r;if(!owner(o))return PAPACC_RESULT_INVALID_STATE;EnterCriticalSection(&o->lock);r=pending_begin06(o);LeaveCriticalSection(&o->lock);return r;}
PAPACC_RESULT papacc_configuration_issuer_poll06(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CAS_RESULT *out)
{PAPACC_RESULT r;if(!owner(o)||o->pending_commit.checkpoint_only)return PAPACC_RESULT_INVALID_STATE;EnterCriticalSection(&o->lock);r=pending_poll06(o,out);LeaveCriticalSection(&o->lock);return r;}

PAPACC_RESULT papacc_configuration_checkpoint_begin07(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_CONFIG_SLOT *slot;PAPACC_ISSUER_LINK link;
 PAPACC_U8 encoded[512];PAPACC_RESULT result=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o))return result;EnterCriticalSection(&o->lock);p=&o->pending_commit;
 if(o->store02!=6||!o->published||o->prepared||o->committed_pending||o->stopped||o->unresolved||
  o->recovery_selected||pending_blocks(o)||o->committed_bytes!=16384||p->checkpoint_only||
  p->result!=PAPACC_CAS_SUCCESS||!p->committed||p->next_bytes!=16384||memcmp(p->next,o->committed,16384))goto done;
 slot=&o->slots[o->current];memset(&link,0,sizeof(link));
 link.revision=slot->view.revision;link.predecessor=slot->view.predecessor;
 link.administrative_epoch=slot->context_epoch;link.policy_epoch=slot->operational_epoch;
 if(!slot->issuer_revision||slot->issuer_revision!=link.revision||!o->issuer_snapshot_valid[o->current]||
  memcmp(o->issuer_snapshots[o->current],o->committed+8192,8192)||
  !papacc_configuration_issuer_hash06(o,p->previous,p->previous_bytes,link.previous_commit_digest)||
  papacc_issuer_link_encode(encoded,sizeof(encoded),o->committed,16384,&link,papacc_configuration_issuer_hash06,o)!=PAPACC_ISSUER_OK)goto done;
 /* Input proof is the confirmed native commit packet plus published slot.
  * Reuse exactly the same completed worker storage; no second writer. */
 memset(p,0,sizeof(*p));p->checkpoint_only=1;p->file=INVALID_HANDLE_VALUE;
 memcpy(p->previous,o->committed,16384);memcpy(p->next,o->committed,16384);
 memcpy(p->next+16384,encoded,512);memcpy(p->path,o->path,sizeof(p->path));
 p->previous_bytes=16384;p->next_bytes=16896;p->result=PAPACC_COMMIT_PENDING;p->active=1;
 p->thread=CreateThread(NULL,PAPACC_PENDING_STACK_BYTES,pending_worker,p,STACK_SIZE_PARAM_IS_A_RESERVATION,NULL);
 if(!p->thread){p->native_error=GetLastError();p->result=PAPACC_COMMIT_FAILED;p->active=0;result=PAPACC_RESULT_INTERNAL_ERROR;goto done;}
 result=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return result;
}
PAPACC_RESULT papacc_configuration_checkpoint_poll07(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 PAPACC_RESULT r;if(!owner(o)||!o->pending_commit.checkpoint_only)return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&o->lock);r=pending_poll06(o,out);LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_checkpoint_data07(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_LINK *out)
{
 PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!out||pending_overlap(o,sizeof(*o),out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);
 if(o->committed_bytes==16896&&!o->unresolved&&!o->pending_commit.active&&
  papacc_issuer_link_decode(o->committed+16384,512,o->committed,16384,out,papacc_configuration_issuer_hash06,o)==PAPACC_ISSUER_OK&&
  (!o->published||(o->slots[o->current].view.revision==out->revision&&
   o->slots[o->current].context_epoch==out->administrative_epoch&&o->slots[o->current].operational_epoch==out->policy_epoch)))
  r=PAPACC_RESULT_NOT_SUPPORTED; /* data exists; recovery/authority unsupported */
 LeaveCriticalSection(&o->lock);return r;
}
