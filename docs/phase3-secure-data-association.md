# Phase 3.D — Secure DATA Association Binding

Status: complete. Phase 3.E now consumes this private gate in the real secure
server CONTROL/DATA path.

## Scope

Phase 3.D binds an independently authenticated DATA connection to the exact
authenticated Session named by a one-time DATA ticket. It does not change the
PACC wire format, enable the production server path, or modify
PapinhoSecureTransport.

A DATA ticket remains structural correlation material. Possession of a valid
ticket is necessary, but is not identity or authorization evidence.

## Secure association order

The secure path performs these steps in order:

1. inspect the ticket without consuming it;
2. validate the independent DATA Connection Security Context;
3. resolve and validate the target Session Security Context;
4. require equality between the authenticated DATA principal and the target
   Session principal;
5. authorize `PAPACC_AUTHORIZATION_ASSOCIATE_DATA`;
6. resolve and validate the Session Security Context again immediately before
   commit;
7. atomically revalidate and consume the exact inspected ticket;
8. bind the DATA channel to that exact Session.

Ticket inspection and commit also require an unexpired ticket, an active
Session, and its bound CONTROL channel. Expired or structurally invalid ticket
lifecycle is cleared without allowing association.

Identity mismatch, invalid security contexts, authorization denial,
authorization error, or a pre-commit revalidation conflict do not consume a
valid ticket. A successful commit consumes it exactly once, so replay fails.

The same `PAPACC_AUTHORIZATION_ASSOCIATE_DATA` action gates ticket issuance and
DATA attachment. This avoids granting association material to a principal that
is not authorized to use it.

## Integration boundary

The Phase 2 post-CONTROL and DATA attach processors retain their original
behavior when no gate is installed. Private opt-in hooks allow security
composition to gate ticket issuance and replace consume-before-authentication
with the secure inspect/authenticate/authorize/commit sequence. Existing
`DATA_TICKET`, `DATA_ATTACH`, and `DATA_ACCEPT` payloads remain unchanged.

Ticket commit and channel binding are intentionally separate operations in the
current architecture. If channel binding fails after a successful commit, the
ticket remains consumed and is not restored. Making consume-and-bind one atomic
transaction would be a separate durable architectural decision.

## Validation

Portable tests cover non-consuming inspection, exact commit, expiry and
Session/CONTROL lifecycle revalidation, principal mismatch, authorization
deny/error, missing or invalid security contexts, pre-commit conflict,
single-use behavior, replay rejection, and exact Session binding.

The real PapinhoSecureTransport TLS 1.3 mTLS loopback harness derives distinct
authenticated Principal A and Principal B contexts. Principal B cannot consume
Principal A's ticket; Principal A can then use that same ticket successfully;
and replay is rejected. Processor tests also confirm that opt-in hooks preserve
the existing successful wire responses.

No new sensitive logging was introduced. Credentials, private keys, tickets,
and arbitrary payloads remain outside log output.
