# PROPOSED — minimal official WPA availability and offline procedure

This is a tooling procedure, not ADR, accepted contract amendment or permission to install. User task §3 requires preparation for approval if WPA is unavailable. No download/installer/package execution has occurred.

## Exact candidate and compatibility

Candidate: Windows ADK for Windows10,version2004, REPUBLISHED MAY2025, selecting ONLY Windows Performance Toolkit. Microsoft's [ADK downloads table](https://learn.microsoft.com/en-us/windows-hardware/get-started/adk-install#other-adk-downloads) explicitly lists compatibility with Windows10 version2004 and later, covering host19045. This is a compatibility-matched candidate, not a claim it is the newest ADK or that older package security guidance is irrelevant. Use the republished official link; do not source an old cached installer or mirror. Microsoft recommends latest serviced ADKs; any selection of a different package/build requires recording its Windows10/WPA requirements before use. The current28000.1 entry is Arm64/Windows11-specific and is not this AMD64 host target.

[WPT requirements](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/) list WPA with Windows8+ and .NET Framework4.5+. Check the selected package's actual prerequisites and installed runtime before install; do not silently enable optional Windows features/reboot. If setup demands additional features/system changes, STOP for scoped review.

Minimal FEATURE is Windows Performance Toolkit, not the entire ADK; includes WPA and accompanying WPT tools. Do not select Deployment Tools, USMT, Assessment Toolkit, WinPE or unrelated features. A WPA already available on a separate approved analysis workstation would avoid installing on the LAB host entirely. Do not copy unrelated host data or upload ETL to a third party.

## Proposed approval scope

Owner approves obtaining the specified official Microsoft republished ADK, verifying downloaded bootstrapper/package signature and hash, and manual interactive installation of ONLY WPT for offline analysis. No silent /quiet install, auto-reboot or traces. Record installer exact version, verified publisher/certificate/hash, selected feature and destination BEFORE effects. Hash is PENDING_DOWNLOAD; no fabricated pin. Publisher expected Microsoft, but actual certificate chain must be verified; invalid/UNKNOWN => STOP.

Installation writes tool files, installer registration and possibly associations/runtime dependencies. It is an environment change, not a no-effect read-only query. Do not claim old environment freshness remains valid afterward. This approval must not authorize SCM/ACL remediation, changing the LAB service, quotas, boot, capture or preflight. If installer proposes effects outside the approved tooling scope, stop; do not promise an uninspected installer has no system effects.

## Manual installation after approval only

