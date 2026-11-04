import unittest,hashlib
from q4_component_candidate import Record,Denied,encode,decode
class ComponentTests(unittest.TestCase):
 def setUp(self):self.r=Record(tuple(hashlib.sha256(bytes([n+1])).digest() for n in range(9)),17,18,42,100)
 def test_roundtrip(self):self.assertEqual(decode(encode(self.r)),self.r)
 def test_each_mutation(self):
  raw=encode(self.r)
  for n in range(512):
   bad=bytearray(raw);bad[n]^=1
   with self.assertRaises(Denied):decode(bytes(bad))
 def test_all_truncations(self):
  raw=encode(self.r)
  for n in range(512):
   with self.assertRaises(Denied):decode(raw[:n])
 def test_no_old_format_adoption(self):
  with self.assertRaises(Denied):decode(self.r.encode()+bytes(140))
if __name__=='__main__':unittest.main(verbosity=2)
