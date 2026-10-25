# Canonical storage hash closure LAB: proof and handoff

Task PACC-PHASE6-CANONICAL-STORAGE-HASH-CLOSURE-LAB-1, 2026-10-22.
Baseline main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91. The scoped LAB
capsule is implemented and qualified offline. It is not integrated into product,
issuer, root codecs, bootstrap/recovery or worker. No general backend was selected
for productive distribution and no accepted ADR was silently modified.

## Verified provenance before source ingestion

Official https://www.bearssl.org/ identifies release0.6 and links its archive/Git
interface. Official Git tags identify v0.6, tag object
7d8e767e79bb1750345e571ec89cca1da13b52df, commit
8ef7680081c61b486622f2d983c0d3d21e83caad. The independently downloaded official
release SHA256 is 6705bba1714961b41a728dfc5debbe348d2966c117649392f8c8139efc83ff14.
Official commit-snapshot SHA256 is
73cbdd89b82305e82d58154bccbd1f352ecc9dd097a0c19a7b0c4f09abd0fdaf.
The selected files from both archives matched byte-for-byte before copying.
HTTPS certificate verification was enabled; no upstream artifact signature or
independent signed checksum is asserted. Saved official tag HTML and response
headers, exact URLs and source hashes document this acquisition. No git clone,
Browser/PST copy or build-time network dependency was introduced.

The final snapshot is the three original C files (sha2small, dec32be, enc32be),
original bearssl_hash.h and LICENSE.txt. The closure-specific inner header keeps
three upstream bytewise encoding helpers verbatim with their MIT notice;
provenance.json distinguishes original files from derived header/compatibility
bindings. No new SHA arithmetic or algorithm is implemented. The shared upstream
SHA224 unit/vtable remains internal; the private capsule API always uses SHA256.
No TLS implementation is imported or linked.

## Exact approval and implementation scope

Official executor review approved exact source-ingestion and correction manifests
before functional LAB edits/execution. Adjacent operations manifests preserve
preimages/postimages. The initially inspected full header closure pulled in unused
TLS declarations with narrowing warnings; it was reduced to the actual SHA header
and exact encoding helpers instead of suppressing conversion warnings or changing
upstream arithmetic. Two missing prototypes and a linker /TC command placement
were corrected. VC6 required its expected existing integer shift helpers and one
test-only explicit byte conversion. All initial diagnostic logs are retained;
no failed intermediate build is counted as a qualified configuration.

Final source files live ONLY in tools/qualification/storage-hash-capsule.
Top-level CMake/product sources were not edited. memory_ops implements bounded
copy/fill glue to eliminate opaque CRT calls in the measured closure. Stop is a
borrowed volatile scalar, not an arbitrary function callback with unknown stack.
Hash contexts retain no input/Stop/output pointers after synchronous return.
Caller ownership, serialization, valid aligned spans and factual quiescence are
preconditions; arithmetic span validation is not virtual-memory validation.

## Equivalence and negative coverage

Each actual compiler/configuration passes350,879 checks, zero failures and zero
final emitted warnings. The40 public vectors have independent Python hashlib
digests, also verified by an independent BCrypt executable. They include empty,
abc, padding boundaries55/56/63/64/65, multiblock, every-byte and fragmented
1/7/64/1024 streaming, million-a and8MiB input. Additional raw spans cover480,
992,1152,8192,16384,30560/30592,31808/31840 and other storage lengths; marker
vectors include BODY/store/issuer/checkpoint/root/PDG/TSC/TSR domains.
These are mathematical public byte spans, NOT valid administrative images,
certificates, CURRENT, fixtures granting authority or rejected .papacc fixtures.
No prefixes/domains are appended, normalized or changed by the adapter.

Negatives exercise invalid NULL/positive spans, address-range wrap, output
capacity/context overlap, input/context overlap, counter maximum/overflow,
invalid/terminal states, busy exclusion, Stop before work, Stop between streaming
chunks, Stop before finalization, callback Stop and unchanged output on failure.
Two independent streaming contexts are live and interleaved; this is not a claim
of multithreaded owner/worker integration. Terminal math state is cleared and
remains caller-owned until synchronous quiescence. No native cleanup/retry,
lease fabrication, timeout release, partial digest or trust result exists.

Existing SHA256 byte semantics are demonstrated; no product call site was replaced.
This does not prove whole-codec/current/issuer equivalence after a future migration.
Storage-only consumers can later use the private callback without globally changing
issuer APIs, subject to exact scope and owner checks.

## Binary independence and compiler/runtime closure

