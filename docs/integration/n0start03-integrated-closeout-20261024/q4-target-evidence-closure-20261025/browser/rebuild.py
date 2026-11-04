from pathlib import Path
import subprocess,os,json,re
R=Path('C:/Projetos/PapinhoBrowser');P=Path('C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser')
results=[]
for cfg in ['Debug','Release']:
 rel='build/q4-target-evidence-20261025/CANDIDATE_065/'+cfg
 cmd='call C:\\MSVC600-master\\VC98\\Bin\\VCVARS32.BAT && nmake /nologo /f Makefile.nt4.mak PST_PROFILE=CANDIDATE_065 CONFIG='+cfg+' OUT_DIR='+rel.replace('/','\\')+' BEARSSL_LIB=build\\sme-20261021\\CANDIDATE_065\\'+cfg+'\\bearssl\\bearssls.lib all'
 env=os.environ.copy();env['CL']='/WX';b=subprocess.run(['cmd.exe','/d','/c',cmd],cwd=R,env=env,capture_output=True);(P/('CANDIDATE_065-'+cfg+'.FINAL.build.log')).write_bytes(b.stdout+b.stderr)
 t=(b.stdout+b.stderr).decode('cp1252',errors='replace');result=dict(configuration=cfg,build=b.returncode,warnings=re.findall(r'.*warning [A-Z]+[0-9]+.*',t,re.I));results.append(result);print(json.dumps(result),flush=True)
 assert b.returncode==0 and not result['warnings']
(P/'FINAL-CANDIDATE-BUILDS.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
