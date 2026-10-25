from pathlib import Path
import struct,json,re,hashlib
r=Path(r'C:\Projetos\PapinhoAccelerator');t=Path(__file__).parent
h=lambda b:hashlib.sha256(b).hexdigest()
def pe(p):
 b=p.read_bytes();off=struct.unpack_from('<I',b,60)[0];assert b[off:off+4]==b'PE\0\0'
 n=struct.unpack_from('<H',b,off+6)[0];size=struct.unpack_from('<H',b,off+20)[0];opt=off+24;assert struct.unpack_from('<H',b,opt)[0]==0x20b
 sec={}
 for i in range(n):
  x=opt+size+40*i;name=b[x:x+8].split(b'\0')[0].decode();vs,va,raw=struct.unpack_from('<III',b,x+8);sec[name]=dict(virtual_size=vs,raw_size=raw,virtual_address=va)
 return dict(sha256=h(b),image_size=struct.unpack_from('<I',b,opt+56)[0],stack_reserve=struct.unpack_from('<Q',b,opt+72)[0],stack_commit=struct.unpack_from('<Q',b,opt+80)[0],heap_reserve=struct.unpack_from('<Q',b,opt+88)[0],heap_commit=struct.unpack_from('<Q',b,opt+96)[0],sections=sec)
result={}
for p in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  tag=p+'-'+c;before=pe(r/f'build/personal-ca-custody-binding-20261022/{p}/{c}/papacc_server.exe');after=pe(r/f'build/windows-scm-lifecycle-20261022/{p}/{c}/papacc_server.exe')
  delta={name:after['sections'].get(name,{}).get('virtual_size',0)-before['sections'].get(name,{}).get('virtual_size',0) for name in sorted(set(before['sections'])|set(after['sections']))}
  asm=(t/f'measure-{tag}.asm').read_text(errors='replace');frames={}
  for m in re.finditer(r'^([A-Za-z_][A-Za-z_0-9]*) PROC.*?\n(.*?)^\1 ENDP',asm,re.M|re.S):
   vals=re.findall(r'\bsub\s+rsp,\s*(\d+)',m[2]);frames[m[1]]=max(map(int,vals),default=0)
  result[tag]=dict(before=before,after=after,PE_section_virtual_size_delta=delta,diagnostic_own_stack_adjustments=frames,stack_scope='Separate same-source diagnostic MSVC own sub-rsp only; excludes pushes, return addresses, callees and opaque Windows/CRT; not complete bound')
(t/'ed11-measurements.json').write_text(json.dumps(result,indent=2))
for tag,x in result.items():print(tag,'PEdelta',x['PE_section_virtual_size_delta'],'frames',{k:v for k,v in x['diagnostic_own_stack_adjustments'].items() if k.startswith('papacc_server_service') or k in ['ps_main','ps_close','ps_report','ps_handler']})
