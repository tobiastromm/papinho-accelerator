/* ROOT06 extends the SAME file/owner; first8192 bytes never change. */
int papacc_configuration_issuer_hash06(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[32])
{
 PAPACC_CONFIGURATION_COMMIT *o=ctx;NTSTATUS s=0,t;int ok;
 if(!owner(o)||o->unresolved||o->issuer_hash||o->issuer_hash_alg||!p||!out||n>ULONG_MAX)return 0;
 s=BCryptOpenAlgorithmProvider((BCRYPT_ALG_HANDLE *)&o->issuer_hash_alg,BCRYPT_SHA256_ALGORITHM,NULL,0);
 if(s<0){if(!o->issuer_hash_error)o->issuer_hash_error=s;if(o->issuer_hash_alg)o->unresolved=1;return 0;}
 s=BCryptCreateHash(o->issuer_hash_alg,(BCRYPT_HASH_HANDLE *)&o->issuer_hash,NULL,0,NULL,0,0);
 if(s<0&&o->issuer_hash){if(!o->issuer_hash_error)o->issuer_hash_error=s;o->unresolved=1;return 0;}
 if(s>=0)s=BCryptHashData(o->issuer_hash,(PUCHAR)p,(ULONG)n,0);
 if(s>=0)s=BCryptFinishHash(o->issuer_hash,out,32,0);ok=s>=0;
 if(s<0&&!o->issuer_hash_error)o->issuer_hash_error=s;
 if(o->issuer_hash){t=BCryptDestroyHash(o->issuer_hash);if(t<0){if(!o->issuer_hash_error)o->issuer_hash_error=t;o->unresolved=1;return 0;}o->issuer_hash=NULL;}
 t=BCryptCloseAlgorithmProvider(o->issuer_hash_alg,0);if(t<0){if(!o->issuer_hash_error)o->issuer_hash_error=t;o->unresolved=1;return 0;}o->issuer_hash_alg=NULL;return ok;
}
static int issuer06_binding(PAPACC_CONFIGURATION_COMMIT *o,const unsigned char *base,const unsigned char *image,const BY_HANDLE_FILE_INFORMATION *f,PAPACC_ISSUER_VIEW *v)
{
 unsigned char hash[32];
 if(memcmp(base,"PACL0501",8)||get64(base+8)!=5U||memcmp(base+16,image+32,32)||
  get64(base+104)!=f->dwVolumeSerialNumber||get64(base+112)!=(((PAPACC_U64)f->nFileIndexHigh<<32)|f->nFileIndexLow))return 0;
 if(!papacc_configuration_issuer_hash06(o,base,PAPACC_CONFIG_STORE_BYTES,hash)||memcmp(hash,image+192,32))return 0;
 if(!papacc_configuration_issuer_hash06(o,base,STORE03_FOOTER,hash)||memcmp(hash,base+STORE03_FOOTER,32))return 0;
 if(papacc_issuer_image_validate(image,v,papacc_configuration_issuer_hash06,o)!=PAPACC_ISSUER_OK)return 0;
 return get64(image+24)==get64(base+48)+1U;
}
static int issuer06_append(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_VIEW *next,const BY_HANDLE_FILE_INFORMATION *f,unsigned char *b)
{
 PAPACC_ISSUER_VIEW v;
 if(!o->issuer_stage_prepared||!issuer06_binding(o,o->committed,o->issuer_staged,f,&v)||v.revision!=next->revision||memcmp(v.root,o->domain,32))return 0;
 memcpy(b,o->committed,PAPACC_CONFIG_STORE_BYTES);memcpy(b+PAPACC_CONFIG_STORE_BYTES,o->issuer_staged,PAPACC_ISSUER_IMAGE_BYTES);return 1;
}
PAPACC_RESULT papacc_configuration_issuer_prepare06(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_U8 *image,PAPACC_U64 expected,PAPACC_U64 expected_high)
{
 PAPACC_ISSUER_VIEW v,old;PAPACC_CONFIG_SLOT *slot;unsigned i;PAPACC_SIZE used;unsigned char hash[32];PAPACC_RESULT result=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!image)return PAPACC_RESULT_INVALID_ARGUMENT;EnterCriticalSection(&o->lock);
 if((o->store02!=5&&o->store02!=6)||!o->published||o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||o->next_serial==(PAPACC_U64)-1||expected==(PAPACC_U64)-1)goto done;
 if(o->slots[o->current].view.revision!=expected)goto done;
 /* Reserve only a free non-current generation; pinned or retained slots
  * remain byte-exact. No waiting or callback is introduced. */
 for(i=0;i<PAPACC_CONFIG_SLOTS;i++)if(i!=o->current&&!o->slots[i].readers&&!o->slots[i].retained)break;
 if(i==PAPACC_CONFIG_SLOTS){result=PAPACC_RESULT_LIMIT_EXCEEDED;goto done;}
 if(papacc_issuer_image_validate(image,&v,papacc_configuration_issuer_hash06,o)!=PAPACC_ISSUER_OK||memcmp(v.root,o->domain,32)||v.revision!=expected+1)goto done;
 if(!papacc_configuration_issuer_hash06(o,o->committed,PAPACC_CONFIG_STORE_BYTES,hash)||memcmp(hash,image+192,32))goto done;
