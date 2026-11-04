// Fixed exceptional disposition candidate; never invoked during offline qualification.
using System;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
using System.IO;
using System.Security.Cryptography;
using Microsoft.Win32.SafeHandles;
public static class SQF1DETAIL21RetainedParticipantsDisposition {
 const uint ControllerPid=11760,RuntimePid=20768,QuerySynchronize=0x00101001;
 const long ControllerCreation=134381469493313573L,RuntimeCreation=134381469733727285L;
 const uint ExceptionalExit=0xe0000001;
 const string ImagePin="78886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5";
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string ControllerSid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string RuntimeSid="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 static int used;
 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high;public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 sealed class Snapshot {public IntPtr process,token;public uint pid;public long creation;public Stats statistics;public string sid;}
 [DllImport("kernel32.dll",SetLastError=true)] static extern IntPtr OpenProcess(uint rights,bool inherit,uint pid);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] static extern uint GetProcessId(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size);
 [DllImport("kernel32.dll",SetLastError=true)] static extern uint WaitForSingleObject(IntPtr h,uint ms);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint bytes);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr OpenSCManagerW(string machine,string db,uint rights);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr OpenServiceW(IntPtr manager,string name,uint rights);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool QueryServiceStatusEx(IntPtr service,int cls,out Status value,uint cap,out uint bytes);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool CloseServiceHandle(IntPtr h);

 [StructLayout(LayoutKind.Sequential)] struct FileInfo {public uint attributes;public Times creation,access,write;public uint volume,sizeHigh,sizeLow,links,indexHigh,indexLow;}
 [StructLayout(LayoutKind.Sequential)] struct Config {public uint type,start,error;public IntPtr binary,load;public uint tag;public IntPtr dependencies,account,display;}
 [StructLayout(LayoutKind.Sequential)] struct FailureActions {public uint reset;public IntPtr reboot,command;public uint count;public IntPtr actions;}
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool TerminateProcess(IntPtr h,uint code);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetExitCodeProcess(IntPtr h,out uint code);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetFileInformationByHandle(SafeFileHandle h,out FileInfo value);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern bool QueryServiceConfigW(IntPtr service,IntPtr buf,uint cap,out uint bytes);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern bool QueryServiceConfig2W(IntPtr service,uint level,IntPtr buf,uint cap,out uint bytes);
 sealed class PinnedImage : IDisposable {
  FileStream stream;
  public void Open(){
   stream=new FileStream(Image,FileMode.Open,FileAccess.Read,FileShare.Read);Validate();
   using(SHA256 hash=SHA256.Create()){Need(BitConverter.ToString(hash.ComputeHash(stream)).Replace("-","").ToLowerInvariant()==ImagePin,"IMAGE_PIN_MISMATCH");}
  }
  public void Validate(){FileInfo info;Native(GetFileInformationByHandle(stream.SafeFileHandle,out info),"IMAGE_ID_QUERY");Need(info.volume==417614100&&info.indexHigh==1638400&&info.indexLow==439388&&info.sizeHigh==0&&info.sizeLow==809984&&info.links==1&&(info.attributes&0x410)==0,"IMAGE_ID_SIZE_ATTRIBUTES_MISMATCH");}
  public void Dispose(){if(stream!=null)stream.Dispose();}
 }
 static string BoundString(IntPtr start,uint bytes,IntPtr value){
  long delta=value.ToInt64()-start.ToInt64();Need(bytes>=2&&value!=IntPtr.Zero&&delta>=0&&delta%2==0&&delta<=(long)bytes-2,"SCM_CONFIG_STRING_RANGE");
  int limit=(int)((bytes-delta)/2);for(int i=0;i<limit;i++)if(Marshal.ReadInt16(value,2*i)==0)return Marshal.PtrToStringUni(value,i);
  throw new InvalidOperationException("SCM_CONFIG_STRING_UNTERMINATED");
 }
 static void Configuration(IntPtr service){
  IntPtr buf=Marshal.AllocHGlobal(8192);
  try {
   uint n;Native(QueryServiceConfigW(service,buf,8192,out n),"SCM_CONFIG_QUERY");Need(n>=Marshal.SizeOf(typeof(Config))&&n<=8192,"SCM_CONFIG_LENGTH");Config c=(Config)Marshal.PtrToStructure(buf,typeof(Config));
   Need(c.type==16&&c.start==3&&c.error==1&&BoundString(buf,n,c.binary)=="\""+Image+"\" service host scq-closed"&&BoundString(buf,n,c.account)=="NT SERVICE\\PapinhoAccelerator","SCM_CONFIG_MISMATCH");
   Native(QueryServiceConfig2W(service,5,buf,8192,out n),"SCM_SID_QUERY");Need(n==4&&Marshal.ReadInt32(buf)==1,"SCM_SID_MISMATCH");
   Native(QueryServiceConfig2W(service,2,buf,8192,out n),"SCM_RECOVERY_QUERY");Need(n>=Marshal.SizeOf(typeof(FailureActions))&&n<=8192,"SCM_RECOVERY_LENGTH");FailureActions actions=(FailureActions)Marshal.PtrToStructure(buf,typeof(FailureActions));
   Need(actions.count==0,"SCM_RECOVERY_ACTIONS_PRESENT_DENY_NO_CHANGE");
  }finally{Marshal.FreeHGlobal(buf);}
 }
 static void Stopped(IntPtr service,string stage){Status value;uint n;Native(QueryServiceStatusEx(service,0,out value,36,out n),stage+"_QUERY");Need(n==36&&value.type==16&&value.state==1&&value.pid==0,stage+"_NOT_STOPPED_PID0");Console.WriteLine("SCM_STOPPED_OBSERVED win32="+value.win32+" specific="+value.specific+"; EXPECTED_EXCEPTIONAL_TERMINATION_NOT_DIAGNOSTIC_SUCCESS");}
 static void ExitObserved(Snapshot s,string role){uint wait=WaitForSingleObject(s.process,60000);Need(wait==0,role+"_EXIT_NOT_OBSERVED_UNKNOWN");uint code;Native(GetExitCodeProcess(s.process,out code),role+"_EXIT_CODE_QUERY");Need(code==ExceptionalExit,role+"_EXIT_CODE_MISMATCH");}
 static void Need(bool ok,string stage){if(!ok)throw new InvalidOperationException(stage);}
 static void Native(bool ok,string stage){if(!ok)throw new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 static void Scm(IntPtr service,string stage){Status value;uint n;Native(QueryServiceStatusEx(service,0,out value,36,out n),stage+"_QUERY");Need(n==36&&value.type==16&&value.state==2&&value.pid==RuntimePid&&value.win32==0&&value.specific==0,stage+"_MISMATCH_NO_FALLBACK");}
 static void ProcessCurrent(Snapshot s,bool initial){
  Times c,e,k,u;Native(GetProcessTimes(s.process,out c,out e,out k,out u),"PROCESS_TIMES");
  Need(GetProcessId(s.process)==s.pid,"PROCESS_PID_MISMATCH");
  Need(c.Value>0&&(initial||c.Value==s.creation),"PROCESS_CREATION_MISMATCH");
  if(initial){s.creation=c.Value;Need(s.creation==(s.pid==ControllerPid?ControllerCreation:RuntimeCreation),"ORIGINAL_CREATION_MISMATCH");}
  Need(WaitForSingleObject(s.process,0)==258,"PROCESS_NOT_LIVE_OR_WAIT_UNKNOWN");
  StringBuilder text=new StringBuilder(1024);uint length=1024;Native(QueryFullProcessImageNameW(s.process,0,text,ref length),"PROCESS_IMAGE_QUERY");
  Need(length>0&&length<1024&&String.Equals(text.ToString(),Image,StringComparison.OrdinalIgnoreCase),"PROCESS_IMAGE_PATH_MISMATCH");
 }
 static bool Same(Luid a,Luid b){return a.low==b.low&&a.high==b.high;}
 static void TokenCurrent(Snapshot s,bool initial){
  IntPtr buf=Marshal.AllocHGlobal(512);
  try {
   uint n;Native(GetTokenInformation(s.token,1,buf,512,out n),"TOKEN_USER_QUERY");Need(n>=IntPtr.Size&&n<=512,"TOKEN_USER_LENGTH");
   IntPtr user=Marshal.ReadIntPtr(buf);long offset=user.ToInt64()-buf.ToInt64();Need(offset>=IntPtr.Size&&offset<=n-8,"TOKEN_USER_RANGE");
   int length=8+4*Marshal.ReadByte(user,1);Need(length<=68&&offset+length<=n,"TOKEN_SID_RANGE");
   string sid=new SecurityIdentifier(user).Value;Need(sid==(s.pid==ControllerPid?ControllerSid:RuntimeSid),"TOKEN_USER_MISMATCH");
   Native(GetTokenInformation(s.token,10,buf,512,out n),"TOKEN_STATS_QUERY");Need(n==Marshal.SizeOf(typeof(Stats)),"TOKEN_STATS_LENGTH");
   Stats value=(Stats)Marshal.PtrToStructure(buf,typeof(Stats));Need(value.type==1,"TOKEN_NOT_PRIMARY");
   if(s.pid==ControllerPid)Need(value.groups==17&&value.token.low==235999083&&value.token.high==0&&value.authentication.low==570063&&value.authentication.high==0&&value.modified.low==235998740&&value.modified.high==0,"CONTROLLER_ORIGINAL_TOKEN_MISMATCH");
   if(s.pid==RuntimePid)Need(value.groups==10&&value.token.low==236012027&&value.token.high==0&&value.authentication.low==236011749&&value.authentication.high==0&&value.modified.low==236011797&&value.modified.high==0,"RUNTIME_OBSERVED_TOKEN_MISMATCH");
   if(initial){s.statistics=value;s.sid=sid;}
   else Need(Same(value.token,s.statistics.token)&&Same(value.authentication,s.statistics.authentication)&&Same(value.modified,s.statistics.modified)&&value.groups==s.statistics.groups&&sid==s.sid,"TOKEN_CONTINUITY_MISMATCH");
  }finally{Marshal.FreeHGlobal(buf);}
 }
 static void Capture(Snapshot s){s.process=OpenProcess(QuerySynchronize,false,s.pid);Native(s.process!=IntPtr.Zero,"PROCESS_OPEN_QUERY_ONLY");ProcessCurrent(s,true);Native(OpenProcessToken(s.process,8,out s.token),"TOKEN_OPEN_QUERY_ONLY");TokenCurrent(s,true);}
 static void Revalidate(Snapshot s){ProcessCurrent(s,false);TokenCurrent(s,false);ProcessCurrent(s,false);}
 static void Print(Snapshot s,string role){Console.WriteLine("PARTICIPANT_OBSERVED role="+role+" pid="+s.pid+" creation="+s.creation+" token.low="+s.statistics.token.low+" token.high="+s.statistics.token.high+" authentication.low="+s.statistics.authentication.low+" authentication.high="+s.statistics.authentication.high+" modified.low="+s.statistics.modified.low+" modified.high="+s.statistics.modified.high+" groups="+s.statistics.groups+" sid="+s.sid+"; CURRENT_CORRELATION_ONLY_NOT_HISTORICAL_ORIGINAL_PROOF_NOT_AUTHORITY");}

 public static int RunFixed(){
  Need(Interlocked.CompareExchange(ref used,1,0)==0,"NO_RETRY");
  Snapshot controller=new Snapshot(),runtime=new Snapshot();controller.pid=ControllerPid;runtime.pid=RuntimePid;
  IntPtr manager=IntPtr.Zero,service=IntPtr.Zero;PinnedImage image=null;int first=0;
  bool consumed=false,runtimeSubmitted=false,runtimeExited=false,controllerSubmitted=false,controllerExited=false,closeKnown=true;
  try {
   manager=OpenSCManagerW(null,null,1);Native(manager!=IntPtr.Zero,"SCM_CONNECT_QUERY_ONLY");
   service=OpenServiceW(manager,"PapinhoAccelerator",5);Native(service!=IntPtr.Zero,"SCM_QUERY_CONFIG_STATUS_OPEN");
   Configuration(service);Scm(service,"SCM_BEFORE");Capture(controller);Capture(runtime);image=new PinnedImage();image.Open();
   Revalidate(controller);Revalidate(runtime);Print(controller,"CONTROLLER_FIXED");Print(runtime,"RUNTIME_FIXED");
   Console.WriteLine("EXCEPTIONAL_DISPOSITION_PLAN runtime20768_then_controller11760; TWO_FIXED_TERMINATE_CALLS_MAX; EXIT=0xe0000001; WAIT60000_EACH_NOT_NATIVE_CEILING; NO_STOP_NO_UPDATE_NO_CLEANUP; APPLICATION_CLOSE_UNKNOWN");
   Console.WriteLine("Type DISPOSE_SQF1DETAIL21_11760_20768_ONCE only after exact owner approval:");
   Need(Console.ReadLine()=="DISPOSE_SQF1DETAIL21_11760_20768_ONCE","CONFIRMATION_CANCELLED_NO_RETRY");
   Configuration(service);Scm(service,"SCM_PRE_SUBMISSION");Revalidate(controller);Revalidate(runtime);image.Validate();
   consumed=true;runtimeSubmitted=true;Native(TerminateProcess(runtime.process,ExceptionalExit),"RUNTIME_TERMINATE_SUBMISSION");
   ExitObserved(runtime,"RUNTIME");runtimeExited=true;
   Stopped(service,"SCM_AFTER_RUNTIME");Configuration(service);Revalidate(controller);image.Validate();
   controllerSubmitted=true;Native(TerminateProcess(controller.process,ExceptionalExit),"CONTROLLER_TERMINATE_SUBMISSION");
   ExitObserved(controller,"CONTROLLER");controllerExited=true;
   Stopped(service,"SCM_FINAL");image.Validate();
  }catch(Exception ex){first=13;Win32Exception native=ex as Win32Exception;if(native!=null&&native.NativeErrorCode!=0)first=native.NativeErrorCode;Console.WriteLine("DISPOSITION_FIRST_FAILURE stage="+ex.Message+" native_error="+(native==null?"UNKNOWN":native.NativeErrorCode.ToString()));}
  finally {
   if(image!=null){try{image.Dispose();}catch{closeKnown=false;Console.WriteLine("OBSERVER_IMAGE_CLOSE_UNKNOWN");}}
   foreach(Snapshot s in new Snapshot[]{runtime,controller}){
    if(s.token!=IntPtr.Zero&&!CloseHandle(s.token)){closeKnown=false;Console.WriteLine("OBSERVER_TOKEN_CLOSE_UNKNOWN pid="+s.pid);}
    if(s.process!=IntPtr.Zero&&!CloseHandle(s.process)){closeKnown=false;Console.WriteLine("OBSERVER_PROCESS_CLOSE_UNKNOWN pid="+s.pid);}
   }
   if(service!=IntPtr.Zero&&!CloseServiceHandle(service)){closeKnown=false;Console.WriteLine("OBSERVER_SERVICE_CLOSE_UNKNOWN");}
   if(manager!=IntPtr.Zero&&!CloseServiceHandle(manager)){closeKnown=false;Console.WriteLine("OBSERVER_MANAGER_CLOSE_UNKNOWN");}
  }
  Console.WriteLine("SQF1DETAIL21_RETAINED_DISPOSITION_RESULT first="+first+" consumed="+consumed+" runtime_submitted="+runtimeSubmitted+" runtime_exit_observed="+runtimeExited+" controller_submitted="+controllerSubmitted+" controller_exit_observed="+controllerExited+" observer_close_known="+closeKnown+"; APPLICATION_CLOSE_AND_ORIGINAL_RUNTIME_BILATERAL_AND_HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY_NO_REFUND_NO_UPDATE_NO_START_NO_CLEANUP");
  return first==0&&runtimeExited&&controllerExited&&closeKnown?0:2;
 }
}
