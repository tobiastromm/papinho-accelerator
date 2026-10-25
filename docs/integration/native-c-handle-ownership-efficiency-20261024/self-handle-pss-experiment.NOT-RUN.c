/* REVIEW ARTIFACT ONLY. NOT BUILT/EXECUTED. Requires separate owner approval.
 * Self-process duplicate references/PSS only; no SCM/files/intent/LAB/threads.
 * Does NOT reproduce or admit Accelerator. Output is sampled, not universal peak.
 */
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0603
#include <windows.h>
#include <processsnapshot.h>
#include <stdio.h>
static HANDLE refs[6];static HPSS snapshot;static HPSSWALK marker;static DWORD first;
static void retain(const char *stage,DWORD error)
{
 if(!first)first=error?error:ERROR_GEN_FAILURE;
 printf("FIXTURE_UNKNOWN stage=%s first=%lu; PRESERVE_OWNER_NO_RETRY\n",stage,(unsigned long)first);fflush(stdout);
 for(;;)Sleep(1000);
}
static void sample(const char *stage)
{
 DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))retain(stage,GetLastError());
 printf("SELF_ONLY stage=%s sampled_handles=%lu; NOT_ACCELERATOR_NOT_PEAK\n",stage,(unsigned long)n);fflush(stdout);
}
int main(void)
{
 unsigned i;DWORD e;sample("baseline");
 for(i=0;i<6;i++){
  if(!DuplicateHandle(GetCurrentProcess(),GetCurrentProcess(),GetCurrentProcess(),&refs[i],PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,0))retain("duplicate",GetLastError());
  sample("duplicate-held");
 }
 e=PssCaptureSnapshot(refs[0],(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_THREADS|PSS_CAPTURE_THREAD_CONTEXT),CONTEXT_CONTROL,&snapshot);
 if(e)retain("capture",e);sample("capture-held");
 e=PssWalkMarkerCreate(NULL,&marker);if(e)retain("marker",e);sample("marker-held");
 e=PssWalkMarkerFree(marker);if(e)retain("marker-close",e);marker=NULL;sample("marker-closed");
 e=PssFreeSnapshot(GetCurrentProcess(),snapshot);if(e)retain("capture-close",e);snapshot=NULL;sample("capture-closed");
 for(i=6;i>0;i--){if(!CloseHandle(refs[i-1]))retain("duplicate-close",GetLastError());refs[i-1]=NULL;sample("duplicate-closed");}
 puts("SELF_FIXTURE_RETURN; NO_SCM_NO_START_NO_FILES; NO_RETRY");return 0;
}
