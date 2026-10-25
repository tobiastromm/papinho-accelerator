# Read functional sources; write ONLY review patch. NEVER apply.
from pathlib import Path
import re,difflib,json
ROOT=Path(__file__).resolve().parents[3];PKG=Path(__file__).resolve().parent
names=['tools/qualification/scm-registration-lab.c','tools/qualification/scm-runtime-cycle-lab.inc','tools/qualification/scm-instrumented-cycle-lab.inc','tools/qualification/scm-instrumented-observer.inc','tools/qualification/scm-n0-context.inc','apps/server/server_service_admission_bd_win32.inc']
old={n:(ROOT/n).read_text() for n in names};new=dict(old)
def put(n,a,b):
 assert new[n].count(a)==1,(n,a,new[n].count(a));new[n]=new[n].replace(a,b)
def edit(n,fn,change):
 pattern=r'(?ms)^((?:static )?(?:DWORD|int) '+re.escape(fn)+r'\([^\n]*\)\s*\{)(.*?)(^\})'
 matches=list(re.finditer(pattern,new[n]));assert len(matches)==1,(n,fn)
 m=matches[0];body=m.group(2);assert not re.search(r'(?m)^static DWORD ',body),(n,fn,'function boundary')
 new[n]=new[n][:m.start(2)]+change(body)+new[n][m.end(2):]
def guard(n,fn,hook=False):
 g='\n#ifdef PAPACC_AUX_RESERVATION_GUARD\n {DWORD aux_guard=PAPACC_AUX_RESERVATION_GUARD();if(aux_guard)return aux_guard;}\n#endif\n' if hook else '\n {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}\n'
 edit(n,fn,lambda s:g+s)
r,c,i,o,n,bd=names
put(r,'#define PAPACC_SCM_REGISTRATION_LAB 1','#define PAPACC_SCM_REGISTRATION_LAB 1\nstatic DWORD lab_aux_gate(void);\n#define PAPACC_AUX_RESERVATION_GUARD() lab_aux_gate()')
put(r,'static HANDLE lab_aux_retained[2];static unsigned lab_aux_count;\nstatic int lab_close_aux(HANDLE h){if(CloseHandle(h))return 1;if(lab_aux_count<2)lab_aux_retained[lab_aux_count++]=h;return 0;}','#include "scm-aux-reservation-core.inc"\n#include "scm-aux-reservation-bridge.inc"')
def oper(s,current):
 if current:
  s=s.replace('HANDLE token=NULL,thread=NULL;','HANDLE token=NULL;A1_LEASE token_lease,thread_lease;')
  s=s.replace('if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){if(!lab_close_aux(thread))return ERROR_INVALID_HANDLE;return ERROR_ACCESS_DENIED;}\n if(GetLastError()!=ERROR_NO_TOKEN)return lab_error();','e=lab_aux_token(&thread_lease,GetCurrentThread(),1,TOKEN_QUERY);if(!e)return lab_aux_close_preserving(thread_lease,ERROR_ACCESS_DENIED);\n if(e!=ERROR_NO_TOKEN)return e;')
  s=s.replace('if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return lab_error();','if((e=lab_aux_token(&token_lease,GetCurrentProcess(),0,TOKEN_QUERY)))return e;\n if((e=lab_aux_borrow(token_lease,&token)))return lab_aux_close_preserving(token_lease,e);')
  s=s.replace('if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;return e;','return lab_aux_close_preserving(token_lease,e);')
 else:
  s=s.replace('HANDLE th=NULL;','A1_LEASE th;DWORD e;')
  s=s.replace('if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&th)){if(!lab_close_aux(th))return ERROR_INVALID_HANDLE;return ERROR_ACCESS_DENIED;}\n if(GetLastError()!=ERROR_NO_TOKEN)return lab_error();','e=lab_aux_token(&th,GetCurrentThread(),1,TOKEN_QUERY);if(!e)return lab_aux_close_preserving(th,ERROR_ACCESS_DENIED);\n if(e!=ERROR_NO_TOKEN)return e;')
 return s
edit(r,'lab_operator',lambda s:oper(s,False));edit(r,'lab_operator_current',lambda s:oper(s,True))
def console(s):
 s=s.replace('HANDLE in,out;','HANDLE in=NULL,out=NULL;A1_LEASE in_lease,out_lease;');a=s.index(' in=CreateFileW(');b=s.index(' if(!GetConsoleMode',a)
 s=s[:a]+' if((e=lab_aux_console(&in_lease,&out_lease)))return e;\n if((e=lab_aux_borrow(in_lease,&in))||(e=lab_aux_borrow(out_lease,&out)))goto done;\n'+s[b:]
 return s.replace('if(!lab_close_aux(in))e=ERROR_INVALID_HANDLE;if(!lab_close_aux(out))e=ERROR_INVALID_HANDLE;return e;','e=lab_aux_close_preserving(in_lease,e);return lab_aux_close_preserving(out_lease,e);')