1. Download from the Microsoft ADK page's Windows10 version2004 REPUBLISHED MAY2025 link into a new external tools directory. Preserve original download URL, timestamp, size and SHA256.
2. Check Authenticode signature/status, file/product version and actual signer for bootstrapper and installed executable. If there is an existing WPT of another version discovered by setup, STOP rather than replace/uninstall automatically.
3. Run setup interactively elevated. Select ONLY Windows Performance Toolkit; inspect dependencies and destination. Do not install WinPE or accept reboot/other components. Follow [Microsoft selective-feature installation guidance](https://learn.microsoft.com/en-us/windows-hardware/get-started/adk-offline-install).
4. Locate installed wpa.exe (typically C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\wpa.exe); verify exact file signature/hash/version, not PATH alone. WPAExporter availability and supported syntax must be checked from that installed release before any automated export. No assumed profile/XML schema or unverified export CLI.
5. Record install outcome, environment change, versions and artifact pins. Nothing re-arms a runtime attempt or changes B4.

## Offline analysis of the same ETL

Verify original SHA256c5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679 before/after. Open existing file with WPA; no recorder button, profile start, boot recording or service action. Store only analysis profiles/exports in a new external analysis subdirectory, never overwrite ETL. Symbols optional: begin without network symbol retrieval. Additional download/symbol configuration requires separate scope if needed for this Light trace.

In available process lifetime/handle tables, select qualifier PID444 creation134372785867046628 / pinned image / --prepare and matching parent12848. Check actual UI/table names and field availability for this release. Resolve Object opcode34/version3 using decoded source/destination table ownership and handle values. If UI/export does not decode it, record that exact tool/version limitation; do not write a speculative binary parser.

Export decoded owner/type/time rows, process start/end and trace diagnostics. Retain header executor separately from owner. Account create,close,duplicate/close-source/inherited baseline without double-counting or value-reuse errors. Establish which generations/initial tables are observable. Align with sample141 only as closely as recorded timestamps permit; report range if exact sample boundary is absent. Do not declare sums of lifetime events as concurrent handles.

Deliver owner/type census and timeline with gaps, duplicate ambiguity, stacks/rundown/initial state limits and loss reports. Determine whether any non-security temporary refs demonstrably outlive need before proposing code changes. Guard128 and credits NONE_GRANTED stay unchanged. A successful ETL read is not ED11 PASS, operational approval or cleanup proof.

INSTALLATION_APPROVAL=PENDING_OWNER
INSTALLATION_EXECUTED=NO
WPA_BINARY_SIGNATURE_VERSION=NOT_AVAILABLE_YET
TRACE_RECAPTURE=NO
PREFLIGHT_RETRY=NO
START_SERVICE=NOT_RUN

## Owner approval and verified download

Owner answered sim to obtaining official installer, signature/hash verification and MANUAL installation of ONLY Windows Performance Toolkit for existing ETL offline analysis. This is approval of tooling procedure only; no ADR/B4/quotas/service/trace/preflight authority. Installation remains not executed by agent.

Official Microsoft download page resolved Windows ADK for Windows10 version2004 link https://go.microsoft.com/fwlink/?linkid=2120254 to https://download.microsoft.com/download/3b40cb81-ff9c-4322-aacd-c78d01b2c2ed/adk/adksetup.exe . Download2026-10-24T01:33:21.412504Z into exclusive C:\Projetos\_tmp\PapinhoAccelerator\wpa-official-installer-20261023\adksetup.exe. Provenance JSON beside file. Size1970056bytes; FileVersion/ProductVersion10.1.19041.5856; Company Microsoft Corporation. SHA25679ffc10e1a7e2e699083467db601d9747f71a419981b458063de1a86fd8c47dd. Authenticode Status0/assinatura verificada, signer CN=Microsoft Corporation, O=Microsoft Corporation, L=Redmond, S=Washington, C=US; thumbprint0BD8C56733FDCC06F8CB919FF5A200E39B1ACF71. Signature establishes downloaded bootstrapper provenance, not signatures/hashes of future downloaded payloads or installed WPA.

Native .NET Full Release533325 observed, above .NET4.5 prerequisite. No optional feature was enabled. No installer/help/extraction execution performed. Network restriction on initial ordinary-shell documentation lookup was resolved via approved scoped escalation; no automatic approval rejection occurred.

Owner's manual command, after inspecting downloaded identity:

```powershell
Start-Process -FilePath "C:\Projetos\_tmp\PapinhoAccelerator\wpa-official-installer-20261023\adksetup.exe" -Verb RunAs -WindowStyle Normal
```

Choose install on this computer; at feature selection leave ONLY Windows Performance Toolkit checked. Do not select Deployment Tools/Assessment Toolkit/USMT/WinPE. If setup detects existing installation or proposes unrelated components/reboot, pause and report; do not replace automatically. After completion, verify installed wpa.exe version/signature/hash and analyze existing ETL without starting WPR. Installer itself may retrieve/verify payloads; their identities remain pending until available. No silent installation or scope expansion.

INSTALLATION_APPROVAL=OWNER_APPROVED_MANUAL_WPT_ONLY
BOOTSTRAPPER_DOWNLOAD=COMPLETE_VERIFIED_MICROSOFT
INSTALLATION_EXECUTED_BY_AGENT=NO
WPA_INSTALLATION=AWAITING_OWNER_MANUAL_ACTION
TRACE_RECAPTURE=NO
PREFLIGHT_RETRY=NO
START_SERVICE=NOT_RUN