#ifndef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 {PAPACC_SIZE pos=256;PAPACC_U32 last=0;while(pos<p03_get32(image+12)){last=p03_get32(image+pos+320);pos+=p03_get32(image+pos+8);}
  if(last==PAPACC_ISSUER_PUBLISHED||last==PAPACC_ISSUER_REVOKED){result=PAPACC_RESULT_NOT_SUPPORTED;goto done;}}
#endif
 if(o->store02==5){if(expected_high!=1||v.high_water!=1||p03_get32(image+16)!=0||get64(image+24)!=expected+1)goto done;}
 else{
  if(papacc_issuer_image_validate(o->committed+PAPACC_CONFIG_STORE_BYTES,&old,papacc_configuration_issuer_hash06,o)!=PAPACC_ISSUER_OK||old.high_water!=expected_high)goto done;
  used=p03_get32(o->committed+PAPACC_CONFIG_STORE_BYTES+12);
  if(p03_get32(image+16)!=p03_get32(o->committed+PAPACC_CONFIG_STORE_BYTES+16)+1||
   memcmp(image+24,o->committed+PAPACC_CONFIG_STORE_BYTES+24,144)||memcmp(image+176,o->committed+PAPACC_CONFIG_STORE_BYTES+176,8)||
   memcmp(image+192,o->committed+PAPACC_CONFIG_STORE_BYTES+192,32)||memcmp(image+256,o->committed+PAPACC_CONFIG_STORE_BYTES+256,used-256))goto done;
 }
 slot=&o->slots[i];slot->view=o->slots[o->current].view;fix(&slot->view);
 slot->view.predecessor=expected;slot->view.revision=expected+1;slot->serial=++o->next_serial;
 o->issuer_snapshot_valid[i]=0;memcpy(o->issuer_staged,image,PAPACC_ISSUER_IMAGE_BYTES);o->issuer_stage_prepared=1;o->pending=i;o->prepared=1;result=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return result;
}
PAPACC_RESULT papacc_configuration_issuer_snapshot06(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_U8 *out)
{
 unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!p||!out||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS)return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);
 if(o->unresolved||!p->ticket||!o->issuer_snapshot_valid[p->index]||
    !o->slots[p->index].readers||o->slots[p->index].serial!=p->serial)goto done;
 for(i=0;i<32;i++)if(o->tickets[i].ticket==p->ticket&&o->tickets[i].serial==p->serial&&o->tickets[i].index==p->index)break;
 if(i==32)goto done;
 /* Pure synchronous copy; no Win32 resource query, hash or callback. Stop
  * denies new pins, but does not free an existing pin's immutable bytes. */
 memcpy(out,o->issuer_snapshots[p->index],PAPACC_ISSUER_IMAGE_BYTES);r=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_pin_read(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_CONFIG_VIEW *v,PAPACC_U64 *epoch)
{
 unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!p||!v||!epoch||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS)return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);
 if(!p->ticket||!o->slots[p->index].readers||o->slots[p->index].serial!=p->serial||o->unresolved)goto done;
 for(i=0;i<32;i++)if(o->tickets[i].ticket==p->ticket&&o->tickets[i].serial==p->serial&&o->tickets[i].index==p->index)break;
 if(i==32)goto done;
 *v=o->slots[p->index].view;fix(v);*epoch=o->slots[p->index].context_epoch;r=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_issuer_publish06(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_U64 epoch;unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o))return r;EnterCriticalSection(&o->lock);
 if(o->store02!=6||!o->committed_pending||o->prepared||o->stopped||o->unresolved||o->recovery_selected||
    !o->issuer_snapshot_valid[o->pending]||o->slots[o->pending].readers||o->slots[o->pending].retained)goto done;
 /* Root-owned post-commit generation: never accept an issuer/caller epoch.
  * No observer sees CURRENT without its epoch: selection is under one lock. */
 if(BCryptGenRandom(NULL,(PUCHAR)&epoch,sizeof(epoch),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){r=PAPACC_RESULT_INTERNAL_ERROR;goto done;}
 if(!epoch)goto done;
 for(i=0;i<PAPACC_CONFIG_SLOTS;i++)if(o->slots[i].context_epoch==epoch)goto done;
 o->slots[o->pending].context_epoch=epoch;o->current=o->pending;o->committed_pending=0;o->published=1;r=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_issuer_copy06(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_U8 *out)
{
 PAPACC_ISSUER_VIEW v;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!out)return PAPACC_RESULT_INVALID_ARGUMENT;EnterCriticalSection(&o->lock);
 if(o->store02==6&&!o->unresolved&&(o->published||o->committed_pending||o->recovery_selected)&&
  papacc_issuer_image_validate(o->committed+PAPACC_CONFIG_STORE_BYTES,&v,papacc_configuration_issuer_hash06,o)==PAPACC_ISSUER_OK){
  memcpy(out,o->committed+PAPACC_CONFIG_STORE_BYTES,PAPACC_ISSUER_IMAGE_BYTES);r=PAPACC_RESULT_OK;}
 LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_issuer_recovery06(PAPACC_CONFIGURATION_COMMIT *o,const char *path)
{
 HANDLE file;LARGE_INTEGER size;DWORD count;BY_HANDLE_FILE_INFORMATION info;PAPACC_ISSUER_VIEW v={0};int ok;
 if(!owner(o)||!path||strcmp(path,o->path)||!path_ok(path)||o->published||o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||o->next_serial)return PAPACC_RESULT_INVALID_STATE;
 file=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(file==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
 ok=GetFileInformationByHandle(file,&info)&&GetFileSizeEx(file,&size)&&size.QuadPart==PAPACC_CONFIG_STORAGE_BYTES&&
  ReadFile(file,o->committed,PAPACC_CONFIG_STORAGE_BYTES,&count,NULL)&&count==PAPACC_CONFIG_STORAGE_BYTES;
 if(ok)ok=issuer06_binding(o,o->committed,o->committed+PAPACC_CONFIG_STORE_BYTES,&info,&v);
 if(!ok){if(!CloseHandle(file)){o->retained_file=file;o->unresolved=1;}return PAPACC_RESULT_INVALID_STATE;}
 /* Exclusive read lease protects observation, NOT freshness or CURRENT. */
 o->retained_file=file;o->recovery_guard=1;o->store02=6;o->recovery_selected=1;memcpy(o->domain,v.root,32);
 memset(&o->slots[0].view,0,sizeof(o->slots[0].view));memcpy(o->slots[0].view.domain,v.root,32);o->slots[0].view.revision=v.revision;o->slots[0].view.predecessor=v.revision-1;
 o->current=0;o->slots[0].serial=++o->next_serial;return PAPACC_RESULT_NOT_SUPPORTED;
}