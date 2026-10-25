# Owner manual qualification â€” UAC / read-only first-install

Run only from the owner's genuine local Windows console, outside the official executor. Windows PowerShell5.1 x64 is the prepared host. The launcher never enables bootstrap or changes UAC policy. Owner explicitly authorized ExecutionPolicy Bypass for these qualification processes only; no persistent policy change is performed.

## Verify before running

Launcher fixed path:
`C:\Projetos\PapinhoAccelerator\tools\qualification\personal-first-install-uac.ps1`

Launcher SHA-256:
`881c3840c90a92f92a274a76aa9335e4d8aab2c770f107c39d6cadaba9b6f849`

The operator must verify this hash against the reviewed handoff before invoking the script. The script holds its own file read-only across UAC, and the elevated process checks the expected parent-side script hash. This is a local owner-controlled qualification tool, not a signature, installer trust authority or protection against a compromised administrator/OS. Noncanonical/reparse paths are denied. Only OS modules are imported by absolute path; profiles and arbitrary scripts/commands are not loaded.

Fixed executable: `C:\Projetos\PapinhoAccelerator\build\local-bootstrap-config-cli-20261019\Release\papacc_server.exe`

Executable SHA-256: `bb618deffcd975060b2f860833f14cbbc50d61e49b595ad1f57181556961925a`.
The exact executable, adjacent libcrypto/libssl and fixed source configuration are hash-checked and held against write/delete sharing before use, on both sides of UAC. PE identity must be x64 PE32+ console. Rebuild/drift means STOP and a new reviewed pin, not an override parameter.

## Commands

Optional check (no UAC, no CLI execution; exit2 is deliberate):

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\personal-first-install-uac.ps1' -Mode Check
```

Manual positive launch (the owner must execute this command personally):

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\personal-first-install-uac.ps1' -Mode Launch
```

The launcher requires a real input/output console before reaching Start-Process -Verb RunAs. Windows UAC alone asks for consent/credentials; never type an administrator password into PowerShell or a script parameter. A new visible Windows PowerShell window runs the same hash-bound launcher. No fallback without elevation, scheduled task, service, stored credential, policy/ACL change or external network exists in this tool.

The elevated window identifies its effective operator and verifies eligibility/session/console. If UAC authenticated a different SID, the launcher says so and asks `USE_EFFECTIVE_OPERATOR` before proceeding with that actual identity. This selects the actor for this read-only test; it is not the F0â€“F9 ceremony confirmation. The identities are never assumed equivalent. Any later token/context divergence denies. The CLI independently captures/revalidates its own native identity and protected parent.

The fixed initial plan disables client admission, Site TLS admission, ED11 admission and network egress and defines no listeners. Parent is the existing pinned Release directory; hypothetical leaf is `personal-readonly-target-not-created`. It must remain absent. The CLI displays the exact plan and fresh attempt and asks the operator to type its current `OWNER_NEW_INSTALL <64 hex digits>` phrase. The CLI now shows a > prompt and echoes the printable declaration as it is typed, including Backspace editing; type only the nonsecret declaration, never a password. Enter alone still denies. No input injection or reusable response is supplied by the launcher. Esc cancels; Ctrl+C requests native Stop. There is no automatic retry or forced kill.

Only stderr status is captured in memory and strictly parsed; stdin/stdout remain attached to the console. No transcript, screenshot or log of the plan, nonce phrase, password, key or custody material is produced. The fixed plan contains no private material.

## Expected outcome

The CLI always returns2 with BOOTSTRAP=DENY and CURRENT_PUBLICATION=DENY. Launcher exit0 means only the pinned CLI reported exact confirmation, successful read-only worker revalidation, CLOSED resources and a subsequent observation that the hypothetical child is absent. It is not bootstrap, CA, signature, operational UTC or ED11 qualification. Launcher exit2 is DENY/cancel/unqualified. UAC cancellation1223 is classified as DENY without fallback.

The elevated receipt is exclusively created under this directory's `manual-results/<correlation>.json`; receipt metadata includes only nonsecret effective operator identity, eligibility/console, pinned binary hash, confirmation/status, exit code and child-absence observation plus denial gates. Correlation GUID is a receipt filename, never the native ceremony nonce or authority. A file/path absence observation is not global history proof or proof of provider cleanup. Creating diagnostic receipts is intentional; no installation state/child/key/certificate/CURRENT may be created.

If RETAINED_UNKNOWN, unexpected output/exit, changed context, inaccessible child observation or unexpected child occurs: STOP; do not retry, rename, delete, reset, generate keys or infer rollback from process exit. Absence is UNKNOWN when GetFileAttributes does not return a factual not-found result. No CA owner is fabricated or opened. Existing CA_INITIALIZE and custody/resource gates remain separate.

## Reproduce negative-only tests

```powershell
python 'C:\Projetos\PapinhoAccelerator\docs\integration\personal-first-install-uac-launcher-20261022\run-negative.py'
```

These tests pipe all standard input/output: even the Launch negative must stop at NO_INTERACTIVE_CONSOLE_NO_UAC. They do not request RunAs, invoke a positive native ceremony or simulate an eligible token. Helper identity/1223 cases are classification models only, not UAC authentication. Manual attempts reached an eligible operator and console but returned DENIED_OR_CANCELLED; full positive qualification has not passed. The result window now waits for F10, ignores Enter and discards queued console input before waiting. The same wait also runs after a caught error.
