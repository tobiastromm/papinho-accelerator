using System;
using System.Runtime.InteropServices;
class TpmIndexAbsenceReadOnly {
 [StructLayout(LayoutKind.Sequential)] struct Params {public UInt32 version,flags;}
 [DllImport("tbs.dll",ExactSpelling=true)] static extern UInt32 Tbsi_Context_Create(ref Params p,out IntPtr h);
 [DllImport("tbs.dll",ExactSpelling=true)] static extern UInt32 Tbsip_Submit_Command(IntPtr h,UInt32 locality,UInt32 priority,byte[] command,UInt32 length,byte[] result,ref UInt32 size);
 [DllImport("tbs.dll",ExactSpelling=true)] static extern UInt32 Tbsip_Context_Close(IntPtr h);
 static UInt32 U(byte[] b,int n) {return ((UInt32)b[n]<<24)|((UInt32)b[n+1]<<16)|((UInt32)b[n+2]<<8)|b[n+3];}
 static void W(byte[] b,int n,UInt32 v){b[n]=(byte)(v>>24);b[n+1]=(byte)(v>>16);b[n+2]=(byte)(v>>8);b[n+3]=(byte)v;}
 static bool Query(IntPtr h,UInt32 prop) {
  byte[] c=new byte[22];c[0]=0x80;c[1]=1;W(c,2,22);W(c,6,0x17a);W(c,10,1);W(c,14,prop);W(c,18,1);
  byte[] b=new byte[4096];UInt32 n=4096;UInt32 e=Tbsip_Submit_Command(h,0,200,c,22,b,ref n);
  Console.WriteLine("GET_CAPABILITY property=0x"+prop.ToString("x8")+" tbs=0x"+e.ToString("x8")+" bytes="+n);
  if(e!=0||n<10||n>4096||U(b,2)!=n)return false;
  UInt32 rc=U(b,6);Console.WriteLine("TPM_RESULT=0x"+rc.ToString("x8"));if(rc!=0)return false;
  if(n<19||b[0]!=0x80||b[1]!=1||b[10]>1||U(b,11)!=1)return false;
  UInt32 count=U(b,15);if(count>1||19+count*4!=n)return false;
  Console.WriteLine("moreData="+b[10]+" count="+count);
  if(count==1&&(U(b,19)<prop||(U(b,19)>>24)!=1))return false;
  if(count==0&&b[10]!=0)return false;
  Console.WriteLine("CANDIDATE=0x"+prop.ToString("x8")+"; ABSENT_AT_OBSERVATION="+(count==0||U(b,19)!=prop)+"; NOT_RESERVED; NO_ADOPTION");
  return true;
 }
 static int Main(){Params p=new Params();p.version=2;p.flags=4;IntPtr h;UInt32 e=Tbsi_Context_Create(ref p,out h);Console.WriteLine("CONTEXT_CREATE=0x"+e.ToString("x8"));if(e!=0)return 2;
  bool ok=false;try{ok=Query(h,0x01804143);}finally{e=Tbsip_Context_Close(h);Console.WriteLine("CONTEXT_CLOSE=0x"+e.ToString("x8")+"; COMMAND_ALLOWLIST=GET_CAPABILITY_ONLY; NV_WRITES=NONE");if(e!=0){Console.WriteLine("UNKNOWN_RETAIN_NO_RETRY");while(true)System.Threading.Thread.Sleep(1000);}}
  return ok?0:2;
 }
}
