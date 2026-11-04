using System;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
interface ITransport {UInt32 Open(out IntPtr h);UInt32 Submit(IntPtr h,byte[] c,byte[] b,ref UInt32 n);UInt32 Close(IntPtr h);}
sealed class NativeTransport:ITransport {
 [StructLayout(LayoutKind.Sequential)]struct P {public UInt32 version,flags;}
 [DllImport("tbs.dll",ExactSpelling=true)]static extern UInt32 Tbsi_Context_Create(ref P p,out IntPtr h);
 [DllImport("tbs.dll",ExactSpelling=true)]static extern UInt32 Tbsip_Submit_Command(IntPtr h,UInt32 l,UInt32 p,byte[] c,UInt32 n,byte[] b,ref UInt32 z);
 [DllImport("tbs.dll",ExactSpelling=true)]static extern UInt32 Tbsip_Context_Close(IntPtr h);
 public UInt32 Open(out IntPtr h){P p=new P();p.version=2;p.flags=4;return Tbsi_Context_Create(ref p,out h);}
 public UInt32 Submit(IntPtr h,byte[] c,byte[] b,ref UInt32 n){if(!Adapter.Allowed(c))return 0x80284002;return Tbsip_Submit_Command(h,0,200,c,(UInt32)c.Length,b,ref n);}
 public UInt32 Close(IntPtr h){return Tbsip_Context_Close(h);}
}
sealed class Result {public UInt32 Tbs,Rc;public byte[] Body;public string Error;}
sealed class Adapter {
 public const UInt32 Index=0x01804143,Attributes=0x00040014;
 readonly object gate=new object();readonly ITransport api;IntPtr context;
 bool used,closed;public bool Frozen,Retained;public string First="NONE";public int Queries;
 public Adapter(ITransport a){api=a;}
 public static UInt32 U(byte[] b,int n){return ((UInt32)b[n]<<24)|((UInt32)b[n+1]<<16)|((UInt32)b[n+2]<<8)|b[n+3];}
 public static void W(byte[] b,int n,UInt32 v){b[n]=(byte)(v>>24);b[n+1]=(byte)(v>>16);b[n+2]=(byte)(v>>8);b[n+3]=(byte)v;}
 public static int S(byte[] b,int n){return (b[n]<<8)|b[n+1];}
 void Fail(string e){if(First=="NONE")First=e;Frozen=true;}
 public bool Open(){lock(gate){if(used||Frozen)return false;used=true;UInt32 e=api.Open(out context);if(e!=0||context==IntPtr.Zero){if(context!=IntPtr.Zero)Retained=true;Fail("OPEN_"+e.ToString("x8"));return false;}return true;}}
 public static byte[] CapabilityCommand(UInt32 capability,UInt32 property){if(!((capability==1&&property==Index)||(capability==6&&(property==0x116||property==0x20b))))throw new InvalidOperationException("COMMAND_DENY");byte[] c=new byte[22];c[0]=0x80;c[1]=1;W(c,2,22);W(c,6,0x17a);W(c,10,capability);W(c,14,property);W(c,18,1);return c;}
 public static byte[] PublicCommand(UInt32 index){if(index!=Index)throw new InvalidOperationException("INDEX_DENY");byte[] c=new byte[14];c[0]=0x80;c[1]=1;W(c,2,14);W(c,6,0x169);W(c,10,index);return c;}
 public static bool Allowed(byte[] c){if(c==null)return false;byte[][] list={CapabilityCommand(6,0x116),CapabilityCommand(6,0x20b),CapabilityCommand(1,Index),PublicCommand(Index)};foreach(byte[] x in list){if(x.Length!=c.Length)continue;int bad=0;for(int i=0;i<x.Length;i++)bad|=x[i]^c[i];if(bad==0)return true;}return false;}
 public static Result Parse(UInt32 e,byte[] b,UInt32 n){Result r=new Result();r.Tbs=e;if(e!=0){r.Error="TBS_"+e.ToString("x8");return r;}if(b==null||n<10||n>b.Length||b[0]!=0x80||b[1]!=1||U(b,2)!=n){r.Error="HEADER_DENY";return r;}r.Rc=U(b,6);if(r.Rc!=0){r.Error=n==10?"TPM_"+r.Rc.ToString("x8"):"ERROR_TRAILING_DENY";return r;}r.Body=new byte[n-10];Array.Copy(b,10,r.Body,0,r.Body.Length);return r;}
 Result Query(byte[] c){lock(gate){if(context==IntPtr.Zero||Frozen||closed)return new Result{Error="STATE_DENY"};byte[] b=new byte[4096];UInt32 n=4096;Queries++;UInt32 e=api.Submit(context,c,b,ref n);Result r=Parse(e,b,n);if(r.Error!=null){Fail(r.Error);}return r;}}
 public Result Capability(UInt32 cap,UInt32 property){return Query(CapabilityCommand(cap,property));}
 public Result Public(){return Query(PublicCommand(Index));}
 public static UInt32? Property(Result r,UInt32 cap,UInt32 property,out bool absent){absent=false;if(cap!=1&&cap!=6)throw new InvalidOperationException("CAP_DENY");if(r.Error!=null||r.Body==null)throw new InvalidOperationException("NO_DATA");byte[] b=r.Body;if(b.Length<9||b[0]>1||U(b,1)!=cap)throw new InvalidOperationException("CAP_HEADER_DENY");UInt32 count=U(b,5);int step=cap==1?4:8;if(count>1||b.Length!=9+count*step)throw new InvalidOperationException("CAP_SIZE_DENY");if(count==0){if(b[0]!=0)throw new InvalidOperationException("CAP_INCOMPLETE");absent=true;return null;}UInt32 key=U(b,9);if(key<property||(cap==1&&(key>>24)!=1))throw new InvalidOperationException("CAP_ORDER_DENY");if(key!=property){absent=true;return null;}return cap==1?key:U(b,13);}
 public static byte[] PublicArea(Result r,bool written){if(r.Error!=null||r.Body==null)throw new InvalidOperationException("NO_PUBLIC_DATA");byte[] b=r.Body;if(b.Length<4)throw new InvalidOperationException("PUBLIC_TRUNCATED");int p=S(b,0);if(p<14||p>78||b.Length<2+p+2)throw new InvalidOperationException("PUBLIC_LENGTH_DENY");if(U(b,2)!=Index||S(b,6)!=11||U(b,8)!=(Attributes|(written?0x20000000U:0)))throw new InvalidOperationException("PUBLIC_BINDING_DENY");int policy=S(b,12);if(policy!=0||p!=14+policy||S(b,14+policy)!=8)throw new InvalidOperationException("PUBLIC_POLICY_SIZE_DENY");int n=S(b,2+p);if(n!=34||b.Length!=4+p+n||S(b,4+p)!=11)throw new InvalidOperationException("NAME_SIZE_DENY");byte[] area=new byte[p];Array.Copy(b,2,area,0,p);using(SHA256 sha=SHA256.Create()){byte[] h=sha.ComputeHash(area);int mismatch=0;for(int i=0;i<32;i++)mismatch|=h[i]^b[6+p+i];if(mismatch!=0)throw new InvalidOperationException("NAME_INTEGRITY_DENY");}return area;}
 public void Reject(string e){lock(gate){Fail(e);}}
 public bool Close(){lock(gate){if(closed)return !Retained;if(context==IntPtr.Zero){closed=true;return !Retained;}if(Retained)return false;UInt32 e=api.Close(context);if(e!=0){Retained=true;Fail("CLOSE_"+e.ToString("x8"));return false;}context=IntPtr.Zero;closed=true;return true;}}
}
class NativeRun {
 [StructLayout(LayoutKind.Sequential)]struct Info {public UInt32 v,t,i,r;}
 [DllImport("tbs.dll",ExactSpelling=true)]static extern UInt32 Tbsi_GetDeviceInfo(UInt32 n,ref Info i);
 static int Main(){Info i=new Info();i.v=2;UInt32 e=Tbsi_GetDeviceInfo(16,ref i);Console.WriteLine("DEVICE tbs=0x"+e.ToString("x8")+" struct="+i.v+" tpm="+i.t+" interface="+i.i);if(e!=0||i.t!=2)return 2;
  Adapter a=new Adapter(new NativeTransport());bool closed=false;try{if(!a.Open())return 2;foreach(UInt32 prop in new UInt32[]{0x116,0x20b}){Result r=a.Capability(6,prop);bool absent;UInt32? value=Adapter.Property(r,6,prop,out absent);if(absent)throw new InvalidOperationException("REQUIRED_PROPERTY_MISSING");Console.WriteLine("PROPERTY 0x"+prop.ToString("x8")+"="+value);}
   Result handles=a.Capability(1,Adapter.Index);bool missing;Adapter.Property(handles,1,Adapter.Index,out missing);Console.WriteLine("CANDIDATE_ABSENT_AT_OBSERVATION="+missing+"; NOT_RESERVED");
   Result pub=a.Public();Console.WriteLine("NV_READ_PUBLIC tbs=0x"+pub.Tbs.ToString("x8")+" tpm_rc=0x"+pub.Rc.ToString("x8")+" outcome="+(pub.Error??"SUCCESS"));if(pub.Error==null){Adapter.PublicArea(pub,false);Console.WriteLine("EXISTING_PUBLIC_PROFILE_MATCH_NOT_OWNERSHIP; STOP_NO_ADOPTION");}
  }catch(InvalidOperationException x){a.Reject(x.Message);}finally{closed=a.Close();Console.WriteLine("FINAL first="+a.First+" closed="+closed+" retained="+a.Retained+" queries="+a.Queries+"; NV_MUTATIONS=NONE; NO_AUTHORITY");if(!closed&&a.Retained){Console.WriteLine("UNKNOWN_RETAIN_NO_RETRY");while(true)System.Threading.Thread.Sleep(1000);}}
  return closed&&a.First=="NONE"?0:2;
 }
}
