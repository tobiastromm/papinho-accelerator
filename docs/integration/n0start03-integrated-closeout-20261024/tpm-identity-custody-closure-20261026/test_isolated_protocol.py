import unittest,dataclasses,hashlib,subprocess,threading
from pathlib import Path
from isolated_protocol import *
from offline_public_evidence import OfflinePublicEvidence,EvidenceDenied
from unittest.mock import patch
B=tuple(hashlib.sha256(x.encode()).digest() for x in FIELDS)
def record():return Record(B,17,18,42,100)
def witness():return SimulatedWitness(17,B[4])
def ok(_):return True
class ProtocolTests(unittest.TestCase):
 def test_codec_roundtrip(self):
  r=record();self.assertEqual(r,Record.decode(r.encode()));self.assertEqual(len(r.encode()),372)
 def test_all_byte_corruptions(self):
  data=record().encode()
  for n in range(len(data)):
   with self.subTest(n=n):
    bad=bytearray(data);bad[n]^=1
    with self.assertRaises(Denied):Record.decode(bytes(bad))
 def test_truncation_and_extension(self):
  for n in range(len(record().encode())):
   with self.assertRaises(Denied):Record.decode(record().encode()[:n])
  with self.assertRaises(Denied):Record.decode(record().encode()+b'x')
 def test_invalid_fields(self):
  for r in [dataclasses.replace(record(),baseline=MAX,expected=0),dataclasses.replace(record(),pid=0),dataclasses.replace(record(),creation=0),dataclasses.replace(record(),bindings=B[:-1]),dataclasses.replace(record(),bindings=(bytes(32),)+B[1:]),dataclasses.replace(record(),state='FREE')]:
   with self.assertRaises(Denied):r.encode()
 def test_clean_single_use(self):
  i=Invocation(record());w=witness();self.assertTrue(i.consume(w,ok,ok,B[4]));self.assertTrue(i.read_once(B,42,100,w));self.assertFalse(i.read_once(B,42,100,w));self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,1)
 def test_prepare_fail_no_increment(self):
  i=Invocation(record());w=witness();self.assertFalse(i.consume(w,lambda _:False,ok,B[4]));self.assertEqual(w.commands,0);self.assertEqual(i.first,'PREPARE_PERSIST')
 def test_commit_fail_burned(self):
  i=Invocation(record());w=witness();self.assertFalse(i.consume(w,ok,lambda _:False,B[4]));self.assertEqual(w.value,18);self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,1)
 def test_crash_before_increment(self):
  i=Invocation(record());w=witness()
  def crash(_):raise RuntimeError()
  self.assertFalse(i.consume(w,crash,ok,B[4]));self.assertEqual(w.commands,0)
 def test_crash_after_increment(self):
  i=Invocation(record());w=witness()
  def crash(_):raise RuntimeError()
  self.assertFalse(i.consume(w,ok,crash,B[4]));self.assertEqual(w.value,18);self.assertFalse(i.read_once(B,42,100,w))
 def test_ambiguous_increment_never_resend(self):
  class Ambiguous(SimulatedWitness):
   def increment(self):super().increment();raise RuntimeError()
  w=Ambiguous(17,B[4]);i=Invocation(record());self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,1)
 def test_restored_ledger_denies(self):
  w=witness();w.value=18;i=Invocation(record());self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,0)
 def test_restart_consumed_always_denies(self):
  for s in ['CONSUMED','RESULT','UNKNOWN']:
   with self.assertRaises(Denied):Invocation(dataclasses.replace(record(),state=s))
 def test_restore_all_demonstrates_assumption(self):
  # Restoring BOTH synthetic witness and ledger is indistinguishable: not hardware proof.
  i=Invocation(record());self.assertTrue(i.consume(witness(),ok,ok,B[4]))
 def test_wrong_device_and_context(self):
  for device in [bytes(32),B[0]]:
   i=Invocation(record());w=witness();self.assertFalse(i.consume(w,ok,ok,device));self.assertEqual(w.commands,0)
 def test_lease_identity(self):
  for pid,creation in [(43,100),(42,101)]:
   i=Invocation(record());w=witness();self.assertTrue(i.consume(w,ok,ok,B[4]));self.assertFalse(i.read_once(B,pid,creation,w))
 def test_each_binding_mismatch(self):
  for n in range(len(B)):
   i=Invocation(record());w=witness();self.assertTrue(i.consume(w,ok,ok,B[4]));b=list(B);b[n]=bytes(32);self.assertFalse(i.read_once(tuple(b),42,100,w))
 def test_intervening_increment(self):
  i=Invocation(record());w=witness();self.assertTrue(i.consume(w,ok,ok,B[4]));w.increment();self.assertFalse(i.read_once(B,42,100,w))
 def test_reentrant_busy(self):
  i=Invocation(record());w=witness()
  def cb(_):self.assertFalse(i.consume(w,ok,ok,B[4]));return True
  self.assertTrue(i.consume(w,cb,ok,B[4]));self.assertEqual(w.commands,1)
 def test_concurrent_original_invocation(self):
  i=Invocation(record());w=witness();results=[];threads=[threading.Thread(target=lambda:results.append(i.consume(w,ok,ok,B[4]))) for _ in range(32)]
  for t in threads:t.start()
  for t in threads:t.join()
  self.assertEqual(sum(results),1);self.assertEqual(w.commands,1)
 def test_two_invocations_require_root_serialization(self):
  w=witness();a=Invocation(record());b=Invocation(record());self.assertTrue(a.consume(w,ok,ok,B[4]));self.assertFalse(b.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,1)
 def test_stop_during_prepare(self):
  i=Invocation(record());w=witness()
  def stopped(_):i.stop();return True
  self.assertFalse(i.consume(w,stopped,ok,B[4]));self.assertEqual(w.commands,0)
 def test_stop_during_commit(self):
  i=Invocation(record());w=witness()
  def stopped(_):i.stop();return True
  self.assertFalse(i.consume(w,ok,stopped,B[4]));self.assertEqual(w.commands,1);self.assertFalse(i.read_once(B,42,100,w))
 def test_stop_before(self):
  i=Invocation(record());w=witness();i.stop();self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertEqual(w.commands,0)
 def test_stop_after(self):
  i=Invocation(record());w=witness();i.consume(w,ok,ok,B[4]);i.stop();self.assertFalse(i.read_once(B,42,100,w))
 def test_unknown_close_no_retry(self):
  i=Invocation(record());self.assertFalse(i.close(False));self.assertTrue(i.retained);self.assertFalse(i.close(True));self.assertFalse(i.closed)
 def test_confirmed_close_no_double_close(self):
  i=Invocation(record());self.assertTrue(i.close(True));self.assertFalse(i.close(True))
 def test_first_error(self):
  i=Invocation(record());i.fail('FIRST');i.fail('SECOND');self.assertEqual(i.first,'FIRST')
 def test_reply_framing_context_and_replay(self):
  secret=bytes(range(32));ctx=B[0];reply=b'\x00\x20'+secret
  self.assertTrue(ActivationReplyFixture(secret,ctx).check(reply,ctx))
  for n in range(len(reply)):
   bad=bytearray(reply);bad[n]^=1;self.assertFalse(ActivationReplyFixture(secret,ctx).check(bytes(bad),ctx))
  a=ActivationReplyFixture(secret,ctx);self.assertFalse(a.check(reply,B[1]));self.assertFalse(a.check(reply,ctx))
  for bad in [reply[:-1],reply+b'x',b'',b'\x00\x1f'+secret]:self.assertFalse(ActivationReplyFixture(secret,ctx).check(bad,ctx))
 def test_first_error_serialized(self):
  i=Invocation(record());i.fail('STOP');a=i.record.encode();i.fail('CLOSE_UNKNOWN');self.assertEqual(Record.decode(a).first,ERRORS['STOP']);self.assertEqual(i.record.first,ERRORS['STOP'])
 def test_thirty_byte_credential_parameters(self):
  a=ActivationReplyFixture(bytes(range(30)),B[0]);self.assertTrue(a.check(b'\x00\x1e'+bytes(range(30)),B[0]));self.assertFalse(a.check(b'\x00\x1e'+bytes(range(30)),B[0]))
 def test_finite_failure_boundaries(self):
  for pre in ('ok','fail','raise','stop'):
   for inc in ('ok','before','after','skip','device'):
    for post in ('ok','fail','raise','stop'):
     for changed in (False,True):
      with self.subTest(pre=pre,inc=inc,post=post,changed=changed):
       i=Invocation(record())
       class W(SimulatedWitness):
        def increment(self):
         if inc=='before':self.commands+=1;raise RuntimeError()
         x=super().increment()
         if inc=='after':raise RuntimeError()
         if inc=='skip':self.value+=1;return self.value
         if inc=='device':self.identity=B[0]
         return x
       w=W(17,B[4])
       def callback(mode):
        def run(_):
         if mode=='raise':raise RuntimeError()
         if mode=='stop':i.stop()
         return mode!='fail'
        return run
       result=i.consume(w,callback(pre),callback(post),B[4])
       read=i.read_once(B,43 if changed else 42,100,w)
       expected=(pre=='ok' and inc=='ok' and post=='ok' and not changed)
       self.assertEqual(read,expected)
       if pre!='ok':self.assertEqual(w.commands,0)
       self.assertFalse(i.consume(w,ok,ok,B[4]));self.assertLessEqual(w.commands,1)
 def test_real_public_parser_rejects_malformed(self):
  tool=Path(r'C:\Strawberry\c\bin\openssl.exe');v=OfflinePublicEvidence(tool,hashlib.sha256(tool.read_bytes()).hexdigest())
  with self.assertRaises(EvidenceDenied):v.command('x509','-inform','DER','-noout',input=b'not a certificate')
 def test_tool_hash_rejected(self):
  with self.assertRaises(EvidenceDenied):OfflinePublicEvidence(r'C:\Strawberry\c\bin\openssl.exe','0'*64)
