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
public static class RetainedP1Disposition {
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
 [DllImport("kernel32.dll",SetLastError=true)] static extern IntPtr OpenProcess(uint rights,bool inherit,uint pid);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] static extern uint GetProcessId(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetProcessTimes(IntPtr h,out Times c,out Times e,out Times k,out Times u);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern bool QueryFullProcessImageNameW(IntPtr h,uint flags,StringBuilder text,ref uint size);
 [DllImport("kernel32.dll",SetLastError=true)] static extern uint WaitForSingleObject(IntPtr h,uint ms);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetExitCodeProcess(IntPtr h,out uint code);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool TerminateProcess(IntPtr h,uint code);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetFileInformationByHandle(SafeFileHandle h,out FileInfo info);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool OpenProcessToken(IntPtr h,uint rights,out IntPtr token);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetTokenInformation(IntPtr h,int cls,IntPtr data,uint cap,out uint length);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr OpenSCManagerW(string m,string db,uint rights);
 [DllImport("advapi32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr OpenServiceW(IntPtr manager,string name,uint rights);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool QueryServiceStatusEx(IntPtr service,int cls,out Status status,uint cap,out uint bytes);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool CloseServiceHandle(IntPtr h);
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
