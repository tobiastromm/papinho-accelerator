"""Protocol feasibility only: simulated nonrollback counter, not TPM writes."""
import unittest,itertools

def assess(ledger,counter,identity=True,available=True,live=False,spent=False,close=True):
 if not identity or not available or ledger is None or counter is None:return 'DENY'
 if not close:return 'UNKNOWN_RETAIN'
 if spent and not live:return 'SPENT_DENY'
 if ledger!=counter:return 'ROLLBACK_OR_INCOMPLETE_DENY'
 return 'MODEL_MATCH_NOT_AUTHORITY'

def schedules(a,b):
 if not a:yield b;return
 if not b:yield a;return
 for s in schedules(a[1:],b):yield a[:1]+s
 for s in schedules(a,b[1:]):yield b[:1]+s

class Tests(unittest.TestCase):
 def test_restore_local_only(self):self.assertEqual(assess(0,1),'ROLLBACK_OR_INCOMPLETE_DENY')
 def test_unchanged_match_no_authority(self):self.assertEqual(assess(1,1),'MODEL_MATCH_NOT_AUTHORITY')
 def test_loss(self):
  for l,c in ((None,1),(1,None),(None,None)):self.assertEqual(assess(l,c),'DENY')
 def test_wrong_hardware_identity(self):self.assertEqual(assess(1,1,identity=False),'DENY')
 def test_unavailable(self):self.assertEqual(assess(1,1,available=False),'DENY')
 def test_restart_spent(self):self.assertEqual(assess(1,1,spent=True),'SPENT_DENY')
 def test_unknown_close(self):self.assertEqual(assess(1,1,close=False),'UNKNOWN_RETAIN')
 def test_crash_before_counter(self):self.assertEqual(assess(None,0),'DENY')
 def test_crash_after_counter(self):self.assertEqual(assess(0,1),'ROLLBACK_OR_INCOMPLETE_DENY')
 def test_crash_after_local_commit(self):self.assertEqual(assess(1,1,spent=True),'SPENT_DENY')
 def test_witness_itself_restored_limitation(self):
  self.assertEqual(assess(0,0),'MODEL_MATCH_NOT_AUTHORITY') # Cannot detect whole witness rollback.
 def test_clear_or_delete_no_reinitialize(self):self.assertEqual(assess(1,None),'DENY')
 def test_all_two_consumer_interleavings(self):
  n=0
  for seq in schedules([(0,'pre'),(0,'inc'),(0,'post')],[(1,'pre'),(1,'inc'),(1,'post')]):
   counter=0;ok=[False,False];admitted=[False,False]
   for actor,step in seq:
    if step=='pre':ok[actor]=counter==0
    elif step=='inc' and ok[actor]:counter+=1
    elif step=='post':admitted[actor]=ok[actor] and counter==1
   self.assertLessEqual(sum(admitted),1,seq);n+=1
  self.assertEqual(n,20)
if __name__=='__main__':unittest.main(verbosity=2)
