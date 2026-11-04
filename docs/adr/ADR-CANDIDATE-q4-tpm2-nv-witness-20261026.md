---
status: proposed
revision: 1
date: 2026-10-26
scope-level: project
supersedes: []
---
# Q4 candidate - TPM2 NV witness, existing canonical root remains local ledger

## W0 - Real candidate and scope
Read-only local investigation confirmed an Intel TPM2.0, firmware500.14.0.0, via Windows TBS and tpmtool. GetCapability properties report NV counters max128/current2/available4 at observation; no capacity reservation follows. TPM identity is not attested by manufacturer/version/interface. Attestation readiness was reported false. No existing Q4 index or hardware authorization has been identified. This is a PROPOSED selection, not acceptance of L0-L7, provisioning authority or deployed witness proof.

## W1 - Preserve single root and purpose
Recommend qualifying one dedicated NV counter as an independently persisted witness bound to the exact root/approval protocol, using the existing root owner and CAS/retention machinery. No second root/journal service, generic IPC/control plane, daemon or issuer use. A narrowly typed local documentary participant/schema still requires exact review. Existing historical intents/approvals are not reused. Preserve cross-boot/R7 original-object proof, RAW1, N7/H0-H9 and UNKNOWN; no readback authority follows.

## W2 - Required provisioning decision before hardware mutation
Before any NV definition/increment: owner approves the exact hardware identity and non-restorable boundary, hierarchy/custody authorization, dedicated index/public area/Name, policy permissions, approved identity generation, limits/resources and one-purpose binding. Choose counter attributes and persistence mode against actual device semantics, including orderly/power-loss behavior. Do not read/alter other existing indices or guess owner/platform auth. Windows command permission and TPM authorization must both be demonstrated without changing GPO/driver/ACL. No Clear, undefine, owner-auth change, fallback, automatic provision, index reuse, baseline reset or secret logging. TPM availability is not permission to allocate it.

## W3 - Consumption protocol to qualify
Bind historical10584 identity, new exact approval, original root/predecessor, current host/boot/operator and exact live reader PID/creation. Existing root serializes the one operation; durable root preparation precedes witnessed irreversible consumption. Validate original device/index identity, authorization and expected baseline; increment only once, verify the exact expected next value and durably commit the corresponding local fact before protected reading. NV_Increment is not an application compare-and-swap and a counter is not an approval registry: test all races, intervening increments and root lineage, and never translate an unexpected value into a new favorable baseline. Competing or ambiguous increments may burn authority and deny both callers, never retry.

Only the original consuming invocation with live retained lease may continue; a restarted observer of matching CONSUMED records is denied. Crash before/after local preparation, NV update or local commit preserves first uncertainty; query-only reconciliation cannot grant a replacement run. UINT64 exhaustion/overflow, unavailable NV, rate limiting, failed response, mismatched counter/identity or uncertain close => DENY/UNKNOWN with retained dependencies. No timeout creates success or rollback/refund.

## W4 - Rollback and hardware discontinuity
The intended guarantee is detection of restored local ledger against independently advanced NV on the SAME non-restored hardware generation. A virtual TPM snapshotted with the ledger is not independent; the actual storage/firmware/device threat boundary needs factual qualification. A matching counter value alone does not authenticate device, index, permission or root identity. NV Name/handle alone does not establish original provisioning generation. Device replacement, Clear, index loss/deletion/redefinition, untrusted firmware, unavailable identity or ambiguous generation continuity deny; never recreate or adopt an index to make old state pass. Do not claim protection against arbitrary TPM/hypervisor rollback or compromise without an independently established boundary. M3 is not inherited.

## W5 - Qualification still required
Native GetCapability and checked TBS context closure are read-only availability proofs, not counter lifetime proof. Current protocol tests use a simulated nonrollback witness. After separate exact authorization, a dedicated isolated provisioning experiment must establish actual command access, index identity/ownership, increment/readback, reboot persistence, fault/power-loss semantics, concurrent writers, identity replacement detection, retention and ED11 costs. No power-cycle, counter write or operational root effect is authorized by this proposal. Hardware experiments remain segregated from Q4 readback and require their own exact effects/Stop/UNKNOWN plan.

## W6 - Alternatives and consequence
TPM is the preferred next qualification candidate because present hardware/TBS support is demonstrated and no new permanent service is needed. It is not yet designated. Existing externally governed atomic-consumption authority is an alternative only if independently identified and approved; none was established in reviewed project mechanisms. An ordinary external disk, DPAPI, registry, event log, Git/hash chain, PCR measurement or a signing key alone is not a demonstrated monotonic consumption witness. If TPM policy, independent persistence or durable use cannot satisfy the contract, retain readback DENY and return with exact failed property; do not reduce protection to local-only storage assumptions.

## W7 - Authority boundary
No acceptance of other proposals, no Q4/LAB/Start/Stop, root CURRENT publication, operational intent, protected input, SCM/ACL/GPO change, cleanup/refund, signature or recovery authority. Original reader/launcher/pins unchanged. ED11/Phase6 PARTIAL, E21 pending and productive authorities DENY. Owner normative selection and separate bounded hardware-provisioning authorization precede mutations; an eventual readback needs another exact approval.
