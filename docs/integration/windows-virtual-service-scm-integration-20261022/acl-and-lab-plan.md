# ACL policy and future LAB lifecycle preparation

NOT EXECUTABLE AUTHORIZATION. No ACL, account, service, CA or installation is created by this document. Policies below derive from V3–V5/CA3.3; exact target resource names/descriptors and native access tests remain missing. Do not substitute account naming/readback for token proof.

| Resource | Operational virtual principal | Administrative owner / required evidence |
|---|---|---|
| ProgramFiles executable and dependencies | read/execute; no replacement/write/delete grant | protected native file/parent identity, binary hash, owner/DACL and substitution tests |
| Nonsecret settings configuration | read; no unauthorized rewrite/authority import | exact selected immutable configuration and revision, current native backing held through effect |
| Canonical root/issuer/history/status | only purpose-specific runtime view; no generic authority/change grant | same root/worker/CAS authorization; no administrative data write by broad ACL |
| Mutable operational data | only actual required operations within qualified target | enumerate producers/consumers and derive exact object-specific masks; no blanket tree modify |
| Logs | only required append/output; not bin/config/CA write | sink ownership/lifetime, retention/Stop and bounded buffers |
| Runtime synchronization/worker objects | purpose-specific minimal rights, protected references | actual token access tests, object DACL and quiescence/retention |
| CA private KSP key | private open/use/sign/export denied | existing administrative CA owner/lease only; minimum admin rights and real operational-token denial still require qualification |
| Network/socket effects | only separately authorized listeners/egress | virtual identity is not a network policy grant; existing admission and ED11 remain required |

Do not apply these as blanket GENERIC_ALL/Modify masks. Existing CA LAB success is not proof of this virtual principal's isolation and no LAB key is reused or touched. No productive ACL modification is authorized. Public/custody reference use does not expose a private key/credential to CLI. Direct console token is independently observed and validated, never replaced by an SCM identity or implicitly elevated.

## Future isolated lifecycle (blocked until factual binding)

Use an explicitly approved isolated Windows host/snapshot with no productive installation/CA. The canonical name remains PapinhoAccelerator; no parallel LAB service/framework. Record OS/build/API support (real Windows7 separately), selected installation/config identities, actual operator authority, binary path/hash and exact attempt. Expected objects and cleanup dispositions must be approved before execution.

1. INSTALL: only after protected binding/ACL plan qualification; create fixed virtual logon with NULL password, stopped/manual, no existing-object adoption. Capture Create result, SID configuration/readback and exact installed identity. Error/UNKNOWN stops sequence; no automatic deletion/retry.
2. START: only when runtime admission/config/token/ACL/ED11 are factual. Query configuration is not the token; capture actual TokenUser/groups/privileges and private CA denial without creating keys. RUNNING requires readiness receipt. Current executable's fixed gate MUST deny here, so it cannot yield positive lifecycle PASS today.
3. STATUS: distinguish observed state/PID, identity correspondence, pending/error; do not infer readiness from PID/state alone.
4. STOP: request once through existing handler; preserve STOP_PENDING, borrows/resources until factual quiescence. Deadline/UNKNOWN retains; no forced kill as cleanup proof.
5. REMOVE: only after required stopped/quiescent binding; DeleteService success is requested/pending. Confirm actual disappearance with supported query separately; never delete data/CA/config/history. Partial/unknown results stay retained and no automatic retry.

Prospective CLI verbs are service install, service start, service status, service stop, service remove. They are a future sequence specification, not a command to run in this task. Exact isolated paths/hashes/approval and qualified binding must precede a manual/native positive operation. No launcher bypass, SYSTEM fallback, account provisioning or real SCM mutation was added to tests. No positive LAB test has been executed or certified.
