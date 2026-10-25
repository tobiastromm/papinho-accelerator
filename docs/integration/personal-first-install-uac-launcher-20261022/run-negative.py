from pathlib import Path
import subprocess,json,sys,os
r=Path(r'C:\Projetos\PapinhoAccelerator');e=Path(__file__).parent
ps=r'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
s=r/'tools/qualification/personal-first-install-uac.ps1'
# subprocess pipes guarantee no console. No RunAs or positive CLI invocation.
cases=[('check',[],2,'CHECK_ONLY_NO_EXECUTION'),('launch-without-console',['-Mode','Launch'],2,'NO_INTERACTIVE_CONSOLE_NO_UAC'),('elevated-missing-binding',['-Mode','Elevated'],2,'MISSING_ELEVATED_BINDING'),('internal-argument-rejected',['-OriginSid','S-1-5-1'],2,'UNEXPECTED_INTERNAL_ARGUMENTS'),('unknown-mode',['-Mode','arbitrary'],1,'ParameterArgumentValidationError'),('sid-injection-rejected',['-OriginSid','S-1-5-1;whoami'],1,'ParameterArgumentValidationError'),('script-hash-divergent',['-Mode','Elevated','-OriginSid','S-1-5-1','-Attempt','1'*32,'-ExpectedScriptSha256','0'*64],2,'SCRIPT_CHANGED_ACROSS_UAC')]
results=[]
for name,args,expected,marker in cases:
 p=subprocess.run([ps,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(s),*args],input='',capture_output=True,text=True,timeout=40)
 assert p.returncode==expected and marker in p.stdout+p.stderr,(name,p.returncode,p.stdout,p.stderr)
 if name=='check':
  q=json.loads(p.stdout);assert not q['interactive_console'] and q['target_child_absent']
 results.append(dict(case=name,exit=p.returncode,stdout=p.stdout,stderr=p.stderr))
# Location negative uses a byte-exact temporary copy, never UAC/CLI.
pth=Path(os.environ['TEMP'])/'pacc-uac-launcher-20261022'/'untrusted-copy.ps1';pth.parent.mkdir(parents=True,exist_ok=True);pth.write_bytes(s.read_bytes())
p=subprocess.run([ps,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(pth)],input='',capture_output=True,text=True,timeout=40)
assert p.returncode==2 and 'UNTRUSTED_SCRIPT_LOCATION' in p.stdout
results.append(dict(case='wrong-script-location',exit=p.returncode,stdout=p.stdout,stderr=p.stderr))
(e/'negative-results.json').write_text(json.dumps(results,indent=2))
p=subprocess.run([ps,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(e/'negative-helpers.ps1')],capture_output=True,text=True,timeout=40)
(e/'helper-results.txt').write_text(p.stdout+p.stderr);assert p.returncode==0,(p.stdout,p.stderr)
print('NEGATIVE_NATIVE_CASES_PASS',len(results));print(p.stdout.strip())
