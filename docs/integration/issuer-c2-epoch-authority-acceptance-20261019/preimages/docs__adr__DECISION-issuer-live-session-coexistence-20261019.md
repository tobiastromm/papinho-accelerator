---
status: accepted
decision-date: 2026-10-19
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
---

# Issuer live publication and Session snapshot coexistence

Exact owner authority: PACC-PHASE6-ISSUER-LIVE-PUBLICATION-SESSION-COEXISTENCE-1. This record precedes dependent implementation. It supplements, and does not replace, the accepted issuer/technical-profile decisions, C2/C8, Model E, ED08 and ED11.

Each existing Session retains a factual reference to the administrative revision under which it was admitted. New publication must not change its grants, replace its immutable snapshot, reinterpret identity/Principal, invalidate borrowed memory or release pinned/retained slots. New Sessions use the newly published CURRENT revision under the normal admission gates. CURRENT is not retrospective authorization for older Sessions.

Issuer revisions use the same root and PREPARE -> VALIDATE -> confirmed durable COMMIT -> CURRENT -> post-commit factual EPOCH publication. Revision/high-water CAS, immutable snapshots, pins, retained older generations and atomic publication are mandatory. Insufficient slots deny; retained slots must not be overwritten. Issuer publication does not grant operational permissions or modify policy merely by issuing data.

Stop, callbacks, leases, quiescence, exactly-once and factual finalization govern release of Session references. Uncertain close retains references and credit. No DESTROYED is inferred. No second root, registry or listener is authorized.

Recovery remains denied without verified high-water, CURRENT revision, reservations, serial/status/generation/history consistency and old-instance fencing. This decision supplies no checkpoint, fencing proof or new recovery authority.

Scope: canonical implementation and offline tests only; no CNG issuance, real CA, certificate generation, network, admission, Browser/PST/wire change or Git publication. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY. Provider/kernel remain UNKNOWN. Quotas and production pin0.6.4 are unchanged. PST0.6.5 remains candidate until full Phase6 closure. Accepted active-Session revocation policy is not expanded by this record.
