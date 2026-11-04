using System;using System.Text;using System.Security.Cryptography;
// Packet/cryptographic verifier with MOCK endpoint only. No TBS imports or native entrypoint.
static class SaltedWire {
 public static byte[] S(int x){return new byte[]{(byte)(x>>8),(byte)x};}
 public static uint U(byte[] b,int p){return ((uint)b[p]<<24)|((uint)b[p+1]<<16)|((uint)b[p+2]<<8)|b[p+3];}
 public static byte[] Hash(byte[] b){using(SHA256 s=SHA256.Create())return s.ComputeHash(b);}
 public static byte[] Slice(byte[] b,int off,int n){if(off<0||n<0||off>b.Length-n)throw new InvalidOperationException("FRAME_DENY");byte[] x=new byte[n];Array.Copy(b,off,x,0,n);return x;}
 public static byte[] Kdf(byte[] key,string label,byte[] u,byte[] v,int bits){byte[] full=Credential.Mac(key,Credential.Cat(Credential.D(1),Encoding.ASCII.GetBytes(label+"\0"),u,v,Credential.D((uint)bits)));return Slice(full,0,bits/8);}
 public static byte[] Start(uint persistentHandle,byte[] nonce,byte[] encryptedSalt){if((persistentHandle&0xff000000U)!=0x81000000U||nonce.Length!=32||encryptedSalt.Length!=256)throw new InvalidOperationException("START_BINDING_DENY");return Credential.Cat(new byte[]{0x80,1},Credential.D(319),Credential.D(0x176),Credential.D(persistentHandle),Credential.D(0x40000007),S(32),nonce,S(256),encryptedSalt,new byte[]{0},S(6),S(128),S(0x43),S(11));}
 public static uint StartResponse(byte[] r,out byte[] nonce){nonce=null;if(r==null||r.Length!=48||r[0]!=0x80||r[1]!=1||U(r,2)!=48||U(r,6)!=0||(U(r,10)&0xff000000U)!=0x02000000U||r[14]!=0||r[15]!=32)throw new InvalidOperationException("START_RESPONSE_UNKNOWN");nonce=Slice(r,16,32);return U(r,10);}
 public static byte[] Query(uint session,byte[] key,byte[] nonceCaller,byte[] nonceTpm){if((session&0xff000000U)!=0x02000000U||key.Length!=32||nonceCaller.Length!=32||nonceTpm.Length!=32)throw new InvalidOperationException("SESSION_DENY");byte[] cp=Hash(Credential.Cat(Credential.D(0x17b),S(32)));byte[] mac=Credential.Mac(key,Credential.Cat(cp,nonceCaller,nonceTpm,new byte[]{0x40}));byte[] auth=Credential.Cat(Credential.D(session),S(32),nonceCaller,new byte[]{0x40},S(32),mac);return Credential.Cat(new byte[]{0x80,2},Credential.D(89),Credential.D(0x17b),Credential.D(73),auth,S(32));}
 public static bool Verify(byte[] response,byte[] key,byte[] caller,out byte[] recovered){
  recovered=null;if(key==null||key.Length!=32||caller==null||caller.Length!=32||response==null||response.Length<85||response.Length>117||response[0]!=0x80||response[1]!=2||U(response,2)!=(uint)response.Length||U(response,6)!=0||response[14]!=0)return false;
  int n=response[15],a=16+n;if(n>32||response.Length!=85+n||U(response,10)!=(uint)(n+2)||response[a]!=0||response[a+1]!=32||response[a+34]!=0x40||response[a+35]!=0||response[a+36]!=32)return false;
  byte[] parameters=Slice(response,14,n+2),nonce=Slice(response,a+2,32),rp=Hash(Credential.Cat(Credential.D(0),Credential.D(0x17b),parameters));byte[] expected=Credential.Mac(key,Credential.Cat(rp,nonce,caller,new byte[]{0x40}));if(!Credential.Eq(expected,Slice(response,a+37,32)))return false;
  // HMAC knowledge of the fresh secret salt proves the selected private-key relationship.
  // Short/empty GetRandom output is legal; no plaintext RNG quality/decryption claim is needed.
  recovered=new byte[0];return true;
 }

}
sealed class SaltedProofOnce {
 readonly object gate=new object();bool used,stopped;public string First="NONE";
 public bool Accept(byte[] response,byte[] key,byte[] nonce,byte[] approvedContext,byte[] currentContext){lock(gate){if(used||stopped)return false;used=true;if(!Credential.Eq(approvedContext,currentContext)){First="CONTEXT_DENY";return false;}byte[] recovered;if(!SaltedWire.Verify(response,key,nonce,out recovered)){First="HMAC_OR_FRAME_UNKNOWN";return false;}Array.Clear(recovered,0,recovered.Length);return true;}}
 public void Stop(){lock(gate){stopped=true;if(First=="NONE")First="STOP";}}
}
class SaltedProofTests {
 static int checks;static void C(bool x){checks++;if(!x)throw new Exception("CHECK_"+checks);}
 static byte[] B(byte v){byte[] x=new byte[32];for(int n=0;n<32;n++)x[n]=(byte)(v+n);return x;}
 static byte[] Response(byte[] key,byte[] caller){byte[] nonce=B(7),material=SaltedWire.Kdf(key,"CFB",nonce,caller,256);byte[] cipher=NativeCfb.Encrypt(SaltedWire.Slice(material,0,16),SaltedWire.Slice(material,16,16),B(9));byte[] parameters=Credential.Cat(new byte[]{0,32},cipher);byte[] rp=SaltedWire.Hash(Credential.Cat(Credential.D(0),Credential.D(0x17b),parameters));byte[] mac=Credential.Mac(key,Credential.Cat(rp,nonce,caller,new byte[]{0x40}));return Credential.Cat(new byte[]{0x80,2},Credential.D(117),Credential.D(0),Credential.D(34),parameters,new byte[]{0,32},nonce,new byte[]{0x40,0,32},mac);}
 static int Main(){try{byte[] salt=B(1),callerStart=B(2),callerProof=B(3),nonceStart=B(4),context=B(5);
 using(RSACng privateSoftware=new RSACng(2048))using(RSACng publicOnly=new RSACng()){
 publicOnly.ImportParameters(privateSoftware.ExportParameters(false));byte[] encryptedSalt=Credential.Rsa(publicOnly,salt,false,"SECRET");C(Credential.Eq(Credential.Rsa(privateSoftware,encryptedSalt,true,"SECRET"),salt));
 byte[] command=SaltedWire.Start(0x81000001,callerStart,encryptedSalt);C(command.Length==319);C(SaltedWire.U(command,6)==0x176&&SaltedWire.U(command,14)==0x40000007);
 byte[] startResponse=Credential.Cat(new byte[]{0x80,1},Credential.D(48),Credential.D(0),Credential.D(0x02000001),new byte[]{0,32},nonceStart),parsedNonce;uint handle=SaltedWire.StartResponse(startResponse,out parsedNonce);C(Credential.Eq(parsedNonce,nonceStart));byte[] key=SaltedWire.Kdf(salt,"ATH",parsedNonce,callerStart,256);C(key.Length==32);byte[] query=SaltedWire.Query(handle,key,callerProof,nonceStart);C(query.Length==89&&SaltedWire.U(query,6)==0x17b&&query[52]==0x40);
 byte[] response=Response(key,callerProof),recovered;C(SaltedWire.Verify(response,key,callerProof,out recovered));C(recovered.Length==0);Array.Clear(recovered,0,recovered.Length);
 for(int j=0;j<response.Length;j++){byte[] bad=(byte[])response.Clone();bad[j]^=1;C(!SaltedWire.Verify(bad,key,callerProof,out recovered));}
 C(!SaltedWire.Verify(response,B(8),callerProof,out recovered));C(!SaltedWire.Verify(response,key,B(8),out recovered));
 for(int j=0;j<startResponse.Length;j++){if(j>=16||(j>=11&&j<=13))continue;byte[] bad=(byte[])startResponse.Clone();bad[j]^=1;bool denied=false;try{SaltedWire.StartResponse(bad,out parsedNonce);}catch(InvalidOperationException){denied=true;}C(denied);}
 foreach(int shortCount in new int[]{0,1,15,16,17,31,32}){byte[] par=Credential.Cat(SaltedWire.S(new byte[shortCount].Length),new byte[shortCount]),nonce=B(7),rp=SaltedWire.Hash(Credential.Cat(Credential.D(0),Credential.D(0x17b),par)),mac=Credential.Mac(key,Credential.Cat(rp,nonce,callerProof,new byte[]{0x40}));byte[] varied=Credential.Cat(new byte[]{0x80,2},Credential.D((uint)(85+shortCount)),Credential.D(0),Credential.D((uint)(2+shortCount)),par,new byte[]{0,32},nonce,new byte[]{0x40,0,32},mac);C(SaltedWire.Verify(varied,key,callerProof,out recovered));C(recovered.Length==0);}
 C(!SaltedWire.Verify(response,new byte[1],callerProof,out recovered));C(!SaltedWire.Verify(response,key,new byte[1],out recovered));
 SaltedProofOnce once=new SaltedProofOnce();C(once.Accept(response,key,callerProof,context,context));C(!once.Accept(response,key,callerProof,context,context));
 once=new SaltedProofOnce();C(!once.Accept(response,key,callerProof,context,B(6)));C(!once.Accept(response,key,callerProof,context,context));
 once=new SaltedProofOnce();once.Stop();C(!once.Accept(response,key,callerProof,context,context));
 SaltedProofOnce concurrent=new SaltedProofOnce();int wins=0;System.Threading.Thread[] ts=new System.Threading.Thread[16];for(int j=0;j<16;j++){ts[j]=new System.Threading.Thread(delegate(){if(concurrent.Accept(response,key,callerProof,context,context))System.Threading.Interlocked.Increment(ref wins);});ts[j].Start();}foreach(System.Threading.Thread t in ts)t.Join();C(wins==1);
 Array.Clear(key,0,key.Length);Array.Clear(salt,0,salt.Length);
 }
 Console.WriteLine("PASS checks="+checks+" SALTED_HMAC_SESSION_MOCK_ONLY; NO_TPM_NO_AUTHORITY");return 0;
 }catch(Exception e){Console.WriteLine("FIRST="+e.Message+";NO_TPM");return 2;}}
}
