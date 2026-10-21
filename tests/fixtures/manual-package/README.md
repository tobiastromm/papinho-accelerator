# Public upstream test fixture only

upstream-public-test-key.pem is copied byte-exactly from OpenSSL3.5.8 test/testrsa2048.pem (official source archive SHA256 a8f84a39918ec6415ce765d9b429d313ba97b8143169c172e734b9514464f5b2). Apache License2.0; full upstream license preserved alongside. This is an already published private TEST key, not secret material, CA, certificate, package-purpose delegation, administrator identity or trust. Never install, deploy or use outside automated tests.

The isolated crypto test uses it only for an in-memory RSA signature-domain positive/negative vector. It never generates a key/certificate or a productive package. It must never be reused by a runtime/CLI or treated as a trusted signer. No production runtime target links this test or adapter. See docs/integration/manual-package-crypto-completion-20261020/implementation-proof-and-browser-handoff.md for measured provider costs and unresolved authority/lifecycle/ED11 gates.
