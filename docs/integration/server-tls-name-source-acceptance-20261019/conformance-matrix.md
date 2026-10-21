# Exact conformance review

No material conflict identified in the delimited normative source relationship. No functional hunks accepted by reference. See the new decision for exact incorporation/interpretation.

| Condition / authority | Result | Delimitation |
|---|---|---|
| Same root / C8 | CONFORMANT | Existing administrative transaction and ADR0010 resolver; no new registry/catalog/owner. |
| ADR0009 listener collection | CONFORMANT | Stable explicit per-listener/security-ref/TLS-role association; no implicit global/default name. |
| Technical profile1 §1.3 | CONFORMANT | Approved exact SAN name, serverAuth/KU/BC/algorithm/issuer/validity preserved; no CN or CSR authority. |
| Technical profile §1.5 | CONFORMANT | Channel issuer association explicit; origin trust/client roles remain separate. |
| Existing issuer reservation | CONFORMANT_WITH_CLARIFICATION | Before certificate exists, bind approved request/SPKI/reservation; after result, associate actual certificate. No fake certificate is required. |
| S4 CSR agreement | CONFORMANT_WITH_CLARIFICATION | Exact CSR/key/correlation and PoP, not mandatory trust in CSR names/extensions. |
| S5 acceptance gate | CONFORMANT_WITH_DELIMITATION | Accept source relationship only; source implementation/storage/functional hunks still require separate review. Historical proposal stays PROPOSED. |
| papacc.conf N1–N8/F grammar | PRESERVED | No new executable directive/AST field accepted or implemented. |
| LST1 C1–C5 | PRESERVED | Reference fragment only; no authority, unknown format fallback or new bytes. |
| D3/BODY05/ROOT06 | PRESERVED | Missing public association requires exact versioned contract; no reserved-byte use or implicit migration. |
| CURRENT/CAS/root epochs | CONFORMANT | Actual pin/source revision, postcommit publication only, no conflict/UNKNOWN fallback; I/R/A/P distinct. |
| issuer/C2 operational projection | CONFORMANT | Name/security update is not presumed issuer-only; equality proof required to preserve P. |
| Model E / ED08 | CONFORMANT | Source lifetime distinct from root pin; retain old generations/leases/uncertain work; Stop is not destruction. |
| Revocation | PRESERVED | Approval revoked => no new effects; active Session disposition remains separate POLICY_PENDING. |
| ED11 | CONFORMANT_GATE_REMAINS | No quota or automatic margin use; actual strings/copies/refs/buffers/retention priced before effects. |
| Portability | PRESERVED | Normative common association independent of Win32/CNG; no box/ISA/NT4 qualification. |

Original S1–S5 are reproduced verbatim; explanatory source proof, old closeout and all proposed implementation/storage choices are not accepted wholesale. No earlier accepted document is modified. The exact new file preimage is ABSENT. All historical file preimages and WIP are in baseline.json; post-write hashes will be verified.

The remaining first implementation gate is the byte-exact SAME-root content/envelope representation and its ED11/lifetime proof. A future binding patch must not invent persistence fields or claim operational approval from this normative record. Existing offline server preparation remains usable only as candidate public preparation.
