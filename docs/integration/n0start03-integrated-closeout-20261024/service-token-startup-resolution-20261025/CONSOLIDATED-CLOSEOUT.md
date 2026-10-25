# Service token and startup — consolidated investigation and qualification

TASK=PACC-PHASE6-ED11-SERVICE-TOKEN-STARTUP-RESOLUTION. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preserved. No new LAB operation, identity, intent, image/sink, SCM/ACL mutation or operational approval. Technical instrumentation and offline qualification completed; original service attributes and positive runtime qualification remain unproved.

## Demonstrated facts and limits

The original owner receipt proves Start1/Stop0/intent1, image/sink/rebind effects, final first13 at OBSERVE_FROM_START_PENDING, separate service return5 at psr_prepare/token.specific_group.enabled. Administrative993/1024 and995/1024 and qualifier112–115 samples remain valid historical observations; no redundant handle experiment or preflight repeated. The original process25396 terminated, its bound original handle signaled, own close0/transport close confirmed. Global quiescence and transitive release remain UNPROVEN; consumed03 and effects are preserved, no refund or replay.

The configured virtual account is NT SERVICE\PapinhoAcceleratorLab20261023; specific SID S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451. Read-only SCM query in the reconciliation observed UNRESTRICTED, STOPPED/PID0/Win325, DEMAND_START, instrumented03 ImagePath, no recovery actions/triggers. Those configuration facts are not a live-token readback. The record reached the specific-group predicate after verifying primary TokenStatistics, TokenUser equality with the resolved service SID, no unexpected thread impersonation token at that probe, bounded groups, no enabled Administrators group and enabled generic SERVICE group S-1-5-6. These are source-linked passed predicates, not archived raw TokenStatistics/SIDs/privileges for all threads.

The operator/qualifier/auxiliary belong to the approved administrative context; the service has its separately checked virtual identity. Do not transfer an operator group, elevation or N7 auxiliary observation to the service. Raw service group bytes, restriction list, exact specific SID attributes and TokenId/AuthId/ModifiedId were not preserved in the final record. Presence-with-disabled/deny-only and absence both produce the same historical false predicate. Historical observed_a/b0 were unset generic fields, NOT evidence of absence. No original NTSTATUS exists for this authorization predicate: nt_valid0. Last API CreateWellKnownSid/api_success1/raw0 was successful.

## Origin and hypothesis

Accepted virtual-service decision V2 requires configured logon, resolved SID, configured SID type and actual TokenUser/groups/attributes as distinct evidence; V4 requires effective least-privilege access and V5 factual lifecycle. B4.C/N1/N8 require actual service token/security outcomes. The literal token.specific_group.enabled is the private LAB runtime's concrete eligibility check, not a named independent ADR clause. Source origin is protected runtime psr_token; the earlier implementation handoff requires actual primary identity and effective checks. No normative authority was found that makes a configured SID type equivalent to live enabled-group proof. Existing guard is preserved, not reinterpreted or bypassed.

Microsoft SERVICE_SID_INFO documents UNRESTRICTED attributes ENABLED_BY_DEFAULT|OWNER; TOKEN_GROUPS distinguishes ENABLED bit0x4 from default0x2/owner0x8. Thus the documented0xA pattern is a discriminating hypothesis: the existing predicate rejects it. A model reproduces that rejection, but cannot establish the original token was0xA. Missing group, token filtering, deny-only and initialization behavior remain distinguishable hypotheses. No account/SID/SCM change or predicate relaxation is technically justified solely from the current historical receipt.

Sources: [SERVICE_SID_INFO](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_sid_info), [TOKEN_GROUPS](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_groups), [TOKEN_USER](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_user), [CreateRestrictedToken](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-createrestrictedtoken). API documentation describes semantics, not this terminated process's actual token.

## Minimal implemented instrumentation

Existing recorder already borrows validated TokenGroups and emits verbose group lines as kind0. Existing psrd_emit deliberately ignores kind0 and writes only the first kind1 or success kind2 bounded frame. Consequently the original final file lacked these facts. Do not create another logger or enlarge2048bytes.

Only in PAPACC_PSR_TRACE_LAB, count exact matches to native.service_sid BEFORE filtering by attributes, OR their original attributes, then use existing PSRT_VALUES before the unchanged group predicates. On token.specific_group.enabled failure, observed_a is matching-entry count and observed_b is raw OR attributes:0/0 absent;1/0 disabled;1/16 deny-only;1/20 contradictory enabled+deny-only;1/10 default+owner without enabled. More than one entry is explicitly ambiguous; no count/OR grants authority. Buffer bounds/IsValidSid checks still precede inspection. No SID/user names/handles/privileges or extra token queries are exported. Other predicate contexts retain their original generic field meaning; do not apply this decoding indiscriminately.

Added IG_CENSUS_ROLE_MASK expected/seen/missing only on the existing missing-role failure, through the same stdout transport. No revalidation/census omission, new wait, replacement handle or role tolerance. The first-error diagnostic still runs first. Raw masks are not exit/identity proof; they identify which inventory roles were absent from that particular snapshot. The historical missing role is still unknowable from the old output. Original-process termination racing the census is plausible, not proven causal ordering between first13 and service5.

