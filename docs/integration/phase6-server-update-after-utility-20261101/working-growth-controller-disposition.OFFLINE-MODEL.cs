// One-off candidate disposition adapter, not a service/authority producer.
// Fixed PID + creation + disk pins + token scope. No arguments select a target.
using System;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Security.Cryptography;
using System.Threading;
using Microsoft.Win32.SafeHandles;
public static class WorkingGrowthDispositionModel {
 public static class Mock {public static int Case,Opened,Closed,StatsCalls,ScmCalls,Terminations; public static void Reset(int c){Case=c;Opened=Closed=StatsCalls=ScmCalls=Terminations=0;used=0;}}
 const uint Pid=21484, Exit=0xe0000001, Rights=0x00101001;
 const long Creation=134381297590479169L;
 const string Image=@"C:\Program Files\Papinho\Accelerator\papacc_server.exe";
 const string Sid="S-1-5-21-785699550-1440458426-1682014700-1108";
 const string Pin="39a9da2f53ad6208ed875a916debd9c2fec760826f7a17d702d65afa28095b15";
 static int used;
 [StructLayout(LayoutKind.Sequential)] struct Times {public uint low,high; public long Value {get{return ((long)high<<32)|low;}}}
 [StructLayout(LayoutKind.Sequential)] struct FileInfo {public uint attributes;public Times creation,access,write;public uint volume,sizeHigh,sizeLow,links,indexHigh,indexLow;}
 [StructLayout(LayoutKind.Sequential)] struct Status {public uint type,state,controls,win32,specific,checkpoint,hint,pid,flags;}
 [StructLayout(LayoutKind.Sequential)] struct Luid {public uint low;public int high;}
 [StructLayout(LayoutKind.Sequential)] struct Stats {public Luid token,authentication;public long expiration;public uint type,impersonation,dynamicCharged,dynamicAvailable,groups,privileges;public Luid modified;}
 static IntPtr OpenProcess(uint rights,bool inherit,uint pid){Mock.Opened++; return Mock.Case==1?IntPtr.Zero:(IntPtr)3;}
 static bool CloseHandle(IntPtr h){Mock.Closed++; return Mock.Case!=19||h!=(IntPtr)3;}
 static uint GetProcessId(IntPtr h){return Mock.Case==2?149U:21484U;}
 static bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u){c=new Times();e=k=u=new Times();c.low=55978305;c.high=31288084;if(Mock.Case==3)c.low++;return true;}
 static bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size){text.Append(Mock.Case==4?@"C:\Wrong\papacc_server.exe":Image);size=(uint)text.Length;return true;}
 static uint WaitForSingleObject(IntPtr h,uint ms){if(ms==0)return Mock.Case==13?0U:258U;return Mock.Case==15?258U:0U;}
 static bool GetExitCodeProcess(IntPtr h,out uint code){code=Mock.Case==17?5U:Exit;return Mock.Case!=16;}
 static bool TerminateProcess(IntPtr h,uint code){Mock.Terminations++;return Mock.Case!=14;}
 static bool GetFileInformationByHandle(SafeFileHandle h,out FileInfo info){info=new FileInfo();info.volume=417614100;info.indexHigh=327680;info.indexLow=333488;info.sizeLow=798208;info.links=1;if(Mock.Case==8)info.indexLow++;return true;}
 static bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token){token=(IntPtr)4;return true;}
 static bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint length){if(cls==1){SecurityIdentifier v=new SecurityIdentifier(Mock.Case==5?"S-1-5-18":Sid);byte[] a=new byte[v.BinaryLength];v.GetBinaryForm(a,0);Marshal.WriteIntPtr(data,IntPtr.Add(data,16));Marshal.Copy(a,0,IntPtr.Add(data,16),a.Length);length=(uint)(16+a.Length);return true;}Stats z=new Stats();z.type=1;z.groups=17;z.token.low=157914109;z.authentication.low=570063;z.modified.low=157913780;if(Mock.Case==6||(Mock.Case==11&&++Mock.StatsCalls>1))z.token.low++;Marshal.StructureToPtr(z,data,false);length=(uint)Marshal.SizeOf(typeof(Stats));return true;}
 static IntPtr OpenSCManagerW(string m,string db,uint rights){return (IntPtr)5;}
 static IntPtr OpenServiceW(IntPtr manager,string name,uint rights){return (IntPtr)6;}
 static bool QueryServiceStatusEx(IntPtr service,int cls,out Status status,uint cap,out uint bytes){status=new Status();status.type=16;status.state=1;bytes=36;Mock.ScmCalls++;if(Mock.Case==7||(Mock.Case==12&&Mock.ScmCalls==2)||(Mock.Case==18&&Mock.ScmCalls==3))status.state=4;return true;}
 static bool CloseServiceHandle(IntPtr h){return Mock.Case!=21;}
 static void Need(bool v,string stage){if(!v)throw new InvalidOperationException(stage);}
 static void Native(bool v,string stage){if(!v)throw new Win32Exception(Marshal.GetLastWin32Error(),stage);}
 static void Stopped(){
  IntPtr m=IntPtr.Zero,s=IntPtr.Zero;Exception original=null;
  try {m=OpenSCManagerW(null,null,1);Native(m!=IntPtr.Zero,"SCM_CONNECT");s=OpenServiceW(m,"PapinhoAccelerator",4);Native(s!=IntPtr.Zero,"SCM_QUERY_OPEN");Status v;uint n;Native(QueryServiceStatusEx(s,0,out v,36,out n),"SCM_QUERY");Need(n==36&&v.type==16&&v.state==1&&v.pid==0,"SCM_NOT_STOPPED_PID0");}
  catch(Exception ex){original=ex;throw;}
  finally {bool a=s==IntPtr.Zero||CloseServiceHandle(s);bool b=m==IntPtr.Zero||CloseServiceHandle(m);if(!a||!b){Console.WriteLine("OBSERVER_SCM_CLOSE=UNKNOWN");if(original==null)throw new InvalidOperationException("SCM_OBSERVER_CLOSE_UNKNOWN");}}
 }
 static void Identity(IntPtr h){
  Times c,e,k,u;Native(GetProcessTimes(h,out c,out e,out k,out u),"PROCESS_TIMES");Need(GetProcessId(h)==Pid&&c.Value==Creation,"PID_REUSED_OR_CREATION_MISMATCH");
  StringBuilder b=new StringBuilder(1024);uint n=1024;Native(QueryFullProcessImageNameW(h,0,b,ref n),"PROCESS_IMAGE");Need(n>0&&n<1024&&String.Equals(b.ToString(),Image,StringComparison.OrdinalIgnoreCase),"IMAGE_PATH_MISMATCH");
  IntPtr token=IntPtr.Zero,buf=IntPtr.Zero;Exception original=null;
  try {Native(OpenProcessToken(h,8,out token),"TOKEN_OPEN");buf=Marshal.AllocHGlobal(512);uint usedBytes;
   Native(GetTokenInformation(token,1,buf,512,out usedBytes),"TOKEN_USER");Need(usedBytes>=IntPtr.Size&&usedBytes<=512,"TOKEN_USER_LENGTH");IntPtr user=Marshal.ReadIntPtr(buf);long offset=user.ToInt64()-buf.ToInt64();Need(offset>=IntPtr.Size&&offset<=usedBytes-8,"TOKEN_USER_RANGE");int len=8+4*Marshal.ReadByte(user,1);Need(len<=68&&offset+len<=usedBytes,"SID_LENGTH");Need(new SecurityIdentifier(user).Value==Sid,"TOKEN_USER_MISMATCH");
   Native(GetTokenInformation(token,10,buf,512,out usedBytes),"TOKEN_STATISTICS");Need(usedBytes==Marshal.SizeOf(typeof(Stats)),"TOKEN_STATS_LENGTH");Stats t=(Stats)Marshal.PtrToStructure(buf,typeof(Stats));
   Need(t.type==1&&t.groups==17&&t.token.low==157914109&&t.token.high==0&&t.authentication.low==570063&&t.authentication.high==0&&t.modified.low==157913780&&t.modified.high==0,"ORIGINAL_TOKEN_STATISTICS_MISMATCH");
  }catch(Exception ex){original=ex;throw;}finally{if(buf!=IntPtr.Zero)Marshal.FreeHGlobal(buf);if(token!=IntPtr.Zero&&!CloseHandle(token)){Console.WriteLine("OBSERVER_TOKEN_CLOSE=UNKNOWN");if(original==null)throw new InvalidOperationException("TOKEN_OBSERVER_CLOSE_UNKNOWN");}}
 }
 static bool Decision(bool same,bool live,bool stopped,bool disk,bool token,bool confirmed,bool alreadyUsed){return same&&live&&stopped&&disk&&token&&confirmed&&!alreadyUsed;}
 public static int RunFixed(){
  Need(Interlocked.CompareExchange(ref used,1,0)==0,"NO_RETRY");IntPtr h=IntPtr.Zero;bool consumed=false,submitted=false,exitObserved=false;int first=0;FileStream image=null;
  try {
   h=OpenProcess(Rights,false,Pid);Native(h!=IntPtr.Zero,"PROCESS_OPEN");Identity(h);Need(WaitForSingleObject(h,0)==258,"PROCESS_NOT_LIVE");Stopped();
   image=new FileStream(Image,FileMode.Open,FileAccess.Read,FileShare.Read);FileInfo f;Native(GetFileInformationByHandle(image.SafeFileHandle,out f),"DISK_ID");
   Need(f.volume==417614100&&f.indexHigh==327680&&f.indexLow==333488&&f.sizeHigh==0&&f.sizeLow==798208&&f.links==1&&(f.attributes&0x410)==0,"DISK_ID_SIZE_ATTRIBUTES_MISMATCH");
   using(SHA256 sha=SHA256.Create()){Need(BitConverter.ToString(sha.ComputeHash(image)).Replace("-","").ToLowerInvariant()==Pin,"DISK_PIN_MISMATCH");}
   Console.WriteLine("DISPOSITION_PLAN PID=21484 CREATION=134381297590479169 IMAGE=39a9da2f53ad6208ed875a916debd9c2fec760826f7a17d702d65afa28095b15; ONE_TERMINATE_NO_RETRY; NOT_SCQ_CLOSE_NO_UPDATE_NO_START");
   Console.WriteLine("Type DISPOSE_RETAINED_P1_21484_134381297590479169_ONCE only after exact owner approval:");
   bool confirm=Console.ReadLine()=="DISPOSE_RETAINED_P1_21484_134381297590479169_ONCE";
   Need(Decision(true,true,true,true,true,confirm,false),"CONFIRMATION_DENY");
   Identity(h);Stopped();Need(WaitForSingleObject(h,0)==258,"EXITED_BEFORE_SUBMISSION_NO_RETRY");
   consumed=true;submitted=true;Native(TerminateProcess(h,Exit),"TERMINATE_SUBMISSION");
   uint wait=WaitForSingleObject(h,60000);Need(wait==0,"EXIT_NOT_OBSERVED_UNKNOWN_NO_RETRY");uint code;Native(GetExitCodeProcess(h,out code),"EXIT_CODE_QUERY");Need(code==Exit,"EXIT_CODE_MISMATCH_UNKNOWN");exitObserved=true;Stopped();
  }catch(Exception ex){first=1;Console.WriteLine("DISPOSITION_FIRST_FAILURE="+ex.GetType().Name+" stage="+ex.Message);}
  finally {
   if(image!=null){try{image.Dispose();}catch{first=1;Console.WriteLine("OBSERVER_IMAGE_CLOSE=UNKNOWN");}}
   if(h!=IntPtr.Zero&&!CloseHandle(h)){first=1;Console.WriteLine("OBSERVER_PROCESS_CLOSE=UNKNOWN");}
  }
  Console.WriteLine("RETAINED_DISPOSITION first="+first+" consumed="+consumed+" submitted="+submitted+" exit_observed="+exitObserved+"; APPLICATION_CLOSE_UNKNOWN; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY_NO_UPDATE_NO_START_NO_CLEANUP");
  return first==0&&exitObserved?0:2;
 }
}

