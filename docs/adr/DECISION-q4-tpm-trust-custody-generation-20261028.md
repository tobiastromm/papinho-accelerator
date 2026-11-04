---
status: accepted
revision: 1
decision-date: 2026-10-28
scope-level: project
supersedes: []
---
# Decision — Q4 TPM trust/custody/generation framework

Owner explicitly approved the normative direction and technical integration in TASK PACC-PHASE6-ED11-TPM-TRUST-CUSTODY-ACCEPTANCE, bound to exact proposal SHA256133d0cdc74458259cfaed9d9a6e28504f090ee18ea41ba24fd0e50395b364845. Entire proposal and W/L/L1/cross-boot incorporated sources read and bytes checked before recording. Incorporate the unchanged [TC0–TC9 proposal](ADR-CANDIDATE-q4-tpm-trust-custody-generation-20261028.md) as the approved framework and fail-closed obligations. Original proposed bytes remain historical and immutable.

This records approval of the framework, NOT a bulk selection of the independent conditional designations reserved by TC9. Record separate current dispositions:

A1: exact Intel root/chain/EK is the sole CANDIDATE for the local purpose. Operational anchor designation is still NOT_GRANTED; no trust-store installation or generic manufacturer acceptance.
A2: strict fail-closed implementation: UNKNOWN is not acceptable witness/NV/Q4 evidence. The optional one-experiment diagnostic UNKNOWN exception has NOT been specifically selected or bounded by the owner and remains NOT_GRANTED. No inference of revocation PASS.
B: custody framework approved, but observed SID1108 has NOT been effectively designated as custodian; ownerAuth source/possession and actual ciphertext protection/recovery are NOT_DEMONSTRATED. No operational secret generated or authority fabricated.
C: approved generation/rollback requirements and DENY on discontinuity; actual nonrestorable hardware boundary, original index generation and persistence are NOT_DEMONSTRATED. Candidate0x01804143 is not allocated/reserved/owned.
D: no operational provision/read/increment/root/Q4 permission. Exact effects and separate approval remain mandatory after factual prerequisites.

TC9 requires those independent exact choices; this explicit owner approval authorizes the framework/technical integration, not presuming missing facts or choosing the optional exception. No change to W2/W4/L2/cross-boot guarantees or inheritance of personal M3. Existing owner, CAS, RAW1, Stop/UNKNOWN, limits and reservations remain mandatory. Framework acceptance never substitutes for a producer of authenticated/native evidence.

Technical policy integration/builds/affected regressions and derived evidence are authorized. No TPM/NV/Clear/hierarchy/Q4/Start/Stop/SCM/ACL/cleanup/refund/CURRENT/signature/recovery. ED11/Phase6 PARTIAL; E21 pending.

[Integration report](../integration/n0start03-integrated-closeout-20261024/tpm-trust-custody-acceptance-20261028/CONSOLIDATED-CLOSEOUT.md).
