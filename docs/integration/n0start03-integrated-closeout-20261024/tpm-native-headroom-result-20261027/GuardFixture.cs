using System;using System.Runtime.InteropServices;
class GuardFixture {
 static bool observerQueryAttempted,observerQuerySucceeded,ok;static uint observerOwnCount,value;static int observerQueryError,apiError,firstError;static string first="NONE";static int calls,checks;
 static bool GetProcessHandleCount(IntPtr h,out uint count){calls++;count=value;SetLastError((uint)apiError);return ok;}
 [DllImport("kernel32.dll",SetLastError=true)]static extern void SetLastError(uint code);
 static void Fail(string x){if(first=="NONE"){first=x;firstError=observerQueryError;}throw new InvalidOperationException(first);}
 static void Guard(){
  observerQueryAttempted=true;uint own;bool queried=GetProcessHandleCount(new IntPtr(-1),out own);
  observerQueryError=queried?0:Marshal.GetLastWin32Error();observerQuerySucceeded=queried;observerOwnCount=own;
  if(!queried)Fail("NATIVE_OBSERVER_QUERY_FAILED");
  if(own>124)Fail("NATIVE_OBSERVER_HEADROOM");
}
 static void Check(bool v){checks++;if(!v)throw new Exception("assert"+checks);}
 static int Main(){
 for(uint n=0;n<=132;n++){
 ok=true;value=n;apiError=5;first="NONE";calls=0;observerQueryAttempted=false;bool passed=true;
 try{Guard();}catch(InvalidOperationException){passed=false;}
 Check(calls==1&&observerQueryAttempted&&observerQuerySucceeded&&observerOwnCount==n&&observerQueryError==0);
 Check(passed==(n<=124));Check(first==(n<=124?"NONE":"NATIVE_OBSERVER_HEADROOM"));
 }
 foreach(int e in new int[]{0,5,6,87,1460}){
 ok=false;value=999;apiError=e;first="NONE";calls=0;
 try{Guard();throw new Exception("unexpected pass");}catch(InvalidOperationException){}
 Check(!observerQuerySucceeded&&calls==1&&observerQueryError==e&&first=="NATIVE_OBSERVER_QUERY_FAILED"&&firstError==e);
 }
 ok=true;value=125;first="NONE";try{Guard();}catch(InvalidOperationException){};ok=false;apiError=6;
 try{Guard();}catch(InvalidOperationException){};Check(first=="NATIVE_OBSERVER_HEADROOM"&&firstError==0);
 Console.WriteLine("PASS "+checks+" source-extracted guard assertions; no TPM APIs; limits124/128 unchanged; first preserved");return 0;
 }
}