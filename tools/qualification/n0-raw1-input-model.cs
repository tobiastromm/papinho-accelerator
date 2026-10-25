// Synthetic IO only. Never opens protected files or calls native placement.
using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Security.AccessControl;
using System.Security.Principal;
public sealed class PaccN2PlacementModel : IPaccN2PlacementIO {
 bool used;public bool BeginOnce(){if(used)return false;used=true;return true;}
 public int FailAt=-1;public string Failure;public int Seen,Creates,Writes,Closes,NativeError;public bool ClosingUnknown,Stopped;public int StopAtCall=-1;
 public readonly List<string> Calls=new List<string>();
 void Hit(string x){Calls.Add(x);if(Calls.Count==FailAt)throw new Exception("SECOND_FILE_FAILURE");if(x==Failure){Seen++;if(NativeError!=0)throw new Win32Exception(NativeError,x);throw new Exception(x);}}
 public bool Stop(){return Stopped||(StopAtCall>=0&&Calls.Count>=StopAtCall);}
 public void Check(){Hit("CHECK");}public void Create(int i){Hit("CREATE");Creates++;}
 public void Verify(int i){Hit("VERIFY");}public void Write(int i){Hit("WRITE");Writes++;}
 public void Flush(int i){Hit("FLUSH");}public void Readback(int i){Hit("READBACK");}
 public bool Close(){Closes++;return !ClosingUnknown;}
}
public static class PaccN2PlacementModelTests {
 static int cases;
 static void T(bool b,string x){cases++;if(!b)throw new Exception("MODEL_ASSERT "+x);}
 static byte[] Bytes(RawSecurityDescriptor sd){byte[] b=new byte[sd.BinaryLength];sd.GetBinaryForm(b,0);return b;}
 public static string Run(byte[] approved) {
  T(PaccN2PlacementNative.SafeDescriptor(approved),"APPROVED_DESCRIPTOR");
  T(!PaccN2PlacementNative.SafeDescriptor(new byte[0]),"EMPTY_DESCRIPTOR");
  var owner=new RawSecurityDescriptor(approved,0);owner.Owner=new SecurityIdentifier("S-1-5-11");T(!PaccN2PlacementNative.SafeDescriptor(Bytes(owner)),"WRONG_OWNER");
  var write=new RawSecurityDescriptor(approved,0);write.DiscretionaryAcl.InsertAce(3,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x12019f,new SecurityIdentifier("S-1-5-11"),false,null));T(!PaccN2PlacementNative.SafeDescriptor(Bytes(write)),"UNAUTHORIZED_AU_WRITE");
  var inherited=new RawSecurityDescriptor(approved,0);inherited.DiscretionaryAcl[0].AceFlags=AceFlags.Inherited;T(!PaccN2PlacementNative.SafeDescriptor(Bytes(inherited)),"INHERITED_ACE");
  var unprotected=new RawSecurityDescriptor(approved,0);unprotected.SetFlags(unprotected.ControlFlags&~ControlFlags.DiscretionaryAclProtected);T(!PaccN2PlacementNative.SafeDescriptor(Bytes(unprotected)),"UNPROTECTED_DACL");
  var mask=new RawSecurityDescriptor(approved,0);((CommonAce)mask.DiscretionaryAcl[2]).AccessMask=0x1f01ff;T(!PaccN2PlacementNative.SafeDescriptor(Bytes(mask)),"OWNER_GRANT_EXPANSION");
  byte[] corrupt=(byte[])approved.Clone();corrupt[0]=7;T(!PaccN2PlacementNative.SafeDescriptor(corrupt),"MALFORMED_REVISION");
  var good=new PaccN2PlacementModel();T(PaccN2PlacementSteps.Run(good)=="NONE"&&good.Creates==2&&good.Writes==2&&good.Closes==1,"TWO_EXCLUSIVE_WRITES");
  T(PaccN2PlacementSteps.Run(good)=="REPLAY_DENY"&&good.Creates==2&&good.Closes==1,"REPLAY_NO_IO");
  foreach(string fail in new string[]{"CHECK","CREATE","VERIFY","WRITE","FLUSH","READBACK"}) {
   var m=new PaccN2PlacementModel();m.Failure=fail;T(PaccN2PlacementSteps.Run(m)==fail&&m.Seen==1&&m.Closes==1,"FIRST_ERROR_NO_RETRY "+fail);
   T(m.Creates<=1&&m.Writes<=1,"NO_SECOND_EFFECT "+fail);
  }
  var collision=new PaccN2PlacementModel();collision.Failure="CREATE";T(PaccN2PlacementSteps.Run(collision)=="CREATE"&&collision.Writes==0,"COLLISION_NO_WRITE");
  var native=new PaccN2PlacementModel();native.Failure="CREATE";native.NativeError=80;native.ClosingUnknown=true;T(PaccN2PlacementSteps.Run(native)=="CREATE WIN32=80"&&native.Writes==0&&native.Closes==1,"NATIVE_FIRST_ERROR_PRESERVED_OVER_CLOSE");
  var replaced=new PaccN2PlacementModel();replaced.Failure="VERIFY";T(PaccN2PlacementSteps.Run(replaced)=="VERIFY"&&replaced.Writes==0,"REPLACED_FILE_NO_WRITE");
  var digest=new PaccN2PlacementModel();digest.Failure="READBACK";T(PaccN2PlacementSteps.Run(digest)=="READBACK"&&digest.Writes==1,"DIGEST_DIVERGENCE_NO_RETRY");
  var stop=new PaccN2PlacementModel();stop.Stopped=true;T(PaccN2PlacementSteps.Run(stop)=="STOP_REQUESTED"&&stop.Creates==0,"STOP_BEFORE_EFFECT");
  for(int at=1;at<=8;at++) {var m=new PaccN2PlacementModel();m.StopAtCall=at;T(PaccN2PlacementSteps.Run(m)=="STOP_REQUESTED"&&m.Creates<=1&&m.Closes==1,"STOP_DURING_FIRST_FILE "+at);}
  stop=new PaccN2PlacementModel();stop.StopAtCall=8;T(PaccN2PlacementSteps.Run(stop)=="STOP_REQUESTED"&&stop.Creates==1&&stop.Writes==1,"STOP_AFTER_FIRST_NO_SECOND_FILE");
  var unknown=new PaccN2PlacementModel();unknown.ClosingUnknown=true;T(PaccN2PlacementSteps.Run(unknown)=="CLOSE_UNKNOWN"&&unknown.Closes==1,"CLOSE_UNKNOWN_ONCE");
  unknown=new PaccN2PlacementModel();unknown.Failure="WRITE";unknown.ClosingUnknown=true;T(PaccN2PlacementSteps.Run(unknown)=="WRITE"&&unknown.Closes==1,"FIRST_WRITE_ERROR_PRESERVED_OVER_CLOSE");
  var partial=new PaccN2PlacementModel();partial.FailAt=10;
  T(PaccN2PlacementSteps.Run(partial)=="SECOND_FILE_FAILURE"&&partial.Creates==1&&partial.Writes==1&&partial.Closes==1,"PARTIAL_FIRST_FILE_RETAINED_NO_RETRY");
  partial=new PaccN2PlacementModel();partial.FailAt=10;partial.ClosingUnknown=true;
  T(PaccN2PlacementSteps.Run(partial)=="SECOND_FILE_FAILURE"&&partial.Closes==1,"PARTIAL_FIRST_ERROR_PRESERVED_ON_CLOSE_UNKNOWN");
  byte[] root=new byte[]{0x01,0x00,0x04,0x90,0x14,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x02,0x00,0x34,0x00,0x02,0x00,0x00,0x00,0x00,0x03,0x14,0x00,0xff,0x01,0x1f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x00,0x03,0x18,0x00,0xff,0x01,0x1f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00};
  string[] ids={"417614100:786432:1087874","417614100:851968:2672648","417614100:720896:2672656"};
  for(int i=2;i<5;i++){
   PaccN2PlacementNative.DirectoryPin(i,ids[i-2],root);T(true,"PINNED_ORIGINAL_ROOT_"+i);
   bool denied=false;try{PaccN2PlacementNative.DirectoryPin(i,"417614100:0:1",root);}catch{denied=true;}T(denied,"DIRECTORY_DIVERGENT_"+i);
   byte[] bad=(byte[])root.Clone();bad[16]^=1;denied=false;try{PaccN2PlacementNative.DirectoryPin(i,ids[i-2],bad);}catch{denied=true;}T(denied,"ROOT_RAW_DIGEST_DIVERGENT_"+i);
  }
  var unsafeRoot=new RawSecurityDescriptor(root,0);unsafeRoot.DiscretionaryAcl.InsertAce(2,new CommonAce(AceFlags.None,AceQualifier.AccessAllowed,0x1f01ff,new SecurityIdentifier("S-1-5-11"),false,null));
  bool unsafeDenied=false;try{PaccN2PlacementNative.DirectoryPin(4,ids[2],Bytes(unsafeRoot));}catch{unsafeDenied=true;}T(unsafeDenied,"UNSAFE_ROOT_ACL_DENY");
  return "N2_PLACEMENT_MODEL_PASS checks="+cases+"; SYNTHETIC_ONLY; NO_NATIVE_CREATION_NO_ACL_MUTATION";
 }
}
