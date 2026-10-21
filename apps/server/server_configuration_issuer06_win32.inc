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
/* Pure semantic projection: all current config members, never padding or
 * a digest alone. Schema changes must extend this comparison and its tests. */
int papacc_configuration_issuer_projection06(const PAPACC_SERVER_CONFIG *x,const PAPACC_SERVER_CONFIG *y)
{
 unsigned i;const PAPACC_ADMIN_POLICY_RULE *a,*b;
 if(!x||!y||papacc_server_config_validate(x)!=PAPACC_RESULT_OK||papacc_server_config_validate(y)!=PAPACC_RESULT_OK)return 0;
#define EQ(f) if(x->f!=y->f)return 0
 EQ(control_port);
 EQ(allow_network_egress);
 EQ(listener_profile);
 EQ(bind_selection.mode);
 EQ(bind_selection.interface_persistent_id_count);
 EQ(security_configuration_ref_valid);
 EQ(security_configuration_ref_length);
 EQ(egress_policy_ref_valid);
 EQ(egress_policy_ref_length);
 EQ(egress_profile_ref_valid);
 EQ(egress_profile_ref_length);
 EQ(outbound_tls_profile_ref_valid);
 EQ(outbound_tls_profile_ref_length);
 EQ(policy.present);
 EQ(policy.representation);
 EQ(policy.profile);
 EQ(policy.admission);
 EQ(policy.listener_port);
 EQ(policy.enabled_count);
 EQ(policy.rule_count);
 EQ(site_tls.present);
 EQ(site_tls.version);
 EQ(site_tls.reference);
 EQ(site_tls.trust_system);
 EQ(site_tls.hostname_required);
 EQ(site_tls.sni_required);
 EQ(site_tls.tls13_only);
 EQ(site_tls.revoked);
 EQ(site_tls.revision);
 EQ(site_tls.profile_revision);
 EQ(site_tls.not_before_utc_ms);
 EQ(site_tls.expires_utc_ms);
 EQ(site_tls.max_bytes);
 EQ(site_tls.max_duration_ms);
 EQ(site_tls.max_concurrent);
 EQ(site_tls.alpn_length);
#undef EQ
 if(memcmp(x->security_configuration_ref,y->security_configuration_ref,sizeof(x->security_configuration_ref)))return 0;
 if(memcmp(x->egress_policy_ref,y->egress_policy_ref,sizeof(x->egress_policy_ref)))return 0;
 if(memcmp(x->egress_profile_ref,y->egress_profile_ref,sizeof(x->egress_profile_ref)))return 0;
 if(memcmp(x->outbound_tls_profile_ref,y->outbound_tls_profile_ref,sizeof(x->outbound_tls_profile_ref)))return 0;
 if(memcmp(x->policy.identity,y->policy.identity,sizeof(x->policy.identity)))return 0;
 if(memcmp(x->site_tls.identity,y->site_tls.identity,sizeof(x->site_tls.identity)))return 0;
 if(memcmp(x->site_tls.profile_identity,y->site_tls.profile_identity,sizeof(x->site_tls.profile_identity)))return 0;
 if(memcmp(x->site_tls.administrative_origin,y->site_tls.administrative_origin,sizeof(x->site_tls.administrative_origin)))return 0;
 if(memcmp(x->site_tls.alpn,y->site_tls.alpn,sizeof(x->site_tls.alpn)))return 0;
 for(i=0;i<x->bind_selection.interface_persistent_id_count;i++)if(x->bind_selection.interface_persistent_ids[i].is_valid!=y->bind_selection.interface_persistent_ids[i].is_valid||x->bind_selection.interface_persistent_ids[i].value!=y->bind_selection.interface_persistent_ids[i].value)return 0;
 for(i=0;i<PAPACC_ADMIN_POLICY_KEYS;i++)if(x->policy.enabled[i].id!=y->policy.enabled[i].id||x->policy.enabled[i].major!=y->policy.enabled[i].major)return 0;
 for(i=0;i<PAPACC_ADMIN_POLICY_RULES;i++){
  a=&x->policy.rules[i];b=&y->policy.rules[i];
  if(a->key.id!=b->key.id||a->key.major!=b->key.major||a->action!=b->action||a->conditions!=b->conditions||
     a->principal16.type!=b->principal16.type||a->principal16.length!=b->principal16.length||
     memcmp(a->principal,b->principal,sizeof(a->principal))||memcmp(a->principal16.value,b->principal16.value,sizeof(a->principal16.value)))return 0;
 }
 return 1;
}
/* No address-addition overflow; outputs cannot overwrite owner or pin. */
static int issuer06_overlap(const void *a,PAPACC_SIZE na,const void *b,PAPACC_SIZE nb)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return na&&nb&&(x<=y?y-x<na:x-y<nb);}
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
/* Synchronous owner-thread inspection only; never opens/adopts a file.
 * store05_select is a read-only historical decoder despite its old signature.
 * Supplied workspace is NOT a current slot. No pointer is retained. */
