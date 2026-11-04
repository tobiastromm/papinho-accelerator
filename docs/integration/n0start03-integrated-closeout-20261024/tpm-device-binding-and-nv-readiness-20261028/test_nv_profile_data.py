import unittest,struct,hashlib
from nv_profile_data import public_profile,next_counter,readiness,INDEX,ATTR,WRITTEN
class Tests(unittest.TestCase):
 def area(self,init=False):return struct.pack(">IHIHH",INDEX,11,ATTR|(WRITTEN if init else 0),0,8)
 def name(self,a):return b"\x00\x0b"+hashlib.sha256(a).digest()
 def test_profiles_and_Name_transition(self):
  a,b=self.area(),self.area(True)
  self.assertNotEqual(self.name(a),self.name(b))
  for init,v in [(False,a),(True,b)]:self.assertEqual(public_profile(v,self.name(v),init)["authority"],"DENY")
  with self.assertRaises(ValueError):public_profile(b,self.name(a),True)
 def test_all_public_byte_mutations_even_rehashed(self):
  for initialized in (False,True):
   a=self.area(initialized)
   for i in range(14):
    for bit in range(8):
     b=bytearray(a);b[i]^=1<<bit;b=bytes(b)
     with self.subTest(i=i,bit=bit,initialized=initialized):
      with self.assertRaises(ValueError):public_profile(b,self.name(b),initialized)
 def test_all_name_byte_mutations(self):
  a=self.area();n=self.name(a)
  for i in range(34):
   b=bytearray(n);b[i]^=1
   with self.assertRaises(ValueError):public_profile(a,bytes(b),False)
 def test_truncated_and_extended(self):
  a=self.area();n=self.name(a)
  for length in range(14):
   with self.assertRaises(ValueError):public_profile(a[:length],n,False)
  for length in range(34):
   with self.assertRaises(ValueError):public_profile(a,n[:length],False)
  with self.assertRaises(ValueError):public_profile(a+b"0",n,False)
 def test_wrong_written_phase(self):
  for init in (False,True):
   a=self.area(init)
   with self.assertRaises(ValueError):public_profile(a,self.name(a),not init)
 def test_counter_no_assumed_initial_or_rebase(self):
  for baseline in (0,1,42,2**64-2):self.assertEqual(next_counter(baseline,baseline+1),baseline+1)
  for a,b in [(42,1),(42,42),(42,44),(2**64-1,0),(-1,0),(True,2),(0,True)]:
   with self.assertRaises(ValueError):next_counter(a,b)
 def test_absent_and_unknown_fail_closed(self):
  f=readiness({});self.assertFalse(f["ready"])
  for k in f["missing"]:
   good={v:True for v in f["missing"]}
   for unknown in (None,False,"UNKNOWN",1):
    good[k]=unknown;self.assertFalse(readiness(good)["ready"])
 def test_all_true_data_still_has_no_authority(self):
  f={k:True for k in readiness({})["missing"]}
  self.assertTrue(readiness(f)["ready"]);self.assertEqual(readiness(f)["authority"],"DENY")
class ReadTests(unittest.TestCase):
 def setUp(self):
  import nv_profile_data as n
  self.n=n;self.args=(b"a"*32,b"b"*32,b"\x00\x0b"+b"n"*32,b"c"*32,b"t"*32,0x02000001)
 def reply(self):
  import hmac
  pars=b"\x00\x08"+struct.pack(">Q",42);nonce=b"r"*32
  rp=hashlib.sha256(struct.pack(">II",0,0x14e)+pars).digest()
  mac=hmac.new(b"a"*32+b"b"*32,rp+nonce+b"c"*32+b"\x00",hashlib.sha256).digest()
  return struct.pack(">HIII",0x8002,93,0,10)+pars+b"\x00\x20"+nonce+b"\x00\x00\x20"+mac
 def test_exact_read_wire(self):
  packet=self.n.read_command(*self.args)
  self.assertEqual(len(packet),99)
  self.assertEqual(struct.unpack(">HIIIII",packet[:22]),(0x8002,99,0x14e,INDEX,INDEX,73))
  self.assertEqual(packet[-4:],b"\x00\x08\x00\x00")
  self.assertEqual(self.n.read_response(self.reply(),*self.args),42)
 def test_all_response_bits_and_truncation(self):
  packet=self.reply()
  for i in range(len(packet)):
   for bit in range(8):
    bad=bytearray(packet);bad[i]^=1<<bit
    with self.assertRaises(ValueError):self.n.read_response(bytes(bad),*self.args)
  for i in range(len(packet)):
   with self.assertRaises(ValueError):self.n.read_response(packet[:i],*self.args)
 def test_replay_nonce_wrong_auth_and_session(self):
  for pos in (0,1,3):
   args=list(self.args);args[pos]=b"x"*32
   with self.assertRaises(ValueError):self.n.read_response(self.reply(),*args)
  for handle in (0x40000001,0x102000001,-1,True):
   args=list(self.args);args[-1]=handle
   with self.assertRaises(ValueError):self.n.read_command(*args)
 def test_Name_changes_request_authentication(self):
  args=list(self.args);args[2]=b"\x00\x0b"+b"m"*32
  self.assertNotEqual(self.n.read_command(*args),self.n.read_command(*self.args))
if __name__=="__main__":unittest.main(verbosity=2)
