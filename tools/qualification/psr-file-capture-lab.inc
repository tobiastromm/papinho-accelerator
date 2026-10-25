/* Private LAB file-sink capsule. Not linked into the host until binding review. */
#ifndef PAPACC_PSR_FILE_CAPSULE_LAB
#error Private LAB compilation only
#endif
#include <windows.h>
#include <string.h>
#define PSRF_BYTES 1024U
#define PSRF_TEXT 960U
typedef struct PSRF_FRAME {
 BYTE magic[16],attempt[32];DWORD version,kind,length,check;BYTE text[PSRF_TEXT];
} PSRF_FRAME;
typedef char PSRF_size_check[(sizeof(PSRF_FRAME)==PSRF_BYTES)?1:-1];
typedef struct PSRF_FILE {
 HANDLE handle;BY_HANDLE_FILE_INFORMATION identity;PSRF_FRAME frame;
 DWORD error;unsigned consumed,retained,created,written,flushed,closed,bound,append_attempted;
} PSRF_FILE;
static const BYTE psrf_magic[16]={'P','A','C','C','_','P','S','R','_','F','I','L','E','0','1',0};
static DWORD psrf_checksum(const PSRF_FRAME *f)
{
 const BYTE *p=(const BYTE *)f;DWORD sum=2166136261UL;unsigned i;
 for(i=0;i<PSRF_BYTES;i++)if(i<(unsigned)FIELD_OFFSET(PSRF_FRAME,check)||i>=(unsigned)FIELD_OFFSET(PSRF_FRAME,check)+sizeof(DWORD)){sum^=p[i];sum*=16777619UL;}
 return sum; /* corruption detection only; ACL/native binding supplies protection */
}
static int psrf_valid(const PSRF_FRAME *f,const BYTE attempt[32],DWORD kind)
{
 unsigned i;if(memcmp(f->magic,psrf_magic,16)||memcmp(f->attempt,attempt,32)||f->version!=1U||f->kind!=kind||f->length>PSRF_TEXT||f->check!=psrf_checksum(f))return 0;
 for(i=f->length;i<PSRF_TEXT;i++)if(f->text[i])return 0;
 return kind?f->length!=0:f->length==0;
}
static DWORD psrf_fail(PSRF_FILE *s,DWORD e)
{if(!s->error)s->error=e?e:ERROR_GEN_FAILURE;if(s->handle&&s->handle!=INVALID_HANDLE_VALUE)s->retained=1U;return s->error;}
static int psrf_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow;}
static DWORD psrf_close(PSRF_FILE *s)
{
 if(s->retained)return ERROR_BUSY;
 if(s->handle){if(!CloseHandle(s->handle))return psrf_fail(s,GetLastError());s->handle=NULL;s->closed=1U;}
 return s->error;
}
static DWORD psrf_write(PSRF_FILE *s)
{
 DWORD n=0; /* current pointer: zero on CREATE_NEW, 1024 after exact arm ReadFile */
 if(!WriteFile(s->handle,&s->frame,PSRF_BYTES,&n,NULL))return psrf_fail(s,GetLastError());
 if(n!=PSRF_BYTES)return psrf_fail(s,ERROR_WRITE_FAULT);
 s->written=1U;if(!FlushFileBuffers(s->handle))return psrf_fail(s,GetLastError());s->flushed=1U;
 return ERROR_SUCCESS;
}
/* Preparation only: caller must hold the approved parent/native binding.
 * Security descriptor is supplied AT CREATION, never changed afterward.
 * No deletion, overwrite, append, truncation, retry or CA/runtime authority. */
