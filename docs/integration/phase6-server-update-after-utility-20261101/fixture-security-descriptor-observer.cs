// Fixed prospective readonly observation. No product authority or key mutation API.
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Security.Principal;

public interface IFixtureSdReadIO {
 bool Stop(); int Provider(); int Open(); int Read(byte[] output,out int needed); bool Close();
}
public sealed class FixtureSdReadSteps {
 bool used; public bool Consumed,CloseKnown,Complete;
 public int First; public string Stage="NONE";
 public readonly List<string> Evidence=new List<string>();
 public static string Hex(int value){return "0x"+unchecked((uint)value).ToString("x8");}
 public void Run(IFixtureSdReadIO io) {
  if(used){First=13;Stage="REPLAY_DENY";return;}used=true;
  try {
   if(io.Stop()){First=1223;Stage="STOP";return;}
   Consumed=true;First=io.Provider();Stage="OPEN_PROVIDER";Evidence.Add(Stage+" status="+Hex(First));if(First!=0)return;
   if(io.Stop()){First=1223;Stage="STOP";return;}
   First=io.Open();Stage="OPEN_FIXED_KEY";Evidence.Add(Stage+" status="+Hex(First));if(First!=0)return;
   if(io.Stop()){First=1223;Stage="STOP";return;}
   byte[] buffer=new byte[1024];int needed=0;
   First=io.Read(buffer,out needed);Stage="GET_OWNER_DACL";
   Evidence.Add(Stage+" status="+Hex(First)+" result_length_snapshot="+needed+" capacity=1024 flags=5; FAILED_RESULT_LENGTH_NOT_PROVEN_WRITTEN");
   if(First!=0)return;
   if(needed<=0||needed>buffer.Length){First=13;Stage="LENGTH_DENY_NO_RETRY";return;}
   byte[] exact=new byte[needed];Buffer.BlockCopy(buffer,0,exact,0,needed);
   Shape(exact,Evidence);Complete=true;Stage="OBSERVATION_COMPLETE_NOT_QUALIFICATION";
  } catch(Exception){if(First==0){First=13;Stage="MANAGED_FAILURE";}}
  finally {try {CloseKnown=io.Close();}catch {CloseKnown=false;}if(!CloseKnown&&First==0){First=6;Stage="OBSERVER_CLOSE_UNKNOWN";}}
 }
 public static void Shape(byte[] descriptor,List<string> evidence) {
  var d=new RawSecurityDescriptor(descriptor,0);var acl=d.DiscretionaryAcl;
  bool owner=d.Owner!=null&&d.Owner.Value==FixtureSdReadNative.Actor;
  bool protectedDacl=(d.ControlFlags&ControlFlags.DiscretionaryAclProtected)!=0;
  evidence.Add("CURRENT_SD owner_operator="+owner+" protected="+protectedDacl+" dacl_count="+(acl==null?-1:acl.Count)+"; CURRENT_DATA_NOT_HISTORICAL_ATTESTATION");
  string reason="NONE";
  if(!owner||!protectedDacl||acl==null||acl.Count!=3)reason="OWNER_PROTECTED_OR_COUNT";
  if(acl!=null)for(int i=0;i<acl.Count;i++){
   var a=acl[i] as CommonAce;
   if(a==null){evidence.Add("CURRENT_ACE index="+i+" unsupported=True");if(reason=="NONE")reason="UNSUPPORTED_ACE";continue;}
   string role=a.SecurityIdentifier.Value==FixtureSdReadNative.Actor?"OPERATOR":a.SecurityIdentifier.Value==FixtureSdReadNative.Runtime?"RUNTIME":a.SecurityIdentifier.IsWellKnown(WellKnownSidType.LocalSystemSid)?"SYSTEM":"OTHER";
   evidence.Add("CURRENT_ACE index="+i+" type="+a.AceQualifier+" flags="+(int)a.AceFlags+" callback="+a.IsCallback+" mask="+Hex(a.AccessMask)+" sid_role="+role);
   if(reason=="NONE"&&a.AceFlags!=AceFlags.None)reason="ACE_FLAGS";
   if(reason=="NONE"&&(a.IsCallback||a.AccessMask!=0x10000000||
     (i==0?(a.AceQualifier!=AceQualifier.AccessDenied||role!="RUNTIME"):
      (a.AceQualifier!=AceQualifier.AccessAllowed||role!=(i==1?"OPERATOR":"SYSTEM")))))reason="EXACT_MASK_ORDER_OR_SID";
  }
  evidence.Add("CURRENT_STRICT_POLICY_PROJECTION="+(reason=="NONE"?"MATCH":"REJECT")+" reason="+reason+"; PROJECTION_NOT_AUTHORITY_NOT_POSITIVE_CONTROL_NOT_READINESS");
 }
}
public sealed class FixtureSdReadNative : IFixtureSdReadIO {
 public const string Actor="S-1-5-21-785699550-1440458426-1682014700-1108";
 public const string Runtime="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 public const string Name="PapinhoAccelerator.CA.dea8648758bebcbbcabde016bf911e77.b4f8ff6a41ba3b4d3e5ff403d0d017a7";
 IntPtr provider,key;bool closed;
 [DllImport("ncrypt.dll",CharSet=CharSet.Unicode,ExactSpelling=true)]static extern int NCryptOpenStorageProvider(out IntPtr result,string name,uint flags);
 [DllImport("ncrypt.dll",CharSet=CharSet.Unicode,ExactSpelling=true)]static extern int NCryptOpenKey(IntPtr provider,out IntPtr result,string name,uint legacy,uint flags);
 [DllImport("ncrypt.dll",CharSet=CharSet.Unicode,ExactSpelling=true)]static extern int NCryptGetProperty(IntPtr key,string property,[Out] byte[] output,int capacity,out int needed,uint flags);
 [DllImport("ncrypt.dll",ExactSpelling=true)]static extern int NCryptFreeObject(IntPtr value);
 public bool Stop(){return Console.KeyAvailable&&Console.ReadKey(true).Key==ConsoleKey.Escape;}
 public int Provider(){int s=NCryptOpenStorageProvider(out provider,"Microsoft Software Key Storage Provider",0);if(s==0&&provider==IntPtr.Zero)throw new InvalidOperationException();return s;}
 public int Open(){int s=NCryptOpenKey(provider,out key,Name,0,0x60);if(s==0&&key==IntPtr.Zero)throw new InvalidOperationException();return s;}
 public int Read(byte[] output,out int needed){if(output==null||output.Length!=1024)throw new InvalidOperationException();return NCryptGetProperty(key,"Security Descr",output,1024,out needed,5);}
 public bool Close(){if(closed)return false;closed=true;
  if(key!=IntPtr.Zero){if(NCryptFreeObject(key)!=0)return false;key=IntPtr.Zero;}
  if(provider!=IntPtr.Zero){if(NCryptFreeObject(provider)!=0)return false;provider=IntPtr.Zero;}return true;
 }
}
