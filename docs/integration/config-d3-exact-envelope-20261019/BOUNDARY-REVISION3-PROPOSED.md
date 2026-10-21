---
status: proposed
revision: 3
scope: configuration candidate/base-root linkage and future same-root envelope
---
# Exact focal decision proposed — not accepted

D1. LST1 revision1 remains exactly as accepted: a historical provenance fragment for R_base/A_base/P_base/I_base and matching existing BODY05 snapshots. It is not the encoded new configuration and cannot supply R_next/A_next/P_next. An administrative source candidate must retain a distinct reference to this base and its expected CAS predecessor. No new operational identifier or epoch is created. Source intent, candidate and CURRENT are not interchangeable.

D2. Separar explicitamente BOOTSTRAP_INITIAL de UPDATE_CURRENT. Ausência de CURRENT não autoriza nenhum dos modos sozinha. BOOTSTRAP_INITIAL exige operação administrativa explícita e autorizada, instalação/store alvo factualmente nunca inicializado, nenhuma root/CURRENT/história anterior, nenhum resultado pendente/incerto ou estado retido e candidato validado por todos os participantes aplicáveis. Reutilizar a prova de caminho/identidade e a criação exclusiva já governadas; verificação de ausência antes da abertura não substitui a criação exclusiva no commit. Store existente, vazio/truncado/corrompido/inacessível/parcial, falha de consulta, resultado de criação incerto, backup restaurado ou histórico não reconciliado não são instalação inicial: DENY. Não renomear/apagar/substituir store para tornar bootstrap elegível. Nenhum pin, R/A/P anterior ou LST1 base é fabricado no bootstrap. Presença de contexto-base na envelope é explicitamente ausente nesse modo; isso não relaxa o header obrigatório do LST1 aceito.

D2.1 No bootstrap, referências só podem vir dos mecanismos administrativos canônicos existentes, com autorização do ator/configuração e identidade/integridade/lifetime factuais: configuração Secure selecionada e validada pelo source/material loader existente, refs e leases exatas de trust/certificados, catálogo Site TLS administrativo já aprovado quando aplicável e policy/capabilities do modelo validado e limitado pelo target. Esses mecanismos não dispensam prova de autoridade por estarem presentes. Falta de prova do ator, loader, material, catálogo, referência ou lease mantém DENY; não usar fixture, callback sintético, hash esperado ou texto como autorização. Não criar provider, CA, Principal ou grants. ALL pode ser intenção de binding sob os validators existentes; SELECTED mantém DENY por aplicabilidade topológica; provider nomeado PENDING; CLOSED/CERTIFICATE_IDENTITY permanece sem suporte no primeiro codec. O contrato não declara implementado nenhum resolvedor de bootstrap.

D2.2 UPDATE_CURRENT exige CURRENT factual na root correspondente, revisão-base exata, pin/lease válido durante validação/uso, conteúdo e referências atuais e CAS contra a seleção esperada. Proteger coexistência de gerações/Sessions e revalidar autoridade/Stop antes do efeito. Snapshot obsoleto, pin apenas histórico, conflito/UNKNOWN, referência divergente ou insuficiência de slots/reserva implica DENY/retenção conforme contrato; nunca fallback para bootstrap ou CURRENT anterior. Não enfraquecer source_resolve existente: ele continua sendo leitor de CURRENT, não fonte de autoridade inicial. Preparação do candidato utiliza os mesmos participantes e owner com autoridade do modo explicitamente demonstrada, sem promover MODEL_BOUND por flag do chamador.

D2.3 Ambos os modos preservam PREPARE -> VALIDATE -> COMMIT durável comprovado -> CURRENT -> emissão/vínculo de A/P pela autoridade existente. R/predecessor de candidato não são CURRENT; A/P novos nunca são preenchidos antecipadamente para satisfazer encode. LST1 continua fragmento histórico offline. No bootstrap não há LST1 de base; após publicação factual ele poderá representar contexto comprovado, sem autorizar acesso. Atualização só preserva P se a projeção operacional for comprovadamente idêntica conforme decisão issuer/C2; nenhuma concessão implícita de grants. Falha pós-commit ou resultado incerto conserva dados, histórico e recursos, sem recriar bootstrap/reutilizar serial. Nenhum modo concede admissão ou executa publicação nesta tarefa.

## D3.0 Scope and sole authority

