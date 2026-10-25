# Native helper for the exact two-file LAB remediation; no operations at load.
Add-Type -TypeDefinition @"
using System;using System.IO;using System.Runtime.InteropServices;using System.Security.AccessControl;using System.Security.Cryptography;using Microsoft.Win32.SafeHandles;
public static class PaccFileAcl {
 [StructLayout(LayoutKind.Sequential)] struct Info {public uint attr;public System.Runtime.InteropServices.ComTypes.FILETIME c,a,w;public uint vol,sh,sl,links,hi,lo;}
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr CreateFileW(string p,uint a,uint s,IntPtr sa,uint d,uint f,IntPtr t);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetFileInformationByHandle(IntPtr h,out Info i);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll")] static extern IntPtr LocalFree(IntPtr p);
 [DllImport("advapi32.dll")] static extern uint GetSecurityInfo(IntPtr h,int type,uint info,out IntPtr o,out IntPtr g,out IntPtr d,out IntPtr s,out IntPtr sd);
 [DllImport("advapi32.dll")] static extern uint GetSecurityDescriptorLength(IntPtr p);
 [DllImport("advapi32.dll")] static extern uint SetSecurityInfo(IntPtr h,int type,uint flags,IntPtr o,IntPtr g,IntPtr d,IntPtr s);
 [DllImport("advapi32.dll")] static extern bool GetSecurityDescriptorOwner(IntPtr p,out IntPtr o,out bool def);
 [DllImport("advapi32.dll")] static extern bool GetSecurityDescriptorDacl(IntPtr p,out bool present,out IntPtr d,out bool def);
 [DllImport("advapi32.dll")] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int n);
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 static IntPtr retainedDescriptor=IntPtr.Zero;
 public static bool DescriptorRetained {get{return retainedDescriptor!=IntPtr.Zero;}}
 public static IntPtr Open(string path,bool write,bool directory) {
  IntPtr h=CreateFileW(path,write?0x001e0081u:(directory?0x00020080u:0x00120081u),1,IntPtr.Zero,3,directory?0x02200000u:0x00200000u,IntPtr.Zero);
  if(h==new IntPtr(-1))throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());return h;
 }
 public static void Identity(IntPtr h,uint high,uint low,ulong size,uint wh,uint wl,bool directory) {
  Info i;if(!GetFileInformationByHandle(h,out i))throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());
  if(i.vol!=417614100u||i.hi!=high||i.lo!=low||(i.attr&0x400)!=0||(((i.attr&16)!=0)!=directory))throw new Exception("IDENTITY_OR_REPARSE_MISMATCH");
  if(!directory&&((((ulong)i.sh<<32)|i.sl)!=size||i.w.dwHighDateTime!=(int)wh||i.w.dwLowDateTime!=(int)wl))throw new Exception("FILE_METADATA_MISMATCH");
 }
 public static byte[] Read(IntPtr h) {
  if(DescriptorRetained)throw new Exception("DESCRIPTOR_RETENTION_STOP");IntPtr o,g,d,s,p;uint e=GetSecurityInfo(h,1,7,out o,out g,out d,out s,out p);if(e!=0)throw new System.ComponentModel.Win32Exception((int)e);
  try {uint n=GetSecurityDescriptorLength(p);if(n==0||n>65536)throw new Exception("SD_LENGTH");byte[] b=new byte[n];Marshal.Copy(p,b,0,(int)n);return b;}
  finally{if(LocalFree(p)!=IntPtr.Zero){retainedDescriptor=p;throw new Exception("LOCALFREE_UNCERTAIN");}}
 }
 public static string Digest(byte[] b){using(var sha=SHA256.Create())return BitConverter.ToString(sha.ComputeHash(b)).Replace("-","").ToLowerInvariant();}
 public static string FileDigest(IntPtr h){using(var safe=new SafeFileHandle(h,false))using(var f=new FileStream(safe,FileAccess.Read,8192,false))using(var sha=SHA256.Create()){f.Position=0;return BitConverter.ToString(sha.ComputeHash(f)).Replace("-","").ToLowerInvariant();}}
 public static byte[] Encode(string s){var d=new RawSecurityDescriptor(s);var b=new byte[d.BinaryLength];d.GetBinaryForm(b,0);return b;}
 public static string Sddl(byte[] b){return new RawSecurityDescriptor(b,0).GetSddlForm(AccessControlSections.Owner|AccessControlSections.Group|AccessControlSections.Access);}
 public static bool Equivalent(byte[] expected,byte[] observed,int count) {
  try {var a=new RawSecurityDescriptor(expected,0);var b=new RawSecurityDescriptor(observed,0);
   if((a.ControlFlags&ControlFlags.DiscretionaryAclProtected)==0||(b.ControlFlags&ControlFlags.DiscretionaryAclProtected)==0||(((int)a.ControlFlags^(int)b.ControlFlags)&~0x400)!=0)return false;
   if(a.Owner==null||b.Owner==null||!a.Owner.Equals(b.Owner)||a.Group==null||b.Group==null||!a.Group.Equals(b.Group)||a.ResourceManagerControl!=b.ResourceManagerControl||a.SystemAcl!=null||b.SystemAcl!=null)return false;
   if(a.DiscretionaryAcl==null||b.DiscretionaryAcl==null||a.DiscretionaryAcl.Count!=count||b.DiscretionaryAcl.Count!=count||a.DiscretionaryAcl.Revision!=b.DiscretionaryAcl.Revision)return false;
   for(int n=0;n<count;n++){var x=a.DiscretionaryAcl[n];var y=b.DiscretionaryAcl[n];byte[] xb=new byte[x.BinaryLength],yb=new byte[y.BinaryLength];x.GetBinaryForm(xb,0);y.GetBinaryForm(yb,0);if(Convert.ToBase64String(xb)!=Convert.ToBase64String(yb))return false;}return true;
  }catch{return false;}
 }
 public static void Apply(IntPtr h,byte[] expectedPost,bool ownerChange){
  if(DescriptorRetained)throw new Exception("RETENTION_DENY");var pin=GCHandle.Alloc(expectedPost,GCHandleType.Pinned);
  try{IntPtr o,d;bool def,present;if(!GetSecurityDescriptorOwner(pin.AddrOfPinnedObject(),out o,out def)||!GetSecurityDescriptorDacl(pin.AddrOfPinnedObject(),out present,out d,out def)||!present)throw new Exception("POST_INVALID");
   uint e=SetSecurityInfo(h,1,ownerChange?0x80000005u:0x80000004u,ownerChange?o:IntPtr.Zero,IntPtr.Zero,d,IntPtr.Zero);if(e!=0)throw new System.ComponentModel.Win32Exception((int)e);
  }finally{pin.Free();}
 }
}
"@
