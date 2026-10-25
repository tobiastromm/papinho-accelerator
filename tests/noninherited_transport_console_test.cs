using System;using System.Runtime.InteropServices;
public static class PaccTransportConsoleTest {
 [StructLayout(LayoutKind.Explicit,Size=20)] struct Input {[FieldOffset(0)]public ushort Type;[FieldOffset(4)]public int Down;[FieldOffset(8)]public ushort Repeat;[FieldOffset(10)]public ushort Key;[FieldOffset(12)]public ushort Scan;[FieldOffset(14)]public char Character;[FieldOffset(16)]public uint Control;}
 [DllImport("kernel32.dll")]static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern bool WriteConsoleInputW(IntPtr h,Input[] input,uint count,out uint written);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool GetProcessHandleCount(IntPtr p,out uint count);
 [DllImport("kernel32.dll")]static extern IntPtr GetCurrentProcess();
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool FreeConsole();
 public static void DetachOwnTestConsole(){if(!FreeConsole())throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());}
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern IntPtr LoadLibrary(string file);
 public static void Preload(string file){if(LoadLibrary(file)==IntPtr.Zero)throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());}
 public static uint Handles(){uint n;if(!GetProcessHandleCount(GetCurrentProcess(),out n))throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());return n;}
 public static void Key(ushort key,char value){Input[] a=new Input[2];a[0].Type=1;a[0].Down=1;a[0].Repeat=1;a[0].Key=key;a[0].Character=value;a[1]=a[0];a[1].Down=0;uint n;if(!WriteConsoleInputW(GetStdHandle(-10),a,2,out n)||n!=2)throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());}
 public static void Text(string s){foreach(char c in s)Key(c=='\r'?(ushort)13:(ushort)char.ToUpperInvariant(c),c);}
 [StructLayout(LayoutKind.Sequential)]struct Security {public int length;public IntPtr descriptor;public int inherit;}
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]static extern IntPtr CreateEvent(ref Security a,bool manual,bool initial,string name);
 [DllImport("kernel32.dll",SetLastError=true)]static extern bool CloseHandle(IntPtr h);
 static readonly IntPtr[] sentinels=new IntPtr[8];
 public static string CreateSentinels(){Security a=new Security();a.length=Marshal.SizeOf(typeof(Security));a.inherit=1;string text="";for(int i=0;i<8;i++){sentinels[i]=CreateEvent(ref a,true,false,null);if(sentinels[i]==IntPtr.Zero)throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());text+=" "+sentinels[i].ToInt64();}return text;}
 public static void CloseSentinels(){for(int i=0;i<8;i++){if(sentinels[i]==IntPtr.Zero)continue;if(!CloseHandle(sentinels[i]))throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());sentinels[i]=IntPtr.Zero;}}
}

public sealed class PaccThrowWriter : System.IO.TextWriter {
 public override System.Text.Encoding Encoding {get{return System.Text.Encoding.ASCII;}}
 public override void WriteLine(string value){throw new System.IO.IOException("SYNTHETIC_STDOUT_WRITE_ERROR");}
}
