# Personal first-install CA custody binding — 2026-10-22

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-CA-CUSTODY-BINDING-1. The independent private binding implementation and offline qualification are complete. End-to-end factual personal first-install custody is NOT completed: the CLI still requests no CA_INITIALIZE, the shared native effect-admissibility producer still returns false, and no productive CA owner/reference has been initialized for that invocation. These guards were not weakened to substitute LAB evidence for authority. CURRENT_TASK_COMPLETED=NO refers to this remaining end-to-end objective, not failure of the completed independent tests.

## Verified LAB evidence, without reuse

The owner's prior elevated run has confirmed Create, all four required properties, Finalize and public validation for the exact new LAB identity `PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0`, Microsoft Software KSP/MACHINE. The active-borrow close denial was expected; after return, handles closed factually. The key remains persisted LAB-only, not deleted. Original earlier LAB disposition remains UNKNOWN; the historical NTE_PERM cause is not proved by the later success. The [verified manual result](../ca-lab-finalization-launcher-completion-20261022/manual-elevated-result-20261022.md) and receipts remain unchanged. This task did not open/query/borrow/create/delete either LAB object. Successful LAB export-policy0/usage/DACL checks are not full productive custody qualification, private export testing or authority.

## Implemented association and lifetime

The same canonical pending packet now has one borrowed CA-owner pointer plus a monotonic used marker (16 aligned bytes on x64). The actual PAPACC_CA_KEY owner remains caller-owned durable storage with the existing thread-affinity/handle/retained resources and single borrower; it must outlive this borrow. No owner copy, second manager, registry/journal/worker/root/pin table, allocator or generic cryptographic API is added.

The private match_data API compares the consumed effect plan, attempt and handoff attempt, operator bytes, protected-parent tuple, exact relative child name, CA authority/generation-to-name association, service bytes and frozen original configuration. This is DATA comparison, not factual custody. It rejects both known LAB key identities, missing handles/created/finalization metadata, wrong thread, Stop/retention and divergent fields. Names and READY metadata alone are never enough for native binding.

Native bind additionally requires the existing native ceremony gate, revalidation of the held parent/token/source and actual exclusive child identity, public CA validation by the unchanged canonical KSP owner, then revalidation again before the existing papacc_ca_key_borrow. It performs NO create/open/delete/private-export/signature operation. Borrow acquisition is one-shot; return does not rearm the marker. The current native ceremony gate is still closed, so no real borrow is acquired in this task. A caller cannot use a model DATA PASS or the LAB receipt to bypass it.

The pointer protects references to the same owner and packet; pending/finish, native child/parent close and DATA retirement deny while it is held. Future preparation must use the private ca_current revalidator with the exact attempt/IDs, then retain immutable state through any legitimate consumer. Release uses only the existing key return and requires worker inactive/thread absent (signal AND close already observed). Stop can invalidate new use while retaining the borrow; return is permitted after Stop only at quiescence. Wrong/retained CA state or return failure keeps the pointer and marks the canonical root unresolved. Return does not close the key owner, delete persistence or grant authority: its producer still owns and must close/account its resources factually.

These are purpose-specific interfaces on the existing owner, not a productive initializer. Their required caller contract is a factual CA_INITIALIZE producer bound to the same live consumed effectful attempt and exclusive target. Such a producer/reference is not supplied by the current read-only CLI. No arbitrary textual key name is adopted. No operational root-preparation/publication entry was enabled; future root preparation must continue to deny without that producer and resource qualification.

## Tests and build defect resolved

Final Legacy and explicitly Selected Windows3MiB MSVC19.51 x64 Debug/Release builds pass /W4 /WX without warnings, including the server. Nine focused suites per build (36 runs) pass. Native-negative tests execute the real denied guard path; models compile only the current-context/file/CA-validation observations to stubs and exercise the SAME binding code plus real metadata borrow/return/borrow-close guard. No fake handle reaches native provider/free calls. Model SIDs/handles/IDs are visibly DATA, not factual authorization or cryptographic fixtures.

Coverage: valid modeled reference; absent/divergent CA, service/operator/name/configuration/parent/attempt; LAB-identity rejection; missing provider; target mismatch; native validation failure; existing active borrower/second bind; Stop; denial of target/plan retirement while borrowed; active-worker release denial; return after Stop; no rebind/double return; retained failed return; native operation remains NOT_SUPPORTED despite a matching model. Existing effect/read-only confirmation/replay/Stop, scratch canary, memory census and issuer authority-denial regressions pass. Actual eligible productive binding, CA creation, real target creation, native CA reopen and root preparation are NOT tested or claimed here.

