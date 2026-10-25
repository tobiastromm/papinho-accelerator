# Read-only file observations â€” not qualified provenance

2026-10-24, separate engineering metadata queries only; NOT a LAB preflight/admission. Get-Item/Get-FileHash/Get-Acl and existing fsutil, no ETW/service/ACL effects; later separate existing signature-cmdlet query below. Queries were sequential, not atomic; no retained original process handle or mapped-image claim. No approved dossier was generated.

- Path C:/Windows/System32/conhost.exe; length867840 bytes; SHA256 b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963.
- FileVersion10.0.19041.1645 (WinBuild.160101.0800), textual metadata only, not host-OS observation or authenticity.
- Owner NT SERVICE/TrustedInstaller. SDDL returned: `O:S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464G:S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464D:PAI(A;;0x1200a9;;;SY)(A;;0x1200a9;;;BA)(A;;0x1200a9;;;BU)(A;;FA;;;S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464)(A;;0x1200a9;;;AC)(A;;0x1200a9;;;S-1-15-2-2)`.
- fsutil file queryfileid returned `0x000000000000000000050000001847c4`; this is tool output, not a new original-handle acquisition receipt.
- fsutil hardlink list returned two names: System32/conhost.exe and Windows/WinSxS/amd64_microsoft-onecore-console-host-core_31bf3856ad364e35_10.0.19041.5198_none_e269c1c7d95bbfaf/conhost.exe. Version/name difference is preserved, not interpreted as corruption or proof.
- fsutil fsinfo volumeinfo C: returned Access denied. Native code not captured; no elevated retry. Different existing `cmd /d /c vol C:` subsequently returned18E4-4914, decimal417614100; this resolves textual volume observation, not native original-handle receipt.
- Signature tools absent from PATH; existing SDK x64 signtool located at Windows Kits/10/bin/10.0.19041.0/x64 and10.0.26100.0/x64. Neither SDK executable executed or installed; tool trust/version not separately qualified by discovery. Existing Get-AuthenticodeSignature WAS then used, as described below.

## Existing-tool signature result and remaining boundary

[Raw signature observation](external-signature-observation.json): Get-AuthenticodeSignature returned Valid/Catalog/IsOSBinary=True, Microsoft Windows signer thumbprint3B77DB29AC72AA6B5880ECB2ED5EC1EC6601D847. A FileShare.Read file handle was held during that query. This is positive external catalog evidence for the queried FILE, not mapped bytes/authenticated console/operational authority. Existing cmdlet trust policy used; cache-only/no-URL behavior NOT established. No network test/listener was executed; do not claim absence of all trust-provider retrieval.

Get-Acl.GetSecurityDescriptorBinaryForm returned248 bytes SHA256b643263616bb2e6cfd85743fbd21f78e4becc259896a0faa46a247477c80ace0. Exact equivalence to adapter GetSecurityInfo OWNER|GROUP|DACL raw descriptor representation has NOT been qualified; semantic ACL observation must not be silently substituted for byte digest.

Current docs evidence directory inherits Modify for nonapproved SIDs ending1002 and941509878, plus Authenticated Users0x1301bf; it cannot supply safe default dossier files under the existing descriptor predicate. Current tool-executor SID ends1003, distinct from approved LAB operator1108; no operator admission is claimed. No ACL changed or protected runtime file created.

First missing assurance: independently reviewed protected dossier binding these external facts to complete native file/SD evidence, then approved exact pins. Positive signature evidence is available; protected-context qualification/placement and native SD representation remain pending. No approved dossier generated, no generic trust return promoted to admission. Required full future N2 live process/token/client/metrics facts remain separate.
