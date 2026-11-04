from pathlib import Path
import subprocess,json
r=Path.cwd();d=r/'docs/integration/phase6-virtual-scm-integration-20261029';p=r/'apps/server/personal_scq_recipient_win32.inc'
mutants=[('DATA_receipt_claims_close','if(t->close_accepted&&(t->legs&8U)){if(t->legs&16U)return 0;operation=16U;}else operation=8U;','if(t->legs&2U)return 0;operation=8U;'),('pending_io_resubmitted','if(t->pending)e=scqt_poll(c,owner);','if(t->pending)e=scqt_start_io(c,owner,operation,local,remote,p.refs,proof,stop);'),('readiness_before_controller_return','if(p.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&(!c->transport.close_confirmed||!(c->transport.legs&32U)))return ERROR_INVALID_STATE;','/* original controller return guard removed */')]
results=[]
for name,a,b in mutants:
 original=p.read_bytes();assert original.count(a.encode())==1
 try:
  p.write_bytes(original.replace(a.encode(),b.encode()))
  build=subprocess.run(['cmd','/c',str(d/'TSP-mutant-build.cmd')],capture_output=True,text=True)
  (d/f'TSP-mutant-{name}-build.log').write_text(build.stdout+build.stderr)
  assert build.returncode==0,(name,build.stdout[-1500:])
  test=subprocess.run([str(r/'build/phase6-functional-20261028/Selected/Debug/papacc_scq_transport_continuation_test.exe')],capture_output=True,text=True)
  (d/f'TSP-mutant-{name}-test.log').write_text(test.stdout+test.stderr)
  results.append(dict(mutant=name,compile='PASS',rejected=test.returncode!=0,exit=test.returncode))
 finally:p.write_bytes(original)
(d/'TSP-MUTANTS.json').write_text(json.dumps(results,indent=2)+'\n')
assert all(x['rejected'] for x in results),results
print(json.dumps(results,indent=2))
