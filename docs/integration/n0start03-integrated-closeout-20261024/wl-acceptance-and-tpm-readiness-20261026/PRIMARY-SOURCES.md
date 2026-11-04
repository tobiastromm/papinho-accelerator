# Primary implementation references

* Microsoft TSS.MSR Sessions.cs: https://raw.githubusercontent.com/microsoft/TSS.MSR/main/TSS.NET/TSS.Net/Sessions.cs — nonce ordering, session-key/auth concatenation and session attributes; command/response HMACs tested on synthetic inputs, not a copied automatic-session/retry client.
* Microsoft TSS.MSR Tpm2.cpp: https://raw.githubusercontent.com/microsoft/TSS.MSR/main/TSS.CPP/Src/Tpm2.cpp — cpHash names/parameters and rpHash return-code/command/response ordering.
* RFC4231: https://www.rfc-editor.org/rfc/rfc4231 — standard HMAC-SHA256 fixture vector.
* NIST SP800-38A F.3.13: https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38a.pdf — AES128-CFB128 vector, two blocks tested.
* Windows CNG properties: https://learn.microsoft.com/en-us/windows/desktop/SecCNG/cng-property-identifiers — ChainingModeCFB and MessageBlockLength16 for full-block feedback.
* Windows ProtectedData: https://learn.microsoft.com/en-us/dotnet/api/system.security.cryptography.protecteddata?view=netframework-4.8 — CurrentUser DPAPI protection. It does not make a protected blob a monotonic witness.

No additional crypto package, TPM provisioning tool or network authority was installed. Existing Windows APIs/providers used only with public test vectors. Upstream links are research provenance, not firmware attestation or operational permission.
