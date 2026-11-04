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
public static class RuntimeTokenParticipantsDispositionModel {
 const uint ControllerPid=6120,RuntimePid=10080,QuerySynchronize=0x00101001;
 const long ControllerCreation=134381245905057064L,RuntimeCreation=134381246145672895L;
 const uint ExceptionalExit=0xe0000001;
 const string ImagePin="bf7d0793428f5f751e87b007b28d676aa10235e6decc36d33bdd1de434508d60";
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string ControllerSid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string RuntimeSid="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 static int used;
 public static class Mock {
  public static int Case,ScmCalls,ControllerStats,RuntimeStats,ClosedCalls,RuntimeCalls,ControllerCalls,ImageOpened,ImageClosed,ImageValidations;
  public static bool RightsOk,RuntimeTerminated,ControllerTerminated;
  public static System.Collections.Generic.HashSet<long> Opened=new System.Collections.Generic.HashSet<long>();
  public static System.Collections.Generic.HashSet<long> CloseAttempts=new System.Collections.Generic.HashSet<long>();
  public static void Reset(int c){Case=c;ScmCalls=ControllerStats=RuntimeStats=ClosedCalls=RuntimeCalls=ControllerCalls=ImageOpened=ImageClosed=ImageValidations=0;RightsOk=true;RuntimeTerminated=ControllerTerminated=false;Opened.Clear();CloseAttempts.Clear();used=0;}
  public static string Confirm(){return Case==21?"CANCEL":"DISPOSE_RUNTIME_TOKEN_6120_10080_ONCE";}
  public static void Open(long h){if(!Opened.Add(h))throw new Exception("MODEL_DUPLICATE_ACQUISITION");}
  public static void Close(long h){ClosedCalls++;if(!Opened.Contains(h)||!CloseAttempts.Add(h))throw new Exception("MODEL_UNOWNED_OR_REPEATED_CLOSE");}
 }

 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high;public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 sealed class Snapshot {public IntPtr process,token;public uint pid;public long creation;public Stats statistics;public string sid;}
 static IntPtr OpenProcess(uint rights,bool inherit,uint pid){Mock.RightsOk&=rights==0x00101001&&!inherit&&(pid==6120||pid==10080);if(Mock.Case==3&&pid==6120||Mock.Case==47&&pid==10080)return IntPtr.Zero;Mock.Open(pid);return new IntPtr(pid);}
 static bool CloseHandle(IntPtr h){Mock.Close(h.ToInt64());return !(Mock.Case==17&&h.ToInt64()==10080||Mock.Case==18&&h.ToInt64()==110080);}
 static uint GetProcessId(IntPtr h){return Mock.Case==15&&h.ToInt64()==10080?10081U:(uint)h.ToInt64();}
 static bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u){long value=h.ToInt64()==10080?RuntimeCreation:ControllerCreation;if(Mock.Case==4&&h.ToInt64()==6120)value++;if(Mock.Case==12&&h.ToInt64()==10080)value=ControllerCreation-1;if(Mock.Case==46&&h.ToInt64()==10080)value++;c=new Times();c.low=(uint)value;c.high=(uint)(value>>32);e=k=u=new Times();return true;}
 static bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size){Mock.RightsOk&=flags==0&&size==1024;text.Append(Mock.Case==5?@"C:\wrong.exe":Image);size=(uint)text.Length;return true;}
 static uint WaitForSingleObject(IntPtr h,uint ms){Mock.RightsOk&=ms==0||ms==60000;if(ms==60000){if(Mock.Case==29&&h.ToInt64()==10080||Mock.Case==33&&h.ToInt64()==6120)return 258;return 0;}return Mock.Case==13&&h.ToInt64()==10080?0U:258U;}
 static bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token){Mock.RightsOk&=rights==8;token=IntPtr.Zero;if(Mock.Case==6&&h.ToInt64()==6120||Mock.Case==27&&h.ToInt64()==10080)return false;token=new IntPtr(100000+h.ToInt64());Mock.Open(token.ToInt64());return true;}
 static bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint bytes){bool ctrl=h.ToInt64()==106120;Mock.RightsOk&=cap==512&&(cls==1||cls==10);bytes=0;if(cls==1){if(Mock.Case==7&&ctrl)return false;SecurityIdentifier sid=new SecurityIdentifier(Mock.Case==11&&!ctrl?ControllerSid:ctrl?ControllerSid:RuntimeSid);byte[] v=new byte[sid.BinaryLength];sid.GetBinaryForm(v,0);IntPtr target=IntPtr.Add(data,IntPtr.Size);Marshal.WriteIntPtr(data,Mock.Case==8?IntPtr.Add(data,513):target);Marshal.Copy(v,0,target,v.Length);bytes=(uint)(IntPtr.Size+v.Length);return true;}Stats st=new Stats();st.type=1;st.groups=ctrl?17U:10U;st.token.low=ctrl?139600747U:139715724U;st.authentication.low=ctrl?570063U:139715443U;st.modified.low=ctrl?139600167U:139715493U;if(ctrl)Mock.ControllerStats++;else Mock.RuntimeStats++;if(Mock.Case==10&&ctrl)st.token.low++;if(Mock.Case==16&&!ctrl&&Mock.RuntimeStats>1||Mock.Case==36&&ctrl&&Mock.RuntimeTerminated)st.modified.low++;if(Mock.Case==24)st.type=2;Marshal.StructureToPtr(st,data,false);bytes=Mock.Case==9?1U:(uint)Marshal.SizeOf(typeof(Stats));return true;}
 static IntPtr OpenSCManagerW(string machine,string db,uint rights){Mock.RightsOk&=machine==null&&db==null&&rights==1;if(Mock.Case==22)return IntPtr.Zero;Mock.Open(31);return new IntPtr(31);}
 static IntPtr OpenServiceW(IntPtr manager,string name,uint rights){Mock.RightsOk&=manager.ToInt64()==31&&name=="PapinhoAccelerator"&&rights==5;if(Mock.Case==23)return IntPtr.Zero;Mock.Open(32);return new IntPtr(32);}
 static bool QueryServiceStatusEx(IntPtr service,int cls,out Status value,uint cap,out uint bytes){Mock.RightsOk&=service.ToInt64()==32&&cls==0&&cap==36;Mock.ScmCalls++;value=new Status();value.type=16;value.state=Mock.RuntimeTerminated?1U:2U;value.pid=Mock.RuntimeTerminated?0U:10080U;value.win32=Mock.RuntimeTerminated?1067U:0U;bytes=36;if(Mock.Case==1)return false;if(Mock.Case==2||Mock.Case==14&&Mock.ScmCalls==2)value.state=1;if(Mock.Case==25)bytes=35;if(Mock.Case==26)value.pid=10081;if(Mock.Case==35&&Mock.RuntimeTerminated){value.state=2;value.pid=10080;}if(Mock.Case==40&&Mock.ControllerTerminated){value.state=2;value.pid=10081;}return true;}
 static bool CloseServiceHandle(IntPtr h){Mock.Close(h.ToInt64());return !(Mock.Case==19&&h.ToInt64()==32||Mock.Case==20&&h.ToInt64()==31);}

 [StructLayout(LayoutKind.Sequential)] struct FileInfo {public uint attributes;public Times creation,access,write;public uint volume,sizeHigh,sizeLow,links,indexHigh,indexLow;}
 [StructLayout(LayoutKind.Sequential)] struct Config {public uint type,start,error;public IntPtr binary,load;public uint tag;public IntPtr dependencies,account,display;}
 [StructLayout(LayoutKind.Sequential)] struct FailureActions {public uint reset;public IntPtr reboot,command;public uint count;public IntPtr actions;}
 static bool TerminateProcess(IntPtr h,uint code){Mock.RightsOk&=code==0xe0000001;if(h.ToInt64()==10080){Mock.RuntimeCalls++;if(Mock.Case==28)return false;Mock.RuntimeTerminated=true;return true;}Mock.RightsOk&=h.ToInt64()==6120&&Mock.RuntimeTerminated;Mock.ControllerCalls++;if(Mock.Case==32)return false;Mock.ControllerTerminated=true;return true;}
 static bool GetExitCodeProcess(IntPtr h,out uint code){code=(Mock.Case==31&&h.ToInt64()==10080||Mock.Case==34&&h.ToInt64()==6120)?5U:0xe0000001U;return !(Mock.Case==30&&h.ToInt64()==10080);}
 static bool GetFileInformationByHandle(SafeFileHandle h,out FileInfo value){value=new FileInfo();throw new Exception("IMAGE_BOUNDARY_SHOULD_BE_MODELED");}
 static bool QueryServiceConfigW(IntPtr service,IntPtr buf,uint cap,out uint bytes){Mock.RightsOk&=service.ToInt64()==32&&cap==8192;Config c=new Config();c.type=16;c.start=3;c.error=1;byte[] binary=Encoding.Unicode.GetBytes((Mock.Case==39?"wrong":((char)34)+Image+((char)34)+" service host scq-closed")+"\0");byte[] account=Encoding.Unicode.GetBytes((Mock.Case==50&&Mock.RuntimeTerminated?"LocalSystem":@"NT SERVICE\PapinhoAccelerator")+"\0");int size=Marshal.SizeOf(typeof(Config));c.binary=IntPtr.Add(buf,size);c.account=IntPtr.Add(buf,size+binary.Length);Marshal.Copy(binary,0,c.binary,binary.Length);Marshal.Copy(account,0,c.account,account.Length);bytes=(uint)(size+binary.Length+account.Length);if(Mock.Case==43)c.binary=IntPtr.Add(buf,9000);Marshal.StructureToPtr(c,buf,false);if(Mock.Case==49)bytes=1;return true;}
 static bool QueryServiceConfig2W(IntPtr service,uint level,IntPtr buf,uint cap,out uint bytes){Mock.RightsOk&=service.ToInt64()==32&&cap==8192&&(level==2||level==5);if(level==5){Marshal.WriteInt32(buf,Mock.Case==41?3:1);bytes=4;return true;}FailureActions a=new FailureActions();a.count=(Mock.Case==38||Mock.Case==51&&Mock.RuntimeTerminated)?1U:0U;Marshal.StructureToPtr(a,buf,false);bytes=(uint)Marshal.SizeOf(typeof(FailureActions));return Mock.Case!=42;}

 sealed class PinnedImage : IDisposable {
  public void Open(){Mock.ImageOpened++;Validate();if(Mock.Case==44)throw new InvalidOperationException("IMAGE_PIN_MISMATCH");}
  public void Validate(){Mock.ImageValidations++;if(Mock.Case==45||Mock.Case==37&&Mock.RuntimeTerminated)throw new InvalidOperationException("IMAGE_ID_SIZE_ATTRIBUTES_MISMATCH");}
  public void Dispose(){Mock.ImageClosed++;if(Mock.Case==48)throw new InvalidOperationException("MODEL_IMAGE_CLOSE_UNKNOWN");}
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
   if(s.pid==ControllerPid)Need(value.groups==17&&value.token.low==139600747&&value.token.high==0&&value.authentication.low==570063&&value.authentication.high==0&&value.modified.low==139600167&&value.modified.high==0,"CONTROLLER_ORIGINAL_TOKEN_MISMATCH");
   if(s.pid==RuntimePid)Need(value.groups==10&&value.token.low==139715724&&value.token.high==0&&value.authentication.low==139715443&&value.authentication.high==0&&value.modified.low==139715493&&value.modified.high==0,"RUNTIME_OBSERVED_TOKEN_MISMATCH");
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
   Console.WriteLine("EXCEPTIONAL_DISPOSITION_PLAN runtime10080_then_controller6120; TWO_FIXED_TERMINATE_CALLS_MAX; EXIT=0xe0000001; WAIT60000_EACH_NOT_NATIVE_CEILING; NO_STOP_NO_UPDATE_NO_CLEANUP; APPLICATION_CLOSE_UNKNOWN");
   Console.WriteLine("Type DISPOSE_RUNTIME_TOKEN_6120_10080_ONCE only after exact owner approval:");
   Need(Mock.Confirm()=="DISPOSE_RUNTIME_TOKEN_6120_10080_ONCE","CONFIRMATION_CANCELLED_NO_RETRY");
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
  Console.WriteLine("RUNTIME_TOKEN_DISPOSITION_RESULT first="+first+" consumed="+consumed+" runtime_submitted="+runtimeSubmitted+" runtime_exit_observed="+runtimeExited+" controller_submitted="+controllerSubmitted+" controller_exit_observed="+controllerExited+" observer_close_known="+closeKnown+"; APPLICATION_CLOSE_AND_ORIGINAL_RUNTIME_BILATERAL_AND_HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY_NO_REFUND_NO_UPDATE_NO_START_NO_CLEANUP");
  return first==0&&runtimeExited&&controllerExited&&closeKnown?0:2;
 }

 public static string OfflineCases(){
  int passed=0;System.IO.TextWriter original=Console.Out;
  for(int c=0;c<=51;c++){
   Mock.Reset(c);System.IO.StringWriter output=new System.IO.StringWriter();int result;
   try{Console.SetOut(output);result=RunFixed();}finally{Console.SetOut(original);}
   string log=output.ToString();bool ok=c==0?result==0:result==2;
   if(!ok||!Mock.RightsOk||Mock.CloseAttempts.Count!=Mock.Opened.Count||Mock.ImageClosed!=Mock.ImageOpened||Mock.RuntimeCalls>1||Mock.ControllerCalls>1)throw new Exception("MODEL_CASE_"+c+" result="+result+" log="+log);
   if(c==0&&(!log.Contains("runtime_exit_observed=True controller_submitted=True controller_exit_observed=True observer_close_known=True")||Mock.RuntimeCalls!=1||Mock.ControllerCalls!=1||Mock.ScmCalls!=4))throw new Exception("MODEL_POSITIVE_INCOMPLETE");
   if(c==21&&(Mock.RuntimeCalls!=0||Mock.ControllerCalls!=0||!log.Contains("consumed=False")))throw new Exception("MODEL_CANCEL_EFFECT");
   if(c>=17&&c<=20||c==48){if(!log.Contains("observer_close_known=False"))throw new Exception("MODEL_CLOSE_UNKNOWN_LOST");}
   if(c==28||c==29||c==30||c==31||c==35||c==36||c==37||c==50||c==51){if(Mock.RuntimeCalls!=1||Mock.ControllerCalls!=0)throw new Exception("MODEL_PARTIAL_NOT_STOPPED_"+c);}
   if(c==38||c==39||c==41||c==42||c==43||c==44||c==45||c==46||c==47||c==49){if(Mock.RuntimeCalls!=0||Mock.ControllerCalls!=0)throw new Exception("MODEL_PRE_GATE_MUTATION_"+c);}
   int before=Mock.ScmCalls;bool denied=false;try{RunFixed();}catch(InvalidOperationException){denied=true;}if(!denied||Mock.ScmCalls!=before)throw new Exception("MODEL_REPLAY");
   passed++;
  }
  return "PASS_EXCEPTIONAL_DISPOSITION cases="+passed+" native_edges=17 IMAGE_HASH_BOUNDARY_MODELED TWO_FIXED_TERMINATIONS_MAX_RUNTIME_THEN_CONTROLLER NO_REAL_TARGET_OPERATION";
 }
}
