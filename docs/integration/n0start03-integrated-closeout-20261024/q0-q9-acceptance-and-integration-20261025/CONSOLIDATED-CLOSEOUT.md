# Q0–Q9 acceptance and technical integration — consolidated closeout

2026-10-25. TASK PACC-PHASE6-ED11-Q0Q9-POSTSTART-INTEGRATION.

## Decision and authority

The exact approved Q0–Q9 revision1 candidate SHA256 is `7eab14040096173ccf28a56c818737115046c94ba657cac51920b36754e9656f`. The original candidate and byte-exact approved copy are unchanged. The designated accepted decision is `docs/adr/DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025.md`, SHA256 `97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca`. Its body preserves Q0–Q9; acceptance, documentary application, functional integration and operational execution are explicitly separate. Owner wording is preserved in owner-normative-approval-record.json. Prior N5/N7/RAW1/P3/H/T decisions remain intact.

The owner's conditional integration TASK supplies technical authority after acceptance. No new scope has been activated. Q2 is NOT_APPLIED; no new identity, annex, operational approval or protected object was generated.

## Integrated implementation and source review

`apps/server/server_service_observation_win32.h` is the qualified borrowed-original diagnostic helper, now shared by actual runtime and census. It creates no handle, owner, logger, thread, service or authority. The isolated fixture now includes this actual source, not the historical NOT-INTEGRATED copy.

In the fixed private LAB trace runtime, the initial primary-token statistics are copied into existing owner storage after the original primary/user-SID checks and before group rejection. Existing TokenUser/Groups buffers stay owner-owned. The same recorder obtains specific-SID count/raw OR attributes, TokenHasRestrictions raw result/returned width, restricted-SID count, query error/known mask/completeness, bounded by original TokenId/AuthId/ModifiedId checks before/after. Missing or invalid snapshots remain UNKNOWN. ENABLED0x4, DEFAULT0x2, OWNER0x8 and DENY_ONLY0x10 are never equated. Duplicate SID entries are reported as a count, not admitted as an authenticated identity.

Metadata is serialized in the existing first-failure AND successful-prepare frame. No second logger or diagnostic file. Original error/API/NTSTATUS validity, original return and LastError are preserved when a predicate already failed. Query failures have their own `q_error/q_known/q_complete`, never become absent/zero facts. If the original preparation would succeed but required token observation fails, one first-failure frame is emitted and readiness is denied; no second query/append/retry. Standalone file-capsule fixtures without a runtime observer explicitly report UNKNOWN/NOT_SUPPORTED and do not constitute the canonical runtime success contract.

Census retains expected/seen/missing masks and timestamps immediately before snapshot acquisition/after enumeration. Only missing roles after a complete enumeration are correlated against borrowed original inventory handles, PID and creation. Wait0 distinguishes signaled versus live AFTER snapshot; it proves neither exit-before-snapshot nor authenticated presence from a PID bit. Query errors, identity divergence and invalid timing remain distinct. A1 freeze/Stop suppress new diagnostic queries. No OpenProcess-by-PID, no missing-role exception, no fifth role, no freshness reset, and the original ERROR_INVALID_DATA remains first.

Non-trace runtime uses the existing no-op successful diagnostic return and unchanged guards. The positive/read-only authority gates, RAW1/token predicates, consumption, SCM, quotas, launcher, transport and protected inputs were not relaxed or repinned.

## Builds and tests

Four product configurations Selected/Legacy × Debug/Release: PASS, zero warnings in final builds (`build-results.json`, four build logs; existing compiler/toolchain/targets preserved). The new shared header also compiles in four isolated configurations with /W4 /WX.

- 76 existing relevant regression executable outcomes: PASS, including actual runtime/file models, A1, self-token, census, BD, profiles, replay/freshness/Stop/UNKNOWN/retention and transport regressions. `regression-results.json` and logs.
- 96 token model outcomes: PASS (24 per configuration), including absent/disabled/DEFAULT+OWNER/deny-only/duplicate SID, filtered-history versus restricting-SID distinctions, native-width1/DWORD4, invalid width, bad bounds, identity/generation changes, query failure and Stop.
- 40 original-process correlation outcomes: PASS (10 per configuration), including live/signaled original handles, identity mismatch, query/wait failure, incomplete enumeration, timing and Stop.
- 32 actual-census-body model outcomes: PASS (8 per configuration), now asserting original PID/creation and live/signaled correlation output, without real SCM or LAB.
- 12 isolated native checks: PASS: four reads of the fixture's OWN token and eight original-process lifetime checks (own process and normally exiting inert fixture child). These are NOT service-token evidence or LAB admission.
- Actual file models assert raw metadata in both failure and successful preparation frames, original first-error preservation, q_error5/known3/complete0 on query failure, no readiness on incomplete successful-prepare observation, no repeat append, Stop, close failure, retained UNKNOWN and existing bounded file/receipt contracts.

Initial qualification exposed missing diagnostic type visibility in a non-trace test translation unit and a signed/unsigned assertion in the census fixture. Both were corrected; no warning suppression or gate weakening. Relevant final builds/tests passed. No native operational execution or service test was run.

## Ownership, memory and evidence limits

No additional acquired references or persistent owner fields. Compiled fixture types: PT_META32bytes and PT_ROLE56bytes. Runtime model still reports PS_DIAGNOSTIC4400, PS_RUNTIME12080, PS_NATIVE24992 and aggregate25208bytes. These are bounded own aggregates, NOT total module/process memory.