PAPACC_RESULT papacc_configuration_issuer_carrier_inspect06(
 PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_U8 *b,PAPACC_SIZE n,
 PAPACC_CONFIG_VIEW *workspace,PAPACC_ISSUER_VIEW *out)
{
 BY_HANDLE_FILE_INFORMATION f;PAPACC_ISSUER_VIEW v;
 if(!owner(o)||!b||n!=PAPACC_ISSUER_ROOT_IMAGE_BYTES||!workspace||!out||
    (ULONG_PTR)b>MAXULONG_PTR-(n-1U)||
    (ULONG_PTR)workspace>MAXULONG_PTR-(sizeof(*workspace)-1U)||
    (ULONG_PTR)out>MAXULONG_PTR-(sizeof(*out)-1U)||
    issuer06_overlap(b,n,o,sizeof(*o))||issuer06_overlap(workspace,sizeof(*workspace),o,sizeof(*o))||
    issuer06_overlap(out,sizeof(*out),o,sizeof(*o))||issuer06_overlap(workspace,sizeof(*workspace),b,n)||
    issuer06_overlap(out,sizeof(*out),b,n)||issuer06_overlap(out,sizeof(*out),workspace,sizeof(*workspace)))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));memset(workspace,0,sizeof(*workspace));
 if(o->stopped||o->unresolved||o->authority_lost||pending_blocks(o)||o->prepared||o->committed_pending)return PAPACC_RESULT_INVALID_STATE;
 memset(&f,0,sizeof(f));f.dwVolumeSerialNumber=(DWORD)get64(b+104);
 f.nFileIndexHigh=(DWORD)(get64(b+112)>>32);f.nFileIndexLow=(DWORD)get64(b+112);
 if(!issuer06_binding(o,b,b+8192,&f,&v)||
    !store05_select((unsigned char *)b,&f,workspace)||
    memcmp(workspace->domain,v.root,32)){
  memset(workspace,0,sizeof(*workspace));return PAPACC_RESULT_INVALID_STATE;}
 /* Header identity above is serialized DATA, not native namespace custody. */
 *out=v;out->epoch=0;out->proofs=0;return PAPACC_RESULT_OK;
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
 if((o->store02!=5&&o->store02!=6)||!o->published||o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)||o->next_serial==(PAPACC_U64)-1||expected==(PAPACC_U64)-1)goto done;
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
 slot->view.predecessor=expected;slot->view.revision=expected+1;slot->serial=++o->next_serial;slot->issuer_revision=v.revision;
 o->issuer_snapshot_valid[i]=0;memcpy(o->issuer_staged,image,PAPACC_ISSUER_IMAGE_BYTES);o->issuer_stage_prepared=1;o->pending=i;o->prepared=1;result=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return result;
}
PAPACC_RESULT papacc_configuration_issuer_snapshot06(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_U8 *out)
{
 unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!p||!out||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||issuer06_overlap(out,PAPACC_ISSUER_IMAGE_BYTES,o,sizeof(*o))||issuer06_overlap(out,PAPACC_ISSUER_IMAGE_BYTES,p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
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
PAPACC_RESULT papacc_configuration_pin_read(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_CONFIG_VIEW *v,PAPACC_U64 *administrative_epoch,PAPACC_U64 *policy_epoch)
{
 unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!p||!v||!administrative_epoch||!policy_epoch||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||
    issuer06_overlap(v,sizeof(*v),o,sizeof(*o))||issuer06_overlap(administrative_epoch,sizeof(*administrative_epoch),o,sizeof(*o))||issuer06_overlap(policy_epoch,sizeof(*policy_epoch),o,sizeof(*o))||
    issuer06_overlap(v,sizeof(*v),p,sizeof(*p))||issuer06_overlap(administrative_epoch,sizeof(*administrative_epoch),p,sizeof(*p))||issuer06_overlap(policy_epoch,sizeof(*policy_epoch),p,sizeof(*p))||
    issuer06_overlap(v,sizeof(*v),administrative_epoch,sizeof(*administrative_epoch))||issuer06_overlap(v,sizeof(*v),policy_epoch,sizeof(*policy_epoch))||
    issuer06_overlap(administrative_epoch,sizeof(*administrative_epoch),policy_epoch,sizeof(*policy_epoch)))return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);
 if(!p->ticket||!o->slots[p->index].readers||o->slots[p->index].serial!=p->serial||o->unresolved)goto done;
 for(i=0;i<32;i++)if(o->tickets[i].ticket==p->ticket&&o->tickets[i].serial==p->serial&&o->tickets[i].index==p->index)break;
 if(i==32)goto done;
 *v=o->slots[p->index].view;fix(v);*administrative_epoch=o->slots[p->index].context_epoch;*policy_epoch=o->slots[p->index].operational_epoch;r=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_issuer_publish06(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_U64 epoch;unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o))return r;EnterCriticalSection(&o->lock);
 if(o->store02!=6||!o->committed_pending||o->prepared||o->stopped||o->unresolved||o->recovery_selected||
    pending_blocks(o)||!o->slots[o->current].operational_epoch||
    !papacc_configuration_issuer_projection06(&o->slots[o->current].view.config,&o->slots[o->pending].view.config)||
    !o->issuer_snapshot_valid[o->pending]||o->slots[o->pending].readers||o->slots[o->pending].retained)goto done;
 /* Root-owned post-commit generation: never accept an issuer/caller epoch.
  * No observer sees CURRENT without its epoch: selection is under one lock. */
 if(BCryptGenRandom(NULL,(PUCHAR)&epoch,sizeof(epoch),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){r=PAPACC_RESULT_INTERNAL_ERROR;goto done;}
 if(!epoch)goto done;
 for(i=0;i<PAPACC_CONFIG_SLOTS;i++)if(o->slots[i].context_epoch==epoch)goto done;
 o->slots[o->pending].operational_epoch=o->slots[o->current].operational_epoch;
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
 HANDLE file;LARGE_INTEGER size={0};DWORD count;BY_HANDLE_FILE_INFORMATION info;PAPACC_ISSUER_VIEW v={0};PAPACC_ISSUER_LINK link;int ok;
 if(!owner(o)||!path||strcmp(path,o->path)||!path_ok(path)||o->published||o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||o->next_serial)return PAPACC_RESULT_INVALID_STATE;
 file=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(file==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
 ok=GetFileInformationByHandle(file,&info)&&GetFileSizeEx(file,&size)&&(size.QuadPart==16384||size.QuadPart==16896)&&
  ReadFile(file,o->committed,(DWORD)size.QuadPart,&count,NULL)&&count==(DWORD)size.QuadPart;
 if(ok)ok=issuer06_binding(o,o->committed,o->committed+PAPACC_CONFIG_STORE_BYTES,&info,&v);
 if(ok&&size.QuadPart==16896)ok=papacc_issuer_link_decode(o->committed+16384,512,o->committed,16384,&link,papacc_configuration_issuer_hash06,o)==PAPACC_ISSUER_OK;
 if(!ok){if(!CloseHandle(file)){o->retained_file=file;o->unresolved=1;}return PAPACC_RESULT_INVALID_STATE;}
 /* Exclusive read lease protects observation, NOT freshness or CURRENT. */
 o->committed_bytes=(DWORD)size.QuadPart;
 o->retained_file=file;o->recovery_guard=1;o->store02=6;o->recovery_selected=1;memcpy(o->domain,v.root,32);
 memset(&o->slots[0].view,0,sizeof(o->slots[0].view));memcpy(o->slots[0].view.domain,v.root,32);o->slots[0].view.revision=v.revision;o->slots[0].view.predecessor=v.revision-1;
 o->current=0;o->slots[0].serial=++o->next_serial;return PAPACC_RESULT_NOT_SUPPORTED;
}