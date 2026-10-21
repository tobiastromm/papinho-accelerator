# CNG diagnostic safety and portability correction — closeout

TASK=PACC-PHASE6-CNG-DIAGNOSTIC-SAFETY-AND-PORTABILITY-CORRECTION-1.
2026-10-19. main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a; baseline514 existing WIP entries. Sources and live-doc preimages preserved before modifications. All submitted writes/builds/offline executions officially approved; no denied action bypass. First preparation stopped on LF/CRLF hunk mismatch before source writes, then exact hunks were resubmitted. No accepted document changed.

## Delivered

- Existing native mutation fixture removed from tests/ca_key_offline_test.c. --native-key now denies before any key operation; old historical executables preserved and must not be run as substitutes.
- tests/ca_key_readonly_diagnostic_win32.c: exact explicit authority/generation/full name/operator/service arguments, effective token check through existing backend, local cancellation, first and final ownership receipts, no mutation or automatic identity selection.
- apps/server/server_ca_key_win32.c: PAPACC_CA_READ_ONLY_DIAGNOSTIC excludes native create/set/finalize path and creation-specific cleanup; explicit create call fails closed before native use. Default product branch preserves operational predicates, flags, call order, DACL/export policy and error behavior. No producer/backend/owner duplication.
- tests/ca_readonly_diagnostic_test.c plus core header-isolation smoke; docs/portability.md and capability live note updated. Full identity, reachability, coverage, ED11 and future invocation gate in safety-identity-portability-handoff.md.

## Validation and limits

Final build3 Debug/Release focal logs: /W4 /WX, zero warnings, complete linkage for four focal executables per configuration. Final test logs:39 read-only assertions +6 argument negatives +598 original synthetic assertions/19 differential preimage cases per configuration, and removed native switch rejection in both executables. Retention/close failures are simulated, not native CNG proof. Synthetic binary imports no NCrypt. Read-only binary imports no create/set/finalize/delete or RNG; only public export call exists in source, fixed BCRYPT_RSAPUBLIC_BLOB. No dynamically selected export type.

Portable-build2 Debug/Release plus executed smoke pass with no Win32/CNG API headers in include trace. Real MSVC x64 target remains; this is header isolation, not box/ISA cross-build or complete common issuer API. Initial portable experiment removing platform macros failed in CRT, and first synthetic build had variable shadow warning; logs retained, both corrected without suppressing warnings or altering product toolchain.

ED11: owner remains472bytes/56byte receipt, no added product/root storage. Standalone read-only harness owns one static owner, Stop flag,32byte IDs plus existing validation scratch and two temporary SID buffers. Failure retention remains charged/UNKNOWN; no full stack/provider/kernel census or budget approval claimed. No quota change. Full portable issuer contract is normative in accepted decisions; full shared issuer API/serial/live registry remains pending, no Windows leakage into existing core found.

Preservation: no Browser/PST/wire/production pin/SDK changes, no clone/worktree/clean/commit/network or native key operation. Previous diagnostic key/executable/log are preserved; old key state remains UNKNOWN. Sources changed only in canonical Accelerator. Product startup/authority/admission unchanged.

```ini
CURRENT_TASK_COMPLETED=YES
DIAGNOSTIC_PRIVATE_EXPORT=UNREACHABLE
DIAGNOSTIC_KEY_DELETE=UNREACHABLE
DIAGNOSTIC_KEY_CREATE=NOT_EXECUTED
DIAGNOSTIC_CREATE_ENTRYPOINT=DENY_NOT_SUPPORTED
DIAGNOSTIC_DACL_AND_EXPORT_POLICY_WRITES=COMPILED_OUT
KEY_STATE=UNKNOWN
EXACT_DIAGNOSTIC_IDENTITY=CONTRACT_IMPLEMENTED_VALUES_NOT_SELECTED
ADMIN_OPERATOR_CONTEXT=EXPLICIT_SID_AND_EFFECTIVE_TOKEN_GUARD_FUTURE_APPOINTMENT_PENDING
CNG_BACKEND_ISOLATION=PRIVATE_WIN32_ADAPTER
PORTABLE_ISSUER_CONTRACT=NORMATIVE_BOUNDARY_PRESERVED_FULL_COMMON_API_PENDING
BOX_ISA_COMPATIBILITY=NOT_QUALIFIED
BUILDS=FOCAL_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=PASS_SYNTHETIC_AND_HEADER_ISOLATION
NETWORK_TESTS=NOT_RUN
REAL_CA_CREATED=NO
REAL_CERTIFICATES_ISSUED=NO
KEY_CREATED=NO
KEY_DELETED=NO
ACL_CHANGED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_PREIMAGES_AND_HISTORICAL_OUTPUTS_PRESERVED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=HISTORICAL_CREATE_FAILURE_NOT_REPRODUCIBLE_UNDER_NO_MUTATION_DIAGNOSTIC_SCOPE
NEXT=OWNER_REVIEW_OF_SAFE_NATIVE_CNG_DIAGNOSTIC
```

Future native read-only invocation still requires exact identity/context and official approval. It does not authorize touching the historical UNKNOWN key. Diagnosing creation requires a separate exact decision for new-key configuration effects; do not silently enable create, grant production authority, or return to old fixture.