Only D3 is replaced. D1/D2/D2.1/D2.2/D2.3/D4/D5 of BOUNDARY-REVISION2-PROPOSED.md remain byte-for-byte unchanged. This is not ACCEPTED and authorizes no code, store, epoch, credential, network or publication. ROOT8 below is a proposed private serialization discriminator, not a new root, registry, journal, worker, operational identifier or authority. The existing root remains the sole transaction/publication authority. No existing BODY, ROOT06, PACICP01 or LST1 byte/reader/hash domain changes.

ROOT8 carries the complete candidate listener collection in BODY0.5 entries; metadata is constrained redundancy and references, not a second policy source. Existing BODY0.5 is the sole content encoding of each candidate model. LST1 is optionally embedded only as a historical base fragment; never candidate CURRENT and never a substitute for the collection. A former singular model is not copied into multiple entries to pretend collection support. The current target still rejects cardinality above one and unsupported profiles before effects. Byte validity never implies target support, RESOLVED, PREPARED, COMMITTED or CURRENT.

## D3.1 Bytes, count and regions

Outer header/checkpoint/descriptor integers are explicit unsigned LITTLE-ENDIAN. Embedded BODY0.5 and ROOT06 issuer carrier preserve their original BIG-ENDIAN codecs; embedded LST1 preserves LITTLE-ENDIAN. Do not endian-convert embedded images. No native struct, pointer, size_t, Win32/SID/handle, secret, runtime interface index/address or compiler padding is serialized. There is no alignment padding between regions. Reserved bytes are zero and cannot carry inferred extensions.

Let N be candidate listener count1..4. For UPDATE let B be base listener count1..4 and L=160+512*B (one complete accepted LST1); for BOOTSTRAP L=0 and B is absent. J is0 or16384 for the optional existing ROOT06 issuer carrier. Bootstrap first profile requires J=0 and no I; updates may carry J as defined in D3.4. Total T=1056+1536*N+L+J, exactly2592..25792. Max is N=B=4,J=16384. Count0 is not a complete root configuration; the offline LST1 codec's count0 remains unchanged and does not create a default listener. No trailing bytes, unreferenced gaps, overlapping/reordered regions, alternate offsets or shorter zero-filled substitutes are accepted.

| Region | Offset | Length |
|---|---:|---:|
| ROOT8 header |0|512|
| Ordered candidate records |512|1536*N|
| Base LST1 or absent zero-length region |512+1536*N|L|
| Existing issuer carrier or absent zero-length region |512+1536*N+L|J|
| Publication checkpoint slot |512+1536*N+L+J|512|
| Whole-image digest W |1024+1536*N+L+J|32|

Absent zero-length regions use their calculated boundary offsets, not arbitrary pointers; they contribute no bytes. Checkpoint is always physically512 bytes, either all zero (no publication witness) or exact CP8 format. Distinct meanings: candidate R is content revision of the pending transaction; root CURRENT is an actual published selection; a stored checkpoint records a later observation and grants neither.

## D3.2 Header512 matrix

|Offset|Bytes|Field/rule|
|---:|---:|---|
|0|8|ASCII PACROOT8, discriminator distinct from every historical reader|
|8|2|format version1|
|10|2|header size512|
|12|4|T exact, within2592..25792|
|16|4|flags: bit0 UPDATE; bit1 ISSUER_CARRIER; all other bits zero; bootstrap flags0|
|20|2|N1..4|
|22|2|candidate descriptor size512|
|24|2|BODY size1024|
|26|2|hash algorithm1=SHA256, no fallback|
|28|2|checkpoint size512|
|30|2|base LST1 version1 for update,0 bootstrap|
|32|4|collection offset512|
|36|4|collection length1536*N|
|40|4|LST offset512+1536*N|
|44|4|LST length L|
|48|4|issuer carrier offset512+1536*N+L|
|52|4|carrier length J|
|56|4|checkpoint offset Q=512+1536*N+L+J|
|60|4|checkpoint length512|
|64|4|whole digest offset Q+512|
|68|4|whole digest length32|
|72|32|root domain/identity, not all zero; supplied by existing root|
|104|8|candidate R, nonzero; not a CURRENT flag|
|112|8|predecessor R_base; R=predecessor+1 without wrap|
|120|8|I of selected issuer content if present; otherwise zero|
|128|4|presence: bits0=root,1=R,2=predecessor mandatory; bit3=I iff carrier; bit4=BASE iff UPDATE; all others zero|
|132|2|candidate model codec5=BODY0.5|
|134|2|candidate descriptor version1|
|136|8|R_base if UPDATE, otherwise zero|
|144|8|A_base if UPDATE, otherwise zero|
|152|8|P_base if UPDATE, otherwise zero|
|160|8|I_base if base issuer present, otherwise zero|
|168|4|base presence0 bootstrap; UPDATE bits0=R,1=A,2=P mandatory,bit3=I optional; other bits zero|
|172|4|zero reserved|
|176|32|expected predecessor commit-content digest C_base for UPDATE; zero bootstrap|
|208|32|expected predecessor whole-file digest W_base for UPDATE; zero bootstrap|
|240|2|prior envelope kind0 bootstrap,1=ROOT8/version1 for UPDATE|
|242|270|zero reserved|

