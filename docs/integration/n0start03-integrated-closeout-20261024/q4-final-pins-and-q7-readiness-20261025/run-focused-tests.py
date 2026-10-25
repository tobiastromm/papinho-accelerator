from pathlib import Path
import json,subprocess,re
r=Path.cwd();p=Path(__file__).resolve().parent;old=p.parent/'q4-annex-acceptance-and-binding-20261025'
s=(old/'q4-binding-test.c').read_text().replace('T(!PACC_LAB_BINDING_READY);T(!IG_N0_DOSSIER_SHA[0]);T(!IG_N0_EVIDENCE_SHA[0]);T(!IG_N0_N7_ED11_SHA[0]);T(!IG_N0_DOSSIER_VOLUME);T(!IG_N0_EVIDENCE_VOLUME);','T(PACC_LAB_BINDING_READY);T(ig_n0_inputs_ready());T(IG_N0_DOSSIER_BYTES==397);T(IG_N0_EVIDENCE_BYTES==8432);T(IG_N0_DOSSIER_LOW==2932527);T(IG_N0_EVIDENCE_LOW==433780);')
s=s.replace('printf("Q4_BINDING_CASES=', '''{BY_HANDLE_FILE_INFORMATION id;unsigned which,j;DWORD *parts;ZeroMemory(&id,sizeof(id));
 for(which=0;which<2;which++){id.dwVolumeSerialNumber=417614100UL;id.nFileIndexHigh=which?2555904UL:327680UL;id.nFileIndexLow=which?433780UL:2932527UL;id.nFileSizeLow=which?8432UL:397UL;id.nNumberOfLinks=1;T(ig_n0_input_identity(&id,which));
 parts=&id.dwVolumeSerialNumber;T(!ig_n0_input_identity(&id,2));
 for(j=0;j<1;j++){*parts^=1;T(!ig_n0_input_identity(&id,which));*parts^=1;}
 id.nFileIndexLow^=1;T(!ig_n0_input_identity(&id,which));id.nFileIndexLow^=1;
 id.nFileIndexHigh^=1;T(!ig_n0_input_identity(&id,which));id.nFileIndexHigh^=1;
 id.nFileSizeLow++;T(!ig_n0_input_identity(&id,which));id.nFileSizeLow--;
 id.nNumberOfLinks=2;T(!ig_n0_input_identity(&id,which));id.nNumberOfLinks=1;
 id.dwFileAttributes=FILE_ATTRIBUTE_REPARSE_POINT;T(!ig_n0_input_identity(&id,which));id.dwFileAttributes=0;
 id.dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;T(!ig_n0_input_identity(&id,which));id.dwFileAttributes=0;
 id.nFileSizeHigh=1;T(!ig_n0_input_identity(&id,which));id.nFileSizeHigh=0;}}
 printf("Q4_BINDING_CASES=''')