#ifndef PAPACC_PSR_FILE_CONSUMER_ONLY
static DWORD psrf_create(PSRF_FILE *s,const WCHAR *path,SECURITY_ATTRIBUTES *security,const BYTE attempt[32])
{
 DWORD e;if(s->consumed||s->handle)return ERROR_INVALID_STATE;s->consumed=1U;
 if(!security||!security->lpSecurityDescriptor||security->bInheritHandle)return psrf_fail(s,ERROR_INVALID_SECURITY_DESCR);
 s->handle=CreateFileW(path,FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,0,security,CREATE_NEW,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(s->handle==INVALID_HANDLE_VALUE){s->handle=NULL;return psrf_fail(s,GetLastError());}s->created=1U;
 if(!GetFileInformationByHandle(s->handle,&s->identity)||s->identity.nNumberOfLinks!=1||(s->identity.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return psrf_fail(s,ERROR_INVALID_DATA);
 ZeroMemory(&s->frame,sizeof(s->frame));memcpy(s->frame.magic,psrf_magic,16);memcpy(s->frame.attempt,attempt,32);s->frame.version=1U;s->frame.check=psrf_checksum(&s->frame);
 e=psrf_write(s);if(e)return e;return psrf_close(s);
}
#endif
/* Service-side one append record, using an already exclusively-created object.
 * Immutable 1024-byte arm + one 1024-byte final = maximum 2048 bytes.
 * Caller must validate protected SD and deliver exact factual native identity.
 * This capsule alone is NOT a path/ACL/authorization producer. */
static DWORD psrf_read_arm(PSRF_FILE *s,const BY_HANDLE_FILE_INFORMATION *expected,const BYTE attempt[32])
{
 DWORD n=0;BY_HANDLE_FILE_INFORMATION id;LARGE_INTEGER size;
 if(!s->handle||s->bound||s->retained||!expected)return psrf_fail(s,ERROR_INVALID_STATE);
 if(!GetFileInformationByHandle(s->handle,&id))return psrf_fail(s,GetLastError());
 if(!psrf_same(&id,expected)||id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||GetFileType(s->handle)!=FILE_TYPE_DISK)return psrf_fail(s,ERROR_INVALID_DATA);
 if(!GetFileSizeEx(s->handle,&size))return psrf_fail(s,GetLastError());if(size.QuadPart!=PSRF_BYTES)return psrf_fail(s,ERROR_INVALID_DATA);
 if(!ReadFile(s->handle,&s->frame,PSRF_BYTES,&n,NULL))return psrf_fail(s,GetLastError());
 if(n!=PSRF_BYTES||!psrf_valid(&s->frame,attempt,0U))return psrf_fail(s,ERROR_INVALID_DATA);
 s->identity=id;s->bound=1U;return ERROR_SUCCESS;
}
static DWORD psrf_finish(PSRF_FILE *s,const BYTE attempt[32],const void *text,DWORD length,DWORD kind,volatile LONG *stop)
{
 DWORD e;BY_HANDLE_FILE_INFORMATION id;
 if(!s->bound||s->retained||s->append_attempted||!s->handle)return psrf_fail(s,ERROR_INVALID_STATE);s->append_attempted=1U;
 if(!length||length>PSRF_TEXT||!text||(kind!=1U&&kind!=2U)||memcmp(s->frame.attempt,attempt,32))return psrf_fail(s,ERROR_INVALID_PARAMETER);
 if(stop&&InterlockedCompareExchange(stop,0,0))return psrf_fail(s,ERROR_CANCELLED);
 if(!GetFileInformationByHandle(s->handle,&id))return psrf_fail(s,GetLastError());if(!psrf_same(&id,&s->identity))return psrf_fail(s,ERROR_INVALID_DATA);
 if(text!=s->frame.text){ZeroMemory(s->frame.text,PSRF_TEXT);memcpy(s->frame.text,text,length);}else ZeroMemory(s->frame.text+length,PSRF_TEXT-length);
 s->frame.length=length;s->frame.kind=kind;s->frame.check=psrf_checksum(&s->frame);
 e=psrf_write(s);if(e)return e;return psrf_close(s);
}
#ifndef PAPACC_PSR_FILE_CONSUMER_ONLY
static DWORD psrf_capture(PSRF_FILE *s,const WCHAR *path,const BY_HANDLE_FILE_INFORMATION *expected,const BYTE attempt[32],const void *text,DWORD length,DWORD kind,volatile LONG *stop)
{
 DWORD e;if(s->consumed||s->handle)return ERROR_INVALID_STATE;s->consumed=1U;
 if(!length||length>PSRF_TEXT||(kind!=1U&&kind!=2U)||!text||!expected)return psrf_fail(s,ERROR_INVALID_PARAMETER);
 if(stop&&InterlockedCompareExchange(stop,0,0))return psrf_fail(s,ERROR_CANCELLED);
 s->handle=CreateFileW(path,FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(s->handle==INVALID_HANDLE_VALUE){s->handle=NULL;return psrf_fail(s,GetLastError());}
 e=psrf_read_arm(s,expected,attempt);if(e)return e;
 return psrf_finish(s,attempt,text,length,kind,stop);
}
#endif
