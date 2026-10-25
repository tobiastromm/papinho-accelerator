# Conformance and implementation handoff

Status: PROPOSED. These documents complete the requested focal terms; they are not ACCEPTED, do not qualify UTC, and do not authorize implementation. Original AT Revision2 and PT Revision1 remain unchanged. The exact patch has six clause/coherence change groups: AT2; AT introductory/AT3/AT4/AT9 references; PT0; PT3; PT7; PT revision/intro metadata. No accepted ADR is edited.

| Requirement | Proposed resolution | Remaining factual gate |
|---|---|---|
| AT2 identity/bootstrap | Independently authenticated bare 32-byte key, canonical authorization, separate status | Source-key/status root representation not implemented; separate layout approval if needed |
| Rotation/revocation/restart | Canonical revision, invalidate, no old key fallback or restart authority | Protected CURRENT and current status |
| PT0 exact version/encoding | RFC 10049 v1, local bounded parsing, byte-exact 2048-byte request | Official errata disposition not conclusively retrievable |
| Campaign/replay/divergence | Three parties, six chained sequential exchanges, full evidence, no favorable selection | Entropy, independence and source calibration |
| PT7 transport | Outbound UDP numeric approved endpoint port5319; no PST/PACC/Site TLS | Platform UDP completion/truncation/resource guarantees |
| Deadlines/retry | 1/3/6/60-second cuts, exponential capped retry, 60-second spacing | PAL and scheduler checks, uncertainty retention |
| Q interval/quality | Outward conversion, common-cut hull, width/age rejection caps, exact Q propagation | rho/E/continuity and source correctness cannot be guessed |
| Lifetime/Stop | Sole retained graph, no timeout release, E0 synchronous borrow unchanged | Native factual closure and all external owners |
| ED11 | Explicit ceilings plus UNKNOWNs; prior full scenario already exceeds limit | STOP implementation; no quota increase |
| Portability | No mandatory Win32/CNG, native adapters and existing crypto only | C89/VC6, Windows/box/ISA qualification separately |

The old review's architectural conditions are resolved as proposed choices, not granted operational facts. No implicit source registration or field extension is authorized. Exact official errata closure remains incomplete, so this task does not assert all acceptance conditions already pass. No conflict with the RFC is knowingly introduced: larger/lower-assurance alternatives are rejected locally; no wire extension is created. UDP-only differs from the RFC's recommended TCP fallback because this bounded first profile deliberately has no second transport; document that availability restriction in eventual acceptance.

Before functional hunks: (1) exact owner/executor review of these proposed terms plus official errata; (2) canonical trust representation mapping and any minimal versioned layout decision; (3) independent source/PAL/backend qualification; (4) safe optimization or separately approved capacity resolution for the existing excessive full scenario; (5) exact lifecycle/C89/ABI/census proof. Do not start adapter integration while the capacity gate fails.

Future validation must cover official vectors and packet bounds/grease, wrong keys and compromise/status changes, no UTC bootstrap circularity, chained nonce/proof validation, all causal comparisons, oversize/truncated/late/duplicate UDP, source divergence, retry caps/restart, checked interval arithmetic, unknown rho/E, Stop/uncertain completion/retention and full ED11. These are a handoff, not tests run here. No fixtures, credentials, packets or cryptographic keys were generated.
