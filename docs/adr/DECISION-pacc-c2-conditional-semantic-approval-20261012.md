---
decision-id: PACC_C2_REVISION3_CONDITIONAL_SEMANTIC_APPROVAL_AND_EXACT_ENCODING_FINAL_GATE
status: accepted
revision: 1
decision-date: 2026-10-12
last-revised: 2026-10-12
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
---

# C2 Revision3 conditional semantic approval
Normative owner-decision record, separate from existing accepted ADRs. No canonical numbered ADR or wire registry allocation is inferred.

## Approved scope and conditions
The owner explicitly accepts the semantic DIRECTION of C2 Revision3 as the basis for normative closeout, conditional on the final review of ED01-ED14. This is not unconditional acceptance of every semantic clause or an accepted wire specification. It approves preparation of final documentary recommendations only.

Preserve byte-exactly semantic revision3, specification0.3, twelve proposed messages, fourteen candidate vectors, layout registry and ED01-ED14 matrix. Semantic contract SHA256 D427ED8B42683B4BA2C1B317C159F81BD71B39844E4D44A52FBBD75D5E0BBDC9. Specification0.3 SHA256 FEE409F2E71A0FC9ABDB57F657E1479B3926D73DED471A0CD63D566F4BDD9FF2. Candidate vectors SHA256 958B151826E7D30349C9717F7376AA8D9D168B0580B1B12A310091D8889BFD50. Exact layouts SHA256 49DF70E2783683A64023227A19BDEB55AF5A3670A5FC5A22899CA9D952F1C8BD. Matrix SHA256 1313CF567B0B41096599A4A98F54D01C812DFD48BEA80E4E31A0C73910A83C5B.

Required final review:
- ED01-03: canonical registry, schema/offset/length/enum/limit and old-peer behavior.
- ED06: target-appropriate CSPRNG, comparison, erasure and abuse controls.
- ED07: real public PST TLS/chain/name/ALPN facts, never invented metadata.
- ED08: all four fences/downstream counters and feasible non-premature TLS closure.
- ED09-10: partial revoke, remote FINAL versus local destruction.
- ED11: total metadata/callback/PST/provider/retained/kernel budget suitable for old targets.
- ED12-13: Browser Internet egress in Mode C, Secure/Legacy distinction, no Legacy production.
- ED14: exact spec/layout/vector/future codec/test consistency.

A documentary resolution may leave explicit implementation proof obligations, but MUST NOT present them as runtime qualification. Progressive human-readable protocol specification and ADR traceability remain mandatory. New recommendations remain proposed pending final owner gate. Actual contradictions with accepted contracts must be reported before dependent work.

## Explicit limits
Encoding candidate1 is NOT approved as definitive wire. C2/C3 implementation, functional Accelerator/Browser/PST/PACC changes and E1-E12 execution remain unauthorized. Production pin0.6.4 unchanged. No commit, push, tag, release or PST promotion. Existing accepted contracts, historical evidence and WIP are preserved.

C2_SEMANTIC_DIRECTION=CONDITIONALLY_APPROVED
C2_WIRE_ENCODING=PROPOSED_PENDING_FINAL_GATE
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
