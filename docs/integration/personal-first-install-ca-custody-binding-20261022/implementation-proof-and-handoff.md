# Private CA reference handoff

See [complete proof and remaining boundary](closeout.md). Canonical interfaces in server_configuration_commit_win32.h:

- personal_ca_match_data: bounded comparison only, no fact/authority.
- personal_ca_bind: existing native gates/revalidation plus one existing CA borrow; no creation/adoption.
- personal_ca_current: revalidate same owner/attempt/source/target before future preparation.
- personal_ca_release: return at worker quiescence, including after Stop; retain on failure.

Caller must keep the actual canonical CA owner alive, immutable except through its APIs, on the same writer thread and reserved until return. A bare name, copied metadata, pointer or LAB receipt is not a producer proof. The same-invocation CA_INITIALIZE producer is absent from the current read-only CLI; no positive factual reference exists. Do not lift personal_native_ceremony_ready, attach the LAB key, fabricate a reference, or enable root prepare/commit to make tests pass. New preparation must continue using the existing owner/worker/CAS and deny without facts.

Operational creation/publication/signature remain unauthorized in this task. No manual generation command is supplied.
