using System;using System.Text;
static class DefineCodec {
 static byte[] Pub(){return new byte[]{1,0x80,0x41,0x43,0,11,0,4,0,0x14,0,0,0,8};}
 static byte[] Trim(byte[] b){int n=b.Length;while(n>0&&b[n-1]==0)n--;byte[] t=new byte[n];Array.Copy(b,t,n);return t;}
 public static byte[] Build(byte[] session,byte[] owner,byte[] newAuth,byte[] caller,byte[] tpm){
  if(session.Length!=32||owner.Length!=32||newAuth.Length!=32||caller.Length!=32||tpm.Length!=32)throw new InvalidOperationException("DEFINE_BINDING_DENY");
  byte[] trimmed=Trim(owner),key=Codec.Cat(session,trimmed),material=null,cfbKey=null,iv=null;
  try{
   material=Codec.Hmac(key,Codec.Cat(Codec.D(1),Encoding.ASCII.GetBytes("CFB\0"),caller,tpm,Codec.D(256)));
   cfbKey=new byte[16];iv=new byte[16];Array.Copy(material,cfbKey,16);Array.Copy(material,16,iv,0,16);
   byte[] cipher=NativeCfb.Encrypt(cfbKey,iv,newAuth);
   byte[] parameters=Codec.Cat(new byte[]{0,32},cipher,new byte[]{0,14},Pub());
   byte[] cp=Codec.Hash(Codec.Cat(Codec.D(0x12a),Codec.D(0x40000001),parameters));
   byte[] mac=Codec.Auth(session,owner,cp,caller,tpm,0x20);
   byte[] auth=Codec.Cat(Codec.D(0x02000001),new byte[]{0,32},caller,new byte[]{0x20,0,32},mac);
   return Codec.Cat(new byte[]{0x80,2},Codec.D(141),Codec.D(0x12a),Codec.D(0x40000001),Codec.D(73),auth,parameters);
  }finally{Array.Clear(trimmed,0,trimmed.Length);Array.Clear(key,0,key.Length);if(material!=null)Array.Clear(material,0,material.Length);if(cfbKey!=null)Array.Clear(cfbKey,0,cfbKey.Length);if(iv!=null)Array.Clear(iv,0,iv.Length);}
 }
}
class DefineTests {
 static int checks;static void C(bool v){checks++;if(!v)throw new Exception("CHECK_"+checks);}
 static byte[] B(byte v){byte[] b=new byte[32];for(int i=0;i<32;i++)b[i]=v;return b;}
 static int Main(){try{byte[] auth=B(7),packet=DefineCodec.Build(B(1),B(2),auth,B(3),B(4));C(packet.Length==141);C(Adapter.U(packet,6)==0x12a&&Adapter.U(packet,10)==0x40000001);C(Adapter.U(packet,14)==73);C(packet[56]==0x20);C(packet[91]==0&&packet[92]==32);C(packet[125]==0&&packet[126]==14);C(Adapter.U(packet,127)==0x01804143);C(!Adapter.Allowed(packet));
  byte[] cipher=new byte[32];Array.Copy(packet,93,cipher,0,32);C(!Codec.Eq(cipher,auth));byte[] changed=DefineCodec.Build(B(1),B(2),auth,B(5),B(4));C(!Codec.Eq(packet,changed));byte[] secondCipher=new byte[32];Array.Copy(changed,93,secondCipher,0,32);C(!Codec.Eq(cipher,secondCipher));
  byte[] differentKey=DefineCodec.Build(B(1),B(9),auth,B(3),B(4));C(!Codec.Eq(packet,differentKey));
  bool denied=false;try{DefineCodec.Build(new byte[0],B(2),auth,B(3),B(4));}catch(InvalidOperationException){denied=true;}C(denied);
  for(int i=0;i<32;i++){auth[i]^=1;byte[] m=DefineCodec.Build(B(1),B(2),auth,B(3),B(4));C(!Codec.Eq(m,packet));C(!Adapter.Allowed(m));auth[i]^=1;}
  Console.WriteLine("PASS checks="+checks+" DEFINE_PACKET_ONLY_NATIVE_CNG_CFB; NO_TPM_WRITE_NO_SESSION_CREATION");return 0;
 }catch(Exception e){Console.WriteLine("FIRST="+e.Message+" NO_TPM_NO_RETRY");return 2;}}
}
