from pathlib import Path
import json,re,subprocess,hashlib
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025';rows=[]
for cfg in ['Debug','Release']:
 b=r/'build/q4-target-evidence-20261025/Selected'/cfg;cmds=json.loads((b/'compile_commands.json').read_text())
 for source,names in [('server_c8_target_evidence_win32.c',['papacc_c8_target_view','papacc_c8_target_view_validate']),('server_launcher_owner_win32.c',['papacc_server_launcher_target_evidence_view'])]:
  cmd=next(x['command'] for x in cmds if x['file'].endswith(source));assert 'PAPACC_SINGLE_LAB_ADMISSION' not in cmd
  stem=p/(cfg+'-'+source);cmd=re.sub(r'/Fo\S+','/Fo'+stem.as_posix()+'.obj',cmd);cmd=re.sub(r'/Fd\S+','/Fd'+stem.as_posix()+'.pdb',cmd);cmd+=' /FAs /Fa'+stem.as_posix()+'.asm'
  f=p/(cfg+'-'+source+'.cmd');f.write_text('@echo off\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\n'+cmd+'\n',encoding='utf-8');q=subprocess.run(['cmd','/c',str(f)],cwd=b,capture_output=True);(p/(cfg+'-'+source+'.log')).write_bytes(q.stdout+q.stderr);assert q.returncode==0
  asm=Path(str(stem)+'.asm').read_text(errors='replace')
  for name in names:
   m=re.search(r'^'+name+r' PROC.*?^'+name+r' ENDP',asm,re.M|re.S);block=m.group(0) if m else 'NOT_SEPARATELY_EMITTED';(p/(cfg+'-'+name+'.frame.txt')).write_text(block,encoding='utf-8');rows.append(dict(source=source,source_sha256=hashlib.sha256((r/'apps/server'/source).read_bytes()).hexdigest(),config=cfg,function=name,sub_rsp=re.findall(r'sub\s+rsp,\s*([^;\n]+)',block),scope='own emitted frame only; pushes/callees/OS stack/CRT not included'))
(p/'ACCELERATOR-COMPILED-FRAMES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8');print(rows)
