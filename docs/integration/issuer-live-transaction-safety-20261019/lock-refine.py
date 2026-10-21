from pathlib import Path
import hashlib,json
p=Path('docs/integration/issuer-live-transaction-safety-20261019');f=p/'proposed/apps__server__server_configuration_pending_win32.inc';s=f.read_text().replace('PAPACC_RESULT papacc_configuration_issuer_begin06(','static PAPACC_RESULT pending_begin06(',1).replace('PAPACC_RESULT papacc_configuration_issuer_poll06(','static PAPACC_RESULT pending_poll06(',1)
s+='''
PAPACC_RESULT papacc_configuration_issuer_begin06(PAPACC_CONFIGURATION_COMMIT *o)
{PAPACC_RESULT r;if(!owner(o))return PAPACC_RESULT_INVALID_STATE;EnterCriticalSection(&o->lock);r=pending_begin06(o);LeaveCriticalSection(&o->lock);return r;}
PAPACC_RESULT papacc_configuration_issuer_poll06(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_ISSUER_CAS_RESULT *out)
{PAPACC_RESULT r;if(!owner(o))return PAPACC_RESULT_INVALID_STATE;EnterCriticalSection(&o->lock);r=pending_poll06(o,out);LeaveCriticalSection(&o->lock);return r;}
''';f.write_bytes(s.encode());m=json.loads((p/'operation.json').read_text())
for e in m:e['after']=hashlib.sha256((p/'proposed'/e['path'].replace('/','__')).read_bytes()).hexdigest()
(p/'operation.json').write_text(json.dumps(m,indent=2))
