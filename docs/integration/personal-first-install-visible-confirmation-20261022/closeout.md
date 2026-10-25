# Visible nonsecret confirmation input — 2026-10-22

Owner reported typing was not visible and only Enter appeared accepted. Code inspection confirmed ReadConsoleInputA consumed raw events without echo. No actual keyboard prohibition was demonstrated; this was a concrete UI feedback defect. Earlier instructions to type invisibly did not resolve it.

Reviewed exact.patch adds a visible > prompt, echo of printable ASCII declaration characters only, bounded repeat-count handling and visible Backspace editing. Enter echoes newline and still requires the same byte-exact phrase; Escape/Stop/overlength/write errors deny. No console mode is changed, no secret prompt or logging is added, and no authority/confirmation fallback is permitted. The user is told to enter the entire nonsecret declaration, not passwords.

The stale launcher ReadKey pause was removed. Only the final F10 wait remains, including error paths. Pinned executable SHA was updated after rebuilding candidate Release (reviewed pin.patch); original manifests and old manual receipts remain historical evidence, not qualification of the new binary.

MSVC Debug/Release: ten suites each PASS; zero warnings. Thirty CLI negative cases, eight UAC-launcher negative cases and17 helper checks PASS, without agent-side UAC/elevation or positive ceremony. Visible echo/Backspace in the owner's real console remains to be observed on the next manual invocation. No target/key/certificate/bootstrap/CURRENT/network effect was executed.

Current launcher SHA-256: 881c3840c90a92f92a274a76aa9335e4d8aab2c770f107c39d6cadaba9b6f849
Current Release SHA-256: bb618deffcd975060b2f860833f14cbbc50d61e49b595ad1f57181556961925a

Only UI source and qualification launcher changed functionally. Accepted contracts, PST, Browser, wire, quotas and production0.6.4 unchanged. Input adds only bounded local echo/repeat state; no owner layout, handles, heap, worker or retention changes. ED11 PARTIAL and publication/signature/recovery/CA gates remain DENY. WIP and manual receipts preserved.
