from pathlib import Path
import subprocess,json,os,re
r=Path.cwd();p=Path(__file__).resolve().parent;br=Path('C:/Projetos/PapinhoBrowser');rows=[]
for cfg in ['Debug','Release']:
 out=r/'build/d0-d9-bounded-diagnostic-20261026/Browser'/cfg;out.mkdir(parents=True,exist_ok=True)
 cache=br/('build/sme-20261021/CANDIDATE_065/'+cfg+'/bearssl/bearssls.lib');assert cache.exists()
 command='call C:\\MSVC600-master\\VC98\\Bin\\VCVARS32.BAT && nmake /nologo /f Makefile.nt4.mak PST_PROFILE=CANDIDATE_065 CONFIG='+cfg+' OUT_DIR='+str(out)+' BEARSSL_LIB='+str(cache)+' all'
 env=os.environ.copy();env['CL']='/WX /Fd"'+str(out/'browser-compiler.pdb')+'"'
 with (p/('Browser-'+cfg+'.build.log')).open('wb') as log:q=subprocess.run(['cmd','/d','/c',command],cwd=br,env=env,stdout=log,stderr=subprocess.STDOUT)
 text=(p/('Browser-'+cfg+'.build.log')).read_text(errors='replace');warnings=len(re.findall(r'warning (?:C|LNK)[0-9]+',text));row=dict(config=cfg,exit=q.returncode,warnings=warnings,command=command);rows.append(row);(p/'BROWSER-BUILDS.json').write_text(json.dumps(rows,indent=2)+'\n');print(row,flush=True)
 if q.returncode or warnings:print(text[-5000:]);raise SystemExit(1)
