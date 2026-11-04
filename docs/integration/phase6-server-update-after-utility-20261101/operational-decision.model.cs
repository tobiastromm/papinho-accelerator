using System;
using System.Collections.Generic;
using System.Security.AccessControl;
using System.Security.Principal;
public static class OperationalDecisionModel {
 sealed class IO : IFirstStartDocumentsIO {
  public int Calls,FailAt,Creates,Writes,Flushes,StopAt,Stops;public bool Uncertain,Closed,Existing;
  public List<int> Destinations=new List<int>();
  void Call(string step){Calls++;if(Calls==FailAt)throw new Exception(step);}
  public bool Stop(){return ++Stops==StopAt;}
  public void Check(){Call("SOURCE_SUBSTITUTED");}
  public void Create(int i){Call("CREATE_FAILED");if(Existing)throw new Exception("EXISTING_DENY");Destinations.Add(i);Creates++;}
  public void Write(int i){Call("PARTIAL_WRITE");Writes++;}
  public void Flush(int i){Call("FLUSH_UNKNOWN");Flushes++;}
  public void Verify(int i){Call("IDENTITY_HASH_SD_DIVERGENT");}
  public bool Close(){Closed=true;return !Uncertain;}
 }
 static int n;static void C(bool b){n++;if(!b)throw new Exception("OP_DECISION_MODEL_"+n);}
 public static string Run(){
  IO good=new IO();var steps=new FirstStartDocumentSteps();C(steps.RunOperationalDecision(good)=="NONE");C(good.Creates==1&&good.Writes==1&&good.Flushes==1&&good.Destinations.Count==1&&good.Destinations[0]==0&&steps.CloseKnown&&steps.Consumed);C(steps.RunOperationalDecision(good)=="REPLAY_DENY"&&good.Creates==1);
  for(int i=1;i<=7;i++){IO f=new IO();f.FailAt=i;var s=new FirstStartDocumentSteps();C(s.RunOperationalDecision(f)!="NONE"&&f.Closed&&s.CloseKnown);C(f.Calls==i&&f.Creates<=1);C(s.Consumed==(i>=2));}
  for(int i=1;i<=good.Stops;i++){IO f=new IO();f.StopAt=i;var s=new FirstStartDocumentSteps();C(s.RunOperationalDecision(f)=="STOP"&&f.Closed&&s.CloseKnown&&f.Creates<=1);}
  IO occupied=new IO();occupied.Existing=true;var o=new FirstStartDocumentSteps();C(o.RunOperationalDecision(occupied)=="EXISTING_DENY"&&occupied.Creates==0&&occupied.Writes==0&&o.Consumed);
  IO unknown=new IO();unknown.Uncertain=true;var u=new FirstStartDocumentSteps();C(u.RunOperationalDecision(unknown)=="CLOSE_UNKNOWN"&&u.Consumed&&!u.CloseKnown&&unknown.Creates==1);C(u.RunOperationalDecision(unknown)=="REPLAY_DENY"&&unknown.Creates==1);
  C(FirstStartDocumentsNative.ExpectedOriginalRequest(417614100U,1310720U,120518U));C(!FirstStartDocumentsNative.ExpectedOriginalRequest(417614101U,1310720U,120518U));C(!FirstStartDocumentsNative.ExpectedOriginalRequest(417614100U,1310721U,120518U));C(!FirstStartDocumentsNative.ExpectedOriginalRequest(417614100U,1310720U,120520U));
  byte[] sd=FirstStartDocumentsNative.CreationDescriptor();C(FirstStartDocumentsNative.ProtectedDocument(sd));var d=new RawSecurityDescriptor(sd,0);d.Owner=new SecurityIdentifier("S-1-5-32-544");byte[] wrong=new byte[d.BinaryLength];d.GetBinaryForm(wrong,0);C(!FirstStartDocumentsNative.ProtectedDocument(wrong));
  return "PASS OPERATIONAL_DECISION_DATA checks="+n+" SINGLE_NEW_LEAF_ONLY EXISTING_REQUEST_ID_BOUND NO_PAIR_RECREATE NO_PREPARATION_PROMOTION FAULTS_STOP_REPLAY_UNKNOWN NATIVE_IO_MODELED NO_SCM_ENDPOINT_START";
 }
}
