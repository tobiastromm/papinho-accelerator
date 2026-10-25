# V4 TIME_SOURCE strict Ed25519 integration

Task PACC-PHASE6-V4-TIME-SOURCE-ED25519-INTEGRATION-1, local2026-10-21. Scoped offline integration, no network. Accepted S0-S6/P0-P5/TS0-TS8/AT/PT/Q0-Q7, Model E and ED08/ED11 remain unchanged.

## Canonical binding

The existing v4 source_keys traversal calls private component kind9 only after header/collection/TSR1 key digest/checkpoint and component validation. Canonical papacc_root8_issuer_carrier_data now dispatches kind9 to pc_root8_time_source_key_data, guarded by existing owner presence, stopped, unresolved and authority_lost state. Prior unsupported kind9 becomes public mathematical DATA validation only. No bypass or fallback to historical kinds.

The private portable adapter checks discriminator9, readable caller-protected row384/header512, p==row, TSR1/version/length and root magic/version4 before calling the existing independent validator on row[88,120). VALID maps to PC_ROOT8_OK, INVALID to INVALID, unavailable to UNSUPPORTED, internal/unknown to INCOMPLETE. No curve arithmetic duplicated or modified. Core source/header and original archive hashes remain unchanged. The generic codec still requires its supplied component callback; model callbacks are not authorities, and canonical dispatch uses the real backend.

PACROOT8/v4,TSC1,TSR1,CP4 codec sources/layouts/offsets/hash domains remain byte-exact. No worker, root schema, source registry, time producer, transport or authority API is changed. Build integration adds the existing math and adapter to the same offline root codec library and a dedicated test; independent qualification CMake remains PST-free. Existing unrelated PST consumer tests still use their already pinned SDK, without modifying PST.

## Authority and lifetime

Mathematical key validity is separate from identity, TIME_SOURCE authorization, source correctness, status/revocation/currentness, CURRENT/pins and Q0-Q7 UTC qualification. Adapter intentionally does not turn status fields into trust: UNKNOWN/REVOKED remain storable historical DATA under the existing codec. Missing factual authority still denies use. No issuer/UPDATE/UTC/emission/admission/publication is enabled.

No new allocation, mutable state, retained pointer, pin table, owner, lease, native I/O or callback borrow escapes. It borrows the already protected key slice in row384 and header512 synchronously; math returns before existing capture/borrow validation can complete. Canonical owner Stop rejects new calls. In-flight borrows cannot be freed by timeout or interpreted as canceled; existing serialized owner/quiescence/retention contracts remain required. Native backings/pins/capture/borrow/Stop/CAS sources unchanged. Tests use diagnostic DATA tickets, not factual CURRENT or operational authority.

Prepatch assessment: no fixed-layout growth, new retained buffers or alternate pin, only one adapter int-local plus existing mathematical call stack and normal CRT/compiler frames. Operational effects remain denied pending full ED11. No quota change or automatic Windows3MiB selection; native regressions use the existing explicitly Selected test build. No claim that Selected proves operational headroom or that Legacy is activated.

## Verification

Four /TC /Za /W4 /WX direct builds: MSVC19.51 x64 and actual VC6 x86 backend+adapter+caller, Debug/Release.313 checks each, zero warnings.296 inherited checks exercise point validity (valid basepoint/RFC8032, null/length, identity, order2/4/8, mixed2L/4L/8L, noncanonical and offcurve);17 adapter boundary/status/input-preservation checks added. Invalid span cases use the direct validator and adapter383/385-byte row tests cover its own lengths. Public mixed4L/8L points remain previously approved local public derivations, not independent external vectors. No private keys/signatures/fixtures generated.

Native MSVC Debug/Release clean builds pass8 CTest cases/config: key DATA, LST1, root8/v2/v3/v4, managed census and immutable v4 backings. Existing v3/issuer/history/predecessor/CAS/lifetime regressions inside those tests preserved. Native v4 model now invokes real key math, while non-key administrative/issuer contexts remain explicitly synthetic. Rehashed identity key is rejected even with a correct TSR1 key digest and TSC1 integrity. Canonical dispatch with absent owner or Stop returns INCOMPLETE; a valid mathematical point returns DATA_OK only. Existing status0..3 model rows remain DATA, not positive status authority.

Native backing tests preserve full v3/v4 sizes30592/31840, slot40120, all3 slots120360, arena32768, borrow104 and Stop/retention/stale-pin/quiescence behavior. CMake standalone Release builds and passes2 tests without PST paths/runtime. NT4/box/ISA not qualified; complete Win32 root/issuer stack was not compiled on VC6, only portable backend/adapter/caller. No claim of full native root VC6 support follows.

## ED11 and handoff

Adapter raw sections, undefined symbols and assembly saved with ed11-census.json. Conservative adapter-local all-push/reserve/return subtotals64/48 MSVC and44/24 VC6 Debug/Release exclude externals. Existing mathematical compiled subtotals4032/3392 and5916/5160 are preserved evidence, not full stack bounds. Parent codec/dispatch, external CRT/helpers, root/issuer/Roughtime concurrency and retained multiplicities still need full operational census. Component adds no owned heap/buffers; CRT initialization/provider/kernel UNKNOWN remain UNKNOWN. Object code/string/data bytes are counted separately, not hidden by unchanged managed-layout1952624. No assumed memory reuse or saving. ED11 PARTIAL.

Next: separately authorized v4 worker persistence and factual TIME_SOURCE binding, preserving complete predecessor/CAS/pins/history and exact administrative authorization. Mathematical DATA_OK cannot substitute for source authority or qualified UTC. No operational CURRENT, signature or authoritative recovery is produced here.