For bootstrap R=1/predecessor0; all base fields, both predecessor digests, LST version/presence/length are absent/zero as above. Candidate root domain is a proposed root identity assigned by existing owner, not a serialized proof of virgin installation. Exclusive creation and administrative authority remain D2 external facts. For UPDATE root domain equals the actual pinned base root; R_base=predecessor, A_base/P_base nonzero and exact; I_base presence/value matches that pin and base LST1. I absence is explicit; not inferred by comparing local counters. Base C/W refer to the SAME exact selected image. Commit digest equality does not replace native path/identity/instance validation or CAS. A_new/P_new never occupy header base fields.

## D3.3 Candidate record1536 and reference binding

Physical record k=0..N-1 starts512+1536*k: descriptor512 followed by exactly1024 bytes from the existing BODY0.5 encoder. Order is normalized configured listener order, not sorting by ID/address; no missing/skipped entries. IDs are unique, case-sensitive and stable in root scope. Two sockets from one binding do not create two logical records.

Descriptor fields are independently specified here, NOT a standalone LST1 record or alteration to LST1:

|Relative offset|Bytes|Field/rule|
|---:|---:|---|
|0|4|presence: id/binding/port/profile/authkind/policy bits0..5 mandatory,security bit6 conditional,auth_ref bit7 conditional; other bits zero|
|4|1|profile Secure1/Legacy2, zero/unknown invalid; support gate remains separate|
|5|1|admission OPEN1/CLOSED2, mandatory enum no default|
|6|1|binding ALL1/SELECTED2|
|7|1|authkind NONE0/CERTIFICATE_IDENTITY1; no implicit authentication|
|8|2|port1..65535|
|10|2|listener_id length1..64|
|12|2|security_ref length|
|14|2|auth_ref length|
|16|2|policy_ref length1..64|
|18|2|interface count0..8|
|20|4|zero|
|24|64|listener_id bytes and zero tail|
|88|64|security_ref bytes and zero tail|
|152|64|auth_ref bytes and zero tail|
|216|64|policy_ref bytes and zero tail|
|280|32|policy identity exact match to BODY policy, not all zero|
|312|32|SHA256 of full1024 BODY bytes including its original trailer, not all zero|
|344|8|ASCII PACCD001, explicit candidate descriptor discriminator|
|352|32|zero reserved|
|384|128|eight16-byte entries: presentU8=1 +7zero +PersistentIDU64 LE; used count entries then all zero|
|512|1024|BODY0.5 image, original encoding/integrity unchanged|

Text/reference validation is exactly accepted LST1 C2's identifier regexp, UTF8 decoded strings, bounds, nonsecret restrictions and zero tails. These shared constraints do not reuse reserved LST1 bytes: a candidate descriptor has its own discriminator. Secure requires security presence/ref; Legacy forbids it. NONE forbids auth_ref. CLOSED/CERTIFICATE_IDENTITY remain recognized candidate representations but UNSUPPORTED in this first root profile, preserving grammar/ADR0028 and provider PENDING. No reinterpretation as channel identity, no password/token storage. Unsupported active requirement rejects the whole transaction, not drops a record.

ALL requires count0. SELECTED requires1..8 unique valid PersistentIDs (zero value allowed with present1) but remains denied for actual preparation/application until topology applicability proven. No name/volatile-index/IP fallback. The PersistentID array is administrative intent, not a persisted runtime target.

