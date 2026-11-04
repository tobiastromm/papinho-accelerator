// Fixed two-document administrative materialization adapter. No SCQ/SCM API.
// Bytes/SD/possession are DATA, never an administrative authorization source.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Security.Cryptography;
using System.Security.Principal;

public interface IFirstStartDocumentsIO {
 bool Stop(); void Check(); void Create(int i); void Write(int i);
 void Flush(int i); void Verify(int i); bool Close();
}
public sealed class FirstStartDocumentSteps {
 bool used; public bool Consumed; public bool CloseKnown;
 public string Run(IFirstStartDocumentsIO io) {
  if(used)return "REPLAY_DENY";used=true;string first="NONE";
  try {
   for(int i=0;i<2;i++) {
    Guard(io);io.Check();Guard(io);Consumed=true;io.Create(i);
    Guard(io);io.Verify(i);Guard(io);io.Write(i);
    Guard(io);io.Flush(i);Guard(io);io.Verify(i);io.Check();
   }
  } catch(Exception e) {first=e.Message;}
  try {CloseKnown=io.Close();if(!CloseKnown&&first=="NONE")first="CLOSE_UNKNOWN";}
  catch {CloseKnown=false;if(first=="NONE")first="CLOSE_UNKNOWN";}
  return first;
 }
 static void Guard(IFirstStartDocumentsIO io) {if(io.Stop())throw new Exception("STOP");}
}
public sealed class FirstStartDocumentsNative : IFirstStartDocumentsIO {
 public const string Actor="S-1-5-21-785699550-1440458426-1682014700-1108";
 public const string Folder=@"C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101";
 const uint Read=0x00120080, ShareRead=1, BackupNoReparse=0x02200000;
 [StructLayout(LayoutKind.Sequential)] struct FT {public uint low,high;}
 [StructLayout(LayoutKind.Sequential)] struct Info {public uint attributes;public FT creation,access,write;public uint volume,highSize,lowSize,links,highId,lowId;}
 [StructLayout(LayoutKind.Sequential)] struct US {public ushort length,max;public IntPtr buffer;}
 [StructLayout(LayoutKind.Sequential)] struct OA {public int length;public IntPtr root,name;public uint attributes;public IntPtr sd,sqos;}
 [StructLayout(LayoutKind.Sequential)] struct IOS {public IntPtr status,information;}
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern IntPtr CreateFileW(string p,uint a,uint s,IntPtr sa,uint d,uint f,IntPtr t);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetFileInformationByHandle(IntPtr h,out Info f);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool WriteFile(IntPtr h,byte[] b,uint n,out uint done,IntPtr ov);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool ReadFile(IntPtr h,byte[] b,uint n,out uint done,IntPtr ov);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool SetFilePointerEx(IntPtr h,long p,out long result,uint method);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool FlushFileBuffers(IntPtr h);
 [DllImport("ntdll.dll")]static extern int NtCreateFile(out IntPtr h,uint rights,ref OA oa,out IOS io,IntPtr size,uint attrs,uint share,uint disposition,uint options,IntPtr ea,uint eaBytes);
 [DllImport("advapi32.dll")]static extern uint GetSecurityInfo(IntPtr h,uint type,uint what,out IntPtr owner,out IntPtr group,out IntPtr dacl,out IntPtr sacl,out IntPtr sd);
 [DllImport("advapi32.dll")]static extern uint GetSecurityDescriptorLength(IntPtr sd);
 [DllImport("kernel32.dll")]static extern IntPtr LocalFree(IntPtr p);
 readonly List<IntPtr> chain=new List<IntPtr>();readonly List<Info> chainIds=new List<Info>();readonly List<string> chainSd=new List<string>();
 readonly List<IntPtr> allocatorRetained=new List<IntPtr>();bool allocatorUncertain;
 readonly IntPtr[] sources=new IntPtr[2],files=new IntPtr[2];readonly Info[] sourceIds=new Info[2],fileIds=new Info[2];
 readonly byte[][] bytes=new byte[2][];readonly string[] pins=new string[2],fileSd=new string[2];readonly bool[] written=new bool[2],flushed=new bool[2];
 public uint CreatedMask,VerifiedMask;
 public readonly List<string> Evidence=new List<string>();
 static Exception Error(string stage){return new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 static bool Bad(IntPtr h){return h==IntPtr.Zero||h==new IntPtr(-1);}
 static Info Facts(IntPtr h,bool directory) {
  Info f;if(!GetFileInformationByHandle(h,out f))throw Error("IDENTITY");
  if((f.attributes&0x400)!=0||((f.attributes&0x10)!=0)!=directory||(!directory&&f.links!=1))throw new Exception("REPARSE_TYPE_LINKS");return f;
 }
 static bool Same(Info a,Info b){return a.volume==b.volume&&a.highId==b.highId&&a.lowId==b.lowId&&a.highSize==b.highSize&&a.lowSize==b.lowSize&&a.attributes==b.attributes&&a.links==b.links;}
 static string Hash(byte[] b){using(var h=SHA256.Create())return BitConverter.ToString(h.ComputeHash(b)).Replace("-","").ToLowerInvariant();}
 byte[] Descriptor(IntPtr h) {
  IntPtr o,g,d,s,sd;uint e=GetSecurityInfo(h,1,5,out o,out g,out d,out s,out sd);
  if(e!=0||sd==IntPtr.Zero){if(sd!=IntPtr.Zero){allocatorRetained.Add(sd);allocatorUncertain=true;}throw new Win32Exception((int)e,"DESCRIPTOR");}
  try {uint n=GetSecurityDescriptorLength(sd);if(n==0||n>65536)throw new Exception("DESCRIPTOR_LENGTH");byte[] b=new byte[n];Marshal.Copy(sd,b,0,(int)n);return b;}
  finally {if(LocalFree(sd)!=IntPtr.Zero){allocatorRetained.Add(sd);allocatorUncertain=true;throw new Exception("DESCRIPTOR_CLOSE_UNKNOWN");}}
 }
 public static byte[] CreationDescriptor() {var d=new RawSecurityDescriptor("O:"+Actor+"G:SYD:P(A;;FA;;;"+Actor+")(A;;FA;;;SY)(A;;FA;;;BA)");byte[] b=new byte[d.BinaryLength];d.GetBinaryForm(b,0);return b;}
 public static bool ProtectedDocument(byte[] b) {
  try {var d=new RawSecurityDescriptor(b,0);if(d.Owner==null||d.Owner.Value!=Actor||(d.ControlFlags&ControlFlags.DiscretionaryAclProtected)==0||d.DiscretionaryAcl==null||d.DiscretionaryAcl.Count!=3)return false;
   string[] sid={Actor,"S-1-5-18","S-1-5-32-544"};
   for(int i=0;i<3;i++){var a=d.DiscretionaryAcl[i] as CommonAce;if(a==null||a.AceType!=AceType.AccessAllowed||a.IsCallback||a.AceFlags!=AceFlags.None||a.SecurityIdentifier.Value!=sid[i]||a.AccessMask!=0x1f01ff)return false;}return true;
  } catch{return false;}
 }
 static byte[] ReadExact(IntPtr h,uint size) {
  if(size==0||size>65536)throw new Exception("DOCUMENT_BOUND");long p;if(!SetFilePointerEx(h,0,out p,0))throw Error("SEEK");
  byte[] b=new byte[size];uint n;if(!ReadFile(h,b,size,out n,IntPtr.Zero)||n!=size)throw Error("READBACK");return b;
 }
 public FirstStartDocumentsNative(string requestPin,string decisionPin) {
  foreach(string pin in new string[]{requestPin,decisionPin}){if(pin==null||pin.Length!=64)throw new Exception("DOCUMENT_PIN");foreach(char c in pin)if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')))throw new Exception("DOCUMENT_PIN");}pins[0]=requestPin;pins[1]=decisionPin;
  // Native context is eligibility only. Manual owner approval remains external.
  using(var impersonated=WindowsIdentity.GetCurrent(true)){if(impersonated!=null)throw new Exception("IMPERSONATION");}
  using(var id=WindowsIdentity.GetCurrent()){if(id.User.Value!=Actor||!new WindowsPrincipal(id).IsInRole(WindowsBuiltInRole.Administrator))throw new Exception("ACTOR");}
 }
 public void Prepare() {
  // Retained originals of every ancestor; no directory creation or ACL edits.
  string p=Path.GetPathRoot(Folder);OpenDirectory(p);
  foreach(string leaf in Folder.Substring(p.Length).Split('\\')){p=Path.Combine(p,leaf);OpenDirectory(p);}
  string[] names={"ONE-G3-P1-FINAL-PLAN-THREE-ACTS-REQUEST.CANDIDATE.md","OWNER-G3-P1-FINAL-PLAN-THREE-ACTS-APPROVAL.txt"};
  for(int i=0;i<2;i++) {
   sources[i]=CreateFileW(Path.Combine(Folder,names[i]),0x80120000,ShareRead,IntPtr.Zero,3,0x00200000,IntPtr.Zero);
   if(Bad(sources[i]))throw Error("SOURCE_OPEN");sourceIds[i]=Facts(sources[i],false);
   if(sourceIds[i].highSize!=0)throw new Exception("SOURCE_SIZE");bytes[i]=ReadExact(sources[i],sourceIds[i].lowSize);
   if(Hash(bytes[i])!=pins[i])throw new Exception("SOURCE_HASH");
  }
  if(sourceIds[0].volume==sourceIds[1].volume&&sourceIds[0].highId==sourceIds[1].highId&&sourceIds[0].lowId==sourceIds[1].lowId)throw new Exception("SOURCE_ALIAS");Check();
 }
 void OpenDirectory(string p) {
  uint rights=Read;if(String.Equals(p,Folder,StringComparison.OrdinalIgnoreCase))rights|=0x22; // target parent: ADD_FILE + TRAVERSE, never WRITE_DAC
  IntPtr h=CreateFileW(p,rights,ShareRead,IntPtr.Zero,3,BackupNoReparse,IntPtr.Zero);if(Bad(h))throw Error("ANCESTOR_OPEN");
  chain.Add(h);chainIds.Add(Facts(h,true));chainSd.Add(Hash(Descriptor(h)));
 }
 public bool Stop(){if(!Console.KeyAvailable)return false;return Console.ReadKey(true).Key==ConsoleKey.Escape;}
 public void Check() {
  if(chain.Count==0||chain.Count!=chainIds.Count||chain.Count!=chainSd.Count)throw new Exception("ANCESTOR_INCOMPLETE");
  for(int i=0;i<chain.Count;i++){Info f=Facts(chain[i],true),old=chainIds[i];if(f.volume!=old.volume||f.highId!=old.highId||f.lowId!=old.lowId||f.attributes!=old.attributes||chainSd[i]!=Hash(Descriptor(chain[i])))throw new Exception("ANCESTOR_CHANGED");}
  for(int i=0;i<2;i++)if(Bad(sources[i])||!Same(sourceIds[i],Facts(sources[i],false))||Hash(ReadExact(sources[i],sourceIds[i].lowSize))!=pins[i])throw new Exception("SOURCE_CHANGED");
 }
 public void Create(int i) {
  string[] leaves={"FIRST-G3-P1-FINAL-PLAN-SESSION-REQUEST.ORIGINAL.md","FIRST-G3-P1-FINAL-PLAN-OPERATIONAL-DECISION.ORIGINAL.txt"};
  if(i<0||i>1||!Bad(files[i]))throw new Exception("CREATE_STATE");byte[] sd=CreationDescriptor();
  IntPtr text=IntPtr.Zero,unicode=IntPtr.Zero,descriptor=IntPtr.Zero;
  try {
   text=Marshal.StringToHGlobalUni(leaves[i]);US u=new US();u.buffer=text;u.length=(ushort)(leaves[i].Length*2);u.max=u.length;
   unicode=Marshal.AllocHGlobal(Marshal.SizeOf(typeof(US)));Marshal.StructureToPtr(u,unicode,false);
   descriptor=Marshal.AllocHGlobal(sd.Length);Marshal.Copy(sd,0,descriptor,sd.Length);
   OA a=new OA();a.length=Marshal.SizeOf(typeof(OA));a.root=chain[chain.Count-1];a.name=unicode;a.attributes=0x1040;a.sd=descriptor;
   IOS io;int status=NtCreateFile(out files[i],0xc0120080,ref a,out io,IntPtr.Zero,0x80,ShareRead,2,0x00200060,IntPtr.Zero,0);
   if(status!=0||io.status!=IntPtr.Zero||io.information.ToInt64()!=2||Bad(files[i]))throw new Exception("EXCLUSIVE_CREATE_FAILED_OR_UNKNOWN");
   CreatedMask|=1U<<i;Evidence.Add("EXCLUSIVE_CREATE_CONFIRMED index="+i+"; PERSISTENT_UNTIL_RECONCILED");
   fileIds[i]=Facts(files[i],false);byte[] actual=Descriptor(files[i]);if(!ProtectedDocument(actual))throw new Exception("NEW_SD");fileSd[i]=Hash(actual);
   Evidence.Add("CREATED index="+i+" identity="+fileIds[i].volume+":"+fileIds[i].highId+":"+fileIds[i].lowId);
  } finally {if(text!=IntPtr.Zero)Marshal.FreeHGlobal(text);if(unicode!=IntPtr.Zero)Marshal.FreeHGlobal(unicode);if(descriptor!=IntPtr.Zero)Marshal.FreeHGlobal(descriptor);}
 }
 public void Write(int i){uint done;if(!WriteFile(files[i],bytes[i],(uint)bytes[i].Length,out done,IntPtr.Zero)||done!=bytes[i].Length)throw Error("WRITE_PARTIAL_NO_RETRY");written[i]=true;}
 public void Flush(int i){if(!FlushFileBuffers(files[i]))throw Error("FLUSH_UNKNOWN");flushed[i]=true;}
 public void Verify(int i) {
  Info f=Facts(files[i],false);byte[] sd=Descriptor(files[i]);
  // Size changes only by this adapter's single write; original ID remains fixed.
  if(f.volume!=fileIds[i].volume||f.highId!=fileIds[i].highId||f.lowId!=fileIds[i].lowId||!ProtectedDocument(sd)||Hash(sd)!=fileSd[i])throw new Exception("NEW_ID_SD_CHANGED");
  if(written[i]&&(f.highSize!=0||f.lowSize!=bytes[i].Length||Hash(ReadExact(files[i],f.lowSize))!=pins[i]))throw new Exception("READBACK_HASH");
  if(written[i]&&flushed[i])VerifiedMask|=1U<<i;
 }
 public bool Close() {
  bool known=!allocatorUncertain;for(int i=1;i>=0;i--){CloseOne(ref files[i],ref known);CloseOne(ref sources[i],ref known);}
  for(int i=chain.Count-1;i>=0;i--){IntPtr h=chain[i];CloseOne(ref h,ref known);chain[i]=h;}return known;
 }
 static void CloseOne(ref IntPtr h,ref bool known){if(Bad(h))return;if(CloseHandle(h))h=IntPtr.Zero;else known=false;}
}