All four closure.exe binaries execute successfully with /NODEFAULTLIB, no CRT
startup and PE import/TLS directories exactly0/0. Their linked graph has no indirect
or unresolved call, BCrypt, CryptHashCertificate2, EVP/provider, PST, Browser,
allocator, clock or network call. Original math uses local remapped copy/fill only.
The callback API means a SHA256 math callback, not a dynamic crypto provider.

Actual VC6 requires static __allshl and __aullshr (the Release optimizer can omit
one). Maps resolve them only to LIBC:llshl.obj/ullshr.obj. Installed LIBC.LIB SHA256
is a0fd53587e409caca8e7a7bc650399ddc982746cdc1280c094357051cca45977.
They are bounded compiler helpers included in the disassembly/stack proof, not
new arithmetic source, dynamic CRT initialization or a copied runtime library.
The capsule has no mutable global state, explicit heap or native resource handles.

The full test harness intentionally imports CRT/stdio services (modern UCRT/
VCRUNTIME or VC6 MSVCRT); separate BCrypt oracle intentionally imports BCrypt.
Those binaries are not the independence probe. Their host/CRT baseline is not
silently charged zero or promoted to capsule allocation. NT4/box/ISA remain
unqualified despite actual VC6 compilation/execution on the modern test host.

Flags are recorded in four scripts. Core uses C /Za /W4 /WX. Only VC6 C4127
constant do/while(0) and C4514 discarded inline diagnostics are specifically
disabled by the compatibility header; type/conversion diagnostics remain enabled.
Modern probe /GS- is a LAB measurement configuration, not product permission.

## ED11 measured bound for exact binaries

| Configuration | Context / two contexts | Direct streaming nested stack | Math callback nested stack including local context | Largest probe/capsule entry nested stack |
| --- | ---: | ---: | ---: | ---: |
| MSVC Debug x64 | 128 /256 | 816 | 1072 | 1152 |
| MSVC Release x64 | 128 /256 | 664 | 824 | 888 |
| VC6 Debug x86 | 128 /256 | 604 | 784 | 812 |
| VC6 Release x86 | 128 /256 | 528 | 684 | 704 |

All values bytes. Underlying BearSSL context is112 on all four builds. Direct
streaming stack excludes caller-owned context; callback stack already includes
its local128, so do not add it again. Two interleaved external contexts plus one
active streaming entry require known256+that entry's stack (1072/920/860/784).
Two truly concurrent callbacks would use two independent stacks:2144/1648/1568/
1368; that is a composition bound, not executed parallel owner qualification.
Caller buffers/Stop tokens, entry ABI home/arguments and OS startup/stack pages
remain caller/target categories outside these incremental closure numbers.

stack-analysis.py parses the actual linked machine instructions and maps. It
requires equal stack depth at CFG joins/back edges, finite fixed adjustments,
resolved direct acyclic calls, balanced returns and tailcall frame reuse. It
rejects unhandled stack instructions/recursion/indirect calls. The reported nested
bound includes return addresses, saved registers, arguments/spills and the two
VC6 helpers. It is not a local-array estimate, sampled watermark or CPU deadline.
Source loops do not grow the stack with input length. PE imports0/TLS0 and the
linked call graph close runtime/helper uncertainty for this capsule only.

| Configuration | Probe .text virtual bytes | Other readonly/relocation/unwind metadata bytes | Probe data/BSS bytes |
| --- | ---: | ---: | ---: |
| MSVC Debug | 7461 | 1256 | 336 |
| MSVC Release | 5376 | 1720 | 320 |
| VC6 Debug | 5503 | 412 | 336 |
| VC6 Release | 3967 | 412 | 336 |

These PE sizes include the small probe entry, two contexts256 and outputs64;
Debug/VC6 include16 other probe bytes. They are NOT a worker/process memory peak.
Raw PE file/section alignment and hashes are in ed11-capsule.json. Code/data are
charged once; shared readonly pages and full OS loader/host attribution are not
summed as independent heaps. Init has no allocator/native provider/TLS/static
initializer in this closure. Cooperative Stop allows at most1024 input bytes
between update polls; final padding has fixed bounded work. No target time bound.

## What remains denied

This proves deterministic memory and exact SHA256 for an isolated math closure.
It removes no existing live BCrypt reference and changes no managed process
ledger, quota, CURRENT or native worker. Certificate/status/UTC/operator/TxF/CRT/
host categories and canonical kind8/cohort/worker v4 gaps from the root-cause
report remain independent. Full ED11 stays PARTIAL, not inferred from this bound.
Next is the owner-requested prepared-storage integration only under separate
scope/executor approval, with exact call-site split and product/authority gates
preserved. No automatic production migration follows.