BODY domain/R/predecessor must equal header domain/R/predecessor. Existing BODY04/05 requires its internal policy_epoch field zero; actual A/P never derive from it. Port/profile/admission/bind intent/IDs/security_ref and policy identity must equal the descriptor and decoded canonical model; mismatch rejects the entire envelope. Model's other administrative refs/policy rules/Site TLS remain governed by BODY05, not overwritten by descriptor labels. Policy_ref is the candidate source label for that exact inline policy model, not a second registry. Security/auth/material refs require exact factual authority/leases under D2 before PREPARED; decoding mere names does not resolve them. Source-to-record mapping and no unused declared objects use the existing bridge. No administrative refs are silently omitted from model content.

LST1 base entries correspond to base models, not these new candidate models. The old base pin/content verifies its own full collection/body digests and security/policy refs; candidates may legitimately differ, subject to all operational projection/authorization gates. Same-ID equality is not authority. No new C2 grant or epoch can follow a descriptor's data alone.

## D3.4 Optional existing issuer carrier

If I present, J=16384 and flag bit1/presence bit3 are mandatory; otherwise J=0 and both absent. The component is the existing ROOT06 prefix: unchanged8192-byte configuration store anchor + unchanged8192-byte issuer image. It is NOT a new issuer codec. Its own existing discriminators/hash/binding/root/high-water/history/generation checks remain required. Header I equals factual issuer revision from the existing validated issuer image; issuer root equals outer root. The carried configuration anchor is solely the immutable issuer-image hash anchor/history, never an alternative active listener collection or fallback policy.

The outer candidate BODY collection alone supplies candidate listener models; archived carrier configuration cannot authorize a listener/Session. Carrier may remain byte-identical when operational config advances; I is not outer R. It may change only through already authorized issuer operations validated under the SAME root, never inferred as I=R. Existing issuer format's internal revision/anchor relationship stays byte-exact within carrier. No replacement of its anchor to match new listener models, no loss/reset of high-water, reserve/revocation history or CA generation. A base issuer cannot silently disappear or regress. Compare actual pinned base carrier and exact event prefix/high-water/status; unproved transition denies. First bootstrap profile has no carrier; this does not authorize CA initialization. Adding issuer requires its own existing administrative/issuer gates.

This container defines no native-file identity shortcut: component hash inspection is data validation, not adoption of an arbitrary historical file. Original origin/leases/identity must be established by the same root during preparation. If the existing issuer adapter cannot safely validate/use this carrier as a component without confusing its anchor with active CURRENT, integration is DENY until a separately reviewed adapter patch proves that separation. No native issuer readiness or production migration is declared here. Carrier never contains PACICP01; historical512-byte checkpoint is not repurposed.

## D3.5 Hashes, CP8 and generation

All SHA256 calls use existing approved adapter; absence/failure is INVALID or INCOMPLETE as applicable, never success/fallback. Three exact domains:

- C (commit-content digest) = SHA256(file bytes[0,Q)). Contains discriminator/header/base links/full models/LST1/carrier, excludes checkpoint and whole trailer. No stored C field inside its own domain. Magic/version are covered; no alternate normalization or zeroing convention.
- Hcp = SHA256(checkpoint bytes[0,480)), stored checkpoint[480,512).
- W (exact image digest) = SHA256(file bytes[0,Q+512)), stored final32 at Q+512. Includes checkpoint exactly as stored, whether absent all-zero or present. No excluded header fields.

A generation is the existing root identity + R + C, a correlation tuple, not a new counter, epoch or wire ID. CP-only persistence changes W but not C/R/I/model content. Hash equality cannot authenticate a writer, prove latest global state, grant CURRENT or fence another installation.

CP8 checkpoint slot is either512 zero bytes (explicit absence) or:

|Offset in checkpoint|Bytes|Field|
|---:|---:|---|
|0|8|ASCII PACR8CP1|
|8|2|version1|
|10|2|size512|
|12|4|presence bit0=I iff outer I; others zero|
|16|32|same root domain|
|48|8|same committed R|
|56|8|same predecessor|
|64|8|actual A after publication, nonzero|
|72|8|actual P bound by existing policy/root after commit, nonzero|
|80|8|I if present, otherwise zero|
|88|32|computed C of this generation|
|120|32|same C_base as header (zero bootstrap)|
|152|32|same W_base as header (zero bootstrap)|
|184|296|zero reserved|
|480|32|Hcp|

