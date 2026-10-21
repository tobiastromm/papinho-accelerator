---
decision-id: ED11_PRESERVE_INCREMENTAL_RESOURCE_ENVELOPES
status: accepted
revision: 1
decision-date: 2026-10-12
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
---

# ED11 incremental envelope meaning and admission constraint

The owner explicitly selected incremental envelopes beyond a factual process baseline. Candidate Browser caps: managed2MiB, incremental user20MiB, incremental kernel2MiB. Accelerator: managed2MiB, incremental user32MiB, incremental kernel8MiB. Numerical caps remain candidates, not qualified limits or minimum RAM. This decision accepts their interpretation, not their adequacy.

ED11 evidence MUST separately record total process usage, pre-C2 baseline, attributed increment, peak, observable kernel resources, shared attribution, Model E retained resources and available target margin. Shared allocations are charged once. Unobservable attribution is explicit, never zero by omission.

Admission MUST consider both incremental quota and factual available host capacity. A fitting increment alone does not permit admission near process or system exhaustion. Retained lifetimes keep original quotas without destructive eviction. Unknown required evidence fails closed. This decision does not accept a guessed host-headroom threshold or opaque provider/kernel hard bound.

Authorized work remains isolated ED11 harness, accounting, admission and measurements. No PST, Browser, wire, API/ABI/SPI, pin0.6.4, C2/C3 production, E1-E12, promotion, commit, push, tag or release authorization is introduced.
