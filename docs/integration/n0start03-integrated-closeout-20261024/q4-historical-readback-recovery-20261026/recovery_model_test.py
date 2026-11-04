"""Isolated proposal model. No Windows API, files, authority or protected reads."""
import unittest
from dataclasses import dataclass, replace
@dataclass(frozen=True)
class Facts:
 host: bool=True
 boot_record: bool=True
 boot_time: bool=True
 current_context_bound: bool=True
 new_approval: bool=True
 unused: bool=True
 operator: bool=True
 primary: bool=True
 elevated: bool=True
 admin: bool=True
 impersonation_absent: bool=True
 original_id: bool=True
 raw1: bool=True
 content: bool=True
 protected_ancestors: bool=True
 no_reparse: bool=True
 stop: bool=False
 close_known: bool=True
 controlled_known: bool=True
 historical_ids_known: bool=False

def old_guard(f):
 return f.host and f.boot_record and f.boot_time

def recovery(f):
 # Deliberately not an operational validator. Proposed current-context approval
 # never rewrites the old boot and never fills historical missing file IDs.
 required=(f.host,f.current_context_bound,f.new_approval,f.unused,f.operator,
 f.primary,f.elevated,f.admin,f.impersonation_absent,f.original_id,f.raw1,
 f.content,f.protected_ancestors,f.no_reparse,f.controlled_known)
 if not all(required) or f.stop:return 'DENY'
 if not f.close_known:return 'UNKNOWN_RETAIN'
 return 'MODEL_OBSERVATION_ONLY'
class Tests(unittest.TestCase):
 def test_old_three_independent_fields(self):
  self.assertTrue(old_guard(Facts()))
  for field in ('host','boot_record','boot_time'):
   with self.subTest(field=field):self.assertFalse(old_guard(replace(Facts(),**{field:False})))
 def test_new_boot_requires_current_binding(self):
  f=Facts(boot_record=False,boot_time=False)
  self.assertEqual(recovery(f),'MODEL_OBSERVATION_ONLY')
  self.assertEqual(recovery(replace(f,current_context_bound=False)),'DENY')
 def test_required_conditions(self):
  for field in ('host','current_context_bound','new_approval','unused','operator','primary','elevated','admin','impersonation_absent','original_id','raw1','content','protected_ancestors','no_reparse','controlled_known'):
   with self.subTest(field=field):self.assertEqual(recovery(replace(Facts(),**{field:False})),'DENY')
 def test_close_uncertain(self):self.assertEqual(recovery(Facts(close_known=False)),'UNKNOWN_RETAIN')
 def test_stop(self):self.assertEqual(recovery(Facts(stop=True)),'DENY')
 def test_missing_historical_ids_not_manufactured(self):
  f=Facts();self.assertEqual(recovery(f),'MODEL_OBSERVATION_ONLY');self.assertFalse(f.historical_ids_known)
 def test_failure_not_overwritten_by_close(self):self.assertEqual(recovery(Facts(raw1=False,close_known=False)),'DENY')
if __name__=='__main__':unittest.main(verbosity=2)
