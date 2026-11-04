from pathlib import Path
import subprocess,os,json,re,concurrent.futures
R=Path('C:/Projetos/PapinhoBrowser');P=Path('C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser')
def build(item):
 profile,cfg=item
 out=R/'build/q4-target-evidence-20261025'/profile/cfg;out.mkdir(parents=True,exist_ok=False)
 cache='build/sme-20261021/'+profile+'/'+cfg+('/bearssl/bearssls.lib' if profile=='CANDIDATE_065' else '/bearssl-md/bearssls.lib')
 if not (R/cache).exists(): cache='build/sme-20261021/CANDIDATE_065/'+cfg+'/bearssl/bearssls.lib'
 assert (R/cache).exists()
 command='call C:\\MSVC600-master\\VC98\\Bin\\VCVARS32.BAT && nmake /nologo /f Makefile.nt4.mak PST_PROFILE='+profile+' CONFIG='+cfg+' OUT_DIR='+str(out.relative_to(R)).replace('/','\\')+' BEARSSL_LIB='+cache.replace('/','\\')+' all'
 env=os.environ.copy();env['CL']='/WX'
 p=subprocess.run(['cmd.exe','/d','/c',command],cwd=R,env=env,capture_output=True)
 (P/(profile+'-'+cfg+'.build.log')).write_bytes(p.stdout+p.stderr)
 t=(p.stdout+p.stderr).decode('cp1252',errors='replace')
 result=dict(profile=profile,configuration=cfg,exit=p.returncode,warnings=re.findall(r'.*warning [A-Z]+[0-9]+.*',t,re.I),errors=re.findall(r'.*(?:fatal error|error [A-Z]+[0-9]+).*',t,re.I),command=command)
 print(json.dumps(result),flush=True);return result
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool: result=list(pool.map(build,[(p,c) for p in ['CANDIDATE_065','PRODUCTION_064'] for c in ['Debug','Release']]))
(P/'BUILDS.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
