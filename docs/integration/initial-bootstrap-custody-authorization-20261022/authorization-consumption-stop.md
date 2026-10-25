# NEW_INSTALL authorization consumption: focused STOP

Task PACC-PHASE6-INITIAL-BOOTSTRAP-CUSTODY-AUTHORIZATION-1, 2026-10-22.
Active main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
No functional patch or accepted contract modification. Existing components and all WIP preserved.

## Exact normative boundary

DECISION-local-bootstrap-ceremony-20261019 P1:

> INITIAL_BOOTSTRAP é operação administrativa explícita da única root, separada de update/restore, com ator efetivamente autorizado fora do candidato.

P1 also states:

> SID informado pelo usuário, token operacional, autenticação de cliente, write no filesystem ou flag CLI não concedem essa autoridade.

P2:

> O software registra a declaração como declaração, distinta de observações nativas.

D2.1 independently requires factual authority and lifetime of the actor/configuration/material references. These clauses establish eligibility and security obligations; they do not themselves constitute a supplied authorization for a concrete installation. P3/P7/P8 define same-preparation custody, retention and denial on uncertainty, but do not supply the missing CLI-to-owner producer/consumer contract for a separately authorized NEW_INSTALL act.

The exact unresolved boundary is how that existing administrative entry obtains the specific authorization outside candidate input, binds its scope to operator/parent/configuration/invocation, then consumes and invalidates it in relation to the first exclusive-creation attempt. No accepted concrete act interface or producer supplying that fact was identified in the inspected sources. This finding is narrower than claiming the whole bootstrap contract is absent or requiring a new authority system. It does not imply that another approval service, token format, journal, CURRENT, or prior root must exist.

The previous report classified the missing flow as an implementation/qualified-input gap. This task explicitly makes definition of authorization and consumption a precondition for implementing that flow. High-level criteria alone cannot justify inserting a boolean, caller SID, CLI flag or UPDATE receipt as its factual source. No new interpretation of P1 is frozen as accepted here.

## Concrete producer inspection

first_install_entry in server_config_cli.c recognizes declaration input and returns AUTHORIZATION=ABSENT before any acquisition. bootstrap_admin checks native role/context and rejects impersonation. bootstrap_auth_native captures/revalidates TokenUser/TokenStatistics and root/container identity, but not a separately supplied NEW_INSTALL act. Its proof is captured in the same packet after exclusive container creation. custody_begin checks mode/owner/resources and native admin eligibility then starts packet9; no explicit authorization/declaration parameter is consumed. These are useful components, not proof that the administrative act exists.

The native target is the exclusively created object under the governed parent, not CLI spelling or hostname. P3 requires same-preparation continuous custody. No object was created to manufacture a positive result. No bootstrap test with diagnostic bypass was run.

CA custody remains separate: PAPACC_CA_KEY requires existing configured authority/generation/operator/service association, validates identity/properties/ACL, and owns key/provider/retained resources. Its borrow is not initialization authorization. Accepted issuer clause6 requires distinct authenticated administrative CA initialization. No actual CA owner/reference was supplied or acquired; no key/certificate was created or opened. Initial v1 no-issuer bootstrap still does not require prior CURRENT or CA generation.

## Minimum resolution before a dependent patch

An exact normative clarification or an identified existing accepted concrete contract must close only the authorization/consumption boundary: the canonical administrative source and eligibility checks for the specific act; what binds the declaration to the governed target/configuration and invocation; the consume point, rejection of reuse, cancellation/Stop, partial failure and UNKNOWN disposition. Preserve P1's local administrative model, P3's native creation/custody, same owner and P7/P8 retention. This is not an invitation to add a second registry, authority or persistent act store. Exact implementation and resource census follow official review; no dependent patch is accepted by reference.

Task item4 requires STOP if authorization or consumption is not defined. Task item10 also denies presumed authorization. Consequently no new integration or native creation is attempted. No implementation defect requiring an independent source patch was demonstrated. Previously passed tests remain historical, not fresh results; builds/tests are NOT_RUN at this stop, and no redundant generic audit or qualification rerun was performed.

ED11 remains PARTIAL. No complete applicable native/provider/kernel/CRT/nested-stack/retained-graph/host bound is supplied by this contract review; nominal managed headroom cannot authorize the operation. No quotas, profiles or production pins changed.
