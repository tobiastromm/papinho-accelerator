#include "delivery.h"
#include <string.h>
static int overlap(const void *a,SIZE_T n,const void *b,SIZE_T m){ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x<=y?y-x<n:x-y<m;}
static int fail(TP_OWNER *o,const char *s,unsigned long rc){return tp_fail(o,s,rc);}
static void retain(ST20_DELIVERY *d){d->retained=1;d->owner->retained=1;}
static int guard(ST20_DELIVERY *d,const ST20_API *a,const char *stage){TP_OWNER *o=d->owner;int accepted;
 if(o->context!=d->original_context){retain(d);return fail(o,"ST20_ORIGINAL_CONTEXT_CHANGED",0);}
 if(o->frozen||o->closed||o->stop||o->retained)return fail(o,"ST20_CONTEXT_OR_CUSTODY_DENY",0);
 accepted=a->guard(o,d,stage);
 if(o->context!=d->original_context){retain(d);return fail(o,"ST20_ORIGINAL_CONTEXT_CHANGED",0);}
 if(!accepted||o->frozen||o->closed||o->stop||o->retained)return fail(o,"ST20_CONTEXT_OR_CUSTODY_DENY",0);
 return 1;
}

int st20_deliver(ST20_DELIVERY *d,const ST20_API *a){TP_OWNER *o;UINT32 n;TBS_RESULT rc;int use_result,ok=0;
 if(!d||!a||!d->owner)return 0;o=d->owner;
 if(o->owner_thread!=GetCurrentThreadId())return 0;
 if(!d->buffer||!d->capacity||d->capacity>ST20_CAPACITY_LIMIT||!d->original_context||!a->get||!a->lock||!a->unlock||!a->guard||!a->use||
  overlap(d->buffer,d->capacity,o,sizeof(*o))||overlap(d->buffer,d->capacity,d,sizeof(*d))||overlap(d->buffer,d->capacity,a,sizeof(*a))||overlap(d,sizeof(*d),o,sizeof(*o))||overlap(d,sizeof(*d),a,sizeof(*a)))return fail(o,"ST20_OVERLAP_OR_SHAPE",0);
 if(InterlockedCompareExchange(&o->busy,1,0))return fail(o,"ST20_REENTRANT",0);
 if(d->retained||d->locked||o->retained||o->frozen||o->closed||o->stop)goto done;
 if(InterlockedCompareExchange(&d->used,1,0)){fail(o,"ST20_USED",0);goto done;}
 if(!guard(d,a,"BEFORE_LOCK"))goto done;
 if(!a->lock(d->buffer,d->capacity)){fail(o,"ST20_LOCK",GetLastError());goto done;}d->locked=1;
 SecureZeroMemory(d->buffer,d->capacity);d->wiped=1;
 if(!guard(d,a,"BEFORE_GET"))goto cleanup;
 n=d->capacity;d->queried=1;d->wiped=0;
 /* No metadata/value retry, empty-password attempt or other auth type. */
 rc=a->get((TBS_HCONTEXT)d->original_context,TBS_OWNERAUTH_TYPE_STORAGE_20,d->buffer,&n);
 if(rc){fail(o,"ST20_SOURCE_API",rc);goto cleanup;}
 if(!n||n>d->capacity){fail(o,"ST20_ABSENT_OR_SIZE",0);goto cleanup;}
 d->length=n;
 if(!guard(d,a,"BEFORE_USE"))goto cleanup;
 d->delivered=1;use_result=a->use(o,d->buffer,n);
 if(use_result==2){fail(o,"ST20_BORROW_UNKNOWN",0);retain(d);goto done;}
 if(use_result!=1){fail(o,"ST20_CONSUMER_REJECTED_NOT_AUTH_VALID",0);goto cleanup;}
 if(!guard(d,a,"AFTER_USE"))goto cleanup;
 ok=1;
 cleanup:
 if(o->context!=d->original_context)retain(d);
 /* API and synchronous known borrower have returned; no pending use remains. */
 SecureZeroMemory(d->buffer,d->capacity);d->wiped=1;d->length=0;
 if(d->locked){if(!a->unlock(d->buffer,d->capacity)){fail(o,"ST20_UNLOCK_UNKNOWN",GetLastError());retain(d);ok=0;}else d->locked=0;}
 done:if(!d->retained)InterlockedExchange(&o->busy,0);return ok;
}
