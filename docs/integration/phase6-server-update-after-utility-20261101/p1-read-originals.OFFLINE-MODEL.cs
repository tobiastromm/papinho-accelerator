using System;
using System.Collections.Generic;
using System.Security.AccessControl;
using System.Security.Principal;
public static class P1ReadDocumentsModel {
 sealed class IO : IFirstStartDocumentsIO {
  public int Fault,Calls,StopAt,Stops,Created;public bool Closed,Uncertain;
  public List<string> Trace=new List<string>();
  void Call(string s){Trace.Add(s);Calls++;if(Fault==Calls)throw new Exception("FAULT_"+s);}
  public bool Stop(){Stops++;return StopAt==Stops;}
  public void Check(){Call("check");}public void Create(int i){Call("create"+i);Created++;}
  public void Write(int i){Call("write"+i);}public void Flush(int i){Call("flush"+i);}
  public void Verify(int i){Call("verify"+i);}public bool Close(){Closed=true;return !Uncertain;}
 }
 static int checks;
 static void C(bool ok){checks++;if(!ok)throw new Exception("MODEL_CHECK_"+checks);}
 static byte[] Encode(RawSecurityDescriptor d){byte[] b=new byte[d.BinaryLength];d.GetBinaryForm(b,0);return b;}
 public static string Run() {
  IO good=new IO();var run=new FirstStartDocumentSteps();C(run.Run(good)=="NONE"&&run.Consumed&&run.CloseKnown&&good.Created==2);
  C(String.Join(",",good.Trace.ToArray())=="check,create0,verify0,write0,flush0,verify0,check,check,create1,verify1,write1,flush1,verify1,check");
  C(run.Run(good)=="REPLAY_DENY"&&good.Created==2);
  for(int i=1;i<=14;i++){IO f=new IO();f.Fault=i;var s=new FirstStartDocumentSteps();C(s.Run(f).StartsWith("FAULT_")&&f.Closed&&s.CloseKnown);C(f.Calls==i);C(s.Consumed==(i>=2));}
  for(int i=1;i<=good.Stops;i++){IO f=new IO();f.StopAt=i;var s=new FirstStartDocumentSteps();C(s.Run(f)=="STOP"&&f.Closed&&s.CloseKnown);C(f.Stops==i);}
  IO uncertain=new IO();uncertain.Uncertain=true;var u=new FirstStartDocumentSteps();C(u.Run(uncertain)=="CLOSE_UNKNOWN"&&!u.CloseKnown&&u.Consumed);
  IO first=new IO();first.Fault=5;first.Uncertain=true;var v=new FirstStartDocumentSteps();C(v.Run(first)=="FAULT_flush0"&&!v.CloseKnown);
  byte[] exact=FirstStartDocumentsNative.CreationDescriptor();C(FirstStartDocumentsNative.ProtectedDocument(exact));
  var sd=new RawSecurityDescriptor(exact,0);sd.Owner=new SecurityIdentifier("S-1-5-32-544");C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);sd.SetFlags(sd.ControlFlags&~ControlFlags.DiscretionaryAclProtected);C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.RemoveAce(0);C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.InsertAce(0,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x1f01ff,new SecurityIdentifier("S-1-5-11"),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);var ace=(CommonAce)sd.DiscretionaryAcl[0];ace.AceFlags=AceFlags.Inherited;C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  C(!FirstStartDocumentsNative.ProtectedDocument(new byte[1]));
  // Accepted L0-L6 strict SID/mask profile, no generic read broadening.
  for(int j=0;j<4;j++){
   sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.RemoveAce(j);C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  }
  foreach(int mask in new int[]{0,0x120088,0x12008b,0x1200a9,0x1f01ff,0x120189,0x160089}){
   sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.RemoveAce(3);sd.DiscretionaryAcl.InsertAce(3,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,mask,new SecurityIdentifier(FirstStartDocumentsNative.RuntimeSid),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  }
  foreach(string badSid in new string[]{"S-1-5-11","S-1-1-0","S-1-5-32-545","S-1-5-80-1-2-3-4-5"}){
   sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.RemoveAce(3);sd.DiscretionaryAcl.InsertAce(3,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x120089,new SecurityIdentifier(badSid),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  }
  sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.InsertAce(4,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x120089,new SecurityIdentifier("S-1-5-11"),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);((CommonAce)sd.DiscretionaryAcl[3]).AceFlags=AceFlags.Inherited;C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  sd=new RawSecurityDescriptor(exact,0);sd.DiscretionaryAcl.RemoveAce(3);sd.DiscretionaryAcl.InsertAce(3,new CommonAce(AceFlags.None,AceQualifier.AccessDenied,0x120089,new SecurityIdentifier(FirstStartDocumentsNative.RuntimeSid),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(sd)));
  return "PASS P1_READ_DOCUMENTS checks="+checks+" FLOW_FAULTS=14 STOP_POINTS="+good.Stops+" STRICT_SD REPLAY_CLOSE_UNKNOWN NO_NATIVE_CONSTRUCTOR_NO_FILE_API_NO_SCM_NO_ENDPOINT";
 }
}