public sealed class FileStream:IDisposable {
 public SafeFileHandle SafeFileHandle {get{return new SafeFileHandle(IntPtr.Zero,false);}}
 public FileStream(string p,FileMode m,FileAccess a,FileShare s){}
 public void Dispose(){if(WorkingGrowthDispositionModel.Mock.Case==20)throw new Exception("MODEL_DISK_CLOSE");}
}
public sealed class SHA256:IDisposable {
 public static SHA256 Create(){return new SHA256();}
 public byte[] ComputeHash(FileStream s){string v="39a9da2f53ad6208ed875a916debd9c2fec760826f7a17d702d65afa28095b15";byte[] b=new byte[32];for(int i=0;i<32;i++)b[i]=Convert.ToByte(v.Substring(i*2,2),16);if(WorkingGrowthDispositionModel.Mock.Case==9)b[0]^=1;return b;}
 public void Dispose(){}
}
public static class Console {
 public static void WriteLine(string s){}
 public static string ReadLine(){return WorkingGrowthDispositionModel.Mock.Case==10?"NO":"DISPOSE_RETAINED_P1_21484_134381297590479169_ONCE";}
}
public static class DispositionOfflineQualification {
 public static string Run(){int checks=0;
 for(int c=0;c<=21;c++){WorkingGrowthDispositionModel.Mock.Reset(c);int r=WorkingGrowthDispositionModel.RunFixed();int term=WorkingGrowthDispositionModel.Mock.Terminations;
  if(c==0){if(r!=0||term!=1)throw new Exception("SUCCESS_MODEL");}
  else {if(r!=2)throw new Exception("NEGATIVE_MODEL "+c);if((c<=13||c==21)&&term!=0)throw new Exception("EARLY_EFFECT "+c);if(c>=14&&c<=20&&term!=1)throw new Exception("SUBMISSION_COUNT "+c);}
  if(WorkingGrowthDispositionModel.Mock.Opened!=1)throw new Exception("REOPEN");checks++;
 }
 WorkingGrowthDispositionModel.Mock.Reset(0);if(WorkingGrowthDispositionModel.RunFixed()!=0)throw new Exception("FIRST_CALL");bool denied=false;try{WorkingGrowthDispositionModel.RunFixed();}catch(InvalidOperationException){denied=true;}if(!denied||WorkingGrowthDispositionModel.Mock.Terminations!=1)throw new Exception("REPLAY");checks++;
 return "DISPOSITION_OFFLINE_PASS cases="+checks+"; ALL_NATIVE_PROCESS_TOKEN_SCM_IO_HASH_CONSOLE_EDGES_MODELED; NO_NATIVE_DISPOSITION";
 }
}
