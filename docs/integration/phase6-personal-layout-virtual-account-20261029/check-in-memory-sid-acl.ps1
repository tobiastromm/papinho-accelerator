$ErrorActionPreference = 'Stop'
# Synthetic, unresolved SID bytes only: never a selected runtime or authority.
# No token, key, filesystem ACL or SCM operation is performed.
$runtime = [Security.Principal.SecurityIdentifier]::new('S-1-5-80-111-222-333-444-555')
$admin = [Security.Principal.SecurityIdentifier]::new('S-1-5-21-111-222-333-1001')
$sd = [Security.AccessControl.RawSecurityDescriptor]::new("D:P(D;;GA;;;$runtime)(A;;GA;;;$admin)(A;;GA;;;SY)")
$checks = [ordered]@{
    distinct_principals = (-not $runtime.Equals($admin))
    protected_dacl = (($sd.ControlFlags -band [Security.AccessControl.ControlFlags]::DiscretionaryAclProtected) -ne 0)
    runtime_deny_first = ($sd.DiscretionaryAcl[0].SecurityIdentifier.Equals($runtime) -and $sd.DiscretionaryAcl[0].AceQualifier -eq [Security.AccessControl.AceQualifier]::AccessDenied -and $sd.DiscretionaryAcl[0].AccessMask -eq 268435456)
    administrator_allow = ($sd.DiscretionaryAcl[1].SecurityIdentifier.Equals($admin) -and $sd.DiscretionaryAcl[1].AceQualifier -eq [Security.AccessControl.AceQualifier]::AccessAllowed)
    only_three_aces = ($sd.DiscretionaryAcl.Count -eq 3)
}
if ($checks.Values -contains $false) { throw 'IN_MEMORY_DESCRIPTOR_CHECK_FAILED' }
[pscustomobject]@{
    scope='IN_MEMORY_SYNTHETIC_SID_DESCRIPTOR_STRUCTURE_ONLY'; checks=$checks
    effective_access='NOT_TESTED'; virtual_logon='NOT_OBSERVED'
    CNG_calls=0; key_creation=0; SCM_calls=0; ACL_mutations=0
    authority='DENY'
} | ConvertTo-Json -Depth 4
