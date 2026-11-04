#include "storage20_binding.h"
#include <string.h>
static int overlaps(const void *a,SIZE_T n,const void *b,SIZE_T m){ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x<=y?y-x<n:x-y<m;}
static int nonzero(const unsigned char *p){unsigned i,n=0;for(i=0;i<32;i++)n|=p[i];return n!=0;}
ST20_GET st20_windows_source(HMODULE original)
{
 wchar_t path[MAX_PATH],expected[MAX_PATH];UINT n;DWORD got;
 if(!original)return NULL;got=GetModuleFileNameW(original,path,MAX_PATH);
 n=GetSystemDirectoryW(expected,MAX_PATH);
 if(!got||got>=MAX_PATH||!n||n>MAX_PATH-9U)return NULL;
 memcpy(expected+n,L"\\tbs.dll",9U*sizeof(wchar_t));
 if(_wcsicmp(path,expected))return NULL;
 return (ST20_GET)(void *)GetProcAddress(original,"Tbsi_Get_OwnerAuth");
}
int st20_root_guard(TP_OWNER *o,const ST20_DELIVERY *d,const char *stage)
{
 ST20_ROOT_BINDING *b;PAPACC_Q4_ST20_DESIGNATION_DATA now;
 if(!o||!d||d->owner!=o||!d->guard_context||!o->boundary||o->owner_thread!=GetCurrentThreadId())return 0;
 b=(ST20_ROOT_BINDING *)d->guard_context;
 if(!b->root_owner||!b->pin||!b->designated_source||d->used_source!=b->designated_source||!nonzero(b->request_digest)||!nonzero(b->approval_digest))return 0;
 if(overlaps(d->buffer,d->capacity,b,sizeof(*b))||overlaps(d->buffer,d->capacity,b->root_owner,sizeof(*b->root_owner))||overlaps(d->buffer,d->capacity,b->pin,sizeof(*b->pin)))return 0;
 if(papacc_configuration_q4_st20_designation_data(b->root_owner,b->pin,&now)!=PAPACC_RESULT_OK)return 0;
 if(memcmp(&now,&b->expected,sizeof(now))||memcmp(now.custodian_sid,b->current_sid,sizeof(now.custodian_sid))||
  memcmp(now.ek_spki,b->current_ek,32)||memcmp(now.certificate,b->current_certificate,32))return 0;
 /* A digest/declared SID is not authority: original owner boundary must validate
  * native source/module/token/lifetime/resources AND exact operational approval.
  * No such production producer is installed by this DATA binding. */
 if(!o->boundary(o,stage)||o->frozen||o->retained||o->stop||o->closed)return 0;
 return papacc_configuration_q4_binding_current(b->root_owner,b->pin,&now.root)==PAPACC_RESULT_OK;
}