for file,fn in [(r,'lab_confirm'),(c,'cycle_confirm'),(i,'ig_confirm')]:edit(file,fn,console)
def file_caller(s):
 s=s.replace('HANDLE h;','HANDLE h=NULL;A1_LEASE lease;').replace('DWORD n;char text[128]','DWORD n,e;char text[128]').replace(';\n if(h==INVALID_HANDLE_VALUE)',';if(h==INVALID_HANDLE_VALUE)')
 m=re.search(r'h=CreateFileW\((.+?),(.+?),(.+?),NULL,(.+?),(.+?),NULL\);if\(h==INVALID_HANDLE_VALUE\)return lab_error\(\);',s);assert m,s[:300]
 path,access,share,disp,flags=m.groups();replacement=f'if((e=lab_aux_file(&lease,{path},{access},{share},{disp},{flags})))return e;\n if((e=lab_aux_borrow(lease,&h)))return lab_aux_close_preserving(lease,e);'
 s=s[:m.start()]+replacement+s[m.end():]
 s=s.replace('if(!lab_close_aux(h))e=ERROR_INVALID_HANDLE;return e;','return lab_aux_close_preserving(lease,e);').replace('DWORD e=lab_error();if(!lab_close_aux(h))return ERROR_INVALID_HANDLE;return e;','e=lab_error();return lab_aux_close_preserving(lease,e);').replace('return lab_close_aux(h)?ERROR_SUCCESS:ERROR_INVALID_HANDLE;','return lab_aux_close_preserving(lease,0);').replace('if(!lab_close_aux(h))return ERROR_INVALID_HANDLE;return e;','return lab_aux_close_preserving(lease,e);')
 return s
for file,fn in [(r,'lab_intent'),(c,'cycle_intent'),(i,'ig_extract')]:edit(file,fn,file_caller)
def live(s):
 return s.replace('HANDLE token=NULL;','A1_LEASE token;DWORD token_error;').replace('if(OpenThreadToken(cycle_thread,TOKEN_QUERY,TRUE,&token)){if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;return ERROR_ACCESS_DENIED;}\n if(GetLastError()!=ERROR_NO_TOKEN)return lab_error();','token_error=lab_aux_token(&token,cycle_thread,1,TOKEN_QUERY);if(!token_error)return lab_aux_close_preserving(token,ERROR_ACCESS_DENIED);\n if(token_error!=ERROR_NO_TOKEN)return token_error;')
edit(c,'cycle_live',live)
def actor(s):
 return s.replace('HANDLE token=NULL;','HANDLE token=NULL;A1_LEASE lease;').replace('if(!OpenProcessToken(ig_observer.process[role],TOKEN_QUERY,&token))return lab_error();','{DWORD z=lab_aux_token(&lease,ig_observer.process[role],0,TOKEN_QUERY);if(z)return z;z=lab_aux_borrow(lease,&token);if(z)return lab_aux_close_preserving(lease,z);}').replace('if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;return ERROR_INVALID_OWNER;','return lab_aux_close_preserving(lease,ERROR_INVALID_OWNER);').replace('if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;','{DWORD z=lab_aux_close_preserving(lease,0);if(z)return z;}')
edit(i,'ig_actor',actor)
# Keep the scoped borrow active across actual native use, not just copying aliases.
def end_last(s,needle,replacement):
 assert needle in s
 a,b=s.rsplit(needle,1);return a+replacement+b
edit(r,'lab_operator_current',lambda s:end_last(s,'return lab_aux_close_preserving(token_lease,e);','return lab_aux_close_preserving(token_lease,lab_aux_borrow_end(e));'))
for file,fn in [(r,'lab_confirm'),(c,'cycle_confirm'),(i,'ig_confirm')]:
 def scoped_pair(s):
  s=s.replace('HANDLE in=NULL,out=NULL;','int borrowed=0;HANDLE in=NULL,out=NULL;')
  s=s.replace('if((e=lab_aux_borrow(in_lease,&in))||(e=lab_aux_borrow(out_lease,&out)))goto done;','if((e=lab_aux_borrow_pair(in_lease,out_lease,&in,&out)))goto done;borrowed=1;')
  return s.replace('done:e=lab_aux_close_preserving','done:if(borrowed)e=lab_aux_borrow_end(e);e=lab_aux_close_preserving')
 edit(file,fn,scoped_pair)
