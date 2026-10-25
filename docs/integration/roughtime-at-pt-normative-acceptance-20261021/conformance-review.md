# AT/PT exact normative review: STOP

Status: PROPOSED. No normative acceptance, implementation permission or positive source qualification is recorded.

## Exact blocking clauses

PT0: "Resolve normative updates/errata and all implementation limits before acceptance."

PT7: "Still UNSET/unqualified:actual source/key/status/bootstrap representation;exact transport/version/backend profile;source calibration/health/fault model;max reply/proof sizes,campaign storage/concurrency,deadlines/backoff/age/uncertainty policy;PAL rho/E/continuity;primitive availability/C89/VC6 compatibility;full operational ED11."

PT7: "Filling them requires separate owner/normative review and exact functional patch approval."

AT2: "Bare-key or other trust modes need exact protocol/trust review;none is implicitly authorized."

Sources: ../authenticated-time-at0-at9-review-20261021/protocol-profile-PROPOSED.md (PT0/PT7), AT0-AT9-REVISION2-PROPOSED.md (AT2). This is an incomplete acceptance precondition, not a demonstrated contradiction with an accepted ADR. Joint acceptance stops under task sections 3, 5 and 9. No unilateral partial acceptance of AT is performed.

## Conformance matrix

| Requirement | Finding | Acceptance consequence |
|---|---|---|
| Substitutable portable source | AT/PT preserve portable Q core and platform adapters | Direction consistent; no target qualification |
| Identity/authenticity/bootstrap | PT1 requires independent canonical authority; representation and trust-mode review remain open | Material normative gate |
| Version/wire/validation | PT0 incorporates the published specification but PT7 still defers exact version/transport profile | Exact profile not frozen |
| Nonce/replay/request binding | PT2 requires complete campaign evidence; no invented wire permitted | Concrete acquisition profile still required |
| Divergent servers | PT5 rejects favorable selection/fallback | Exact campaign/failure policy must be completed |
| UTC interval/rho/E | PT3/PT4 preserve conditional conversion and independently qualified PAL guarantees | No factual quality claim |
| Continuity/restart/rollback | PT2/PT5 deny restored authority and obsolete evidence | Preserved direction; native proof pending |
| Stop/retention/E0 | PT6 preserves ownership, synchronous borrows and no timeout release | Preserved direction; implementation not accepted |
| Limits/deadlines/age/concurrency | PT7 explicitly UNSET; PT0 requires closure before acceptance | Material normative gate |
| Historical root/issuer authority | No format or accepted authority changed | Preserved |
| C89/Windows/box/ISA | No mandatory Win32/CNG dependency introduced | Compatibility unqualified, no tests |
| ED11 | Operational costs unbounded in current profile | PARTIAL; no quota/admission permission |

## RFC evidence boundary

RFC 10049 was read in full from the official raw publication; its digest is recorded separately. Review anchors are sections 4-5 (encoding and validation), 6 (local frequency assumption), 8 (campaign and trust configuration), 9 (security) and 12 (version registration). Exact RFC encoding may be incorporated by reference; it need not be reinvented or copied into project wire. A valid signature alone is not clock correctness. The basic single-server exchange must not be presented as the multi-server security campaign. Server-list governance remains an administrative responsibility. See https://www.rfc-editor.org/rfc/rfc10049.html .

## Normative gaps versus deployment gates

Choosing a vendor/server is excluded and is not a prerequisite imposed by this review. Actual keys, calibration, platform qualification, factual CURRENT and operational measurements can remain deployment gates in an accepted architecture. The blocker here is the proposal's express deferral of the exact trust/profile rules and bounded acquisition policy required for this requested exact-profile acceptance. This review neither supplies defaults nor treats runtime qualification as normative acceptance.
