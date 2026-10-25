from pathlib import Path
import subprocess,json,hashlib,shutil
p=Path(__file__).resolve().parent
script=p/'q2-protected-readback.ps1'
digest=hashlib.sha256(script.read_bytes()).hexdigest()
ps='C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe'
rows=[]
def run(name,mode,expected=None,path=script,pin=digest):
 log=p/(name+'.log')
 with log.open('wb') as out:
  q=subprocess.Popen([ps,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(path),'-Mode',mode,'-ExpectedScriptSha256',pin],stdout=out,stderr=subprocess.STDOUT)
  code=q.wait(timeout=45)
 data=log.read_text(errors='replace')
 passed=(code==0) if expected is None else (code!=0 and expected in data)
 rows.append(dict(test=name,exit=code,pass_expected=passed,expected_deny=expected,log=log.name))
 (p/'qualification-results.json').write_text(json.dumps(rows,indent=2))
 print(name,code,passed,flush=True)
 if not passed:print(data);raise SystemExit(1)
for mode in ['CompileOnly','Model','NativeFixture']:run(mode,mode)
run('wrong-script-pin','CompileOnly','SCRIPT_PIN_DENY',pin='0'*64)
run('missing-confirmation','Verify','EXACT_READONLY_CONFIRMATION_DENY')
tamper=p/'tampered-source-fixture'
tamper.mkdir(exist_ok=True)
shutil.copyfile(script,tamper/script.name)
(tamper/'q2-protected-readback.cs').write_bytes((p/'q2-protected-readback.cs').read_bytes()+b'\n// deliberate isolated tamper\n')
run('wrong-source-pin','CompileOnly','SOURCE_PIN_DENY',path=tamper/script.name)
