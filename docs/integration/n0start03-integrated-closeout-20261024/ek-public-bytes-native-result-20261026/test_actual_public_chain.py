from pathlib import Path
import importlib.util,json,hashlib,unittest
p=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('offline_corrected',p/'offline_public_evidence.CORRECTED.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
sha=lambda x:hashlib.sha256(Path(x).read_bytes()).hexdigest()
class ActualPublicTests(unittest.TestCase):
 def run_analysis(self,cert='ek-certificate.PUBLIC.der',key='ek-public.SPKI.der',intermediate=True):
  receipt=json.loads((p/'OWNER-RESULT.PARSED.json').read_text());time=receipt['context']['process_creation_filetime']//10000000-11644473600
  v=module.OfflinePublicEvidence(r'C:\Strawberry\c\bin\openssl.exe',sha(r'C:\Strawberry\c\bin\openssl.exe'))
  return v.analyze(p/cert,p/key,p/'intel-root.DOWNLOADED.pem',sha(p/cert),sha(p/key),sha(p/'intel-root.DOWNLOADED.pem'),time,p/'intel-intermediate.DOWNLOADED.pem' if intermediate else None)
 def test_real_chain_and_key(self):
  r=self.run_analysis();self.assertEqual(r['public_key_match'],'PASS_DATA_ONLY');self.assertEqual(r['authority'],'DENY');self.assertEqual(r['anchor_designation'],'NOT_GRANTED');self.assertEqual(r['possession'],'NOT_EXECUTED');(p/'CORRECTED-HELPER-RESULT.json').write_text(json.dumps(r,indent=2)+'\n')
 def test_wrong_public_key(self):
  with self.assertRaises(module.EvidenceDenied):self.run_analysis(key='root-public.WRONG-EK.SPKI.der')
 def test_signature_corruption(self):
  b=bytearray((p/'ek-certificate.PUBLIC.der').read_bytes());b[-1]^=1;(p/'certificate.SIGNATURE-CORRUPTED.fixture.der').write_bytes(b)
  with self.assertRaises(module.EvidenceDenied):self.run_analysis(cert='certificate.SIGNATURE-CORRUPTED.fixture.der')
 def test_missing_intermediate(self):
  with self.assertRaises(module.EvidenceDenied):self.run_analysis(intermediate=False)
if __name__=='__main__':unittest.main(verbosity=2)
