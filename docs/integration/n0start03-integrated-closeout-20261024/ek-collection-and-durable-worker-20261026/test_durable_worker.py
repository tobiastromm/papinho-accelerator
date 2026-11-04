import unittest,subprocess,sys,os,hashlib,struct,json,uuid,threading
from pathlib import Path
from isolated_protocol import Record
from durable_worker_fixture import DurableWorker,parse,Denied
HERE=Path(__file__).resolve().parent
B=tuple(hashlib.sha256(('synthetic-'+str(n)).encode()).digest() for n in range(9))
class WorkerTests(unittest.TestCase):
 def setup_case(self):
  folder=HERE/'runs'/uuid.uuid4().hex;folder.mkdir(parents=True);(folder/'root.fixture').write_bytes(b'R')
  v=B[4]+struct.pack('>Q',17);(folder/'nv.SIMULATED.fixture').write_bytes(v+hashlib.sha256(v).digest())
  r=Record(B,17,18,42,100);(folder/'record.fixture').write_bytes(r.encode());return folder,r
 def run_worker(self,folder,fault=None):
  env=os.environ.copy();env.pop('Q4_FAULT',None)
  if fault:env['Q4_FAULT']=fault
  r=subprocess.run([sys.executable,str(HERE/'durable_worker_fixture.py'),'--synthetic-only',str(folder),str(folder/'record.fixture'),'SIMULATED_NO_AUTHORITY'],env=env,capture_output=True,timeout=10)
  (folder/('receipt-'+uuid.uuid4().hex+'.json')).write_bytes(r.stdout)
  return r
 def test_clean_persists_and_restart_denies(self):
  f,_=self.setup_case();a=self.run_worker(f);self.assertEqual(a.returncode,0,a.stdout);self.assertEqual([x.state for x in parse((f/'root.fixture').read_bytes())[0]],['PREPARED','CONSUMED','RESULT']);self.assertEqual(self.run_worker(f).returncode,2)
 def test_crash_boundaries(self):
  for fault in ['before_prepare','after_prepare','after_increment','after_commit','after_read_slot','after_result','torn_prepare','torn_commit','torn_result']:
   with self.subTest(fault=fault):
    f,_=self.setup_case();a=self.run_worker(f,fault);self.assertEqual(a.returncode,90)
    b=self.run_worker(f)
    # Before any persisted preparation: a fresh synthetic call may succeed, no consumed command existed.
    self.assertEqual(b.returncode,0 if fault=='before_prepare' else 2,b.stdout)
 def test_rollback_root_only(self):
  f,_=self.setup_case();self.assertEqual(self.run_worker(f).returncode,0);(f/'root.fixture').write_bytes(b'R');self.assertEqual(self.run_worker(f).returncode,2)
 def test_restoring_both_is_not_independence(self):
  f,_=self.setup_case();old=(f/'nv.SIMULATED.fixture').read_bytes();self.run_worker(f);(f/'root.fixture').write_bytes(b'R');(f/'nv.SIMULATED.fixture').write_bytes(old);self.assertEqual(self.run_worker(f).returncode,0)
 def test_missing_witness(self):
  f,_=self.setup_case();(f/'nv.SIMULATED.fixture').rename(f/'witness.lost.fixture');self.assertEqual(self.run_worker(f).returncode,2);self.assertEqual((f/'root.fixture').read_bytes(),b'R')
 def test_wrong_witness_identity(self):
  f,_=self.setup_case();v=B[0]+struct.pack('>Q',17);(f/'nv.SIMULATED.fixture').write_bytes(v+hashlib.sha256(v).digest());self.assertEqual(self.run_worker(f).returncode,2)
 def test_corrupt_witness(self):
  f,_=self.setup_case();(f/'nv.SIMULATED.fixture').write_bytes(b'bad');self.assertEqual(self.run_worker(f).returncode,2)
 def test_ambiguous_increment_no_replay(self):
  f,_=self.setup_case();a=self.run_worker(f,'ambiguous_increment');self.assertEqual(a.returncode,2);self.assertIn(b'INCREMENT_UNKNOWN',a.stdout);self.assertEqual(parse((f/'root.fixture').read_bytes())[0][-1].state,'UNKNOWN');self.assertEqual(self.run_worker(f).returncode,2);self.assertEqual(struct.unpack('>Q',(f/'nv.SIMULATED.fixture').read_bytes()[32:40])[0],18)
 def test_stop_after_prepare_no_increment(self):
  f,_=self.setup_case();self.assertEqual(self.run_worker(f,'stop').returncode,2);self.assertEqual(struct.unpack('>Q',(f/'nv.SIMULATED.fixture').read_bytes()[32:40])[0],17);self.assertEqual(self.run_worker(f).returncode,2)
 def test_close_unknown(self):
  f,_=self.setup_case();r=self.run_worker(f,'close_unknown');self.assertEqual(r.returncode,2);data=json.loads(r.stdout);self.assertTrue(data['retained']);self.assertFalse(data['close']);self.assertEqual(data['first'],'CLOSE_UNKNOWN')
 def test_process_concurrency(self):
  f,_=self.setup_case();args=[sys.executable,str(HERE/'durable_worker_fixture.py'),'--synthetic-only',str(f),str(f/'record.fixture'),'SIMULATED_NO_AUTHORITY'];ps=[subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE) for _ in range(12)];exits=[]
  for p in ps:p.communicate(timeout=15);exits.append(p.returncode)
  self.assertEqual(exits.count(0),1,exits);self.assertEqual(struct.unpack('>Q',(f/'nv.SIMULATED.fixture').read_bytes()[32:40])[0],18)
 def test_original_lease_busy(self):
  f,r=self.setup_case();a=DurableWorker(f/'root.fixture',f/'nv.SIMULATED.fixture',r)
  try:self.assertEqual(self.run_worker(f).returncode,2);self.assertTrue(a.run());self.assertFalse(a.run())
  finally:a.close()
 def test_corrupt_root(self):
  f,_=self.setup_case();self.run_worker(f);raw=bytearray((f/'root.fixture').read_bytes());raw[100]^=1;(f/'root.fixture').write_bytes(raw);self.assertEqual(self.run_worker(f).returncode,2)
 def test_wrong_approval_replay(self):
  f,r=self.setup_case();self.run_worker(f);from dataclasses import replace
  (f/'record.fixture').write_bytes(replace(r,bindings=(B[1],)+B[1:]).encode());self.assertEqual(self.run_worker(f).returncode,2)
if __name__=='__main__':unittest.main(verbosity=2)
