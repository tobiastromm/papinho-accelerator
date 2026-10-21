/* Same root, immutable per-generation DATA only; no dispatch/publication. */
#include <wincrypt.h>
static PAPACC_U32 v3b32(const PAPACC_U8 *p)
{return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U64 v3b64(const PAPACC_U8 *p)
{PAPACC_U64 x=0;unsigned j;for(j=0;j<8U;j++)x|=(PAPACC_U64)p[j]<<(8U*j);return x;}
static int v3b_range(const void *p,PAPACC_SIZE n)
{return p&&n&&n-1U<=MAXULONG_PTR-(ULONG_PTR)p;}
/* Public DER -> canonical SPKI digest. No context, key, handle or pointer escapes.
 * Existing CryptoAPI primitives, caller arena, no key creation/private operation.
 * DER is candidate DATA, never CA authorization or profile qualification. */
static int v3b_ca(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_U8 *der,PAPACC_SIZE bytes,
 PAPACC_ISSUER_WINDOWS_WORKSPACE *w,PAPACC_U8 key[32])
{
 CERT_SIGNED_CONTENT_INFO *signed_info;CERT_INFO *info;PAPACC_U8 *cursor=w->bytes;
 DWORD n=0,left=(DWORD)w->capacity,rounded;
 w->native_error=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,der,(DWORD)bytes,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))goto error;
 if(n>0xfffffff8U||(rounded=(n+7U)&~7U)>left)return 0;
 signed_info=(CERT_SIGNED_CONTENT_INFO *)cursor;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT,der,(DWORD)bytes,CRYPT_DECODE_NOCOPY_FLAG,NULL,signed_info,&n))goto error;
 cursor+=rounded;left-=rounded;n=left;
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,signed_info,0,NULL,cursor,&n)||n!=bytes||memcmp(cursor,der,bytes))return 0;
 n=0;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,NULL,&n))goto error;
 if(n>0xfffffff8U||(rounded=(n+7U)&~7U)>left)return 0;
 info=(CERT_INFO *)cursor;
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,signed_info->ToBeSigned.pbData,signed_info->ToBeSigned.cbData,CRYPT_DECODE_NOCOPY_FLAG,NULL,info,&n))goto error;
 cursor+=rounded;left-=rounded;n=left;
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_PUBLIC_KEY_INFO,&info->SubjectPublicKeyInfo,0,NULL,cursor,&n))goto error;
 return papacc_configuration_issuer_hash06(o,cursor,n,key);
