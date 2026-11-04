using System;using System.Text;using System.Security.Cryptography;using System.Runtime.InteropServices;
// Isolated software RSA/CNG fixture. No TBS, NCrypt persisted-key creation, CA, certificate or authority.
static class Credential {
 [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Unicode)]struct Padding { [MarshalAs(UnmanagedType.LPWStr)] public string alg;public IntPtr label;public int length;}
 [DllImport("ncrypt.dll")]static extern int NCryptEncrypt(IntPtr key,byte[] input,int length,ref Padding info,byte[] output,int capacity,out int used,int flags);
 [DllImport("ncrypt.dll")]static extern int NCryptDecrypt(IntPtr key,byte[] input,int length,ref Padding info,byte[] output,int capacity,out int used,int flags);
 public static byte[] Rsa(RSACng rsa,byte[] input,bool decrypt,string label){
  byte[] labelBytes=Encoding.ASCII.GetBytes(label+"\0"),output=new byte[256];GCHandle pin=GCHandle.Alloc(labelBytes,GCHandleType.Pinned);bool borrowed=false;
  using(Microsoft.Win32.SafeHandles.SafeNCryptKeyHandle handle=rsa.Key.Handle){
   try{handle.DangerousAddRef(ref borrowed);Padding pad=new Padding();pad.alg="SHA256";pad.label=pin.AddrOfPinnedObject();pad.length=labelBytes.Length;int n;
    int e=decrypt?NCryptDecrypt(handle.DangerousGetHandle(),input,input.Length,ref pad,output,output.Length,out n,4):NCryptEncrypt(handle.DangerousGetHandle(),input,input.Length,ref pad,output,output.Length,out n,4);
    if(e!=0||n<=0||n>256)throw new InvalidOperationException("RSA_OAEP_DENY");byte[] result=new byte[n];Array.Copy(output,result,n);return result;
   }finally{Array.Clear(output,0,output.Length);if(borrowed)handle.DangerousRelease();pin.Free();}
  }
 }
 public static byte[] Cat(params byte[][] parts){int n=0;foreach(byte[] p in parts)n=checked(n+p.Length);byte[] b=new byte[n];n=0;foreach(byte[] p in parts){Array.Copy(p,0,b,n,p.Length);n+=p.Length;}return b;}
 public static byte[] D(uint n){return new byte[]{(byte)(n>>24),(byte)(n>>16),(byte)(n>>8),(byte)n};}
 public static byte[] Mac(byte[] k,byte[] b){using(HMACSHA256 h=new HMACSHA256(k))return h.ComputeHash(b);}
 public static byte[] Kdf(byte[] seed,string label,byte[] name,int bits){byte[] full=Mac(seed,Cat(D(1),Encoding.ASCII.GetBytes(label+"\0"),name,D((uint)bits)));byte[] key=new byte[bits/8];Array.Copy(full,key,key.Length);Array.Clear(full,0,full.Length);return key;}
 public static bool Eq(byte[] a,byte[] b){if(a.Length!=b.Length)return false;int x=0;for(int n=0;n<a.Length;n++)x|=a[n]^b[n];return x==0;}
 public static byte[][] Make(RSACng publicOnly,byte[] seed,byte[] name,byte[] challenge){
  if(seed.Length!=32||name.Length!=34||name[0]!=0||name[1]!=11||challenge.Length!=30)throw new InvalidOperationException("SHAPE_DENY");
  byte[] sym=Kdf(seed,"STORAGE",name,128),integrity=Kdf(seed,"INTEGRITY",new byte[0],256),plain=Cat(new byte[]{0,30},challenge);
  try{byte[] cipher=NativeCfb.Encrypt(sym,new byte[16],plain);byte[] mac=Mac(integrity,Cat(cipher,name));return new byte[][]{Rsa(publicOnly,seed,false,"IDENTITY"),cipher,mac};}
  finally{Array.Clear(sym,0,sym.Length);Array.Clear(integrity,0,integrity.Length);Array.Clear(plain,0,plain.Length);}
 }
 public static byte[] Open(RSACng softwarePrivate,byte[][] blob,byte[] name){
  if(blob.Length!=3||blob[0].Length!=256||blob[1].Length!=32||blob[2].Length!=32||name.Length!=34||name[0]!=0||name[1]!=11)throw new InvalidOperationException("SHAPE_DENY");
  byte[] seed=Rsa(softwarePrivate,blob[0],true,"IDENTITY"),sym=null,integrity=null,plain=null;
  try{if(seed.Length!=32)throw new InvalidOperationException("SEED_DENY");sym=Kdf(seed,"STORAGE",name,128);integrity=Kdf(seed,"INTEGRITY",new byte[0],256);
   if(!Eq(blob[2],Mac(integrity,Cat(blob[1],name))))throw new InvalidOperationException("INTEGRITY_DENY");plain=NativeCfbDecrypt.Decrypt(sym,new byte[16],blob[1]);
   if(plain.Length!=32||plain[0]!=0||plain[1]!=30)throw new InvalidOperationException("CREDENTIAL_DENY");byte[] result=new byte[30];Array.Copy(plain,2,result,0,30);return result;
  }finally{Array.Clear(seed,0,seed.Length);if(sym!=null)Array.Clear(sym,0,sym.Length);if(integrity!=null)Array.Clear(integrity,0,integrity.Length);if(plain!=null)Array.Clear(plain,0,plain.Length);}
 }
}
class CredentialTests {
 static int checks;static void C(bool yes){checks++;if(!yes)throw new Exception("CHECK_"+checks);}
 static void Deny(Action a){bool denied=false;try{a();}catch(InvalidOperationException){denied=true;}C(denied);}
 static byte[] B(int n,byte start){byte[] b=new byte[n];for(int i=0;i<n;i++)b[i]=(byte)(start+i);return b;}
 static int Main(){try{
  using(RSACng softwarePrivate=new RSACng(2048))using(RSACng publicOnly=new RSACng())using(RSACng wrongPrivate=new RSACng(2048)){
   C(softwarePrivate.Key.IsEphemeral);C(wrongPrivate.Key.IsEphemeral);publicOnly.ImportParameters(softwarePrivate.ExportParameters(false));C(publicOnly.Key.IsEphemeral);
   byte[] seed=B(32,1),name=Credential.Cat(new byte[]{0,11},B(32,3)),challenge=B(30,7);byte[][] blob=Credential.Make(publicOnly,seed,name,challenge);
   C(Credential.Eq(challenge,Credential.Open(softwarePrivate,blob,name)));
   C(BitConverter.ToString(Credential.Kdf(seed,"STORAGE",name,128)).Replace("-","").ToLowerInvariant()=="10e5d45ca196ea4b158f7f7f2276d88f");
   C(BitConverter.ToString(Credential.Kdf(seed,"INTEGRITY",new byte[0],256)).Replace("-","").ToLowerInvariant()=="094f9376bf7def7a00894381e0887d107ee8c0f04e09748c5dc96b7be9232c5a");
   Deny(delegate{Credential.Open(wrongPrivate,blob,name);});
   Deny(delegate{Credential.Rsa(softwarePrivate,blob[0],true,"WRONG_LABEL");});
   for(int part=0;part<3;part++)for(int n=0;n<blob[part].Length;n++){blob[part][n]^=1;Deny(delegate{Credential.Open(softwarePrivate,blob,name);});blob[part][n]^=1;}
   for(int n=0;n<name.Length;n++){name[n]^=1;Deny(delegate{Credential.Open(softwarePrivate,blob,name);});name[n]^=1;}
   Deny(delegate{Credential.Make(publicOnly,new byte[0],name,challenge);});
   Deny(delegate{Credential.Open(softwarePrivate,new byte[][]{new byte[0],blob[1],blob[2]},name);});
   Array.Clear(seed,0,seed.Length);Array.Clear(challenge,0,challenge.Length);
  }
  Console.WriteLine("PASS checks="+checks+" SOFTWARE_CREDENTIAL_ONLY; RSA_OAEP_SHA256_IDENTITY_NUL; CNG_AES_CFB128; NO_TPM_NO_PERSISTED_KEY; MANAGED_DISPOSE_RETURNED");return 0;
 }catch(Exception e){Console.WriteLine("FIRST="+e.GetType().Name+":"+e.Message+"; NO_TPM_NO_RETRY");return 2;}}
}
