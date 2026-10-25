// Offline-compiled .NET Framework binding. No admission authority or finalizer.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Text;
namespace Pacc.N0PrivateStdout {
 public sealed class Supervisor {
  [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] static extern IntPtr LoadLibraryEx(string file,IntPtr unused,uint flags);
  [DllImport("kernel32.dll",CharSet=CharSet.Unicode)]static extern IntPtr GetModuleHandle(string name);
  [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern uint GetModuleFileName(IntPtr module,StringBuilder name,int size);
  [DllImport("kernel32.dll",CharSet=CharSet.Ansi,ExactSpelling=true,SetLastError=true)] static extern IntPtr GetProcAddress(IntPtr module,string name);
  [UnmanagedFunctionPointer(CallingConvention.Winapi)] delegate uint Simple();
  [UnmanagedFunctionPointer(CallingConvention.Winapi,CharSet=CharSet.Unicode)] delegate uint Create(string file,string args);
  [UnmanagedFunctionPointer(CallingConvention.Winapi)] delegate uint Read([Out] byte[] bytes,uint cap,out uint length);
  [UnmanagedFunctionPointer(CallingConvention.Winapi)] delegate uint Wait(out uint code);
  [UnmanagedFunctionPointer(CallingConvention.Winapi)] delegate uint Status(out uint pid,out ulong created,out uint first,out uint retained);
  readonly IntPtr module; readonly Simple open,authenticate,close;readonly Create create;readonly Read read;readonly Wait wait;readonly Status status;
  readonly byte[] bytes=new byte[4096];readonly StringBuilder line=new StringBuilder(4096);
  uint offset,length;bool eof,overflow;long chars;public bool Truncated {get;private set;}
  public bool Closed {get;private set;}
  Delegate Bind(string name,Type type){IntPtr p=GetProcAddress(module,name);if(p==IntPtr.Zero)throw new Win32Exception(Marshal.GetLastWin32Error());return Marshal.GetDelegateForFunctionPointer(p,type);}
  static void Check(uint error){if(error!=0)throw new Win32Exception((int)error);}
  public Supervisor(string dll){if(GetModuleHandle(System.IO.Path.GetFileName(dll))!=IntPtr.Zero)throw new InvalidOperationException("NATIVE_MODULE_ALREADY_PRESENT_DENY");module=LoadLibraryEx(dll,IntPtr.Zero,0x1100);if(module==IntPtr.Zero)throw new Win32Exception(Marshal.GetLastWin32Error());StringBuilder location=new StringBuilder(32768);uint size=GetModuleFileName(module,location,location.Capacity);if(size==0||size>=(uint)location.Capacity||!String.Equals(location.ToString(),dll,StringComparison.OrdinalIgnoreCase))throw new InvalidOperationException("NATIVE_MODULE_LOCATION_DENY");open=(Simple)Bind("PaccTransportOpen",typeof(Simple));authenticate=(Simple)Bind("PaccTransportAuthenticate",typeof(Simple));close=(Simple)Bind("PaccTransportClose",typeof(Simple));create=(Create)Bind("PaccTransportCreate",typeof(Create));read=(Read)Bind("PaccTransportRead",typeof(Read));wait=(Wait)Bind("PaccTransportWait",typeof(Wait));status=(Status)Bind("PaccTransportStatus",typeof(Status));}
  public void Open(){Check(open());}public void Start(string file,string args){Check(create(file,args));}public void Authenticate(){Check(authenticate());}
  // Keep draining after limits. Bound line storage; never ReadToEnd.
  public string ReadLine(){while(true){if(offset==length){if(eof){if(line.Length==0&&!overflow)return null;return TakeLine();}Check(read(bytes,(uint)bytes.Length,out length));offset=0;if(length==0){eof=true;continue;}}
   byte b=bytes[offset++];if(b==10)return TakeLine();if(b>127)throw new InvalidOperationException("NON_ASCII_PROTOCOL_DENY");chars++;if(chars>131072)Truncated=true;if(line.Length<4096)line.Append((char)b);else{overflow=true;Truncated=true;}
  }}
  string TakeLine(){string s=line.ToString();if(s.EndsWith("\r",StringComparison.Ordinal))s=s.Substring(0,s.Length-1);line.Length=0;if(overflow)s="N0_OVERLONG_LINE_TRUNCATED "+s;overflow=false;return s;}
  public uint WaitOriginal(){uint code;Check(wait(out code));return code;}
  public void CloseChecked(){if(Closed)throw new InvalidOperationException("DOUBLE_CLOSE_DENY");Check(close());Closed=true;}
  public bool CloseUnstartedIfKnown(){uint pid,first,retained;ulong created;Check(status(out pid,out created,out first,out retained));if(pid!=0||created!=0||retained!=0)return false;CloseChecked();return true;}
  public string Receipt(){uint pid,first,retained;ulong created;Check(status(out pid,out created,out first,out retained));return "pid="+pid+" creation="+created+" first="+first+" retained="+retained;}
 }
}