for file,fn in [(r,'lab_intent'),(i,'ig_extract')]:edit(file,fn,lambda s:end_last(s,'return lab_aux_close_preserving(lease,e);','return lab_aux_close_preserving(lease,lab_aux_borrow_end(e));'))
edit(c,'cycle_intent',lambda s:s.replace('e=lab_error();return lab_aux_close_preserving(lease,e);','e=lab_error();return lab_aux_close_preserving(lease,lab_aux_borrow_end(e));').replace('return lab_aux_close_preserving(lease,0);','return lab_aux_close_preserving(lease,lab_aux_borrow_end(0));'))
edit(i,'ig_actor',lambda s:s.replace('return lab_aux_close_preserving(lease,ERROR_INVALID_OWNER);','return lab_aux_close_preserving(lease,lab_aux_borrow_end(ERROR_INVALID_OWNER));').replace('{DWORD z=lab_aux_close_preserving(lease,0);if(z)return z;}','{DWORD z=lab_aux_borrow_end(0);z=lab_aux_close_preserving(lease,z);if(z)return z;}'))
for file,functions in [(r,['lab_acl','lab_bound_ex']),(c,['cycle_open_relative','cycle_dependencies_current_ex','cycle_files','cycle_copy','cycle_live']),(i,['ig_approval','ig_actor','ig_project_census','ig_create_intent','ig_create_diagnostic','ig_extract','ig_bind_service']),(o,['ig_measure_process','ig_observe','ig_boot']),(n,['ig_n0_sd','ig_n0_open','ig_n0_roots_current','ig_n0_current','ig_n0_bind'])]:
 for fn in functions:guard(file,fn)
for fn in ['psa_bd_aux_current','psa_bd_current','papacc_server_service_admission_bd_bind']:guard(bd,fn,True)
needle='  if(papacc_server_console_win32_stop_requested()&&!e)e=ERROR_OPERATION_ABORTED;';assert new[i].count(needle)==1
new[i]=new[i].replace(needle,'  if(lab_aux_owner.state&A1_FROZEN)goto done; /* existing bounded safe Stop branch remains above */\n'+needle)
needle=' done:\n if(e)ig_diag_note';assert needle in new[i]
new[i]=new[i].replace(needle,' done:\n if(lab_aux_owner.state&A1_FROZEN){ig_gate.retained=1;if(!e)e=lab_aux_owner.first_error?lab_aux_owner.first_error:ERROR_INVALID_STATE;}\n if(e)ig_diag_note').replace('if(ig_diag.created&&!diagnostic_attempted)','if(!(lab_aux_owner.state&A1_FROZEN)&&ig_diag.created&&!diagnostic_attempted)')
# Preserve safe Stop only when the existing canonical owner accepts it.
new[i]=new[i].replace('   (void)papacc_server_service_admission_stop();','   if((z=papacc_server_service_admission_stop())!=0){if(!e)e=z;goto done;}')
assert all('lab_close_aux(' not in s for s in new.values())
patch=[]
for name in names:patch.extend(difflib.unified_diff(old[name].splitlines(True),new[name].splitlines(True),fromfile='a/'+name,tofile='b/'+name))
extras={'tools/qualification/scm-aux-reservation-core.inc':(PKG/'a1-reservation-model.h').read_text(),'tools/qualification/scm-aux-reservation-bridge.inc':(PKG/'candidate-bridge.NOT-APPLIED.inc').read_text()}
for name,text in extras.items():patch.extend(difflib.unified_diff([],text.splitlines(True),fromfile='/dev/null',tofile='b/'+name))
(PKG/'A1-functional-candidate.NOT-APPLIED.patch').write_text(''.join(patch))
(PKG/'candidate-metadata.json').write_text(json.dumps({'status':'REVIEW_ONLY_NOT_APPLIED_NOT_NATIVE_COMPILED','modified_if_applied':names,'added_if_applied':list(extras),'production_default_hook':'undefined; canonical guards unchanged outside LAB','old_close_by_value_calls_remaining_in_candidate':0,'existing_busy_fixture':'must convert old lab_aux_count reset/unsafe characterization to reservation models in authorized implementation'},indent=2)+'\n')
print('Candidate patch generated ONLY in package. Operational integration compilation NOT_RUN.')
