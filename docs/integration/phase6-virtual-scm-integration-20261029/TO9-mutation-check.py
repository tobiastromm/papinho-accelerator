from pathlib import Path
import subprocess,json
r=Path.cwd();d=r/'docs/integration/phase6-virtual-scm-integration-20261029'
mutants=[
 ('noncanonical_key_accepted','apps/server/personal_scq_profile_win32.inc','scqp_nonzero((const BYTE *)p->key,sizeof(p->key))','0','papacc_scq_private_profile_test'),
 ('transport_reaches_ksp_dispatch','apps/server/personal_scq_dispatch_win32.inc','if(d->purpose!=PAPACC_SCQ_PURPOSE_READ){d->result=PAPACC_CNG_PROBE_LOCAL_DENY;return d->result;}','/* guard removed by isolated qualification mutant */','papacc_personal_scq_dispatch_test'),
 ('administrative_purpose_not_revalidated','apps/server/personal_scq_admin_win32.inc','c->value.purpose!=p.purpose||c->value.deadline!=','c->value.deadline!=','papacc_scq_pre_readiness_chain_test')]
results=[]
for name,file,old,new,target in mutants:
 p=r/file;original=p.read_bytes();s=original.decode();assert s.count(old)==1
 cmd=d/('TO9-mutant-'+name+'.cmd')
 cmd.write_text('@echo off\nset VSLANG=1033\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\nif errorlevel 1 exit /b %errorlevel%\ncmake --build build/phase6-functional-20261028/Selected/Debug --parallel 2 --target '+target+'\nexit /b %errorlevel%\n')
 try:
  p.write_bytes(s.replace(old,new).encode())
  b=subprocess.run(['cmd','/c',str(cmd)],capture_output=True,text=True)
  (d/('TO9-mutant-'+name+'-build.log')).write_text(b.stdout+b.stderr)
  assert b.returncode==0,(name,b.stdout[-2000:])
  test=subprocess.run([str(r/'build/phase6-functional-20261028/Selected/Debug'/(target+'.exe'))],capture_output=True,text=True)
  (d/('TO9-mutant-'+name+'-test.log')).write_text(test.stdout+test.stderr)
  results.append({'mutant':name,'compile':'PASS','exit':test.returncode,'rejected':test.returncode!=0,'scope':'OFFLINE_MODEL_ONLY'})
 finally:p.write_bytes(original)
(d/'TO9-MUTANTS.json').write_text(json.dumps(results,indent=2)+'\n')
assert all(x['rejected'] for x in results),results
print(json.dumps(results,indent=2))
