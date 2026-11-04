"""Synthetic analysis checks. Never native target or admission evidence."""
import copy, hashlib, json, unittest
import mode_c_pair_offline_test as base
m=base.m

def coverage():
    receipts, artifacts=base.fixture();rows=[]
    for body in receipts:
        role=body['role']
        for i,phase in enumerate(['BASELINE','INITIALIZED','HANDSHAKE','ACTIVE','STOP','FINAL']):
            rows.append(dict(role=role,process=body['process'],creation=body['creation'],image=body['image'],sdk=body['sdk'],join=body['join'],origin=body['origin'],sequence=i+1,root=body['process']+10,phase=phase,clock='WIN32_GETTICKCOUNT_U32' if role=='BROWSER' else 'PAL_MONOTONIC_NS_TO_MS',begin=i*10,end=i*10+1,provider='retrozilla-nss' if role=='BROWSER' else 'openssl',initialized=phase!='BASELINE',prefixes=[10,20,30,40] if phase=='ACTIVE' else None,metrics=dict(private_bytes=0,commit_bytes=100,handles=12,paged_pool_bytes=None,nonpaged_pool_bytes=None),socket_options=dict(send_bytes=None,receive_bytes=None),lifetime='CLOSED_REPORTED' if phase=='FINAL' else 'LIVE'))
    return [base.pack(x) for x in receipts],artifacts,dict(format='ONE_OPERATION_OFFLINE_COVERAGE',version=1,rows=rows)

class Coverage(unittest.TestCase):
    def assess(self,mutate=None,raw_change=None):
        receipts,artifacts,doc=coverage()
        if mutate:mutate(doc)
        raw=m.canonical(doc)
        digest=hashlib.sha256(raw).hexdigest()
        if raw_change:raw=raw_change(raw)
        return m.assess_one_operation_coverage(*receipts,artifacts,raw,digest)
    def test_reported_never_capacity(self):
        x=self.assess();self.assertEqual(x['status'],'INCOMPLETE');self.assertFalse(x['ED11_PASS']);self.assertFalse(x['credit_granted']);self.assertFalse(x['native_qualified']);self.assertEqual(x['admission'],'DENY');self.assertIn('ACTIVE_KERNEL_ENVELOPE',x['reason']);self.assertEqual(x['peaks']['BROWSER']['private_bytes'],0)
    def test_missing_initialization(self):
        x=self.assess(lambda d:d['rows'][1].update(initialized=None));self.assertIn('PROVIDER_INITIALIZATION_UNPROVEN',x['reason'])
    def test_uninitialized(self):
        x=self.assess(lambda d:d['rows'][1].update(initialized=False));self.assertIn('PROVIDER_INITIALIZATION_UNPROVEN',x['reason'])
    def test_retention(self):
        x=self.assess(lambda d:d['rows'][5].update(lifetime='RETAINED'));self.assertIn('RETAINED_UNKNOWN',x['reason']);self.assertFalse(x['credit_granted'])
    def test_absent_phase(self):self.assertIn('HANDSHAKE_ABSENT',self.assess(lambda d:d['rows'].pop(2))['reason'])
    def test_clock_domains_not_compared(self):
        def change(d):
            for row in d['rows'][6:]:row['begin']+=100000000000;row['end']+=100000000000
        self.assertEqual(self.assess(change)['status'],'INCOMPLETE')
    def test_missing_metrics_not_zero(self):
        x=self.assess(lambda d:[row['metrics'].update(private_bytes=None) for row in d['rows']]);self.assertNotIn('private_bytes',x['peaks']['BROWSER']);self.assertIn('private_bytes',x['reason'])
    def test_prefix_absent(self):self.assertIn('CONNECTION_COUNTERS_ABSENT',self.assess(lambda d:d['rows'][3].update(prefixes=None))['reason'])
    def test_no_rows(self):self.assertIn('BASELINE_ABSENT',self.assess(lambda d:d.update(rows=[]))['reason'])
    def test_hash(self):self.assertEqual(self.assess(raw_change=lambda raw:raw+b' ')['status'],'INVALID')

# Mutations distinguish contradictory identity/data from missing observations.
invalid_cases=[('pid',0,'process',999),('creation',0,'creation',999),('image',0,'image','a'*64),('sdk',0,'sdk','a'*64),('backend',0,'provider','openssl'),('origin',0,'origin','NATIVE'),('clock',0,'clock','PAL_MONOTONIC_NS_TO_MS'),('root_change',1,'root',999),('sequence_replay',1,'sequence',1),('phase_replay',1,'phase','BASELINE'),('clock_regression',1,'end',0),('wrap',0,'end',2**32),('bool_identity',0,'process',True),('bad_init',0,'initialized',1),('prefix_mismatch',3,'prefixes',[0,0,0,0]),('closed_early',0,'lifetime','CLOSED_REPORTED')]
def invalid_test(index,key,value):
    def test(self):self.assertEqual(self.assess(lambda d:d['rows'][index].update({key:value}))['status'],'INVALID')
    return test
for name,index,key,value in invalid_cases:setattr(Coverage,'test_invalid_'+name,invalid_test(index,key,value))
class InvalidShape(unittest.TestCase):
    assess=Coverage.assess
    def test_extra_field(self):self.assertEqual(self.assess(lambda d:d.update(grant=True))['status'],'INVALID')
    def test_bool_metric(self):self.assertEqual(self.assess(lambda d:d['rows'][0]['metrics'].update(handles=True))['status'],'INVALID')
    def test_negative_socket(self):self.assertEqual(self.assess(lambda d:d['rows'][0]['socket_options'].update(send_bytes=-1))['status'],'INVALID')
    def test_wrong_join(self):
        def change(d):d['rows'][0]['join']=dict(d['rows'][0]['join'],operation=999)
        self.assertEqual(self.assess(change)['status'],'INVALID')
class ArtifactBounds(unittest.TestCase):
    def test_large_image_preserves_receipt_limit(self):
        bodies,artifacts=base.fixture()
        for i,b in enumerate(bodies):
            del artifacts[b['image']]
            raw=bytes([i+1])*(m.MAX+1)
            b['image']=hashlib.sha256(raw).hexdigest();artifacts[b['image']]=raw
        result=m.consume_mode_c(*[base.pack(b) for b in bodies],artifacts)
        self.assertEqual(result['status'],'CORRELATED');self.assertEqual(m.MAX,262144)
        self.assertEqual(result['admission'],'DENY')
    def test_aggregate_bound(self):
        bodies,artifacts=base.fixture();artifacts['x']=b'x'*(m.ARTIFACT_BYTES+1)
        self.assertEqual(m.consume_mode_c(*[base.pack(b) for b in bodies],artifacts)['status'],'INVALID')
    def test_count_bound(self):
        bodies,artifacts=base.fixture()
        for n in range(17):artifacts[str(n)]=b'x'
        self.assertEqual(m.consume_mode_c(*[base.pack(b) for b in bodies],artifacts)['status'],'INVALID')
if __name__=='__main__':unittest.main(verbosity=2)