class PublicDataTests(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.folder=Path(__file__).parent/'synthetic-public-inputs';cls.folder.mkdir(exist_ok=True)
  cls.cert=cls.folder/'not-real-certificate.der';cls.key=cls.folder/'not-real-spki.der';cls.roots=cls.folder/'not-real-roots.pem'
  for path,value in [(cls.cert,b'SYNTHETIC_CERT'),(cls.key,b'SYNTHETIC_SPKI'),(cls.roots,b'SYNTHETIC_ROOT')]:
   if not path.exists():path.write_bytes(value)
   assert path.read_bytes()==value
 def analyzer(self,mode='ok'):
  def runner(args,**kwargs):
   if 'verify' in args:
    self.assertIn('-no-CAstore',args);self.assertIn('-no-CAfile',args);self.assertIn('-no-CApath',args);self.assertIn('-trusted',args)
    return subprocess.CompletedProcess(args,1 if mode=='chain' else 0,b'OK',b'')
   if '-ext' in args:return subprocess.CompletedProcess(args,0,b'CA:FALSE 2.23.133.8.1' if mode!='purpose' else b'CA:TRUE serverAuth',b'')
   if 'pkey' in args:return subprocess.CompletedProcess(args,0,b'BAD' if mode=='key' and kwargs['input']!=b'SYNTHETIC_SPKI' else b'SYNTHETIC_SPKI',b'')
   return subprocess.CompletedProcess(args,0,b'SYNTHETIC_PEM',b'')
  tool=Path(r'C:\Strawberry\c\bin\openssl.exe')
  return OfflinePublicEvidence(tool,hashlib.sha256(tool.read_bytes()).hexdigest(),runner)
 def run_data(self,v):
  return v.analyze(self.cert,self.key,self.roots,*[hashlib.sha256(x.read_bytes()).hexdigest() for x in (self.cert,self.key,self.roots)],1793050516)
 def test_mocked_path_is_never_authority(self):
  r=self.run_data(self.analyzer());self.assertEqual(r['authority'],'DENY');self.assertEqual(r['anchor_designation'],'NOT_GRANTED');self.assertEqual(r['possession'],'NOT_EXECUTED');self.assertEqual(r['revocation'],'UNKNOWN_OFFLINE')
 def test_key_mismatch(self):
  with self.assertRaises(EvidenceDenied):self.run_data(self.analyzer('key'))
 def test_wrong_purpose(self):
  with self.assertRaises(EvidenceDenied):self.run_data(self.analyzer('purpose'))
 def test_untrusted_path(self):
  with self.assertRaises(EvidenceDenied):self.run_data(self.analyzer('chain'))
 def test_wrong_input_hash(self):
  with self.assertRaises(EvidenceDenied):self.analyzer().analyze(self.cert,self.key,self.roots,'0'*64,'0'*64,'0'*64,1793050516)
if __name__=='__main__':unittest.main(verbosity=2)
