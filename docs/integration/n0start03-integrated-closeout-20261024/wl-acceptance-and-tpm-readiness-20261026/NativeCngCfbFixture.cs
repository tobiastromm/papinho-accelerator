using System;using System.Runtime.InteropServices;using System.Text;
static class NativeCfb {
 [DllImport("bcrypt.dll",CharSet=CharSet.Unicode)]static extern int BCryptOpenAlgorithmProvider(out IntPtr h,string a,string impl,uint flags);
 [DllImport("bcrypt.dll",CharSet=CharSet.Unicode)]static extern int BCryptSetProperty(IntPtr h,string p,byte[] b,int n,uint f);
 [DllImport("bcrypt.dll",CharSet=CharSet.Unicode)]static extern int BCryptGetProperty(IntPtr h,string p,byte[] b,int n,out int used,uint f);
 [DllImport("bcrypt.dll")]static extern int BCryptGenerateSymmetricKey(IntPtr a,out IntPtr k,IntPtr obj,int objn,byte[] secret,int n,uint f);
 [DllImport("bcrypt.dll")]static extern int BCryptEncrypt(IntPtr k,byte[] input,int n,IntPtr padding,byte[] iv,int ivn,byte[] output,int outn,out int written,uint f);
 [DllImport("bcrypt.dll")]static extern int BCryptDestroyKey(IntPtr k);
 [DllImport("bcrypt.dll")]static extern int BCryptCloseAlgorithmProvider(IntPtr a,uint f);
 static int busy;static IntPtr retainedAlg,retainedKey;static GCHandle retainedPin;static byte[] retainedObject;
 public static byte[] Encrypt(byte[] secret,byte[] iv,byte[] plaintext){
  if(secret.Length!=16||iv.Length!=16||plaintext.Length<16||plaintext.Length>32||plaintext.Length%16!=0)throw new InvalidOperationException("CFB_SHAPE_DENY");
  if(System.Threading.Interlocked.CompareExchange(ref busy,1,0)!=0)throw new InvalidOperationException("CFB_BUSY_DENY");
  IntPtr a=IntPtr.Zero,k=IntPtr.Zero;GCHandle pin=new GCHandle();byte[] obj=null,localIv=(byte[])iv.Clone(),output=new byte[plaintext.Length];string first=null;bool retain=false;int e,n;
  try{
   e=BCryptOpenAlgorithmProvider(out a,"AES",null,0);if(e!=0||a==IntPtr.Zero){if(e!=0&&a!=IntPtr.Zero)retain=true;throw new InvalidOperationException("OPEN_"+e.ToString("x8"));}
   byte[] mode=Encoding.Unicode.GetBytes("ChainingModeCFB\0");e=BCryptSetProperty(a,"ChainingMode",mode,mode.Length,0);if(e!=0)throw new InvalidOperationException("MODE_"+e.ToString("x8"));
   byte[] size=new byte[4];e=BCryptGetProperty(a,"ObjectLength",size,4,out n,0);if(e!=0||n!=4)throw new InvalidOperationException("OBJECT_LENGTH_DENY");int objectSize=BitConverter.ToInt32(size,0);if(objectSize<=0||objectSize>1048576)throw new InvalidOperationException("OBJECT_LIMIT_DENY");
   obj=new byte[objectSize];pin=GCHandle.Alloc(obj,GCHandleType.Pinned);e=BCryptGenerateSymmetricKey(a,out k,pin.AddrOfPinnedObject(),obj.Length,secret,secret.Length,0);if(e!=0||k==IntPtr.Zero){if(e!=0&&k!=IntPtr.Zero)retain=true;throw new InvalidOperationException("KEY_"+e.ToString("x8"));}
   byte[] block=BitConverter.GetBytes(16);e=BCryptSetProperty(k,"MessageBlockLength",block,4,0);if(e!=0)throw new InvalidOperationException("FEEDBACK_"+e.ToString("x8"));
   e=BCryptEncrypt(k,plaintext,plaintext.Length,IntPtr.Zero,localIv,16,output,output.Length,out n,0);if(e!=0||n!=output.Length)throw new InvalidOperationException("ENCRYPT_"+e.ToString("x8"));
  }catch(Exception x){first=x.Message;}
  finally{
   if(!retain&&k!=IntPtr.Zero){e=BCryptDestroyKey(k);if(e!=0){retain=true;if(first==null)first="KEY_CLOSE_UNKNOWN";}else{k=IntPtr.Zero;}}
   if(!retain&&a!=IntPtr.Zero){e=BCryptCloseAlgorithmProvider(a,0);if(e!=0){retain=true;if(first==null)first="ALG_CLOSE_UNKNOWN";}else{a=IntPtr.Zero;}}
   if(retain){retainedAlg=a;retainedKey=k;retainedPin=pin;retainedObject=obj;Console.WriteLine("CNG_UNKNOWN_RETAIN first="+first);while(true)System.Threading.Thread.Sleep(1000);}
   if(pin.IsAllocated)pin.Free();if(obj!=null)Array.Clear(obj,0,obj.Length);Array.Clear(localIv,0,localIv.Length);System.Threading.Interlocked.Exchange(ref busy,0);
  }
  GC.KeepAlive(retainedAlg);GC.KeepAlive(retainedKey);GC.KeepAlive(retainedPin);GC.KeepAlive(retainedObject);
  if(first!=null)throw new InvalidOperationException(first);return output;
 }
}
class NativeCngCfbFixture {
 static byte[] X(string s){byte[] b=new byte[s.Length/2];for(int i=0;i<b.Length;i++)b[i]=Convert.ToByte(s.Substring(2*i,2),16);return b;}
 static int Main(){try{byte[] key=X("2b7e151628aed2a6abf7158809cf4f3c"),iv=X("000102030405060708090a0b0c0d0e0f");byte[] p=X("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51");string actual=BitConverter.ToString(NativeCfb.Encrypt(key,iv,p)).Replace("-","").ToLowerInvariant();if(actual!="3b3fd92eb72dad20333449f8e83cfb4ac8a64537a0b3a93fcde3cdad9f1ce58b")return 2;Console.WriteLine("PASS_CNG_CFB128_2BLOCK_NIST; CLOSE_CONFIRMED; NO_TPM");return 0;}catch(Exception e){Console.WriteLine("FIRST="+e.Message+"; NO_RETRY_NO_TPM");return 2;}}
}
