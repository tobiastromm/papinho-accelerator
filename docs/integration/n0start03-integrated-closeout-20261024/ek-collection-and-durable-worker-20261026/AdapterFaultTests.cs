using System;
// No native transport in this class. Replies/close status injected; one command maximum.
sealed class WriteOnceFixture {
 public string First="NONE";public bool Frozen,Retained;public int Calls;bool issued;
 public bool Send(byte[] request,byte[] response,byte[] session,byte[] auth,byte[] caller,bool close){
  if(issued||Frozen)return false;issued=true;
  if(request==null||request.Length!=95||Adapter.U(request,6)!=0x134){First="SERIALIZATION_DENY";Frozen=true;return false;}
  Calls++;bool ok=false;
  if(response==null||response.Length<10||Adapter.U(response,2)!=(uint)response.Length){First="RESPONSE_UNKNOWN";Retained=true;}
  else if(response.Length==10&&response[0]==0x80&&response[1]==1&&Adapter.U(response,6)!=0){First="TPM_"+Adapter.U(response,6).ToString("x8");}
  else if(Codec.Response(response,session,auth,caller))ok=true;
  else{First="RESPONSE_AUTH_UNKNOWN";Retained=true;}
  if(!close){if(First=="NONE")First="CLOSE_UNKNOWN";Retained=true;ok=false;}
  Frozen=!ok||Retained;return ok;
 }
}
class AdapterFaultTests {
 static int checks;static void C(bool v){checks++;if(!v)throw new Exception("CHECK_"+checks);}
 static byte[] B(byte x){byte[] b=new byte[32];for(int n=0;n<b.Length;n++)b[n]=x;return b;}
 static byte[] Reply(byte[] key,byte[] auth,byte[] caller){byte[] nonce=B(4),rp=Codec.Hash(Codec.Cat(Codec.D(0),Codec.D(0x134)));return Codec.Cat(new byte[]{0x80,2},Codec.D(83),Codec.D(0),Codec.D(0),new byte[]{0,32},nonce,new byte[]{0,0,32},Codec.Auth(key,auth,rp,nonce,caller));}
 static int Main(){byte[] key=B(1),auth=B(2),caller=B(3),name=Codec.Cat(new byte[]{0,11},B(6));byte[] request=Codec.Increment(key,auth,name,caller,B(5));C(!Adapter.Allowed(request));
 foreach(uint rc in new uint[]{0x9a2,0x922,0x920,0x902,0x101}){WriteOnceFixture w=new WriteOnceFixture();byte[] error=Codec.Cat(new byte[]{0x80,1},Codec.D(10),Codec.D(rc));C(!w.Send(request,error,key,auth,caller,true));C(w.First=="TPM_"+rc.ToString("x8"));C(!w.Send(request,Reply(key,auth,caller),key,auth,caller,true));C(w.Calls==1);}
 for(int n=0;n<10;n++){WriteOnceFixture w=new WriteOnceFixture();C(!w.Send(request,new byte[n],key,auth,caller,true));C(w.Retained);C(!w.Send(request,Reply(key,auth,caller),key,auth,caller,true));C(w.Calls==1);}
 WriteOnceFixture clean=new WriteOnceFixture();C(clean.Send(request,Reply(key,auth,caller),key,auth,caller,true));C(!clean.Send(request,Reply(key,auth,caller),key,auth,caller,true));
 WriteOnceFixture close=new WriteOnceFixture();C(!close.Send(request,Reply(key,auth,caller),key,auth,caller,false));C(close.First=="CLOSE_UNKNOWN"&&close.Retained);
 WriteOnceFixture first=new WriteOnceFixture();C(!first.Send(request,Codec.Cat(new byte[]{0x80,1},Codec.D(10),Codec.D(0x9a2)),key,auth,caller,false));C(first.First=="TPM_000009a2"&&first.Retained);
 Console.WriteLine("PASS checks="+checks+" SERIALIZATION_AND_RC_MOCK_ONLY; NO_NATIVE_WRITE; NO_RETRY");return 0;}
}
