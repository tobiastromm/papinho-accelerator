# OpenSSL x64 Model E R4: governed upstream mechanism request

2026-10-12. PREPARED_LOCAL_NOT_SENT. Separate from the NSS correction owned by PST.

## Authority and observed limitation

PST's model-e-consumer-conformance-matrix.md defines R4 as RESOURCE_FAILURE plus query-only retention, minimum class "falha governada". R1/R2/R3, synthetic consumer policy, NSS native failure and OpenSSL failure are different evidence classes. PST ADR-0008 assigns native close/linger/classification to the provider; the consumer cannot implement its own native close or retry. Isolated Accelerator ADR-0033 requires persistent owner/manager/provider/runtime references and no destructive fallback. ADR-0033 is absent in the main tree and remains scoped to the governed isolated API3 tree; this handoff does not transplant or broaden that ADR.

Normal source src/backends/openssl/pst_backend_openssl.c:99 implements one closesocket on the bound owned socket, with RESOURCE_FAILURE/CLOSE_UNRESOLVED_RETAINED only on unconfirmed native close. Public package headers expose release/query but no consumer close-failure control. Existing NEED_WRITE packages establish TLS write pendency, not a failed native release. A peer reset is a transport failure, not proof that closesocket failed. No normal OpenSSL R4 return was observed.

Accelerator will not close a transferred handle, corrupt an alias, set consumer linger policy, hook Winsock, rebuild PST, fabricate PST_RELEASE_INFO, or present the auxiliary synthetic model as R4.

## Smallest requested return

Owner PST should provide either an already governed OpenSSL x64 R4 artifact/recipe for the same candidate commit da7190930700f54ecc92611f5edbcb0932caa5ec or a separately authorized test-only package. Normal version0.6.5/API3.0.0/SPI4.0 remains byte-exact; no public API/ABI/SPI change.

The owner must state which class the returned mechanism proves and whether it satisfies R4's minimum. A deterministic provider-boundary test-only failure may be suitable for governed retention logic, but MUST NOT be called a real Winsock failure. A real-native lane requires an owner-validated safe recipe and factual code/result without stale handles, double close or unsafe corruption. Never fabricate native error fields. The consumer will not choose a native injection recipe autonomously.

Private controls, if provided, must be scoped to exact Connection/token/generation, opt-in and absent from normal package. One legitimate release attempt yields matching function/record RESOURCE_FAILURE and CLOSE_UNRESOLVED_RETAINED. No provider destroy, native retry, transport destroy, native alias reuse or dependent lease cleanup follows. Queries preserve identity. Repeated Stop/cancel and late callbacks must not issue another release. Second connection remains independent. Process exit, if required for containment, is not destruction or clean completion.

Return patch/recipe/flags/manifest/SHA256SUMS/header/library/DLL/executable/trace hashes; source identity, normal/test-only separation; gate OFF, wrong token, stale generation, duplicate association/control, early release/membership, second connection and late callback negatives. Identify required owner-runtime/credential/WinSock leases and approved retention observation.

## Accelerator follow-up

Stage in a new test-only directory, verify every hash, never overwrite normal SDK. Integrate only through governed control and public PST release/query. Observe attempt/result/disposition separately by run/Connection/op/generation; retain durable manager and quotas after initiating scope returns. Validate no I/O/release retry/rebind/reuse/fallback and no premature dependency destroy. Revalidate only impacted isolated gates; no NSS duplication, production C2/C3, E1-E12, promotion or pin change.

R4_OPENSSL_MODEL_E=BLOCKED_GOVERNED_UPSTREAM_MECHANISM_REQUIRED
NORMAL_SDK_CHANGED=NO
