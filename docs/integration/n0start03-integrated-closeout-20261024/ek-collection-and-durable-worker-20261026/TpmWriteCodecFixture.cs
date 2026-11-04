using System;using System.Security.Cryptography;using System.Text;using System.IO;
// Wire/HMAC preparation and simulated transport ONLY; no native TBS imports.
static class Codec {
 public static byte[] Cat(params byte[][] xs){int n=0;foreach(byte[] x in xs)n=checked(n+x.Length);byte[] b=new byte[n];n=0;foreach(byte[] x in xs){Array.Copy(x,0,b,n,x.Length);n+=x.Length;}return b;}
 public static byte[] D(UInt32 n){return new byte[]{(byte)(n>>24),(byte)(n>>16),(byte)(n>>8),(byte)n};}
 public static byte[] Hash(byte[] b){using(SHA256 s=SHA256.Create())return s.ComputeHash(b);}
 public static byte[] Hmac(byte[] key,byte[] b){using(HMACSHA256 h=new HMACSHA256(key))return h.ComputeHash(b);}
 public static bool Eq(byte[] a,byte[] b){if(a.Length!=b.Length)return false;int x=0;for(int i=0;i<a.Length;i++)x|=a[i]^b[i];return x==0;}
 static byte[] Trim(byte[] b){int n=b.Length;while(n>0&&b[n-1]==0)n--;byte[] r=new byte[n];Array.Copy(b,r,n);return r;}
 public static byte[] Auth(byte[] session,byte[] secret,byte[] digest,byte[] newer,byte[] older,byte attrs=0){if(session.Length!=32||secret.Length!=32||digest.Length!=32||newer.Length!=32||older.Length!=32)throw new InvalidOperationException("AUTH_SHAPE_DENY");byte[] t=Trim(secret),k=Cat(session,t),msg=Cat(digest,newer,older,new byte[]{attrs});try{return Hmac(k,msg);}finally{Array.Clear(t,0,t.Length);Array.Clear(k,0,k.Length);Array.Clear(msg,0,msg.Length);}}
 public static byte[] Increment(byte[] session,byte[] secret,byte[] name,byte[] caller,byte[] tpm){if(name.Length!=34||name[0]!=0||name[1]!=11)throw new InvalidOperationException("NAME_DENY");byte[] cp=Hash(Cat(D(0x134),name,name));byte[] mac=Auth(session,secret,cp,caller,tpm);byte[] auth=Cat(D(0x02000001),new byte[]{0,32},caller,new byte[]{0,0,32},mac);return Cat(new byte[]{0x80,2},D(95),D(0x134),D(0x01804143),D(0x01804143),D((UInt32)auth.Length),auth);}
 public static bool Response(byte[] b,byte[] session,byte[] secret,byte[] caller){if(b==null||b.Length!=83||b[0]!=0x80||b[1]!=2||Adapter.U(b,2)!=83||Adapter.U(b,6)!=0||Adapter.U(b,10)!=0||b[14]!=0||b[15]!=32||b[48]!=0||b[49]!=0||b[50]!=32)return false;byte[] nonce=new byte[32],mac=new byte[32];Array.Copy(b,16,nonce,0,32);Array.Copy(b,51,mac,0,32);byte[] rp=Hash(Cat(D(0),D(0x134)));return Eq(mac,Auth(session,secret,rp,nonce,caller));}
}
sealed class MockOnlyWriteAdapter {
 bool used;public string First="NONE";public bool Retained;public int Commands;
 public bool Once(byte[] request,byte[] response,byte[] session,byte[] secret,byte[] caller,byte[] name,byte[] old){if(used||Retained)return false;used=true;if(request==null||request.Length!=95||Adapter.U(request,6)!=0x134||Adapter.U(request,10)!=Adapter.Index||Adapter.U(request,14)!=Adapter.Index){First="REQUEST_DENY";return false;}if(!Codec.Eq(request,Codec.Increment(session,secret,name,caller,old))){First="REQUEST_AUTH_DENY";return false;}Commands++;if(!Codec.Response(response,session,secret,caller)){First="UNKNOWN_RESPONSE";Retained=true;return false;}return true;}
}
class WriteTests {
 static int checks;static void C(bool v){checks++;if(!v)throw new Exception("CHECK_"+checks);}
 static byte[] B(int n,byte v){byte[] b=new byte[n];for(int i=0;i<n;i++)b[i]=v;return b;}
 static string Hex(byte[] b){return BitConverter.ToString(b).Replace("-","").ToLowerInvariant();}
 static byte[] Reply(byte[] key,byte[] secret,byte[] caller){byte[] nonce=B(32,4);byte[] rp=Codec.Hash(Codec.Cat(Codec.D(0),Codec.D(0x134)));byte[] mac=Codec.Auth(key,secret,rp,nonce,caller);return Codec.Cat(new byte[]{0x80,2},Codec.D(83),Codec.D(0),Codec.D(0),new byte[]{0,32},nonce,new byte[]{0,0,32},mac);}
 static int Main(){byte[] key=B(32,1),secret=B(32,2),caller=B(32,3),old=B(32,5),name=Codec.Cat(new byte[]{0,11},B(32,6));
 C(Hex(Codec.Hmac(B(20,0x0b),Encoding.ASCII.GetBytes("Hi There")))=="b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7");
 byte[] command=Codec.Increment(key,secret,name,caller,old);C(command.Length==95);C(!Adapter.Allowed(command));C(Adapter.U(command,6)==0x134&&Adapter.U(command,18)==73);byte[] response=Reply(key,secret,caller);C(Codec.Response(response,key,secret,caller));
 for(int i=0;i<83;i++){byte[] bad=(byte[])response.Clone();bad[i]^=1;C(!Codec.Response(bad,key,secret,caller));}
 C(!Codec.Response(response,key,secret,B(32,7)));C(!Codec.Response(response,B(32,7),secret,caller));C(!Codec.Response(response,key,B(32,7),caller));
 C(!Codec.Eq(command,Codec.Increment(key,secret,name,B(32,7),old)));C(!Codec.Eq(command,Codec.Increment(key,secret,name,caller,B(32,7))));
 MockOnlyWriteAdapter a=new MockOnlyWriteAdapter();C(a.Once(command,response,key,secret,caller,name,old));C(!a.Once(command,response,key,secret,caller,name,old)&&a.Commands==1);
 for(int j=0;j<95;j++){byte[] bad=(byte[])command.Clone();bad[j]^=1;MockOnlyWriteAdapter reject=new MockOnlyWriteAdapter();C(!reject.Once(bad,response,key,secret,caller,name,old));C(reject.Commands==0);}
  response[82]^=1;a=new MockOnlyWriteAdapter();C(!a.Once(command,response,key,secret,caller,name,old)&&a.Retained);C(!a.Once(command,Reply(key,secret,caller),key,secret,caller,name,old)&&a.Commands==1);C(a.First=="UNKNOWN_RESPONSE");
 Console.WriteLine("PASS checks="+checks+" MOCK_WRITE_ONLY; AUTH_TRANSCRIPT_NOT_NATIVE_SESSION_PROOF");return 0;}
}
class CustodyFixture {
 static byte[] Fixture(){byte[] b=new byte[32];for(int i=0;i<32;i++)b[i]=(byte)(i+1);return b;}
 static int Main(string[] a){if(a.Length!=2)return 90;byte[] binding=Encoding.ASCII.GetBytes("SYNTHETIC_Q4_DEVICE_ROOT_INDEX_01804143_NOT_AUTHORITY");byte[] raw=null;
 try{if(a[0]=="seal"){raw=Fixture();byte[] blob=ProtectedData.Protect(raw,binding,DataProtectionScope.CurrentUser);using(FileStream f=new FileStream(a[1],FileMode.CreateNew,FileAccess.Write,FileShare.None,4096,FileOptions.WriteThrough)){f.Write(blob,0,blob.Length);f.Flush(true);}Console.WriteLine("SYNTHETIC_DPAPI_CIPHERTEXT_ONLY_CREATED");return 0;}
 byte[] stored=File.ReadAllBytes(a[1]);if(a[0]=="wrong_binding")binding[0]^=1;if(a[0]=="corrupt")stored[stored.Length-1]^=1;
 raw=ProtectedData.Unprotect(stored,binding,DataProtectionScope.CurrentUser);if(a[0]!="unseal"||!Codec.Eq(raw,Fixture()))return 2;Console.WriteLine("SYNTHETIC_DPAPI_PROCESS_RESTART=PASS_NO_SECRET_OUTPUT");return 0;
 }catch(CryptographicException e){Console.WriteLine("SYNTHETIC_CUSTODY_DENY_NO_RETRY hresult=0x"+e.HResult.ToString("x8"));return 2;}finally{if(raw!=null)Array.Clear(raw,0,raw.Length);Array.Clear(binding,0,binding.Length);}}
}
