// Fixed one-off read-only observer. No target arguments or disposition capability.
using System;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
public static class SQB3ParticipantsReadOnlyModel {
 const uint ControllerPid=4184,QuerySynchronize=0x00101000;
 const long ControllerCreation=134381373154465967L;
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string ControllerSid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string RuntimeSid="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 static int used;
 public static class Mock {
  public static int Case,ScmCalls,ControllerStats,RuntimeStats,ClosedCalls;public static bool RightsOk;
  public static System.Collections.Generic.HashSet<long> Opened=new System.Collections.Generic.HashSet<long>();
  public static System.Collections.Generic.HashSet<long> CloseAttempts=new System.Collections.Generic.HashSet<long>();
  public static void Reset(int c){Case=c;ScmCalls=ControllerStats=RuntimeStats=ClosedCalls=0;RightsOk=true;Opened.Clear();CloseAttempts.Clear();used=0;}
  public static string Confirm(){return Case==21?"CANCEL":"OBSERVE_SQB3_CURRENT_PARTICIPANTS_ONCE";}
  public static void Open(long h){if(!Opened.Add(h))throw new Exception("MODEL_DUPLICATE_ACQUISITION");}
  public static void Close(long h){ClosedCalls++;if(!Opened.Contains(h)||!CloseAttempts.Add(h))throw new Exception("MODEL_UNOWNED_OR_REPEATED_CLOSE");}
 }

 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high;public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 sealed class Snapshot {public IntPtr process,token;public uint pid;public long creation;public Stats statistics;public string sid;}
 static IntPtr OpenProcess(uint rights,bool inherit,uint pid){Mock.RightsOk&=rights==0x00101000&&!inherit&&(pid==4184||pid==18000);if(Mock.Case==3&&pid==4184)return IntPtr.Zero;Mock.Open(pid);return new IntPtr(pid);}
 static bool CloseHandle(IntPtr h){Mock.Close(h.ToInt64());return !(Mock.Case==17&&h.ToInt64()==18000||Mock.Case==18&&h.ToInt64()==118000);}
 static uint GetProcessId(IntPtr h){return Mock.Case==15&&h.ToInt64()==18000?18001U:(uint)h.ToInt64();}
 static bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u){long value=ControllerCreation+(h.ToInt64()==18000?1000:0);if(Mock.Case==4&&h.ToInt64()==4184)value++;if(Mock.Case==12&&h.ToInt64()==18000)value=ControllerCreation-1;c=new Times();c.low=(uint)value;c.high=(uint)(value>>32);e=k=u=new Times();return true;}
 static bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size){Mock.RightsOk&=flags==0&&size==1024;text.Append(Mock.Case==5?@"C:\wrong.exe":Image);size=(uint)text.Length;return true;}
 static uint WaitForSingleObject(IntPtr h,uint ms){Mock.RightsOk&=ms==0;return Mock.Case==13&&h.ToInt64()==18000?0U:258U;}
 static bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token){Mock.RightsOk&=rights==8;token=IntPtr.Zero;if(Mock.Case==6&&h.ToInt64()==4184||Mock.Case==27&&h.ToInt64()==18000)return false;token=new IntPtr(100000+h.ToInt64());Mock.Open(token.ToInt64());return true;}
 static bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint bytes){bool ctrl=h.ToInt64()==104184;Mock.RightsOk&=cap==512&&(cls==1||cls==10);bytes=0;if(cls==1){if(Mock.Case==7&&ctrl)return false;SecurityIdentifier sid=new SecurityIdentifier(Mock.Case==11&&!ctrl?ControllerSid:ctrl?ControllerSid:RuntimeSid);byte[] v=new byte[sid.BinaryLength];sid.GetBinaryForm(v,0);IntPtr target=IntPtr.Add(data,IntPtr.Size);Marshal.WriteIntPtr(data,Mock.Case==8?IntPtr.Add(data,513):target);Marshal.Copy(v,0,target,v.Length);bytes=(uint)(IntPtr.Size+v.Length);return true;}Stats st=new Stats();st.type=1;st.groups=ctrl?17U:12U;st.token.low=ctrl?191041869U:1001U;st.authentication.low=ctrl?570063U:2001U;st.modified.low=ctrl?191041540U:3001U;if(ctrl)Mock.ControllerStats++;else Mock.RuntimeStats++;if(Mock.Case==10&&ctrl)st.token.low++;if(Mock.Case==16&&!ctrl&&Mock.RuntimeStats>1)st.modified.low++;if(Mock.Case==24)st.type=2;Marshal.StructureToPtr(st,data,false);bytes=Mock.Case==9?1U:(uint)Marshal.SizeOf(typeof(Stats));return true;}
 static IntPtr OpenSCManagerW(string machine,string db,uint rights){Mock.RightsOk&=machine==null&&db==null&&rights==1;if(Mock.Case==22)return IntPtr.Zero;Mock.Open(31);return new IntPtr(31);}
 static IntPtr OpenServiceW(IntPtr manager,string name,uint rights){Mock.RightsOk&=manager.ToInt64()==31&&name=="PapinhoAccelerator"&&rights==4;if(Mock.Case==23)return IntPtr.Zero;Mock.Open(32);return new IntPtr(32);}
 static bool QueryServiceStatusEx(IntPtr service,int cls,out Status value,uint cap,out uint bytes){Mock.RightsOk&=service.ToInt64()==32&&cls==0&&cap==36;Mock.ScmCalls++;value=new Status();value.type=16;value.state=2;value.pid=18000;bytes=36;if(Mock.Case==1)return false;if(Mock.Case==2||Mock.Case==14&&Mock.ScmCalls==2)value.state=1;if(Mock.Case==25)bytes=35;if(Mock.Case==26)value.pid=ControllerPid;if(Mock.Case==28&&Mock.ScmCalls==2)value.pid=18001;if(Mock.Case==29)value.pid=0;return true;}
 static bool CloseServiceHandle(IntPtr h){Mock.Close(h.ToInt64());return !(Mock.Case==19&&h.ToInt64()==32||Mock.Case==20&&h.ToInt64()==31);}
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
   if(s.pid==ControllerPid)Need(value.groups==17&&value.token.low==191041869&&value.token.high==0&&value.authentication.low==570063&&value.authentication.high==0&&value.modified.low==191041540&&value.modified.high==0,"CONTROLLER_ORIGINAL_TOKEN_MISMATCH");
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
  Console.WriteLine("ONE_READONLY_SQB3_PARTICIPANTS; NO_STOP_NO_KILL_NO_START_NO_UPDATE_NO_RETRY; PID_IS_LOCATOR_ONLY");
  try {
   Console.WriteLine("Type OBSERVE_SQB3_CURRENT_PARTICIPANTS_ONCE only after specific owner approval:");
   Need(Mock.Confirm()=="OBSERVE_SQB3_CURRENT_PARTICIPANTS_ONCE","CONFIRMATION_CANCELLED");
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

 public static string OfflineCases(){
  int passed=0;System.IO.TextWriter original=Console.Out;
  for(int c=0;c<=29;c++){
   Mock.Reset(c);System.IO.StringWriter output=new System.IO.StringWriter();int result;
   try{Console.SetOut(output);result=RunFixed();}finally{Console.SetOut(original);}
   string log=output.ToString();bool ok=c==0?result==0:result==2;
   if(!ok||!Mock.RightsOk||Mock.CloseAttempts.Count!=Mock.Opened.Count)throw new Exception("MODEL_CASE_"+c+" result="+result+" log="+log);
   if(c==0&&(!log.Contains("complete=True observer_close_known=True")||!log.Contains("role=RUNTIME_CURRENT_SCM_CANDIDATE")||Mock.ScmCalls!=2||Mock.Opened.Count!=6))throw new Exception("MODEL_POSITIVE_INCOMPLETE");
   if(c==21&&(Mock.Opened.Count!=0||!log.Contains("consumed=False")))throw new Exception("MODEL_CANCEL_EFFECT");
   if(c>=17&&c<=20&&!log.Contains("observer_close_known=False"))throw new Exception("MODEL_CLOSE_UNKNOWN_LOST");
   int before=Mock.ScmCalls;bool denied=false;try{RunFixed();}catch(InvalidOperationException){denied=true;}if(!denied||Mock.ScmCalls!=before)throw new Exception("MODEL_REPLAY");
   passed++;
  }
  return "PASS_READONLY_PARTICIPANTS cases="+passed+" native_edges=12 RIGHTS_QUERY_ONLY ALL_OWNED_CLOSES_ONCE NO_REAL_SCM_PROCESS_TOKEN_QUERY";
 }
}