Token helper has a fixed512byte restricting-SID buffer and two local statistics snapshots; Selected Release emitted token-body frame reserves704bytes plus saved registers/callers. Census adds two64bit timestamps and one transient56byte role result; success formatter grows192→384bytes and uses224byte metadata scratch. This is not zero-cost storage, a whole-call-chain stack bound or ED11 credit. New code/read-only strings and transitive runtime/Windows cost are not represented by the unchanged owner aggregate. Fresh future stack/handle/memory gates remain mandatory.

The existing960byte payload/1024byte frame/2048byte file and exclusive one-result append remain unchanged. Metadata serializer uses bounded buffers; an unavailable suffix reports UNKNOWN_BOUNDS where it cannot fit. Full diagnostic completeness is required to use a future observation as token evidence. Sampled handle peaks and historical budgets are not reused; 128/1024/four roles/60s+1s/headroom/NONE_GRANTED remain unchanged.

## Historical disposition and remaining facts

N0START-03 remains consumed: starts1/stops0, qualifier first13, service specific-group predicate5, no valid NTSTATUS for that Boolean predicate. Original local closes and original service-process termination are evidenced within the archived receipt/code scope; global quiescence is UNPROVEN. Old service SID attributes and missing-role mask remain UNKNOWN because the original recorder did not archive them. No current operator/auxiliary/qualifier token substitutes for the terminated service token.

Canonical historical intent is the `.intent` in `build/scm-protected-installation-binding-20261022/Selected/Release/`, NOT the docs/manual-native-intent path in the old request. Q8 discrepancy remains recorded; old request/approval bytes are preserved. The legacy docs-path check remains unchanged and does not establish canonical consumed-intent absence. Future exact request must derive its intent from the accepted annex and canonical owner and test agreement.

Intent and sink content/RAW1 native readback still require authorized protected verification. Prior agent denied reads were not retried or bypassed. Historical image, sink, intent, dossier and SCM ImagePath are preserved; administrative shared lifetimes, opaque kernel/cache/stack residue and real retained reservations are not refunded or zeroed. Q2 cannot be applied while mandatory protected inventory/evidence is missing. P3 pre-Start is inapplicable to starts1.

## Hashes and pins

Selected Release qualifier candidate: `104bab97051a41e3a5718aeb5804f31076cf3aa07c5919a6727e08257cffc581`.

Selected Release service candidate: `89529bd869c8ca92a4b6d6c1816e7d67a2af8326a38401288d1a05aaabbe0ac7`.

Unchanged canonical launcher: `e817b72d792539199430ea9992d39478a6134664b9d5a6b8607fe3746d33e88d`.

Consumed protected image remains `44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c`; no overwrite. All four candidate pairs/source/test hashes are in artifact-hashes.json. Preimages and previous candidate binaries were saved. Derived-pins.CANDIDATE.json is NOT_ACTIVATED. The historical launcher's executable pins are intentionally not repointed to this diagnostic build, nor do compiled historical03 bindings authorize replay or a new identity.

## First pending gate and concrete next procedure

1. Complete an exactly reviewed, authorized native read-only inventory of original consumed intent/image/sink and their protected parent/RAW1, preserving raw descriptors/native identities/hashes and checked close. Existing root verification alone is insufficient. No ACL repair, adoption, Place, overwrite, retry or service action. NEXT-OBSERVATION.REQUIREMENTS.json is deliberately nonexecutable: unknown script/native facts are not invented.
2. Review Q2's exact evidence package and obtain separate documentary application approval; acceptance here did not apply the marker or release any physical resource.
3. For the missing live service-token/census facts, an ordinary read-only query cannot recover the already terminated token. The owner must select/authorize a NEW identity and approve the exact Q4 annex before new-scope binding/dossier/N7/budget and a separate one-shot diagnostic operational request. No automatic04, no consumed03 reuse, no positive executable command. The newly qualified mechanism is ready to bind when that authority exists.

Success criteria for that eventual observation: exact service SID count/attributes and restriction fields with complete generation checks, original-role masks/identity/snapshot/wait timing, independent fresh ED11 budgets and factual close/retention. It may legitimately refuse before RUNNING; no retry until success is authorized. Known controlled pending/UNKNOWN, incompatible reservations, identity/protection divergence, missing budget or failed gate means DENY. One future diagnostic operation is not admitted by this document.

ED11 matrix and capability live documentation were updated; future recovery directions remain deferred and convey no cleanup authority. E16 remains PARTIAL and actual complete running-service metrics/Stop remain unproved. Phase6 is not closed by this integration.

CURRENT_TASK_COMPLETED=YES (authorized normative registration and technical integration/qualification)
Q0_Q9=ACCEPTED
DIAGNOSTIC_INTEGRATION=QUALIFIED_OFFLINE_NOT_BOUND_TO_NEW_IDENTITY
ADMINISTRATIVE_TRANSITION_APPLIED=NO
HISTORICAL_TOKEN_AND_MISSING_ROLE=UNKNOWN
UNKNOWN_CHARGES=RETAINED
NEW_IDENTITY=NOT_CREATED
OPERATIONAL_PINS=NOT_ACTIVATED
START_STOP_EXECUTED=NO_THIS_TASK
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
REFUND_AUTHORIZED=NO
SUCCESSOR_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=PROTECTED_HISTORICAL_INVENTORY_AND_Q2_Q4_EXACT_OWNER_REVIEW
