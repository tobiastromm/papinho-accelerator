"""Producer binding fixtures remain SYNTHETIC; no native HTTPS or approval."""
import copy
import hashlib
import importlib.util
from pathlib import Path
import unittest
spec=importlib.util.spec_from_file_location('prior',Path(__file__).with_name('mode_c_pair_offline_test.py'))
prior=importlib.util.module_from_spec(spec);spec.loader.exec_module(prior)
m=prior.m

def producer_fixture():
    bodies,artifacts=prior.fixture(); records={}
    for b in bodies:
        p=dict(format='C8_OWN_PROCESS_PROVENANCE' if b['role']=='ACCELERATOR' else 'BROWSER_OWN_PROCESS_PROVENANCE',version=1,origin='SYNTHETIC',process=b['process'],creation=b['creation'],thread=9,invocation=1,sequence=1,revision=2,epoch=4,configured=1,current=1,phase=1,build_debug=1,sdk_inventory='c'*64,image_path='C:\\fixture\\'+b['role']+'.exe',mapped_path='\\Device\\Fixture\\'+b['role']+'.exe',ed11=dict(private=12,baseline=10,peak=13,handles=1,physical=20,commit=30),provider='UNKNOWN',kernel='UNKNOWN',admission='DENY')
        l=dict(origin='SYNTHETIC',process=b['process'],creation=b['creation'],observed_module_path=p['image_path'],executed_path=p['image_path'],mapped_image_path=p['mapped_path'],actual_file_sha256=b['image'],file_sha256_before=b['image'],exit=0)
        build=dict(image_sha256=b['image'],sdk_artifact_sha256=b['sdk'],sdk_compiled_identity=p['sdk_inventory'],sdk_profile='NORMAL_PST_065_API3_SPI4',configuration='Debug')
        raw=m.canonical(build);b['build_ref']=hashlib.sha256(raw).hexdigest();artifacts[b['build_ref']]=raw
        b['producer_ref']=hashlib.sha256(m.canonical(p)).hexdigest();records[b['role']]=[p,l,build]
    return bodies,artifacts,records

class Cases(unittest.TestCase):
    def run_case(self,change=None,drop=None):
        b,a,p=producer_fixture()
        if change:change(b,a,p)
        bindings={}
        for body in b:
            role=body['role'];proof,launch,build=p[role]
            # Reseal actual fixture documents to isolate semantic mismatch tests.
            body['producer_ref']=hashlib.sha256(m.canonical(proof)).hexdigest()
            body['build_ref']=hashlib.sha256(m.canonical(build)).hexdigest();a[body['build_ref']]=m.canonical(build)
            bindings[role]=(m.canonical(proof),m.canonical(launch),m.canonical(build))
        if drop:bindings.pop(drop)
        return m.consume_provenanced_mode_c(prior.pack(b[0]),prior.pack(b[1]),a,bindings)
    def test_valid_synthetic(self):
        r=self.run_case();self.assertEqual(r['status'],'CORRELATED');self.assertFalse(r['native_qualified']);self.assertEqual(r['admission'],'DENY')
    def test_pid_reuse(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['BROWSER'][0].update(creation=999))['status'],'INVALID')
    def test_image(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][1].update(actual_file_sha256='d'*64))['status'],'INVALID')
    def test_sdk(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][0].update(sdk_inventory='d'*64))['status'],'INVALID')
    def test_no_producer(self):self.assertEqual(self.run_case(drop='BROWSER')['status'],'INCOMPLETE')
    def test_epoch(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][0].update(epoch=9))['status'],'INVALID')
    def test_absent_current(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][0].update(current=0,configured=0,epoch=0,revision=0))['status'],'INCOMPLETE')
    def test_expected_path_only(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][1].update(observed_module_path='C:/other.exe'))['status'],'INVALID')
    def test_wrong_build(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['BROWSER'][2].update(configuration='Release'))['status'],'INVALID')
    def test_integrity(self):
        b,a,p=producer_fixture();q,l,d=p['ACCELERATOR'];b[1]['producer_ref']='0'*64
        self.assertEqual(m.validate_producer_binding(prior.pack(b[1]),m.canonical(q),m.canonical(l),m.canonical(d))['status'],'INVALID')
    def test_missing_proof(self):
        b,a,p=producer_fixture();self.assertEqual(m.validate_producer_binding(prior.pack(b[0]),None,None,None)['status'],'INCOMPLETE')
    def test_bad_peak(self):
        def change(b,a,p):p['BROWSER'][0]['ed11']['peak']=1
        self.assertEqual(self.run_case(change)['status'],'INVALID')
    def test_unknown_preserved(self):
        r=self.run_case();self.assertFalse(r['ED11_PASS']);self.assertEqual(r['admission'],'DENY')
    def test_operation_generation(self):
        def change(b,a,p):b[0]['join']['generation']=88
        self.assertEqual(self.run_case(change)['status'],'INVALID')
    def test_native_claim_from_fixture(self):
        self.assertEqual(self.run_case(lambda b,a,p:p['ACCELERATOR'][0].update(origin='NATIVE'))['status'],'INVALID')

if __name__=='__main__':unittest.main(verbosity=2)