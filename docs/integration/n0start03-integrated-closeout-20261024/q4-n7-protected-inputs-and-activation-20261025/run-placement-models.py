from pathlib import Path
import subprocess,hashlib,json
p=Path(__file__).resolve().parent;script=p/'q4-input-placement.CANDIDATE.ps1';sha=hashlib.sha256(script.read_bytes()).hexdigest();rows=[]
for cfg in ['Debug','Release']:
 log=p/f'placement-model-final-{cfg}.log'
 with log.open('wb') as out:q=subprocess.run(['C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(script),'-Mode','ModelTest','-FixtureConfig',cfg,'-ExpectedScriptSha256',sha],stdout=out,stderr=subprocess.STDOUT,timeout=40)
 assert q.returncode==0,log.read_text(errors='replace');s=log.read_text(errors='replace');assert all(x in s for x in ['checks=50','cases=8','cases=6','cases=9']);rows.append(dict(config=cfg,exit=0,compiler_warnings_as_errors=True,model_checks=73,script_sha256=sha,native_sha256=hashlib.sha256((p/'q4-input-placement-native.CANDIDATE.cs').read_bytes()).hexdigest(),log=log.name));p.joinpath('placement-model-final-results.json').write_text(json.dumps(rows,indent=2));print('CS_MODEL_PASS',cfg,73,flush=True)
