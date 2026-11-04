# ED11 full closure drive — consolidated review and decisions

TASK PACC-PHASE6-ED11-FINAL-CLOSURE-EXECUTION incorporates the preceding SERVICE-IDENTITY-Q6A3-AND-Q4-DISPOSITION work. Canonical project C:/Projetos/PapinhoAccelerator, active main/HEADf341985924d29870d8ff627ed37c8a0874ad6469, previous WIP and protected histories preserved. ED11 final closure is NOT ACHIEVED because the precise normative/operational dependencies below are not authorized or demonstrated. All useful currently authorized isolated qualification and package preparation described here is complete.

## Complete matrix, no repeated closed gate

CURRENT-ED11-22-GATE-MATRIX.csv is the compact current E01–E22 view; the canonical ED11_GATE_MATRIX.csv E01–E22 rows now agree. Its supplemental Q4/Q7 rows remain historical chronology, not competing current statuses. Prior matrix bytes retained in preimages.

Classification:4 PASS demonstrado (E01/E05/E09/E14),12 PASS limitado ao escopo observado (E02/E03/E04/E06/E07/E08/E10/E11/E12/E13/E15/E16),2 pendentes de decisão normativa (E17/E21),4 pendentes de observação operacional (E18/E19/E20/E22, with E22 derived). No gate is marked non-applicable or erased to produce PASS. A mechanism PASS never certifies all actual runtime uses. No new production implementation defect was demonstrated that would justify silently changing Q6/A3.

Stale matrix gaps saying Q4 was NOT_ACTIVATED, service SID attributes were unobserved, and positive effects NOT_RUN were corrected in the current rows. The old token attributes remain historically UNKNOWN; new Q4 evidence is distinct. Provisioning failure is NOT_DEMONSTRATED, identity mismatch NOT_OBSERVED, normative contract mismatch DEMONSTRATED, and full service resource/lifecycle qualification remains PENDING.

## Exact identity contract and diagnosis

Expected specific SID S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451 is defined by the virtual account NT SERVICE/PapinhoAcceleratorLab20261023, resolved by LookupAccountNameW into native.service_sid in apps/server/server_service_virtual_win32.inc and constrained to S-1-5-80 namespace. The compile-time account/service literals live in server_service_identity_win32.h; N0 pins and evidence bind the exact resolved SID. This is not a SAM local group. Generic SERVICE S-1-5-6 and BUILTIN Administrators S-1-5-32-544 are different SIDs built via CreateWellKnownSid.

The SCM adapter separately creates/configures the virtual logon account, SID typeUNRESTRICTED, own-process/start/ImagePath and readbacks. psr_token in server_service_runtime_win32.inc already requires original primary TokenUser equality with native.service_sid, then scans complete groups for an ENABLED/non-deny-only entry of that same SID. Thus the runtime imposes BOTH user equality and a second group grant. ENABLED is meaningful for group access eligibility; DEFAULT/OWNER flags are not substitutes. This duplicate group requirement is a concrete private runtime guard; V2 explicitly distinguishes configured account, SID type, actual TokenUser and group attributes and allows equal SID bytes. Q6/A3 deliberately freezes the guard for the completed diagnostic, so removing it is a normative boundary, not a harmless pin correction.

The real Q4 token passed primary TokenUser equality but had zero matching group entries, q_error0/q_known15/q_complete1, filtered-history0/width1, restricting count0. Original service process22620/creation134374440955188877 ended. Service rejection5 and qualifier first13 are preserved. This proves the immediate incompatibility with the current group test, not a universal OS reason for omitting the duplicate or all service privileges. Existing code/configuration matches selected provisioning; current read-only qc/qsidtype/showsid/queryex agree on exact account/SID/UNRESTRICTED/DEMAND and preserved10584 ImagePath, STOPPED/PID0/Win325. No remediation or provisioning mutation is justified.