error:w->native_error=GetLastError();return 0;
}
static int v3b_free_prepared(PAPACC_CONFIGURATION_COMMIT *o)
{
 return !PAPACC_V3_UPDATE_BUSY(o)&&o->prepared&&!o->committed_pending&&!o->stopped&&!o->unresolved&&!o->authority_lost&&!pending_blocks(o)&&
 o->pending<PAPACC_CONFIG_SLOTS&&(!o->published||o->pending!=o->current)&&
 !o->slots[o->pending].readers&&!o->slots[o->pending].retained;
}
PAPACC_RESULT papacc_configuration_v3_capture_data(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *ca,PAPACC_SIZE cn,
 PAPACC_ISSUER_WINDOWS_WORKSPACE *w,PC_ROOT8_COMPONENT component,void *cc)
{
 PAPACC_CONFIG_V3_BACKING *b;PAPACC_CONFIG_SLOT *slot;PAPACC_U8 key[32],ph[32],ch[32];
 PAPACC_CONFIG_VIEW *cv;PAPACC_SIZE off;PAPACC_U64 serial;unsigned index,j;PAPACC_ISSUER_VIEW iv;PC_ROOT8_RESULT result;
 if(!owner(o)||!v3b_range(p,n)||!v3b_range(ca,cn)||!v3b_range(w,sizeof(*w))||
 n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX||cn>PAPACC_CONFIG_V3_CA_BYTES||
 !v3b_range(w->bytes,w->capacity)||((ULONG_PTR)w->bytes&7U)||w->capacity>0xffffffffU||w->capacity<sizeof(PAPACC_CONFIG_VIEW)||!component||
 issuer06_overlap(p,n,o,sizeof(*o))||issuer06_overlap(ca,cn,o,sizeof(*o))||
 issuer06_overlap(w,sizeof(*w),o,sizeof(*o))||issuer06_overlap(w->bytes,w->capacity,o,sizeof(*o))||
 issuer06_overlap(p,n,ca,cn)||issuer06_overlap(w,sizeof(*w),p,n)||issuer06_overlap(w,sizeof(*w),ca,cn)||
 issuer06_overlap(w->bytes,w->capacity,p,n)||issuer06_overlap(w->bytes,w->capacity,ca,cn)||
 issuer06_overlap(w->bytes,w->capacity,w,sizeof(*w)))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(w->stopped)return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&o->lock);
 if(!v3b_free_prepared(o)){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 index=o->pending;slot=&o->slots[index];serial=slot->serial;
 if(!serial||o->v3_backings[index].serial==serial){LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 LeaveCriticalSection(&o->lock);
 /* Owner-thread serialized, nonreentrant ports; never called on Stop path.
  * Validation outside lock: no native/codec call in the Stop/close path. */
 result=pc_root8_v3_validate(p,n,papacc_configuration_issuer_hash06,o,component,cc);
 if(result!=PC_ROOT8_OK)return PAPACC_RESULT_INVALID_STATE;
 off=v3b32(p+48);
 if(papacc_issuer_image_validate(p+off+8192,&iv,papacc_configuration_issuer_hash06,o)!=PAPACC_ISSUER_OK||
 memcmp(iv.root,p+72,32)||iv.revision!=v3b64(p+120)||
 !v3b_ca(o,ca,cn,w,key)||memcmp(key,iv.ca_spki_digest,32)||
 !papacc_configuration_issuer_hash06(o,p,n,ph)||!papacc_configuration_issuer_hash06(o,ca,cn,ch))return PAPACC_RESULT_INVALID_STATE;
 /* Every SNT row refers to this same public issuer/CA, not a second catalog. */
 for(j=0;j<(unsigned)(p[252]|((unsigned)p[253]<<8));j++){
 const PAPACC_U8 *row=p+v3b32(p+244)+64U+768U*j;
 if(memcmp(row+40,iv.authority,16)||memcmp(row+56,iv.generation,16)||memcmp(row+568,key,32))return PAPACC_RESULT_INVALID_STATE;
 }
 cv=w->bytes; /* crypto decode borrows ended; same caller arena reused */
 if(papacc_configuration_body05_decode(p+1024U,1024U,cv)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&o->lock);
 if(!v3b_free_prepared(o)||o->pending!=index||slot->serial!=serial||w->stopped||
 memcmp(o->domain,p+72,32)||!papacc_configuration_issuer_projection06(&slot->view.config,&cv->config)||
 memcmp(slot->view.domain,p+72,32)||slot->view.revision!=v3b64(p+104)||
 slot->view.predecessor!=v3b64(p+112)||(slot->issuer_revision&&slot->issuer_revision!=iv.revision)||
 (o->issuer_snapshot_valid[index]&&memcmp(o->issuer_snapshots[index],p+off+8192,8192))){
 LeaveCriticalSection(&o->lock);return PAPACC_RESULT_INVALID_STATE;}
 b=&o->v3_backings[index];b->serial=0;
 memcpy(b->packet,p,n);memcpy(b->ca,ca,cn);memcpy(b->packet_hash,ph,32);memcpy(b->ca_hash,ch,32);
 b->packet_bytes=n;b->ca_bytes=cn;b->serial=serial;
 LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_v3_borrow_data(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CONFIG_PIN *p,PAPACC_CONFIG_V3_BORROW *out)
{
 PAPACC_CONFIG_V3_BACKING *b;PAPACC_CONFIG_SLOT *slot;PAPACC_CONFIG_V3_BORROW v;
 PAPACC_U8 d[32];unsigned j;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o)||!v3b_range(p,sizeof(*p))||!v3b_range(out,sizeof(*out))||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||
 issuer06_overlap(out,sizeof(*out),o,sizeof(*o))||issuer06_overlap(out,sizeof(*out),p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);slot=&o->slots[p->index];b=&o->v3_backings[p->index];
 if(o->stopped||o->unresolved||o->authority_lost||!p->ticket||!slot->readers||slot->serial!=p->serial||b->serial!=p->serial)goto done;
 for(j=0;j<32U;j++)if(o->tickets[j].ticket==p->ticket&&o->tickets[j].serial==p->serial&&o->tickets[j].index==p->index)break;
 if(j==32U||b->packet_bytes<PC_ROOT8_V3_MIN||b->packet_bytes>PC_ROOT8_V3_MAX||!b->ca_bytes||b->ca_bytes>PAPACC_CONFIG_V3_CA_BYTES)goto done;
 if(!papacc_configuration_issuer_hash06(o,b->packet,b->packet_bytes,d)||memcmp(d,b->packet_hash,32)||
 !papacc_configuration_issuer_hash06(o,b->ca,b->ca_bytes,d)||memcmp(d,b->ca_hash,32)||
 memcmp(slot->view.domain,b->packet+72,32)||slot->view.revision!=v3b64(b->packet+104)||
 slot->view.predecessor!=v3b64(b->packet+112)||
 (slot->issuer_revision&&slot->issuer_revision!=v3b64(b->packet+120)))goto done;
 if(o->issuer_snapshot_valid[p->index]&&memcmp(o->issuer_snapshots[p->index],b->packet+v3b32(b->packet+48)+8192,8192))goto done;
 memset(&v,0,sizeof(v));v.packet=b->packet;v.packet_bytes=b->packet_bytes;
 v.snt=b->packet+v3b32(b->packet+244);v.snt_bytes=v3b32(b->packet+248);
 v.pdg=b->packet+v3b32(b->packet+260);v.pdg_bytes=v3b32(b->packet+264);
 v.issuer_image=b->packet+v3b32(b->packet+48)+8192;v.ca=b->ca;v.ca_bytes=b->ca_bytes;
 v.revision=slot->view.revision;v.issuer_revision=v3b64(b->packet+120);*out=v;r=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return r;
}

/* Correlation of already protected DATA, deliberately not a CURRENT factory. */
PAPACC_RESULT papacc_configuration_v3_checkpoint_match_data(
 PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *pin)
{
 PAPACC_CONFIG_V3_BORROW borrow;PAPACC_CONFIG_SLOT *slot;const PAPACC_U8 *cp;
 PAPACC_RESULT result;
 if(!owner(o)||!pin)return PAPACC_RESULT_INVALID_ARGUMENT;
 result=papacc_configuration_v3_borrow_data(o,pin,&borrow);if(result!=PAPACC_RESULT_OK)return result;
 EnterCriticalSection(&o->lock);result=PAPACC_RESULT_INVALID_STATE;
 if(!o->published||o->prepared||o->committed_pending||pending_blocks(o)||o->recovery_selected||
 pin->index!=o->current||!o->issuer_snapshot_valid[pin->index])goto done;
 slot=&o->slots[pin->index];
 if(!slot->context_epoch||!slot->operational_epoch||!slot->issuer_revision)goto done;
 if(borrow.packet_bytes>sizeof(o->committed)){result=PAPACC_RESULT_NOT_SUPPORTED;goto done;}
 if(o->committed_bytes!=borrow.packet_bytes||memcmp(o->committed,borrow.packet,borrow.packet_bytes))goto done;
 cp=borrow.packet+v3b32(borrow.packet+56);
 if(memcmp(cp,"PACR8CP3",8)||cp[8]!=3U||cp[9]||
 memcmp(cp+16,slot->view.domain,32)||v3b64(cp+48)!=slot->view.revision||
 v3b64(cp+56)!=slot->view.predecessor||v3b64(cp+64)!=slot->context_epoch||
 v3b64(cp+72)!=slot->operational_epoch||v3b64(cp+80)!=slot->issuer_revision)goto done;
 result=PAPACC_RESULT_OK; /* DATA only; no provenance/freshness/fencing claim */
done:LeaveCriticalSection(&o->lock);return result;
}
