from pathlib import Path
import json,re,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/r0-r9-acceptance-and-integration-20261025';b=r/'build/r0-r9-integration-20261025/Selected/Release'
a=json.loads((b/'compile_commands.json').read_text());cmd=next(x['command'] for x in a if x['file'].endswith('server_service_win32.c') and 'papacc_server_lab_host.dir' in x['command'])
cmd=re.sub(r'/Fo\S+','/Fo'+(p/'service-frames.obj').as_posix(),cmd);cmd=re.sub(r'/Fd\S+','/Fd'+(p/'service-frames.pdb').as_posix(),cmd);cmd+=' /FAs /Fa'+(p/'service-frames.asm').as_posix()
f=p/'measure-frames.cmd';f.write_text('@echo off\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\n'+cmd+'\n',encoding='utf-8');q=subprocess.run(['cmd','/c',str(f)],cwd=b,capture_output=True);(p/'measure-frames.log').write_bytes(q.stdout+q.stderr);print(q.returncode)
if q.returncode==0:
 text=(p/'service-frames.asm').read_text(errors='replace');rows=[]
 for name in ['psr_r4_current','psr_token','psr_current']:
  m=re.search(r'^'+name+r' PROC.*?^'+name+r' ENDP',text,re.M|re.S)
  block=m.group(0) if m else 'INLINED_OR_NOT_EMITTED';(p/(name+'-assembly.txt')).write_text(block,encoding='utf-8');rows.append(dict(function=name,sub_rsp=re.findall(r'sub\s+rsp,\s*([^;\n]+)',block),scope='Selected Release own frame only; excludes callees/PSS/kernel/stack reserve'))
 (p/'COMPILED-FRAMES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8');print(rows)
