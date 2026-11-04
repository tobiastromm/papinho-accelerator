using System;
static class PublicAreaGate {
 public static byte[] Validate(byte[] r,byte[] expectedModulus,out byte[] name){name=null;
  if(r==null||r.Length<20||r.Length>1024||r[0]!=0x80||r[1]!=1||SaltedWire.U(r,2)!=(uint)r.Length||SaltedWire.U(r,6)!=0)throw new InvalidOperationException("READ_PUBLIC_FRAME");
  int len=(r[10]<<8)|r[11];if(len!=314||r.Length!=398)throw new InvalidOperationException("PUBLIC_PROFILE_SIZE");byte[] p=SaltedWire.Slice(r,12,len);
  if(p[0]!=0||p[1]!=1||p[2]!=0||p[3]!=11||SaltedWire.U(p,4)!=0x000300b2U||p[8]!=0||p[9]!=32||p[42]!=0||p[43]!=6||p[44]!=0||p[45]!=128||p[46]!=0||p[47]!=0x43||p[48]!=0||p[49]!=0x10||p[50]!=8||p[51]!=0||p[56]!=1||p[57]!=0)throw new InvalidOperationException("PUBLIC_PROFILE");
  uint exponent=SaltedWire.U(p,52);if(exponent!=0&&exponent!=65537)throw new InvalidOperationException("EXPONENT");if(expectedModulus.Length!=256||!Credential.Eq(SaltedWire.Slice(p,58,256),expectedModulus))throw new InvalidOperationException("EK_MISMATCH");
  if(r[326]!=0||r[327]!=34||r[362]!=0||r[363]!=34||r[364]!=0||r[365]!=11)throw new InvalidOperationException("NAME_FRAME");name=SaltedWire.Slice(r,328,34);if(!Credential.Eq(name,Credential.Cat(new byte[]{0,11},SaltedWire.Hash(p))))throw new InvalidOperationException("NAME_HASH");return p;
 }
 public static uint[] Handles(byte[] r){if(r==null||r.Length<19||r[0]!=0x80||r[1]!=1||SaltedWire.U(r,2)!=(uint)r.Length||SaltedWire.U(r,6)!=0||r[10]!=0||SaltedWire.U(r,11)!=1)throw new InvalidOperationException("CAPABILITY_INCOMPLETE");uint count=SaltedWire.U(r,15);if(count>8||r.Length!=19+count*4)throw new InvalidOperationException("HANDLE_COUNT");uint[] hs=new uint[count];for(int i=0;i<hs.Length;i++){hs[i]=SaltedWire.U(r,19+i*4);if((hs[i]&0xff000000U)!=0x81000000U||(i>0&&hs[i]<=hs[i-1]))throw new InvalidOperationException("HANDLE_ORDER");}return hs;}
}
class PublicAreaTests {
 static int checks;static void C(bool x){checks++;if(!x)throw new Exception("CHECK_"+checks);}
 static byte[] B(int n,byte v){byte[] b=new byte[n];for(int i=0;i<n;i++)b[i]=v;return b;}
 static byte[] Packet(byte[] pub){byte[] name=Credential.Cat(new byte[]{0,11},SaltedWire.Hash(pub));return Credential.Cat(new byte[]{0x80,1},Credential.D(398),Credential.D(0),new byte[]{1,58},pub,new byte[]{0,34},name,new byte[]{0,34,0,11},B(32,9));}
 static int Main(){byte[] mod=B(256,7);mod[0]=0x80;byte[] pub=Credential.Cat(new byte[]{0,1,0,11},Credential.D(0x300b2),new byte[]{0,32},B(32,6),new byte[]{0,6,0,128,0,0x43,0,0x10,8,0},Credential.D(0),new byte[]{1,0},mod);C(pub.Length==314);byte[] response=Packet(pub),name;C(Credential.Eq(PublicAreaGate.Validate(response,mod,out name),pub));C(name.Length==34);
 for(int i=0;i<response.Length;i++){if(i>=364)continue;byte[] bad=(byte[])response.Clone();bad[i]^=1;bool denied=false;try{PublicAreaGate.Validate(bad,mod,out name);}catch(InvalidOperationException){denied=true;}C(denied);}
 byte[] cap=Credential.Cat(new byte[]{0x80,1},Credential.D(27),Credential.D(0),new byte[]{0},Credential.D(1),Credential.D(2),Credential.D(0x81000001),Credential.D(0x81000002));C(PublicAreaGate.Handles(cap).Length==2);cap[10]=1;bool reject=false;try{PublicAreaGate.Handles(cap);}catch(InvalidOperationException){reject=true;}C(reject);
 Console.WriteLine("PASS checks="+checks+" PUBLIC_AREA_NAME_AND_EK_MATCH_MOCK_ONLY; NO_TPM");return 0;
 }
}
