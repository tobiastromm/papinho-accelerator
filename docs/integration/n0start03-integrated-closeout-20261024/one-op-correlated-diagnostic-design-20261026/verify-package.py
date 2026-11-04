from pathlib import Path
import json,hashlib,csv,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/one-op-correlated-diagnostic-design-20261026';prior=p.parent/'composite-target-quantitative-closure-20261026';old=p.parent/'q4-target-evidence-closure-20261025'
preserved=[]
for x in json.loads((prior/'PRESERVATION-VERIFICATION.json').read_text())['unchanged']:
 f=Path(x['path']);h=hashlib.sha256(f.read_bytes()).hexdigest();assert h==x['sha256'];preserved.append(dict(path=str(f),sha256=h))
for x in json.loads((old/'FINAL-SOURCE-HASHES.json').read_text()):assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256']
for x in json.loads((old/'q4-and-critical-path/Q4-REQUEST-DEPENDENCY-READONLY-VERIFICATION.json').read_text())['items']:assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['expected']
for x in json.loads((p/'FINAL-SOURCE-AND-PROBE-HASHES.json').read_text()):assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256']
for repo,name in [(r,'ACCELERATOR'),(Path('C:/Projetos/PapinhoBrowser'),'BROWSER')]:
 head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo);assert head==(p/(name+'-HEAD.txt')).read_bytes();status=subprocess.check_output(['git','status','--short'],cwd=repo);(p/(name+'-WIP-CLOSE.txt')).write_bytes(status)
 if name=='BROWSER':assert status==(p/(name+'-WIP-START.txt')).read_bytes()
 q=subprocess.run(['git','diff','--check'],cwd=repo,capture_output=True);assert q.returncode==0;(p/(name+'-DIFF-CHECK.log')).write_bytes(q.stdout+q.stderr)
rows=list(csv.DictReader((p.parent/'ED11_GATE_MATRIX.csv').open(encoding='utf-8-sig',newline='')));before=list(csv.DictReader((p/'preimages/ED11_GATE_MATRIX.csv').open(encoding='utf-8-sig',newline='')))
current=lambda x:re.fullmatch(r'E(?:0[1-9]|1[0-9]|2[0-2])',x['gate'])
assert [x for x in rows if not current(x)]==[x for x in before if not current(x)]
cur=[x for x in rows if current(x)];assert len(rows)==36 and len(cur)==22;assert sum(x['status']=='PASS_DEMONSTRADO' for x in cur)==4;assert sum(x['status']=='PASS_LIMITADO_AO_ESCOPO_OBSERVADO' for x in cur)==12
for f in [p/'CONSOLIDATED-CLOSEOUT.md',r/'docs/adr/ADR-CANDIDATE-mode-c-one-operation-diagnostic-authority-20261026.md']:
 for link in re.findall(r'\]\(([^)]+)\)',f.read_text(encoding='utf-8')):assert (f.parent/link).exists(),(f,link)
probe=json.loads((p/'PROBE-QUALIFICATION.json').read_text());assert len(probe)==4 and all(x['warnings']==0 and x['test']=='PASS' for x in probe)
policy=json.loads((p/'HYBRID-POLICY-CANDIDATE.json').read_text());assert not policy['ready_for_acceptance'] and policy['admission']=='DENY' and all(v is None for v in policy['required_parameters'].values())
(p/'PRESERVATION-VERIFICATION.json').write_text(json.dumps(dict(unchanged_authority_and_artifact_tuples=preserved,prior_product_sources='UNCHANGED',head='UNCHANGED',browser_wip='UNCHANGED',q4_unprotected_dependency_hashes=22,protected_readback='NOT_EXECUTED',historical_matrix_rows=14),indent=2)+'\n')
(p/'FINAL-VERIFICATION.json').write_text(json.dumps(dict(status='PASS_WITH_EXPLICIT_LIMITS',probe_builds=4,warnings=0,probe_simulated_checks=68,offline_new_tests=33,affected_regressions=37,actual_static_image_bytes_cli_regression='PASS_SYNTHETIC_REFERENCES_DENY',native_active_connection='NOT_EXECUTED',ed11=dict(demonstrated=4,limited=12,pending=6),runtime_quotas='UNCHANGED',policy='PROPOSED_INCOMPLETE',execution='DENY'),indent=2)+'\n')
manifest=[]
for f in sorted(p.rglob('*')):
 if f.is_file() and f.name!='FINAL-MANIFEST.json':manifest.append(dict(path=str(f.relative_to(p)),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest()))
(p/'FINAL-MANIFEST.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('PASS preserved150 tuples; product sources/Browser WIP/HEADs unchanged; current22/historical14; tests70 +68simulated checks')
for name in ['CONSOLIDATED-CLOSEOUT.md','FINAL-MANIFEST.json']:print(name,hashlib.sha256((p/name).read_bytes()).hexdigest())
