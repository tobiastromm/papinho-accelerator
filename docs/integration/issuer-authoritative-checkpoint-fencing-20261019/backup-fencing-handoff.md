# Backend and same-root checkpoint handoff

No implementation authority or normative acceptance is granted by this report. Owner confirms no concrete backup/fencing source. Existing accepted profile2.4/3.5-3.6 and relation P6 apply.

1. Complete the same-root durable checkpoint's exact layout and crash protocol for R/predecessor/I/CURRENT/A/P/ack. Preserve historical BODY/issuer bytes and existing slots/pins; no second journal. Local metadata validation remains denied recovery.
2. Owner selects concrete protected backup/restore mechanism, independently retained latest history/high-water custodian and proof preventing ALL earlier installations from signing the same generation, including offline clones/uncertain signatures. Raw booleans/file locks/CAS are insufficient. No external service/network introduced by this task.
3. A portable adapter must bind source/root/authority/generation/key, latest reservations/serial/revocation and externally published certificates, exact correlation, instance exclusivity and stale-proof invalidation. Missing/uncertain source means DENY; retain resources, do not reduce high-water/re-sign/automatically rotate CA.
4. Demonstrate Stop/quiescence/lifetime, crash before/after commit/CURRENT/ack, journal truncation, stale concurrent writer, restored clone and proof-revocation behavior with real approved mechanism. Logic fixtures do not qualify native exclusivity.
5. Charge descriptors/stack/copies/checkpoint coexistence and retention under unchanged2MiB before runtime hookup. Preserve platform independence, normal PST0.6.5 candidate, pin0.6.4 and denied admissions. CNG integration cannot bypass these gates.
