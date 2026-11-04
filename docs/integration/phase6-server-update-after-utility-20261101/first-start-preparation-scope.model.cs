using System;
using System.Collections.Generic;
public static class FirstStartPreparationScopeModel {
 sealed class IO : IFirstStartDocumentsIO {
  public string Failure,At;public bool Uncertain,Closed,Stopped;public int Created,Writes,Flushes;
  public readonly HashSet<int> Existing=new HashSet<int>();
  public bool Stop(){return Stopped;}
  void CheckAt(string step){if(step==At)throw new Exception(Failure);}
  public void Check(){CheckAt("check");}
  public void Create(int i){CheckAt("create"+i);if(!Existing.Add(i))throw new Exception("EXISTING_NO_ADOPTION");Created++;}
  public void Write(int i){CheckAt("write"+i);Writes++;}
  public void Flush(int i){CheckAt("flush"+i);Flushes++;}
  public void Verify(int i){CheckAt("verify"+i);}
  public bool Close(){Closed=true;return !Uncertain;}
 }
 static int checks;
 static void C(bool ok){checks++;if(!ok)throw new Exception("PREPARATION_MODEL_"+checks);}
 public static string Run(){
  IO valid=new IO();var one=new FirstStartDocumentSteps();C(one.Run(valid)=="NONE");C(valid.Created==2&&valid.Writes==2&&valid.Flushes==2&&one.CloseKnown&&one.Consumed);
  C(one.Run(valid)=="REPLAY_DENY"&&valid.Created==2);
  foreach(int existing in new int[]{0,1}){IO io=new IO();io.Existing.Add(existing);var s=new FirstStartDocumentSteps();C(s.Run(io)=="EXISTING_NO_ADOPTION");C(io.Created==existing&&s.Consumed&&s.CloseKnown&&io.Closed);C(io.Existing.Contains(existing));}
  string[] reasons={"IDENTITY_CHANGED","PROTECTION_CHANGED","CONTENT_HASH_CHANGED","REPARSE","SOURCE_ALIAS","PARTIAL_CREATE_UNKNOWN","PARTIAL_WRITE","FLUSH_UNKNOWN","READBACK_CHANGED"};
  string[] steps={"check","check","check","check","check","create1","write1","flush1","verify1"};
  for(int i=0;i<reasons.Length;i++){IO io=new IO();io.Failure=reasons[i];io.At=steps[i];var s=new FirstStartDocumentSteps();C(s.Run(io)==reasons[i]&&s.CloseKnown&&io.Closed);C(io.Created<2||io.Existing.Count==2);C(s.Consumed==(i>=5));if(i>=5)C(io.Existing.Contains(0));}
  IO stop=new IO();stop.Stopped=true;var stopped=new FirstStartDocumentSteps();C(stopped.Run(stop)=="STOP"&&!stopped.Consumed&&stop.Created==0&&stop.Closed);
  IO retained=new IO();retained.At="create1";retained.Failure="PARTIAL_CREATE_UNKNOWN";retained.Uncertain=true;var uncertain=new FirstStartDocumentSteps();C(uncertain.Run(retained)=="PARTIAL_CREATE_UNKNOWN"&&!uncertain.CloseKnown&&uncertain.Consumed);C(retained.Existing.Contains(0)&&retained.Created==1);C(uncertain.Run(retained)=="REPLAY_DENY"&&retained.Created==1);
  return "PASS PREPARATION_SCOPE_MODEL checks="+checks+" IO_FAULTS_INJECTED EXISTING_NO_ADOPTION FIRST_CAUSE_STOP_REPLAY_RETENTION_UNKNOWN NO_NATIVE_IO_NO_SCM_NO_ENDPOINT_NO_START";
 }
}
