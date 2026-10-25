/* Read-only diagnostic, NOT an adapter/source of qualified UTC.
 * No networking, synchronization, service changes, clock changes, allocation,
 * thread, wait, owned native handle or persistent state. SDK Windows only. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "qualified_utc.h"
typedef VOID (WINAPI *PRECISE_TIME)(LPFILETIME);
typedef BOOL (WINAPI *PRECISE_ADJUSTMENT)(PDWORD64,PDWORD64,PBOOL);
typedef VOID (WINAPI *PRECISE_INTERRUPT)(PULONGLONG);
static ULONGLONG ticks(FILETIME f)
{
 ULARGE_INTEGER u;u.LowPart=f.dwLowDateTime;u.HighPart=f.dwHighDateTime;return u.QuadPart;
}
int main(void)
{
 HMODULE module;FARPROC address;PRECISE_TIME precise=NULL;
 PRECISE_ADJUSTMENT adjustment=NULL;PRECISE_INTERRUPT unbiased=NULL;
 LARGE_INTEGER frequency,before,after,last;FILETIME wall,precise_wall;
 DWORD old_adjust=0,increment=0;DWORD64 adjust=0,precise_increment=0;
 BOOL disabled=FALSE,precise_disabled=FALSE;BOOL old_ok,precise_ok=FALSE;
 ULONGLONG previous_wall=0,previous_precise=0,interrupt_value=0;
 unsigned i,regressions=0,wall_regressions=0,precise_regressions=0;
 PAPACC_UTC_OBSERVATION output,saved;
 module=GetModuleHandleA("kernel32.dll");if(!module)return 2;
 address=GetProcAddress(module,"GetSystemTimePreciseAsFileTime");
 if(address){if(sizeof(address)!=sizeof(precise))return 2;memcpy(&precise,&address,sizeof(precise));}
 address=GetProcAddress(module,"GetSystemTimeAdjustmentPrecise");
 if(address){if(sizeof(address)!=sizeof(adjustment))return 2;memcpy(&adjustment,&address,sizeof(adjustment));}
 address=GetProcAddress(module,"QueryUnbiasedInterruptTimePrecise");
 if(address){if(sizeof(address)!=sizeof(unbiased))return 2;memcpy(&unbiased,&address,sizeof(unbiased));}
 if(!QueryPerformanceFrequency(&frequency)||frequency.QuadPart<=0||!QueryPerformanceCounter(&last))return 2;
 old_ok=GetSystemTimeAdjustment(&old_adjust,&increment,&disabled);
 if(adjustment)precise_ok=adjustment(&adjust,&precise_increment,&precise_disabled);
 for(i=0;i<256;++i){
  if(!QueryPerformanceCounter(&before))return 2;
  GetSystemTimeAsFileTime(&wall);precise_wall=wall;
  if(precise)precise(&precise_wall);
  if(unbiased)unbiased(&interrupt_value);
  if(!QueryPerformanceCounter(&after))return 2;
  if(before.QuadPart<last.QuadPart||after.QuadPart<before.QuadPart)++regressions;
  if(i&&ticks(wall)<previous_wall)++wall_regressions;
  if(i&&ticks(precise_wall)<previous_precise)++precise_regressions;
  previous_wall=ticks(wall);previous_precise=ticks(precise_wall);last=after;
 }
 memset(&output,0x5a,sizeof(output));saved=output;
 if(papacc_issuer_qualified_utc_read(NULL,&output)!=PAPACC_RESULT_NOT_SUPPORTED||memcmp(&output,&saved,sizeof(output)))return 3;
 printf("RAW_SAMPLES=256 QPC_FREQUENCY=%I64d QPC_REGRESSIONS=%u WALL_REGRESSIONS=%u PRECISE_REGRESSIONS=%u\n",frequency.QuadPart,regressions,wall_regressions,precise_regressions);
 printf("PRECISE_TIME_AVAILABLE=%u PRECISE_ADJUSTMENT_AVAILABLE=%u UNBIASED_PRECISE_AVAILABLE=%u\n",precise!=NULL,adjustment!=NULL,unbiased!=NULL);
 printf("ADJUSTMENT_READ_OK=%u INCREMENT_100NS=%lu ADJUSTMENT_100NS=%lu PERIODIC_DISABLED=%u\n",old_ok!=FALSE,increment,old_adjust,disabled!=FALSE);
 printf("PRECISE_ADJUSTMENT_READ_OK=%u INCREMENT_100NS=%I64u ADJUSTMENT_100NS=%I64u PERIODIC_DISABLED=%u\n",precise_ok!=FALSE,precise_increment,adjust,precise_disabled!=FALSE);
 printf("QUALIFIED_SOURCE=NOT_SUPPORTED OUTPUT_UNCHANGED=YES AUTHENTICITY=UNKNOWN UTC_ERROR=UNKNOWN RHO=UNKNOWN PAL_E=UNKNOWN CONTINUITY=UNKNOWN\n");
 return regressions?4:0;
}
