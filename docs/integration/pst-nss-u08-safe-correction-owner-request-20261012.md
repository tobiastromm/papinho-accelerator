# NSS U08 correction: owner decision and governed return request

Status: proposed; no PST/third-party implementation performed.

The Accelerator source audit is reproducible from [archive/member hashes and exact excerpts](../capabilities/pst-nss-u08-source-reconstruction-20261012.json). sslsecur.c:731 discards SSL3_SendAlert; lines 734-738 return lower-layer shutdown. pst_backend_nss.c:1161-1162 treats PR_Shutdown success as local close_notify sent. ssl3con.c:2403-2462 permits saved ciphertext after would-block/partial sends; lines 2753-2755 can therefore report alert success before native acceptance. The first incorrect proof boundary is NSS secure shutdown; PST subsequently relies on it.

Required owner decision: authorize a specific governed RetroZilla NSS third-party correction and reproducible package rebuild, without public API/ABI/SPI change. Current authorization explicitly stops for an unauthorized third-party alteration; a safe consumer-only one-line fix is not established. This request is prepared locally, not sent externally.

The correction design must enqueue close_notify once, preserve pending ciphertext and locks, drain/retry under genuine nonblocking readiness, and avoid lower-layer shutdown until the required native-acceptance boundary. Alert callbacks, enqueue success and native acceptance must remain distinct. Establish behavior for partial writes, repeated shutdown, cancel, truncation, failure, and Model E retained ownership before coding. Do not expose a fabricated completion signal or infer drain from PR_SUCCESS.

Return: patch/recipe/flags/source identity; byte-exact normal predecessor; corrected DLL/library/package hashes; TLS compatibility and negative probes; real pending/partial-send tests; ownership and exactly-once. The historical NSS native-retention package 9E82D64C07966BEF18959BEA327670E362C619CC0B769CB4E12F5FB813A836D5 must remain untouched. Reexecute only impacted retention/owner/credential/WinSock gates against the new DLL, distinguishing historical and new evidence. No promotion or production pin change.
