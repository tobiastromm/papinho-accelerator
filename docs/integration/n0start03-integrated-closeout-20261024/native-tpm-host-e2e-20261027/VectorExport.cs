using System;using System.IO;using System.Text;
class VectorExport {
 static byte[] B(int n,int v){byte[] b=new byte[n];for(int i=0;i<n;i++)b[i]=(byte)(v+i);return b;}
 static void Item(Stream s,byte[] b){byte[] n=Credential.D((uint)b.Length);s.Write(n,0,4);s.Write(b,0,b.Length);}
 static int Main(string[] args){byte[] salt=B(32,1),start=B(32,2),caller=B(32,3),tpm=B(32,4),key=SaltedWire.Kdf(salt,"ATH",tpm,start,256),cipher=B(256,0),nonce=B(32,7),material=SaltedWire.Kdf(key,"CFB",nonce,caller,256),encrypted=NativeCfb.Encrypt(SaltedWire.Slice(material,0,16),SaltedWire.Slice(material,16,16),B(32,9));
 byte[] parameters=Credential.Cat(new byte[]{0,32},encrypted),rp=SaltedWire.Hash(Credential.Cat(Credential.D(0),Credential.D(0x17b),parameters)),mac=Credential.Mac(key,Credential.Cat(rp,nonce,caller,new byte[]{0x40}));
 byte[] reply=Credential.Cat(new byte[]{0x80,2},Credential.D(117),Credential.D(0),Credential.D(34),parameters,new byte[]{0,32},nonce,new byte[]{0x40,0,32},mac),mod=B(256,7);mod[0]=0x80;
 byte[] area=Credential.Cat(new byte[]{0,1,0,11},Credential.D(0x300b2),new byte[]{0,32},B(32,6),new byte[]{0,6,0,128,0,0x43,0,0x10,8,0},Credential.D(0),new byte[]{1,0},mod),name=Credential.Cat(new byte[]{0,11},SaltedWire.Hash(area));
 byte[] pub=Credential.Cat(new byte[]{0x80,1},Credential.D(398),Credential.D(0),new byte[]{1,58},area,new byte[]{0,34},name,new byte[]{0,34,0,11},B(32,9));
 byte[] cap=Credential.Cat(new byte[]{0x80,1},Credential.D(27),Credential.D(0),new byte[]{0},Credential.D(1),Credential.D(2),Credential.D(0x81000001),Credential.D(0x81000002));
 using(FileStream f=new FileStream(args[0],FileMode.CreateNew,FileAccess.Write,FileShare.None)){byte[] magic=Encoding.ASCII.GetBytes("TPVECT01");f.Write(magic,0,8);
 foreach(byte[] b in new byte[][]{salt,start,caller,tpm,key,SaltedWire.Start(0x81000001,start,cipher),Credential.Cat(new byte[]{0x80,1},Credential.D(48),Credential.D(0),Credential.D(0x02000001),new byte[]{0,32},tpm),SaltedWire.Query(0x02000001,key,caller,tpm),reply,pub,cap,encrypted})Item(f,b);
 byte[] mask=new byte[pub.Length];for(int i=0;i<pub.Length;i++){byte[] a=(byte[])pub.Clone();a[i]^=1;byte[] ignored;try{PublicAreaGate.Validate(a,mod,out ignored);mask[i]=1;}catch(InvalidOperationException){mask[i]=0;}}Item(f,mask);f.Flush(true);}
 Console.WriteLine("PASS CSHARP_ORACLE_EXPORT synthetic public fixtures only; no TBS/TPM or private material exported");return 0;}
}