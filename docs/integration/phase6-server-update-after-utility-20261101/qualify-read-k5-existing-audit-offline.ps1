$ErrorActionPreference='Stop'
$f=Split-Path -Parent $PSCommandPath
$tok=$null;$err=$null;$ast=[System.Management.Automation.Language.Parser]::ParseFile((Join-Path $f 'observe-read-k5-existing-audit.MANUAL.CANDIDATE.ps1'),[ref]$tok,[ref]$err)
if($err.Count){throw ($err|Out-String)}
$fn=$ast.Find({param($x)$x-is[System.Management.Automation.Language.FunctionDefinitionAst]-and$x.Name-eq'Convert-K5AuditRecord'},$true)
. ([scriptblock]::Create($fn.Extent.Text))
$key='PapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c'
$x='<Event xmlns="http://schemas.microsoft.com/win/2004/08/events/event"><System><Provider Name="Microsoft-Windows-Security-Auditing"/><EventID>5058</EventID><TimeCreated SystemTime="2026-11-03T21:13:20Z"/><EventRecordID>321</EventRecordID></System><EventData><Data Name="KeyName">'+$key+'</Data><Data Name="ProviderName">Microsoft Software Key Storage Provider</Data><Data Name="SubjectUserSid">S-1-5-21-785699550-1440458426-1682014700-1108</Data><Data Name="Operation">%%2458</Data><Data Name="ReturnCode">0x80090016</Data><Data Name="KeyType">%%2499</Data><Data Name="KeyFilePath">SENSITIVE_PATH_MUST_NOT_PRINT</Data><Data Name="Other">PRIVATE_MUST_NOT_PRINT</Data></EventData></Event>'
$a=Convert-K5AuditRecord $x;$serialized=$a|ConvertTo-Json -Compress
if($a.event_id-ne5058-or$a.subject_role-cne'OPERATOR'-or!$a.key_file_path_present-or$serialized.Contains('MUST_NOT_PRINT')){throw 'PROJECTION_DENY'}
$y=$x.Replace('5058','5061').Replace('S-1-5-21-785699550-1440458426-1682014700-1108','S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099')
if((Convert-K5AuditRecord $y).subject_role-cne'RUNTIME_SID_ONLY_NOT_ORIGINAL_TOKEN'){throw 'RUNTIME_DENY'}
$bad=@($x.Replace('5058','5000'),$x.Replace('21:13:20','20:13:20'),$x.Replace($key,'OTHER'),$x.Replace('Microsoft Software Key Storage Provider','OTHER'),$x.Replace('S-1-5-21-785699550-1440458426-1682014700-1108','S-1-5-18'),$x.Replace('0x80090016','PRIVATE'),$x.Replace('</EventData>','<Data Name="ReturnCode">0x0</Data></EventData>'),$x.Replace('Name="ReturnCode"','Name="Absent"'),('<!DOCTYPE Event [<!ENTITY e SYSTEM "file:///should-never-open">]>'+$x),('x'*65537))
foreach($b in $bad){$denied=$false;try{$null=Convert-K5AuditRecord $b}catch{$denied=$true};if(!$denied){throw 'NEGATIVE_NOT_DENIED'}}
$q=[pscustomobject]@{state='OFFLINE_PASS_NOT_EXECUTED';ast='PASS';positive_projection_cases=2;negative_cases=10;sensitive_fields_omitted=$true;real_event_log_reads=0;trace_or_audit_changes=0;native_query_costs='UNKNOWN_NOT_ZERO';eventlog_xpath_compatibility='PENDING_ORIGINAL_MANUAL_QUERY_NOT_NATIVE_VALIDATED'}
$q|ConvertTo-Json|Set-Content -LiteralPath (Join-Path $f 'READ-K5-EXISTING-AUDIT-OFFLINE-QUALIFICATION.json') -Encoding UTF8
'AST_PASS PROJECTION_12_CASES_PASS NO_EVENTLOG_READ_NO_TRACE'
