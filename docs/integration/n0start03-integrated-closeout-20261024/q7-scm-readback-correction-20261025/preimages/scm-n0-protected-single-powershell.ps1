#requires -Version 5.1
[CmdletBinding()]
param(
 [ValidateSet('Review','Prepare','Cycle')][string]$Mode='Prepare',
 [ValidateSet('EMPTY_DENY','N0START03ReadOnly','N0START03Positive','Q4TokenDiagnostic')][string]$Profile='EMPTY_DENY',
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [ValidatePattern('^[0-9a-f]{64}$')][string]$ExactApprovalSha256,
 [uint32]$AuxiliaryPid=0,
 [uint64]$AuxiliaryCreation=0
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ExpectedPath=$Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1'
$Qualifier='C:\Projetos\PapinhoAccelerator\build\ed11-private-stdout-20261025\Selected\Release\papacc_scm_instrumented_lab.exe'
$QualifierHash='6f58cd11624360413d63480933a9ce01385525989381f5fd0b5af675061d5b5f'
$TransportNative=$Root+'\build\ed11-private-stdout-20261025\Selected\Release\papacc_n0_private_stdout.dll'
$TransportBinding=$Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll'
$TransportNativeHash='0d3f611010254082673a170e81118728b303d92f3ac74e7b86a21a2aee906abf'
$TransportBindingHash='513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4'
$ApprovalPath=$Root+'\docs\integration\n0start03-integrated-closeout-20261024\t0-t9-acceptance-and-integration\EXACT-TRANSPORT-READONLY-APPROVED.json'
$T10DecisionHash='1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5'
if($Profile-ceq'N0START03Positive'){
 $Qualifier=$Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_scm_instrumented_lab.exe'
 $QualifierHash='80060c591258f89fe9ca28bcc67a08520edd6a434debfd1c0ce7e926d3c12aef'
 $TransportNative=$Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_n0_private_stdout.dll'
 $TransportNativeHash='c351ecb559834d74ebe36dd735b6174b4ca6226526ae928b7a2cc5f44c4d792c'
 $ApprovalPath=$Root+'\docs\integration\n0start03-integrated-closeout-20261024\admin-handle-budget-resolution-20261025\EXACT-POSITIVE-ADMIN-BUDGET-APPROVED.json'
}
# Reviewed existing inputs. These pins grant no execution or Start authority.
$DossierHash=''
if($Profile-ceq'N0START03ReadOnly' -or $Profile-ceq'N0START03Positive'){$DossierHash='5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050'}
$EvidenceHash=''
if($Profile-ceq'N0START03ReadOnly' -or $Profile-ceq'N0START03Positive'){$EvidenceHash='15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7'}
$Attempt='0b1de869e11493b7cb08fb329578f6ce'
$Operation='N0START-20261024-03'
if($Profile-ceq'Q4TokenDiagnostic'){
 $Attempt='10584f02e11f92c92b43cddf0d9d316f'
 $Operation='N0TOKEN-20261025-01'
 $Qualifier=$Root+'\build\q7-source-pin-correction-20261025\Selected\Release\papacc_scm_instrumented_lab.exe'
 $QualifierHash='d36828987cd8dedc1ad2faeae65d41af536e2afc024be1d025e3c6833d95808a'
 $TransportNative=$Root+'\build\q4-final-binding-20261025\Selected\Release\papacc_n0_private_stdout.dll'
 $TransportNativeHash='e66735a59cf1fc3c63776e4adb97f67e1e95cdb7f1963790659b0a19637756b5'
 $DossierHash='c63209c7591f404eead7ca8e3e4b91b5b3c8db13e47ad0e637c6e2c3a36a46e6'
 $EvidenceHash='bb548b4f2a866d33445d39e2bb72e0093b60b04380cc4da6fd70c4855a0038fa'
 $ApprovalPath=$Root+'\docs\integration\n0start03-integrated-closeout-20261024\q7-source-image-pin-correction-20261025\EXACT-Q7-CORRECTION-APPROVED.json'
}
$Operator='S-1-5-21-785699550-1440458426-1682014700-1108'
$script:N0RetainedProcess=$null
$script:N0RetainedPins=$null
$pins=[Collections.Generic.List[IDisposable]]::new()
$locks=[Collections.Generic.List[IntPtr]]::new()
$script:N0RetainedLocks=$null
$process=$null;$reader=$null;$started=$false;$launchAttempted=$false;$returned=$false;$final=$false;$closeFailed=$false;$exitCode=2;$outputTruncated=$false;$affinity=$false
function Hash-Stream($f) {
 $sha=[Security.Cryptography.SHA256]::Create()
 try {$f.Position=0;$v=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant();$f.Position=0;return $v}finally{$sha.Dispose()}
}
function Pin([string]$path,[string]$hash) {
 if($hash -cnotmatch '^[0-9a-f]{64}$'){throw 'MISSING_QUALIFIED_PIN'}
 if([IO.Path]::GetFullPath($path) -cne $path){throw 'NONCANONICAL_PIN_PATH'}
 $walk=$path
 while($walk){if(([IO.File]::GetAttributes($walk) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PIN_PATH'};$walk=[IO.Path]::GetDirectoryName($walk)}
 $file=[IO.File]::Open($path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $pins.Add($file)
 if((Hash-Stream $file) -cne $hash){throw 'HASH_DIVERGENCE'}
}
# Reuse the original native read-only locking helper; no new authority.
function Lock-QualifierPath {
 if('PaccFileAcl' -as [type]){throw 'FRESH_OPERATOR_PROCESS_REQUIRED'}
 $path=$Root+'\tools\qualification\scm-file-acl-native.ps1'
 $file=[IO.File]::Open($path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$pins.Add($file)
 if((Hash-Stream $file)-cne'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75'){throw 'LOCK_HELPER_PIN_DENY'}
 $buffer=[IO.MemoryStream]::new();try{$file.CopyTo($buffer);$bytes=$buffer.ToArray()}finally{$buffer.Dispose()}
 & ([scriptblock]::Create([Text.UTF8Encoding]::new($false,$true).GetString($bytes).TrimStart([char]0xFEFF)))
 $chain=[Collections.Generic.Stack[string]]::new();$walk=[IO.Path]::GetDirectoryName($Qualifier)
 while($walk){$chain.Push($walk);$parent=[IO.Directory]::GetParent($walk);if($null-eq$parent){$walk=$null}else{$walk=$parent.FullName}}
 while($chain.Count){$walk=$chain.Pop();$h=[PaccFileAcl]::Open($walk,$false,$true);$locks.Add($h);if(([IO.File]::GetAttributes($walk)-band[IO.FileAttributes]::ReparsePoint)-ne0){throw 'QUALIFIER_ANCESTOR_REPARSE_DENY'}}
}
function Select-AuxiliaryCandidate([uint64]$ActorCreation,[int]$Session) {
 # Name/session/creation nominate candidates only. Native N2 remains mandatory.
 $found=@();$items=[Diagnostics.Process]::GetProcessesByName('conhost')
 try {
  foreach($item in $items){if($item.SessionId-eq$Session){$created=[uint64]$item.StartTime.ToUniversalTime().ToFileTimeUtc();if($created-gt$ActorCreation){$found+=@{Id=[uint32]$item.Id;Creation=$created}}}}
 }finally{foreach($item in $items){$item.Dispose()}}
 if($found.Count-ne1){throw ('AUXILIARY_CANDIDATE_COUNT_DENY count='+$found.Count)}
 return $found[0]
}
# Semantic type gate on the SAME pinned approval stream; no reopen or new authority.
function Assert-PositiveApproval($stream) {
 if($stream.Length-le0-or$stream.Length-gt16384){throw 'POSITIVE_APPROVAL_SIZE_DENY'}
 $bytes=[byte[]]::new([int]$stream.Length);$stream.Position=0
 try {if($stream.Read($bytes,0,$bytes.Length)-ne$bytes.Length){throw 'POSITIVE_APPROVAL_READ_DENY'};$text=[Text.UTF8Encoding]::new($false,$true).GetString($bytes)}finally{$stream.Position=0}
 $record=$text|ConvertFrom-Json
 foreach($name in @('status','operation','attempt','mode','request_sha256','T10_T14_sha256')){if($record.$name-isnot[string]){throw 'POSITIVE_APPROVAL_SCALAR_DENY'}}
 if($record.manual_only-isnot[bool]-or$record.automatic_execution-isnot[bool]){throw 'POSITIVE_APPROVAL_SCALAR_DENY'}
 foreach($name in @('launcher','qualifier','native_resource','framework_binding')){if($record.artifacts.$name.path-isnot[string]-or$record.artifacts.$name.sha256-isnot[string]){throw 'POSITIVE_APPROVAL_SCALAR_DENY'}}
 if($record.status-cne'EXACT_MANUAL_POSITIVE_OWNER_APPROVED'-or$record.operation-cne$Operation-or$record.attempt-cne$Attempt-or$record.mode-cne'Cycle'-or$record.manual_only-ne$true-or$record.automatic_execution-ne$false){throw 'POSITIVE_APPROVAL_TYPE_DENY'}
 if($record.request_sha256-cnotmatch'^[0-9a-f]{64}$'-or$record.T10_T14_sha256-cne$T10DecisionHash){throw 'POSITIVE_APPROVAL_POLICY_DENY'}
 if($record.artifacts.launcher.sha256-cne$ExpectedScriptSha256-or$record.artifacts.qualifier.sha256-cne$QualifierHash-or$record.artifacts.native_resource.sha256-cne$TransportNativeHash-or$record.artifacts.framework_binding.sha256-cne$TransportBindingHash){throw 'POSITIVE_APPROVAL_ARTIFACT_DENY'}
 if($Profile-ceq'Q4TokenDiagnostic'){
  if($record.Q0_Q9_sha256-isnot[string]-or$record.Q0_Q9_sha256-cne'97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca'){throw 'Q7_POLICY_BINDING_DENY'}
  if($record.Q4_annex_sha256-isnot[string]-or$record.Q4_annex_sha256-cne'590aa1c695adce55d6ba17e9ce508d747090c05c2d61cd44a7ced603cf6c4db4'){throw 'Q7_POLICY_BINDING_DENY'}
  if($record.Q2_record_sha256-isnot[string]-or$record.Q2_record_sha256-cne'5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1'){throw 'Q7_POLICY_BINDING_DENY'}
  if($record.N7_sha256-isnot[string]-or$record.N7_sha256-cne'1d6bca5386e3520034570d17cf6f526fdef4c19b52aee633113e1bd8e5b40fe7'){throw 'Q7_POLICY_BINDING_DENY'}
  if($record.evaluation_plan_sha256-isnot[string]-or$record.evaluation_plan_sha256-cne'd93cff9c596ad68529fb836f1dc45280af9686828efcf939ac48621a01b4e9e6'){throw 'Q7_POLICY_BINDING_DENY'}
  if($record.q7_activation_authorized-isnot[bool]-or$record.q7_activation_authorized-ne$true){throw 'Q7_EXACT_OWNER_AUTHORITY_REQUIRED'}
 }
 if($record.artifacts.launcher.path-cne$ExpectedPath-or$record.artifacts.qualifier.path-cne$Qualifier-or$record.artifacts.native_resource.path-cne$TransportNative-or$record.artifacts.framework_binding.path-cne$TransportBinding){throw 'POSITIVE_APPROVAL_PATH_DENY'}
}
try {
 if($PSCommandPath -cne $ExpectedPath -or [IntPtr]::Size -ne 8 -or $PSVersionTable.PSEdition -cne 'Desktop' -or $PSVersionTable.PSVersion.Major-ne5 -or $PSVersionTable.PSVersion.Minor-ne1){throw 'SCRIPT_OR_RUNTIME_DIVERGENCE'}
 Pin $ExpectedPath $ExpectedScriptSha256
 if($Profile-cne'N0START03ReadOnly'-and$Profile-cne'N0START03Positive'-and$Profile-cne'Q4TokenDiagnostic'){throw 'DEFAULT_PROFILE_EMPTY_DENY'}
 if((($Profile-ceq'N0START03Positive'-or$Profile-ceq'Q4TokenDiagnostic')-and$Mode-cne'Cycle'-and$Mode-cne'Review')-or($Profile-ceq'N0START03ReadOnly'-and$Mode-ceq'Cycle')){throw 'PROFILE_MODE_MISMATCH_DENY'}
 if($Mode-eq'Prepare' -and $ExactApprovalSha256-cnotmatch'^[0-9a-f]{64}$'){throw 'SEPARATE_EXACT_READONLY_APPROVAL_REQUIRED'}
 if($Mode-eq'Review'){
  if($Profile-ceq'N0START03Positive'){Pin ($Root+'\docs\adr\DECISION-n0start03-positive-private-transport-composition-20261025.md') $T10DecisionHash}
  Pin $Qualifier $QualifierHash
  Pin $TransportNative $TransportNativeHash
  Pin $TransportBinding $TransportBindingHash
  $global:LASTEXITCODE=0;Write-Host 'N0_LAUNCHER_REVIEW=PASS; STATIC_PINS_ONLY_NO_NATIVE_ENTRY_NO_PREFLIGHT';return
 }
 if($Mode-eq'Cycle' -and $ExactApprovalSha256-cnotmatch'^[0-9a-f]{64}$'){throw 'SEPARATE_EXACT_OPERATION_APPROVAL_PIN_REQUIRED'}
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {if($identity.User.Value -cne $Operator -or -not ([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'OPERATOR_OR_ELEVATION_DENY'}}finally{$identity.Dispose()}
 $imp=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null -ne $imp){$imp.Dispose();throw 'IMPERSONATION_DENY'}
 if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
 if($DossierHash -cnotmatch '^[0-9a-f]{64}$' -or $EvidenceHash -cnotmatch '^[0-9a-f]{64}$'){throw 'WINDOWS_PROVENANCE_UNKNOWN_DENY'}
 Pin 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' '9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3'
 Pin $ApprovalPath $ExactApprovalSha256
 if($Profile-ceq'N0START03Positive'-or$Profile-ceq'Q4TokenDiagnostic'){Assert-PositiveApproval $pins[$pins.Count-1];Pin ($Root+'\docs\adr\DECISION-n0start03-positive-private-transport-composition-20261025.md') $T10DecisionHash}
 if($Profile-ceq'Q4TokenDiagnostic'){
  Pin ($Root+'\docs\adr\DECISION-q4-specific-n7-and-independent-evaluation-20261025.md') '1d6bca5386e3520034570d17cf6f526fdef4c19b52aee633113e1bd8e5b40fe7'
  Pin ($Root+'\docs\adr\DECISION-q4-independent-service-token-observation-annex-20261025.md') '590aa1c695adce55d6ba17e9ce508d747090c05c2d61cd44a7ced603cf6c4db4'
  Pin ($Root+'\docs\adr\DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025.md') '97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca'
  Pin ($Root+'\docs\integration\n0start03-integrated-closeout-20261024\q2-protected-readback-native-result-20261025\Q2-EXACT-DISPOSITION-RECORDED.md') '5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1'
  Pin ($Root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025\INDEPENDENT-ED11-EVALUATION-PLAN.md') 'd93cff9c596ad68529fb836f1dc45280af9686828efcf939ac48621a01b4e9e6'
 }else{
 Pin ($Root+'\docs\adr\DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md') 'c17aa105580aa7680e8d97042abc9daa32640dcf1e69dcdce49c59b8462109ac'
 }
 Lock-QualifierPath
 Pin $Qualifier $QualifierHash
 Pin ($Root+'\docs\adr\DECISION-n0start03-noninherited-local-transport-20261025.md') 'c9ac85be53fc0df9c356b6a0856c520dcf25d212593e2ec78ee05ef949f7c330'
 Pin $TransportNative $TransportNativeHash
 Pin $TransportBinding $TransportBindingHash
 if('Pacc.N0PrivateStdout.Supervisor' -as [type]){throw 'FRESH_OPERATOR_PROCESS_REQUIRED'}
 $bindingAssembly=[Reflection.Assembly]::LoadFrom($TransportBinding)
 if(-not[String]::Equals($bindingAssembly.Location,$TransportBinding,[StringComparison]::OrdinalIgnoreCase)){throw 'BINDING_ASSEMBLY_LOCATION_DENY'}
 Pin ('C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-'+$Attempt+'.approved') $DossierHash
 Pin ('C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-'+$Attempt+'.evidence') $EvidenceHash
 Pin ($Root+'\docs\adr\DECISION-first-native-lab-diagnostic-boundary-20261024.md') 'bec9d0c275d5ee8bd68de4d42fe173a08f124a9900c119e7a08d23757d9d7941'
 Pin ($Root+'\docs\adr\DECISION-canonical-lab-console-topology-bd-20261023.md') 'eb93002cf3f6d863a060fb97b7380389aa66b6fbe68e66c1af4fb17b988d3290'
 Pin ($Root+'\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md') 'e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
 Pin ($Root+'\docs\adr\DECISION-n0-n9-security-descriptor-digest-profile-20261024.md') '8b042e2bbf6500aa90aa3407fcd3423f45d96cf6b16568ffa95b98f3d081c972'
 $self=[Diagnostics.Process]::GetCurrentProcess()
 try {$session=$self.SessionId;$birth=[uint64]$self.StartTime.ToUniversalTime().ToFileTimeUtc();if(-not [String]::Equals($self.MainModule.FileName,'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe',[StringComparison]::OrdinalIgnoreCase)){throw 'ACTOR_IMAGE_DENY'}}finally{$self.Dispose()}
 if(($AuxiliaryPid-eq0)-ne($AuxiliaryCreation-eq0)){throw 'INCOMPLETE_AUXILIARY_TUPLE_DENY'}
 if($AuxiliaryPid-eq0){$candidate=Select-AuxiliaryCandidate $birth $session;$AuxiliaryPid=$candidate.Id;$AuxiliaryCreation=$candidate.Creation}
 if($Mode-eq'Cycle'){[Console]::Out.WriteLine('OPERATION='+$Operation+'; MODE='+$Mode+'; N0-N9; ATTEMPT='+$Attempt+'; auxiliary PID='+$AuxiliaryPid+' creation='+$AuxiliaryCreation)}else{Write-Host ('OPERATION='+$Operation+'; MODE='+$Mode+'; N0-N9; ATTEMPT='+$Attempt+'; auxiliary PID='+$AuxiliaryPid+' creation='+$AuxiliaryCreation)}
 if($Mode-eq'Cycle'){[Console]::Out.WriteLine('ONE LAB: consume intent; exclusive image and sink; explicit rebind; ONE Start; at most ONE safe Stop; partial effects retained. No ACL remediation/delete/rollback/retry. Separate owner approval REQUIRED.')}else{Write-Host 'Read-only: no Start/Stop/rebind/ACL/intent. Missing facts DENY. No retry.'}
 $confirmation=if($Mode-eq'Cycle'){'START_N0_SINGLE_LAB '+$Operation+' '+$Attempt+' '+$AuxiliaryPid+' '+$AuxiliaryCreation}else{'PREPARE_N0_READ_ONLY '+$Attempt+' '+$AuxiliaryPid+' '+$AuxiliaryCreation}
 if($Mode-eq'Cycle'){[Console]::Out.WriteLine('Type exactly: '+$confirmation)}else{Write-Host ('Type exactly: '+$confirmation)}
 if((Read-Host) -cne $confirmation){throw 'CONFIRMATION_DIVERGENCE'}
 [Threading.Thread]::BeginThreadAffinity();$affinity=$true
 try {
  # Framework OS thread ID is a candidate only; native owner rechecks original
  # thread/process identity, lifetime, no impersonation. Never ManagedThreadId.
  $tid=[uint32][AppDomain]::GetCurrentThreadId()
  # Native original-process resource; bInheritHandles=FALSE, no helper process.
  $nativeMode=if($Mode-ceq'Cycle'){'--cycle-n0'}else{'--prepare-n0'}
  $arguments=$nativeMode+' '+$Attempt+' '+$PID+' '+$birth+' '+$tid+' '+$AuxiliaryPid+' '+$AuxiliaryCreation+' '+$ExactApprovalSha256
  $process=[Pacc.N0PrivateStdout.Supervisor]::new($TransportNative)
  $launchAttempted=$true
  $process.Open()
  $process.Start($Qualifier,$arguments);$started=$true
  $process.Authenticate();$reader=$process
  $chars=0
  while($null -ne ($line=$reader.ReadLine())) {
   if($reader.Truncated -and -not $outputTruncated){$outputTruncated=$true;if($Mode-eq'Cycle'){[Console]::Out.WriteLine('N0_OUTPUT_TRUNCATED; DRAIN_CONTINUES_UNTIL_FACTUAL_RETURN; NO_RETRY')}else{Write-Host 'N0_OUTPUT_TRUNCATED; DRAIN_CONTINUES_UNTIL_FACTUAL_RETURN; NO_RETRY'}}
   $chars+=$line.Length
   if($line.Length -gt 4096 -or $chars -gt 131072){if(-not $outputTruncated){$outputTruncated=$true;if($Mode-eq'Cycle'){[Console]::Out.WriteLine('N0_OUTPUT_TRUNCATED; DRAIN_CONTINUES_UNTIL_FACTUAL_RETURN; NO_RETRY')}else{Write-Host 'N0_OUTPUT_TRUNCATED; DRAIN_CONTINUES_UNTIL_FACTUAL_RETURN; NO_RETRY'}}}
   if(-not $outputTruncated -or $line-cmatch'^(IG_FINAL |IG_RETAINED |IG_DIAGNOSTIC |IG_DIAGNOSTIC_RECEIPT|IG_FIRST_RECORD)'){[Console]::Out.WriteLine($line)}
   if($Mode-ne'Cycle' -and $line -cmatch '^IG_FINAL stage=\S+ first=\d+ close=0 starts=0 stops=0 quiescent=0 intent_consumed=0 effect=0 diagnostic_created=0;'){$final=$true}
   if($Mode-eq'Cycle' -and $line-cmatch'^IG_FINAL stage=\S+ first=\d+ close=0 starts=([01]) stops=([01]) quiescent=([01]) intent_consumed=([01]) effect=([012]) diagnostic_created=([01]);'){if([int]$Matches[2]-le[int]$Matches[1] -and (($Matches[1]-eq'0'-and$Matches[3]-eq'0')-or($Matches[1]-eq'1'-and$Matches[3]-eq'1')) -and (($Matches[4]-eq'0'-and$Matches[1]-eq'0'-and$Matches[5]-eq'0'-and$Matches[6]-eq'0')-or($Matches[4]-eq'1'-and($Matches[1]-eq'0'-or($Matches[5]-eq'2'-and$Matches[6]-eq'1'))))){$final=$true}} 
  }
  $exitCode=[int]$process.WaitOriginal();if($outputTruncated){$exitCode=2};$returned=$true
  if(-not $final){throw 'FACTUAL_FINAL_RECEIPT_MISSING_RETAIN'}
 } finally {# Affinity remains held through checked native close below.
 }
} catch {$exitCode=2;Write-Host ('N0_LAUNCHER_ERROR='+$_.Exception.Message)} finally {
 if($launchAttempted -and -not $started -and $null-ne$process){
  try {if($process.CloseUnstartedIfKnown()){$launchAttempted=$false;Write-Host ('TRANSPORT_NO_CHILD_LOCAL_CLOSE=CONFIRMED; '+$process.Receipt())}}catch{$closeFailed=$true}
 }

 if(($launchAttempted -and -not $started) -or ($started -and (-not $returned -or -not $final))) {
  $script:N0RetainedProcess=$process;$script:N0RetainedPins=$pins;$script:N0RetainedLocks=$locks
  Write-Host 'RESOURCE_RETENTION=UNKNOWN; NO_CLOSE_NO_RETRY_NO_KILL; DO_NOT_CLOSE_OPERATOR'
  while($true){Start-Sleep -Seconds 1}
 } else {
  try {if($process){$process.CloseChecked();Write-Host ('TRANSPORT_LOCAL_CLOSE=CONFIRMED; '+$process.Receipt())};foreach($pin in $pins){$pin.Dispose()};for($i=$locks.Count-1;$i-ge0;$i--){if(-not [PaccFileAcl]::CloseHandle($locks[$i])){throw 'QUALIFIER_PATH_CLOSE_UNKNOWN'};$locks[$i]=[IntPtr]::Zero}}catch{$exitCode=2;$closeFailed=$true;$script:N0RetainedPins=$pins;$script:N0RetainedLocks=$locks;Write-Host 'CLOSE_UNKNOWN_NO_RETRY';while($true){Start-Sleep -Seconds 1}}
  if(-not $closeFailed -and $started -and $returned -and $final) {
   Write-Host 'Qualifier returned with factual final close receipt. F10 returns to this existing operator console; no retry.'
   do {$key=[Console]::ReadKey($true)}while($key.Key -ne [ConsoleKey]::F10)
  }
 }
}
if($affinity){[Threading.Thread]::EndThreadAffinity();$affinity=$false}
$global:LASTEXITCODE=$exitCode
if($Mode-eq'Cycle'){Write-Host ('N0_CYCLE_EXIT='+$exitCode+'; START_RESULT=SEE_NATIVE_RECEIPT; ED11=PARTIAL; PRODUCT_AUTHORITY=DENY')}else{Write-Host ('N0_READ_ONLY_EXIT='+$exitCode+'; START_EXECUTED=NO; ED11=PARTIAL; AUTHORITY=DENY')}
# Invocation is & script in THIS already elevated PowerShell, never a new powershell.exe.
