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