(p/'q4-binding-test.c').write_text(s)
# Compile the actual runtime plan function, not a reimplementation. No process/SCM/native effects.
src=(r/'tools/qualification/scm-n0-context.inc').read_text();a=src.index('#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC\n#define IG_N0_DISPOSITION_PLAN');b=src.index('static DWORD ig_n0_sd',a);plan=src[a:b]
prefix='''#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <bcrypt.h>
#include "../../../../apps/server/server_service_admission_bd_win32.h"
#include "../../../../tools/qualification/scm-n0-context-model.h"
static unsigned ig_n0_positive=1;static const char *ig_n0_approval_sha="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static struct {BY_HANDLE_FILE_INFORMATION dossier_id,evidence_id;} ig_n0;
static size_t rendered;
static int pacc_storage_hash_callback(void *c,const BYTE *b,size_t n,BYTE out[32]){(void)c;rendered=n;return BCryptHash(NULL,NULL,0,NULL,0,NULL,0)==0?0:!!(memset(out,0,32)&&b&&n);}
static void ig_n0_hex(const BYTE b[32],char out[65]){unsigned i;for(i=0;i<32;i++){out[2*i]="0123456789abcdef"[b[i]>>4];out[2*i+1]="0123456789abcdef"[b[i]&15];}out[64]=0;}
'''
# Callback only inspects rendering boundary; hashing itself is existing qualified product backend.
prefix=prefix[:prefix.index('static int pacc_storage_hash_callback')]+'''static char captured[2304];
static int pacc_storage_hash_callback(void *c,const BYTE *b,size_t n,BYTE out[32]){(void)c;rendered=n;if(n>=sizeof(captured))return 0;memcpy(captured,b,n);captured[n]=0;memset(out,1,32);return 1;}
static void ig_n0_hex(const BYTE b[32],char out[65]){unsigned i;for(i=0;i<32;i++){out[2*i]="0123456789abcdef"[b[i]>>4];out[2*i+1]="0123456789abcdef"[b[i]&15];}out[64]=0;}
'''
main='''int main(void){PS_BD_DESCRIPTOR d;char out[65];ZeroMemory(&d,sizeof(d));
 memset(d.attempt,'a',32);memset(d.config_hash,'b',64);memset(d.image_hash,'c',64);memset(d.policy,'d',64);memset(d.host,'e',64);
 memset(d.auxiliary.file_hash,'f',64);memset(d.auxiliary.security_hash,'a',64);memset(d.auxiliary.provenance_hash,'b',64);
 d.boot=d.owner_creation=d.actor.creation=d.challenge=d.auxiliary.instance.creation=~(ULONGLONG)0;
 d.owner_pid=d.actor.pid=d.actor.session=d.control_tid=d.auxiliary.parent=d.auxiliary.instance.pid=d.auxiliary.instance.session=MAXDWORD;
 memset(&d.statistics.TokenId,0xff,sizeof(LUID));memset(&d.statistics.AuthenticationId,0xff,sizeof(LUID));memset(&d.statistics.ModifiedId,0xff,sizeof(LUID));
 memset(&d.config_id,0xff,sizeof(d.config_id));memset(&d.image_id,0xff,sizeof(d.image_id));memset(&d.auxiliary.file_id,0xff,sizeof(d.auxiliary.file_id));memset(&ig_n0,0xff,sizeof(ig_n0));
 if(ig_n0_plan(&d,out)||!strstr(captured,"Q0=97763395")||!strstr(captured,"Q4=590aa1c6")||!strstr(captured,"Q2=5794e172")||!strstr(captured,"N7_ED11=1d6bca53")||strstr(captured,"L1=")||strstr(captured,"P3="))return 1;
 printf("ACTUAL_PLAN_MAX_WIDTH_BYTES=%zu CAPACITY=2304; Q4_Q0_Q2_N7_PRESENT; NO_L1_P3_INHERITANCE\\n",rendered);
 return 0;}
'''
(p/'q4-actual-plan-test.c').write_text(prefix+plan+main)
rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  for test in ['binding','actual-plan']:
   (r/'build/q4-final-binding-20261025'/profile/cfg).mkdir(parents=True,exist_ok=True)
   recipe=(old/f'build-binding-test-{profile}-{cfg}.cmd').read_text().replace('q4-binding-20261025','q4-final-binding-20261025').replace('q4-annex-acceptance-and-binding-20261025','q4-final-pins-and-q7-readiness-20261025')
   if test=='actual-plan':recipe=recipe.replace('q4-binding-test','q4-actual-plan-test')
   cmd=p/f'build-{test}-test-{profile}-{cfg}.cmd';cmd.write_text(recipe);log=p/f'build-{test}-test-{profile}-{cfg}.log'
   with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(cmd)],stdout=out,stderr=subprocess.STDOUT)
   assert q.returncode==0 and not re.search(r'warning (?:C|LNK)\d+',log.read_text(errors='replace')),(cmd,log)
   exe=r/'build/q4-final-binding-20261025'/profile/cfg/f'q4-{test}-test.exe';q=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
   (p/f'{test}-{profile}-{cfg}.log').write_text(q.stdout+q.stderr);assert q.returncode==0,(exe,q.stdout,q.stderr)
   rows.append(dict(profile=profile,config=cfg,test=test,exit=q.returncode,output=q.stdout));print(profile,cfg,test,q.stdout.strip(),flush=True)
(p/'focused-test-results.json').write_text(json.dumps(rows,indent=2))
