using System;using System.IO;using System.Text;using System.Collections.Generic;using System.Security.Principal;using System.Security.Cryptography;using System.Security.Cryptography.X509Certificates;using System.Runtime.InteropServices;using System.Web.Script.Serialization;using System.Diagnostics;using System.Diagnostics.Eventing.Reader;using System.Xml;
// Native mode is reachable ONLY with exact owner approval/request/executable pins and manual confirmation.
// Default --selfcheck never opens TBS. No NV, hierarchy, key creation, persistent object or signing command exists.
class NativeProofHost {
 [StructLayout(LayoutKind.Sequential)]struct Params {public uint version,flags;}
 [DllImport("tbs.dll",ExactSpelling=true)]static extern uint Tbsi_Context_Create(ref Params p,out IntPtr h);
 [DllImport("tbs.dll",ExactSpelling=true)]static extern uint Tbsip_Submit_Command(IntPtr h,uint locality,uint priority,byte[] command,uint n,byte[] response,ref uint size);
 [DllImport("tbs.dll",ExactSpelling=true)]static extern uint Tbsip_Context_Close(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetProcessHandleCount(IntPtr p,out uint n);
 static bool observerQueryAttempted,observerQuerySucceeded;static uint observerOwnCount;static int observerQueryError;
 static SaltedSessionPlan retainedPlan;static byte[] initialToken;static string liveBinding;static IntPtr parentReference,consoleReference;static bool observerUnknown;static uint parentPidPin,consolePidPin;static long parentCreation,consoleCreation;static readonly List<string> adminSamples=new List<string>();static readonly List<uint> handleSamples=new List<uint>();static IntPtr retainedContext;static string stage="OWNER_METADATA",firstStage="NONE",first="NONE";static int observerCloseError;static bool contextOpenAttempted,contextOpened,sessionAttempted,sessionEnded;static int calls;static string folder=AppDomain.CurrentDomain.BaseDirectory;
 [DllImport("advapi32.dll",SetLastError=true)]static extern bool GetTokenInformation(IntPtr h,int cls,byte[] data,uint size,out uint used);
 static byte[] TokenState(){using(WindowsIdentity id=WindowsIdentity.GetCurrent()){
  if(id.User.Value!="S-1-5-21-785699550-1440458426-1682014700-1108"||!(new WindowsPrincipal(id)).IsInRole(WindowsBuiltInRole.Administrator))throw new InvalidOperationException("TOKEN_DENY");
  WindowsIdentity thread=WindowsIdentity.GetCurrent(true);if(thread!=null){thread.Dispose();throw new InvalidOperationException("IMPERSONATION_DENY");}
  byte[] data=new byte[56];uint used;if(!GetTokenInformation(id.Token,10,data,56,out used)||used!=56)throw new InvalidOperationException("TOKEN_STATISTICS_UNKNOWN");return Credential.Cat(SaltedWire.Slice(data,0,16),SaltedWire.Slice(data,48,8));}}
 static string BootBinding(){
  if(Hash(Encoding.UTF8.GetBytes(Environment.MachineName))!="fc2d00eb95bdc15ee7f56c7abd78f1c88670ba8359a0786ab4b49523fd91f482")throw new InvalidOperationException("HOST_METADATA_DENY");
  EventLogQuery q=new EventLogQuery("System",PathType.LogName,"*[System[Provider[@Name='Microsoft-Windows-Kernel-General'] and EventID=12]]");q.ReverseDirection=true;
  using(EventLogReader reader=new EventLogReader(q))using(EventRecord record=reader.ReadEvent()){
   if(record==null||record.RecordId!=237900)throw new InvalidOperationException("BOOT_DENY");XmlDocument x=new XmlDocument();x.LoadXml(record.ToXml());XmlNode computer=x.SelectSingleNode("/*[local-name()='Event']/*[local-name()='System']/*[local-name()='Computer']"),start=x.SelectSingleNode("/*[local-name()='Event']/*[local-name()='EventData']/*[local-name()='Data'][@Name='StartTime']");
   if(computer==null||start==null||Hash(Encoding.UTF8.GetBytes(computer.InnerText))!="2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff"||DateTimeOffset.Parse(start.InnerText).UtcDateTime.ToString("o")!="2026-10-26T21:35:16.5000000Z")throw new InvalidOperationException("BOOT_METADATA_DENY");
   using(Process process=Process.GetCurrentProcess())return record.RecordId+"|"+process.Id+"|"+process.StartTime.ToUniversalTime().ToFileTimeUtc();
  }
 }
 [DllImport("ntdll.dll")]static extern int NtQueryInformationProcess(IntPtr h,int cls,byte[] b,int n,out int used);
 [DllImport("kernel32.dll")]static extern uint GetCurrentProcessId();
 [DllImport("kernel32.dll")]static extern IntPtr GetConsoleWindow();
 [DllImport("user32.dll")]static extern uint GetWindowThreadProcessId(IntPtr hwnd,out uint pid);
 [DllImport("kernel32.dll")]static extern uint GetConsoleProcessList(uint[] ids,uint n);
 [DllImport("kernel32.dll",SetLastError=true)]static extern IntPtr OpenProcess(uint access,bool inherit,uint pid);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetProcessTimes(IntPtr h,out long created,out long exited,out long kernel,out long user);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool CloseHandle(IntPtr h);
 static void AdministrativeSample(){
  observerQueryAttempted=true;uint own;bool queried=GetProcessHandleCount(new IntPtr(-1),out own);
  observerQueryError=queried?0:Marshal.GetLastWin32Error();observerQuerySucceeded=queried;observerOwnCount=own;
  if(!queried)Fail("NATIVE_OBSERVER_QUERY_FAILED");
  if(own>124)Fail("NATIVE_OBSERVER_HEADROOM");
  byte[] basic=new byte[48];int used;if(NtQueryInformationProcess(new IntPtr(-1),0,basic,48,out used)!=0||used!=48)Fail("PARENT_UNKNOWN");long parent=BitConverter.ToInt64(basic,40);if(parent<=0||parent>uint.MaxValue)Fail("PARENT_UNKNOWN");
  uint aux=0;if(GetConsoleWindow()==IntPtr.Zero||GetWindowThreadProcessId(GetConsoleWindow(),out aux)==0)Fail("CONSOLE_METADATA_UNKNOWN");uint[] ids=new uint[8];uint count=GetConsoleProcessList(ids,8);uint self=GetCurrentProcessId();bool seenSelf=false,seenParent=false;for(int i=0;i<count&&i<8;i++){if(ids[i]==self)seenSelf=true;if(ids[i]==(uint)parent)seenParent=true;}
  if(count!=2||!seenSelf||!seenParent||aux==0||aux==self||aux==(uint)parent)Fail("PARTICIPANT_TOPOLOGY_DENY");
  if(parentPidPin!=0&&(parentPidPin!=(uint)parent||consolePidPin!=aux))Fail("ADMIN_PID_CHANGED");
  try{
   parentReference=OpenProcess(0x1000,false,(uint)parent);if(parentReference==IntPtr.Zero)Fail("PARENT_QUERY_DENY");consoleReference=OpenProcess(0x1000,false,aux);if(consoleReference==IntPtr.Zero)Fail("CONSOLE_QUERY_DENY");
   long pc=0,ac=0,e,k,u;uint ph=0,ah=0;if(!GetProcessTimes(parentReference,out pc,out e,out k,out u)||!GetProcessTimes(consoleReference,out ac,out e,out k,out u)||!GetProcessHandleCount(parentReference,out ph)||!GetProcessHandleCount(consoleReference,out ah))Fail("ADMIN_SAMPLE_UNKNOWN");
   if(pc<=0||ac<=0||ph==0||ah==0)Fail("ADMIN_METADATA_UNKNOWN");if(parentCreation==0){parentPidPin=(uint)parent;consolePidPin=aux;parentCreation=pc;consoleCreation=ac;}if(parentCreation!=pc||consoleCreation!=ac)Fail("ADMIN_PID_REUSE");if((ulong)ph+(ulong)ah>1024)Fail("ADMIN_BUDGET_DENY");adminSamples.Add(parent+":"+pc+":"+ph+"|"+aux+":"+ac+":"+ah);
  }finally{
   if(consoleReference!=IntPtr.Zero){if(CloseHandle(consoleReference))consoleReference=IntPtr.Zero;else{observerUnknown=true;observerCloseError=Marshal.GetLastWin32Error();}}
   if(!observerUnknown&&parentReference!=IntPtr.Zero){if(CloseHandle(parentReference))parentReference=IntPtr.Zero;else{observerUnknown=true;observerCloseError=Marshal.GetLastWin32Error();}}
  }
  if(observerUnknown)Fail("OBSERVER_CLOSE_UNKNOWN");
 }
 static string Hex(byte[] b){return BitConverter.ToString(b).Replace("-","").ToLowerInvariant();}
 static string Hash(byte[] b){return Hex(SaltedWire.Hash(b));}
 static void Fail(string s){if(first=="NONE"){first=s;firstStage=stage;}throw new InvalidOperationException(first);}
 static byte[] Send(IntPtr h,byte[] command){stage="PRE_SUBMIT_"+SaltedWire.U(command,6).ToString("x8");if(!Console.IsInputRedirected&&Console.KeyAvailable&&Console.ReadKey(true).Key==ConsoleKey.Escape)Fail("OWNER_STOP");if(!Credential.Eq(initialToken,TokenState()))Fail("TOKEN_CONTINUITY_DENY");AdministrativeSample();uint handles;if(!GetProcessHandleCount(new IntPtr(-1),out handles)||handles>128)Fail("NATIVE_HANDLE_GATE");handleSamples.Add(handles);if(++calls>12)Fail("CALL_LIMIT");uint cc=SaltedWire.U(command,6);if(cc!=0x17a&&cc!=0x173&&cc!=0x176&&cc!=0x17b)Fail("COMMAND_NOT_ALLOWED");byte[] response=new byte[4096];uint size=4096;if(cc==0x176)sessionAttempted=true;stage="SUBMIT_"+cc.ToString("x8");uint code=Tbsip_Submit_Command(h,0,200,command,(uint)command.Length,response,ref size);if(code!=0)Fail("TBS_"+code.ToString("x8"));if(size<10||size>4096)Fail("RESPONSE_SIZE_UNKNOWN");byte[] r=SaltedWire.Slice(response,0,(int)size);if(SaltedWire.U(r,2)!=size)Fail("RESPONSE_FRAME_UNKNOWN");if(SaltedWire.U(r,6)!=0)Fail("TPM_"+SaltedWire.U(r,6).ToString("x8"));stage="VERIFY_RESPONSE_"+cc.ToString("x8");return r;}
 static Dictionary<string,object> Json(string path){return new JavaScriptSerializer().Deserialize<Dictionary<string,object>>(File.ReadAllText(path));}
 static int Main(string[] args){
  if(args.Length==1&&args[0]=="--selfcheck"){
   byte[] cert=File.ReadAllBytes(Path.Combine(folder,"ek-certificate.PUBLIC.der"));if(Hash(cert)!="965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761")return 2;
   Console.WriteLine("SELF_CHECK_PASS; NATIVE_MODE_NOT_ENTERED; NO_TPM; COMMANDS=GetCapability/ReadPublic/StartAuthSession/GetRandom; NO_PERSISTENT_COMMAND");return 0;
  }
  if(args.Length!=3||args[0]!="--owner-manual-proof"){Console.WriteLine("DENY_NO_EXPLICIT_MANUAL_PROOF_PINS; NO_TPM");return 2;}
  IntPtr h=IntPtr.Zero;bool retained=false,close=false,proof=false;byte[] publicAreaName=null;string requestHash=null;
  try{
   using(WindowsIdentity identity=WindowsIdentity.GetCurrent()){
    if(identity.User.Value!="S-1-5-21-785699550-1440458426-1682014700-1108"||!(new WindowsPrincipal(identity)).IsInRole(WindowsBuiltInRole.Administrator))Fail("AUTHORIZED_ELEVATED_OPERATOR_REQUIRED");
    WindowsIdentity impersonation=WindowsIdentity.GetCurrent(true);if(impersonation!=null){impersonation.Dispose();Fail("IMPERSONATION_DENY");}
   }
   if(Console.IsInputRedirected)Fail("INTERACTIVE_CONSOLE_REQUIRED");initialToken=TokenState();liveBinding=BootBinding();string exe=System.Reflection.Assembly.GetExecutingAssembly().Location;string exeHash=Hash(File.ReadAllBytes(exe));if(exeHash!=args[1])Fail("EXE_PIN");
   string approvalPath=Path.Combine(folder,"OWNER-APPROVED-NATIVE-PROOF.json");byte[] approvalBytes=File.ReadAllBytes(approvalPath);if(Hash(approvalBytes)!=args[2])Fail("APPROVAL_PIN");Dictionary<string,object> approval=Json(approvalPath);
   requestHash=Hash(File.ReadAllBytes(Path.Combine(folder,"NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json")));
   if((string)approval["kind"]!="OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY"||(string)approval["request_sha256"]!=requestHash||(string)approval["executable_sha256"]!=exeHash||!(bool)approval["one_manual_execution"]||!(bool)approval["ack_unknown_revocation"]||!(bool)approval["ack_no_hardware_admission"]||!(bool)approval["ack_internal_tpm_accounting_unknown"])Fail("EXACT_APPROVAL_SCOPE");
   AdministrativeSample();Console.WriteLine("TRANSIENT SALTED EK POSSESSION EXPERIMENT ONLY; no NV/AK/key creation/hierarchy/Start/Stop/Q4. Revocation UNKNOWN. One attempt, no retry. Type: PROVE_EK_SESSION "+requestHash);
   if(Console.ReadLine()!="PROVE_EK_SESSION "+requestHash)Fail("CONFIRMATION_DENY");
   byte[] certBytes=File.ReadAllBytes(Path.Combine(folder,"ek-certificate.PUBLIC.der"));if(Hash(certBytes)!="965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761")Fail("CERT_PIN");
   // Administrative one-shot receipt only, never a second rollback authority or Q4 ledger.
   stage="ADMIN_RECEIPT";using(FileStream once=new FileStream(Path.Combine(folder,"NATIVE-PROOF-ATTEMPT.ONCE.receipt"),FileMode.CreateNew,FileAccess.Write,FileShare.None,4096,FileOptions.WriteThrough)){byte[] receipt=Encoding.UTF8.GetBytes("REQUEST="+requestHash+"\nAPPROVAL="+args[2]+"\nNO_RETRY; NOT_Q4_AUTHORITY\n");once.Write(receipt,0,receipt.Length);once.Flush(true);}
   using(X509Certificate2 cert=new X509Certificate2(certBytes))using(RSA certKey=cert.GetRSAPublicKey())using(RSACng ekPublic=new RSACng()){
    RSAParameters ek=certKey.ExportParameters(false);if(ek.Modulus.Length!=256||!Credential.Eq(ek.Exponent,new byte[]{1,0,1}))Fail("EK_PROFILE");ekPublic.ImportParameters(ek);
    AdministrativeSample();Params param=new Params();param.version=2;param.flags=4;stage="TBS_CONTEXT_OPEN";contextOpenAttempted=true;uint opened=Tbsi_Context_Create(ref param,out h);if(opened!=0||h==IntPtr.Zero){if(h!=IntPtr.Zero)retained=true;Fail("TBS_OPEN_"+opened.ToString("x8"));}contextOpened=true;
    byte[] cap=Credential.Cat(new byte[]{0x80,1},Credential.D(22),Credential.D(0x17a),Credential.D(1),Credential.D(0x81000000),Credential.D(8));uint[] handles=PublicAreaGate.Handles(Send(h,cap));uint selected=0;byte[] selectedArea=null;
    foreach(uint handle in handles){byte[] read=Credential.Cat(new byte[]{0x80,1},Credential.D(14),Credential.D(0x173),Credential.D(handle));byte[] r=Send(h,read),name;
     try{byte[] area=PublicAreaGate.Validate(r,ek.Modulus,out name);if(selected!=0)Fail("MULTIPLE_MATCHES");selected=handle;selectedArea=area;publicAreaName=name;}
     catch(InvalidOperationException e){if(e.Message!="PUBLIC_PROFILE_SIZE"&&e.Message!="PUBLIC_PROFILE"&&e.Message!="EXPONENT"&&e.Message!="EK_MISMATCH")throw;}
    }
    if(selected==0)Fail("NO_MATCHING_ACCESSIBLE_EK_NO_FALLBACK");
    byte[] context=SaltedWire.Hash(Credential.Cat(Encoding.UTF8.GetBytes(requestHash+"|"+args[2]+"|"+liveBinding),initialToken,publicAreaName));SaltedSessionPlan plan=new SaltedSessionPlan(context);retainedPlan=plan;
    byte[] start=plan.BuildStart(selected,ekPublic,context);byte[] query=plan.AcceptStartAndBuildQuery(Send(h,start),context);
    byte[] reply=Send(h,query);if(!plan.AcceptProof(reply,context))Fail("PROOF_HMAC_UNKNOWN");sessionEnded=true;
    byte[] finalRead=Credential.Cat(new byte[]{0x80,1},Credential.D(14),Credential.D(0x173),Credential.D(selected)),finalName;byte[] finalArea=PublicAreaGate.Validate(Send(h,finalRead),ek.Modulus,out finalName);
    if(!Credential.Eq(selectedArea,finalArea)||!Credential.Eq(publicAreaName,finalName))Fail("FINAL_EK_CHANGED");proof=true;
   }
  }catch(Exception e){if(first=="NONE"){first=e.Message;firstStage=stage;}}
  if(observerUnknown)retained=true;if(h!=IntPtr.Zero){if(retained||(sessionAttempted&&!sessionEnded)){retained=true;retainedContext=h;}else{uint code=Tbsip_Context_Close(h);if(code==0){close=true;h=IntPtr.Zero;}else{if(first=="NONE"){first="TBS_CLOSE_"+code.ToString("x8");firstStage="TBS_CONTEXT_CLOSE";}retained=true;retainedContext=h;}}}
  Console.WriteLine(new JavaScriptSerializer().Serialize(new {first=first,first_stage=firstStage,observer_query_attempted=observerQueryAttempted,observer_query_succeeded=observerQuerySucceeded,observer_own_count=observerQuerySucceeded?(uint?)observerOwnCount:null,observer_query_error=observerQueryError,observer_reserve_limit=124,observer_close_error=observerCloseError,proof=proof,request_sha256=requestHash,live_binding=liveBinding,calls=calls,handle_samples=handleSamples.ToArray(),administrative_samples=adminSamples.ToArray(),console_association="METADATA_NOT_AUTHENTICATED",unsampled_peak="UNKNOWN",public_area_name=publicAreaName==null?null:Hex(publicAreaName),session_end=sessionEnded?"AUTHENTICATED_CONTINUE_CLEAR_SEMANTIC":"NOT_CONFIRMED",context_close=close,context_opened=contextOpened,context_state=close?"CLOSED_CONFIRMED":h!=IntPtr.Zero?"UNKNOWN_RETAINED":contextOpenAttempted?"OPEN_FAILED_NO_HANDLE":"NOT_OPENED",observer_refs_retained=observerUnknown,retained=retained,revocation="UNKNOWN",device_generation="NOT_ESTABLISHED",NV_mutations="NONE",Q4="NOT_EXECUTED",authority="DENY",no_retry=true}));
  if(retained){Console.WriteLine("UNKNOWN: original TBS reference retained; no retry, no FlushContext guessing. Abrupt host loss leaves UNKNOWN.");while(true)System.Threading.Thread.Sleep(1000);}
  if(retainedPlan!=null){retainedPlan.CloseFactualContext();retainedPlan=null;}GC.KeepAlive(retainedContext);return proof&&close&&first=="NONE"?0:2;
 }
}
