using System;using System.Threading;using System.Security.Cryptography;
sealed class FakeTransport:ITransport {
 public UInt32 OpenCode,SubmitCode,CloseCode;public bool BadHandle;public int Opens,Submits,Closes,Active,Peak;public byte[] Response;public UInt32? Size;
 public UInt32 Open(out IntPtr h){Opens++;h=BadHandle?IntPtr.Zero:new IntPtr(123);return OpenCode;}
 public UInt32 Submit(IntPtr h,byte[] c,byte[] b,ref UInt32 n){Submits++;int a=Interlocked.Increment(ref Active);if(a>Peak)Peak=a;Thread.Sleep(1);byte[] data=Response??Tests.Header(10,0);Array.Copy(data,b,data.Length);n=Size??(UInt32)data.Length;Interlocked.Decrement(ref Active);return SubmitCode;}
 public UInt32 Close(IntPtr h){Closes++;return CloseCode;}
}
class Tests {
 static int checks;static void C(bool b){checks++;if(!b)throw new Exception("CHECK_"+checks);}
 static void Bad(Action a){bool fail=false;try{a();}catch(InvalidOperationException){fail=true;}C(fail);}
 public static byte[] Header(int n,UInt32 rc){byte[] b=new byte[n];b[0]=0x80;b[1]=1;Adapter.W(b,2,(UInt32)n);Adapter.W(b,6,rc);return b;}
 static byte[] Cap(UInt32 cap,UInt32 key,UInt32 value){byte[] b=Header(cap==1?23:27,0);Adapter.W(b,11,cap);Adapter.W(b,15,1);Adapter.W(b,19,key);if(cap==6)Adapter.W(b,23,value);return b;}
 static byte[] Pub(bool written){byte[] b=Header(62,0);b[11]=14;Adapter.W(b,12,Adapter.Index);b[17]=11;Adapter.W(b,18,Adapter.Attributes|(written?0x20000000U:0));b[25]=8;b[27]=34;b[29]=11;byte[] area=new byte[14];Array.Copy(b,12,area,0,14);using(SHA256 sha=SHA256.Create())Array.Copy(sha.ComputeHash(area),0,b,30,32);return b;}
 static Result P(byte[] b){return Adapter.Parse(0,b,(UInt32)b.Length);}
 static int Main(){
  C(Adapter.Allowed(Adapter.PublicCommand(Adapter.Index)));C(Adapter.Allowed(Adapter.CapabilityCommand(1,Adapter.Index)));C(Adapter.Allowed(Adapter.CapabilityCommand(6,0x116)));C(Adapter.Allowed(Adapter.CapabilityCommand(6,0x20b)));C(!Adapter.Allowed(null));
  byte[] c=Adapter.PublicCommand(Adapter.Index);c[9]=0x34;C(!Adapter.Allowed(c));Bad(delegate{Adapter.PublicCommand(0x01000000);});Bad(delegate{Adapter.CapabilityCommand(6,0x100);});
  for(int n=0;n<10;n++)C(Adapter.Parse(0,new byte[10],(UInt32)n).Error=="HEADER_DENY");
  C(Adapter.Parse(1,null,0).Error=="TBS_00000001");C(Adapter.Parse(0,null,10).Error=="HEADER_DENY");C(Adapter.Parse(0,new byte[10],11).Error=="HEADER_DENY");
  byte[] b=Header(10,0);b[1]=2;C(P(b).Error=="HEADER_DENY");b=Header(10,0);b[5]=9;C(P(b).Error=="HEADER_DENY");
  foreach(UInt32 code in new UInt32[]{0x18b,0x98e,0x9a2,0x920,0x923}){Result e=P(Header(10,code));C(e.Rc==code&&e.Body==null&&e.Error!=null);}C(P(Header(11,0x18b)).Error=="ERROR_TRAILING_DENY");
  bool absent;C(Adapter.Property(P(Cap(6,0x116,128)),6,0x116,out absent)==128&&!absent);C(Adapter.Property(P(Cap(1,Adapter.Index,0)),1,Adapter.Index,out absent)==Adapter.Index&&!absent);C(Adapter.Property(P(Cap(1,Adapter.Index+1,0)),1,Adapter.Index,out absent)==null&&absent);
  Bad(delegate{bool x;Adapter.Property(P(Cap(1,Adapter.Index-1,0)),1,Adapter.Index,out x);});Bad(delegate{bool x;Adapter.Property(P(Cap(1,Adapter.Index,0)),6,Adapter.Index,out x);});
  b=Cap(6,0x116,128);b[18]=2;Bad(delegate{bool x;Adapter.Property(P(b),6,0x116,out x);});
  C(Adapter.PublicArea(P(Pub(false)),false).Length==14);C(Adapter.PublicArea(P(Pub(true)),true).Length==14);Bad(delegate{Adapter.PublicArea(P(Pub(true)),false);});
  foreach(int pos in new int[]{11,12,17,18,22,25,27,29,30,61}){byte[] corrupt=Pub(false);corrupt[pos]^=1;Bad(delegate{Adapter.PublicArea(P(corrupt),false);});}
  for(int n=10;n<62;n++){byte[] shortb=new byte[n];Array.Copy(Pub(false),shortb,n);Adapter.W(shortb,2,(UInt32)n);Bad(delegate{Adapter.PublicArea(P(shortb),false);});}
  FakeTransport f=new FakeTransport();Adapter a=new Adapter(f);C(a.Open());C(!a.Open());C(a.Close());C(a.Close());C(f.Closes==1);C(a.Public().Error=="STATE_DENY"&&f.Submits==0);
  f=new FakeTransport();f.SubmitCode=0x80284001;a=new Adapter(f);C(a.Open());C(a.Public().Error=="TBS_80284001");C(a.Frozen);C(a.Public().Error=="STATE_DENY"&&f.Submits==1);f.CloseCode=1;C(!a.Close()&&a.Retained);C(a.First=="TBS_80284001");C(!a.Close()&&f.Closes==1);
  f=new FakeTransport();f.OpenCode=1;a=new Adapter(f);C(!a.Open()&&a.Retained);C(!a.Close()&&f.Closes==0);
  f=new FakeTransport();f.BadHandle=true;a=new Adapter(f);C(!a.Open());C(a.Close());
  f=new FakeTransport();f.Response=Header(10,0x9a2);a=new Adapter(f);C(a.Open());C(a.Public().Rc==0x9a2);C(a.Public().Error=="STATE_DENY"&&f.Submits==1);C(a.Close());
  f=new FakeTransport();f.Size=4097;a=new Adapter(f);C(a.Open());C(a.Public().Error=="HEADER_DENY");C(a.Close());
  f=new FakeTransport();a=new Adapter(f);C(a.Open());Thread[] ts=new Thread[16];for(int i=0;i<ts.Length;i++){ts[i]=new Thread(delegate(){a.Public();});ts[i].Start();}foreach(Thread t in ts)t.Join();C(f.Submits==16&&f.Peak==1);C(a.Close()&&f.Closes==1);
  Console.WriteLine("PASS checks="+checks+"; REAL_ADAPTER_WITH_FAKE_APIS; NO_TPM_MUTATION");return 0;
 }
}
