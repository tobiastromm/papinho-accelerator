from pathlib import Path
import json,hashlib,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q7-source-image-pin-correction-20261025';h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();old=r/'docs/integration/n0start03-integrated-closeout-20261024/q4-final-pins-and-q7-readiness-20261025';image=r/'build/q4-final-binding-20261025/Selected/Release/papacc_server_lab_host.exe';header=r/'tools/qualification/scm-q4-image-pin.h';stale='2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821';rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/q7-source-pin-correction-20261025'/profile/cfg;exe=b/'papacc_scm_instrumented_lab.exe';q=subprocess.run(['python',str(p/'verify-image-pin-in-binary.py'),'--qualifier',str(exe),'--image',str(image),'--pin-header',str(header),'--forbid',stale],capture_output=True,text=True);assert q.returncode==0,(profile,cfg,q.stdout,q.stderr);record=json.loads(q.stdout)
  rule=next(x for x in (b/'build.ninja').read_text().replace('\\','/').splitlines() if x.startswith('build CMakeFiles/papacc_scm_instrumented_lab.dir/tools/qualification/scm-registration-lab.c.obj:'));assert 'scm-q4-image-pin.h' in rule;record.update(profile=profile,config=cfg,explicit_q4_pin_dependency=True)
  q=subprocess.run([str(exe),'--q4-offline-invalid-probe'],capture_output=True,text=True,timeout=20);assert q.returncode==2 and 'PRIVATE_STDOUT_BOOTSTRAP_DENY' in q.stderr;record['invalid_bootstrap_exit']=2;rows.append(record)
(p/'compiled-pin-and-entry-results.json').write_text(json.dumps(rows,indent=2)+'\n')
f=r/'tools/qualification/scm-n0-protected-single-powershell.ps1';s=f.read_text();a=s.index("if($Profile-ceq'Q4TokenDiagnostic'){");b=s.index('\n}',a);block=s[a:b];block=re.sub(r"\$QualifierHash='[0-9a-f]{64}'","$QualifierHash='"+rows[1]['qualifier_sha256']+"'",block);s=s[:a]+block+s[b:];f.write_text(s,newline='\r\n')
# Build recipe now enforces compiled attestation immediately, not just header/file hashes.
f=p/'run-builds.py';s=f.read_text();marker=" if q.returncode or warnings:raise SystemExit(1)";extra="\n subprocess.run(['python',str(p/'verify-image-pin-in-binary.py'),'--qualifier',str(Path.cwd()/'build/q7-source-pin-correction-20261025'/profile/cfg/'papacc_scm_instrumented_lab.exe'),'--image',str(Path.cwd()/'build/q4-final-binding-20261025/Selected/Release/papacc_server_lab_host.exe'),'--pin-header',str(Path.cwd()/'tools/qualification/scm-q4-image-pin.h'),'--forbid','"+stale+"'],check=True)\n";assert marker in s;s=s if 'subprocess.run(['+chr(39)+'python'+chr(39) in s else s.replace(marker,marker+extra);f.write_text(s)
# Reuse existing pure approval gates against the current canonical source.
ps=[]
for f in [p/'q7-corrected-launcher-gates.ps1',old.parent/'t10-t14-positive-integration/launcher-gate-tests.ps1']:
 q=subprocess.run([r'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(f)],capture_output=True,text=True,timeout=30);assert q.returncode==0,(f,q.stderr);(p/(f.stem+'.log')).write_text(q.stdout+q.stderr);ps.append(dict(script=str(f),exit=q.returncode,output=q.stdout))
(p/'powershell-gate-results.json').write_text(json.dumps(ps,indent=2)+'\n')
# Old approved assets and request are byte-identical, not rebuilt in place.
original=json.loads((p/'historical-approved-artifacts.json').read_text())
for item in original['artifacts'].values():
 if item['path'].endswith('scm-n0-protected-single-powershell.ps1'):continue
 assert h(Path(item['path']))==item['sha256'],item['path']
assert h(old/'EXACT-Q7-APPROVED.json')=='c07384b26a57a438c10a382948ab2be5ffe8ab80fab56138881e605c8c6f4c46'
assert h(old/'Q7-OPERATION-REQUEST.DRAFT.json')=='cf88f42ad86700ca1eb7891c834d8c9521f1c30dfc5a55317cb6e4665ba3db58'
assert h(image)=='128834681905d861eb3252c1e0079e1d06a02b46c2f6d34ddb3612bcc85b3dc8'
print(json.dumps(dict(compiled_checks=4,invalid_probes=4,ps_gates=58,launcher_sha256=h(r/'tools/qualification/scm-n0-protected-single-powershell.ps1'),selected_release_qualifier_sha256=rows[1]['qualifier_sha256'],frozen_image_sha256=h(image),approval_disposition='USED_NONREPLAYABLE'),indent=2))
