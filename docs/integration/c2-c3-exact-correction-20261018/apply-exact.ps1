$ErrorActionPreference='Stop'
$workspace='C:\Projetos\PapinhoAccelerator'
$evidence=Join-Path $workspace 'docs\integration\c2-c3-exact-correction-20261018'
$m=Get-Content (Join-Path $evidence 'exact-patch.json') -Raw|ConvertFrom-Json
if((& git -C $workspace rev-parse HEAD) -cne $m.head){throw 'HEAD changed'}
$pending=@()
foreach($f in $m.files){
 $p=Join-Path $workspace $f.path
 if((Get-FileHash -LiteralPath $p).Hash -ine $f.before){throw 'Preimage SHA mismatch'}
 $t=[IO.File]::ReadAllText($p)
 foreach($h in @($m.hunks|Where-Object {$_.path -eq $f.path}|Sort-Object {$_.offset} -Descending)){
  if($t.Substring($h.offset,$h.old.Length) -cne $h.old){throw 'Exact hunk mismatch'}
  $t=$t.Substring(0,$h.offset)+$h.new+$t.Substring($h.offset+$h.old.Length)
 }
 $bytes=[Text.UTF8Encoding]::new($false).GetBytes($t)
 $hash=[Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($bytes))
 if($hash -ine $f.after){throw 'Proposed SHA mismatch'}
 $pending+=@{target=$p;bytes=$bytes;hash=$hash}
}
foreach($p in $pending){
 [IO.File]::WriteAllBytes($p.target,$p.bytes)
 if((Get-FileHash -LiteralPath $p.target).Hash -ine $p.hash){throw 'Written SHA mismatch'}
}
'EXACT_PATCH_APPLIED=8_HUNKS_3_FILES'
