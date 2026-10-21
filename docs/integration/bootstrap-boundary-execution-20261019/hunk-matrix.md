# Exact functional hunk review

| File / function | Change | Ownership and negative gates | Regression |
|---|---|---|---|
| pending header | Two namespace handles and identity/state values in existing packet | Same root/worker; no serialized fields/new owner | sizeof, retention |
| commit/pending_blocks | Retain packet while custody handles exist | No other begin can overwrite retained leases | pending/finish denial |
| pending_worker/poll | New internal phase9 before historical format discrimination | Same thread; no persistence/publication; inputs frozen; outputs read only after signal | native create/Stop/poll |
| bootstrap include | Enabled local admin check, NtCreateFile parent and atomic relative FILE_CREATE directory | Reject impersonated caller; no ACL change; OBJ_DONT_REPARSE; handles retained; parent/object namespace, not global path/fencing | rename/delete denied; reparse/existing/unknown/Stop cases |
| custody begin/ready/close | Owner-thread entry, budget check; nonwaiting poll; factual close after Stop | No CURRENT/previous history; created/uncertain operation not reusable; close once/no retry | loss/retention |
| config source binding / INITIAL_BOOTSTRAP_RESOLVE | Separate initial result type; source/policy/model + actual custody | Update resolver unchanged; no synthetic CURRENT; explicit epoch absence; binding.resolved stays false | initial absent/invalid authority, existing update guard |
| test/CMake | Compile-isolated diagnostic native primitive bypass | Macro only separate test binary; never sets native admin proof; cannot resolve initial or appear in product config | native primitive tests distinguished from authorization proof |

Preimages/postimages: source-operations.json and preimages/. No historical format, grants, reservation or operational admission changes. Namespace proof refers to stable parent object and atomic relative child creation; ancestors need not retain a path spelling because worker does not re-open it. There is no store commit from phase9: later use for COMMIT remains unqualified and denied. This patch is an independent custody/initial-reference boundary, not completed bootstrap transaction or production qualification.

Managed delta: packet adds two HANDLE(16), six DWORD(24), four unsigned(16), alignment initially56bytes; exact compiled layouts must verify. Additional worker scratch is bounded by2048 checked before begin, within existing65536 reserved stack but conservatively charged separately. Initial-result output and2048 scratch are checked by resolver; no quota increase. Provider/kernel remain UNKNOWN. Required full managed/cumulative census after build; do not approve by guessed margin.

Native administrator token is NOT enabled in current executor (medium integrity, no enabled Administrators group observed). Actual administrative positive preparation is therefore expected to deny. Diagnostic bypass proves only native namespace primitive and cannot create an approved custody-ready result. No elevation requested/attempted. Full completion must remain NO unless authoritative positive integration and all tests are established.
