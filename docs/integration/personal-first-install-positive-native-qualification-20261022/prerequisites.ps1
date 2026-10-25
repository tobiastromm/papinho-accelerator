Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public static class PaccReadOnlyPrerequisites {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("kernel32.dll")] public static extern uint WTSGetActiveConsoleSessionId();
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
}
"@
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 $principal=[Security.Principal.WindowsPrincipal]::new($identity)
 $elev=0;$kind=0;$ret=0;$inMode=0;$outMode=0
 $elevOk=[PaccReadOnlyPrerequisites]::GetTokenInformation($identity.Token,20,[ref]$elev,4,[ref]$ret)
 $typeOk=[PaccReadOnlyPrerequisites]::GetTokenInformation($identity.Token,18,[ref]$kind,4,[ref]$ret)
 $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 $hasThread=$null -ne $thread
 if($hasThread){$thread.Dispose()}
 $session=[Diagnostics.Process]::GetCurrentProcess().SessionId
 $active=[PaccReadOnlyPrerequisites]::WTSGetActiveConsoleSessionId()
 $admin=$principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
 $interactive=$principal.IsInRole([Security.Principal.SecurityIdentifier]::new('S-1-5-4'))
 $service=$principal.IsInRole([Security.Principal.SecurityIdentifier]::new('S-1-5-6'))
 $cin=[PaccReadOnlyPrerequisites]::GetConsoleMode([PaccReadOnlyPrerequisites]::GetStdHandle(-10),[ref]$inMode)
 $cout=[PaccReadOnlyPrerequisites]::GetConsoleMode([PaccReadOnlyPrerequisites]::GetStdHandle(-11),[ref]$outMode)
 [ordered]@{query='read-only current process prerequisites';administrator_enabled=$admin;interactive_membership=$interactive;service_membership=$service;impersonation_present=$hasThread;elevation_query_ok=$elevOk;elevated=$elev;type_query_ok=$typeOk;elevation_type=$kind;session=$session;active_console_session=$active;stdin_console=$cin;stdout_console=$cout;eligible=($admin -and $interactive -and !$service -and !$hasThread -and $elevOk -and $elev -eq 1 -and $typeOk -and $kind -ne 3 -and $session -ne 0 -and $session -eq $active);ceremony_executed=$false} | ConvertTo-Json
} finally {$identity.Dispose()}
