// Fixed one-off read-only observer. No target arguments or disposition capability.
using System;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
public static class READOFFERParticipantsReadOnly {
 const uint ControllerPid=15896,QuerySynchronize=0x00101000;
 const long ControllerCreation=134382037035307596L;
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
 static void Need(bool ok,string stage){if(!ok)throw new InvalidOperationException(stage);}
 static void Native(bool ok,string stage){if(!ok)throw new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 static uint Scm(IntPtr service,string stage){Status value;uint n;Native(QueryServiceStatusEx(service,0,out value,36,out n),stage+"_QUERY");Need(n==36,stage+"_LENGTH");Console.WriteLine("SCM_OBSERVED state="+value.state+" pid="+value.pid+" win32="+value.win32+" specific="+value.specific+"; CURRENT_ONLY_NOT_AUTHORITY");Need(value.type==16&&value.state==2&&value.pid!=0&&value.pid!=ControllerPid&&value.win32==0&&value.specific==0,stage+"_MISMATCH_NO_FALLBACK");Console.WriteLine("SCM_CANDIDATE state="+value.state+" pid="+value.pid+"; CURRENT_ONLY_NOT_ORIGINAL_ATTESTATION");return value.pid;}
 static void ProcessCurrent(Snapshot s,bool initial){
  Times c,e,k,u;Native(GetProcessTimes(s.process,out c,out e,out k,out u),"PROCESS_TIMES");
  Need(GetProcessId(s.process)==s.pid,"PROCESS_PID_MISMATCH");
  Need(c.Value>0&&(initial||c.Value==s.creation),"PROCESS_CREATION_MISMATCH");
  if(initial){s.creation=c.Value;if(s.pid==ControllerPid)Need(s.creation==ControllerCreation,"CONTROLLER_CREATION_MISMATCH");}
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
   if(initial){s.statistics=value;s.sid=sid;}
   else Need(Same(value.token,s.statistics.token)&&Same(value.authentication,s.statistics.authentication)&&Same(value.modified,s.statistics.modified)&&value.groups==s.statistics.groups&&sid==s.sid,"TOKEN_CONTINUITY_MISMATCH");
  }finally{Marshal.FreeHGlobal(buf);}
 }
 static void Capture(Snapshot s){s.process=OpenProcess(QuerySynchronize,false,s.pid);Native(s.process!=IntPtr.Zero,"PROCESS_OPEN_QUERY_ONLY");ProcessCurrent(s,true);Native(OpenProcessToken(s.process,8,out s.token),"TOKEN_OPEN_QUERY_ONLY");TokenCurrent(s,true);}
 static void Revalidate(Snapshot s){ProcessCurrent(s,false);TokenCurrent(s,false);ProcessCurrent(s,false);}
 static void Print(Snapshot s,string role){Console.WriteLine("PARTICIPANT_OBSERVED role="+role+" pid="+s.pid+" creation="+s.creation+" token.low="+s.statistics.token.low+" token.high="+s.statistics.token.high+" authentication.low="+s.statistics.authentication.low+" authentication.high="+s.statistics.authentication.high+" modified.low="+s.statistics.modified.low+" modified.high="+s.statistics.modified.high+" groups="+s.statistics.groups+" sid="+s.sid+"; CURRENT_CORRELATION_ONLY_NOT_HISTORICAL_ORIGINAL_PROOF_NOT_AUTHORITY");}
 public static int RunFixed(){
  Need(Interlocked.CompareExchange(ref used,1,0)==0,"NO_RETRY");
  Snapshot controller=new Snapshot(),runtime=new Snapshot();controller.pid=ControllerPid;
  IntPtr manager=IntPtr.Zero,service=IntPtr.Zero;int first=0;bool consumed=false,complete=false,closeKnown=true;
  Console.WriteLine("ONE_READONLY_READOFFER_PARTICIPANTS; NO_STOP_NO_KILL_NO_START_NO_UPDATE_NO_RETRY; PID_IS_LOCATOR_ONLY");
  try {
   Console.WriteLine("Type OBSERVE_READOFFER_CURRENT_PARTICIPANTS_ONCE only after specific owner approval:");
   Need(Console.ReadLine()=="OBSERVE_READOFFER_CURRENT_PARTICIPANTS_ONCE","CONFIRMATION_CANCELLED");
   consumed=true;
   manager=OpenSCManagerW(null,null,1);Native(manager!=IntPtr.Zero,"SCM_CONNECT_QUERY_ONLY");
   service=OpenServiceW(manager,"PapinhoAccelerator",4);Native(service!=IntPtr.Zero,"SCM_OPEN_QUERY_STATUS_ONLY");
   runtime.pid=Scm(service,"SCM_BEFORE");Capture(controller);Capture(runtime);
   Need(controller.creation<=runtime.creation&&controller.pid!=runtime.pid,"PARTICIPANT_ORDER_MISMATCH");
   Revalidate(controller);Revalidate(runtime);Need(Scm(service,"SCM_AFTER")==runtime.pid,"SCM_CANDIDATE_CHANGED_NO_ADOPTION");
   Print(controller,"CONTROLLER_ORIGINAL_MATCH");Print(runtime,"RUNTIME_CURRENT_SCM_CANDIDATE");complete=true;
  }catch(Exception ex){first=13;Win32Exception native=ex as Win32Exception;if(native!=null&&native.NativeErrorCode!=0)first=native.NativeErrorCode;Console.WriteLine("PARTICIPANTS_FIRST_FAILURE stage="+ex.Message+" native_error="+(native==null?"UNKNOWN":native.NativeErrorCode.ToString()));}
  finally {
   foreach(Snapshot s in new Snapshot[]{runtime,controller}){
    if(s.token!=IntPtr.Zero&&!CloseHandle(s.token)){closeKnown=false;Console.WriteLine("OBSERVER_TOKEN_CLOSE_UNKNOWN pid="+s.pid);}
    if(s.process!=IntPtr.Zero&&!CloseHandle(s.process)){closeKnown=false;Console.WriteLine("OBSERVER_PROCESS_CLOSE_UNKNOWN pid="+s.pid);}
   }
   if(service!=IntPtr.Zero&&!CloseServiceHandle(service)){closeKnown=false;Console.WriteLine("OBSERVER_SERVICE_CLOSE_UNKNOWN");}
   if(manager!=IntPtr.Zero&&!CloseServiceHandle(manager)){closeKnown=false;Console.WriteLine("OBSERVER_MANAGER_CLOSE_UNKNOWN");}
  }
  Console.WriteLine("PARTICIPANTS_READONLY_RESULT first="+first+" consumed="+consumed+" complete="+complete+" observer_close_known="+closeKnown+"; ORIGINAL_RUNTIME_BILATERAL_AND_APPLICATION_CLOSE_AND_HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY_NO_DISPOSITION_AUTHORITY");
  return first==0&&complete&&closeKnown?0:2;
 }
}
