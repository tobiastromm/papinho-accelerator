Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$p35Evidence='C:\Projetos\PapinhoAccelerator\docs\integration\p35-protected-dossier-authorized-readonly-20261024'
& 'C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-input-readonly.ps1' -Mode CompileOnly -ExpectedScriptSha256 6732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0
$p35Id=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 $p35N=0;$p35Elevation=0;$p35Type=0
 if(-not[PaccFileAcl]::GetTokenInformation($p35Id.Token,20,[ref]$p35Elevation,4,[ref]$p35N)){throw 'TOKEN_ELEVATION_QUERY_FAILED'}
 if(-not[PaccFileAcl]::GetTokenInformation($p35Id.Token,18,[ref]$p35Type,4,[ref]$p35N)){throw 'TOKEN_ELEVATION_TYPE_QUERY_FAILED'}
 $p35Thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$p35Impersonating=$null-ne$p35Thread;if($p35Thread){$p35Thread.Dispose()}
 [pscustomobject]@{ObservationUtc=[DateTime]::UtcNow.ToString('o');HistoricalToken='NOT_CAPTURED_CANNOT_RETROACTIVELY_IDENTIFY';PSVersion=$PSVersionTable.PSVersion.ToString();UserSid=$p35Id.User.Value;AdministratorEnabled=([Security.Principal.WindowsPrincipal]::new($p35Id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);TokenIsElevated=$p35Elevation;TokenElevationType=$p35Type;ThreadImpersonationObserved=$p35Impersonating;IdentityImpersonationLevel=$p35Id.ImpersonationLevel.ToString();ProtectedTargetOpened=$false}|ConvertTo-Json|Set-Content -LiteralPath ($p35Evidence+'\current-external-token.json') -Encoding UTF8
}finally{$p35Id.Dispose()}
Get-Content -LiteralPath ($p35Evidence+'\current-external-token.json')
