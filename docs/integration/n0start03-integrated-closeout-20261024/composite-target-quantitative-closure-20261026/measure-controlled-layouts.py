from pathlib import Path
import json,re,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/composite-target-quantitative-closure-20261026';out=r/'build/composite-target-quantitative-20261026/layout';out.mkdir(parents=True,exist_ok=True)
a=['sizeof(PAPACC_C2_FLOW)','sizeof(PAPACC_C2_OPERATION)','sizeof(PAPACC_C2_OPERATION_MANAGER)','sizeof(PAPACC_C3_QUEUE)','sizeof(PAPACC_C3_BRIDGE)','sizeof(PAPACC_C8_PRIVATE_STORAGE_LAYOUT)','PAPACC_C8_KNOWN_MANAGED_BYTES','PAPACC_C8_STORAGE_LIMIT','4U*PAPACC_C2_PAYLOAD_CAPACITY+2U*PAPACC_C3_QUEUE_CAPACITY']
b=['sizeof(papinho_c2_s6_context)','sizeof(C2_ROLE)','sizeof(papinho_session_storage)','sizeof(papinho_accel_pst_group)','sizeof(papinho_accelerator_persistent_request_job)','sizeof(papinho_pst_owner)','sizeof(papinho_c2_s6_context)+sizeof(papinho_session_storage)+sizeof(papinho_accel_pst_group)+4U*sizeof(papinho_accelerator_persistent_request_job)+3U*sizeof(papinho_pst_owner)+2U*C2_HTTP_CAP+65536U']
(p/'accelerator-layout.c').write_text('#include "server_c8_private_storage_layout_win32.h"\nconst unsigned long quant_layout[] = {\n'+',\n'.join(a)+'\n};\n')
(p/'browser-layout.c').write_text('#include "C:/Projetos/PapinhoBrowser/src/papinho_accelerator_client.c"\nconst unsigned long quant_layout[] = {\n'+',\n'.join(b)+'\n};\n')
rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  c=json.loads((r/f'build/q4-target-evidence-20261025/{profile}/{cfg}/compile_commands.json').read_text());cmd=next(x['command'] for x in c if x['file'].endswith('target_typed_evidence_test.c'));cmd=cmd[:cmd.index(' /Fo')];tag='Accelerator-'+profile+'-'+cfg;asm=out/(tag+'.asm');cmd+=' /FA /Fa"'+str(asm)+'" /Fo"'+str(out/(tag+'.obj'))+'" /Fd"'+str(out/(tag+'.pdb'))+'" /c "'+str(p/'accelerator-layout.c')+'"';vc='C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat';rows.append((tag,vc,cmd,r,a,asm))
br=Path('C:/Projetos/PapinhoBrowser')
for cfg in ['Debug','Release']:
 tag='Browser-VC6-'+cfg;asm=out/(tag+'.asm');cmd='cl /nologo /TC /W3 /WX /MD /DPAPINHO_PST_TARGET_CANDIDATE_065 /DWIN32 /D_WINDOWS /D_CONSOLE /D_CRT_SECURE_NO_WARNINGS '+('/Od /Z7 /DPAPINHO_C2_NATIVE_BUILD_DEBUG=1 ' if cfg=='Debug' else '/O2 /DPAPINHO_C2_NATIVE_BUILD_DEBUG=0 ')
 for inc in ['.','core','platform','third_party/compat/vc6','third_party/bearssl/inc','third_party/papinhopacc/include','third_party/papinhopacc/include/papacc/pacc','third_party/pst-sdk-0.6.5-normal-4dbd2783/include']:cmd+='/I"'+str(br/inc)+'" '
 cmd+='/FI"'+str(br/'third_party/compat/vc6/vc6_compat.h')+'" /FI"'+str(br/'build-config/pst-target-contract.h')+'" /FA /Fa"'+str(asm)+'" /Fo"'+str(out/(tag+'.obj'))+'" /Fd"'+str(out/(tag+'.pdb'))+'" /c "'+str(p/'browser-layout.c')+'"';rows.append((tag,'C:/MSVC600-master/VC98/Bin/VCVARS32.BAT',cmd,br,b,asm))
result=[]
for tag,vc,cmd,cwd,expr,asm in rows:
 batch=p/(tag+'.cmd');batch.write_text('@echo off\ncall "'+vc+'"\nif errorlevel 1 exit /b %errorlevel%\n'+cmd+'\nexit /b %errorlevel%\n');q=subprocess.run(['cmd','/c',str(batch)],cwd=cwd,capture_output=True);log=q.stdout+q.stderr;(p/(tag+'.log')).write_bytes(log)
 if q.returncode or re.search(rb'warning [A-Z]+[0-9]+',log):print(log.decode(errors='replace'));raise SystemExit(1)
 text=asm.read_text(errors='replace');start=re.search(r'^_?quant_layout\s+DD',text,re.M);assert start,tag
 block=text[start.start():].split('END',1)[0];nums=re.findall(r'\bDD\s+([0-9a-fA-F]+H?|[0-9]+)',block);values=[int(x[:-1],16) if x.endswith('H') else int(x) for x in nums[:len(expr)]];assert len(values)==len(expr),(tag,nums)
 result.append(dict(target=tag,exit=q.returncode,warnings=0,measurements=dict(zip(expr,values)),scope='compiler constants only; TU never linked or executed'));print(result[-1],flush=True)
(p/'COMPILED-CONTROLLED-LAYOUTS.json').write_text(json.dumps(result,indent=2)+'\n')
