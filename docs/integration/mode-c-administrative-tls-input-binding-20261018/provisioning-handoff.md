# Mode C administrative and TLS provisioning handoff

Status: PREPARATION_ONLY / INPUTS_UNBOUND. Date: 2026-10-18.
Owner confirmed that no previously approved campaign configuration, certificates or leases are available to identify. This document prepares provisioning; it does not approve materials, run a transaction, generate credentials or issue authority.

## Provisioning matrix

| Input | Existing canonical mechanism / factual source | Missing selection or evidence | Responsible party / next action |
|---|---|---|---|
| Administrative storage | `apps/server/server_configuration_store05_win32.inc`, `server_configuration_body05_win32.inc`: private BODY 0.5, caller-supplied path, first store creation rather than overwrite/adoption | Approved absolute store path, administrative domain, revision, predecessor and configuration identity | Accelerator administrator selects path and configuration; use existing encoder/transaction, never hand-edit persistent bytes |
| Root and CURRENT | `server_configuration_root_transaction_win32.inc`, `server_configuration_participants_win32.inc` | Successful real preparation, durable commit and current pin for selected inputs | Accelerator integration owner invokes existing transaction through a verified administrative entry point; deployed CLI binding is not established by the presence of these APIs |
| Policy and Principal | `server_policy_model.h`; existing authenticated resolver and ordered administrative rules | Owner-selected Secure profile/admission mode, rules, enrolled client and exact resolver Principal16 | Administrator approves intended scope. Runtime resolver supplies factual identity; no padded Principal, certificate fingerprint substitute or invented Session |
| Grants | Existing root/policy and C2 authorization | Actual authenticated request and authorization facts | Product issues through existing path only after applicable operational gates; configuration presence never issues a grant |
| Site TLS profile | `server_site_tls_profile.h/.c`, BODY 0.5 | Catalog/profile identities, revisions, reference, approved validity interval, max_bytes, max_duration_ms, concurrency and administrative origin | Administrator supplies approved values. One logical operation is campaign scope, not approval of unspecified profile limits |
| Channel server material | `server_security_source_win32.c` | Approved server certificate, protected PKCS8 key reference and configuration reference | PKI/material custodian supplies existing approved material or requests specific issuance approval |
| Channel client material | Existing Browser credential importer, validator, generation and lease store | Approved client certificate/key reference, channel trust, actual Browser user/store identity | Browser Codex provisions only after corresponding approval, using existing import and commit path |
| Channel authorization | Existing security source client enrollment and Principal resolver | Approved client leaf digest and policy binding | Administrator enrolls approved client; enrollment alone does not authorize Site TLS |
| Origin TLS | Existing Site TLS SYSTEM trust path and operation hostname/SNI | Actual loopback origin, DNS hostname, certificate chain and key custody; evidence of SYSTEM trust for execution account | Origin operator supplies materials from existing approved PKI. Do not replace hostname with IP or import a fixture CA |
| ED11 and campaign admission | Existing local producers, ledgers and external observer | Quantitative freshness/headroom, target evidence and any separate laboratory exception decision | Owner reviews evidence-backed parameters. Provider/kernel remain UNKNOWN; all admission remains denied |

## Exact configuration and storage interfaces

The Accelerator private store is 8192 bytes with three slots; BODY 0.5 is 1024 bytes and preserves typed Principal16 representation. It stores configuration/policy/catalog metadata, not private key contents. The path is an explicit API argument, not an established installation default. No campaign store has been selected or created.

Use the existing sequence with real participants and owned preparation: `papacc_server_launcher_configuration_apply_dormant`, `papacc_server_launcher_configuration_transaction_prepare_store`, `papacc_server_launcher_configuration_transaction_commit`, `papacc_server_launcher_configuration_transaction_publish`, then the existing report/current-pin interfaces. Their preconditions remain authoritative; this list is not an executable substitute for preparation. The first-store path rejects existing initialization/unsupported expected revision. Failure, unresolved commit or teardown requires existing conservative retention, not retry or store replacement.

The current-policy authority generates the factual epoch only after durable commit. The campaign must capture its report/pin, not choose an epoch, derive it from revision or patch persistent bytes. CURRENT, grants and epoch are absent until proved by their respective paths.

