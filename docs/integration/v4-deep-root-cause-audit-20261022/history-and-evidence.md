# Historical origin: proven facts and bounded inference

Git read-only searches cover all reachable refs. The first reachable addition
for stored functions in history-evidence.json is commit
913c90cf2328f7939d8f5b927640601903ac6e91 (2026-10-21T11:20:33-03:00, phase 6).
The repository has no intermediate reachable commits giving those functions
separate creation dates. A dated handoff is evidence of a task/artifact, not an
unrecorded commit. Worktree changes must not be attributed to HEAD silently.

| Element | Git/documentary origin | Original purpose / constraint | Later consumers or accumulated responsibility / disposition |
| --- | --- | --- | --- |
| store02_hash | First reachable HEAD; BODY0.2 source comments and older config/transaction artifacts | SHA256 integrity of older complete model store, no grants/epochs from decoding | Independent historical helper kept local native handles; legacy path remains, not v4 CAS; cannot reuse failed-close pattern |
| store03_hash | First reachable HEAD, BODY0.3 private profile with policy bodies | Same custody/selection with four1024-byte bodies; SHA checksums | BODY04/05, D3 worker and larger image checks reused it; originally local BCrypt refs, later explicit helper |
| papacc_store_hash | Untracked WIP; bcrypt-failed-cleanup-lifetime task exact operations manifests | Repair lost-reference failed cleanup; owner selected old APIs instead of Win10-only one-shot | Shared owner/worker contexts now used by store03 AND old digest; ownership split must follow call sites, not function name |
| digest(record,480) | HEAD legacy record integrity; current WIP delegates to helper | Protect exact first480 bytes, persist footer at480 | Nested BODY04/05 callers keep old integrity; rerouting common helper affects historical callers |
| issuer_hash06 | First reachable HEAD; internal issuer/ROOT06 tasks and native owner integration20261019 | SHA256 callback for issuer images/history plus owner-retained native pair | Later root v2/v3/v4 codecs, backing CA/SPKI, act/predecessor correlations reuse it; math/native authorization responsibilities now coupled |
| generic hash callbacks | issuer_codec / root codec sources first reachable HEAD | Keep portable codecs free of native API; define SHA bytes separately from provider | Correct seam exists but Windows binding often selects issuer_hash06; callback need not become a new service/authority |
| BCrypt | HEAD Windows backend, later native binding artifacts | Existing available Windows SHA/RNG; no own primitives | Provider allocation/cleanup brings opaque lifecycle/byte costs; not required by digest semantics |
| CryptHashCertificate2 | HEAD server_issuer_windows_validation; internal-ca-windows-backend-continuation artifacts20261019 | Canonical CSR/SPKI/certificate hashes integrated with CryptoAPI verification | Signer component validation, CA/issuer consumers; unrelated to whole-file CAS; remains outside a blanket hash replacement |
| transactional pending_worker / UNKNOWN | First reachable HEAD; native issuer worker / d3-canonical-worker-no-issuer20261019 evidence | Offload only native persistence, full file/identity CAS, factual result/retained errors | ROOT08 then v3 image guards reused same worker; packet11 supports v3 only; no v4 attachment automatically follows |
| comparison scratch512 | root08-worker-storage-strategy20261020 exact patch/evidence (included in HEAD) | Eliminate redundant full observed image while comparing ALL bytes | Actual proved reuse after each synchronous ReadFile; retain previous/next/committed images |
| G7 backings/pins | tracked v3_backings source in HEAD; v3-immutable-backings-pin-lifetime and after-G7 artifacts20261021 | Replace temporary decoded pointers with same-owner immutable copies under real pin lifetime | Whole packet/CA rehash at borrow; candidate serial0 deliberately unexportable; published borrow cannot validate an unpublished candidate |
| backing_capture_data_version / v4 wrappers | Current generic symbol absent from reachable Git pickaxe; WIP v4-backings-pins-ed1120261021 | Add v4 DATA storage to same G7 slots, not another owner | Still invokes common issuer callback and external component port; closure is not canonical v4 transaction proof |
| ED11 | ADR0034/spec acceptance artifact20261012 in HEAD, incremental-envelope decision20261012 | Bounded process-managed allocations, retained quotas, separate target/native/host facts | Windows selected cap3MiB decision20261020; default2MiB unchanged. Later prepatch audits must not convert scenarios into peaks or implementation proof into authority |
| v4 / TIME_SOURCE / strict math | TS0-TS8 accepted artifact20261021, v4 codecs/key adapters untracked WIP | Represent source policy independently; validate Ed25519 DATA with independent backend | No native CURRENT/status/UTC from representation. Existing kind8 and worker version checks remain separate unfinished work |

Supporting documents remain in their original integration directories; no
historical proposal or accepted ADR was edited. Exact source hashes are in the
input manifest. Intent not explicit in an ADR/source comment is marked inference
in the main report, not converted into an author statement.
