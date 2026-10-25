import pathlib,json,hashlib,shutil,subprocess,datetime
s=pathlib.Path(r'C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021');h=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert h(s/'lab-operation.json')=='93130c8f4f60d04b274cd18833e081aa9de3f59ca601c78233837d54690e2943'
op=json.loads((s/'lab-operation.json').read_text());exe=s/'lab-generator/lab_generate.exe';source=s/'roughtime_lab_generate.c'
assert h(exe)==op['generator_sha256'] and h(source)==op['source_sha256']
dll=pathlib.Path(r'C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\runtime\libcrypto-3-x64.dll');assert h(dll)==op['pinned_DLL_sha256']
shutil.copyfile(dll,exe.parent/dll.name);assert h(exe.parent/dll.name)==op['pinned_DLL_sha256']
out=pathlib.Path(op['output_directory']).resolve();assert out==s/'lab-output';assert not out.exists();out.mkdir()
result=subprocess.run([str(exe),str(out)],capture_output=True,text=True);(s/'lab-generation.log').write_bytes((result.stdout+result.stderr).encode());print(result.stdout);assert result.returncode==0
expected={f'request-{i}.bin' for i in range(6)}|{f'response-{i}.bin' for i in range(6)}|{'long-term-keys.bin','nonce-randoms.bin'};assert {p.name for p in out.iterdir()}==expected
files=[]
for p in sorted(out.iterdir()):
 expected_size=2048 if p.name.startswith('request-') else 480 if p.name.startswith('response-') else 96 if p.name=='long-term-keys.bin' else 192
 assert p.stat().st_size==expected_size
 files.append({'file':p.name,'bytes':p.stat().st_size,'sha256':h(p)})
record={'scope':'LAB_ONLY','generated_at_utc_record_not_qualified_time':datetime.datetime.now(datetime.timezone.utc).isoformat(),'operation_sha256':'93130c8f4f60d04b274cd18833e081aa9de3f59ca601c78233837d54690e2943','files':files,'private_material_exported':False,'network':False,'UTC_SOURCE':'NOT_QUALIFIED'}
(s/'lab-transcript-manifest.json').write_bytes((json.dumps(record,indent=2)+'\n').encode());print('Public LAB transcript archived; no private material exported')
