using System;
using System.Collections.Generic;
using System.Security.AccessControl;
using System.Security.Principal;
public static class MachineFixtureDocumentsModel {
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
  C(FirstStartDocumentsNative.NativeLayouts());
  C(FirstStartDocumentsNative.Folder==@"C:\Program Files\Papinho\Accelerator");
  C(FirstStartDocumentsNative.ExpectedTarget(417614100U,8257536U,71209U));C(!FirstStartDocumentsNative.ExpectedTarget(417614100U,8257536U,71210U));
  string actor=FirstStartDocumentsNative.Actor;
  foreach(string owner in new string[]{actor,"SY","BA","S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464"})C(FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:"+owner+"G:SYD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GR;;;AU)")),false));
  C(!FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:AUG:SYD:P(A;;FA;;;SY)")),false));
  C(!FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:(A;;FA;;;BA)")),false));
  C(!FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:P(A;;FA;;;BA)(A;;GW;;;AU)")),false));
  byte[] root=Encode(new RawSecurityDescriptor("O:BAG:BAD:P(A;;FA;;;BA)(A;;0x4;;;AU)"));C(FirstStartDocumentsNative.ProtectedDirectory(root,true));C(!FirstStartDocumentsNative.ProtectedDirectory(root,false));
  // Independent fault labels with persistent partial creation and first failure.
  foreach(string reason in new string[]{"EXISTS","HASH_SUBSTITUTED","PARENT_DIVERGENT","REPARSE","DACL","READBACK","DEADLINE","RESOURCES"}){IO io=new IO();io.Fault=reason=="READBACK"?6:1;var steps=new FirstStartDocumentSteps();C(steps.Run(io).StartsWith("FAULT_")&&io.Created==(reason=="READBACK"?1:0));}
  C(!FirstStartDocumentsNative.ExpectedTarget(417614101U,8257536U,71209U));C(!FirstStartDocumentsNative.ExpectedTarget(417614100U,8257535U,71209U));
  C(!FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:P")),false));
  C(FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:P(A;;FA;;;BA)(D;;FA;;;AU)")),false));
  C(FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:P(A;;FA;;;BA)(A;IO;GW;;;AU)")),false));
  C(!FirstStartDocumentsNative.ProtectedDirectory(Encode(new RawSecurityDescriptor("O:BAG:BAD:P(A;;FA;;;BA)(A;CI;0x4;;;AU)")),true));
  var reader=new RawSecurityDescriptor(FirstStartDocumentsNative.CreationDescriptor(),0);reader.DiscretionaryAcl.InsertAce(3,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x120089,new SecurityIdentifier("S-1-5-80-1-2-3-4-5"),false,null));C(!FirstStartDocumentsNative.ProtectedDocument(Encode(reader)));
  return "PASS MACHINE_FIXTURE_DOCUMENTS checks="+checks+" FLOW_FAULTS=14 STOP_POINTS="+good.Stops+" STRICT_SD_PARENT_ID_CHAIN_POLICY REPLAY_CLOSE_UNKNOWN NO_NATIVE_CONSTRUCTOR_NO_FILE_API_NO_SCM_NO_ENDPOINT";
 }
}