Any nonzero slot not satisfying every CP rule rejects the image; do not treat a corrupt checkpoint as absent or strip it and recover. CP presence does not mean reader has authority: it is historical publication evidence requiring external custody/freshness/fencing validation. Existing PACICP01 BIG-ENDIAN/version1/domain16384 is never accepted as CP8 or vice versa. No header flag can predeclare A_new/P_new. Serial/high-water/generation are preserved within issuer carrier, not regenerated from CP fields.

## D3.6 Transaction and exact CAS

PREPARE owns immutable source/candidate/models, authorized refs/material leases, expected base full bytes/identity and required budget in existing root participant; VALIDATE checks full collection and mode D2. This proposal does not execute either. Bootstrap uses exclusive virgin creation with external absence/history proof; expected prior image is ABSENT, not an all-zero byte blob/digest masquerading as predecessor. Initial candidate R1/predecessor0/LST absent/CP zero. Update requires actual base CURRENT pin with exact R/A/P/I, complete matching base LST1, exact prior bytes plus C_base/W_base and native identity. This version admits only prior ROOT8/version1; ROOT06 and old checkpoints are recognized historical formats but NOT accepted update inputs. No silent migration; migration is a separately governed explicit gate. Same authority/store/worker, no second journal.

COMMIT first writes one complete image with CP all zero under the existing durable compare-and-commit semantics. Compare native store identity and exact full previous bytes/length, not revision/hash alone; exclusive-create only for proven bootstrap. CAS conflict/UNKNOWN invalidates use of historical authority for the new effect and retains packet/buffers/pins/credit; no fallback, blind overwrite/retry or bootstrap reset. A valid computed C/W without factual durable success cannot become CURRENT. Failed/partial writes cannot be decoded as accepted prefixes. The existing worker must own all bytes until factual completion; Stop does not wait for it or destroy uncertain handles.

After commit confirmed, the existing root performs CURRENT/publication confirmation and issues/binds actual A/P per accepted root/issuer-C2 semantics. No observer may receive CURRENT positive while its mandatory publication context/ack is incomplete; a partial in-memory selection cannot authorize effects. No epoch before commit; no new source of epoch; I/R/A/P stay distinct; preserving P requires factual identical operational projection. Sessions retain immutable snapshots/pins; no retroactive grants or reuse. Complete publication evidence is copied into CP8 only AFTER those facts exist. Persist CP8 with the SAME worker's checkpoint-only CAS against exact previous zero-CP image; all prefix bytes[0,Q) and R must remain byte-for-byte identical (not merely digest-equal), W changes. Computed C remains identical; only checkpoint and whole trailer may differ. No arbitrary checkpoint edits or epoch selection.

CP persistence is not another root revision/authority transaction or a new worker. If it fails/conflicts/is UNKNOWN, retain both images/results; do not fabricate historical proof or retry destructively. A root may have an in-memory factual publication and missing durable checkpoint; restart recovery remains DENY and next administrative update is denied until that result is reconciled and checkpoint durability proved. Stop is never forced to wait for reconciliation. First version allows checkpoint absent->one exact factual CP only; replacement/revocation of an existing CP is not authorized by rehash. Subsequent update requires the predecessor image with complete, durable matching checkpoint AND live actual CURRENT proof. Saved CP alone cannot provide that live proof.

## D3.7 Validation/recovery and historical dispatch

Decode validates pointer/range/disjoint-output/lifetime bounds before access, header512/magic/version/T/count/offset equations, all reserved/presence/enums, unique IDs, no gaps/trailing, existing BODY and LST1 formats/relations, carrier integrity, optional CP and W. Use immutable input until copied; no borrowed view after pin release. Output is data-only with explicit INVALID/INCOMPLETE, not resolved/current. Never accept part of a mixed-generation collection. Overflow/wrap/hash failure/unknown version/invalid UTF8/unknown required refs denies. Target cardinality/profile/provider/topology/security gates still apply even after valid decode. Do not resurrect old CURRENT from retained snapshots.

Relative rollback detection requires a separately retained proven baseline: lower R, different C at equal R, changed predecessor/domain or unproved CA-generation transition, issuer high-water/event-prefix/revocation regression, or missing/divergent CP where that baseline recorded one -> ROLLBACK/DIVERGENT/INCOMPLETE; preserve evidence. For a single next generation, verify base digest/tuple against actual prior selection. A jump cannot be made authoritative from a single hash link without the required intervening history. Same-R checkpoint-only zero->present is assessable solely as D3.6's exact transition; replaying zero after proven CP is regression. Even a perfect relative match is not latest global state.

