"""Check derived audit artifacts, not product qualification or admission."""
import csv
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
errors=[]
def read(name):
    return json.loads((HERE/name).read_text(encoding='utf-8'))
def rows(name):
    return list(csv.DictReader((HERE/name).open(encoding='utf-8-sig',newline='')))
e=read('EXECUTION-VERIFICATION.json')
if e['errors'] or e['status']!='PASS_SOURCE_AND_DOCUMENT_AUDIT_ONLY': errors.append('source audit failed')
for name,pin in e['pins'].items():
    p=(ROOT.parent/'PapinhoBrowser'/name[2:]) if name.startswith('B:') else ROOT/name
    if hashlib.sha256(p.read_bytes()).hexdigest()!=pin: errors.append('source changed: '+name)
a=read('D4-D5-ANNEX.json')
if a['operational_request_ready'] or a['command'] is not None or a['approval'] is not None:
    errors.append('premature operational request')
for name in ('target_maximum_age','target_collection_validity','diagnostic_overall_duration','physical_headroom','commit_headroom'):
    found=[p for p in a['parameters'] if p['name']==name]
    if len(found)!=1 or found[0]['value'] is not None: errors.append('invented/missing parameter: '+name)
if len(rows('JOURNEYS.csv'))!=21: errors.append('incomplete journey checklist')
if len(rows('CALLER-MAP.csv'))!=36: errors.append('incomplete caller map')
if any(d['status']!='ACCEPTED' or d['operations_granted'] for d in read('DECISION-STATE.json')):
    errors.append('decision/operation confusion')
matrix=rows('../phase6-original-scope-recovery-20261028/REQUIREMENTS.csv')
byid={r['id']:r for r in matrix}
for name in ('PROV-MANUAL','PROV-AUTO','PROV-CA'):
    if 'phase6-product-closure-offline' not in byid[name]['existing_evidence']:
        errors.append('matrix missing current source findings: '+name)
files={str(p.relative_to(HERE)):hashlib.sha256(p.read_bytes()).hexdigest()
       for p in HERE.rglob('*') if p.is_file() and p.name!='FINAL-VERIFICATION.json'}
result=dict(status='PASS_DERIVED_AUDIT_CONSISTENCY_ONLY' if not errors else 'FAIL',errors=errors,
            files=files,product_builds='NOT_RUN',product_regressions='NOT_RUN',operations='NONE',
            source_pins_unchanged=True if not errors else None,ed11='PARTIAL',phase6='PARTIAL')
(HERE/'FINAL-VERIFICATION.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps({k:v for k,v in result.items() if k!='files'}))
raise SystemExit(bool(errors))
