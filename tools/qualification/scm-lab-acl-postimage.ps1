# Read-only comparison of this LAB plan. No OS security writes.
function Test-PaccLabAclPostimage([string]$Expected,[string]$Observed,[string]$ExpectedIdentity,[string]$ObservedIdentity) {
 try {
  if($ExpectedIdentity -cne '417614100:1966080:79426' -or $ObservedIdentity -cne $ExpectedIdentity){return $false}
  $a=[Security.AccessControl.RawSecurityDescriptor]::new($Expected)
  $b=[Security.AccessControl.RawSecurityDescriptor]::new($Observed)
  if(([int]$a.ControlFlags -band 0x1000) -eq 0 -or ([int]$b.ControlFlags -band 0x1000) -eq 0){return $false}
  if((([int]$a.ControlFlags -bxor [int]$b.ControlFlags) -band (-bnot 0x400)) -ne 0){return $false}
  if($null -eq $a.Owner -or $null -eq $b.Owner -or $a.Owner.Value -cne $b.Owner.Value -or $a.Owner.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108'){return $false}
  if($null -eq $a.Group -or $null -eq $b.Group -or $a.Group.Value -cne $b.Group.Value -or $a.ResourceManagerControl -ne $b.ResourceManagerControl){return $false}
  if($null -ne $a.SystemAcl -or $null -ne $b.SystemAcl){return $false} # Scope is OWNER/GROUP/DACL only.
  if($null -eq $a.DiscretionaryAcl -or $null -eq $b.DiscretionaryAcl -or $a.DiscretionaryAcl.Count -ne 7 -or $b.DiscretionaryAcl.Count -ne 7 -or $a.DiscretionaryAcl.Revision -ne $b.DiscretionaryAcl.Revision){return $false}
  for($i=0;$i -lt 7;$i++){
   # Preserve order, ACE type, SID, mask, inheritance/propagation flags and opaque bytes.
   $x=$a.DiscretionaryAcl[$i];$y=$b.DiscretionaryAcl[$i]
   $xb=New-Object byte[] $x.BinaryLength;$yb=New-Object byte[] $y.BinaryLength
   $x.GetBinaryForm($xb,0);$y.GetBinaryForm($yb,0)
   if([Convert]::ToBase64String($xb) -cne [Convert]::ToBase64String($yb)){return $false}
  }
  return $true
 }catch{return $false}
}
