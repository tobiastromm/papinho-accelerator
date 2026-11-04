# Two independent normative decisions - awaiting owner

No acceptance is recorded by this package. Decisions may be made separately; neither authorizes NV mutation or Q4.

1. W0-W7: source `docs/adr/ADR-CANDIDATE-q4-tpm2-nv-witness-20261026.md`, SHA256 `c99e4a8c599a9f3937e3dda08458b043136ecee1a14c1bcc7a1d3ae1ed3a057a`. Requested selection: TPM witness for the narrow Q4 ledger-continuity purpose under all original W conditions and factual qualification, not generic recovery authority.
2. L0-L7: source `docs/adr/ADR-CANDIDATE-q4-consumption-witness-and-root-ledger-designation-20261026.md`, SHA256 `eb32e4e79476f53f9aa7be53dfe911a4b5688c4b67ce1f2fefed6f87c32d90c8`. Requested design direction: existing sole canonical root as LOCAL documentary ledger with independently qualified TPM witness under W, preserving L's independent persistence/no-replay/STOP requirements. This is not proof that hardware or custody is already qualified or permission to publish CURRENT. No unspecified component schema is accepted by inference.

After any approval, independently verify exact bytes/conditions, register only approved scope, and continue technical implementation where authorized. Missing actual identity/custody and resource/session proof do not become true from normative acceptance. The hardware provisioning final candidate remains non-executable until those prerequisites are resolved, then needs separate exact manual approval.

## Public identity fact collection

IDENTITY-PUBLIC-COLLECTION-MANUAL.txt contains the exact command for an already elevated Windows PowerShell5.1x64. This is the public query already technically in scope, not a new provisioning/Q4 approval. Do not elevate or change policy automatically. Return its entire public output; missing fields, string diagnostic or failure is STOP. No secret ownerAuth, NV write, chain-network request or signature is included. Public hash/cert metadata is a candidate identity input and not authenticated device proof by itself.
