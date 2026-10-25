# Manual handoff — no Start authorization

B4.B.5 now passes for the pinned, single-LAB canonical owner implementation and isolated tests. This does NOT qualify the real service token/Start/Stop, grant current capacity, or approve an SCM rebind. The last partial host sample was below5GiB available physical; no automatic new query/Start/retry is authorized.

Exact safe static check (no UAC/native entry/SCM effect):

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1" -Mode Check
```

Prepared manual elevated READ-ONLY preflight, if the owner proceeds with preparation:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1" -Mode Prepare
```

Prepare uses native UAC/operator console, original participant handles, protected binding/SCM readback and transient canonical exclusion. It creates only its bounded preparation log/receipt, no protected positive launch log/intent/image/diagnostic, no rebind or Start. It does not arm or consume positive authorization and its stale capacity cannot authorize a later Start. Never repeat automatically after any uncertainty.

The exact [proposed operation](operation-PROPOSED.json) contains source/helper/script hashes, native references, planned records and explicit rebind. Approval of current implementation is NOT approval of that operation. EXACT-OPERATION-APPROVED.json remains absent. Before any positive operation, require official exact review of this updated plan, eligible operator/live preflight, immutable identities, fresh admission, and explicit owner approval specifically covering new image/launch log/intent/diagnostic, rebind, one Start and at most one safe Stop. The eventual approved-record SHA is then an input to Launch; no executable positive command is delivered as authorized here.

Do not reuse historical launcher pins/receipts or edit accepted policy to erase UNKNOWN. Do not delete consumed markers, rearm, rollback, kill, release uncertain refs, change ACLs, or infer STOPPED from timeout. Failure/UNKNOWN preserves immediate durable retained receipt and original references. No listener/network/CA/key/certificate/bootstrap/CURRENT/signature/recovery authority.
