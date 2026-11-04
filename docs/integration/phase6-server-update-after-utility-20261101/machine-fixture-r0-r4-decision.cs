// Accepted F3/R4: ONE new decision copy; retain historical request DATA. No native constructor in models.
// Fixed single-decision specialization of the existing materialization adapter. No SCQ/SCM API.
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
   for(int i=0;i<1;i++) {
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
 public const string DecisionLeaf="MACHINE-TEST-ONLY-FIXTURE-R0-R4-DECISION.ORIGINAL.txt";
 public const string Actor="S-1-5-21-785699550-1440458426-1682014700-1108";
 public const string Folder=@"C:\Program Files\Papinho\Accelerator";
 public const string SourceFolder=@"C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101";
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
 [DllImport("ntdll.dll")]static extern int NtCreateFile(IntPtr handleMemory,uint rights,IntPtr oa,IntPtr io,IntPtr size,uint attrs,uint share,uint disposition,uint options,IntPtr ea,uint eaBytes);
 [DllImport("advapi32.dll")]static extern uint GetSecurityInfo(IntPtr h,uint type,uint what,out IntPtr owner,out IntPtr group,out IntPtr dacl,out IntPtr sacl,out IntPtr sd);
 [DllImport("advapi32.dll")]static extern uint GetSecurityDescriptorLength(IntPtr sd);
 [DllImport("kernel32.dll")]static extern IntPtr LocalFree(IntPtr p);
 readonly List<IntPtr> chain=new List<IntPtr>();readonly List<Info> chainIds=new List<Info>();readonly List<string> chainSd=new List<string>();
 readonly List<IntPtr> binaryHandles=new List<IntPtr>();readonly List<Info> binaryIds=new List<Info>();readonly List<string> binaryPins=new List<string>();
 readonly List<IntPtr> allocatorRetained=new List<IntPtr>();bool allocatorUncertain,closeAttempted;
 string originalDescriptorHash;
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
 public static bool ProtectedDirectory(byte[] b,bool anchor) {
  try {var d=new RawSecurityDescriptor(b,0);if(d.Owner==null||!Trusted(d.Owner.Value)||(d.ControlFlags&ControlFlags.DiscretionaryAclProtected)==0||d.DiscretionaryAcl==null||d.DiscretionaryAcl.Count==0)return false;
   const uint write=0x500d0156; // GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|file writes/delete-child
   foreach(GenericAce ga in d.DiscretionaryAcl){CommonAce a=ga as CommonAce;if(a==null||a.IsCallback||(a.AceType!=AceType.AccessAllowed&&a.AceType!=AceType.AccessDenied))return false;
    if((a.AceFlags&AceFlags.InheritOnly)!=0)continue;
    if(a.AceQualifier==AceQualifier.AccessAllowed&&((uint)a.AccessMask&write)!=0&&!Trusted(a.SecurityIdentifier.Value)){
     if(anchor&&a.AceFlags==AceFlags.None&&a.AccessMask==4&&a.SecurityIdentifier.Value=="S-1-5-11")continue;return false;
    }
   }return true;
  }catch{return false;}
 }
 static bool Trusted(string s){return s==Actor||s=="S-1-5-18"||s=="S-1-5-32-544"||s=="S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464";}
 public static bool ExpectedTarget(uint volume,uint high,uint low){return volume==417614100U&&high==8257536U&&low==71209U;}
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
  foreach(string pin in new string[]{requestPin,decisionPin}){if(pin==null||pin.Length!=64)throw new Exception("DOCUMENT_PIN");foreach(char c in pin)if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')))throw new Exception("DOCUMENT_PIN");}if(requestPin!="fc91b7bce523b014c28ce677b60ea889e32c8bc50c39c83b264096d6f923ab5c"||!FreshDecisionPin(decisionPin))throw new Exception("ORIGINAL_SCOPE");pins[0]=decisionPin;pins[1]=requestPin;
  // Native context is eligibility only. Manual owner approval remains external.
  using(var impersonated=WindowsIdentity.GetCurrent(true)){if(impersonated!=null)throw new Exception("IMPERSONATION");}
  using(var id=WindowsIdentity.GetCurrent()){if(id.User.Value!=Actor||!new WindowsPrincipal(id).IsInRole(WindowsBuiltInRole.Administrator))throw new Exception("ACTOR");}
 }
 public void Prepare() {
  // Retained originals of every ancestor; no directory creation or ACL edits.
  string p=Path.GetPathRoot(Folder);OpenDirectory(p);
  foreach(string leaf in Folder.Substring(p.Length).Split('\\')){p=Path.Combine(p,leaf);OpenDirectory(p);}
  OpenBinaries();
  string[] names={"OWNER-MACHINE-FIXTURE-R0-R4-PREPARATION-APPROVAL.txt",Path.Combine(Folder,"MACHINE-TEST-ONLY-FIXTURE-REQUEST.ORIGINAL.md")};
  for(int i=0;i<2;i++) {
   sources[i]=CreateFileW(Path.Combine(SourceFolder,names[i]),0x80120000,ShareRead,IntPtr.Zero,3,0x00200000,IntPtr.Zero);
   if(Bad(sources[i]))throw Error("SOURCE_OPEN");sourceIds[i]=Facts(sources[i],false);
   if(sourceIds[i].highSize!=0)throw new Exception("SOURCE_SIZE");bytes[i]=ReadExact(sources[i],sourceIds[i].lowSize);
   if(Hash(bytes[i])!=pins[i])throw new Exception("SOURCE_HASH");
  }
  if(sourceIds[0].volume==sourceIds[1].volume&&sourceIds[0].highId==sourceIds[1].highId&&sourceIds[0].lowId==sourceIds[1].lowId)throw new Exception("SOURCE_ALIAS");Check();
 }
 void OpenDirectory(string p) {
  uint rights=Read;if(String.Equals(p,Folder,StringComparison.OrdinalIgnoreCase))rights|=0x22; // target parent: ADD_FILE + TRAVERSE, never WRITE_DAC
  IntPtr h=CreateFileW(p,rights,ShareRead,IntPtr.Zero,3,BackupNoReparse,IntPtr.Zero);if(Bad(h))throw Error("ANCESTOR_OPEN");
  chain.Add(h);Info info=Facts(h,true);byte[] sd=Descriptor(h);
  if(!ProtectedDirectory(sd,chain.Count==1))throw new Exception("ANCESTOR_PROTECTION");
  if(String.Equals(p,Folder,StringComparison.OrdinalIgnoreCase)&&!ExpectedTarget(info.volume,info.highId,info.lowId))throw new Exception("PARENT_ID");
  if(String.Equals(p,@"C:\Program Files",StringComparison.OrdinalIgnoreCase)&&(info.volume!=417614100U||info.highId!=131072U||info.lowId!=226316U))throw new Exception("ANCHOR_ID");
  chainIds.Add(info);chainSd.Add(Hash(sd));
 }
 public static bool NativeLayouts(){return IntPtr.Size==8&&Marshal.SizeOf(typeof(OA))==48&&Marshal.SizeOf(typeof(IOS))==16&&Marshal.SizeOf(typeof(US))==16&&Marshal.SizeOf(typeof(Info))==52&&Marshal.OffsetOf(typeof(OA),"root").ToInt32()==8&&Marshal.OffsetOf(typeof(US),"buffer").ToInt32()==8;}
 public static bool ExpectedExecutor(IntPtr h){Info z=Facts(h,false);return z.volume==417614100U&&z.highId==10223616U&&z.lowId==73344U&&z.highSize==0&&z.lowSize==871936U;}
 void OpenBinaries(){
  string[] names={"papacc_server.exe","libssl-3-x64.dll","libcrypto-3-x64.dll"};
  uint[] sizes={871936,1321472,7217664};string[] hashes={"a6b8ee1ac5ef3e7b6908686f3690ce2a6ca6c4171812a201b7d3024ea6f2ab8a","3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed","09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044"};
  for(int i=0;i<3;i++){IntPtr h=CreateFileW(Path.Combine(Folder,names[i]),0x80120000,ShareRead,IntPtr.Zero,3,0x00200000,IntPtr.Zero);if(Bad(h))throw Error("BINARY_OPEN");binaryHandles.Add(h);Info z=Facts(h,false);binaryIds.Add(z);binaryPins.Add(hashes[i]);
   if(z.highSize!=0||z.lowSize!=sizes[i]||(i==0&&!ExpectedExecutor(h))||!ProtectedDirectory(Descriptor(h),false)||HashBinary(h,z.lowSize)!=hashes[i])throw new Exception("BINARY_BINDING");
  }
 }
 static string HashBinary(IntPtr handle,uint size){
  long pos;if(!SetFilePointerEx(handle,0,out pos,0))throw Error("BINARY_SEEK");byte[] block=new byte[4096];uint read,total=0;
  using(var sha=SHA256.Create()){while(total<size){uint wanted=Math.Min(4096U,size-total);if(!ReadFile(handle,block,wanted,out read,IntPtr.Zero)||read==0||read>wanted)throw Error("BINARY_READ");sha.TransformBlock(block,0,(int)read,null,0);total+=read;}sha.TransformFinalBlock(new byte[0],0,0);return BitConverter.ToString(sha.Hash).Replace("-","").ToLowerInvariant();}
 }
 public bool Stop(){if(!Console.KeyAvailable)return false;return Console.ReadKey(true).Key==ConsoleKey.Escape;}
 public static bool FreshDecisionPin(string pin){return pin!=null&&pin.Length==64&&pin!="6ee864d0ec398d8fbd966822b71ea329bf908d1af2bc589f1298c22368f8a9bf"&&pin!="fff38e9245bd93f0ff271f847a9064460ae9ba79bb0ad62387bcd6a23360feb7"&&pin!="e2e47dc1062f26b5952bddc68e720426210d66bb9148419b06562bb1f90118f3"&&pin!="f6347e98c020a0a705e534f4c3304700c9bb3bc457db6259a1794197ec36d021";}
 public static bool HistoricalRequest(uint volume,uint high,uint low){return volume==417614100U&&high==6815744U&&low==9313U;}
 public void Check() {
  if(chain.Count==0||chain.Count!=chainIds.Count||chain.Count!=chainSd.Count)throw new Exception("ANCESTOR_INCOMPLETE");
  for(int i=0;i<chain.Count;i++){Info f=Facts(chain[i],true),old=chainIds[i];byte[] sd=Descriptor(chain[i]);if(!ProtectedDirectory(sd,i==0))throw new Exception("ANCESTOR_PROTECTION");if(f.volume!=old.volume||f.highId!=old.highId||f.lowId!=old.lowId||f.attributes!=old.attributes||chainSd[i]!=Hash(sd))throw new Exception("ANCESTOR_CHANGED");}
  for(int i=0;i<binaryHandles.Count;i++)if(!Same(binaryIds[i],Facts(binaryHandles[i],false))||!ProtectedDirectory(Descriptor(binaryHandles[i]),false)||HashBinary(binaryHandles[i],binaryIds[i].lowSize)!=binaryPins[i])throw new Exception("BINARY_CHANGED");
  for(int i=0;i<2;i++)if(Bad(sources[i])||!Same(sourceIds[i],Facts(sources[i],false))||Hash(ReadExact(sources[i],sourceIds[i].lowSize))!=pins[i])throw new Exception("SOURCE_CHANGED");
  if(!HistoricalRequest(sourceIds[1].volume,sourceIds[1].highId,sourceIds[1].lowId))throw new Exception("ORIGINAL_REQUEST_ID");
  byte[] requestSd=Descriptor(sources[1]);if(!ProtectedDocument(requestSd))throw new Exception("ORIGINAL_REQUEST_SD");
  string requestSdHash=Hash(requestSd);if(originalDescriptorHash==null)originalDescriptorHash=requestSdHash;else if(originalDescriptorHash!=requestSdHash)throw new Exception("ORIGINAL_REQUEST_SD_CHANGED");
 }
 public void Create(int i) {
  string[] leaves={DecisionLeaf};
  if(i!=0||!Bad(files[i]))throw new Exception("CREATE_STATE");byte[] sd=CreationDescriptor();
  IntPtr text=IntPtr.Zero,unicode=IntPtr.Zero,descriptor=IntPtr.Zero,oaMemory=IntPtr.Zero,ioMemory=IntPtr.Zero,handleMemory=IntPtr.Zero;bool pending=false;
  try {
   text=Marshal.StringToHGlobalUni(leaves[i]);US u=new US();u.buffer=text;u.length=(ushort)(leaves[i].Length*2);u.max=u.length;
   unicode=Marshal.AllocHGlobal(Marshal.SizeOf(typeof(US)));Marshal.StructureToPtr(u,unicode,false);
   descriptor=Marshal.AllocHGlobal(sd.Length);Marshal.Copy(sd,0,descriptor,sd.Length);
   OA a=new OA();a.length=Marshal.SizeOf(typeof(OA));a.root=chain[chain.Count-1];a.name=unicode;a.attributes=0x1040;a.sd=descriptor;
   oaMemory=Marshal.AllocHGlobal(Marshal.SizeOf(typeof(OA)));Marshal.StructureToPtr(a,oaMemory,false);
   ioMemory=Marshal.AllocHGlobal(Marshal.SizeOf(typeof(IOS)));Marshal.StructureToPtr(new IOS(),ioMemory,false);
   handleMemory=Marshal.AllocHGlobal(IntPtr.Size);Marshal.WriteIntPtr(handleMemory,IntPtr.Zero);
   int status=NtCreateFile(handleMemory,0xc0120080,oaMemory,ioMemory,IntPtr.Zero,0x80,ShareRead,2,0x00200060,IntPtr.Zero,0);files[i]=Marshal.ReadIntPtr(handleMemory);IOS io=(IOS)Marshal.PtrToStructure(ioMemory,typeof(IOS));
   if(status>0||io.status.ToInt64()==0x103){pending=true;allocatorUncertain=true;}
   if(status!=0||io.status!=IntPtr.Zero||io.information.ToInt64()!=2||Bad(files[i]))throw new Exception("EXCLUSIVE_CREATE_FAILED_OR_UNKNOWN");
   CreatedMask|=1U<<i;Evidence.Add("EXCLUSIVE_CREATE_CONFIRMED index="+i+"; PERSISTENT_UNTIL_RECONCILED");
   fileIds[i]=Facts(files[i],false);byte[] actual=Descriptor(files[i]);if(!ProtectedDocument(actual))throw new Exception("NEW_SD");fileSd[i]=Hash(actual);
   Evidence.Add("CREATED index="+i+" identity="+fileIds[i].volume+":"+fileIds[i].highId+":"+fileIds[i].lowId);
  } finally {if(pending){foreach(IntPtr q in new IntPtr[]{text,unicode,descriptor,oaMemory,ioMemory,handleMemory})if(q!=IntPtr.Zero)allocatorRetained.Add(q);}else{if(text!=IntPtr.Zero)Marshal.FreeHGlobal(text);if(unicode!=IntPtr.Zero)Marshal.FreeHGlobal(unicode);if(descriptor!=IntPtr.Zero)Marshal.FreeHGlobal(descriptor);if(oaMemory!=IntPtr.Zero)Marshal.FreeHGlobal(oaMemory);if(ioMemory!=IntPtr.Zero)Marshal.FreeHGlobal(ioMemory);if(handleMemory!=IntPtr.Zero)Marshal.FreeHGlobal(handleMemory);}}
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
  if(closeAttempted)return false;closeAttempted=true;if(allocatorUncertain)return false;bool known=true;for(int i=1;i>=0;i--){CloseOne(ref files[i],ref known);CloseOne(ref sources[i],ref known);}
  for(int i=binaryHandles.Count-1;i>=0;i--){IntPtr h=binaryHandles[i];CloseOne(ref h,ref known);binaryHandles[i]=h;}
  for(int i=chain.Count-1;i>=0;i--){IntPtr h=chain[i];CloseOne(ref h,ref known);chain[i]=h;}return known;
 }
 static void CloseOne(ref IntPtr h,ref bool known){if(Bad(h))return;if(CloseHandle(h))h=IntPtr.Zero;else known=false;}
}
