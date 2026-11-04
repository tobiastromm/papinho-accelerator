using System;
using System.IO;
using System.Text;
// ISOLATED synthetic claim experiment. No protected reader or authority.
class ClaimFixture {
 static int Main(string[] a) {
  if(a.Length!=2)return 90;
  if(a[1]=="before")return 20;
  FileStream f=null;
  try {
   f=new FileStream(a[0],FileMode.CreateNew,FileAccess.ReadWrite,FileShare.None,4096,FileOptions.WriteThrough);
   if(a[1]=="after_create")Environment.Exit(21);
   byte[] b=Encoding.ASCII.GetBytes("SYNTHETIC_ONLY|historic=10584f02e11f92c92b43cddf0d9d316f|approval=fixture-not-authority");
   f.Write(b,0,b.Length);
   if(a[1]=="persist")throw new IOException("SIMULATED_PERSISTENCE_UNKNOWN");
   f.Flush(true);
   if(a[1]=="after_flush")Environment.Exit(22);
   if(a[1]=="close_unknown") {Console.WriteLine("UNKNOWN_RETAIN_NO_READ");Environment.Exit(23);}
   f.Dispose();f=null;
   Console.WriteLine("CLAIM_FLUSHED_AND_CLOSED_SYNTHETIC_ONLY");return 0;
  } catch(IOException) {return 24;}
  finally {if(f!=null)f.Dispose();}
 }
}