Unknown/missing prior content needed to validate LST1/model history remains INCOMPLETE. No guessing, restore-as-bootstrap, new CA generation, serial reuse or issuer reset. Crash before commit: candidate not CURRENT; uncertain commit: retain and deny; crash after durable commit/before CURRENT: CP absent, deny authority; crash after actual CURRENT/before CP durable: deny authority after restart; CP durable: data/history can be checked, but independent checkpoint freshness and fencing still required. AUTHORITATIVE_RECOVERY=DENY in all cases in this slice. Corrupt/unknown formats do not select an older historical fallback.

Historical formats remain their exact size/discriminator/endian/hash domains and existing explicit readers. New ROOT8 reader rejects them as ROOT8; dispatcher may report HISTORICAL_NOT_APPLICABLE, never truncate a ROOT8 file to feed a ROOT06 reader. No auto import, reinterpretation, old magic inside new reserved bytes or rewriting historical evidence. This contract does not open a migration or widen ADR0052/C8 CONTROL exceptions.

## D3.8 ED11 and gates

Serialized size is not live reservation. New minimum2592; maximum25792. At maximum the existing three pending byte buffers, if widened to E, require3E=77376 instead of50688 (delta26688). One separate committed E plus those3 is4E=103168 instead of67584 (delta35584). These are exact byte-array subtotals under the existing separate-buffer implementation, not a whole-root price or permission to allocate.

Two old full encoded generations retained in addition to current committed and three worker images add2E=51584, subtotal6E=154752; if a design instead holds decoded immutable models or shares proven immutable storage, price its actual ownership graph, not this example plus the same bytes twice. Slots/pins/decoded views/CAS packets/material leases/AST59880/input8192/LST1 normalized2176/encode-decode scratch/BODY1024/hash context/CRT/native provider/kernel/worker stack/old-new coexistence are additional where truly live. The existing current I snapshot bytes cannot silently disappear or be counted twice as a full carrier. Native identity/provider allocations and total transitive stack stay UNKNOWN until evidence. Historical limit2097152 does not increase. No feasibility/headroom/reservation PASS from these subtotals; implementation must compute actual sizeof/multiplicity and checked-add against actual reserved graph before effects. Insufficient capacity denies rather than shrinking collection, dropping history or evicting pinned generations.

Implementation needs official exact patch/preimages after normative acceptance, strict portable codec and explicit native adapter/lifetime/CAS review. No implementation or operational admission in this task. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY; CURRENT publication denied. Browser/PST/PACC wire/production0.6.4 unchanged; PST0.6.5 remains candidate until full Phase6 completion.

D4. Preparação sem persistência distingue SOURCE/PARSED/VALIDATED/MODEL_BOUND/RESOLVED/PREPARED. Para avançar além de MODEL_BOUND, exige prova factual dos participantes e da autoridade do modo D2: bootstrap inicial comprovado ou base CURRENT válida, nunca CURRENT fictício no bootstrap. Sem contrato aprovado da envelope, o candidato pode ser inspecionado offline, mas não serializado/inserido como transação completa na root nem tratado como COMMITTED/CURRENT. Não criar caminho paralelo de owner, pins ou persistência. Esta proposta não aprova patch funcional.

D5. Named provider remains PENDING; SELECTED topology applicability DENY. CLOSED/certificate identity remains representable/unsupported by first codec, without implicit authentication/grants. Listeners remain a collection with explicit profile and runtime cardinality rejection, never truncation/Legacy fallback. CAS conflict/UNKNOWN, Stop, retained buffers/pins/credits and Session snapshots follow existing Model E/root/issuer-C2 rules. No quota increase; complete compiled joint pipeline census required. Provider/kernel UNKNOWN. CLIENT_ADMISSION=DISABLED, SITE_TLS_ADMISSION=DENY, ED11_ADMISSION=DENY. No network, Browser/PST/wire/production0.6.4 change or PST0.6.5 release.

Owner decision needed: confirm the distinct historical-base/candidate interpretation D1-D2 and authorize exact envelope contract preparation under D3, or specify a different existing accepted contract/source that already supplies this composition. None is accepted by this document or by executor approval of its storage.
