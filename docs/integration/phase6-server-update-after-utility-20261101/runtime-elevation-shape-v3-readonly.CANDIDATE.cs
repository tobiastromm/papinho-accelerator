// Fixed one-off read-only observer. No target arguments or disposition capability.
using System;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
public static class RuntimeElevationReadOnlyV3 {
 const uint ControllerPid=6120,QuerySynchronize=0x00101000;
 static uint RuntimePid;
 const long ControllerCreation=134381245905057064L;
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string ControllerSid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string RuntimeSid="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 static int used;
 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high;public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 sealed class Snapshot {public IntPtr process,token;public uint pid;public long creation;public Stats statistics;public string sid;public System.Collections.Generic.List<string> details=new System.Collections.Generic.List<string>();}
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
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern bool LookupPrivilegeNameW(string system,ref Luid luid,StringBuilder text,ref uint chars);
 static void Need(bool ok,string stage){if(!ok)throw new InvalidOperationException(stage);}
 static void Native(bool ok,string stage){if(!ok)throw new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 static void Scm(IntPtr service,string stage,bool capture){Status value;uint n;Native(QueryServiceStatusEx(service,0,out value,36,out n),stage+"_QUERY");Need(n==36&&value.type==16&&value.state==2&&value.pid!=0&&value.pid!=ControllerPid&&value.win32==0&&value.specific==0,stage+"_MISMATCH_NO_FALLBACK");if(capture)RuntimePid=value.pid;else Need(value.pid==RuntimePid,stage+"_PID_CHANGED");}
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
   if(s.pid==ControllerPid)Need(value.groups==17&&value.token.low==139600747&&value.token.high==0&&value.authentication.low==570063&&value.authentication.high==0&&value.modified.low==139600167&&value.modified.high==0,"CONTROLLER_ORIGINAL_TOKEN_MISMATCH");
   if(initial){s.statistics=value;s.sid=sid;}
   else Need(Same(value.token,s.statistics.token)&&Same(value.authentication,s.statistics.authentication)&&Same(value.modified,s.statistics.modified)&&value.groups==s.statistics.groups&&sid==s.sid,"TOKEN_CONTINUITY_MISMATCH");
  }finally{Marshal.FreeHGlobal(buf);}
 }

 [StructLayout(LayoutKind.Sequential)] struct SidAttr {public IntPtr sid;public uint attributes;}
 [StructLayout(LayoutKind.Sequential)] struct GroupsHead {public uint count;public SidAttr first;}
 [StructLayout(LayoutKind.Sequential)] struct PrivAttr {public Luid luid;public uint attributes;}
 [StructLayout(LayoutKind.Sequential)] struct PrivHead {public uint count;public PrivAttr first;}
 static string BufferSid(IntPtr buf,uint n,IntPtr sid){long offset=sid.ToInt64()-buf.ToInt64();Need(n>=8&&sid!=IntPtr.Zero&&offset>=0&&offset<=(long)n-8,"SHAPE_SID_RANGE");int length=8+4*Marshal.ReadByte(sid,1);Need(length<=68&&offset+length<=n,"SHAPE_SID_LENGTH");return new SecurityIdentifier(sid).Value;}
 static uint ShapeQuery(IntPtr token,int cls,IntPtr buf){uint n;Native(GetTokenInformation(token,cls,buf,8192,out n),"SHAPE_QUERY_CLASS_"+cls);Need(n>0&&n<=8192,"SHAPE_QUERY_LENGTH_CLASS_"+cls);return n;}
 static uint Scalar(IntPtr token,int cls,IntPtr buf){bool byteClass=cls==21;if(byteClass){Need(buf!=IntPtr.Zero&&(buf.ToInt64()&3)==0,"CLASS21_DWORD_ALIGNMENT_DENY");Marshal.WriteInt32(buf,0);}uint n;bool succeeded=GetTokenInformation(token,cls,buf,4,out n);int error=succeeded?0:Marshal.GetLastWin32Error();Exception outputFailure=null;try{Console.WriteLine("SCALAR_QUERY_CAPTURE class="+cls+" requested=4 succeeded="+succeeded+" return_length_snapshot="+n+" return_length_known="+succeeded+" native_error_known="+(!succeeded)+" native_error_snapshot="+error+"; ORIGINAL_CALL_ONLY_FAILED_RETURN_LENGTH_NOT_ATTESTED_NO_NEW_QUERY");}catch(Exception ex){outputFailure=ex;}if(!succeeded)throw new Win32Exception(error,"SHAPE_FIXED_QUERY_CLASS_"+cls);uint expected=byteClass?1U:4U;Need(n==expected,"SHAPE_SCALAR_LENGTH_CLASS_"+cls+"_EXPECTED_"+expected+"_RETURNED_"+n);if(outputFailure!=null)throw new InvalidOperationException("SCALAR_CAPTURE_OUTPUT_FAILED");uint value=byteClass?(uint)Marshal.ReadByte(buf):unchecked((uint)Marshal.ReadInt32(buf));Console.WriteLine("SCALAR_VALUE_CAPTURE class="+cls+" value="+value+"; VALIDATED_CLASS_SPECIFIC_LENGTH_ONLY_NOT_FINAL_REVALIDATION_NOT_AUTHORITY");return value;}
 static void GroupsData(Snapshot s,IntPtr buf,int cls){
  uint n=ShapeQuery(s.token,cls,buf);int offset=(int)Marshal.OffsetOf(typeof(GroupsHead),"first"),entry=Marshal.SizeOf(typeof(SidAttr));Need(n>=offset,"SHAPE_GROUPS_HEADER");uint count=unchecked((uint)Marshal.ReadInt32(buf));Need(count<=(n-offset)/entry,"SHAPE_GROUPS_COUNT");if(cls==2)Need(count==s.statistics.groups,"SHAPE_GROUPS_STATS_MISMATCH");
  s.details.Add("TOKEN_SHAPE class="+cls+" count="+count+"; OBSERVATION_NOT_ADMISSION");
  for(uint i=0;i<count;i++){SidAttr v=(SidAttr)Marshal.PtrToStructure(IntPtr.Add(buf,offset+(int)i*entry),typeof(SidAttr));string sid=BufferSid(buf,n,v.sid);bool admin=sid=="S-1-5-32-544"&&(v.attributes&4)!=0&&(v.attributes&16)==0;s.details.Add("TOKEN_GROUP class="+cls+" index="+i+" sid="+sid+" attributes="+v.attributes+" administrators_enabled_entry="+admin+"; ENTRY_NOT_FULL_EFFECTIVE_ACCESS_PROOF");}
 }
 static void Shape(Snapshot s){
  IntPtr buf=Marshal.AllocHGlobal(8192);
  try {
   uint elevated=Scalar(s.token,20,buf),type=Scalar(s.token,18,buf);Need(type>=1&&type<=3,"SHAPE_ELEVATION_TYPE_INVALID");
   s.details.Add("TOKEN_ELEVATION_OBSERVED raw="+elevated+" type="+type+"; NONZERO_NOT_AUTOMATICALLY_ADMITTED_DEFAULT_NOT_NON_ELEVATION_PROOF");
   GroupsData(s,buf,2);GroupsData(s,buf,11);
   uint has=Scalar(s.token,21,buf),session=Scalar(s.token,12,buf);s.details.Add("TOKEN_CONTEXT has_ever_been_filtered_raw="+has+" session="+session+"; NO_AUTHORITY");
   uint n=ShapeQuery(s.token,3,buf);int offset=(int)Marshal.OffsetOf(typeof(PrivHead),"first"),entry=Marshal.SizeOf(typeof(PrivAttr));Need(n>=offset,"SHAPE_PRIVILEGES_HEADER");uint count=unchecked((uint)Marshal.ReadInt32(buf));Need(count<=(n-offset)/entry,"SHAPE_PRIVILEGES_COUNT");s.details.Add("TOKEN_PRIVILEGES count="+count);
   for(uint i=0;i<count;i++){PrivAttr v=(PrivAttr)Marshal.PtrToStructure(IntPtr.Add(buf,offset+(int)i*entry),typeof(PrivAttr));StringBuilder name=new StringBuilder(256);uint chars=256;Native(LookupPrivilegeNameW(null,ref v.luid,name,ref chars),"SHAPE_PRIVILEGE_NAME_QUERY");Need(chars>0&&chars<256,"SHAPE_PRIVILEGE_NAME_LENGTH");s.details.Add("TOKEN_PRIVILEGE index="+i+" luid.low="+v.luid.low+" luid.high="+v.luid.high+" name="+name.ToString()+" attributes="+v.attributes+"; NOT_A_GRANTED_ACTION");}
   n=ShapeQuery(s.token,25,buf);Need(n>=Marshal.SizeOf(typeof(SidAttr)),"SHAPE_INTEGRITY_LENGTH");SidAttr integrity=(SidAttr)Marshal.PtrToStructure(buf,typeof(SidAttr));s.details.Add("TOKEN_INTEGRITY sid="+BufferSid(buf,n,integrity.sid)+" attributes="+integrity.attributes+"; NO_AUTHORITY");
  }finally{Marshal.FreeHGlobal(buf);}
 }
 static void Capture(Snapshot s){s.process=OpenProcess(QuerySynchronize,false,s.pid);Native(s.process!=IntPtr.Zero,"PROCESS_OPEN_QUERY_ONLY");ProcessCurrent(s,true);Native(OpenProcessToken(s.process,8,out s.token),"TOKEN_OPEN_QUERY_ONLY");TokenCurrent(s,true);}
 static void Revalidate(Snapshot s){ProcessCurrent(s,false);TokenCurrent(s,false);ProcessCurrent(s,false);}
 static void Print(Snapshot s,string role){Console.WriteLine("PARTICIPANT_OBSERVED role="+role+" pid="+s.pid+" creation="+s.creation+" token.low="+s.statistics.token.low+" token.high="+s.statistics.token.high+" authentication.low="+s.statistics.authentication.low+" authentication.high="+s.statistics.authentication.high+" modified.low="+s.statistics.modified.low+" modified.high="+s.statistics.modified.high+" groups="+s.statistics.groups+" sid="+s.sid+"; CURRENT_CORRELATION_ONLY_NOT_HISTORICAL_ORIGINAL_PROOF_NOT_AUTHORITY");}
 public static int RunFixed(){
  Need(Interlocked.CompareExchange(ref used,1,0)==0,"NO_RETRY");
  Snapshot controller=new Snapshot(),runtime=new Snapshot();controller.pid=ControllerPid;runtime.pid=0;
  IntPtr manager=IntPtr.Zero,service=IntPtr.Zero;int first=0;bool consumed=false,complete=false,closeKnown=true;
  Console.WriteLine("ONE_READONLY_RUNTIME_ELEVATION_SHAPE; NO_STOP_NO_KILL_NO_START_NO_UPDATE_NO_RETRY; PID_IS_LOCATOR_ONLY");
  try {
   Console.WriteLine("Type OBSERVE_RUNTIME_ELEVATION_SHAPE_V3_ONCE only after specific owner approval:");
   Need(Console.ReadLine()=="OBSERVE_RUNTIME_ELEVATION_SHAPE_V3_ONCE","CONFIRMATION_CANCELLED");
   consumed=true;
   manager=OpenSCManagerW(null,null,1);Native(manager!=IntPtr.Zero,"SCM_CONNECT_QUERY_ONLY");
   service=OpenServiceW(manager,"PapinhoAccelerator",4);Native(service!=IntPtr.Zero,"SCM_OPEN_QUERY_STATUS_ONLY");
   Scm(service,"SCM_BEFORE",true);runtime.pid=RuntimePid;Capture(controller);Capture(runtime);Shape(runtime);
   Need(controller.creation<=runtime.creation&&controller.pid!=runtime.pid,"PARTICIPANT_ORDER_MISMATCH");
   Revalidate(controller);Revalidate(runtime);Scm(service,"SCM_AFTER",false);
   Print(controller,"CONTROLLER_ORIGINAL_MATCH");Print(runtime,"RUNTIME_CURRENT_SCM_CANDIDATE");foreach(string detail in runtime.details)Console.WriteLine(detail);complete=true;
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