Source equivalence check: removing exactly the three new trace-only blocks makes runtime byte-equivalent to its preimage. The census patch adds one error-message call after the unchanged denial. No guard, close/borrow, Stop, ownership, freshness, allocation or quota logic changed. A1 freeze/B1/self-token, RAW1/N7/P3/H/T preserved. Accepted documents untouched.

## Costs and qualification

Four Selected/Legacy Debug/Release product builds PASS, zero warnings. 76 final regression executable/configuration outcomes PASS, including actual runtime/closed/file models, BD identity/replay/Stop, A1 lifetime/UNKNOWN and freshness. Expanded actual psr_token caller tests cover missing, disabled, deny-only, contradictory flags, default+owner0xA, valid enabled, TokenUser divergence, primary-vs-impersonation mismatch, unexpected thread token and query failure. Existing token-change/ACL/Stop/uncertain-close tests remain valid.

Actual final-frame caller models prove five differing group failures persist in the original1024-byte final frame/2048-byte logical file, no later failure overwrite and no handle export/valid-NTSTATUS fabrication. No actual protected file is written by these models.

Eight standalone fixture builds/runs PASS: actual extracted census body in four configurations x8 cases=32 simulated query outcomes (complete census, service absent, qualifier absent, snapshot failure, prequery failure, uncertain close retention, Stop before acquisition, identity-query rejection). Masks are captured from actual printf varargs, not reconstructed by the test. PID/lifetime authenticity is delegated to unchanged BD checks and their regression suite, not proved by synthetic IDs.

Four actual native restricted-token tests PASS: own-process query and reduced token, impersonation duplicate only for AccessCheck, restricted allow denial despite World allow, no thread impersonation, checked closes. They never start a service or impersonate a service account. The first fixture failed87 because its synthetic restriction was incompatible with the already restricted execution context. Corrected fixture intersects with an existing restricting SID under documented rules; it does not elevate, remove restrictions or obtain a substitute token. The initial failure log is preserved. Real service restriction state remains UNKNOWN.

Compiled PS_DIAGNOSTIC4400/PS_RUNTIME12080/PS_NATIVE24992/total storage25208 unchanged. New trace locals are8 source bytes. Same-toolchain Selected Release isolated assembly comparison: psr_token stack allocation336→352, +16bytes including compiler alignment, plus unchanged saved-frame mechanics. This is one compiled frame, not whole stack or transitive memory proof. No new persistent mutable storage, native handles, threads, logger, buffer pool or credits. Test-only4096byte restricted-SID buffer is not product storage. Never claim total memory cost zero.

## Artifacts and authority

Current diagnostic qualifier Selected Release SHA256 `5aefedf1f5119f2e8f27606f0ca3c0f210a9009539143ddf250feb6c66a41f12`.
Current diagnostic service candidate Selected Release SHA256 `c4b4021c5661832dd22c51118ef33ab894dc4d781ddebe8d4ae64f4483f926dd`.
All four-build artifacts/source pre/post hashes: DERIVED-PINS.NOT-ACTIVATED.json. Source diffs/preimages and frame assembly are preserved. These are new build candidates, not the consumed03 executable/dossier. Historical protected image remains44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c; old request/approval bytes intact.

Launcher and operational pins are intentionally unchanged; its consumed positive qualifier pin no longer matches the rebuilt diagnostic candidate, so replay remains fail-closed. No new executable record, approval address or request is activated. A future candidate must bind the actual new binary hashes to a legitimately authorized fresh identity/dossier. No historical protected files are regenerated to match new builds.

## Next real gate

See TARGETED-NEXT-GATE-PROPOSAL.md. No equivalent read-only handle observation or blind Start retry is needed. The original token cannot now be recovered through querying current SCM. The next discriminating runtime evidence is specific-group match count/raw attributes plus census masks using this bounded instrumentation, only after valid scope/disposition/identity authorization.

Important normative limit: P0–P9/P3 cover consumed-before-Start02 only. P3.2 expressly excludes any StartService call, even one that fails. L0–L9 authorize one successor03, not an automatic04. H0–H9 do not silently extend to new scopes. Since03 started, do not apply the pre-Start transition or create another identity/intent automatically. A post-Start retained-effects/closure/successor decision is a real owner gate, separate from approving diagnostic code.

No safe eligibility correction is applied without original distinguishing facts and security review. No current report claims service RUNNING, readiness, governed Stop, complete concurrent metrics, global quiescence, refund or ED11/Phase6 completion. Existing administrative observed PASS remains preserved.

TECHNICAL_INSTRUMENTATION=QUALIFIED_OFFLINE
SERVICE_TOKEN_ROOT_CAUSE=SPECIFIC_GROUP_PREDICATE_FALSE_ATTRIBUTES_NOT_ARCHIVED
HISTORICAL_SPECIFIC_SID_ATTRIBUTES=UNKNOWN
FIRST13_MISSING_ROLE=UNKNOWN_OLD_MASKS_ABSENT
SECURITY_PREDICATES=UNCHANGED
NEW_OPERATION=NOT_CREATED
START_EXECUTED=NO_THIS_WORK
STOP_EXECUTED=NO_THIS_WORK
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_POSTSTART03_SCOPE_DISPOSITION_AND_TARGETED_TOKEN_DIAGNOSTIC_DECISION
