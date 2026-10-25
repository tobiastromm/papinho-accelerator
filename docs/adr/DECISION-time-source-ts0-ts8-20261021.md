---
status: accepted
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: exact TS0-TS8 canonical Roughtime source purpose and private v4 persistence only
decision-makers: [Tobias Tromm]
supersedes: []
---
# Canonical TIME_SOURCE — TS0-TS8

## Authority and exact incorporation

Task PACC-PHASE6-TIME-SOURCE-TS0-TS8-ACCEPTANCE-1 conditionally authorizes this exact normative acceptance. This record takes effect only after official executor approval of the exact document operations, incorporation and verified post-hashes. No functional implementation, source selection or qualification is authorized.

Accept ONLY TS0-TS8 from `docs/integration/roughtime-canonical-source-binding-20261021/TIME-SOURCE-BINDING-PROPOSED.md`, whole source 17265 bytes, SHA256 `142260ce7318a8cad3dcfe4cce046e3e892de9992a09611e3611d4a50b94f2b9`, byte interval[715,17265), 16550 clause bytes, SHA256 `0f4d604e732167e2804e05e0188e7d808ca452cddeb6e334d4cfe811763c0038`. Exact original bytes are incorporated through [TS0-TS8-frozen-clause-bytes.txt](../integration/time-source-ts0-ts8-acceptance-20261021/TS0-TS8-frozen-clause-bytes.txt), without encoding or line-ending normalization. Source frontmatter/title/preparatory introduction are excluded. Proposal remains unchanged. [Incorporation and preimages](../integration/time-source-ts0-ts8-acceptance-20261021/incorporation-and-preimages.json) pins all referenced inherited authorities; no future proposal edit is accepted by reference.

Historical PROPOSED/not-accepted/review-before-acceptance wording in frozen clauses is superseded ONLY as to normative status of these exact clauses. All implementation, evidence-source, lifetime, qualification, reserve/admission and operation restrictions remain effective. No API, native producer, codec source, test fixture, deployment key/endpoint or functional patch is accepted by reference.

## Delimited precedence

TS0-TS8 defines TIME_SOURCE as a separate public administrative policy purpose for a bare Ed25519 key, and exclusively NEW PACROOT8/version4, TSC1/TSR1 version1 and CP4 version4 in the same administrative root/owner/worker/transaction. Version4 header fields274..292 and source insertion/hash domains are authorized ONLY under version4. Version3/CP3/PDG1, version2/CP2, version1/CP1, ROOT06/BODY0.5/D3/LST1/SNT1, issuer history/high-water and RFC/PACC/PST/package wire retain their exact bytes and meanings. No old reserved field is repurposed.

The inherited G2/G4 rules apply except the explicit TS4/TS5 v4-only replacements; G5/G7 historical/recovery/lifetime protections remain. Only UPDATE from factual complete v3/CP3 or v4/CP4 is represented. No direct v2 migration, bootstrap, CURRENT fabrication, silent downgrade or authoritative recovery. Existing native CURRENT/pin/predecessor/CAS and R/A/P/I authority remain required. A new temporal purpose is not a CLIENT/SERVER/PACKAGE issuer purpose, PACKAGE_SIGNING delegation, capability or grant.

This adds the distinct TIME_SOURCE authorization requirement to the existing administrative entry; U0-U7's Windows SNT/PDG act and bootstrap permission are not implicitly reused. Operator/act/evidence IDs remain correlations requiring actual protected canonical resolution, never a new evidence store or self-authorizing proof. No default temporal trust/TOFU, source/provider selection, independently operated-source premise, calibration or status freshness is proved here. AT2/PT1 and Q0-Q7 remain governing requirements for actual use, including first-contact independence and missing/unknown evidence denial. No circular UTC bootstrap, timeless status or restored operational authority after restart is introduced.

## Lifecycle, portability and capacity gates

The contract requires immutable source backings in existing snapshots/pins and synchronous borrows of already protected references. External evidence requires its own factual lifetime protection; a root pin alone is insufficient. No second pin table, owner, worker, trust store, registry, journal or asynchronous acquire. Stop invalidates effects without freeing retained graphs; factual quiescence/closure govern release and credits. E0-E8 RELEASE_INVALID remains no attempt/no effect, preserving known borrow/ownership. Package evidence API is not turned into temporal authority.

TSC1 bounds96..1248, record384, header512, CP4512 and image20704..31840 are serialization contracts, not memory reservations. Complete compiled buffers/old-candidate/root-issuer coexistence/snapshots/pins/native/provider/stack/retention require prepatch proof; unknown costs are not zero. ED11 total PARTIAL. Windows3145728 only explicit process-profile selection, legacy2097152 and individual quotas unchanged. No host-headroom or reuse discount follows. Core remains portable C89 with substitutable PAL/backends; no target, VC6/NT4/box/ISA or native lifetime is qualified by text.

## Result

TIME_SOURCE_CONTRACT=ACCEPTED
PERSISTENCE_LAYOUT=PACROOT8_V4_TSC1_TSR1_CP4_ACCEPTED_NORMATIVE_ONLY
SOURCE_AUTHORITY=DENY_UNTIL_FACTUAL_CANONICAL_BINDING_AND_QUALIFICATION
SNAPSHOT_PIN_LIFETIME=ACCEPTED_CONTRACT_NOT_NATIVE_QUALIFICATION
IMPLEMENTATION_AUTHORIZED=NO
UTC_SOURCE=NOT_QUALIFIED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ISSUER_STATUS_BINDING=DENY
UPDATE_U0_U7_BINDING=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4

[Conformance](../integration/time-source-ts0-ts8-acceptance-20261021/conformance-matrix.md) | [Closeout](../integration/time-source-ts0-ts8-acceptance-20261021/closeout.md). No sources, builds, tests, network, clocks, keys/certificates, Browser/PST/wire, quotas, production, clone/worktree or commit/push/tag/release changed. WIP and four LAB preserved. Next scoped implementation still requires separate authorization/exact patch approval and prepatch lifetime/capacity gates.
