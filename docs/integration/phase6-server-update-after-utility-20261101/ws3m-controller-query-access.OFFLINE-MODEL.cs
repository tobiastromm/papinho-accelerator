// Fixed one-off read-only observer. No target arguments or disposition capability.
using System;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
public static class RuntimeQueryAccessReadOnlyModel {
 const uint ControllerPid=19860,QuerySynchronize=0x00101000;
 static uint RuntimePid;
 const long ControllerCreation=134381311017119087L;
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string ControllerSid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string RuntimeSid="S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099";
 static int used;
 public static class Mock {
  public static int Case,ScmCalls,RuntimeStats,LastError;public static bool RightsOk;
  public static System.Collections.Generic.HashSet<long> Opened=new System.Collections.Generic.HashSet<long>(),Closed=new System.Collections.Generic.HashSet<long>();
  public static void Reset(int c){Case=c;ScmCalls=RuntimeStats=0;LastError=5;RightsOk=true;Opened.Clear();Closed.Clear();used=0;RuntimePid=0;}
  public static string Confirm(){return Case==19?"CANCEL":"OBSERVE_WS3M_CONTROLLER_QUERY_ACCESS_ONCE";}
  public static void Open(long h){if(!Opened.Add(h))throw new Exception("MODEL_DUPLICATE_OPEN");}
  public static void Close(long h){if(!Opened.Contains(h)||!Closed.Add(h))throw new Exception("MODEL_CLOSE_NOT_OWNED_OR_REPEATED");}
  public static uint Sid(IntPtr buf,int offset,string value){SecurityIdentifier sid=new SecurityIdentifier(value);byte[] bytes=new byte[sid.BinaryLength];sid.GetBinaryForm(bytes,0);Marshal.Copy(bytes,0,IntPtr.Add(buf,offset),bytes.Length);return (uint)bytes.Length;}
  public static uint Groups(IntPtr buf,int cls){int offset=(int)Marshal.OffsetOf(typeof(GroupsHead),"first"),entry=Marshal.SizeOf(typeof(SidAttr));int count=cls==2?(Case==30?9:10):(Case==8?1:0);int tail=offset+count*entry;Marshal.WriteInt32(buf,Case==6&&cls==2||Case==26&&cls==11?4096:count);for(int i=0;i<count;i++){SidAttr v=new SidAttr();v.sid=IntPtr.Add(buf,tail);v.attributes=4;if(Case==5&&i==0)v.attributes=16;string sid=cls==2&&i==0&&(Case==4||Case==5)?"S-1-5-32-544":i==0?RuntimeSid:i==1?"S-1-5-6":"S-1-1-0";tail+=(int)Sid(buf,tail,sid);if(Case==7&&i==0)v.sid=IntPtr.Add(buf,9000);Marshal.StructureToPtr(v,IntPtr.Add(buf,offset+i*entry),false);}return Case==25&&cls==2?0U:(uint)tail;}
 }

 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high;public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 sealed class Snapshot {public IntPtr process,token;public uint pid;public long creation;public Stats statistics;public string sid;public System.Collections.Generic.List<string> details=new System.Collections.Generic.List<string>();}
 static IntPtr OpenProcess(uint rights,bool inherit,uint pid){Mock.RightsOk&=rights==(pid==ControllerPid?0x00121000U:0x00101000U)&&!inherit&&(pid==19860||pid==19836);if(Mock.Case==20&&pid==19836)return IntPtr.Zero;Mock.Open(pid);return new IntPtr(pid);}
 static bool CloseHandle(IntPtr h){Mock.Close(h.ToInt64());return !(Mock.Case==15&&h.ToInt64()==19836);}
 static uint GetProcessId(IntPtr h){return (uint)h.ToInt64();}
 static bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u){long value=ControllerCreation+(h.ToInt64()==19836?1000:0);if(Mock.Case==14&&h.ToInt64()==19860)value++;if(Mock.Case==28&&h.ToInt64()==19836)value=ControllerCreation-1;c=new Times();c.low=(uint)value;c.high=(uint)(value>>32);e=k=u=new Times();return true;}
 static bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size){Mock.RightsOk&=flags==0&&size==1024;text.Append(Image);size=(uint)text.Length;return true;}
 static uint WaitForSingleObject(IntPtr h,uint ms){Mock.RightsOk&=ms==0;return 258;}
 static bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token){Mock.RightsOk&=rights==(h.ToInt64()==ControllerPid?0x00020008U:8U);token=new IntPtr(100000+h.ToInt64());Mock.Open(token.ToInt64());return true;}
 static bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint bytes){bool ctrl=h.ToInt64()==119860;bytes=0;if(cls==20||cls==18||cls==21||cls==12){if(cap!=4){Mock.LastError=24;return false;}Mock.RightsOk&=!ctrl;if(Mock.Case==2&&cls==20||Mock.Case==22&&cls==21||Mock.Case==36&&cls==20||Mock.Case==41&&cls==21){Mock.LastError=24;return false;}int v=cls==20?(Mock.Case==1?0:1):cls==18?(Mock.Case==3?99:Mock.Case==16?2:Mock.Case==17?3:1):cls==21?(Mock.Case==8||Mock.Case==31?1:0):(Mock.Case==29?1:0);if(cls==21){v=Mock.Case==43?2:Mock.Case==44?255:v;Marshal.WriteByte(data,(byte)v);if(Mock.Case==46)for(int i=1;i<4;i++)Marshal.WriteByte(data,i,0xa5);}else Marshal.WriteInt32(data,v);bytes=Mock.Case==35&&cls==20?8U:Mock.Case==37&&cls==18?0U:Mock.Case==39&&cls==21?0U:Mock.Case==40&&cls==21?8U:Mock.Case==42&&cls==21?4U:cls==21?1U:4U;return true;}if(cap==8192){Mock.RightsOk&=!ctrl;if(Mock.Case==2&&cls==20||Mock.Case==21&&cls==2||Mock.Case==22&&cls==21||Mock.Case==23&&cls==3)return false;if(cls==2||cls==11){bytes=Mock.Groups(data,cls);return true;}if(cls==3){int offset=(int)Marshal.OffsetOf(typeof(PrivHead),"first"),entry=Marshal.SizeOf(typeof(PrivAttr));Marshal.WriteInt32(data,Mock.Case==9?4096:1);PrivAttr v=new PrivAttr();v.luid.low=23;v.attributes=3;Marshal.StructureToPtr(v,IntPtr.Add(data,offset),false);bytes=(uint)(offset+entry);return true;}if(cls==25){SidAttr v=new SidAttr();int offset=Marshal.SizeOf(typeof(SidAttr));v.sid=IntPtr.Add(data,offset);v.attributes=32;bytes=(uint)offset+Mock.Sid(data,offset,Mock.Case==32?"S-1-16-12288":"S-1-16-8192");if(Mock.Case==10)v.sid=IntPtr.Add(data,9000);Marshal.StructureToPtr(v,data,false);if(Mock.Case==24)bytes=4;return true;}throw new Exception("MODEL_UNKNOWN_SHAPE_CLASS");}Mock.RightsOk&=cap==512&&(cls==1||cls==10);if(cls==1){string user=Mock.Case==12&&!ctrl?ControllerSid:ctrl?ControllerSid:RuntimeSid;Marshal.WriteIntPtr(data,IntPtr.Add(data,IntPtr.Size));bytes=(uint)IntPtr.Size+Mock.Sid(data,IntPtr.Size,user);return true;}Stats st=new Stats();st.type=1;st.groups=ctrl?17U:10U;st.token.low=ctrl?164979794U:123456U;st.authentication.low=ctrl?570063U:234567U;st.modified.low=ctrl?164979462U:345678U;if(!ctrl)Mock.RuntimeStats++;if(Mock.Case==18&&ctrl)st.token.low++;if(Mock.Case==11&&!ctrl&&Mock.RuntimeStats>1)st.modified.low++;Marshal.StructureToPtr(st,data,false);bytes=(uint)Marshal.SizeOf(typeof(Stats));return true;}
 static IntPtr OpenSCManagerW(string machine,string db,uint rights){Mock.RightsOk&=machine==null&&db==null&&rights==1;Mock.Open(31);return new IntPtr(31);}
 static IntPtr OpenServiceW(IntPtr manager,string name,uint rights){Mock.RightsOk&=manager.ToInt64()==31&&name=="PapinhoAccelerator"&&rights==4;Mock.Open(32);return new IntPtr(32);}
 static bool QueryServiceStatusEx(IntPtr service,int cls,out Status value,uint cap,out uint bytes){Mock.RightsOk&=service.ToInt64()==32&&cls==0&&cap==36;Mock.ScmCalls++;value=new Status();value.type=16;value.state=2;value.pid=Mock.Case==27?0U:Mock.Case==13&&Mock.ScmCalls==2?26001U:19836U;bytes=36;return true;}
 static bool CloseServiceHandle(IntPtr h){Mock.Close(h.ToInt64());return true;}
 static bool LookupPrivilegeNameW(string system,ref Luid luid,StringBuilder text,ref uint chars){Mock.RightsOk&=system==null&&luid.low==23&&chars==256;text.Append("SeChangeNotifyPrivilege");chars=Mock.Case==33?0U:(uint)text.Length;return Mock.Case!=34;}
 static bool GetKernelObjectSecurity(IntPtr handle,uint flags,IntPtr buffer,uint capacity,out uint needed){
   Mock.RightsOk&=flags==5U&&capacity==16384U&&(handle.ToInt64()==ControllerPid||handle.ToInt64()==100000+ControllerPid);
   needed=0;if(Mock.Case==47){Mock.LastError=5;return false;}
   if(Mock.Case==48){needed=16385U;return true;}
   if(Mock.Case==49){needed=20U;for(int i=0;i<20;i++)Marshal.WriteByte(buffer,i,0);return true;}
   System.Security.AccessControl.RawSecurityDescriptor sd=new System.Security.AccessControl.RawSecurityDescriptor("O:SYD:(A;;GA;;;SY)");
   byte[] bytes=new byte[sd.BinaryLength];sd.GetBinaryForm(bytes,0);needed=(uint)bytes.Length;Marshal.Copy(bytes,0,buffer,bytes.Length);return true;
  }
 static void Need(bool ok,string stage){if(!ok)throw new InvalidOperationException(stage);}
 static void Native(bool ok,string stage){if(!ok)throw new Win32Exception(Mock.LastError,stage);}
 static void Scm(IntPtr service,string stage,bool capture){Status value;uint n;Native(QueryServiceStatusEx(service,0,out value,36,out n),stage+"_QUERY");Need(n==36&&value.type==16&&value.state==2&&value.pid!=0&&value.pid!=ControllerPid&&value.pid==19836U&&value.win32==0&&value.specific==0,stage+"_MISMATCH_NO_FALLBACK");if(capture)RuntimePid=value.pid;else Need(value.pid==RuntimePid,stage+"_PID_CHANGED");}
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
   if(s.pid==ControllerPid)Need(value.groups==17&&value.token.low==164979794&&value.token.high==0&&value.authentication.low==570063&&value.authentication.high==0&&value.modified.low==164979462&&value.modified.high==0,"CONTROLLER_ORIGINAL_TOKEN_MISMATCH");
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
 static uint Scalar(IntPtr token,int cls,IntPtr buf){bool byteClass=cls==21;if(byteClass){Need(buf!=IntPtr.Zero&&(buf.ToInt64()&3)==0,"CLASS21_DWORD_ALIGNMENT_DENY");Marshal.WriteInt32(buf,0);}uint n;bool succeeded=GetTokenInformation(token,cls,buf,4,out n);int error=succeeded?0:Mock.LastError;Exception outputFailure=null;try{Console.WriteLine("SCALAR_QUERY_CAPTURE class="+cls+" requested=4 succeeded="+succeeded+" return_length_snapshot="+n+" return_length_known="+succeeded+" native_error_known="+(!succeeded)+" native_error_snapshot="+error+"; ORIGINAL_CALL_ONLY_FAILED_RETURN_LENGTH_NOT_ATTESTED_NO_NEW_QUERY");}catch(Exception ex){outputFailure=ex;}if(!succeeded)throw new Win32Exception(error,"SHAPE_FIXED_QUERY_CLASS_"+cls);uint expected=byteClass?1U:4U;Need(n==expected,"SHAPE_SCALAR_LENGTH_CLASS_"+cls+"_EXPECTED_"+expected+"_RETURNED_"+n);if(outputFailure!=null)throw new InvalidOperationException("SCALAR_CAPTURE_OUTPUT_FAILED");uint value=byteClass?(uint)Marshal.ReadByte(buf):unchecked((uint)Marshal.ReadInt32(buf));Console.WriteLine("SCALAR_VALUE_CAPTURE class="+cls+" value="+value+"; VALIDATED_CLASS_SPECIFIC_LENGTH_ONLY_NOT_FINAL_REVALIDATION_NOT_AUTHORITY");return value;}
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
 static string SecurityDescriptor(IntPtr handle,string role){
  IntPtr buffer=Marshal.AllocHGlobal(16384);
  try {uint n;bool ok=GetKernelObjectSecurity(handle,5U,buffer,16384U,out n);int error=ok?0:Mock.LastError;
   if(!ok)throw new Win32Exception(error,"SD_QUERY_"+role);
   Need(n>=20U&&n<=16384U,"SD_LENGTH_"+role+"_"+n);
   byte[] bytes=new byte[(int)n];Marshal.Copy(buffer,bytes,0,(int)n);
   Need((((uint)bytes[2]|((uint)bytes[3]<<8))&0x8000U)!=0,"SD_NOT_SELF_RELATIVE_"+role);
   System.Security.AccessControl.RawSecurityDescriptor sd=new System.Security.AccessControl.RawSecurityDescriptor(bytes,0);
   Need(sd.DiscretionaryAcl==null||sd.DiscretionaryAcl.Count<=256,"SD_ACE_BOUND_"+role);
   string owner=sd.Owner==null?"NONE":sd.Owner.Value;
   return "CONTROLLER_SD role="+role+" bytes="+n+" owner="+owner+" dacl_null="+(sd.DiscretionaryAcl==null)+" flags="+sd.ControlFlags+" sddl="+sd.GetSddlForm(System.Security.AccessControl.AccessControlSections.Owner|System.Security.AccessControl.AccessControlSections.Access)+"; CURRENT_DESCRIPTOR_DATA_NOT_EFFECTIVE_ACCESS_PROOF_NOT_HISTORICAL_FIRST_CAUSE_NO_SACL";
  }finally{Marshal.FreeHGlobal(buffer);}
 }
 static void Capture(Snapshot s){s.process=OpenProcess(s.pid==ControllerPid?QuerySynchronize|0x00020000U:QuerySynchronize,false,s.pid);Native(s.process!=IntPtr.Zero,"PROCESS_OPEN_QUERY_ONLY");ProcessCurrent(s,true);Native(OpenProcessToken(s.process,s.pid==ControllerPid?0x00020008U:8U,out s.token),"TOKEN_OPEN_QUERY_ONLY");TokenCurrent(s,true);}
 static void Revalidate(Snapshot s){ProcessCurrent(s,false);TokenCurrent(s,false);ProcessCurrent(s,false);}
 static void Print(Snapshot s,string role){Console.WriteLine("PARTICIPANT_OBSERVED role="+role+" pid="+s.pid+" creation="+s.creation+" token.low="+s.statistics.token.low+" token.high="+s.statistics.token.high+" authentication.low="+s.statistics.authentication.low+" authentication.high="+s.statistics.authentication.high+" modified.low="+s.statistics.modified.low+" modified.high="+s.statistics.modified.high+" groups="+s.statistics.groups+" sid="+s.sid+"; CURRENT_CORRELATION_ONLY_NOT_HISTORICAL_ORIGINAL_PROOF_NOT_AUTHORITY");}
 public static int RunFixed(){
  Need(Interlocked.CompareExchange(ref used,1,0)==0,"NO_RETRY");
  Snapshot controller=new Snapshot(),runtime=new Snapshot();controller.pid=ControllerPid;runtime.pid=0;
  IntPtr manager=IntPtr.Zero,service=IntPtr.Zero;int first=0;bool consumed=false,complete=false,closeKnown=true;
  Console.WriteLine("ONE_READONLY_RUNTIME_ELEVATION_SHAPE; NO_STOP_NO_KILL_NO_START_NO_UPDATE_NO_RETRY; PID_IS_LOCATOR_ONLY");
  try {
   Console.WriteLine("Type OBSERVE_WS3M_CONTROLLER_QUERY_ACCESS_ONCE only after specific owner approval:");
   Need(Mock.Confirm()=="OBSERVE_WS3M_CONTROLLER_QUERY_ACCESS_ONCE","CONFIRMATION_CANCELLED");
   consumed=true;
   manager=OpenSCManagerW(null,null,1);Native(manager!=IntPtr.Zero,"SCM_CONNECT_QUERY_ONLY");
   service=OpenServiceW(manager,"PapinhoAccelerator",4);Native(service!=IntPtr.Zero,"SCM_OPEN_QUERY_STATUS_ONLY");
   Scm(service,"SCM_BEFORE",true);runtime.pid=RuntimePid;Capture(controller);Capture(runtime);Shape(runtime);runtime.details.Add(SecurityDescriptor(controller.process,"PROCESS"));runtime.details.Add(SecurityDescriptor(controller.token,"TOKEN"));
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

 sealed class FailOutput : System.IO.StringWriter {public override void WriteLine(string value){throw new System.IO.IOException("MODEL_OUTPUT_FAILED");}}
 public static string OfflineCases(){
  int lengthChecks=0;IntPtr probe=Marshal.AllocHGlobal(8192);System.IO.TextWriter probeOutput=Console.Out;Console.SetOut(new System.IO.StringWriter());
  try{Mock.Reset(0);bool oldDenied=false;try{ShapeQuery(new IntPtr(119836),20,probe);}catch(Win32Exception ex){if(ex.NativeErrorCode!=24)throw;oldDenied=true;}if(!oldDenied)throw new Exception("MODEL_OLD_SCALAR_PATH_NOT_REJECTED");foreach(int cls in new int[]{20,18,21,12}){Mock.Reset(0);uint n;foreach(uint bad in new uint[]{0,3,8,8192}){if(GetTokenInformation(new IntPtr(119836),cls,probe,bad,out n)||Mock.LastError!=24)throw new Exception("MODEL_FIXED_BAD_LENGTH_ACCEPTED");lengthChecks++;}Mock.Reset(0);Scalar(new IntPtr(119836),cls,probe);if(!Mock.RightsOk)throw new Exception("MODEL_FIXED_GOOD_LENGTH_DENIED");lengthChecks++;}}finally{Marshal.FreeHGlobal(probe);Console.SetOut(probeOutput);}
  IntPtr alignProbe=Marshal.AllocHGlobal(8);try{Mock.Reset(0);bool denied=false;try{Scalar(new IntPtr(119836),21,IntPtr.Add(alignProbe,1));}catch(InvalidOperationException e){denied=e.Message=="CLASS21_DWORD_ALIGNMENT_DENY";}if(!denied)throw new Exception("MODEL_UNALIGNED_CLASS21_ACCEPTED");}finally{Marshal.FreeHGlobal(alignProbe);}
  System.IO.TextWriter savedOutput=Console.Out;IntPtr outputProbe=Marshal.AllocHGlobal(8);int outputChecks=0;
  try{foreach(int fault in new int[]{41,39,0}){Mock.Reset(fault);Console.SetOut(new FailOutput());bool correct=false;try{Scalar(new IntPtr(119836),fault==0?20:21,outputProbe);}catch(Win32Exception e){correct=fault==41&&e.NativeErrorCode==24;}catch(InvalidOperationException e){correct=fault==39?e.Message=="SHAPE_SCALAR_LENGTH_CLASS_21_EXPECTED_1_RETURNED_0":fault==0&&e.Message=="SCALAR_CAPTURE_OUTPUT_FAILED";}finally{Console.SetOut(savedOutput);}if(!correct)throw new Exception("MODEL_OUTPUT_REPLACED_FIRST_CAUSE");outputChecks++;}}finally{Console.SetOut(savedOutput);Marshal.FreeHGlobal(outputProbe);}
  int passed=0;System.IO.TextWriter original=Console.Out;
  for(int c=0;c<=49;c++){
   Mock.Reset(c);System.IO.StringWriter output=new System.IO.StringWriter();int result;
   try{Console.SetOut(output);result=RunFixed();}finally{Console.SetOut(original);}
   bool good=c==0||c==1||c==4||c==5||c==8||c==16||c==17||c==29||c==31||c==32||c==38||c==43||c==44||c==45||c==46;string log=output.ToString();
   if(result!=(good?0:2)||!Mock.RightsOk||Mock.Opened.Count!=Mock.Closed.Count)throw new Exception("MODEL_CASE_"+c+" result="+result+" log="+log);
   if(good&&(!log.Contains("complete=True observer_close_known=True")||!log.Contains("role=RUNTIME_CURRENT_SCM_CANDIDATE pid=19836")||Mock.ScmCalls!=2))throw new Exception("MODEL_INCOMPLETE");
   if(good&&!log.Contains("CONTROLLER_SD role=TOKEN"))throw new Exception("MODEL_SD_NOT_CAPTURED");
   if(c>=47&&log.Contains("CONTROLLER_SD role="))throw new Exception("MODEL_PARTIAL_SD_PUBLISHED");
   if(c==0&&!log.Contains("raw=1 type=1"))throw new Exception("MODEL_ELEVATED_SCRUBBED");
   if(c==4&&!log.Contains("administrators_enabled_entry=True"))throw new Exception("MODEL_ADMIN_MEMBERSHIP_HIDDEN");
   if(c==5&&log.Contains("administrators_enabled_entry=True"))throw new Exception("MODEL_DENY_ONLY_AS_ENABLED");
   if(c==15&&!log.Contains("observer_close_known=False"))throw new Exception("MODEL_CLOSE_UNKNOWN");
   if(c==19&&(Mock.Opened.Count!=0||!log.Contains("consumed=False")))throw new Exception("MODEL_CANCEL_EFFECT");
   if(!good&&c!=15&&log.Contains("TOKEN_ELEVATION_OBSERVED"))throw new Exception("MODEL_PARTIAL_SHAPE_PUBLISHED");
   if(c==38&&!log.Contains("SCALAR_VALUE_CAPTURE class=21"))throw new Exception("MODEL_ONE_BYTE_CLASS21_DENIED");
   if(c==39&&!log.Contains("SHAPE_SCALAR_LENGTH_CLASS_21_EXPECTED_1_RETURNED_0"))throw new Exception("MODEL_ZERO_LENGTH_LOST");
   if(c==40&&!log.Contains("SHAPE_SCALAR_LENGTH_CLASS_21_EXPECTED_1_RETURNED_8"))throw new Exception("MODEL_OVERSIZE_LENGTH_LOST");
   if((c==39||c==40||c==41||c==42)&&log.Contains("SCALAR_VALUE_CAPTURE class=21"))throw new Exception("MODEL_INVALID_VALUE_READ_OR_PUBLISHED");
   if(c==36&&!log.Contains("native_error_known=True native_error_snapshot=24"))throw new Exception("MODEL_NATIVE_ERROR_REPLACED");
   if(good&&!log.Contains("SCALAR_VALUE_CAPTURE class=20"))throw new Exception("MODEL_VALID_SCALAR_NOT_CAPTURED");

   if(c==42&&!log.Contains("SHAPE_SCALAR_LENGTH_CLASS_21_EXPECTED_1_RETURNED_4"))throw new Exception("MODEL_CLASS21_DWORD_NOT_DENIED");
   if(c==43&&!log.Contains("has_ever_been_filtered_raw=2"))throw new Exception("MODEL_BOOLEAN_NONZERO_2_LOST");
   if(c==44&&!log.Contains("has_ever_been_filtered_raw=255"))throw new Exception("MODEL_BOOLEAN_NONZERO_255_LOST");
   if(c==46&&!log.Contains("has_ever_been_filtered_raw=0"))throw new Exception("MODEL_CLASS21_TRAILING_BYTES_CONSUMED");
   int before=Mock.ScmCalls;bool denied=false;try{RunFixed();}catch(InvalidOperationException){denied=true;}if(!denied||Mock.ScmCalls!=before)throw new Exception("MODEL_REPLAY");passed++;
  }
  return "PASS_RUNTIME_SHAPE cases="+passed+" fixed_length_checks="+lengthChecks+" alignment_checks=1 output_preservation_checks="+outputChecks+" old_scalar_path_error=24 native_edges=14 ELEVATION_NOT_ADMISSION NO_TOKEN_MUTATION_NO_REAL_QUERY";
 }
}
