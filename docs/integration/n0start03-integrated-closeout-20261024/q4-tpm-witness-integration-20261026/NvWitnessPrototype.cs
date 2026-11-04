using System;
using System.Security.Cryptography;
using System.Threading;
// Isolated simulation and private TPM public-area encoding. NO TBS import/transport.
static class PublicArea {
 public const UInt32 Index=0x01804143, Attributes=0x00040014, Written=0x20000000;
 static void W(byte[] b,int n,UInt32 v){b[n]=(byte)(v>>24);b[n+1]=(byte)(v>>16);b[n+2]=(byte)(v>>8);b[n+3]=(byte)v;}
 public static byte[] Encode(UInt32 index,UInt32 attrs) {
  if(index!=Index||(attrs!=Attributes&&attrs!=(Attributes|Written)))throw new InvalidOperationException("PUBLIC_DENY");
  byte[] b=new byte[14];W(b,0,index);b[4]=0;b[5]=11;W(b,6,attrs);b[10]=0;b[11]=0;b[12]=0;b[13]=8;return b;
 }
 public static byte[] Name(bool written){byte[] p=Encode(Index,Attributes|(written?Written:0));using(SHA256 s=SHA256.Create()){byte[] d=s.ComputeHash(p);byte[] n=new byte[34];n[1]=11;Array.Copy(d,0,n,2,32);return n;}}
 public static bool Same(byte[] a,byte[] b){if(a==null||b==null||a.Length!=b.Length)return false;int x=0;for(int i=0;i<a.Length;i++)x|=a[i]^b[i];return x==0;}
}
sealed class FakeNv {
 public bool Defined,Written,Available=true,Auth=true,UnknownIncrement;
 public UInt64 Value,GlobalInitial=81;public int Definitions,Increments;
 public void Define(int authBytes){if(Defined)throw new InvalidOperationException("COLLISION");if(authBytes!=32)throw new InvalidOperationException("AUTH_CUSTODY_DENY");Defined=true;Definitions++;}
 public UInt64 Read(){if(!Available)throw new InvalidOperationException("TPM_UNAVAILABLE");if(!Auth)throw new InvalidOperationException("AUTH_DENY");if(!Defined||!Written)throw new InvalidOperationException("UNWRITTEN");return Value;}
 public void Increment(){if(!Available||!Auth||!Defined)throw new InvalidOperationException("INCREMENT_DENY");if(Written&&Value==UInt64.MaxValue)throw new InvalidOperationException("EXHAUSTED");Value=Written?Value+1:GlobalInitial+1;Written=true;Increments++;if(UnknownIncrement)throw new InvalidOperationException("PERSISTENCE_UNKNOWN");}
}
sealed class Lease {
 public readonly string Historic,Root,Approval,Device;public readonly long Pid,Creation;public readonly UInt64 CounterBaseline;
 public Lease(string h,string r,string a,string d,long p,long c){Historic=h;Root=r;Approval=a;Device=d;Pid=p;Creation=c;CounterBaseline=82;}
}
sealed class Protocol {
 readonly object gate=new object();readonly FakeNv nv;readonly Lease binding;
 public bool Used,Retained,Committed;public string First="NONE";public int Reads;
 public Protocol(FakeNv n,Lease b){nv=n;binding=b;}
 void Fail(string e){if(First=="NONE")First=e;Retained=true;}
 public bool Consume(Lease b,UInt64 baseline,bool restart,string fault){
  lock(gate){
   if(Used||Retained)return false;Used=true; // per-invocation simulated gate, NOT durable authority
   if(b==null||restart||b.Historic!=binding.Historic||b.Root!=binding.Root||b.Approval!=binding.Approval||b.Device!=binding.Device||b.Pid!=binding.Pid||b.Creation!=binding.Creation||b.CounterBaseline!=binding.CounterBaseline||baseline!=binding.CounterBaseline){Fail("BINDING_OR_REPLAY_DENY");return false;}
   try{
    if(fault=="before"){Fail("STOP_BEFORE_CONSUME");return false;}
    if(nv.Read()!=baseline||baseline==UInt64.MaxValue){Fail("COUNTER_DIVERGENCE");return false;}
    nv.Increment();
    if(fault=="after"){Fail("CRASH_AFTER_INCREMENT");return false;}
    if(nv.Read()!=baseline+1){Fail("READBACK_DIVERGENCE");return false;}
    if(fault=="commit"){Fail("LOCAL_PERSISTENCE_UNKNOWN");return false;}
    Committed=true;
    if(fault=="close"){Fail("CLOSE_UNKNOWN");return false;}
    Reads++;return true; // synthetic observation counter only; NEVER a protected reader
   }catch(InvalidOperationException e){Fail(e.Message);return false;}
  }
 }
}
class Tests {
 static int checks;static void Check(bool b){checks++;if(!b)throw new Exception("CHECK_"+checks);}
 static string Hex(byte[] b){return BitConverter.ToString(b).Replace("-","").ToLowerInvariant();}
 static Lease B(){return new Lease("10584f02e11f92c92b43cddf0d9d316f","SYNTHETIC_ROOT","SYNTHETIC_APPROVAL","SYNTHETIC_DEVICE",123,456);}
 static FakeNv Init(){FakeNv n=new FakeNv();n.Define(32);n.Increment();return n;}
 static void Denied(Lease b,string fault,bool restart){FakeNv n=Init();Protocol p=new Protocol(n,B());Check(!p.Consume(b,n.Value,restart,fault));Check(p.Reads==0);Check(p.First!="NONE");int count=n.Increments;Check(!p.Consume(B(),n.Value,false,""));Check(n.Increments==count);}
 static int Main(){
  Check(Hex(PublicArea.Encode(PublicArea.Index,PublicArea.Attributes))=="01804143000b0004001400000008");
  Check(PublicArea.Name(false).Length==34);Check(!PublicArea.Same(PublicArea.Name(false),PublicArea.Name(true)));Check(PublicArea.Same(PublicArea.Name(true),PublicArea.Name(true)));
  for(int bit=0;bit<32;bit++){UInt32 a=PublicArea.Attributes^(1U<<bit);if(a==(PublicArea.Attributes|PublicArea.Written))continue;bool bad=false;try{PublicArea.Encode(PublicArea.Index,a);}catch(InvalidOperationException){bad=true;}Check(bad);}
  FakeNv never=new FakeNv();bool denied=false;try{never.Read();}catch(InvalidOperationException){denied=true;}Check(denied);never.Define(32);Check(!never.Written);never.Increment();Check(never.Value==82);Check(never.Value!=1);
  denied=false;try{never.Define(32);}catch(InvalidOperationException){denied=true;}Check(denied&&never.Definitions==1);
  denied=false;try{new FakeNv().Define(0);}catch(InvalidOperationException){denied=true;}Check(denied);
  FakeNv n=Init();UInt64 baseline=n.Value;Protocol p=new Protocol(n,B());Check(p.Consume(B(),baseline,false,""));Check(p.Reads==1&&p.Committed);Check(!p.Consume(B(),baseline,false,""));Check(n.Value==baseline+1);
  Protocol fresh=new Protocol(n,B());Check(!fresh.Consume(B(),baseline,false,""));Check(fresh.Reads==0);Check(n.Value==baseline+1);
  Protocol rebase=new Protocol(n,B());Check(!rebase.Consume(B(),n.Value,false,""));Check(n.Value==baseline+1);
  foreach(string f in new string[]{"before","after","commit","close"})Denied(B(),f,false);
  Denied(B(),"",true);Denied(new Lease("OLD","SYNTHETIC_ROOT","SYNTHETIC_APPROVAL","SYNTHETIC_DEVICE",123,456),"",false);
  Denied(new Lease(B().Historic,"WRONG_ROOT",B().Approval,B().Device,123,456),"",false);
  Denied(new Lease(B().Historic,B().Root,"WRONG_APPROVAL",B().Device,123,456),"",false);
  Denied(new Lease(B().Historic,B().Root,B().Approval,"WRONG_DEVICE",123,456),"",false);
  Denied(new Lease(B().Historic,B().Root,B().Approval,B().Device,123,457),"",false);
  foreach(string f in new string[]{"access","auth","unknown","exhaust"}){
   n=Init();if(f=="access")n.Available=false;if(f=="auth")n.Auth=false;if(f=="unknown")n.UnknownIncrement=true;if(f=="exhaust")n.Value=UInt64.MaxValue;
   p=new Protocol(n,B());Check(!p.Consume(B(),n.Value,false,""));Check(p.Retained&&p.Reads==0);string first=p.First;Check(!p.Consume(B(),n.Value,false,""));Check(p.First==first);
  }
  n=Init();p=new Protocol(n,B());baseline=n.Value;int wins=0;Thread[] threads=new Thread[16];for(int i=0;i<16;i++){threads[i]=new Thread(delegate(){if(p.Consume(B(),baseline,false,""))Interlocked.Increment(ref wins);});threads[i].Start();}foreach(Thread t in threads)t.Join();Check(wins==1&&p.Reads==1&&n.Value==baseline+1);
  Console.WriteLine("PASS checks="+checks+"; SIMULATED_NV_ONLY; NO_NATIVE_MUTATIONS; NO_AUTHORITY");return 0;
 }
}
