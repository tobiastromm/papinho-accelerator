# Q7 approved — one owner-manual execution pending

Owner decision and final artifact binding are registered in `docs/adr/DECISION-q4-final-binding-and-one-manual-q7-20261025.md`. Exact manual approval SHA256 `c07384b26a57a438c10a382948ab2be5ffe8ab80fab56138881e605c8c6f4c46`; request `cf88f42ad86700ca1eb7891c834d8c9521f1c30dfc5a55317cb6e4665ba3db58`. No execution has occurred. Q4 NOT_ACTIVATED, budget NOT_GRANTED until factual fresh gates. ED11/Phase6 PARTIAL.

Use one existing Windows PowerShell5.1 x64 already elevated, authorized operator, matching host/boot, original interactive console, no impersonation/redirection. Do not invoke Prepare separately. Execute this command once manually:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -Profile Q4TokenDiagnostic -ExpectedScriptSha256 a78f6c573e65322a34cafc32dd67b08ec6d1fc964552a18ee2c65ab00fed505d -ExactApprovalSha256 c07384b26a57a438c10a382948ab2be5ffe8ab80fab56138881e605c8c6f4c46
```

Use the exact confirmation lines printed at runtime; PID/creation/challenge must be current, never copied from history. Final confirmation is `INSTRUMENT_SCM_LAB 10584f02e11f92c92b43cddf0d9d316f`. The native gates must first revalidate original protected identity/RAW1/content, N7, exclusivity, four roles,128/1024 budgets,60s/1s freshness, headroom, ownership and Stop/freeze. The command does not pre-grant budget or waive any DENY.

Effects limited to the approved request: one new consume, exclusive image/sink/embedded arm, one explicit rebind, one Start and at most one safe accepted Stop. No retry after refusal/failure/UNKNOWN, no overwrite/delete/cleanup/rollback/kill/forced mutex release. Preserve uncertain references and partial effects. Escape requests Stop between synchronous calls. Use F10 only when the launcher confirms factual closure and requests it. Send the entire original output back for reconciliation; do not rerun. The future new token does not establish historical03 attributes or global quiescence.

All candidate documents, protected inputs and historical approvals remain unchanged. No build/retest/native preflight/SCM/ACL operation was performed to register this decision. Productive authorities remain DENY.
