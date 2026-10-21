"""Synthetic reference receipts; no native or ED11 qualification."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import unittest
spec=importlib.util.spec_from_file_location('pair',Path(__file__).resolve().parents[1]/'tools/offline/mode_c_pair.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

def pack(body):
    raw=m.canonical(body)
    return m.canonical(dict(format='MODE_C_RECEIPT_REFERENCES',version=1,body=body,body_bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()))

def fixture(change=None):
    artifacts={}; receipts=[]
    def add(raw):
        h=hashlib.sha256(raw).hexdigest();artifacts[h]=raw;return h
    for role,pid in [('BROWSER',11),('ACCELERATOR',22)]:
        join=dict(request=1,operation=2,generation=3,epoch=4,attempt=5,resolution=6)
        body=dict(role=role,run=role+'-full-run-identity',process=pid,creation=100+pid,image=add((role+' image').encode()),sdk=add((role+' normal SDK').encode()),build_ref=add((role+' manifest').encode()),origin='SYNTHETIC',join=join,refs=[],incomplete=False)
        facts=[]
        for seq,kind in enumerate(m.STAGES,1):
            action=('VALIDATED_RECEIVE' if role=='BROWSER' else 'FULL_WRITE') if kind in ('OPEN_RESULT','ACK_APP','ACK_RECORD','TLS_READY','FINAL') else ('FULL_WRITE' if role=='BROWSER' else 'VALIDATED_RECEIVE') if kind in ('BIND_APP','BIND_RECORD') else 'LOCAL_FACT'
            value={'version':0x0304,'verification':7,'alpn':'http/1.1'} if kind=='TLS_READY' else [10,20,30,40] if kind in ('PREFIXES','FENCES','ACKS') else dict(disposition='DESTROYED',callbacks=0,leases=0) if kind=='TEARDOWN' else dict(outcome=1,ownership=1) if kind=='FINAL' else None
            facts.append(dict(sequence=seq,kind=kind,action=action,values=value))
        if change: change(role,body,facts)
        source={k:body[k] for k in ('role','run','process','creation','origin','join')};source['facts']=facts
        digest=add(m.canonical(source))
        body['refs']=[dict(artifact=digest,sequence=f['sequence'],kind=f['kind']) for f in facts]
        receipts.append(body)
    return receipts,artifacts

class Cases(unittest.TestCase):
    def result(self,change=None,mutate=None):
        b,a=fixture(change)
        if mutate:mutate(b,a)
        return m.consume_mode_c(pack(b[0]),pack(b[1]),a)
    def test_valid(self):
        r=self.result();self.assertEqual(r['status'],'CORRELATED');self.assertFalse(r['native_qualified']);self.assertEqual(r['admission'],'DENY')
    def test_identity(self):
        def change(b,a): b[0]['process']=99
        self.assertEqual(self.result(mutate=change)['status'],'INVALID')
    def test_generation(self):
        self.assertEqual(self.result(lambda role,b,f:b['join'].update(generation=9) if role=='BROWSER' else None)['status'],'INVALID')
    def test_epoch(self):
        self.assertEqual(self.result(lambda role,b,f:b['join'].update(epoch=9) if role=='BROWSER' else None)['status'],'INVALID')
    def test_missing_open_provenance(self):
        self.assertEqual(self.result(lambda role,b,f:b['join'].update(attempt=0))['status'],'INVALID')
    def test_ack_without_receipt(self):
        self.assertEqual(self.result(lambda role,b,f:f[2].update(action='SEND_ONLY') if role=='BROWSER' else None)['status'],'INVALID')
    def test_final_without_teardown(self):
        self.assertEqual(self.result(lambda role,b,f:f.pop())['status'],'INCOMPLETE')
    def test_retained(self):
        def change(role,b,f):f[-1]['values']['disposition']='CLOSE_UNRESOLVED_RETAINED'
        self.assertEqual(self.result(change)['status'],'INCOMPLETE')
    def test_missing_evidence(self):
        def change(b,a): del a[b[0]['refs'][0]['artifact']]
        self.assertEqual(self.result(mutate=change)['status'],'INCOMPLETE')
    def test_duplicate(self):
        def change(b,a):b[0]['refs'].append(copy.deepcopy(b[0]['refs'][0]))
        self.assertEqual(self.result(mutate=change)['status'],'INVALID')
    def test_integrity(self):
        b,a=fixture();env=json.loads(pack(b[0]));env['sha256']='0'*64
        self.assertEqual(m.consume_mode_c(m.canonical(env),pack(b[1]),a)['status'],'INVALID')
    def test_causal_order(self):
        def change(role,b,f):f[1]['kind'],f[2]['kind']=f[2]['kind'],f[1]['kind'];f[1]['action'],f[2]['action']=f[2]['action'],f[1]['action']
        self.assertEqual(self.result(change)['status'],'INVALID')
    def test_origin(self):
        self.assertEqual(self.result(lambda role,b,f:b.update(origin='NATIVE') if role=='BROWSER' else None)['status'],'INVALID')
    def test_prefix(self):
        self.assertEqual(self.result(lambda role,b,f:f[6]['values'].__setitem__(0,99))['status'],'INVALID')
    def test_null_join(self):
        self.assertEqual(self.result(lambda role,b,f:b.update(join=None))['status'],'INCOMPLETE')
    def test_snapshot_incomplete(self):
        self.assertEqual(self.result(lambda role,b,f:b.update(incomplete=True))['status'],'INCOMPLETE')
    def test_source_hash(self):
        def change(b,a):a[b[0]['refs'][0]['artifact']]+=b' '
        self.assertEqual(self.result(mutate=change)['status'],'INVALID')

    def test_boolean_final(self):
        def change(role,b,f):f[9]['values']['outcome']=True
        self.assertEqual(self.result(change)['status'],'INVALID')
    def test_boolean_join(self):
        self.assertEqual(self.result(lambda role,b,f:b['join'].update(request=True))['status'],'INVALID')
if __name__=='__main__':unittest.main(verbosity=2)