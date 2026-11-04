from pathlib import Path
import csv,json,hashlib,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/composite-target-quantitative-closure-20261026';old=p.parent/'q4-target-evidence-closure-20261025'
assert len(json.loads((p/'BUILD-RESULTS.json').read_text()))==4
assert all(x['exit']==0 and x['warnings']==0 for x in json.loads((p/'BUILD-RESULTS.json').read_text()))
assert len(json.loads((p/'COMPILED-CONTROLLED-LAYOUTS.json').read_text()))==6
assert len(json.loads((p/'RUNTIME-RUNS.json').read_text()))==20
assert len(list(csv.DictReader((p/'RUNTIME-SAMPLES.csv').open())))==500
assert len(list(csv.DictReader((p/'PROVIDER-STATE.csv').open())))==240
for x in json.loads((p/'SOURCE-AND-ARTIFACT-HASHES.json').read_text()):assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256']
for x in json.loads((old/'FINAL-SOURCE-HASHES.json').read_text()):assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256']
for x in json.loads((p/'PRESERVATION-VERIFICATION.json').read_text())['unchanged']:assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256']
for x in json.loads((old/'q4-and-critical-path/Q4-REQUEST-DEPENDENCY-READONLY-VERIFICATION.json').read_text())['items']:assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['expected']
# Links in new prose resolve locally, without performing any linked operation.
for f in [p/'CONSOLIDATED-CLOSEOUT.md',r/'docs/adr/ADR-CANDIDATE-mode-c-hybrid-quantitative-evidence-20261026.md']:
 for target in re.findall(r'\]\(([^)]+)\)',f.read_text()):assert (f.parent/target).exists(),(f,target)
current=subprocess.check_output(['git','status','--short'],cwd='C:/Projetos/PapinhoBrowser',text=True).strip();start=(p/'BROWSER-WIP-AT-START.txt').read_text(encoding='utf-8-sig').strip();assert current==start
rows=list(csv.DictReader((p/'CURRENT-ED11-22-GATE-MATRIX.csv').open()));assert len(rows)==22
assert sum(x['status']=='PASS_DEMONSTRADO' for x in rows)==4
assert sum(x['status']=='PASS_LIMITADO_AO_ESCOPO_OBSERVADO' for x in rows)==12
(p/'FINAL-VERIFICATION.json').write_text(json.dumps(dict(result='PASS',builds=4,compile_only_probes=6,analysis_checks=35,successful_native_processes=20,runtime_cycles=240,process_samples=500,provider_records=240,frozen_tuples=150,q4_dependency_hashes=22,prior_source_hashes='UNCHANGED',browser_wip='UNCHANGED',operational_execution=False,ed11='PARTIAL'),indent=2)+'\n')
manifest=[]
for f in sorted(p.rglob('*')):
 if f.is_file() and f.name!='FINAL-MANIFEST.json':manifest.append(dict(path=str(f.relative_to(p)),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest()))
(p/'FINAL-MANIFEST.json').write_text(json.dumps(manifest,indent=2)+'\n')
for name in ['CONSOLIDATED-CLOSEOUT.md','FINAL-MANIFEST.json']:print(name,hashlib.sha256((p/name).read_bytes()).hexdigest())
print('FINAL PASS; files',len(manifest))
