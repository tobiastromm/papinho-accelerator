$ErrorActionPreference='Stop'
Add-Type -ReferencedAssemblies ([PSObject].Assembly.Location),'System.dll','System.Core.dll' -TypeDefinition @"
using System;using System.Text;using System.Security;using System.Globalization;using System.Collections.Generic;using System.Collections.ObjectModel;using System.Management.Automation;using System.Management.Automation.Host;
public sealed class ForwardingHostUI:PSHostUserInterface {
 public StringBuilder Seen=new StringBuilder(); public override PSHostRawUserInterface RawUI{get{return null;}}
 public override string ReadLine(){throw new NotSupportedException();}public override SecureString ReadLineAsSecureString(){throw new NotSupportedException();}
 public override void Write(string s){Seen.Append(s);}public override void Write(ConsoleColor f,ConsoleColor b,string s){Seen.Append(s);}public override void WriteLine(string s){Seen.AppendLine(s);}
 public override void WriteErrorLine(string s){Seen.AppendLine(s);}public override void WriteDebugLine(string s){}public override void WriteVerboseLine(string s){}public override void WriteWarningLine(string s){}public override void WriteProgress(long id,ProgressRecord r){}
 public override Dictionary<string,PSObject> Prompt(string c,string m,Collection<FieldDescription> d){throw new NotSupportedException();}
 public override PSCredential PromptForCredential(string c,string m,string u,string t){throw new NotSupportedException();}
 public override PSCredential PromptForCredential(string c,string m,string u,string t,PSCredentialTypes ty,PSCredentialUIOptions o){throw new NotSupportedException();}
 public override int PromptForChoice(string c,string m,Collection<ChoiceDescription> d,int i){throw new NotSupportedException();}
}
public sealed class ForwardingHost:PSHost {
 public ForwardingHostUI View=new ForwardingHostUI();private Guid id=Guid.NewGuid();
 public override Guid InstanceId{get{return id;}}public override string Name{get{return "ISOLATED_TEST_HOST";}}public override Version Version{get{return new Version(1,0);}}
 public override CultureInfo CurrentCulture{get{return CultureInfo.InvariantCulture;}}public override CultureInfo CurrentUICulture{get{return CultureInfo.InvariantCulture;}}
 public override PSHostUserInterface UI{get{return View;}}public override void SetShouldExit(int code){}public override void EnterNestedPrompt(){throw new NotSupportedException();}public override void ExitNestedPrompt(){throw new NotSupportedException();}public override void NotifyBeginApplication(){}public override void NotifyEndApplication(){}
}
"@
$hostFixture=[ForwardingHost]::new();$runspace=[Management.Automation.Runspaces.RunspaceFactory]::CreateRunspace($hostFixture);$runspace.Open();$ps=[Management.Automation.PowerShell]::Create();$ps.Runspace=$runspace
$saved=[Console]::Out;$sink=[IO.StringWriter]::new()
try{
 [Console]::SetOut($sink)
 $null=$ps.AddScript("Write-Host 'CUSTOM_HOST_ROUTE'; [Console]::Out.WriteLine('DIRECT_STDOUT_ROUTE')")
 $null=$ps.Invoke();if($ps.HadErrors){throw ($ps.Streams.Error|Out-String)}
 $r=@{host_ui=$hostFixture.View.Seen.ToString();direct_stdout=$sink.ToString();information_count=$ps.Streams.Information.Count;information_text=($ps.Streams.Information|Out-String)}
}finally{[Console]::SetOut($saved);$ps.Dispose();$runspace.Dispose();$sink.Dispose()}
if(-not$r.host_ui.Contains('CUSTOM_HOST_ROUTE')-or$r.host_ui.Contains('DIRECT_STDOUT_ROUTE')-or-not$r.direct_stdout.Contains('DIRECT_STDOUT_ROUTE')-or$r.information_count-ne1){throw 'HOST_ROUTES'}
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'custom-host-results.json'),($r|ConvertTo-Json),[Text.UTF8Encoding]::new($false))
[Console]::Out.WriteLine('CUSTOM_HOST_ROUTE_DIFFERENCE_QUALIFIED; SYNTHETIC_NO_OPERATIONAL_ENTRY')
