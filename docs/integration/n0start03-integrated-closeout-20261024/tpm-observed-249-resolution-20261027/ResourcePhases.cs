using System;using System.IO;using System.Text;using System.Security.Principal;using System.Security.Cryptography;using System.Diagnostics;using System.Diagnostics.Eventing.Reader;using System.Xml;using System.Runtime.InteropServices;using System.Runtime.CompilerServices;
class ResourcePhases {
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetProcessHandleCount(IntPtr p,out uint n);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern bool CreateProcess(string app,StringBuilder cmd,IntPtr pa,IntPtr ta,bool inherit,uint flags,IntPtr env,string cwd,ref SI si,out PI pi);
 [DllImport("kernel32.dll",SetLastError=true)]static extern uint WaitForSingleObject(IntPtr p,uint timeout);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool CloseHandle(IntPtr p);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetExitCodeProcess(IntPtr p,out uint code);
 [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Unicode)]struct SI {public uint cb;public string reserved,desktop,title;public uint x,y,w,h,cx,cy,fill,flags;public short show,cb2;public IntPtr reserved2,input,output,error;}
 [StructLayout(LayoutKind.Sequential)]struct PI {public IntPtr process,thread;public uint pid,tid;}
 static string observations="";
 [MethodImpl(MethodImplOptions.NoInlining)]static void Sample(string phase){uint n;if(!GetProcessHandleCount(new IntPtr(-1),out n))throw new Exception("QUERY "+Marshal.GetLastWin32Error());observations+=phase+"="+n+"\n";}
 [MethodImpl(MethodImplOptions.NoInlining)]static void Identity(){using(WindowsIdentity id=WindowsIdentity.GetCurrent()){string s=id.User.Value;bool admin=(new WindowsPrincipal(id)).IsInRole(WindowsBuiltInRole.Administrator);GC.KeepAlive(s);GC.KeepAlive(admin);}WindowsIdentity thread=WindowsIdentity.GetCurrent(true);if(thread!=null)thread.Dispose();}
 [MethodImpl(MethodImplOptions.NoInlining)]static void Boot(){EventLogQuery q=new EventLogQuery("System",PathType.LogName,"*[System[Provider[@Name='Microsoft-Windows-Kernel-General'] and EventID=12]]");q.ReverseDirection=true;using(EventLogReader reader=new EventLogReader(q))using(EventRecord record=reader.ReadEvent()){if(record!=null){XmlDocument x=new XmlDocument();x.LoadXml(record.ToXml());GC.KeepAlive(x);}}using(Process proc=Process.GetCurrentProcess()){GC.KeepAlive(proc.Id);GC.KeepAlive(proc.StartTime);}}
 [MethodImpl(MethodImplOptions.NoInlining)]static void HashFiles(){string file=System.Reflection.Assembly.GetExecutingAssembly().Location;using(SHA256 hash=SHA256.Create()){GC.KeepAlive(hash.ComputeHash(File.ReadAllBytes(file)));}}
 [MethodImpl(MethodImplOptions.NoInlining)]static void Save(string path){using(FileStream f=new FileStream(path,FileMode.CreateNew,FileAccess.Write,FileShare.None)){byte[] b=Encoding.ASCII.GetBytes(observations);f.Write(b,0,b.Length);f.Flush(true);}}
 static int Main(string[] args){try{
 if(args.Length==2&&args[0]=="child"){Sample("entry");Identity();Sample("after_identity");try{Boot();}catch(Exception e){observations+="boot_error="+e.GetType().Name+"\n";}Sample("after_eventlog_process");HashFiles();Sample("after_hash_file");GC.Collect();GC.WaitForPendingFinalizers();Sample("after_gc_DIAGNOSTIC_ONLY_NOT_A_FIX");Save(args[1]);return 0;}
 if(args.Length==2&&args[0]=="parent"){
 string exe=System.Reflection.Assembly.GetExecutingAssembly().Location;foreach(bool inherit in new bool[]{false,true}){string path=args[1]+(inherit?"-inherit.txt":"-noinherit.txt");SI si=new SI();si.cb=(uint)Marshal.SizeOf(si);PI pi;
 if(!CreateProcess(exe,new StringBuilder("\""+exe+"\" child \""+path+"\""),IntPtr.Zero,IntPtr.Zero,inherit,0x08000000,IntPtr.Zero,null,ref si,out pi))throw new Exception("CREATE "+Marshal.GetLastWin32Error());
 uint wait=WaitForSingleObject(pi.process,30000),code;if(wait!=0||!GetExitCodeProcess(pi.process,out code))throw new Exception("UNKNOWN_RETAIN_NO_KILL");
 if(!CloseHandle(pi.thread))throw new Exception("THREAD_CLOSE_UNKNOWN");if(!CloseHandle(pi.process))throw new Exception("PROCESS_CLOSE_UNKNOWN");if(code!=0)throw new Exception("CHILD "+code);
 Console.WriteLine("child inherit="+inherit+" closed confirmed; fixture only");}return 0;}
 return 2;}catch(Exception e){Console.WriteLine(e.Message);return 1;}}
}