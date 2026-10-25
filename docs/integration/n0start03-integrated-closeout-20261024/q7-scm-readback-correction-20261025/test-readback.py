from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=Path(__file__).resolve().parent;old=p.parent/'q4-annex-acceptance-and-binding-20261025';src=(r/'tools/qualification/scm-runtime-cycle-lab.inc').read_text();a=src.index('/* Exact preserved historical predecessor');b=src.index('static DWORD cycle_confirm(',a);actual=src[a:b]
prefix='''#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static struct {WCHAR command[1050];struct {struct {DWORD dwErrorControl;} aligned;} query;} native;
static const WCHAR cycle_path[]=L"C:\\\\fixture\\\\new10584.exe";
static const WCHAR *observed;static DWORD injected;static int calls,append_calls,fail_append;
static int lab_append(WCHAR *out,size_t cap,const WCHAR *text){size_t n=wcslen(out),m=wcslen(text);if(++append_calls==fail_append||n>=cap||m>=cap-n)return 0;memcpy(out+n,text,(m+1)*sizeof(WCHAR));return 1;}
static DWORD ps_virtual_readback_start(DWORD startup){calls++;if(injected)return injected;if(startup!=SERVICE_DEMAND_START||wcscmp(native.command,observed))return ERROR_INVALID_DATA;return 0;}
#define T(x) do{cases++;if(!(x)){printf("FAIL %u line%d\\n",cases,__LINE__);return 1;}}while(0)
'''
main='''int main(void){unsigned cases=0;int i;WCHAR preserved[1050];
 wcscpy_s(native.command,1050,L"\\\"C:\\\\old-generic.exe\\\" service host");memcpy(preserved,native.command,sizeof(preserved));native.query.aligned.dwErrorControl=SERVICE_ERROR_NORMAL;
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
 observed=L"\\\"C:\\\\Projetos\\\\PapinhoAccelerator\\\\build\\\\scm-protected-installation-binding-20261022\\\\Selected\\\\Release\\\\papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe\\\" service host";
#else
 observed=preserved;
#endif
 T(cycle_readback(SERVICE_DEMAND_START,0)==0);T(!memcmp(native.command,preserved,sizeof(preserved)));T(calls==1);
 observed=L"\\\"C:\\\\fixture\\\\new10584.exe\\\" service host";T(cycle_readback(SERVICE_DEMAND_START,1)==0);T(!memcmp(native.command,preserved,sizeof(preserved)));
 observed=L"\\\"C:\\\\other.exe\\\" service host";T(cycle_readback(SERVICE_DEMAND_START,0)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));
 injected=ERROR_ACCESS_DENIED;T(cycle_readback(SERVICE_DEMAND_START,0)==ERROR_ACCESS_DENIED);injected=0;
 observed=L"\\\"C:\\\\fixture\\\\new10584.exe\\\" service host";native.query.aligned.dwErrorControl=2;T(cycle_readback(SERVICE_DEMAND_START,1)==ERROR_INVALID_DATA);native.query.aligned.dwErrorControl=SERVICE_ERROR_NORMAL;
 for(i=1;i<=3;i++){append_calls=0;fail_append=i;T(cycle_readback(SERVICE_DEMAND_START,1)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));}fail_append=0;
 T(cycle_readback(SERVICE_DISABLED,1)==ERROR_INVALID_DATA);T(!memcmp(native.command,preserved,sizeof(preserved)));
 printf("ACTUAL_CYCLE_READBACK=PASS cases=%u; MOCK_SCM_ONLY_NO_HANDLES\\n",cases);return 0;}
'''
(p/'actual-readback-test.c').write_text(prefix+actual+main);rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  for mode in ['Q4','default']:
   d=r/'build/q7-readback-unit-20261025'/profile/cfg/mode;d.mkdir(parents=True,exist_ok=True)
   recipe=(old/f'build-binding-test-{profile}-{cfg}.cmd').read_text();recipe=recipe.replace('q4-annex-acceptance-and-binding-20261025\\q4-binding-test.c','q7-scm-readback-correction-20261025\\actual-readback-test.c').replace('q4-binding-20261025\\'+profile+'\\'+cfg, 'q7-readback-unit-20261025\\'+profile+'\\'+cfg+'\\'+mode)
   if mode=='default':recipe=recipe.replace('/DPAPACC_Q4_TOKEN_DIAGNOSTIC=1','')
   script=p/f'unit-{profile}-{cfg}-{mode}.cmd';script.write_text(recipe);q=subprocess.run(['cmd','/c',str(script)],capture_output=True);(p/f'unit-build-{profile}-{cfg}-{mode}.log').write_bytes(q.stdout+q.stderr);assert q.returncode==0 and not re.search(rb'warning (?:C|LNK)\d+',q.stdout),(script,q.stdout[-1200:])
   q=subprocess.run([str(d/'q4-binding-test.exe')],capture_output=True,text=True,timeout=20);assert q.returncode==0,(profile,cfg,mode,q.stdout,q.stderr);rows.append(dict(profile=profile,config=cfg,mode=mode,exit=q.returncode,output=q.stdout));print(profile,cfg,mode,q.stdout.strip(),flush=True)
(p/'actual-readback-test-results.json').write_text(json.dumps(rows,indent=2)+'\n')
