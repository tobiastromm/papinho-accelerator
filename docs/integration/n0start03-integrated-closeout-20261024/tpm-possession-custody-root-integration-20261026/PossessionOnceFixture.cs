using System;using System.Security.Cryptography;
// Isolated verifier state around previously qualified software credential construction.
// No native TPM sender or attestation authority. Recovered bytes injected by fixture.
sealed class PossessionOnce {
 readonly object gate=new object();readonly byte[] context,ek,name,challenge;readonly long issued,maxAge;bool spent,stopped;
 public string First="NONE";
 public PossessionOnce(byte[] c,byte[] e,byte[] n,long now,long age){if(c.Length!=32||e.Length!=32||n.Length!=34||n[0]!=0||n[1]!=11||now<0||age<=0)throw new InvalidOperationException("BINDING_DENY");context=(byte[])c.Clone();ek=(byte[])e.Clone();name=(byte[])n.Clone();issued=now;maxAge=age;challenge=new byte[30];using(RandomNumberGenerator r=RandomNumberGenerator.Create())r.GetBytes(challenge);}
 public byte[][] Make(RSACng publicOnly,byte[] seed){lock(gate){if(spent||stopped)throw new InvalidOperationException("SPENT");return Credential.Make(publicOnly,seed,name,challenge);}}
 public bool Verify(byte[] recovered,byte[] c,byte[] e,byte[] n,long now){lock(gate){if(spent||stopped)return false;spent=true;
  if(now<issued||now-issued>maxAge){First="FRESHNESS_DENY";return false;}
  if(!Credential.Eq(context,c)||!Credential.Eq(ek,e)||!Credential.Eq(name,n)){First="IDENTITY_DENY";return false;}
  if(!Credential.Eq(challenge,recovered)){First="CHALLENGE_DENY";return false;}return true;
 }}
 public void Stop(){lock(gate){stopped=true;if(First=="NONE")First="STOP";Array.Clear(challenge,0,challenge.Length);}}
 public void Clear(){lock(gate){stopped=true;Array.Clear(challenge,0,challenge.Length);}}
}
class PossessionTests {
 static int checks;static void C(bool b){checks++;if(!b)throw new Exception("CHECK_"+checks);}
 static byte[] B(int n,byte x){byte[] b=new byte[n];for(int i=0;i<n;i++)b[i]=x;return b;}
 static int Main(){byte[] c=B(32,1),ek=B(32,2),name=Credential.Cat(new byte[]{0,11},B(32,3)),seed=B(32,4);
 using(RSACng privateSoftware=new RSACng(2048))using(RSACng publicOnly=new RSACng()){
 publicOnly.ImportParameters(privateSoftware.ExportParameters(false));
 for(int scenario=0;scenario<9;scenario++){
  PossessionOnce p=new PossessionOnce(c,ek,name,100,20);byte[][] blob=p.Make(publicOnly,seed);byte[] recovered=Credential.Open(privateSoftware,blob,name);
  long time=110;byte[] cc=(byte[])c.Clone(),ee=(byte[])ek.Clone(),nn=(byte[])name.Clone();
  switch(scenario){case 1:cc[0]^=1;break;case 2:ee[0]^=1;break;case 3:nn[2]^=1;break;case 4:recovered[0]^=1;break;case 5:time=121;break;case 6:time=99;break;case 7:p.Stop();break;case 8:time=120;break;}
  C(p.Verify(recovered,cc,ee,nn,time)==(scenario==0||scenario==8));C(!p.Verify(recovered,c,ek,name,110));p.Clear();Array.Clear(recovered,0,recovered.Length);
 }
 PossessionOnce a=new PossessionOnce(c,ek,name,100,20),b=new PossessionOnce(c,ek,name,100,20);byte[] aa=Credential.Open(privateSoftware,a.Make(publicOnly,seed),name),bb=Credential.Open(privateSoftware,b.Make(publicOnly,seed),name);C(!Credential.Eq(aa,bb));C(!a.Verify(bb,c,ek,name,110));C(!a.Verify(aa,c,ek,name,110));a.Clear();b.Clear();Array.Clear(aa,0,aa.Length);Array.Clear(bb,0,bb.Length);
 PossessionOnce concurrent=new PossessionOnce(c,ek,name,100,20);byte[] answer=Credential.Open(privateSoftware,concurrent.Make(publicOnly,seed),name);int winners=0;System.Threading.Thread[] threads=new System.Threading.Thread[16];for(int j=0;j<16;j++){threads[j]=new System.Threading.Thread(delegate(){if(concurrent.Verify(answer,c,ek,name,110))System.Threading.Interlocked.Increment(ref winners);});threads[j].Start();}foreach(System.Threading.Thread t in threads)t.Join();C(winners==1);concurrent.Clear();C(!concurrent.Verify(new byte[30],c,ek,name,110));Array.Clear(answer,0,answer.Length);
 }
 Console.WriteLine("PASS checks="+checks+" FRESH_CHALLENGE_AND_CONTEXT_SOFTWARE_ONLY; NO_TPM_NO_AUTHORITY");return 0;
 }
}
