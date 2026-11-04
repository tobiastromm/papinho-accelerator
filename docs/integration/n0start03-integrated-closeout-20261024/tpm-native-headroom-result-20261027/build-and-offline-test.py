from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent;csc=Path('C:/Windows/Microsoft.NET/Framework64/v4.0.30319/csc.exe');results=[]
for config in ['Debug','Release']:
 for name,src,main in [('GuardFixture',['GuardFixture.cs'],'GuardFixture'),('NativeProofCandidate',['NativeProofHost.DIAGNOSTIC-CANDIDATE.cs','SaltedProofFixture.cs','PublicAreaGateFixture.cs','SaltedSessionPlanFixture.cs','CredentialActivationFixture.cs','NativeCfb.REUSED.cs','NativeCfbDecrypt.FIXTURE.cs'],'NativeProofHost')]:
  exe=p/(name+config+'.exe');cmd=[str(csc),'/nologo','/warn:4','/warnaserror+','/platform:x64','/target:exe','/main:'+main,'/out:'+str(exe),'/debug+' if config=='Debug' else '/debug-','/optimize-' if config=='Debug' else '/optimize+','/r:System.Web.Extensions.dll','/r:System.Security.dll']+[str(p/x) for x in src]
  r=subprocess.run(cmd,capture_output=True,text=True);(p/(name+config+'-build.log')).write_text(r.stdout+r.stderr,encoding='utf-8')
  if r.returncode:print(r.stdout+r.stderr);raise SystemExit(r.returncode)
  args=[] if name=='GuardFixture' else ['--selfcheck']
  t=subprocess.run([str(exe)]+args,capture_output=True,text=True,timeout=30);(p/(name+config+'-test.log')).write_text(t.stdout+t.stderr,encoding='utf-8');print(name,config,t.returncode,t.stdout.strip());assert t.returncode==0
  results.append(dict(target=name,configuration=config,build=0,warnings=0,test_exit=0,result=t.stdout.strip(),native_mode='NOT_EXECUTED'))
(p/'BUILD-TEST-RESULTS.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
