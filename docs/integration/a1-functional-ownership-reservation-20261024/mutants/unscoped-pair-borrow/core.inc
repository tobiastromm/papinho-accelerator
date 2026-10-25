/* Private two-slot ownership state in the existing qualifier control thread. */
#ifndef A1_RESERVATION_MODEL_H
#define A1_RESERVATION_MODEL_H
#include <windows.h>
#include <stdint.h>
#define A1_FREE 0U
#define A1_RESERVED 1U
#define A1_OWNED 2U
#define A1_UNKNOWN 3U
#define A1_FROZEN 0x10U
#define A1_STOP 0x20U
#define A1_BUSY 0x40U
#define A1_RETAINED 0x80U
typedef struct {
 HANDLE handle[2]; /* exactly the two existing ownership slots */
 ULONGLONG ticket0,ticket1; /* reject stale leases, including recycled values */
 DWORD state,first_error,first_close_error;
} A1_OWNER;
typedef struct {unsigned slot;ULONGLONG ticket;} A1_LEASE;
typedef DWORD (*A1_ACQUIRE)(void *,HANDLE *);
typedef DWORD (*A1_CLOSE)(void *,HANDLE);
typedef DWORD (*A1_USE)(void *,HANDLE);
static unsigned a1_state(const A1_OWNER *o,unsigned i)
{return (o->state>>(i*2U))&3U;}
static void a1_state_set(A1_OWNER *o,unsigned i,unsigned state)
{o->state=(o->state&~(3U<<(i*2U)))|(state<<(i*2U));}
static ULONGLONG *a1_ticket(A1_OWNER *o,unsigned i)
{return i?&o->ticket1:&o->ticket0;}
static DWORD a1_note(A1_OWNER *o,DWORD e)
{if(e&&!o->first_error)o->first_error=e;return e;}
static DWORD a1_deny(A1_OWNER *o,DWORD e)
{o->state|=A1_STOP;return a1_note(o,e);}
static DWORD a1_gate(const A1_OWNER *o)
{return (o->state&A1_FROZEN)?ERROR_INVALID_STATE:(o->state&A1_BUSY)?ERROR_BUSY:(o->state&A1_STOP)?ERROR_OPERATION_ABORTED:0;}
static void a1_stop(A1_OWNER *o)
{(void)a1_deny(o,ERROR_OPERATION_ABORTED);}
static void a1_retain(A1_OWNER *o,DWORD cause)
{o->state|=A1_FROZEN|A1_RETAINED;(void)a1_deny(o,cause?cause:ERROR_INVALID_STATE);}
static int a1_match(A1_OWNER *o,A1_LEASE l,unsigned state)
{return l.slot<2&&l.ticket&&*a1_ticket(o,l.slot)==l.ticket&&a1_state(o,l.slot)==state;}
static DWORD a1_reserve(A1_OWNER *o,A1_LEASE *l)
{
 unsigned i;DWORD e=a1_gate(o);
 if(e)return a1_deny(o,e);
 if(!l)return a1_deny(o,ERROR_INVALID_PARAMETER);
 for(i=0;i<2;i++)if(a1_state(o,i)==A1_FREE){
  ULONGLONG *ticket=a1_ticket(o,i);
  if(*ticket==~(ULONGLONG)0)return a1_deny(o,ERROR_ARITHMETIC_OVERFLOW);
  ++*ticket;l->slot=i;l->ticket=*ticket;a1_state_set(o,i,A1_RESERVED);return 0;
 }
 return a1_deny(o,ERROR_TOO_MANY_OPEN_FILES);
}
/* Atomic at the single control-thread boundary: no callback between checks. */
static DWORD a1_reserve_pair(A1_OWNER *o,A1_LEASE *a,A1_LEASE *b)
{
 DWORD e=a1_gate(o);if(e)return a1_deny(o,e);
 if(!a||!b||a==b)return a1_deny(o,ERROR_INVALID_PARAMETER);
 if(a1_state(o,0)!=A1_FREE||a1_state(o,1)!=A1_FREE)return a1_deny(o,ERROR_TOO_MANY_OPEN_FILES);
 if(o->ticket0==~(ULONGLONG)0||o->ticket1==~(ULONGLONG)0)return a1_deny(o,ERROR_ARITHMETIC_OVERFLOW);
 ++o->ticket0;++o->ticket1;a->slot=0;a->ticket=o->ticket0;b->slot=1;b->ticket=o->ticket1;
 a1_state_set(o,0,A1_RESERVED);a1_state_set(o,1,A1_RESERVED);return 0;
}
static DWORD a1_cancel(A1_OWNER *o,A1_LEASE l)
{
 if(o->state&(A1_FROZEN|A1_BUSY))return a1_deny(o,ERROR_INVALID_STATE);
 if(!a1_match(o,l,A1_RESERVED)||o->handle[l.slot])return a1_deny(o,ERROR_INVALID_PARAMETER);
 a1_state_set(o,l.slot,A1_FREE);return 0;
}
/* The model adapter writes DIRECTLY to owner storage, never a local owner. */
static DWORD a1_acquire(A1_OWNER *o,A1_LEASE l,A1_ACQUIRE api,void *arg,DWORD expected_absence)
{
 DWORD e,gate=a1_gate(o);HANDLE h;
 if(gate)return a1_deny(o,gate);
 if(!api||!a1_match(o,l,A1_RESERVED)||o->handle[l.slot])return a1_deny(o,ERROR_INVALID_PARAMETER);
 o->state|=A1_BUSY;e=api(arg,&o->handle[l.slot]);o->state&=~A1_BUSY;h=o->handle[l.slot];
 if((!e&&(!h||h==INVALID_HANDLE_VALUE))||(e&&h)){
  a1_state_set(o,l.slot,A1_UNKNOWN);o->state|=A1_FROZEN|A1_RETAINED;
  return a1_deny(o,e?e:ERROR_INVALID_DATA);
 }
 if(e){a1_state_set(o,l.slot,A1_FREE);if(e!=expected_absence)(void)a1_deny(o,e);return e;}
 a1_state_set(o,l.slot,A1_OWNED);
 return (o->state&A1_STOP)?(o->first_error?o->first_error:a1_note(o,ERROR_OPERATION_ABORTED)):0;
}
/* Scoped borrow: BUSY stays set for the ENTIRE native-use interval. */
static DWORD a1_borrow_begin(A1_OWNER *o,A1_LEASE l,HANDLE *out)
{
 DWORD e=a1_gate(o);if(e)return a1_deny(o,e);
 if(!out||!a1_match(o,l,A1_OWNED))return a1_deny(o,ERROR_INVALID_PARAMETER);
 o->state|=A1_BUSY;*out=o->handle[l.slot];return 0;
}
static DWORD a1_borrow_pair_begin(A1_OWNER *o,A1_LEASE a,A1_LEASE b,HANDLE *in,HANDLE *out)
{
 DWORD e=a1_gate(o);if(e)return a1_deny(o,e);
 if(!in||!out||in==out||a.slot==b.slot||!a1_match(o,a,A1_OWNED)||!a1_match(o,b,A1_OWNED))return a1_deny(o,ERROR_INVALID_PARAMETER);
 *in=o->handle[a.slot];*out=o->handle[b.slot];return 0;
}
static DWORD a1_borrow_end(A1_OWNER *o,DWORD result)
{
 if(!(o->state&A1_BUSY))return a1_deny(o,ERROR_INVALID_STATE);
 o->state&=~A1_BUSY;if(result)(void)a1_deny(o,result);
 return o->first_error&&(o->state&A1_STOP)?o->first_error:result;
}
#ifdef A1_ENABLE_CALLBACK_TEST_HELPER
static DWORD a1_use(A1_OWNER *o,A1_LEASE l,A1_USE use,void *arg)
{
 HANDLE h;DWORD e;if(!use)return a1_deny(o,ERROR_INVALID_PARAMETER);
 e=a1_borrow_begin(o,l,&h);if(e)return e;
 e=use(arg,h);return a1_borrow_end(o,e);
}
#endif
static DWORD a1_close(A1_OWNER *o,A1_LEASE l,A1_CLOSE close,void *arg)
{
 DWORD e;
 /* Stop alone permits checked close; UNKNOWN prohibits dependent cleanup. */
 if(o->state&(A1_FROZEN|A1_BUSY))return a1_deny(o,ERROR_INVALID_STATE);
 if(!close||!a1_match(o,l,A1_OWNED))return a1_deny(o,ERROR_INVALID_PARAMETER);
 o->state|=A1_BUSY;e=close(arg,o->handle[l.slot]);o->state&=~A1_BUSY;
 if(e){a1_state_set(o,l.slot,A1_UNKNOWN);o->state|=A1_FROZEN|A1_RETAINED;
  if(!o->first_close_error)o->first_close_error=e;
  return a1_deny(o,e);
 }
 o->handle[l.slot]=NULL;a1_state_set(o,l.slot,A1_FREE);return 0;
}
static int a1_finish(const A1_OWNER *o)
{return !(o->state&(A1_FROZEN|A1_BUSY|A1_RETAINED))&&a1_state(o,0)==A1_FREE&&a1_state(o,1)==A1_FREE;}
#endif
