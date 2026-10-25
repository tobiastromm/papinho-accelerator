# Conformance and acceptance — 2026-10-22

Task PACC-PHASE6-CA-LAB-NONOVERWRITE-GATE-ACCEPTANCE-1. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. No native key/lookup/cleanup/UAC/build/test or operational network action. Official documentation read as requested.

G1 fixes a distinct LAB-only identity and leaves the previous name UNKNOWN. G2 preserves MACHINE/no-overwrite and STOP on NTE_EXISTS/every failure. G3 separates Create, setters, Finalize and public validation, without full-sequence atomicity. G4 preserves same-owner retention, borrow/Stop and separate persistent disposition; no uncertain automatic delete. G5 preserves manual eligible UAC/console/hash/confirmation and no productive authority.

Issuer clause6 and bootstrap P8 concern replacement or trying other identities to bypass uncertain initialization. The accepted scope is one independent disposable LAB experiment, not retry/recovery of the old CA, with old unknown debt preserved. No conflict is found within that strict scope; any actual replacement use remains prohibited. F0–F9 and productive bootstrap are unchanged. The decision explicitly distinguishes all lifetime observations required by this task, without new owner/ABI or free-on-error semantics. Model E/ED08 quiescence and ED11 opaque-cost requirements remain intact; ED11 PARTIAL.

Official references checked 2026-10-22:
- [CreatePersistedKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptcreatepersistedkey): named collision is refused without overwrite; freeing a handle differs from deleting storage.
- [FinalizeKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptfinalizekey): separate completion with failure reporting, not a full transaction guarantee.

Source proposal SHA-256 a38f42437cfefb2268fec9fe6dfe32f18962304de303c8c0e750607799200d6e; accepted G1–G5 segment SHA-256 05f4d542a9e65d2b300bb4d16bd96f6dc1485cedf5aa83e41a851c0137e6999e. The original proposal is unchanged. The exact accepted registration is subject to official write review; post-write byte/hash checks establish incorporation. Existing code, scripts, bins, prior receipts and accepted ADRs remain byte-exact. Only a new normative record, review/preparation evidence and live append are written.

CURRENT_TASK_COMPLETED=YES
LAB_NONOVERWRITE_GATE=ACCEPTED
KSP_COLLISION_POLICY=NTE_EXISTS_STOP
PRIOR_ABSENCE_PROOF=NOT_REQUIRED_IF_ACCEPTED
FINALIZATION_ATOMICITY=NOT_CLAIMED
IMPLEMENTATION_AUTHORIZED=NO
NEW_LAB_EXECUTION=NOT_RUN
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
CA_CUSTODY_INITIALIZATION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=LAUNCHER_IMPLEMENTATION_AND_MANUAL_NATIVE_QUALIFICATION_PENDING
NEXT=CA_CUSTODY_ELEVATED_LAB_FINALIZATION
