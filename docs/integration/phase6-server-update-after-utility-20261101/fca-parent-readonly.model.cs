using System;
using System.Security.AccessControl;
public static class FcaParentReadModel {
 sealed class IO : IFcaParentReadIO {
  public int Calls,Fail,Stops,StopAt,Opened,Inspected,Closed;public bool Unknown;
  void Next(){if(++Calls==Fail)throw new Exception("INJECTED_FIRST");}
  public bool Stop(){return ++Stops==StopAt;}
  public string Resolve(){Next();return @"C:\ProgramData\Papinho";}
  public void Open(int index,string parent){Next();Opened++;}
  public string Inspect(int index,bool again){Next();Inspected++;return "MOCK index="+index+" revalidation="+again;}
  public bool Close(){Closed++;return !Unknown;}
 }
 static int checks;static void C(bool ok){checks++;if(!ok)throw new Exception("FCA_PARENT_MODEL_"+checks);}
 static byte[] Sd(string text){var sd=new RawSecurityDescriptor(text);byte[] b=new byte[sd.BinaryLength];sd.GetBinaryForm(b,0);return b;}
 public static string Run(byte[] historic) {
  IO io=new IO();var s=new FcaParentReadSteps();s.Run(io);C(s.First=="NONE"&&s.Consumed&&s.Complete&&s.CloseKnown);C(io.Opened==3&&io.Inspected==6&&io.Closed==1&&io.Calls==10);s.Run(io);C(s.First=="REPLAY_DENY"&&io.Calls==10&&io.Closed==1);
  for(int i=1;i<=10;i++){var f=new IO();f.Fail=i;var t=new FcaParentReadSteps();t.Run(f);C(t.First=="INJECTED_FIRST"&&!t.Complete&&t.Consumed&&t.CloseKnown&&f.Calls==i&&f.Closed==1);}
  for(int i=1;i<=7;i++){var f=new IO();f.StopAt=i;var t=new FcaParentReadSteps();t.Run(f);C(t.First=="STOP"&&!t.Complete&&t.CloseKnown&&f.Closed==1&&t.Consumed==(i>1));}
  var unknown=new IO();unknown.Unknown=true;var u=new FcaParentReadSteps();u.Run(unknown);C(u.First=="CLOSE_UNKNOWN"&&!u.CloseKnown&&u.Complete&&unknown.Closed==1);
  foreach(string bad in new string[]{null,"",@"C:\ProgramData\..\Papinho",@"C:\ProgramData\Papinho\",@"C:\ProgramData\x:y\Papinho",@"\\server\Papinho",@"C:\a\\Papinho",@"c:\ProgramData\Papinho"}){bool denied=false;try{FcaParentReadNative.Components(bad);}catch{denied=true;}C(denied);}
  C(FcaParentReadNative.Components(@"C:\ProgramData\Papinho").Length==2);C(FcaParentReadNative.Layouts());
  int ace;uint mask;C(!FcaParentReadNative.Protected(historic,false,out ace,out mask)&&ace==4&&mask==0x116);
  // Same policy function in the existing materializer, compiled but never instantiated.
  C(!FirstStartDocumentsNative.ProtectedDirectory(historic,false));
  string prefix="O:SYG:SYD:P(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)";
  foreach(string extra in new string[]{"","(A;OICI;FR;;;BU)","(A;OICI;FW;;;BU)","(A;OICIIO;GA;;;CO)","(D;;GA;;;BU)","(A;;LC;;;AU)","(A;CI;LC;;;AU)","(A;;WD;;;BU)","(A;;DC;;;BU)"})foreach(bool anchor in new bool[]{false,true}){byte[] b=Sd(prefix+extra);C(FcaParentReadNative.Protected(b,anchor,out ace,out mask)==FirstStartDocumentsNative.ProtectedDirectory(b,anchor));}
  C(!FcaParentReadNative.Protected(Sd("O:SYG:SYD:(A;;FA;;;SY)"),false,out ace,out mask));
  C(!FcaParentReadNative.Protected(new byte[4],false,out ace,out mask));
  int imports=0;foreach(var m in typeof(FcaParentReadNative).GetMethods(System.Reflection.BindingFlags.Static|System.Reflection.BindingFlags.NonPublic)){
   var attrs=m.GetCustomAttributes(typeof(System.Runtime.InteropServices.DllImportAttribute),false);if(attrs.Length==0)continue;imports++;
   var a=(System.Runtime.InteropServices.DllImportAttribute)attrs[0];C(a.ExactSpelling&&Array.IndexOf(new string[]{"SHGetKnownFolderPath","CoTaskMemFree","QueryDosDeviceW","NtCreateFile","NtQuerySecurityObject","GetFileInformationByHandle","GetHandleInformation","GetFileType","CloseHandle"},m.Name)>=0);
  }C(imports==9);
  return "PASS FCA_PARENT_READ_MODEL checks="+checks+"; NATIVE_IO_MOCKED_NO_NATIVE_CONSTRUCTOR_NO_REAL_QUERY; HISTORIC_PROGRAMDATA_POLICY_REJECT_ACE4_MASK0x116; NO_AUTHORITY";
 }
}
