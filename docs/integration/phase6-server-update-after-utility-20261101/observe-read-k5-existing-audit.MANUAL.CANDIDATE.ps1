# Candidate diagnostic only. Do not run before specific owner approval.
# Reads existing Security records only; no audit configuration or trace session.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
function Convert-K5AuditRecord([string]$Text) {
 if(!$Text -or $Text.Length-gt65536){throw 'EVENT_XML_SIZE_DENY'}
 $settings=[Xml.XmlReaderSettings]::new();$settings.DtdProcessing=[Xml.DtdProcessing]::Prohibit;$settings.XmlResolver=$null
 $inputText=[IO.StringReader]::new($Text);$reader=[Xml.XmlReader]::Create($inputText,$settings)
 $xml=[Xml.XmlDocument]::new();$xml.XmlResolver=$null
 try {$xml.Load($reader)} finally {$reader.Dispose();$inputText.Dispose()}
 $ns=[Xml.XmlNamespaceManager]::new($xml.NameTable);$ns.AddNamespace('e','http://schemas.microsoft.com/win/2004/08/events/event')
 $id=$xml.SelectSingleNode('/e:Event/e:System/e:EventID',$ns)
 $provider=$xml.SelectSingleNode('/e:Event/e:System/e:Provider',$ns)
 $time=$xml.SelectSingleNode('/e:Event/e:System/e:TimeCreated',$ns)
 $record=$xml.SelectSingleNode('/e:Event/e:System/e:EventRecordID',$ns)
 if(!$id-or!$provider-or!$time-or!$record){throw 'EVENT_SYSTEM_DENY'}
 if($id.InnerText-notin@('5058','5061')-or$provider.GetAttribute('Name')-cne'Microsoft-Windows-Security-Auditing'){throw 'EVENT_PROVIDER_ID_DENY'}
 $when=[DateTimeOffset]::Parse($time.GetAttribute('SystemTime'),[Globalization.CultureInfo]::InvariantCulture)
 if($when-lt[DateTimeOffset]::Parse('2026-11-03T21:12:54.438685Z')-or$when-gt[DateTimeOffset]::Parse('2026-11-03T21:20:59.438685Z')){throw 'EVENT_TIME_DENY'}
 $fields=@{}
 foreach($d in $xml.SelectNodes('/e:Event/e:EventData/e:Data',$ns)){$name=$d.GetAttribute('Name');if($fields.ContainsKey($name)){throw 'EVENT_DUPLICATE_FIELD_DENY'};$fields[$name]=$d.InnerText}
 foreach($name in @('KeyName','ProviderName','SubjectUserSid','Operation','ReturnCode','KeyType')){if(!$fields.ContainsKey($name)){throw 'EVENT_REQUIRED_FIELD_DENY'}}
 if($fields.KeyName-cne'PapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c'-or$fields.ProviderName-cne'Microsoft Software Key Storage Provider'){throw 'EVENT_FIXTURE_PROVIDER_DENY'}
 $role=if($fields.SubjectUserSid-ceq'S-1-5-21-785699550-1440458426-1682014700-1108'){'OPERATOR'}elseif($fields.SubjectUserSid-ceq'S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099'){'RUNTIME_SID_ONLY_NOT_ORIGINAL_TOKEN'}else{throw 'EVENT_SUBJECT_DENY'}
 if($record.InnerText-notmatch'^\d{1,20}$'-or$fields.Operation-notmatch'^(%%\d{1,8}|[A-Za-z][A-Za-z ]{0,63})$'-or$fields.ReturnCode-notmatch'^0x[0-9a-fA-F]{1,8}$'-or$fields.KeyType-notmatch'^(%%\d{1,8}|[A-Za-z][A-Za-z ]{0,31})$'){throw 'EVENT_SCALAR_DENY'}
 # Whitelist projection: do not print XML, key/file paths, accounts, blobs or arbitrary messages.
 return [pscustomobject]@{event_id=[int]$id.InnerText;record_id=$record.InnerText;utc=$when.ToUniversalTime().ToString('o');subject_role=$role;operation=$fields.Operation;return_code=$fields.ReturnCode.ToLowerInvariant();key_type=$fields.KeyType;key_file_path_present=($fields.ContainsKey('KeyFilePath')-and!!$fields.KeyFilePath)}
}
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash-ne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$request=Join-Path $PSScriptRoot 'ONE-READ-K5-EXISTING-AUDIT-REQUEST.CANDIDATE.md'
if((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash-ne$ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if($PSVersionTable.PSEdition-ne'Desktop'-or$PSVersionTable.PSVersion.Major-ne5-or[IntPtr]::Size-ne8-or[Environment]::MachineName-cne'TCORP-TOBIAS'-or[Console]::IsInputRedirected-or[Console]::IsOutputRedirected){throw 'HOST_OR_CONSOLE_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$impersonated){$impersonated.Dispose();throw 'IMPERSONATION_DENY'}
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {if($identity.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108'-or-not([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'OPERATOR_DENY'}}finally{$identity.Dispose()}
Write-Host 'ONE_EXISTING_SECURITY_AUDIT_READ; FIXED_KEY_FIXED_HISTORICAL_WINDOW; NO_TRACE_ENABLE_NO_AUDIT_CHANGE_NO_KSP_NO_START_NO_RETRY'
Write-Host 'Type READ_K5_EXISTING_5058_5061_ONCE only after specific owner approval:'
if([Console]::ReadLine()-cne'READ_K5_EXISTING_5058_5061_ONCE'){throw 'CANCELLED_NO_READ'}
$xpath="*[System[Provider[@Name='Microsoft-Windows-Security-Auditing'] and (EventID=5058 or EventID=5061) and TimeCreated[@SystemTime>='2026-11-03T21:12:54.438685Z' and @SystemTime<='2026-11-03T21:20:59.438685Z']] and EventData[Data[@Name='KeyName']='PapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c' and Data[@Name='ProviderName']='Microsoft Software Key Storage Provider' and (Data[@Name='SubjectUserSid']='S-1-5-21-785699550-1440458426-1682014700-1108' or Data[@Name='SubjectUserSid']='S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099')]]"
$eventReader=$null;$event=$null;$count=0;$first='NONE';$complete=$false;$closed=$true;$consumed=$true;$watch=[Diagnostics.Stopwatch]::StartNew()
try {
 $query=[Diagnostics.Eventing.Reader.EventLogQuery]::new('Security',[Diagnostics.Eventing.Reader.PathType]::LogName,$xpath)
 $query.ReverseDirection=$false;$query.TolerateQueryErrors=$false
 $eventReader=[Diagnostics.Eventing.Reader.EventLogReader]::new($query);$eventReader.BatchSize=1
 while($count-lt16){
  if($watch.Elapsed.TotalSeconds-ge30){throw 'OBSERVER_ELAPSED_LIMIT_UNKNOWN'}
  $event=$eventReader.ReadEvent([TimeSpan]::FromSeconds(5))
  if($null-eq$event){$complete=$true;break}
  try {$projection=Convert-K5AuditRecord $event.ToXml();Write-Host ('K5_EXISTING_AUDIT '+($projection|ConvertTo-Json -Compress));$count++}finally{$event.Dispose();$event=$null}
 }
 if(!$complete){$first='COUNT_LIMIT_REACHED_NOT_COMPLETE_NO_FALLBACK'}
} catch {$first='READ_OR_PROJECTION_FAILED_UNKNOWN';Write-Host ('K5_AUDIT_FAILURE_TYPE='+$_.Exception.GetType().FullName)}finally{
 try {if($null-ne$event){$event.Dispose()};if($null-ne$eventReader){$eventReader.Dispose()}}catch{$closed=$false}
 $watch.Stop()
 Write-Host ('K5_EXISTING_AUDIT_RESULT first='+$first+' consumed='+$consumed+' records='+$count+' query_end_observed='+$complete+' observer_close_known='+$closed+'; EVENT_ABSENCE_NOT_PROOF; TOKEN_CAUSE_AND_K5_PASS_NOT_INFERRED; HISTORICAL_RETENTION_OS_QUIESCENCE_UNKNOWN; NO_RETRY')
}
if($first-ne'NONE'-or!$closed){exit 2}
exit 0
