// Read-only reuse of original-handle PaccFileAcl primitives; no admission authority.
using System;using System.Collections.Generic;using System.ComponentModel;using System.Runtime.InteropServices;using System.Text;
public static class PaccN0InputRead {
 [StructLayout(LayoutKind.Sequential)] struct Info {public uint attr;public System.Runtime.InteropServices.ComTypes.FILETIME c,a,w;public uint vol,sh,sl,links,hi,lo;}
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetFileInformationByHandle(IntPtr h,out Info i);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetHandleInformation(IntPtr h,out uint flags);
 [DllImport("kernel32.dll",SetLastError=true)]static extern uint GetFileType(IntPtr h);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern uint GetFinalPathNameByHandleW(IntPtr h,StringBuilder p,uint n,uint flags);
 static readonly string[] paths={@"C:\",@"C:\ProgramData",@"C:\ProgramData\PapinhoAccelerator",@"C:\ProgramData\PapinhoAccelerator\Protected",@"C:\ProgramData\PapinhoAccelerator\Protected\N0",@"C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence",@"C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved"};
 static readonly uint[] hi={327680,131072,786432,851968,720896,196608,131072},lo={5,227641,1087874,2672648,2672656,2841429,2841430};
 static readonly string[] sd={"9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8","c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579","0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814","0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814"};
 static readonly string[] content={"babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0","5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294"};
 static readonly List<IntPtr> held=new List<IntPtr>();public static bool Retained;static bool used;
 static void Check(int index,IntPtr h) {
  Info f;uint flags;if(!GetFileInformationByHandle(h,out f)||!GetHandleInformation(h,out flags))throw new Win32Exception(Marshal.GetLastWin32Error());
  if((flags&1)!=0||GetFileType(h)!=1||f.vol!=417614100||f.hi!=hi[index]||f.lo!=lo[index]||(f.attr&0x400)!=0||(((f.attr&16)!=0)!=(index<5)))throw new Exception("NATIVE_ID_TYPE_DENY");
  var path=new StringBuilder(1100);uint n=GetFinalPathNameByHandleW(h,path,1100,0);if(n<4||n>=1100||!String.Equals(path.ToString(),@"\\?\"+paths[index],StringComparison.OrdinalIgnoreCase))throw new Exception("FINAL_PATH_DENY");
  byte[] raw=PaccFileAcl.Read(h);if(PaccFileAcl.Digest(raw)!=sd[index])throw new Exception("RAW1_DIGEST_DENY");
  if(index>=2&&(raw.Length!=(index<5?104:152)||BitConverter.ToUInt16(raw,2)!=0x9004))throw new Exception("RAW1_PROFILE_DENY");
  if(index>=5&&(f.sh!=0||f.sl!=(index==5?5358:326)||f.links!=1||PaccFileAcl.FileDigest(h)!=content[index-5]))throw new Exception("CONTENT_SIZE_LINK_DENY");
 }
 public static string Run() {
  if(used)return "REPLAY_DENY";used=true;string first="NONE";
  try {
   for(int i=0;i<paths.Length;i++){IntPtr h=PaccFileAcl.Open(paths[i],false,i<5);held.Add(h);Check(i,h);}
   for(int i=0;i<paths.Length;i++){Check(i,held[i]);Console.WriteLine("N0_INPUT_READ_VERIFIED path="+paths[i]+" native=417614100:"+hi[i]+":"+lo[i]+" raw_sha256="+sd[i]+(i>=5?" content_sha256="+content[i-5]:""));}
  }catch(Exception e){first=e.Message;var native=e as Win32Exception;if(native!=null)first+=" WIN32="+native.NativeErrorCode;}
  if(PaccFileAcl.DescriptorRetained){Retained=true;if(first=="NONE")first="DESCRIPTOR_CLOSE_UNKNOWN";return first;}
  for(int i=held.Count-1;i>=0;i--){if(!PaccFileAcl.CloseHandle(held[i])){Retained=true;if(first=="NONE")first="CLOSE_UNKNOWN WIN32="+Marshal.GetLastWin32Error();break;}held.RemoveAt(i);}
  return first;
 }
}
