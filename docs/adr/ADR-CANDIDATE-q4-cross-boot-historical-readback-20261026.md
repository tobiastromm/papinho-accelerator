---
status: proposed
revision: 1
date: 2026-10-26
scope-level: project
supersedes: []
---
# Historical Q4 readback after boot change

## Problem and decision required
The original Q4 readback pins host hash, boot event RecordId236441 and boot start2026-10-23T18:36:34.5000000Z. Its compound HOST_BOOT_DENY cannot identify the mismatching field. The owner reports denial before either protected reader. No current host/boot observation has been supplied. Replacing those pins under the old consumed approval would change the authorized contract and is forbidden.

Propose a separate one-use manual documentary readback, only after explicit normative and exact operational approval. Preserve the original request, boot, approval and failed attempt unchanged. Same-host evidence must be established; different-host migration is excluded. Hash of Computer text is a name binding, not hardware authentication. Event RecordId is local log position, not a universal boot identity. Collect current computer/boot record/start through non-protected context observation, report each field independently, bind exact current context in the NEW request and approve it before protected access. Missing/ambiguous host continuity denies; no automatic adoption.

## Nonwaivable checks
Reuse original relative native handle readers and ordered RAW1 owner/group/DACL checks without normalization. Pin root/ancestors, known original sink and dossier native identities, image/content and exact historical records. Observe/revalidate original opened objects on the same handles; no reparse, path-hash substitute or existing-file adoption. Unprinted historical intent/image creation IDs remain UNKNOWN: a new current observation cannot prove continuity across reboot or recreate an old handle. R7 original-object proof must be assessed separately; if unavailable, report a limited historical observation and leave disposition denied.

Require authorized primary elevated operator, enabled Administrators, no impersonation, exact script/dependencies, exclusive one-use request and factual current process identity. In-process used flags are not durable anti-replay across process restart. Before operational implementation, designate the existing governed approval-consumption ledger and its durable exactly-once protocol; if no suitable existing mechanism can enforce consumption, STOP and return for review, do not invent a new registry or authority. A failed attempt consumes its approval; no retry.

## Threats and limits
Counter renamed/spoofed host metadata, log reset, recycled PID, file-ID reuse/substitution, stale receipt replay, descriptor changes and concurrent replacement. Boot change terminates the validity of old process/handle/freshness claims; it does not prove closure, global quiescence, credit release or original object continuity. Preserve all UNKNOWN and reservations. No production authority, new LAB, Start/Stop, SCM/ACL mutation, cleanup, refund or documentary disposition follows.

## Failure and ownership
Preserve first failure. No further acquisition after freeze/Stop. Uncertain close retains original references/dependencies and forbids retry/double-close; never declare CLOSED from process exit alone. Existing retention contract applies. Success is scoped readback only, not R7 satisfaction or E21 PASS by itself.

## Qualification and acceptance
Isolated model covers independent boot fields, different host, current binding, replay, operator/token, object/RAW1/content substitution, Stop and UNKNOWN. It proves model predicates only, not native readers, durable replay enforcement, actual host continuity or authorization. Operational implementation must additionally qualify real original readers, concurrent substitutions, durable ledger failures and all cleanup paths before a separately approved manual execution. Original launcher and reader pins remain unchanged.
