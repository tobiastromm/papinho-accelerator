# Exact normative acceptance closeout

TASK=PACC-PHASE6-INTERNAL-ISSUER-MANUAL-CUSTODY-NORMATIVE-ACCEPTANCE-1.
2026-10-18. Owner explicitly authorized exact acceptance subject to official executor review. Canonical Accelerator main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a; initial WIP500 status entries, retained in BASELINE.json.

## Accepted record and provenance

[DECISION-internal-client-issuer-and-manual-custody-20261018.md](../../adr/DECISION-internal-client-issuer-and-manual-custody-20261018.md) was submitted as an exact new-file operation via the executor's require_escalated approval flow and approved. SHA256:6302723d48887b7059f6e4ae2aca1d7de20440ec8bba70641199f07abf9eb9b3. Existing destination absent; CreateNew prevented overwrite. Staged and persisted bytes matched.

Proposal source SHA256:5e3564b92bf713d742f2edb9a5b12592999851d26c36e282e1343a1871b3b26e. Original proposal, issuer implementation handoff and closeout remain unchanged as historical PROPOSED documents. Eleven numbered decision clauses copied without edits; exact case-sensitive clause-section comparison passed after persistence. Only acceptance metadata/context, explicit precedence and limits surround the copied clauses. No serial encoding, package algorithm/threshold, backend or active-session policy was accepted through a handoff reference.

## Clause-by-clause conformance

| Clause | Checked scope | Review |
|---|---|---|
| 1 | Explicitly initialized internal private CA; no external requirement; dedicated CA key/certificate/identity, no TLS-key reuse/startup regeneration | CONFORMANT |
| 2 | Issuance, persistent history/status/recovery/audit, same manual/automatic registry, Principal/policy separate, no implicit grants | CONFORMANT |
| 3 | MANUAL-only generated client key, protected authenticated package, no persistent plaintext/recovery password, owned transient buffers/credit/wipe | CONFORMANT_SCOPED_EXCEPTION |
| 4 | Additional manual server/delivery compromise risk and cloning/erasure limits explicit | CONFORMANT |
| 5 | AUTOMATIC local key/CSR/authorized pairing unchanged | CONFORMANT |
| 6 | Authenticated administrative CA init/backup/restore/rotation with correspondence/history; uncertainty fails closed | CONFORMANT |
| 7 | Existing root prepare/validate/durable commit/CURRENT/epoch and live publication obligation; no parallel root/listener or dormant bypass | CONFORMANT_NOT_IMPLEMENTATION_PROOF |
| 8 | Persistent revoke/new-auth deny and renewal; no new active Session behavior; Model E/ED08 retained | CONFORMANT_ACTIVE_POLICY_PENDING |
| 9 | Package v2 remains proposal, no crypto/bounds acceptance, simple and advanced imports converge on existing generations/leases | CONFORMANT |
| 10 | Reviewed replaceable purpose-specific mechanisms, preserve certificates/history, no PapinhoTrustStore implementation/dependency now | CONFORMANT |
| 11 | ED11 costs/UNKNOWN, quotas/gates/pin/wire/release constraints preserved | CONFORMANT |

## Precedence review

ADR-0006 client-local generation and its general server-compromise protection claim receive only the explicit complete-MANUAL exception in clauses3–4. Item2 of the earlier accepted provisioning decision receives the corresponding scoped authority; no blanket private-key export. Internal CA management selects ADR-0006 private-CA option with separate administrative key custody, not a normal TLS server credential. Automatic/all other contexts and remaining accepted rules stay in force. Earlier ADR files not edited or globally superseded.

ADR-0028 trust/AuthN/AuthZ boundaries, immutable Session capability limits, factual root epoch, Model E retention/quiescence and ED08 remain unchanged. ED11 provider/kernel UNKNOWN stays UNKNOWN, not approval. No runtime grants, committed configuration, policy CURRENT or epoch fabricated. Actual implementation remains incomplete: first dormant root commit is not live issuance registry integration. Existing Sessions do not acquire new revocation behavior.

## Preservation and limits

10/10 selected baseline authority/source hashes unchanged, including original accepted ADRs, proposed/historical documents, root source and production pin. Original proposal's preimage remains in its original file and is hash-pinned; new accepted destination had no preimage. No source writes or historical artefact overwrite. git diff --check passed. This scoped baseline is not a full fresh hash inventory of ignored outputs.

No code implementation, build, tests, network, real CA/key/certificate/credential, Browser/PST/PACC modification, admission, Git config/ownership change, clone/worktree, commit/push/tag/release. No runtime ED11 delta from documentation. Six provisioning gates OPEN. Documentary closeout/baseline/hash writes also submitted for official approval. Acceptance does not start implementation automatically.

CURRENT_TASK_COMPLETED=YES
INTERNAL_CA_NORMATIVE_STATUS=ACCEPTED
MANUAL_CUSTODY_EXCEPTION=ACCEPTED
AUTOMATIC_LOCAL_KEY_RULE=PRESERVED
ACTIVE_SESSION_REVOCATION=POLICY_PENDING
IMPLEMENTATION_AUTHORIZED=NO
REAL_CA_CREATED=NO
CERTIFICATES_GENERATED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_SCOPED_HASH_AND_WRITE_SCOPE_VERIFICATION
EXECUTOR_APPROVALS=OFFICIAL_ACCEPTED_RECORD_WRITE_APPROVED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NONE_WITHIN_EXACT_ACCEPTANCE_SCOPE
NEXT=SCOPED_INTERNAL_ISSUER_IMPLEMENTATION