GOVERNANCE CONFLICT: integrating a relaxed group rule before an explicit Q6/A3 amendment would contradict accepted authority. Therefore no product security predicate, launcher, operational pin or accepted ADR was changed. R0–R9 candidate proposes an explicit narrow prospective rule, not retrospective reinterpretation: primary virtual-user identity AND complete group/restriction/continuity/security evidence AND effective kernel membership/resource access; absent duplicate permitted only under those conditions. Present disabled/default-only/deny-only/duplicate/contradictory entry, filtered/restricted token or any unknown proof remains DENY. Default/non-LAB behavior remains unchanged. Candidate acceptance alone authorizes no new operation.

## Qualification and concrete corrections

1. Four Selected/Legacy Debug/Release isolated candidate builds: PASS, zero warnings. Each includes the actual unchanged runtime caller plus prior component-model checks,9 actual psr_token mode invocations and46 additional assertions. Total184 additional checks; actual guard still rejects the observed no-group shape. Candidate semantics explicitly deny wrong/missing user/query/SID type/restriction/access/Stop/close facts. These are simulated API facts, not service-native proof.
2. Four matching builds of own-token/in-memory descriptor native fixtures: PASS, zero warnings;32 checks each (128), original own primary-token/duplicate membership, stable identity, read/execute allow and write-data/WRITE_DAC denial when the operational user is not descriptor owner; checked closure. Separate self-owner case intentionally confirms implicit WRITE_DAC owner rights. Initial mistaken expectation that an owner deny ACE alone guarantees no WRITE_DAC was corrected in the fixture; original failed observation retained. This protects the proposed access requirement from an actual security mistake. It is not access evidence for the terminated service token.
3. Reused qualified original-parent/read-only machinery to prepare a new Q4 effects reader, with exact current attempt/sinkID/payload/size, source image and protected input pins. CompileOnly with C# warnings-as-errors PASS,30 model cases PASS (replacement/reparse/link/hash/ACL/replay/Stop/freeze/uncertain close/no retry/first error), native synthetic parent/file sharing/absence/Stop/same-handle revalidation/close suite PASS, wrong script hash DENY before compile/read. No Verify/protected history read was executed. Fixture-generated files remain synthetic and preserved.
4. Candidate script BOM decoding and content pins were corrected during isolated qualification; actual file bytes, not logical newline strings, determine final hashes. No old script or operational record was edited. Synthetic model frames were reconstructed from the archived642-byte payload for tests only; they are explicitly NOT original protected sink bytes.
5. No product rebuild or broad unaffected regression repetition: product C, frozen qualifier/service image, launcher and pins are unchanged. Existing four qualified product builds and applicable prior regressions are reused within their prior scopes. No compiler/backend/target/quotas changed. New candidate storage is not a zero-cost admission credit and still needs compiled product/frame accounting before future integration.

## Q4 post-Start disposition, separately governed

Intent consumed, image/sink created, explicit rebind and Start1, Stop0. Original-process termination and local close confirmed; global quiescence UNPROVEN. Sink ID417614100:10354688:7367; initial1024bytes; extracted result kind1 length642/ID_MATCH1. Original intent/image creation IDs were never printed and remain UNKNOWN, not inferred from hashes. New file/readback observations cannot retroactively fill them. Current SCM ImagePath is the consumed Q4 image; it is preserved.

Readback request and script are technically prepared but unapproved and unexecuted. They perform no preflight, admission, Start/Stop, SCM/ACL mutation, new intent/image/sink/dossier, cleanup, refund or transition. Current original-object content/RAW1/native continuity must still be verified under an authorized elevated context. R6–R8 proposes a documentary-only local disposition for THIS consumed Q4 identity, applying the same nonwaivable controlled-reference/borrows/pending-work/process-termination/readback evidence obligations as historical03 Q2. It is not an automatic Q2 application or P3 pre-Start reuse. Normative acceptance and actual documentary application remain separate; no application is performed now.

Keep every consumed object, real unavailable reserve and UNKNOWN charge. Opaque Windows/cache/kernel residue and global quiescence remain explicitly unresolved as already permitted within exact historical residual scopes, not newly required universal closure proofs. Controlled UNKNOWN cannot be waived or relabeled opaque. Neither the disposition nor R0–R9 grants residual coverage for another attempt.

## Resource evidence and remaining operational gates

