# Exact AT/PT review: acceptance remains PROPOSED

Task PACC-PHASE6-ROUGHTIME-AT-PT-ACCEPTANCE-ED11-1. Reviewed AT Revision3, PT Revision2, exact patch and preimages. Previous 14 manifest entries match; regeneration of the unified patch from the preserved source proposals produces identical bytes. No proposed or accepted clauses are modified by this review.

| Subject | Finding |
|---|---|
| AT2 bootstrap | Explicit independent administrative bare-key authorization; no response-derived anchor or circular X.509 date exception. Actual canonical representation/CURRENT/status remains a separate implementation gate. |
| Identity/authentication | Pin and delegated protocol verification separated from source correctness and administrative authority. No provider selected. |
| PT0 limits | Exact packet and campaign ceilings supplied. Oversize valid packets are unsupported locally; no truncation-as-success. |
| Replay | Full retained request/proof correlation and six-exchange campaign; no favorable retry or old observation reuse. |
| PT7 transport | UDP-only client with independently authorized numeric endpoint. TCP fallback recommended by RFC is deliberately excluded; this availability restriction requires explicit review, not an assertion that all RFC recommendations are implemented. |
| Deadlines | Explicit rejection caps and exponential backoff; cancellation/timeout not resource release. |
| Uncertainty | Conditional common-cut hull includes delay and independent rho/E. No authentication-to-accuracy inference. Full civil validity remains required. |
| Rollback/restart | Invalidates operation and does not restore CURRENT, approval or observation authority. |
| Stop/retention | Same owner, retained graph, factual quiescence. No timeout refund. |
| Portability | Core has no mandatory Win32/CNG; actual C89/backend/PAL qualification is unproved. |
| ED11 | Historical synthetic full composition is not an operational measured peak; see census and lifetime review. No operational reservation granted. |

The byte-exact request table was arithmetically inspected: 12 outer + 40 map header + 72 fixed values + 1924 zero padding = 2048 bytes. This inspection is not a parser regression or interoperability test.

## Mandatory outstanding acceptance condition

PT0 expressly says: "Errata disposition must be verified at the acceptance review."

Official queries to https://errata.rfc-editor.org/search/?rfc_number=10049 and RFC Editor errata routes were inaccessible through the documentary tool. This is not evidence that no errata exist. The previously archived official RFC10049 raw text/digest remains the reviewed edition; no operational requests were made. Because this necessary acceptance condition is unverified, AT/PT remain PROPOSED under task section 3. No ACCEPTED ADR, frozen acceptance annex or functional patch is authorized. Remaining deployment gates are not conflated with this documentary acceptance blocker.

RFC reference: https://www.rfc-editor.org/rfc/rfc10049.html . Official documentary retrieval does not select or contact a time provider.
