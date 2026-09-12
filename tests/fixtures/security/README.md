# Autonomous mTLS test PKI

**TEST ONLY — DO NOT USE IN PRODUCTION.**

These Base64 files contain DER certificates and unencrypted PKCS#8 DER private keys used only by the PapinhoAccelerator real-mTLS integration test.

They were generated once on 2026-09-11 with OpenSSL. The hierarchy contains one RSA-2048 test CA, a server certificate for `papacc-test-server` with the matching DNS SAN and server-auth purpose, and three distinct client certificates with client-auth purpose. All leaf certificates are signed by the same test CA.

The private keys are public test fixtures. They must never be used for production, deployment, or any identity outside the automated test. The test has no runtime dependency on an OpenSSL CLI or a PapinhoSecureTransport source checkout.

These fixtures are test data only and are not installed or staged with PapinhoAccelerator runtime artifacts.

Fixture roles:

- `server`: server-auth certificate with `DNS:papacc-test-server` SAN;
- `client-a`: enrolled credential used for ALLOW and policy-DENY cases;
- `client-b`: TLS-valid credential intentionally not enrolled;
- `client-c`: distinct TLS-valid credential mapped to the same test Principal as `client-a` to prove certificate rotation.
