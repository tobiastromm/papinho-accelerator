from pathlib import Path
import csv,json,hashlib,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/composite-target-quantitative-closure-20261026';old=p.parent/'q4-target-evidence-closure-20261025'
pres=[]
for x in json.loads((old/'HISTORICAL-AUTHORITY-PRESERVATION.json').read_text()):
 f=Path(x['path']);h=hashlib.sha256(f.read_bytes()).hexdigest();assert h==x['sha256'];pres.append(dict(path=str(f),sha256=h,unchanged=True))
for manifest in [old/'ACCELERATOR-ARTIFACT-HASHES.json',old/'browser/FINAL-ARTIFACT-HASHES.json']:
 data=json.loads(manifest.read_text());items=data if isinstance(data,list) else [dict(path=k,**v) for k,v in data.items()]
 for x in items:
  f=Path(x['path']);h=hashlib.sha256(f.read_bytes()).hexdigest();assert h==x['sha256'];pres.append(dict(path=str(f),sha256=h,unchanged=True))
for x in json.loads((old/'q4-and-critical-path/Q4-REQUEST-DEPENDENCY-READONLY-VERIFICATION.json').read_text())['items']:
 f=Path(x['path']);assert hashlib.sha256(f.read_bytes()).hexdigest()==x['expected']
for repo,expect in [(r,'f341985924d29870d8ff627ed37c8a0874ad6469'),(Path('C:/Projetos/PapinhoBrowser'),'96f6cd2f9ec4f834999828fa13bb7d170db66a98')]:assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()==expect
(p/'PRESERVATION-VERIFICATION.json').write_text(json.dumps(dict(unchanged=pres,q4_unprotected_dependency_hashes_verified=22,protected_q4_readback='NOT_EXECUTED',head_unchanged=True),indent=2)+'\n')
# Keep all historical rows in live matrix. The snapshot contains exactly current E01-E22.
f=p.parent/'ED11_GATE_MATRIX.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);pre=p/'preimages';pre.mkdir(exist_ok=True);(pre/f.name).write_bytes(f.read_bytes());ref='composite-target-quantitative-closure-20261026/CONSOLIDATED-CLOSEOUT.md'
for x in rows:
 if x['gate']=='E02':x['current_evidence']+='; compiled bounds: C2 manager527824, bridge132448, Browser child charge1858020bytes; '+ref
 if x['gate']=='E19':
  x['current_evidence']+='; new native runtime-core20processes/240lifetimes/500samples providerinitialized0; six compiled layout probes; '+ref
  x['factual_gap']='Active provider/kernel/transitive ceilings or delimited empirical coverage missing; core-only zero sampled deltas are not zero cost. Hybrid candidate not approved; target freshness/headroom absent'
 if x['gate']=='E20':
  x['current_evidence']+='; actual VC6 Browser charge1858020bytes fits once, twice3716040 exceeds2097152 controlled cap; no quota change; '+ref
  x['factual_gap']='No active correlated pair or approved policy. Slot maximum2 is not guarantee of two reservations; even one remains denied without provider/kernel/host policy'
 if x['gate']=='E22':x['current_evidence']+='; quantitative investigation adds controlled bounds/core observations only; '+ref
with f.open('w',encoding='utf-8',newline='') as o:w=csv.DictWriter(o,fields);w.writeheader();w.writerows(rows)
current=[x for x in rows if re.fullmatch(r'E(?:0[1-9]|1[0-9]|2[0-2])',x['gate'])];assert len(current)==22 and len(rows)==36
with (p/'CURRENT-ED11-22-GATE-MATRIX.csv').open('w',encoding='utf-8',newline='') as o:w=csv.DictWriter(o,fields);w.writeheader();w.writerows(current)
(p/'MATRIX-CLASSIFICATION.json').write_text(json.dumps(dict(current_gates=22,historical_rows_preserved=14,demonstrated=4,limited=12,pending=6,ed11='PARTIAL',phase6='PARTIAL'),indent=2)+'\n')
print('Preserved',len(pres),'authority/artifact tuples; Q4 hashes22; current gates22/historical14')
