// Manual TWO-FILE engineering placement only. No Accelerator admission authority.
// Production/admission pins, SCM and ACLs of existing objects are never modified.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using System.Text;
using System.Security.Principal;

public interface IPaccN2PlacementIO {
 bool BeginOnce(); bool Stop(); void Check(); void Create(int i); void Verify(int i);
 void Write(int i); void Flush(int i); void Readback(int i); bool Close();
}
public static class PaccN2PlacementSteps {
 public static string Run(IPaccN2PlacementIO io) {
  if(!io.BeginOnce())return "REPLAY_DENY";
  string first="NONE";
  try {
   for(int i=0;i<2;i++) {
    Guard(io); io.Check(); Guard(io); io.Create(i);
    Guard(io); io.Verify(i); Guard(io); io.Write(i);
    Guard(io); io.Flush(i); Guard(io); io.Readback(i);
    Guard(io); io.Verify(i); io.Check();
   }
  } catch(Exception e) { first=Describe(e); }
  try {if(!io.Close() && first=="NONE")first="CLOSE_UNKNOWN";} catch(Exception e) {if(first=="NONE")first=Describe(e);}
  return first;
 }
 public static string Describe(Exception e) {
  while(e.InnerException!=null)e=e.InnerException;
  var native=e as Win32Exception;
  return native==null?e.Message:e.Message+" WIN32="+native.NativeErrorCode;
 }
 static void Guard(IPaccN2PlacementIO io) {if(io.Stop())throw new Exception("STOP_REQUESTED");}
}
public sealed class PaccN2PlacementNative : IPaccN2PlacementIO {
 const string Root=@"C:\Projetos\PapinhoAccelerator";
 const string Parent=@"C:\ProgramData\PapinhoAccelerator\Protected\N0";
 const string Op="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string SdHash="0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814";
 const string ParentHash="688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579";
 static readonly string[] Leaves={"papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence","papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved"};
 static readonly string[] Sources={"windows-origin-evidence.PREPARED.txt","OPENED-WINDOWS-FILE.PREPARED.txt"};
 static readonly string[] Hashes={"babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0","5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294"};
 static readonly int[] Sizes={5358,326};
 [StructLayout(LayoutKind.Sequential)] struct Info {public uint attr; public System.Runtime.InteropServices.ComTypes.FILETIME c,a,w; public uint vol,sh,sl,links,hi,lo;}
 [StructLayout(LayoutKind.Sequential)] struct SA {public int length;public IntPtr sd;[MarshalAs(UnmanagedType.Bool)]public bool inherit;}
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern IntPtr CreateFileW(string p,uint a,uint s,IntPtr sa,uint d,uint f,IntPtr t);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern IntPtr CreateFileW(string p,uint a,uint s,ref SA sa,uint d,uint f,IntPtr t);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetFileInformationByHandle(IntPtr h,out Info i);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern uint GetFinalPathNameByHandleW(IntPtr h,StringBuilder p,uint n,uint flags);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetHandleInformation(IntPtr h,out uint flags);
 [DllImport("kernel32.dll",SetLastError=true)]static extern uint GetFileType(IntPtr h);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern uint GetFileAttributesW(string path);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool WriteFile(IntPtr h,byte[] b,uint n,out uint written,IntPtr o);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool ReadFile(IntPtr h,byte[] b,uint n,out uint read,IntPtr o);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool FlushFileBuffers(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool SetFilePointerEx(IntPtr h,long x,out long next,uint mode);
 [DllImport("advapi32.dll")]static extern bool IsValidSecurityDescriptor(IntPtr p);
 [DllImport("advapi32.dll")]static extern bool GetSecurityDescriptorControl(IntPtr p,out ushort control,out uint revision);
 [DllImport("advapi32.dll",SetLastError=true)]static extern bool GetTokenInformation(IntPtr h,int cls,byte[] data,uint size,out uint returned);
 readonly List<IntPtr> owned=new List<IntPtr>();
 readonly List<string> dirPaths=new List<string>();readonly List<IntPtr> dirs=new List<IntPtr>();
 readonly List<string> dirIds=new List<string>();
 readonly List<IntPtr> sources=new List<IntPtr>();readonly List<string> sourceIds=new List<string>();
 readonly IntPtr[] files=new IntPtr[2];readonly string[] ids=new string[2];readonly byte[][] sds=new byte[2][];
 readonly byte[][] data=new byte[2][];byte[] creation;
 readonly List<IntPtr> pinFiles=new List<IntPtr>();readonly List<string> pinIds=new List<string>();readonly List<string> pinHashes=new List<string>();
 byte[] tokenStatistics;
 bool used; public bool BeginOnce(){if(used)return false;used=true;return true;}
 public bool Retained;public readonly List<string> Evidence=new List<string>();
 static Exception Err(string stage) {return new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 IntPtr Open(string path,bool directory) {
  IntPtr h=CreateFileW(path,directory?0x20080u:0x80120000u,1,IntPtr.Zero,3,directory?0x02200000u:0x00200000u,IntPtr.Zero);
  if(h==new IntPtr(-1))throw Err("OPEN "+path);owned.Add(h);return h;
 }
 static Info Facts(IntPtr h,bool directory) {
  Info i;uint flags;if(!GetFileInformationByHandle(h,out i)||!GetHandleInformation(h,out flags))throw Err("FILE_INFORMATION");if((flags&1)!=0)throw new Exception("INHERITABLE_HANDLE_DENY");
  if((i.attr&0x400)!=0 || ((i.attr&16)!=0)!=directory || GetFileType(h)!=1)throw new Exception("TYPE_REPARSE");return i;
 }
 static string Id(Info i) {return i.vol+":"+i.hi+":"+i.lo;}
 static void PathIs(IntPtr h,string expected) {
  var b=new StringBuilder(1100);uint n=GetFinalPathNameByHandleW(h,b,1100,0);
  if(n<4||n>=1100||!String.Equals(b.ToString(),@"\\?\"+expected,StringComparison.OrdinalIgnoreCase))throw new Exception("FINAL_PATH_MISMATCH");
 }
 static byte[] ReadExact(IntPtr h,int size) {
  long pos;if(!SetFilePointerEx(h,0,out pos,0)||pos!=0)throw Err("SEEK");
  byte[] b=new byte[size];uint n;if(!ReadFile(h,b,(uint)size,out n,IntPtr.Zero))throw Err("READ");if(n!=size)throw new Exception("SHORT_READ");return b;
 }
 static bool Equal(byte[] a,byte[] b) {if(a.Length!=b.Length)return false;for(int i=0;i<a.Length;i++)if(a[i]!=b[i])return false;return true;}
 public static bool SafeDescriptor(byte[] b) {
  try {
   if(b==null||b.Length<20||b.Length>65536)return false;
   // Bounds-checked decode before passing a supplied managed buffer to Win32.
   var d=new RawSecurityDescriptor(b,0);
   var pin=GCHandle.Alloc(b,GCHandleType.Pinned);
   try {ushort c;uint rev;if(!IsValidSecurityDescriptor(pin.AddrOfPinnedObject())||!GetSecurityDescriptorControl(pin.AddrOfPinnedObject(),out c,out rev)||rev!=1||(c!=0x9004&&c!=0x9404))return false;}finally{pin.Free();}
   if(d.Owner==null||d.Owner.Value!=Op||d.Group==null||d.Group.Value!="S-1-5-32-544"||d.DiscretionaryAcl==null||d.DiscretionaryAcl.Count!=3)return false;
   string[] sid={"S-1-5-18","S-1-5-32-544",Op};int[] mask={0x1f01ff,0x1f01ff,0x12019f};
   for(int i=0;i<3;i++){var a=d.DiscretionaryAcl[i] as CommonAce;if(a==null||a.IsCallback||a.AceType!=AceType.AccessAllowed||a.AceFlags!=AceFlags.None||a.SecurityIdentifier.Value!=sid[i]||a.AccessMask!=mask[i])return false;}
   return true;
  }catch{return false;}
 }
 void Operator() {
  using(var thread=WindowsIdentity.GetCurrent(true)){if(thread!=null)throw new Exception("IMPERSONATION_DENY");}
  using(var id=WindowsIdentity.GetCurrent()) {
   if(id.User==null||id.User.Value!=Op||!new WindowsPrincipal(id).IsInRole(WindowsBuiltInRole.Administrator))throw new Exception("OPERATOR_DENY");
   uint n;byte[] elevation=new byte[4],stat=new byte[56];
   if(!GetTokenInformation(id.Token,20,elevation,4,out n)||n!=4||BitConverter.ToUInt32(elevation,0)!=1)throw new Exception("ELEVATION_DENY");
   if(!GetTokenInformation(id.Token,10,stat,56,out n)||n!=56)throw new Exception("TOKEN_STATISTICS_DENY");
   if(tokenStatistics!=null&&!Equal(tokenStatistics,stat))throw new Exception("TOKEN_CHANGED");tokenStatistics=stat;
  }
 }
 void Pin(string path,string hash) {
  IntPtr h=Open(path,false);Info f=Facts(h,false);if(f.links!=1||PaccFileAcl.FileDigest(h)!=hash)throw new Exception("BOUND_INPUT_PIN "+path);
  pinFiles.Add(h);pinIds.Add(Id(f));pinHashes.Add(hash);
 }
 public string VerifyRootReadOnly() {
  string first="NONE";
  try {
   Operator();
   if(dirs.Count!=0)throw new Exception("ROOT_QUERY_REPLAY_DENY");
   var stack=new Stack<string>();string p=Parent;
   while(p!=null){stack.Push(p);p=Directory.GetParent(p)==null?null:Directory.GetParent(p).FullName;}
   while(stack.Count>0){p=stack.Pop();IntPtr h=Open(p,true);Info f=Facts(h,true);PathIs(h,p);dirPaths.Add(p);dirs.Add(h);dirIds.Add(Id(f));}
   CheckDirectories();
   for(int i=0;i<dirs.Count;i++)Evidence.Add("ROOT_READONLY_VERIFIED "+dirPaths[i]+" native="+dirIds[i]+" sha256="+PaccFileAcl.Digest(PaccFileAcl.Read(dirs[i])));
  }catch(Exception e){first=PaccN2PlacementSteps.Describe(e);}
  try{if(!Close()&&first=="NONE")first="CLOSE_UNKNOWN";}catch(Exception e){Retained=true;if(first=="NONE")first=PaccN2PlacementSteps.Describe(e);}
  return first;
 }
 public void Prepare() {
  Operator();
  // Lock every path component against writes/rename/delete before effects.
  // Sharing conflict anywhere stops; never weaken flags to get past it.
  var stack=new Stack<string>();string p=Parent;
  while(p!=null){stack.Push(p);p=Directory.GetParent(p)==null?null:Directory.GetParent(p).FullName;}
  while(stack.Count>0){p=stack.Pop();IntPtr h=Open(p,true);Info f=Facts(h,true);PathIs(h,p);dirPaths.Add(p);dirs.Add(h);dirIds.Add(Id(f));}
  if(dirs.Count!=5)throw new Exception("ANCESTOR_COUNT");
  CheckDirectories();
  Pin(Root+@"\docs\integration\n0-protected-root-creator-implementation-20261024\manual-N0R-20261024-01.txt", "52c87ef41db38971f30f62912211abd9483aee2896ec9d3c5df3d19dace6afc1");
  string sdPath=Root+@"\docs\integration\n0-n9-protected-input-placement-20261024\proposed-new-file-creation.raw";
  IntPtr sh=Open(sdPath,false);Info si=Facts(sh,false);if(si.sh!=0||si.sl!=152||si.links!=1)throw new Exception("CREATION_SD_FILE");
  creation=ReadExact(sh,152);if(PaccFileAcl.Digest(creation)!=SdHash||!SafeDescriptor(creation))throw new Exception("CREATION_SD_PIN");
  for(int i=0;i<2;i++){
   string path=Root+@"\docs\integration\n0-n9-protected-dossier-placement-20261024\"+Sources[i];IntPtr h=Open(path,false);Info f=Facts(h,false);
   if(f.sh!=0||f.sl!=Sizes[i]||f.links!=1)throw new Exception("SOURCE_SIZE_LINK");data[i]=ReadExact(h,Sizes[i]);
   if(PaccFileAcl.Digest(data[i])!=Hashes[i])throw new Exception("SOURCE_HASH");sources.Add(h);sourceIds.Add(Id(f));
  }
  Pin(Root+@"\build\scm-protected-installation-binding-20261022\Selected\Release\papacc.conf","a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44");
  Pin(Root+@"\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe","eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b");
  Pin(Root+@"\docs\adr\DECISION-first-native-lab-diagnostic-boundary-20261024.md","bec9d0c275d5ee8bd68de4d42fe173a08f124a9900c119e7a08d23757d9d7941");
  Pin(Root+@"\docs\adr\DECISION-canonical-lab-console-topology-bd-20261023.md","eb93002cf3f6d863a060fb97b7380389aa66b6fbe68e66c1af4fb17b988d3290");
  Pin(Root+@"\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md","e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7");
  Pin(Root+@"\docs\adr\DECISION-n0-n9-security-descriptor-digest-profile-20261024.md","8b042e2bbf6500aa90aa3407fcd3423f45d96cf6b16568ffa95b98f3d081c972");
  for(int i=0;i<2;i++) {
   if(GetFileAttributesW(Parent+"\\"+Leaves[i])!=0xffffffffu)throw new Exception("PREEXISTING_INPUT_NO_EFFECT");
   if(Marshal.GetLastWin32Error()!=2)throw Err("ABSENCE_UNPROVED");
  }
  Check();
 }
 bool stopped;
 public bool Stop() {if(stopped)return true;if(!Console.KeyAvailable)return false;stopped=Console.ReadKey(true).Key==ConsoleKey.Escape;return stopped;}
 public static void DirectoryPin(int index,string identity,byte[] raw) {
  string[] expected={"417614100:327680:5","417614100:131072:227641","417614100:786432:1087874","417614100:851968:2672648","417614100:720896:2672656"};
  string[] hashes={"9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8","c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6",ParentHash,ParentHash,ParentHash};
  if(index<0||index>=5||identity!=expected[index]||PaccFileAcl.Digest(raw)!=hashes[index])throw new Exception("ANCESTOR_ID_OR_RAW_DENY");
  if(index>=2&&(raw.Length!=104||BitConverter.ToUInt16(raw,2)!=0x9004))throw new Exception("PROTECTED_ANCESTOR_PROFILE");
 }
 void CheckDirectories() {
  if(dirs.Count!=5)throw new Exception("ANCESTOR_COUNT");
  for(int i=0;i<dirs.Count;i++){
   string current=Id(Facts(dirs[i],true));if(current!=dirIds[i])throw new Exception("DIRECTORY_REPLACED");PathIs(dirs[i],dirPaths[i]);
   byte[] raw=PaccFileAcl.Read(dirs[i]);DirectoryPin(i,current,raw);
  }
 }
 public void Check() {
  Operator();
  CheckDirectories();
  if(PaccFileAcl.Digest(PaccFileAcl.Read(dirs[dirs.Count-1]))!=ParentHash)throw new Exception("PARENT_RAW_PREIMAGE_CHANGED");
  for(int i=0;i<sources.Count;i++){Info f=Facts(sources[i],false);if(Id(f)!=sourceIds[i]||f.sh!=0||f.sl!=Sizes[i]||f.links!=1||PaccFileAcl.Digest(ReadExact(sources[i],Sizes[i]))!=Hashes[i])throw new Exception("SOURCE_REPLACED");}
  for(int i=0;i<pinFiles.Count;i++){if(Id(Facts(pinFiles[i],false))!=pinIds[i]||PaccFileAcl.FileDigest(pinFiles[i])!=pinHashes[i])throw new Exception("BOUND_INPUT_CHANGED");}
 }
 public void Create(int i) {
  Check();
  if(Stop())throw new Exception("STOP_REQUESTED");
  var pin=GCHandle.Alloc(creation,GCHandleType.Pinned);
  try {var sa=new SA();sa.length=Marshal.SizeOf(typeof(SA));sa.sd=pin.AddrOfPinnedObject();sa.inherit=false;
   IntPtr h=CreateFileW(Parent+"\\"+Leaves[i],0xc0020000u,0,ref sa,1,0x00200080u,IntPtr.Zero);
   if(h==new IntPtr(-1))throw Err("CREATE_NEW "+Leaves[i]);files[i]=h;owned.Add(h);
   // Log effect immediately; zero-length/failed validation is still a created file.
   Evidence.Add("CREATED "+Leaves[i]);Info f=Facts(h,false);ids[i]=Id(f);Evidence.Add("NATIVE "+Leaves[i]+" "+ids[i]);
  }finally{pin.Free();}
 }
 public void Verify(int i) {
  Info f=Facts(files[i],false);if(Id(f)!=ids[i]||f.vol!=417614100||f.links!=1)throw new Exception("CHILD_IDENTITY_LINK");PathIs(files[i],Parent+"\\"+Leaves[i]);
  if(sds[i]==null&&(f.sh!=0||f.sl!=0))throw new Exception("NEW_FILE_NOT_EMPTY");
  byte[] sd=PaccFileAcl.Read(files[i]);if(!SafeDescriptor(sd))throw new Exception("CHILD_OWNER_DACL");
  if(sds[i]!=null&&!Equal(sd,sds[i]))throw new Exception("CHILD_RAW_SD_CHANGED");sds[i]=sd;
  Evidence.Add("CHILD_SD "+Leaves[i]+" "+PaccFileAcl.Digest(sd)+" RAW="+Convert.ToBase64String(sd));
 }
 public void Write(int i) {uint n;if(!WriteFile(files[i],data[i],(uint)data[i].Length,out n,IntPtr.Zero))throw Err("WRITE");if(n!=data[i].Length)throw new Exception("PARTIAL_WRITE_NO_RETRY");}
 public void Flush(int i) {if(!FlushFileBuffers(files[i]))throw Err("FLUSH_UNKNOWN");}
 public void Readback(int i) {Info f=Facts(files[i],false);if(f.sh!=0||f.sl!=Sizes[i]||PaccFileAcl.Digest(ReadExact(files[i],Sizes[i]))!=Hashes[i])throw new Exception("SIZE_OR_DIGEST");Evidence.Add("VERIFIED "+Leaves[i]+" "+Hashes[i]);}
 public bool Close() {
  if(PaccFileAcl.DescriptorRetained){Retained=true;return false;}
  for(int i=owned.Count-1;i>=0;i--){if(!PaccFileAcl.CloseHandle(owned[i])){int error=Marshal.GetLastWin32Error();Retained=true;Evidence.Add("CLOSE_UNKNOWN WIN32="+error);return false;}owned.RemoveAt(i);}
  Evidence.Add("OWNED_REFERENCES_CLOSED");return true;
 }
}
