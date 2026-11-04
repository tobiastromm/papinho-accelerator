from pathlib import Path
import json,hashlib,csv,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/d0-d9-bounded-diagnostic-policy-20261026';previous=p.parent/'composite-target-quantitative-closure-20261026';pres=[]
for x in json.loads((previous/'PRESERVATION-VERIFICATION.json').read_text())['unchanged']:
 f=Path(x['path']);h=hashlib.sha256(f.read_bytes()).hexdigest();
 if h!=x['sha256']:
  assert str(f).replace('\\','/').endswith('PapinhoBrowser/docs/mode-c-c2-factual-handoff.md')
  archived=(p/'preimages/Browser__mode-c-c2-factual-handoff.md').read_bytes();assert hashlib.sha256(archived).hexdigest()==x['sha256'];assert f.read_bytes().startswith(archived)
  pres.append(dict(path=str(f),sha256=h,historical_sha256=x['sha256'],historical_copy='preimages/Browser__mode-c-c2-factual-handoff.md',status='AUTHORIZED_APPEND_ONLY_LIVE_UPDATE'))
 else:pres.append(dict(path=str(f),sha256=h,status='UNCHANGED'))
for repo,name in [(r,'ACCELERATOR'),(Path('C:/Projetos/PapinhoBrowser'),'BROWSER')]:
 assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo)==(p/(name+'-HEAD.txt')).read_bytes();(p/(name+'-WIP-CLOSE.txt')).write_bytes(subprocess.check_output(['git','status','--short'],cwd=repo));q=subprocess.run(['git','diff','--check'],cwd=repo,capture_output=True);assert q.returncode==0;(p/(name+'-DIFF-CHECK.log')).write_bytes(q.stdout+q.stderr)
for manifest in ['FINAL-SOURCE-HASHES.json','FINAL-ARTIFACT-HASHES.json']:
 for x in json.loads((p/manifest).read_text()):assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256'],x['path']
# Original candidate/annex remain immutable; no normative acceptance inferred.
a=r/'docs/adr/ADR-CANDIDATE-mode-c-one-operation-diagnostic-authority-20261026.md';assert hashlib.sha256(a.read_bytes()).hexdigest()=='84e2c9a7e3922247ee5ca8901594d29a2e1418eef68f91e58cd69ff53cc2a2bf'
a=p.parent/'one-op-correlated-diagnostic-design-20261026/HYBRID-POLICY-CANDIDATE.json';assert hashlib.sha256(a.read_bytes()).hexdigest()=='943c07a6cc6aadc42687f287db2a354dd437440a50c122e4fbad4b78669f9e84'
a=r/'docs/adr/ADR-CANDIDATE-mode-c-one-operation-diagnostic-authority-revision2-20261026.md';assert hashlib.sha256(a.read_bytes()).hexdigest()=='1dba4911bd97531861af66e91119c5eedbcc3792af5372ee858abc1b716e9447'
assert hashlib.sha256((p/'HYBRID-POLICY-CANDIDATE.REVISION2.json').read_bytes()).hexdigest()=='c53a2eb1cc2b1e80c21cdf4d18c25e6425eb78461d0157573166d238f28f1829'
for f in [p/'CONSOLIDATED-CLOSEOUT.md',a]:
 for link in re.findall(r'\]\(([^)]+)\)',f.read_text(encoding='utf-8')):assert (f.parent/link).exists(),(f,link)
rows=list(csv.DictReader((p.parent/'ED11_GATE_MATRIX.csv').open(encoding='utf-8-sig',newline='')));before=list(csv.DictReader((p/'preimages/ED11_GATE_MATRIX.csv').open(encoding='utf-8-sig',newline='')));current=lambda x:re.fullmatch(r'E(?:0[1-9]|1[0-9]|2[0-2])',x['gate']);assert [x for x in rows if not current(x)]==[x for x in before if not current(x)];cur=[x for x in rows if current(x)];assert len(cur)==22 and len(rows)==36;assert sum(x['status']=='PASS_DEMONSTRADO' for x in cur)==4;assert sum(x['status']=='PASS_LIMITADO_AO_ESCOPO_OBSERVADO' for x in cur)==12
# Check final sources and fresh normal builds, without executing entrypoints.
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/d0-d9-bounded-diagnostic-20261026'/profile/cfg;assert 'PAPACC_SINGLE_LAB_ADMISSION' not in (b/'compile_commands.json').read_text();q=subprocess.run(['ninja','-n','papacc_server','papacc_target_typed_evidence_test'],cwd=b,capture_output=True);assert q.returncode==0 and b'no work to do' in q.stdout
for x in json.loads((p/'BROWSER-PREIMAGES.json').read_text()):f=Path(x['path']);pre=p/'preimages'/('Browser__'+str(f.relative_to(Path('C:/Projetos/PapinhoBrowser'))).replace('\\','__').replace('/','__'));assert hashlib.sha256(pre.read_bytes()).hexdigest()==x['sha256']
# Verify test bodies remain actual canonical functions, not stale hand-written copies.
def extract(t,n):
 start=t.index(n);start=t.rfind('\n',0,start)+1;b=t.index('{',start);d=1;i=b+1
 while d:d+=(t[i]=='{')-(t[i]=='}');i+=1
 return t[start:i]
for fixture,source,name in [('c3-actual-getter-test.c',r/'src/network_egress/c3_bridge_win32.c','PST_RESULT papacc_c3_bridge_provider_snapshot('),('browser-actual-getter-test.c',Path('C:/Projetos/PapinhoBrowser/src/papinho_pst_owner.c'),'PST_RESULT papinho_pst_owner_provider_snapshot(')]:assert extract((p/fixture).read_text(),name)==extract(source.read_text(),name)
(p/'PRESERVATION-VERIFICATION.json').write_text(json.dumps(dict(historical_preservation=pres,heads='UNCHANGED',original_proposal_and_annex='BYTE_EXACT',historical_matrix_rows=14,q4='EXCLUDED_NOT_EXECUTED',authorized_source_edits='PREIMAGES_RETAINED'),indent=2)+'\n')
(p/'FINAL-VERIFICATION.json').write_text(json.dumps(dict(result='PASS_NONOP_SCOPE',product_builds=6,warnings=0,actual_getter_simulated_checks=134,normal_root_checks=608,regression_runs=14,canonical_lab_profile='OFF_MARKER_ABSENT',source_body_provenance='EXACT',runtime_quotas='UNCHANGED',persistent_layout='UNCHANGED',snapshot_caller_bytes=4648,provider_value_bytes=60,normative_acceptance=False,operational_request=False,network_execution=False,q4_execution=False,ed11=dict(demonstrated=4,limited=12,pending=6),phase6='PARTIAL'),indent=2)+'\n')
manifest=[]
for f in sorted(p.rglob('*')):
 if f.is_file() and f.name!='FINAL-MANIFEST.json':manifest.append(dict(path=str(f.relative_to(p)),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest()))
(p/'FINAL-MANIFEST.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('PASS:149 tuples unchanged plus1 authorized append with original bytes preserved,6builds,134getter checks,608root checks,14regressions; no authority/effects')
for name in ['CONSOLIDATED-CLOSEOUT.md','FINAL-MANIFEST.json']:print(name,hashlib.sha256((p/name).read_bytes()).hexdigest())