The catalog validator requires nonzero revisions/identities, a valid expiry interval and nonzero byte/duration limits. Pending values cannot be encoded as zero placeholders and treated as a valid profile. Preserve SYSTEM trust, required hostname/SNI, TLS 1.3 and exact ALPN `http/1.1`; profile lookup must match CURRENT revision and epoch. No implicit default or unknown-profile fallback.

Existing Accelerator laboratory security-source inputs are `PAPACC_SECURITY_CONFIGURATION_REF`, `PAPACC_SERVER_CERTIFICATE_DER_B64`, `PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64`, `PAPACC_CLIENT_CA_DER_B64`, and `PAPACC_ALLOWED_CLIENT_CERT_SHA256`. The three B64 variables name files containing raw base64 DER, not inline secrets. Their existence as mechanisms does not approve laboratory authorization for this campaign. Preserve source prepare/commit/publish/lease ownership. Channel ALPN is `papacc/1`, distinct from origin `http/1.1`.

Browser reference only: `src/papinho_credential_store.c` resolves the actual user's AppData and `PapinhoBrowserCredentials-lifetime-v1`, with generated credential-reference directories containing `trust.der`, `certificate.der`, `private.pk8`, `complete`. Access controls are not encryption at rest. Do not open/create the store in the executor's account as a substitute for the actual Browser user. Preserve existing owner/DACL/reparse checks.

The Browser importer supports bounded DER/raw base64 and unencrypted PKCS8, not arbitrary PEM/PKCS12. Reuse its material validator and store preparation, then existing configuration-generation commit under `HKCU\Software\Papinho\PapinhoBrowser\Accelerator`. Mark the candidate committed only after the factual `ActiveConfiguration` flip; acquire/return leases through existing interfaces. Do not manually copy private material or edit registry generations. Browser changes belong to Browser Codex, not this repository task.

## Ordered provisioning gate

1. Owner/admin selects existing PKI issuer, channel server hostname, origin hostname and loopback endpoint, execution accounts, protected material/store locations, intended access rules and profile values. These remain pending, not inferred from fixtures.
2. Custodian inventories existing approved public certificates and protected key references without disclosing keys. Validate identities, EKU, validity, chain/trust and key correspondence offline through existing mechanisms. Avoid revocation/AIA network retrieval in this task; an unavailable trust/revocation fact remains INCOMPLETE.
3. If suitable material does not exist, submit a separate exact request for key/CSR creation, signing or import identifying issuer, subjects/SANs/EKU, algorithm, validity, accounts, destinations, custody and affected stores. Execute nothing until specifically approved. No new CA or fixture-root installation is proposed.
4. Browser Codex imports approved material and publishes its configuration through its existing gate; reports only references, public hashes, identities, status and lease disposition. Accelerator administrator prepares its source and BODY 0.5 via existing transaction with C2/Secure participants. This preparation handoff does not authorize running that transaction now.
5. Capture real commit/CURRENT/epoch and later authenticated Principal/grant evidence only when separately authorized. Bind public references to campaign revision 3; never write secrets, fabricated IDs, or retrospectively assign an observation to a phase.
6. Keep execution blocked until inputs, process provenance, causal A-H association, Stop, ED11 parameters and any laboratory exception have their own approvals. A valid certificate or successful store commit is not HTTPS qualification or admission approval.

## Evidence boundary and reconciliation

Campaign revision 2 was preserved byte-exact in `preimages/manifest-revision2.json`; revision 3 selects the current canonical Browser real-HWND diagnostic-qualified Release artifact and retains null administrative/TLS bindings. The Browser idle offline Stop result does not qualify active Mode C, and its diagnostic command-line flag is not a Mode C launch command. Executable file hashes identify selected artifacts, not loaded process images.

Public `tests/fixtures/security` certificates were inspected only as excluded TEST_ONLY material. Date validity does not establish SYSTEM trust, intended hostname, key ownership or administrative approval. No private keys were read, generated, exported or imported; no trust store was changed. No mTLS handshake or origin HTTPS was performed.

Freshness/headroom remain PENDING. Provider/kernel remain UNKNOWN. No quotas, credits, reservations or managed resource limits changed. No new operational storage was added by this documentary work. No total operational ED11 PASS is claimed.

CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
EXECUTION_AUTHORIZED=NO
NEXT=OWNER_SELECTION_OF_EXACT_ADMINISTRATIVE_INPUTS_AND_PKI_PROVISIONING_SCOPE