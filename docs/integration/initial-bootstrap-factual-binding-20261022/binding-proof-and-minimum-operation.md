# Initial bootstrap: factual binding boundary and STOP

Task PACC-PHASE6-INITIAL-BOOTSTRAP-FACTUAL-BINDING-1; 2026-10-22.
Active main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.
No functional patch applied. Source preimages and final verification are adjacent.

## Concrete native sources

Operator: server_configuration_bootstrap_win32.inc/bootstrap_admin rejects thread impersonation and checks enabled local Administrators membership. This is role/context eligibility, not approval for an arbitrary target. bootstrap_commit_win32.inc/bootstrap_auth_native queries the actual process TokenUser and TokenStatistics, captures SID and TokenId/AuthenticationId/ModifiedId, then compares them during revalidation. It does not query TokenElevation, TokenElevationType, complete privileges or the full context required by the existing qualification handoff. No unobserved elevation/context is inferred. These functions are private/static to the existing canonical commit unit, not callable CLI utilities.

Target: bootstrap_worker observes the native parent and the new container via retained handles, nonreparse checks and GetFileInformationByHandle volume/file-index identity. It requires FILE_CREATE/io.Information==2 for the NEW container, same invocation and custody. bootstrap_auth_capture runs after that creation and binds root/domain and container identity into BOOTSTRAP_AUTH_PROOF in the SAME packet scratch. Commit scope adds candidate digest and revalidates before effect. A hostname, spelling, absent path, root digest or operator SID alone cannot substitute. The target of this proof is the administrative store container, not the host display name.

Entry: main.c.api3.inc calls the existing config CLI before normal runtime acquisition. first-install currently returns DENY without native owner/custody acquisition. Its arguments are only synchronous input borrows. Custody_begin starts packet9 and calls the worker that creates the container; using it to obtain a positive identity now would perform part of the ceremony. The existing token/target capture cannot simply be called from the CLI without the same protected owner/invocation and disposition. No stand-alone scratch/handle manager, exported static arithmetic or alternate identity producer is added.

## CA custody is a different resource

server_ca_key_win32.h/.c provide PAPACC_CA_KEY, ca_identity, ca_begin and papacc_ca_key_create/open/validate/borrow/return/close. ca_identity checks the actual effective token against the supplied configured operator SID; the configured service SID must differ. Unlike bootstrap_admin, this adapter may use a thread token, so the two eligibility models must not be equated by copying a SID. The create/open functions also require nonzero authority and generation identifiers, derive the private key name, acquire the KSP, and verify key properties and ACL. Possession of a matching token does not bind a NEW_INSTALL act or authorize initial CA generation.

The key owner tracks provider/key, retained token/local allocations, original errors and one borrower. Close rejects active borrow, failed NCryptFreeObject retains ownership, and unknown cleanup is not a successful release. The CNG issuer backend additionally consumes root/administrative authorize/UTC ports and validates factual records/reservation before signing. None of these callbacks is an authorization source merely because it exists.

Accepted internal-client-issuer/manual-custody clauses1/6 require separate explicit authenticated CA initialization. No configured authority/generation/operator/service association, protected key, corresponding CA certificate or qualified custody source has been supplied for this operation. Opening a guessed named key or generating one would not qualify that missing source. No CA operation was attempted. The initial root v1 no-issuer envelope remains distinct from CA initialization and later UPDATE versions; no prior CURRENT is demanded for initial bootstrap.

## First missing binding and minimum operation

Accepted ceremony P1/P2/P3 requires the designated NEW-installation declaration attributed to the actual authorized actor, exact governed parent and new container under continuous custody. The CLI has no such factual operation context today. The owner already confirmed absence of a qualified installation; no substitute is created.

The minimum next implementation/qualification is to carry the explicit declaration and configuration into the SAME canonical owner/invocation, qualify the exact effective operator context and governed parent rights, and retain/revalidate those references across the existing packet9 custody phase. No candidate revision, UTC validity, lease, root identity or approval may be invented to fill the gap. Actual new-container identity must come from the separately authorized exclusive-creation operation, not a read-only path probe. Effects require complete ED11 and exact execution authorization; this task does not run that operation. The future distinct CA-init act must bind actual operator/service and authority/generation to the existing key owner before any create/open, without regenerating on restart or restoring authority from historical bytes.

This is an implementation/qualified-input gap, not a need for another CLI, registry, journal, root, resolver or generic authorization framework. No new normative contract is declared accepted. No positive binding is implemented because its required observations are not available within this nonexecuting scope.

## New independent validation

Read target-specific existing build instructions and inspected test bodies before execution. Actual MSVC Debug/Release server and eight selected targets built successfully, zero compiler warnings emitted. Eight suites each passed: issuer.production_gate, issuer.cng_production_gate, config_binding.model/native_guard, phase6.memory_census, store_hash.lifetime/owner, bootstrap.scratch. The newly built issuer/CNG tests reject synthetic authority and absent live root, verify reservation guards, and use MODEL key handles only in borrow/return guards that perform NO native key operation. No fixture is promoted to authority.

Thirty real candidate CLI negative invocations also pass (15 per configuration), including declared input, missing declaration/source/parent, changed target, replay, purported operator/authorization flags, bounds, controls and unchanged apply denial. These are syntactic/model and negative guard results, not a native identity or token-elevation qualification. Native elevated/non-elevated operator variants, real target identity/mismatch, CA custody and positive first-install binding remain NOT_RUN/NOT_QUALIFIED. Existing Stop and hash-cleanup retention tests are separate regression scopes, not evidence that a CA or container was acquired and released here.

## ED11

No product layout, ownership, buffer, heap, native handle or runtime acquisition was added. Existing objects must still be charged for a future operation: token handles/query buffers, BOOTSTRAP_AUTH_PROOF in packet scratch (do not double-count its backing), retained parent/container and worker, source/configuration references, key/provider/retained-token/local allocations, CA workspace, nested stack and CRT/host costs. Handle cardinality is not an opaque byte bound; failed closure remains charged until factual disposition.

Existing Legacy build profile is used (2097152 managed limit; Selected 3145728 only when explicitly selected). Latest known fixed ledger remains Legacy1790952/Selected1956448, not a complete peak. Actual source/key-custody graph, native/provider/kernel, complete nested stack, retained/concurrent credits, runtime and host admission attribution remain UNKNOWN. No reserve is guessed, no timeout releases a resource, no margin is authorization. ED11 remains PARTIAL, bootstrap and all operational authority DENY.