Q4 fully paired administrative samples997/999 of1024 and qualifier118/120/121 of128 remain historical samples. Last operator732 lacks a fresh auxiliary pair; no exact final administrative total or universal peak is fabricated. Host pre/final physical/commit observations passed5/9GiB. Actual running service metrics, four-role live composition, effective resource access after readiness, cycle_live, safe accepted/completed Stop and complete native final closure are still missing. A new approved independent positive lifecycle can address those ONLY after valid identity policy, consumed-effect disposition, new scope and compatible obligations. Q4 is exhausted; no retry or new identity generated.

E19/E20 separately require the real target's total/baseline/increment/shared/retained/provider/kernel accounting and actual Browser/C2/C3 composition. Historical OpenSSL/C1/model/component evidence is not that target. H/Q opaque historical coverage does not waive general target hard categories or qualify an unknown live provider. A future target plan must preserve applicable limits/topology and cannot add a fifth participant under the Q7 four-role scope. If an actually required hard category cannot be supported, the honest outcomes are STOP or a separately approved exact empirical policy; never guessed ceilings. One more service Start cannot close full ED11.

## Single critical path and reserved decisions

See MINIMUM-CRITICAL-PATH.md. Two decisions are concrete now: exact R0–R9 normative candidate (or retain existing DENY), and independently one read-only protected Q4 effects collection. They can be named separately in one response; one does not imply the other. After facts, a documentary application needs its exact owner decision. Functional integration then uses the accepted rule and existing technical authority; a future independent lifecycle still needs fresh identity/binding/N7/budget/operational approval. Full-target E19/E20 qualification requires its own factual topology/artifacts and authority; not a reused Q4 approval. E22 is derived from all applicable evidence and requires no extra closure-only experiment.

Phase6 separation remains: resource closure would not prove ED08/Model E upstream classes, full protocol/provider/Browser behavior, CA/custody/provisioning/CURRENT, revocation/end-to-end Stop or productive publication. Deferred personal backup/recovery is not introduced as a new MVP blocker. CURRENT/signature/recovery stay DENY. No accepted decision or historical evidence is rewritten.

## Primary external references and their limits

- https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_sid_info — SID type controls service-token SID configuration; account names use NT SERVICE/name; documented UNRESTRICTED flags do not prove this original token's group list.
- https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_user — user SID is distinct from TokenGroups; group ENABLED flags must not be invented for TokenUser.
- https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_groups — group flags ENABLED/DEFAULT/OWNER/DENY_ONLY have different meanings.
- https://learn.microsoft.com/en-us/windows/win32/secauthz/sid-attributes-in-an-access-token — access eligibility and deny-only rules; semantics are not factual evidence about a terminated token.

These Microsoft definitions support the distinction; the claimed Q4 token shape comes only from the archived native observation and linked code.

ED11_FINAL_CLOSURE=NOT_ACHIEVED
TECHNICAL_PREPARATION=QUALIFIED_ISOLATED
NORMATIVE_RULE=PROPOSED_NOT_ACCEPTED
Q4_DISPOSITION=PROPOSED_NOT_APPLIED
PROTECTED_READBACK=PREPARED_NOT_EXECUTED
NEW_OPERATION=NOT_CREATED
START_STOP_EXECUTED_BY_AGENT=NO
SCM_ACL_MUTATIONS=NONE
REFUND_CREDITS=NONE_GRANTED
UNKNOWN=RETAINED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=Q6_A3_RULE_CHANGE_NEEDS_EXPLICIT_NORMATIVE_DECISION

Final candidate SHA256: `d19ad5922874569af416baab7e86b196b916eaead057418a97dd4226bfc8d9ef`. Readback request SHA256: `2aa9e0b81352b0efe28e23135fa4644b70d17241565e1fb6e6a6d191312c663b`. Exact final artifacts are listed in FINAL-HASHES.json.

## Current execution-context limit

CURRENT-TOOL-CONTEXT.json confirms the tool process runs as a different SID from the fixed authorized operator, with Administrators not enabled and PowerShell Core. This is not the qualified elevated Windows PowerShell5.1 context. No protected Verify was attempted, no elevation requested and no ACL/UAC workaround used. Isolated own-token fixtures concern this test process only. The exact protected readback therefore needs the approved operator manual context; technical autonomy cannot manufacture that identity.
