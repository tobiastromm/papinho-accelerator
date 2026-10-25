# Full Roughtime transcript: offline proof and handoff

Task PACC-PHASE6-ROUGHTIME-FULL-TRANSCRIPT-ED11-1, 2026-10-21. Authority remains the accepted corrected AT/PT decision, RFC10049, Q0-Q7, Model E and ED11. No normative clause or production source changed.

## Authorized LAB operation

The owner confirmed that no complete positive2048-byte transcript had previously been authorized. The owner then authorized preparation of a strictly offline ephemeral-TEST operation, conditional on official executor review. The exact generation operation manifest SHA256 is `93130c8f4f60d04b274cd18833e081aa9de3f59ca601c78233837d54690e2943`; its command was `python C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\run-lab.py`. Official executor approval preceded its single successful execution. See archived operation, runner and generation log. No repeated generation is part of builds/tests.

The existing pinned OpenSSL3.5.8 generated three long-term TEST keypairs and three delegated TEST keypairs in memory. No CA, X509 certificate, real credential or CURRENT was used. No private seed/key was exported into the14 archived public files. This is not a qualification of provider zeroization, crash dumps or pagefile custody. No network or system-clock change occurred. This is unrelated to the rejected .papacc fixture operation.

## Complete public transcript

Six raw RFC requests of2048 bytes and six corresponding480-byte responses follow source identities A,B,C,A,B,C. Three TEST identities do not demonstrate three independently administered operational sources. The fixed MIDP2000000000 and RADI3 are synthetic, not observed UTC. Raw public packets,96 public-key bytes and192 nonce-chain random bytes total15456 bytes, pinned in the fixture manifest and isolated test CMake. No custom wire container was introduced.

The consumer verifies framing, version, bounded maps/tags/offsets, SRV domain0xff, request NONC, full-request Merkle leaf0x00, two-level path/index and internal domain0x01, Ed25519 delegation and response signatures with exact NUL-terminated RFC domains, delegated validity interval, source-key association and chained nonces. Independent Python structure/hash checks corroborated all six pairs; actual Ed25519 verification used the pinned native backend. The supplemental checker initially omitted the SRV0xff domain; correcting that checker required no fixture/product change.

Negative checks mutate nonce chain, NONC, signatures, delegated key, version, Merkle path/index and source key; reject every truncation of every480-byte response, oversized response and short request. Event checks reject duplicate receipt, retain Stop state and enforce deadline boundary. Failed validation preserves output. Synthetic Q inputs exercise interval propagation/hull only. Replay coverage is request-chain and exchange-state DATA, not operational process-wide replay admission or native transport continuity. Existing public AppendixB signature negatives remain unchanged.

## Validation scope

VC6 Debug/Release /TC /Za /W4 /WX passed19 portable checks per execution. MSVC x64 Debug/Release portable checks also passed19 each. These prove portable DATA/test compilation and framing, not native Ed25519 on VC6 or NT4 qualification. Pinned OpenSSL x64 Debug/Release each passed5940 checks, with no warnings. Isolated CTest passed3/3 per configuration: existing crypto, capacity, complete LAB transcript. Existing root ED11 regressions passed6/6 per configuration before the test-only follow-up; root/product sources were unchanged afterward. Logs and exact command files are archived. No network tests ran.

## Lifetime and next gate

The caller/test owns the entire graph throughout synchronous verification. All six requests and responses coexist without overwrite/reuse deduction. Stop preserves bytes and ownership; hashes of all12 full slots match. Context close leaves provider-global allocations retained; only final test-process global cleanup after all synchronous calls return reaches zero counted live allocations. No I/O or borrower was started. This does not prove native cancel/close quiescence or operational retention bounds.

Next: implement/qualify native transport only after its exact reviewed patch and capacity/lifetime proof; authenticate an administratively authorized factual source, qualify PAL continuity/rho/E and source uncertainty, and reconcile full root/issuer/snapshot/pin coexistence and UNKNOWN costs. No positive issuer status, UPDATE, TRUST, CURRENT or admission follows from this LAB proof.
