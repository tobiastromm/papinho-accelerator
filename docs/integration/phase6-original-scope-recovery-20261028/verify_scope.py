"""Passive documentary validation. No product tests or operational APIs."""
import csv
import hashlib
import json
import subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
BROWSER = ROOT.parent / 'PapinhoBrowser'
def gitref(root):
    return subprocess.check_output(['git', '-C', str(root), 'rev-parse', 'HEAD'], text=True).strip()
def resolve(name):
    return BROWSER / name[2:] if name.startswith('B:') else ROOT / name

rows=list(csv.DictReader((HERE/'REQUIREMENTS.csv').open(encoding='utf-8-sig',newline='')))
ids=[r['id'] for r in rows]
required={f'C{i}' for i in range(13)} | {f'E{i:02}' for i in range(1,23)} | {f'MODE-{i}' for i in 'ABCD'}
errors=[]
if len(ids)!=len(set(ids)): errors.append('duplicate IDs')
if required-set(ids): errors.append('missing mandatory rows: '+str(sorted(required-set(ids))))
for row in rows:
    if any(not str(value).strip() for value in row.values()): errors.append('empty field '+row['id'])

pins={}
for row in rows:
    for field in ('normative_origin','existing_evidence'):
        # E01–E22 carry verbatim narrative evidence, with the original CSV pinned separately.
        for name in row[field].split(';'):
            name=name.strip()
            if not (name.startswith('docs/') or name.startswith('B:docs/')): continue
            # Narrative evidence strings are not machine paths.
            if not name.endswith(('.md','.txt','.csv','.json')): continue
            path=resolve(name)
            if not path.is_file(): errors.append('unresolved reference '+row['id']+': '+name)
            else: pins[name]=hashlib.sha256(path.read_bytes()).hexdigest()
for entry in csv.DictReader((HERE/'CALLER-BOUNDARIES.csv').open(encoding='utf-8',newline='')):
    path=resolve(entry['source'])
    if not path.is_file() or entry['symbol'] not in path.read_text(encoding='utf-8-sig',errors='replace'):
        errors.append('source/symbol missing: '+entry['source']+':'+entry['symbol'])
    else: pins[entry['source']]=hashlib.sha256(path.read_bytes()).hexdigest()
for name in ('README.md','DEPENDENCY-CHAIN.md','NEXT-TECHNICAL-EXECUTION.md','CALLER-BOUNDARIES.csv','REQUIREMENTS.csv','build_matrix.py','verify_scope.py'):
    pins[name]=hashlib.sha256((HERE/name).read_bytes()).hexdigest()

original=ROOT/'docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv'
orig={r['gate']:r['status'] for r in csv.DictReader(original.open(encoding='utf-8-sig',newline='')) if r['gate'] in required}
for row in rows:
    if row['id'] in orig and row['implementation']!=orig[row['id']]: errors.append('historical status changed '+row['id'])
for name in ('docs/capabilities/phase6-scope-and-closure-20261028.md',
             'docs/capabilities/canonical-phase6-consolidation.md',
             'docs/phase6-network-tls-offload.md'):
    main=ROOT/name
    pins[name]=hashlib.sha256(main.read_bytes()).hexdigest()
result=dict(status='PASS_DOCUMENT_STRUCTURE_ONLY' if not errors else 'FAIL',errors=errors,
            accelerator_head=gitref(ROOT),browser_head=gitref(BROWSER),rows=len(rows),
            c0_c12=13,ed11_rows=22,placements=4,source_hashes=pins,
            product_builds='NOT_RUN_BY_THIS_DOCUMENT_CHECK; SEE_FUNCTIONAL_QUALIFICATION_REPORT',operations='NONE',
            historical_reservations='PRESERVED_NOT_REFUNDED',ed11='PARTIAL',phase6='PARTIAL',authority='DENY')
(HERE/'SCOPE-VERIFICATION.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps({k:v for k,v in result.items() if k!='source_hashes'},ensure_ascii=False))
raise SystemExit(bool(errors))
