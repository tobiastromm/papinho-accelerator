import copy,json,unittest
from pathlib import Path
from review_context import review,Denied
BASE=json.loads(Path(__file__).with_name("OWNER-MANUAL-OUTPUT.json").read_text())
class ContextTests(unittest.TestCase):
    def test_actual_data(self):
        r=review(BASE)
        self.assertEqual((r["actual_parent_pid"],r["actual_auxiliary_pid"]),(20976,13944))
        self.assertEqual(r["admin_handles_observed"],982)
        self.assertEqual(r["outside_current_context_observed"],[7200,17204,20144])
        self.assertEqual(r["supplied_seed_binding"],"DENY_CROSS_CONSOLE_SEED")
        self.assertEqual(r["admission"],"DENY")
    def test_old_expected_parent_rejected(self):
        with self.assertRaisesRegex(Denied,"EXPECTED_PARENT_MISMATCH"):review(BASE,17204,134376272948693901)
    def test_exact_current_data_identity(self):
        self.assertEqual(review(BASE,20976,134376368499650721)["admission"],"DENY")
    def test_wrong_expected_creation(self):
        with self.assertRaisesRegex(Denied,"EXPECTED_CREATION_MISMATCH"):review(BASE,20976,1)
    def test_unrelated_context_resources_not_added(self):
        d=copy.deepcopy(BASE)
        for n in d["nodes"]:
            if n["pid"] in [17204,20144,7200]:n["handles"]=999999
        self.assertEqual(review(d)["admin_handles_observed"],982)
    def test_input_not_mutated(self):
        d=copy.deepcopy(BASE);review(d);self.assertEqual(d,BASE)
    def test_negative_matrix(self):
        cases=[
          ("retained",None,"retained",True,"FAILED_OR_RETAINED"),
          ("unclosed",None,"controlled_original_refs_remaining",1,"UNCLOSED_REFERENCE"),
          ("client extra",None,"client_count",3,"CLIENT_SET"),
          ("bad client",None,"clients",[20952,17204],"CLIENT_SET"),
          ("duplicate client",None,"clients",[20952,20952],"CLIENT_SET"),
          ("observer original",20952,"original_handle_identity",False,"IDENTITY_LIFETIME"),
          ("parent lifetime",20976,"live_stable",False,"IDENTITY_LIFETIME"),
          ("parent creation",20976,"creation_after",1,"IDENTITY_LIFETIME"),
          ("parent missing",20952,"parent_metadata",999,"MISSING_ORIGINAL_PROCESS"),
          ("class width",20952,"class49_bytes",4,"CLASS49_QUERY"),
          ("class denied",20952,"class49_status",-1,"CLASS49_QUERY"),
          ("class changed",20976,"class49_after_raw",20145,"CLASS49_CHANGED"),
          ("no second query",20952,"class49_after_queried",False,"CLASS49_CHANGED"),
          ("no elevation",20976,"elevated",False,"PROCESS_TOKEN_UNKNOWN"),
          ("wrong SID",13944,"token_user","other","TOKEN_SESSION_MISMATCH"),
          ("wrong session",13944,"session",2,"TOKEN_SESSION_MISMATCH"),
          ("missing file",13944,"file_identity_known",False,"FILE_EVIDENCE"),
          ("missing metric",13944,"metrics_known",False,"METRIC_UNKNOWN"),
          ("budget1025",20976,"handles",755,"ADMIN_HANDLES"),
          ("reserve125",None,"own_peak",125,"OWN_OBSERVED_RESERVE"),
        ]
        for label,pid,key,value,error in cases:
            with self.subTest(label=label):
                d=copy.deepcopy(BASE);target=d if pid is None else next(n for n in d["nodes"] if n["pid"]==pid);target[key]=value
                with self.assertRaisesRegex(Denied,error):review(d)
    def test_duplicate_process_identity_rejected(self):
        d=copy.deepcopy(BASE);d["nodes"].append(copy.deepcopy(d["nodes"][0]))
        with self.assertRaisesRegex(Denied,"DUPLICATE_PID"):review(d)
    def test_contexts_must_converge(self):
        d=copy.deepcopy(BASE);n=next(n for n in d["nodes"] if n["pid"]==20976);n["class49_raw"]=n["class49_after_raw"]=20145
        with self.assertRaisesRegex(Denied,"CONSOLE_CONTEXT_MISMATCH"):review(d)
if __name__=="__main__":unittest.main(verbosity=2)
