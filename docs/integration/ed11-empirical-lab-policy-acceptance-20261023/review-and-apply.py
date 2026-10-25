from pathlib import Path
import json,hashlib,subprocess,datetime
r=Path(r"C:\Projetos\PapinhoAccelerator");d=r/'docs/integration/ed11-empirical-lab-policy-acceptance-20261023'
manifest=d/'exact-write-manifest.json';assert hashlib.sha256(manifest.read_bytes()).hexdigest()=='d8e193caee53e7f6e59642958362e79c5e0999b7f83875d9807acfde9f7e8e6a'
a=json.loads(manifest.read_text(encoding='utf-8'));base=json.loads((d/'preimages.json').read_text(encoding='utf-8'))
assert len(a)==6 and len(base)==8424
assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=r,text=True).strip()=='913c90cf2328f7939d8f5b927640601903ac6e91'
assert subprocess.check_output(['git','branch','--show-current'],cwd=r,text=True).strip()=='main'
for name,expected in base.items():
 p=r/name;assert p.is_file() and hashlib.sha256(p.read_bytes()).hexdigest()==expected,name
for x in a:
 p=(r/x['path']).resolve();assert p.is_relative_to(r) and p.suffix in ['.md','.json'];stage=Path(x['stage']).resolve();assert stage.is_relative_to(d/'stage-files');assert hashlib.sha256(stage.read_bytes()).hexdigest()==x['post'];assert (hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None)==x['pre']
source=(r/'docs/adr/ADR-CANDIDATE-ed11-single-lab-diagnostic-native-boundary-20261023.md').read_bytes();assert hashlib.sha256(source).hexdigest()=='f2376662b0d5246328ff3393de43d0b7eb2fb14cd73e4f2c6570bfc088214c34'
body=source[source.index(b'B0. '):source.index(b'## Current decision state')];assert hashlib.sha256(body).hexdigest()=='4fbd74df7711950de53a3b71adf1cfcf84bee219e17e631c7f7e6bdab0a7fe61'
accepted=Path(a[0]['stage']).read_bytes();assert accepted[accepted.index(b'B0. '):accepted.index(b'## Current status and next gate')]==body
assert b'status: accepted' in accepted and b'NEW_START_AUTHORIZED=NO' in accepted
# Exact reviewed documentation writes only; no SCM/ACL/native operation.
for x in a:
 p=r/x['path'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(Path(x['stage']).read_bytes())
for x in a:assert hashlib.sha256((r/x['path']).read_bytes()).hexdigest()==x['post'],x['path']
assert hashlib.sha256((r/'docs/adr/ADR-CANDIDATE-ed11-single-lab-diagnostic-native-boundary-20261023.md').read_bytes()).hexdigest()=='f2376662b0d5246328ff3393de43d0b7eb2fb14cd73e4f2c6570bfc088214c34'
receipt={'scope':'OWNER_APPROVED_NORMATIVE_POLICY_ONLY_EXACT_OFFICIAL_DOCUMENT_WRITE','utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'manifest_sha256':hashlib.sha256(manifest.read_bytes()).hexdigest(),'accepted_decision_sha256':a[0]['post'],'approved_body_sha256':hashlib.sha256(body).hexdigest(),'document_writes':len(a),'owner_decision':'EXPLICIT_OPTION1_PLUS_OWNER_DECISION_ED11_B0_B6_REVISION4','start_executed':False,'scm_mutations':'NONE','acl_mutations':'NONE','ed11_total':'PARTIAL','new_start_authorized':False}
(d/'official-write-receipt.json').write_text(json.dumps(receipt,indent=2),encoding='utf-8');print(json.dumps(receipt))
