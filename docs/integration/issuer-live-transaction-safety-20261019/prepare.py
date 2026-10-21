from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/issuer-live-transaction-safety-20261019'
changes={}
def edit(p,fn):
 b=(r/p).read_bytes();(d/'preimages'/p.replace('/','__')).write_bytes(b)
 s=b.decode(); n=fn(s);assert n!=s,p;changes[p]=n.encode()
def put(p,s):
 assert not (r/p).exists();changes[p]=s.encode()
put('src/issuer/issuer_transaction.h','''#ifndef PAPACC_ISSUER_TRANSACTION_H
#define PAPACC_ISSUER_TRANSACTION_H
/* Private nonserialized outcomes. No platform types, epoch or authority.
 * PENDING/UNKNOWN never mean cancellation or failure without effects. */
typedef enum PAPACC_ISSUER_CAS_RESULT {
 PAPACC_COMMIT_IDLE=0,PAPACC_COMMIT_PENDING,PAPACC_CAS_SUCCESS,
 PAPACC_CAS_CONFLICT,PAPACC_CAS_UNKNOWN,PAPACC_COMMIT_FAILED
} PAPACC_ISSUER_CAS_RESULT;
#endif
''')
put('apps/server/server_configuration_pending_win32.h','''#ifndef PAPACC_CONFIGURATION_PENDING_WIN32_H
#define PAPACC_CONFIGURATION_PENDING_WIN32_H
#include "../../src/issuer/issuer_transaction.h"
/* Embedded scratch in the SAME durable root. Worker owns no root pointer,
 * lock, Session, lease, publication callback or operational authority.
 * Inputs frozen before CreateThread; outputs read only after thread signal.
 * 64KiB reserved stack, one thread, no queue, no retry or forced termination. */
#define PAPACC_PENDING_STACK_BYTES 65536U
typedef struct PAPACC_CONFIG_PENDING {
 unsigned char previous[16384],next[16384],observed[16384];
 char path[MAX_PATH];DWORD previous_bytes,native_error,close_error;
 HANDLE thread,file,transaction;volatile LONG stop;
 PAPACC_ISSUER_CAS_RESULT result;int active,committed;
} PAPACC_CONFIG_PENDING;
#endif
''')
edit('apps/server/server_configuration_commit_win32.h',lambda s:s.replace('#include "../../src/issuer/issuer_codec.h"','#include "../../src/issuer/issuer_codec.h"\n#include "server_configuration_pending_win32.h"',1).replace('    int recovery_guard;','    PAPACC_CONFIG_PENDING pending_commit;\n    int authority_lost; /* CAS conflict/unknown is not old CURRENT. */\n    int recovery_guard;',1).replace('/* BODY0.3 is a COMPONENT','PAPACC_RESULT papacc_configuration_issuer_begin06(PAPACC_CONFIGURATION_COMMIT *);\nPAPACC_RESULT papacc_configuration_issuer_poll06(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);\n/* BODY0.3 is a COMPONENT',1))
put('apps/server/server_configuration_pending_win32.inc','''/* Only native persistence runs off-owner. No root pointer reaches worker. */
static void pending_error(PAPACC_CONFIG_PENDING *p)
{if(!p->native_error)p->native_error=GetLastError();}
static DWORD WINAPI pending_worker(void *arg)
{
 PAPACC_CONFIG_PENDING *p=arg;DWORD n;LARGE_INTEGER size;BY_HANDLE_FILE_INFORMATION f;
 int ok=0,conflict=0,wrote=0,rolled=0;
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
 if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;
 if(SetFilePointer(p->file,0,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER){pending_error(p);goto cleanup;}
 wrote=1;
 if(!WriteFile(p->file,p->next,16384,&n,NULL)||n!=16384||!FlushFileBuffers(p->file)){pending_error(p);goto cleanup;}
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
 p->result=conflict?PAPACC_CAS_CONFLICT:PAPACC_COMMIT_FAILED;
 (void)wrote;return 0;
}
PAPACC_RESULT papacc_configuration_issuer_begin06(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_ISSUER_VIEW v;BY_HANDLE_FILE_INFORMATION f;
 if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;
 if(p->active||p->thread||o->authority_lost||o->unresolved||o->stopped||o->recovery_selected||
    !o->published||!o->prepared||!o->issuer_stage_prepared||o->committed_pending||
    (o->store02!=5&&o->store02!=6)||!path_ok(o->path))return PAPACC_RESULT_INVALID_STATE;
 /* Validate existing signed-free ROOT06 codec and immutable base identity.
  * Actual file identity and full CAS are checked again by the native worker. */
 memset(&f,0,sizeof(f));f.dwVolumeSerialNumber=(DWORD)get64(o->committed+104);
 f.nFileIndexHigh=(DWORD)(get64(o->committed+112)>>32);f.nFileIndexLow=(DWORD)get64(o->committed+112);
 if(!issuer06_binding(o,o->committed,o->issuer_staged,&f,&v)||
    v.revision!=o->slots[o->pending].view.revision)return PAPACC_RESULT_INVALID_STATE;
 memset(p,0,sizeof(*p));p->file=INVALID_HANDLE_VALUE;
 memcpy(p->previous,o->committed,16384);memcpy(p->next,o->committed,8192);
 memcpy(p->next+8192,o->issuer_staged,8192);memcpy(p->path,o->path,sizeof(p->path));
 p->previous_bytes=o->store02==6?16384:8192;p->result=PAPACC_COMMIT_PENDING;p->active=1;
 p->thread=CreateThread(NULL,PAPACC_PENDING_STACK_BYTES,pending_worker,p,STACK_SIZE_PARAM_IS_A_RESERVATION,NULL);
 if(!p->thread){p->native_error=GetLastError();p->result=PAPACC_COMMIT_FAILED;p->active=0;return PAPACC_RESULT_INTERNAL_ERROR;}
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_issuer_poll06(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 PAPACC_CONFIG_PENDING *p;DWORD w;
 if(!owner(o)||!out)return PAPACC_RESULT_INVALID_ARGUMENT;p=&o->pending_commit;
 if(!p->active){*out=p->result;return PAPACC_RESULT_INVALID_STATE;}
 w=WaitForSingleObject(p->thread,0);
 if(w==WAIT_TIMEOUT){*out=PAPACC_COMMIT_PENDING;return PAPACC_RESULT_OK;}
 if(w!=WAIT_OBJECT_0){o->authority_lost=1;*out=PAPACC_CAS_UNKNOWN;return PAPACC_RESULT_INTERNAL_ERROR;}
 /* A signaled thread proves no remaining access to embedded buffers. Closing
  * the handle is separate from commit and has its own factual result. */
 if(!CloseHandle(p->thread)){p->close_error=GetLastError();o->authority_lost=1;*out=p->result;return PAPACC_RESULT_INTERNAL_ERROR;}
 p->thread=NULL;p->active=0;*out=p->result;
 if(p->result==PAPACC_CAS_SUCCESS){
  memcpy(o->committed,p->next,16384);memcpy(o->issuer_snapshots[o->pending],p->next+8192,8192);
  o->issuer_snapshot_valid[o->pending]=1;o->store02=6;o->prepared=0;o->issuer_stage_prepared=0;o->committed_pending=1;
 }else if(p->result==PAPACC_CAS_CONFLICT||p->result==PAPACC_CAS_UNKNOWN)o->authority_lost=1;
 if(p->file!=INVALID_HANDLE_VALUE||p->transaction!=NULL||p->close_error)o->unresolved=1;
 return PAPACC_RESULT_OK;
}
''')
# Guard mutating and authority APIs against pending worker; unpin remains legal.
def commit(s):
 s=s.replace('static void put64(','''static int pending_blocks(PAPACC_CONFIGURATION_COMMIT *o)
{
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 return o->pending_commit.active||o->authority_lost;
#else
 (void)o;return 0;
#endif
}
static void put64(''',1)
 s=s.replace('o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected','o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)')
 s=s.replace('!o->prepared||o->stopped||o->unresolved||o->recovery_selected||!path_ok','!o->prepared||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)||!path_ok')
 s=s.replace('!o->committed_pending||o->stopped||o->unresolved','!o->committed_pending||o->stopped||o->unresolved||pending_blocks(o)')
 s=s.replace('!o->prepared||o->committed_pending||o->unresolved','!o->prepared||o->committed_pending||o->unresolved||pending_blocks(o)')
 s=s.replace('!o->published||o->committed_pending||o->stopped||o->unresolved','!o->published||o->committed_pending||o->stopped||o->unresolved||pending_blocks(o)')
 s=s.replace('ok=o->published&&!o->committed_pending&&!o->stopped&&!o->unresolved&&','ok=o->published&&!o->committed_pending&&!o->stopped&&!o->unresolved&&!pending_blocks(o)&&')
 s=s.replace('!o->stopped||o->prepared||o->committed_pending||o->unresolved','!o->stopped||o->prepared||o->committed_pending||o->unresolved||pending_blocks(o)')
 old='{if(!owner(o))return PAPACC_RESULT_INVALID_STATE;EnterCriticalSection(&o->lock);o->stopped=1;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;}'
 new='''{if(!owner(o))return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 (void)InterlockedExchange(&o->pending_commit.stop,1);
#endif
 EnterCriticalSection(&o->lock);o->stopped=1;LeaveCriticalSection(&o->lock);return PAPACC_RESULT_OK;}'''
 assert old in s;s=s.replace(old,new,1)
 s+='\n#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE\n#include "server_configuration_pending_win32.inc"\n#endif\n'
 return s
edit('apps/server/server_configuration_commit_win32.c',commit)
edit('apps/server/server_configuration_issuer06_win32.inc',lambda s:s.replace('o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected','o->prepared||o->committed_pending||o->stopped||o->unresolved||o->recovery_selected||pending_blocks(o)',1))
for p,b in changes.items():(d/'proposed'/p.replace('/','__')).write_bytes(b)
manifest=[]
for p,b in changes.items():
 src=r/p;manifest.append(dict(path=p,before=hashlib.sha256(src.read_bytes()).hexdigest() if src.exists() else None,after=hashlib.sha256(b).hexdigest()))
(d/'operation.json').write_text(json.dumps(manifest,indent=2))
print('Prepared',len(changes),'files; sources untouched')
