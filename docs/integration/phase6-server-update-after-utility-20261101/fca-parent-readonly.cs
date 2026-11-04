// Focal DATA observer; reuses the existing native original-directory/SD pattern.
// No create disposition, mutation API, key provider, SCM or authority producer.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Security.Cryptography;
using System.Text;

public interface IFcaParentReadIO {
 bool Stop(); string Resolve(); void Open(int index,string parent);
 string Inspect(int index,bool revalidate); bool Close();
}
public sealed class FcaParentReadSteps {
 bool used; public bool Consumed,Complete,CloseKnown; public string First="NONE";
 public readonly List<string> Evidence=new List<string>();
 public void Run(IFcaParentReadIO io) {
  if(used){First="REPLAY_DENY";return;}used=true;
  try {
   if(io.Stop())throw new Exception("STOP");Consumed=true;
   string parent=io.Resolve();int count=FcaParentReadNative.Components(parent).Length+1;
   for(int i=0;i<count;i++) {if(io.Stop())throw new Exception("STOP");io.Open(i,parent);Evidence.Add(io.Inspect(i,false));}
   for(int i=0;i<count;i++) {if(io.Stop())throw new Exception("STOP");Evidence.Add(io.Inspect(i,true));}
   Complete=true;
  }catch(Exception e){First=e.Message;}
  try {CloseKnown=io.Close();if(!CloseKnown&&First=="NONE")First="CLOSE_UNKNOWN";}catch{CloseKnown=false;if(First=="NONE")First="CLOSE_UNKNOWN";}
 }
}
public sealed class FcaParentReadNative : IFcaParentReadIO {
 public const string Actor="S-1-5-21-785699550-1440458426-1682014700-1108";
 public const string Installer="S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464";
 const uint Rights=0x001200a0,Options=0x00200021;
 [StructLayout(LayoutKind.Sequential)] struct FT {public uint low,high;}
 [StructLayout(LayoutKind.Sequential)] struct Info {public uint attr;public FT c,a,w;public uint volume,sh,sl,links,hi,lo;}
 [StructLayout(LayoutKind.Sequential)] struct US {public ushort length,max;public IntPtr text;}
 [StructLayout(LayoutKind.Sequential)] struct OA {public int length;public IntPtr root,name;public uint flags;public IntPtr sd,sqos;}
 [StructLayout(LayoutKind.Sequential)] struct IOS {public IntPtr status,information;}
 [DllImport("shell32.dll",ExactSpelling=true)]static extern int SHGetKnownFolderPath(ref Guid id,uint flags,IntPtr token,out IntPtr path);
 [DllImport("ole32.dll",ExactSpelling=true)]static extern void CoTaskMemFree(IntPtr p);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,ExactSpelling=true,SetLastError=true)]static extern uint QueryDosDeviceW(string name,StringBuilder target,uint n);
 [DllImport("ntdll.dll",ExactSpelling=true)]static extern int NtCreateFile(IntPtr handle,uint access,IntPtr oa,IntPtr ios,IntPtr size,uint attr,uint share,uint disposition,uint options,IntPtr ea,uint eaBytes);
 [DllImport("ntdll.dll",ExactSpelling=true)]static extern int NtQuerySecurityObject(IntPtr handle,uint information,IntPtr sd,uint capacity,out uint needed);
 [DllImport("kernel32.dll",ExactSpelling=true,SetLastError=true)]static extern bool GetFileInformationByHandle(IntPtr h,out Info info);
 [DllImport("kernel32.dll",ExactSpelling=true,SetLastError=true)]static extern bool GetHandleInformation(IntPtr h,out uint flags);
 [DllImport("kernel32.dll",ExactSpelling=true,SetLastError=true)]static extern uint GetFileType(IntPtr h);
 [DllImport("kernel32.dll",ExactSpelling=true,SetLastError=true)]static extern bool CloseHandle(IntPtr h);
 readonly List<IntPtr> handles=new List<IntPtr>(),backing=new List<IntPtr>();
 readonly List<Info> ids=new List<Info>();readonly List<string> sdHashes=new List<string>();
 bool uncertain,closed;string mapped;
 public static string[] Components(string parent) {
  if(parent==null||parent.Length<4||parent.Length>=260||parent[0]<'A'||parent[0]>'Z'||parent[1]!=':'||parent[2]!='\\'||!parent.EndsWith("\\Papinho",StringComparison.Ordinal))throw new Exception("RESOLVED_PATH_SHAPE");
  foreach(char c in parent)if(c<32||c>126||c=='/'||c=='"')throw new Exception("RESOLVED_PATH_SHAPE");
  string[] parts=parent.Substring(3).Split('\\');if(parts.Length+1>8)throw new Exception("DIRECTORY_SLOTS");
  foreach(string p in parts)if(p.Length==0||p=="."||p==".."||p.IndexOf(':')>=0||p.EndsWith(".",StringComparison.Ordinal))throw new Exception("PATH_COMPONENT");return parts;
 }
 public static bool Protected(byte[] raw,bool anchor,out int rejected,out uint mask) {
  rejected=-1;mask=0;
  try {
   var sd=new RawSecurityDescriptor(raw,0);
   if(sd.Owner==null||!Trusted(sd.Owner.Value)||(sd.ControlFlags&ControlFlags.DiscretionaryAclProtected)==0||sd.DiscretionaryAcl==null||sd.DiscretionaryAcl.Count==0)return false;
   for(int i=0;i<sd.DiscretionaryAcl.Count;i++) {
    var a=sd.DiscretionaryAcl[i] as CommonAce;rejected=i;
    if(a==null||a.IsCallback||(a.AceType!=AceType.AccessAllowed&&a.AceType!=AceType.AccessDenied))return false;
    if((a.AceFlags&AceFlags.InheritOnly)!=0)continue;
    mask=unchecked((uint)a.AccessMask);
    if(a.AceType==AceType.AccessAllowed&&(mask&0x500d0156U)!=0&&!Trusted(a.SecurityIdentifier.Value)) {
     if(anchor&&a.AceFlags==AceFlags.None&&mask==4&&a.SecurityIdentifier.Value=="S-1-5-11")continue;return false;
    }
   }rejected=-1;mask=0;return true;
  }catch{return false;}
 }
 static bool Trusted(string sid){return sid==Actor||sid==Installer||sid=="S-1-5-18"||sid=="S-1-5-32-544";}
 static string Hash(byte[] b){using(var h=SHA256.Create())return BitConverter.ToString(h.ComputeHash(b)).Replace("-","").ToLowerInvariant();}
 public bool Stop(){return Console.KeyAvailable&&Console.ReadKey(true).Key==ConsoleKey.Escape;}
 public string Resolve() {
  Guid id=new Guid("62AB5D82-FDC1-4DC3-A9DD-070D1D495D97");IntPtr text=IntPtr.Zero;
  int status=SHGetKnownFolderPath(ref id,0x4000,IntPtr.Zero,out text);
  try {if(status!=0||text==IntPtr.Zero)throw new Exception("KNOWNFOLDER status=0x"+unchecked((uint)status).ToString("x8"));
   string root=Marshal.PtrToStringUni(text);if(root==null||root.Length>=251)throw new Exception("KNOWNFOLDER_BOUND");
   string parent=root+(root.EndsWith("\\",StringComparison.Ordinal)?"":"\\")+"Papinho";Components(parent);
   var device=new StringBuilder(512);uint n=QueryDosDeviceW(parent.Substring(0,2),device,512);
   if(n==0)throw new Win32Exception(Marshal.GetLastWin32Error(),"DEVICE_QUERY");mapped=device.ToString();
   if(!mapped.StartsWith("\\Device\\",StringComparison.Ordinal)||mapped.Length+1>=264)throw new Exception("DEVICE_NAMESPACE");
   Console.WriteLine("FCA_PARENT_LOCATOR parent="+parent+"; KNOWNFOLDER_DONT_VERIFY_NOT_AUTHORITY; NO_ENV_NO_REGISTRY_FALLBACK");return parent;
  }finally{if(text!=IntPtr.Zero)CoTaskMemFree(text);}
 }
 public void Open(int index,string parent) {
  string[] parts=Components(parent);if(closed||uncertain||index!=handles.Count||index>parts.Length)throw new Exception("OPEN_ORDER");
  string name=index==0?mapped+"\\":parts[index-1];
  IntPtr text=Marshal.StringToHGlobalUni(name),unicode=Marshal.AllocHGlobal(16),oa=Marshal.AllocHGlobal(48),ios=Marshal.AllocHGlobal(16),returned=Marshal.AllocHGlobal(8);
  foreach(IntPtr p in new IntPtr[]{text,unicode,oa,ios,returned})backing.Add(p);
  US u=new US();u.text=text;u.length=(ushort)(name.Length*2);u.max=u.length;Marshal.StructureToPtr(u,unicode,false);
  OA a=new OA();a.length=48;a.root=index==0?IntPtr.Zero:handles[index-1];a.name=unicode;a.flags=0x1040;Marshal.StructureToPtr(a,oa,false);
  Marshal.StructureToPtr(new IOS(),ios,false);Marshal.WriteIntPtr(returned,IntPtr.Zero);
  if(Stop())throw new Exception("STOP");
  int status=NtCreateFile(returned,Rights,oa,ios,IntPtr.Zero,0x80,1,1,Options,IntPtr.Zero,0);
  IntPtr h=Marshal.ReadIntPtr(returned);IOS io=(IOS)Marshal.PtrToStructure(ios,typeof(IOS));
  Console.WriteLine("FCA_PARENT_OPEN index="+index+" ntstatus=0x"+unchecked((uint)status).ToString("x8")+" handle_returned="+(h!=IntPtr.Zero)+"; IOSB_SNAPSHOT_NOT_PROOF_ON_FAILURE");
  if(status!=0||io.status!=IntPtr.Zero||io.information.ToInt64()!=1||h==IntPtr.Zero){if(status>0||io.status.ToInt64()==0x103||h!=IntPtr.Zero)uncertain=true;throw new Exception("NATIVE_OPEN index="+index+" ntstatus=0x"+unchecked((uint)status).ToString("x8"));}
  handles.Add(h);FreeBacking();
 }
 void FreeBacking(){foreach(IntPtr p in backing)Marshal.FreeHGlobal(p);backing.Clear();}
 public string Inspect(int index,bool revalidate) {
  if(Stop())throw new Exception("STOP");Info f;uint flags;IntPtr h=handles[index];
  if(!GetHandleInformation(h,out flags))throw new Win32Exception(Marshal.GetLastWin32Error(),"HANDLE_FLAGS_QUERY");
  if(flags!=0||GetFileType(h)!=1)throw new Exception("HANDLE_FLAGS_OR_TYPE index="+index);
  if(!GetFileInformationByHandle(h,out f))throw new Win32Exception(Marshal.GetLastWin32Error(),"IDENTITY_QUERY");
  if((f.attr&0x10)==0||(f.attr&0x400)!=0)throw new Exception("REPARSE_OR_TYPE index="+index);
  // Same two-call NT OWNER|DACL pattern as scqi_native_file_sd, bounded at 64KiB.
  if(Stop())throw new Exception("STOP");uint needed;int status=NtQuerySecurityObject(h,5,IntPtr.Zero,0,out needed);
  if(unchecked((uint)status)!=0xc0000023U||needed<20||needed>65536)throw new Exception("SD_SIZE index="+index+" status=0x"+unchecked((uint)status).ToString("x8"));
  IntPtr sd=Marshal.AllocHGlobal((int)needed);byte[] raw;
  try {for(int i=0;i<(int)needed;i++)Marshal.WriteByte(sd,i,0);if(Stop())throw new Exception("STOP");uint used;status=NtQuerySecurityObject(h,5,sd,needed,out used);if(status!=0||used<20||used>needed)throw new Exception("SD_READ index="+index+" status=0x"+unchecked((uint)status).ToString("x8"));raw=new byte[used];Marshal.Copy(sd,raw,0,(int)used);var view=new RawSecurityDescriptor(raw,0);if(view.BinaryLength!=used||(view.ControlFlags&ControlFlags.SelfRelative)==0)throw new Exception("SD_STRUCTURE");}finally{Marshal.FreeHGlobal(sd);}
  string hash=Hash(raw);if(revalidate){Info old=ids[index];if(f.volume!=old.volume||f.hi!=old.hi||f.lo!=old.lo||f.attr!=old.attr||hash!=sdHashes[index])throw new Exception("CONTINUITY index="+index);}else{ids.Add(f);sdHashes.Add(hash);}
  int rejected;uint mask;bool matches=Protected(raw,index==0,out rejected,out mask);
  var shape=new RawSecurityDescriptor(raw,0);string role="NONE";
  if(rejected>=0&&shape.DiscretionaryAcl!=null&&rejected<shape.DiscretionaryAcl.Count){var ace=shape.DiscretionaryAcl[rejected] as CommonAce;if(ace!=null){string sid=ace.SecurityIdentifier.Value;role=sid=="S-1-5-32-545"?"BUILTIN_USERS":sid=="S-1-5-11"?"AUTHENTICATED_USERS":Trusted(sid)?"TRUSTED":"OTHER_NOT_EXPORTED";}}
  if(Stop())throw new Exception("STOP");
  return "FCA_PARENT_COMPONENT index="+index+" revalidation="+revalidate+" identity="+f.volume+":"+f.hi+":"+f.lo+" sd_sha256="+hash+" policy_match="+matches+" rejected_ace="+rejected+" rejected_role="+role+" mask=0x"+mask.ToString("x8")+" owner_trusted="+(shape.Owner!=null&&Trusted(shape.Owner.Value))+" protected="+((shape.ControlFlags&ControlFlags.DiscretionaryAclProtected)!=0)+"; PROJECTION_ONLY_NOT_FCA_ADMISSION";
 }
 public bool Close(){if(closed)return false;closed=true;if(uncertain)return false;for(int i=handles.Count-1;i>=0;i--){if(!CloseHandle(handles[i]))return false;handles.RemoveAt(i);}FreeBacking();return true;}
 public static bool Layouts(){return IntPtr.Size==8&&Marshal.SizeOf(typeof(US))==16&&Marshal.SizeOf(typeof(OA))==48&&Marshal.SizeOf(typeof(IOS))==16&&Marshal.SizeOf(typeof(Info))==52;}
}