An initial test-model global-name shadow warning was fixed without suppressions. An incremental regression then demonstrated stale header layout: owner object expected DATA offset0x9150 while an unrecompiled test still used0x9148. Existing CMake prefix `Observa??o: incluindo arquivo:` did not match UTF-8 compiler output. The minimal CMake prefix correction to `Observação: incluindo arquivo:` restored Ninja header dependency recording. Only the four new build directories were cleaned/rebuilt. All regressions pass on the consistent layout, and archived Ninja dependency output confirms the actual pending header is now tracked. Toolchain/backend/locale policy were not changed; no historical build was cleaned.

## ED11 reconciliation

Compiled packet/root footprint increased16 bytes (borrow pointer, monotonic marker and alignment). Current known C8 composition: Legacy1799624/2097152; Selected1965120/3145728. Nominal remainders297528 and1180608 are arithmetic ONLY, not operational margin. One existing64KiB worker and8KiB scratch reservation are shared as before, not charged twice. CA owner storage480 bytes is separate caller-owned storage; if not already reserved elsewhere, it must be charged ONCE additionally (lower-bound scenarios1800104 and1965600). This does not establish a complete peak.

Observed own-function stack adjustments excluding pushed registers/return addresses/callees: match_data Debug608/Release400; bind/current64/48; release48/32 bytes. Source/target revalidation, CA public-validation512-byte scratch, ACL/token/SID allocations, CRT, handles, provider/kernel/isolation, retained objects and full nested stack/host admission still require transitive accounting. No UNKNOWN is zero. Previous UNKNOWN namespace debt and the persisted successful LAB key are distinct retained resources. No quota, profile transfer, new thread/concurrency pool or ED11 PASS. ED11 PARTIAL; dependent operational effects remain denied.

## Governance and remaining boundary

main HEAD913c90cf2328f7939d8f5b927640601903ac6e91 unchanged. Exact source hunks and fixes were officially reviewed before application; no rejected action bypassed. Source pre/post hashes and build/test evidence archived. Existing unrelated WIP, accepted ADRs, manual receipts, LAB executables and four historical LABs preserved. No PST, Browser, wire, production0.6.4, clone/worktree, commit/push/tag/release, key operation, operational network, bootstrap/certificate/CURRENT. Relevant future/capability relationships were searched; no CA-specific future direction is promoted to authority or changed.

First unresolved boundary: current config first-install is read-only and supplies neither an effectful factual CA_INITIALIZE producer nor its protected owner/reference. Accepted F0–F9/F7 and issuer clause6 require that distinct authorization and factual association; LAB success cannot supply them. The concrete integration to enable later is: effectful exact-plan confirmation/one consumption, native exclusive target and quiescent handoff, same-invocation CA initializer/reference producer, this guarded borrow/current interface, then initial root preparation. No creation is authorized by this handoff.

CURRENT_TASK_COMPLETED=NO
CA_CUSTODY_BINDING=PRIVATE_GUARDED_INTERFACE_IMPLEMENTED_OFFLINE_QUALIFIED
NATIVE_KSP_REFERENCE=NOT_ACQUIRED_FACTUAL_PRODUCER_ABSENT
PERSONAL_CEREMONY_INTEGRATION=DATA_ASSOCIATION_AND_LIFETIME_COMPLETE_NATIVE_EFFECT_GATE_DENY
CA_PRIVATE_KEY_PROTECTION=PRIOR_LAB_PROPERTIES_VERIFIED_PRODUCT_NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=LEGACY_SELECTED_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=36_OF_36_PASS_INCLUDING_NATIVE_NEGATIVES
NATIVE_TESTS=DENIED_PATHS_AND_READ_ONLY_PREFLIGHT_PASS_NO_KSP
NETWORK_TESTS=NOT_RUN
NEW_KEY_GENERATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
LAB_KEY_PRESERVED=YES
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=EFFECTFUL_CEREMONY_AND_SAME_INVOCATION_FACTUAL_CA_INITIALIZER_REFERENCE_NOT_SUPPLIED
NEXT=PERSONAL_FIRST_INSTALL_INITIAL_ROOT_PREPARATION

NEXT permits independent DATA preparation only until the stated factual prerequisites qualify; it is not an operational readiness assertion.
