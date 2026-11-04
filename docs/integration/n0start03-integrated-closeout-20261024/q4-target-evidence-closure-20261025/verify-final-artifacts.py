from pathlib import Path
import json,hashlib,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025';rows=[]
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/q4-target-evidence-20261025'/pr/cfg;cmds=(b/'compile_commands.json').read_text();assert 'PAPACC_SINGLE_LAB_ADMISSION' not in cmds
  for name in ['papacc_server','papacc_target_typed_evidence_test','papacc_process_provenance_offline_test','papacc_pair_receipt_offline_test']:
   f=b/(name+'.exe');data=f.read_bytes();marker='Global\\PapinhoAccelerator.ED11.SingleLabAdmission';has=marker.encode('utf-16-le') in data or marker.encode() in data;assert not has
   rows.append(dict(profile=pr,config=cfg,artifact=name,path=str(f),bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),canonical_lab_mutex_marker_present=has,operational_activation='NOT_AUTHORIZED'))
(p/'ACCELERATOR-ARTIFACT-HASHES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
old=p.parent/'r0-r9-acceptance-and-integration-20261025';pres=[]
for file in ['AUTHORITY-PRESERVATION.json','HISTORICAL-ARTIFACT-PRESERVATION.json']:
 for x in json.loads((old/file).read_text(encoding='utf-8')):
  path=Path(x['path']);expect=x.get('actual',x.get('expected'));h=hashlib.sha256(path.read_bytes()).hexdigest();pres.append(dict(path=str(path),sha256=h,expected=expect,unchanged=h==expect))
assert all(x['unchanged'] for x in pres)
(p/'HISTORICAL-AUTHORITY-PRESERVATION.json').write_text(json.dumps(pres,indent=2)+'\n',encoding='utf-8')
q=json.loads((p/'q4-and-critical-path/Q4-REQUEST-DEPENDENCY-READONLY-VERIFICATION.json').read_text())
for x in q['items']:assert hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['expected']
for path,x in json.loads((p/'browser/FINAL-ARTIFACT-HASHES.json').read_text()).items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==x['sha256']
print('Final normal fixture binaries exclude LAB marker; frozen tuples',len(pres),'Q4 hashes',len(q['items']),'Browser hashes match')
print('Selected Release server',next(x['sha256'] for x in rows if x['artifact']=='papacc_server' and x['profile']=='Selected' and x['config']=='Release'))
