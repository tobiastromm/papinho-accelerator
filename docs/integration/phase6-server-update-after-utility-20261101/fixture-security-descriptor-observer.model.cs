using System;
using System.Collections.Generic;
using System.Security.AccessControl;
public static class FixtureSdReadModel {
 sealed class IO : IFixtureSdReadIO {
  public int At,Fail,Stops,StopAt,Providers,Opens,Reads,Closes;public bool Unknown,WrongLength,Malformed;
  int Next(){return ++At==Fail?unchecked((int)0x80090010):0;}
  public bool Stop(){return ++Stops==StopAt;}
  public int Provider(){Providers++;return Next();}public int Open(){Opens++;return Next();}
  public int Read(byte[] output,out int n){Reads++;int status=Next();byte[] sd=Sd("D:P",true);n=WrongLength?1025:Malformed?1:sd.Length;if(!WrongLength&&!Malformed)Buffer.BlockCopy(sd,0,output,0,sd.Length);return status;}
  public bool Close(){Closes++;return !Unknown;}
 }
 static int checks;static void C(bool ok){checks++;if(!ok)throw new Exception("SD_MODEL_"+checks);}
 static byte[] Sd(string flags,bool generic){string mask=generic?"GA":"0x000f003f";var d=new RawSecurityDescriptor("O:"+FixtureSdReadNative.Actor+"G:SY"+flags+"(D;;"+mask+";;;"+FixtureSdReadNative.Runtime+")(A;;"+mask+";;;"+FixtureSdReadNative.Actor+")(A;;"+mask+";;;SY)");byte[] b=new byte[d.BinaryLength];d.GetBinaryForm(b,0);return b;}
 public static string Run(){
  IO good=new IO();var s=new FixtureSdReadSteps();s.Run(good);C(s.Complete&&s.CloseKnown&&s.Consumed&&s.First==0);C(good.Providers==1&&good.Opens==1&&good.Reads==1&&good.Closes==1);
  C(String.Join("\n",s.Evidence.ToArray()).Contains("PROJECTION=MATCH"));s.Run(good);C(s.First==13&&good.Providers==1&&good.Closes==1);
  for(int i=1;i<=3;i++){IO f=new IO();f.Fail=i;var t=new FixtureSdReadSteps();t.Run(f);C(!t.Complete&&t.CloseKnown&&t.Consumed&&t.First==unchecked((int)0x80090010));C(f.At==i&&f.Providers<=1&&f.Opens<=1&&f.Reads<=1&&f.Closes==1);}
  for(int i=1;i<=3;i++){IO f=new IO();f.StopAt=i;var t=new FixtureSdReadSteps();t.Run(f);C(t.First==1223&&t.CloseKnown&&!t.Complete&&t.Consumed==(i>1));C(f.Closes==1);}
  IO size=new IO();size.WrongLength=true;var a=new FixtureSdReadSteps();a.Run(size);C(a.First==13&&!a.Complete&&size.Reads==1&&a.CloseKnown);
  IO malformed=new IO();malformed.Malformed=true;var m=new FixtureSdReadSteps();m.Run(malformed);C(m.First==13&&!m.Complete&&m.CloseKnown);
  IO unknown=new IO();unknown.Unknown=true;var u=new FixtureSdReadSteps();u.Run(unknown);C(u.First==6&&!u.CloseKnown&&unknown.Closes==1);u.Run(unknown);C(unknown.Closes==1);
  foreach(bool generic in new bool[]{true,false}){var e=new List<string>();FixtureSdReadSteps.Shape(Sd("D:P",generic),e);C(String.Join("\n",e.ToArray()).Contains(generic?"PROJECTION=MATCH":"reason=EXACT_MASK_ORDER_OR_SID"));}
  var unprotected=new List<string>();FixtureSdReadSteps.Shape(Sd("D:",true),unprotected);C(String.Join("\n",unprotected.ToArray()).Contains("reason=OWNER_PROTECTED_OR_COUNT"));
  C(FixtureSdReadSteps.Hex(unchecked((int)0x80090010))=="0x80090010");
  int nativeMethods=0;foreach(var method in typeof(FixtureSdReadNative).GetMethods(System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Static)){
   var attrs=method.GetCustomAttributes(typeof(System.Runtime.InteropServices.DllImportAttribute),false);if(attrs.Length==0)continue;nativeMethods++;
   var attr=(System.Runtime.InteropServices.DllImportAttribute)attrs[0];C(attr.Value=="ncrypt.dll"&&attr.ExactSpelling&&Array.IndexOf(new string[]{"NCryptOpenStorageProvider","NCryptOpenKey","NCryptGetProperty","NCryptFreeObject"},method.Name)>=0);
  }C(nativeMethods==4);
  return "PASS FIXTURE_SD_READ_MODEL checks="+checks+" NATIVE_IO_MOCKED_NO_CONSTRUCTOR_NO_KSP NO_RETRY_STOP_PARTIAL_CLOSE_UNKNOWN_CURRENT_ONLY";
 }
}
