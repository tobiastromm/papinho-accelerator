param([string]$ExpectedScriptSha256)
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
$scriptPath=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025\q4-input-placement.CANDIDATE.ps1'
$fakeSource=@"
public sealed class PlacementWorkflowPin : System.IDisposable {public bool Fail;public int Calls;public void Dispose(){Calls++;if(Fail)throw new System.Exception("SYNTHETIC_PIN_CLOSE_UNKNOWN");}}
public sealed class PlacementWorkflowFake : IPaccN2PlacementIO {
 public static string Scenario;public bool Retained;public bool EffectsCreated;public string[] Evidence=new string[]{"FIXTURE_ONLY"};int creates;bool used;
 public void Prepare(){if(Scenario=="prepare"||Scenario=="parent")throw new Exception("PREPARE_DENY");}
 public void RevalidateBeforeEffects(){if(Scenario=="revalidate")throw new Exception("REVALIDATION_DENY");}
 public bool BeginOnce(){if(used)return false;used=true;return true;}
 public bool Stop(){return Scenario=="stop";}
 public void Check(){}
 public void Create(int i){if((i==0&&Scenario=="first")||(i==1&&Scenario=="second"))throw new Exception("CREATE_DENY");creates++;EffectsCreated=true;}
 public void Verify(int i){}public void Write(int i){}public void Flush(int i){}public void Readback(int i){}
 public bool Close(){if(Scenario=="close"){Retained=true;return false;}return true;}
}
"@
$compile=[IO.File]::ReadAllText($scriptPath).Replace('$parameters=[CodeDom.Compiler.CompilerParameters]::new()','$code+="`n"+$fakeSource; $parameters=[CodeDom.Compiler.CompilerParameters]::new()')
. ([scriptblock]::Create($compile)) -Mode ModelTest -ExpectedScriptSha256 $ExpectedScriptSha256
$cases=@()
foreach($s in @('prepare','parent','confirm','revalidate','stop','first','second','receipt','close','authority-close','approval-close','success')){
 [PlacementWorkflowFake]::Scenario=$s
 $script:Q4N7Stream=$null;$script:Q4ApprovalStream=$null
 if($s-eq'authority-close'-or$s-eq'approval-close'){$script:Q4N7Stream=[PlacementWorkflowPin]::new();$script:Q4ApprovalStream=[PlacementWorkflowPin]::new();$savedN7=$script:Q4N7Stream;$savedApproval=$script:Q4ApprovalStream;if($s-eq'authority-close'){$savedN7.Fail=$true}else{$savedApproval.Fail=$true}}
 $dir=Join-Path $pkg ('workflow-fixture-r2-'+$s);if([IO.Directory]::Exists($dir)){throw 'FIXTURE_REPLAY_NO_OVERWRITE'};[void][IO.Directory]::CreateDirectory($dir)
 $receipt=Join-Path $dir 'receipt.txt'
 if($s-eq'receipt'){[IO.File]::WriteAllText($receipt,'COLLISION_PRESERVED')}
 $tail=[IO.File]::ReadAllText($scriptPath);$tail=$tail.Substring($tail.IndexOf('$receipt=$pkg+'))
 $tail=[regex]::Replace($tail,'(?m)^\$receipt=.*$', '$receipt=$script:fixtureReceipt')
 $tail=$tail.Replace('[PaccN2PlacementNative]::new()','[PlacementWorkflowFake]::new()').Replace('while($true){[Threading.Thread]::Sleep(1000)}',"Write-Host 'FIXTURE_RETAINED_NO_LOOP'")
 $script:fixtureReceipt=$receipt
 function Check-OperatorBoot {}
 function Read-Host {param($Prompt)if([PlacementWorkflowFake]::Scenario-eq'confirm'){'WRONG'}else{'PLACE_N2_INPUTS Q4P-20261025-01 10584f02e11f92c92b43cddf0d9d316f'}}
 function Write-Host {param($Object)}
 . ([scriptblock]::Create($tail))
 $exists=[IO.File]::Exists($receipt)
 $before=@('prepare','parent','confirm','revalidate','stop','first') -contains $s
 if($before -and ($exists -or $owner.EffectsCreated)){throw ('EARLY_EFFECT '+$s)}
 if($s-eq'second' -and (-not $exists -or -not $owner.EffectsCreated -or $first-eq'NONE')){throw 'PARTIAL_NOT_RECORDED'}
 if($s-eq'receipt' -and ([IO.File]::ReadAllText($receipt)-cne'COLLISION_PRESERVED' -or $first-notlike'ADMIN_RECEIPT_FAILED*')){throw 'RECEIPT_COLLISION_OVERWRITE'}
 if($s-eq'close' -and (-not $owner.Retained -or $global:LASTEXITCODE-eq0)){throw 'CLOSE_UNKNOWN_PASS'}
 if($s-eq'success' -and ($global:LASTEXITCODE-ne0 -or -not $exists)){throw 'SUCCESS_FIXTURE_FAIL'}
 if($s-eq'authority-close'-and(-not$receiptRetained-or$first-cne'AUTHORITY_PIN_CLOSE_UNKNOWN'-or$savedN7.Calls-ne1-or$savedApproval.Calls-ne0)){throw 'N7_PIN_CLOSE_RETENTION_FAIL'}
 if($s-eq'approval-close'-and(-not$receiptRetained-or$first-cne'AUTHORITY_PIN_CLOSE_UNKNOWN'-or$savedN7.Calls-ne1-or$savedApproval.Calls-ne1)){throw 'APPROVAL_PIN_CLOSE_RETENTION_FAIL'}
 $cases+=[pscustomobject]@{scenario=$s;inputs=$owner.EffectsCreated;receipt=$exists;first=$first;retained=$owner.Retained;exit=$global:LASTEXITCODE;fixture_path=$dir}
}
$statuses=@(0xc0000034L,0L,0xc0000022L,0xc0000043L,0xc0000279L,0xc000003aL,0x103L,0x40000000L)
foreach($status in $statuses){
 $actual=[PaccN2PlacementNative]::AbsenceDecision([uint32]$status,0,$false)
 $expected=if($status-eq0xc0000034L){'ABSENT'}elseif($status-eq0x103L){'PENDING_UNKNOWN'}else{'DENY'}
 if($actual-cne$expected){throw ('ABSENCE_STATUS_FAIL '+$status)}
}
if([PaccN2PlacementNative]::AbsenceDecision(0,0,$true)-cne'COLLISION'){throw 'COLLISION'}
if([PaccN2PlacementNative]::AbsenceDecision(0xc0000034L,0x103,$false)-cne'PENDING_UNKNOWN'){throw 'PENDING_IOSB'}
$cases|ConvertTo-Json -Depth 5
