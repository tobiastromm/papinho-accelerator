# D3 bounded no-carrier memory and lifetime proof

Scoped implementation: pc_root8_encode_no_issuer / pc_root8_decode_no_issuer in the existing portable codec. Exact source-operations.json and decode-operations.json approved by official executor; pre/postimages and function hunks preserved. General encode/decode, wire/layout, BODY05/LST1/hash validation, root/issuer/Session/Stop/reaper and all reservations unchanged. No new normative decision or operational authority.

## Exact footprint and applicability

Accepted D3 equation T=1056+1536*N+L+J. Current native carrier adapter is UNSUPPORTED. For J=0,N<=4,L<=2208, T<=9408 and owned fields<=9376. The new path allocates neither a full25760 model nor a25792 encoded output: actual caller arrays are9376 and9408, total18784. Full general API/maximum25792 retained for separate callers. Carrier is explicitly refused, never stripped or truncated; this is not issuer carrier implementation. Four candidate AND four base listeners remain representable. Bytes for all16 update count pairs match general encoder byte-for-byte and decode to identical owned fields. No AST/format change.

Before ALL:1974203+85560+25760+25792=2111315, excess14163.
After ALL:1974203+85560+9376+9408=2078547, margin18605.
SELECTED after2078563, margin18589. Delta-32768. Limit2097152 unchanged.
These are the SAME previous diagnostic coexistence scenarios with one bounded codec operation, NOT the complete future ROOT8 product allocation. No root/issuer/input/AST caller reserve reduced. The16384 existing scratch allowance remains; explicit shape locals2720 excluding scalars remain, no second BODY view charge added. Complete transitive native/CRT stack and provider/kernel not qualified. Future root pending/committed images, pinned generations and issuer snapshot/carrier multiplicities require their own actual integration census; none widened or removed here. No operational reserve approval or total ED11 PASS.

## Ownership by function

Encoder: caller owns immutable bounded fields and exclusive disjoint output/used storage for synchronous call. Checked ranges/capacity/overlap before writes. Unsupported carrier rejected without callbacks. Full existing shape, BODY adapter, LST1 base validation and SHA checks precede copy. No callback retains/reenters; caller lifetime contract unchanged. Failed hash clears used and never publishes partial bytes. Invalid preconditions write nothing; caller must use return, not previous bytes. Output can be retained independently after successful copy. No lock, wait, pin, heap, worker or release acquired.

Decoder: immutable encoded input and disjoint caller-owned9376 output plus completion. Safe preconditions first; then output and completion cleared. Carrier refused. Full SHA/shape/component checks before completed owned copy. No input pointer escapes; tail remains zero. Input can be retired only after synchronous return and all OTHER factual users have finished. Decoder does not retire it. No owner/Session mutation.

Input and output coexist throughout each call: no aliasing or claimed mutual exclusion. Test transitions encode->decode only after synchronous return, when this codec has no retained borrow. Production retained/CAS UNKNOWN buffers cannot be recycled by this API. Each concurrent operation needs its own separately charged pair; two pairs plus the selected scenario exceed the cap and must be denied, never shared. Existing pinned snapshots, issuer generations, worker buffers and credit remain untouched. Stop/UNKNOWN/conflict/timeout do not trigger reuse, release or fallback. Caller must not invoke native hash on owner event loop/Stop; existing contract unchanged. Codec supplies no bounded native latency guarantee.

## Validation scope

Debug and Release /WX builds and linkage pass, zero compiler warnings.11 selected CTests per config and6 existing dormant root cases per config:34 final invocations pass. ROOT8 extended tests cover all16 count pairs, exact equivalence/owned decode, maximum no-carrier buffers, undersized capacity/no writes, overlap, corruption/output clearing, failed hash, carrier refusal and previous output preservation without authority. Existing codec suite covers every-byte corruption/truncation, bounds, old formats and data-history conflict/missing history; LST1 suite remains green. GCC strict C90 -pedantic-errors -Wall -Wextra -Werror passes. SDK46/46 hashes verified.

Synthetic histories/checkpoints/configuration are not native CURRENT/CAS evidence. Existing native BODY05/SHA calls validate bytes only. Native dormant root tests cover snapshot/copy/Stop/retention with NO_CURRENT NO_COMMIT NO_NETWORK. No new root participant CAS conflict/UNKNOWN/rollback/native publication was executed; the codec cannot perform those effects. No new root or retained-snapshot cleanup code changed. Python arithmetic joins compiled footprint and actual dormant budget logs; it does not qualify native headroom or provider/kernel. Box/ISA not qualified.

## Handoff to D3 canonical root integration

Use bounded no-carrier arrays only where explicit carrier rejection is preserved. Keep general codec API for independent callers; do not allocate both pairs simultaneously or call general decode into an undersized cast. Build actual preparation owner graph with separately charged retained predecessor, pending worker packets, immutable Session slots and every live buffer; no reserve subtraction by assumption. Guard checked coexistence against2097152 before allocation/effects, reject additional retained/concurrent work when insufficient. Do not call native hashing from Stop. Preserve existing single root/worker, CAS uncertainty ownership and snapshots/pins. No historical migration or carrier fallback. D3 root integration, CURRENT, recovery and all admission stay DENY until applicable transaction/lifetime/native ED11 gates qualify. No new architecture/roadmap.
