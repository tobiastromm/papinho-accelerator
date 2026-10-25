/* Private fixed-attempt admission, not a new quota/authority. */
#ifndef PACC_INSTRUMENTED_POLICY_H
#define PACC_INSTRUMENTED_POLICY_H
#include <windows.h>
#include "../../apps/server/server_service_admission_bd_win32.h"
#define IG_ATTEMPT "0b1de869e11493b7cb08fb329578f6ce"
#define IG_ID_ALL 511U
#define IG_GIB 1073741824ULL
#define IG_MIB 1048576ULL
typedef struct IG_PROCESS {ULONGLONG private_bytes,commit,working_set,stack;DWORD handles,threads,error;unsigned powershell;} IG_PROCESS;
typedef struct IG_SAMPLE {ULONGLONG oldest,physical,commit;IG_PROCESS process[4];unsigned count,identity;DWORD error;unsigned bd;PS_BD_DESCRIPTOR binding;PS_BD_INVENTORY inventory;} IG_SAMPLE;
typedef struct IG_GATE {ULONGLONG preflight,last;unsigned preflight_valid,consumed,starts,stops,retained,quiescent;DWORD first;} IG_GATE;
/* Pure policy helpers: supplied data is not a qualified native readback. */
static __inline DWORD ig_recovery_policy(DWORD query_error,DWORD action_count,const SC_ACTION *actions,BOOL failure_flag,DWORD triggers,int command_present)
{
 DWORD i;if(query_error)return query_error;if(action_count>32U||(action_count&&!actions))return ERROR_INVALID_DATA;
 if(failure_flag||triggers||command_present)return ERROR_ACCESS_DENIED;
 for(i=0;i<action_count;i++)if(actions[i].Type!=SC_ACTION_NONE)return ERROR_ACCESS_DENIED;
 return 0;
}
static __inline int ig_n0_positive_command(int argc,char **argv)
{return argc==9&&argv&&argv[1]&&!strcmp(argv[1],"--cycle-n0");}
static DWORD ig_fail(IG_GATE *g,DWORD e){if(!g->first)g->first=e;return e;}
static DWORD ig_metric(const IG_SAMPLE *s,int admission)
{
 unsigned i;ULONGLONG sum=0;DWORD handles=0,threads=0;
 if(s->error||!s->count||s->count>4)return s->error?s->error:ERROR_INVALID_DATA;
 if(s->identity!=IG_ID_ALL)return ERROR_INVALID_OWNER;
 if(s->physical<(admission?5:4)*IG_GIB||s->commit<(admission?9:8)*IG_GIB)return ERROR_NOT_ENOUGH_MEMORY;
 if(s->bd){
  PS_BD_COST c[PS_BD_ROLES];DWORD e=ps_bd_inventory(&s->binding,&s->inventory,s->oldest);
  if(e)return e;if(s->count!=s->inventory.count)return ERROR_INVALID_DATA;ZeroMemory(c,sizeof(c));
  for(i=0;i<PS_BD_ROLES;i++)if(s->inventory.present[i]){
   const IG_PROCESS *p=&s->process[i];c[i].private_bytes=p->private_bytes;c[i].commit=p->commit;c[i].working_set=p->working_set;c[i].stack=p->stack;c[i].handles=p->handles;c[i].threads=p->threads;c[i].error=p->error;
  }
  return ps_bd_costs(&s->inventory,c);
 }
 for(i=0;i<s->count;i++){
 const IG_PROCESS *p=&s->process[i];ULONGLONG cap=(p->powershell?256:64)*IG_MIB;
 if(p->error)return p->error;
 if(p->private_bytes>cap||p->commit>cap||p->working_set>cap||p->stack>(p->powershell?32:8)*IG_MIB||
 p->handles>(p->powershell?1024U:128U)||!p->threads||p->threads>(p->powershell?64U:16U))return ERROR_NOT_ENOUGH_QUOTA;
 if(sum>671088640ULL-p->private_bytes)return ERROR_ARITHMETIC_OVERFLOW;sum+=p->private_bytes;handles+=p->handles;threads+=p->threads;
 }
 return sum<=671088640ULL&&handles<=2304U&&threads<=160U?ERROR_SUCCESS:ERROR_NOT_ENOUGH_QUOTA;
}
static DWORD ig_preflight(IG_GATE *g,const IG_SAMPLE *s,ULONGLONG now)
{DWORD e;if(g->consumed||g->retained||g->preflight_valid)return ig_fail(g,ERROR_INVALID_STATE);if(now<s->oldest||now-s->oldest>60000U)return ig_fail(g,ERROR_TIMEOUT);e=ig_metric(s,1);if(e)return ig_fail(g,e);g->preflight=s->oldest;g->preflight_valid=1;return 0;}
static DWORD ig_final(IG_GATE *g,const IG_SAMPLE *s,ULONGLONG now)
{DWORD e;if(!g->preflight_valid||g->consumed||g->retained)return ig_fail(g,ERROR_INVALID_STATE);if(now<g->preflight||now-g->preflight>60000U||now<s->oldest||now-s->oldest>1000U)return ig_fail(g,ERROR_TIMEOUT);e=ig_metric(s,1);return e?ig_fail(g,e):0;}
static DWORD ig_consume(IG_GATE *g)
{if(g->consumed||g->retained||!g->preflight_valid)return ig_fail(g,ERROR_INVALID_STATE);g->consumed=1;return 0;}
static DWORD ig_start(IG_GATE *g,const IG_SAMPLE *s,ULONGLONG now)
{DWORD e;if(!g->consumed||g->starts||g->retained)return ig_fail(g,ERROR_INVALID_STATE);if(now<g->preflight||now-g->preflight>60000U||now<s->oldest||now-s->oldest>1000U)return ig_fail(g,ERROR_TIMEOUT);e=ig_metric(s,1);if(e)return ig_fail(g,e);g->starts=1;g->last=now;return 0;}
static DWORD ig_monitor(IG_GATE *g,const IG_SAMPLE *s,ULONGLONG now)
{DWORD e;if(!g->starts||g->quiescent||g->retained)return ig_fail(g,ERROR_INVALID_STATE);if(now<g->last||now-g->last>1000U)return ig_fail(g,ERROR_TIMEOUT);e=ig_metric(s,0);g->last=now;return e?ig_fail(g,e):0;}
static DWORD ig_stop(IG_GATE *g)
{if(!g->starts||g->stops||g->quiescent)return ig_fail(g,ERROR_INVALID_STATE);g->stops=1;return 0;}
static int ig_retire(const IG_GATE *g){return !g->retained&&(!g->starts||g->quiescent);}
#endif
