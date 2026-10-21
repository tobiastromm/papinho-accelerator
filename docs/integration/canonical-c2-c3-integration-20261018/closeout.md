# Canonical C2/C3 integration — independent SDK slice and factual stop

Task: PAPINHOACCELERATOR-PHASE6-CANONICAL-C2-C3-MODEL-E-INTEGRATION-1.
Date: 2026-10-18.
CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES

## Completed source slice

Canonical main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a.
CMakeLists.txt adds the default-OFF PAPACC_ENABLE_CANONICAL_API3_CANDIDATE
selection and a standalone offline SDK ABI target. The opt-in uses /MD in
both configurations. cmake/PapaccPstConsumer.cmake now selects pinned normal
API3/SPI4 x64/MSVC/OpenSSL archive, manifest, header and library identities
when that option is enabled. The previous default production and API2
qualification selections retain their original values. TEST_ONLY mixing is
rejected in the new candidate selection; production pin remains0.6.4.

New tests/pst_api3_offline_contract_test.c checks the actual normal library:
28-byte release-info layout/result offset, initialization/API/disposition NONE,
and rejection of try_release(NULL, info). It creates no PST runtime,
Connection, wait-set, socket, listener or operation. It does not emulate
DESTROYED or prove a native release, owner retention or ED08.

Debug and Release configured, compiled and linked with MSVC19.51 x64,
/MD and /W4 /WX. Each executed only papacc.pst.api3_offline_contract:
1/1 PASS in Debug and1/1 PASS in Release, zero compiler/linker warnings.
Builds are under build/canonical-c2-c3-api3-sdk-20261018, with fresh outputs.
The API3 application/listener/owner was NOT built or qualified. Remaining
canonical API2 release calls were not replaced. No C2/C3/Model E functional
source was copied or modified. No C8/E10 source or diagnostics were imported.

SDK root: build/ed08-r4-integrated-20261012/sdk-normal.
Normal archive SHA256:
79E53A2D47AD6D509EC893BD1C6D9B8C26B98C5A8A9ECD75E4ADB33A240B132F.
Manifest SHA256:
03130F62C40B0742B83A8AF636112349A51CF1B1D949F6F8EB18EF2BAD1877CB.
Source commit4dbd2783ebaf135114da2b7247e36c0ad6bc550a, API3.0.0/SPI4.0.
No Browser x86/NSS SDK or R2 package was selected.

## First unresolved dependency / STOP

The historical non-C8 papacc_server_c2_owner_win32_adopt_session uses
session-array ordinal+1, epoch1 and a local offer/result factory.
The historical S6 c2_site_policy admits the fixed localhost fixture.
Those paths cannot satisfy this task's requirements for factual complete
identities, current policy/epoch and independent Site TLS authorization.

The newer factual path is server_c2_root_binding_win32.inc:
papacc_server_launcher_c2_capability_policy and
papacc_server_launcher_c2_adopt. It consumes the committed root, authenticated
CONTROL, immutable real grant snapshot, policy CURRENT, root domain/revision,
invocation and issued epoch. It depends on root.listener_composition,
configuration_c2_owner, administrative policy evaluation and preparation/root
authority maintained by the historical C8 composition. These producers are
not present in the canonical listener composition.

Accordingly, neither historical adoption branch can be transplanted as-is:
one would import precisely the fixture authority excluded by this task;
the other is not an independently linkable component outside its root graph.
This is a source/provenance dependency, not a new wire incompatibility,
executor denial, or a claim that a new normative decision is necessarily
required. No fake epoch/Principal/grant or permissive adapter was implemented.

Owner task section6 prohibits localhost permission/epoch fixture authority;
section7 keeps admission closed; section12 requires stopping at fictitious
authority or unsafe lifecycle. This slice stops before dependent owner wiring.
C2 capability/dispatch, OPEN/BIND, flows, Site TLS, FINAL and real-owner Model E
remain NOT_INTEGRATED. A zero-warning SDK probe is not their qualification.

## Minimum remaining integration

Reuse the existing listener and historical components. First isolate the
factual root-bound adoption/policy/epoch/Site TLS dependencies from the
historical root graph, preserving existing accepted semantics and explicit
absence/denial. Determine their exact canonical producers and lifetime;
do not manufacture operational readiness or build a parallel authority.
A closed boundary may represent missing facts but cannot qualify the positive
C2 path. No whole-C8/E10 transplant is implied.

Then integrate Model E plus C2/C3 real-owner hooks selectively with candidate
build guards, preserve the API2 source path, and run the original impacted
offline positive/negative oracles. Do not re-run the valid SDK ABI probes
unless their inputs change. No scheduler redesign or new listener is needed.
No additional source authority is inferred from this closeout.

## ED11 and preserved scope

No C2 owner/Session/Connection/C3/buffer/retained storage was added by this
SDK-only slice. Its test stack object is not an operational C2 reservation.
The effective C2 composition census and shared credits are NOT_ESTABLISHED;
provider/kernel remain UNKNOWN; ED11_TOTAL_STATUS=PARTIAL.
Operational admission remains DENY. The later source integration still needs
its actual aggregate census, CONTROL/CANCEL/FINAL reserves, retained charge
and approved target evidence. No historical or Browser census was imported.

Preimages and baseline preserve the prior WIP. Only CMakeLists.txt and
cmake/PapaccPstConsumer.cmake among the prior WIP are edited; the new test and
this evidence directory are additive. No production code, Browser/PST source,
pin, accepted contract or PACC wire changed. No network, joint HTTPS, C8,
Legacy, E10 or NT4 execution occurred. No clean, commit, push, tag or release.
PST0.6.5 publication remains deferred until full Phase6 completion.

See preservation.json, sdk-build-results.json, source.diff and
final-manifest.json for actual verification. SDK-only results do not close
this task or imply that the Mode C supervised peer is ready.

NEXT=RESOLVE_FACTUAL_ROOT_POLICY_DEPENDENCIES_BEFORE_C2_OWNER_INTEGRATION