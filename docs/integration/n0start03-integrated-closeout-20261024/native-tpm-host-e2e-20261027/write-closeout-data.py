from pathlib import Path
import json,csv,hashlib,subprocess,difflib
root=Path.cwd();base=Path('docs/integration/n0start03-integrated-closeout-20261024');p=base/'native-tpm-host-e2e-20261027';sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest();request=p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json';rh=sha(request);ex=sha(p/'manual-candidate/NativeTpmProof.exe')
(p/'MANUAL-CANDIDATE-HANDOFF.md').write_text(f'''# Pedido candidato: prova EK no host C nativo

Estado: PREPARED_NOT_APPROVED_NOT_EXECUTED. Este arquivo não autoriza execução.

Pedido exato: [NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json](manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json).
SHA256 `{rh}`.
Executável Selected Release copiado byte a byte: [NativeTpmProof.exe](manual-candidate/NativeTpmProof.exe), SHA256 `{ex}`.
Certificado público DER: `965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761`.
[Demais pins](MANUAL-CANDIDATE-PINS.json). O parser/contrato C real validou o pedido:450 nodes,14032bytes. Não existe OWNER-APPROVED-NATIVE-PROOF.json neste diretório.

## Decisão necessária

Uma NOVA aprovação específica do proprietário para uma única invocação manual deste pedido. As aprovações dos dois hosts gerenciados foram usadas; ausência de receipt antes do guard não as restaura. O identificador do candidato é inerte, não novo intent/identidade operacional Q4.

Após aprovação: registrar separadamente o kind OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY, request_sha256 e executable_sha256 exatos, one_manual_execution=true e os três reconhecimentos explícitos de revogação UNKNOWN, ausência de admissão de hardware e accounting interno UNKNOWN. Calcular o SHA256 literal do novo registro; revalidar os três arquivos e os pins compilados. Só então gerar a linha final --owner-manual-proof com ambos os hashes literais. Não foi criado registro aprovado nem comando operacional ativado neste pacote.

## Procedimento futuro condicionado

Usar Windows PowerShell5.1 x64 já elevado e o mesmo operador/host/boot237900 fixados. Não contornar UAC. Mudança de boot/token/artefatos exige revisão; não editar pins durante a execução. A execução manual será feita pelo proprietário uma única vez, sem retry.

O console deve satisfazer o guard REAL: exatamente pai e prova como clientes e um processo de console distinto. No console privado do ensaio, HWND pertencia ao cliente; o guard real negou corretamente. O ensaio positivo usou papéis administrativos modelados, SEM comprovar1024. Não alterar flags globais/herança/topologia para fabricar contagem. Se o ambiente manual não satisfizer o guard, DENY; não repetir para procurar ambiente favorável sob a mesma aprovação.

Confirmar exatamente `PROVE_EK_SESSION {rh}` quando solicitado pelo executável aprovado. Escape solicita Stop entre chamadas síncronas; não promete cancelamento interno. O receipt local CREATE_NEW é apenas evidência administrativa suplementar, não ledger Q4 resistente a rollback. Qualquer invocação usa a aprovação mesmo que ainda não crie receipt.

## Efeitos e STOP

No máximo um contexto TBS, uma sessão HMAC transitória com EK EXISTENTE e12 Submits diretos: GetCapability1, ReadPublic até9, StartAuthSession1, GetRandom1. Nenhum AK/CreatePrimary/NV/hierarchy/Clear/FlushContext. A contagem não abrange chamadas internas opacas do TBS.

Exigir pré-observador<=124, próprias<=128, pai+console<=1024, identidade PID/criação e LUIDs, nenhum impersonation, hash/ID/objeto original íntegros. Nenhum crédito/concessão de orçamento. Divergência, Stop, erro ou fechamento incerto congela aquisições/comandos; primeira falha preservada. Sessão sem fim autenticado/contexto incerto retém referências e dependências no processo vivo. Não matar, repetir ou adivinhar FlushContext; perda do host deixa UNKNOWN. Não limpar receipt/resíduos.

Devolver a saída integral NATIVE_API_COUNTS e JSON e qualquer mensagem UNKNOWN. Conferir query/count/error, fases/limites, Name, prova, context/session end, retained, artefatos e fechamento. Mesmo um PASS matemático não autentica geração física, revogação, custódia NV ou readback Q4.

Provisionamento=NOT_READY. E21=PENDING; ED11/Phase6=PARTIAL. Proibidos Q4/Start/Stop/SCM/ACL/CURRENT/signature/recovery/refund/cleanup.
''',encoding='utf-8')
(p/'OWNERSHIP-AND-BOUNDARIES.csv').write_text('''reference,acquisition,owner_and_borrow,lifetime,uncertain_close,measurement_scope
CNG providers,BCryptOpenAlgorithmProvider,TP_OWNER original slots only,dependencies before hashes/key until context/session known closed,retain provider and all dependent originals; no retry,logical reference not necessarily one process table handle
hash,BCryptCreateHash,TP_OWNER work_hash borrowed synchronously,one digest at a time; close before next acquisition,freeze and retain original work_hash and provider,held checkpoints count entire process table
RSA EK public key,BCryptImportKeyPair,TP_OWNER public_key original,certificate matched then all proof/final public checks,retain key/provider if destroy uncertain,no private EK material requested
certificate/parser public import,CertCreateCertificateContext/CryptImportPublicKeyInfoEx2,TP_PLATFORM cert then TP_OWNER work_key slot,temporary parse/import before protocol,retain cert/key and dependencies if close uncertain,real public certificate data qualified separately
artifact files,CreateFileW read with FILE_SHARE_READ,TP_PLATFORM four original slots no replace-by-hash,held from qualification to final platform close,retain original file slots; UNKNOWN,no Q4 protected input opened; native IDs/hash logged separately
operator token,OpenProcessToken/OpenThreadToken,TP_PLATFORM token slot,query current token and LUID continuity at each boundary; no impersonation,retain original token and freeze,token fixture plus actual metadata/CNG resource observations
observer process refs,OpenProcess query-only,TP_PLATFORM parent and console slots; synchronous borrow,PID/creation and counts sampled together then known close,retain original observer references and freeze; no new observation,real guard tests with OS substitution; full positive role mapping explicitly MODEL
boot event/render,Windows Evt APIs,TP_PLATFORM query/event/render original slots,read metadata before artifacts and close all known refs,retain originals if EvtClose uncertain,actual read-only metadata matched observed boot; no device authentication
supplemental receipt,CREATE_NEW and checked Write/Flush,TP_PLATFORM token slot reused only after known token close,one administrative marker before TBS then known close,retain original handle; shortwrite/flush uncertainty is failure; no retry,not canonical durable ledger and not rollback witness
TBS module,LoadLibraryExW System32 only,RUNTIME original module,only after manual authority/confirmation/receipt; kept until context and CNG closed,retain module on uncertain release or any dependent unknown,not loaded by offline tests; on-disk hash only
TBS context,Tbsi_Context_Create,TP_OWNER context; no other owner,one native context for all allowed commands,session/context uncertain retains original and CNG/files; no guessed FlushContext,no real TBS open/submit/close executed
TPM session,StartAuthSession,TP_OWNER session scalar and nonce/key arrays,one session until authenticated response continue-clear,unknown keeps original context and secrets; no second session,mathematical software endpoint only
fixture software endpoint key,BCryptGenerateKeyPair synthetic,test endpoint private_key separate from proof public key,only software responder; destroy before proof provider after known session end,unknown intentionally retained in fault fixtures; process loss not confirmed close,not TPM EK; process samples include its real Windows/CNG overhead
''',encoding='utf-8')
(p/'TEST-COVERAGE.csv').write_text('''area,positive_and_negative_coverage,evidence,scope
protocol,C# exact KDF/319/48/89/HMAC/Public398/capability/AES and398 mutation decisions,Debug-FINAL-ctest.log;Release-FINAL-ctest.log,deterministic vectors plus native CNG software RSA endpoint; no TPM
session,zero/short32 output and every response-byte mutation; wrong HMAC/nonce; max8candidates12Submits,proof_test.c21modes and4375assertions each configuration,valid software endpoint does not attest real EK
ownership,first native error; contradictory handle; start malformed; context close uncertainty; key close; hash failure; repeat finish,proof_test.c modes1..20,seven retained model states until fixture process loss; not operational closure
resources,query failure vs count;124/125; real synthetic own>128; admin1025; role/PID-reuse/close freeze,platform_test.c plus proof_test.c,real process event pressure plus substituted guard APIs; no quota grant
identity,operator wrong; impersonation; ModifiedId during/after query; token query failure; wrong host metadata guards,platform_test.c58assertions and62with metadata option,actual host read-only metadata plus substituted negatives
artifacts,original file ID mismatch; content pin; incompatible write share; JSON duplicate/shape/overflow; actual DER import,artifact_test.c149assertions,synthetic files and real public DER; no real approval created
replay_receipt,CREATE_NEW duplicate; six concurrent creators exactly one winner; precreate Stop; write/shortwrite/close errors,artifact_test.c,local administrative once receipt only; no rollback-resistance claim
console_flow,clean exact line; wrong line; Escape; errors propagate; clean key/context/provider/file close,Debug-FINAL-CLOSE-ctest.log;Release-FINAL-CLOSE-ctest.log,real private console and CNG; modelled authority/artifact payload/EK/admin roles; TPM mocked
console_topology,private pseudo-console HWND client owner rejected by unchanged real guard,console-fixture-Release-02;console-fixture-Release-03,negative factual observation; genuine admitted topology not demonstrated here
no_authority,noargs DENY; selfcheck CNG only; candidate parser data-only; no approval JSON in candidate,FINAL-ctest logs;REQUEST-DATA-CHECK.log,zero real TPM/API commands
''',encoding='utf-8')
# Update E21 evidence while preserving every existing field's meaning and historical contents.
f=base/'ED11_GATE_MATRIX.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);g=next(x for x in rows if x['gate']=='E21');g['current_evidence']+='; native C complete control-flow qualified offline:2MSVC builds zero warnings;14CTest plus affected-close reruns PASS;4375core/149artifact/58guard assertions per configuration; model full peak91; original topology guard unchanged; native-tpm-host-e2e-20261027/CONSOLIDATED-CLOSEOUT.md';g['closure_evidence_or_action']='New exact native-C manual-proof decision then fresh real guards and TBS evidence; candidate '+rh+'. No reuse of managed approvals. Mathematical proof still does not grant hardware generation/custody/NV/Q4; those remain independently pending.'
with f.open('w',encoding='utf-8',newline='') as out:w=csv.DictWriter(out,fieldnames=fields);w.writeheader();w.writerows(rows)
# Assert no gate status was promoted or other row changed.
old=list(csv.DictReader((p/'ED11_GATE_MATRIX.csv.PREIMAGE').open(encoding='utf-8-sig',newline='')))
assert len(old)==len(rows)
for a,b in zip(old,rows):
 if a['gate']=='E21':assert a['status']==b['status'] and a['closure_class']==b['closure_class']
 else:assert a==b
live=Path('docs/capabilities/canonical-phase6-consolidation.md')
with live.open('a',encoding='utf-8') as out:out.write(f'''\n\n## 2026-10-27 — Complete native C TPM proof host qualified offline\n\nOpt-in standalone modern MSVC/x64 C host implements the salted protocol and guards without CLR. Debug/Release /W4 /WX PASS;14 CTests plus affected close-flow reruns PASS. Each configuration:4375 core/vector assertions,149 artifact/persistence/concurrency assertions,58 guard assertions and4 actual metadata checks beyond the repeated guard suite. Full mocked TPM control flow with real console/CNG sampled73..91 handles and84 after known controlled close; no real TBS cost/admission established. Authority/EK/admin roles in the positive console fixture explicitly modeled; unchanged real topology guard denied this test pseudoconsole. No credit or historical249-91 operational saving claimed. Native executable/request/certificate pins prepared inert; no approval or activated manual command. First actual remaining gate: exact new owner-manual request `{rh}`, then fresh real console/operator/token/metadata/budget/TBS evidence. TPM possession/generation/custody/NV and Q4 still absent; E17/E18 and target D4/D5/E19/E20 independent. [Consolidated qualification](../integration/n0start03-integrated-closeout-20261024/native-tpm-host-e2e-20261027/CONSOLIDATED-CLOSEOUT.md). E21 pending; ED11/Phase6 PARTIAL; source/history/operational pins and UNKNOWN/reservations preserved.\n''')
# Reviewable WIP-relative integration patch, not HEAD diff of unrelated historical WIP.
a=(p/'CMakeLists.PREIMAGE').read_text().splitlines(True);b=Path('CMakeLists.txt').read_text().splitlines(True);(p/'CMAKE-INTEGRATION.WIP-RELATIVE.patch').write_text(''.join(difflib.unified_diff(a,b,fromfile='CMakeLists.txt.before-task-WIP',tofile='CMakeLists.txt')),encoding='utf-8')
(p/'PARALLEL-ED11-CRITICAL-PATH.json').write_text(json.dumps({'E17':{'state':'R0-R9 accepted/offline; fresh service identity/effective access/readiness operational proof pending','source_reuse':'HISTORICAL-AND-REUSE-PINS.json','TPM_dependency':False},'E18':{'state':'safe Stop/lifecycle/resource release/global-quiescence proof pending; no historical inference','TPM_dependency':False},'E19':{'D2':'ACCEPTED_PREPARATION_ONLY','D4_D5':'maximumAge/validity/duration/physical/commit values still null in applicable target decision','provider_kernel':'UNKNOWN','TPM_dependency':False},'E20':{'state':'one real Browser/C2/C3/provider composition bound to accepted quantitative policy and actual root/CURRENT/epoch/SiteTLS/backend/destination still pending','managed_browser_bytes':1858020,'managed_limit_bytes':2097152,'difference_not_provider_headroom':239132,'TPM_dependency':False},'E21':{'native_C_host':'QUALIFIED_OFFLINE_SCOPED','actual_possession':'NOT_DEMONSTRATED','custody_NV_durable_generation':'NOT_DEMONSTRATED','Q4_readback_and_disposition':'PENDING_NO_AUTO_EXECUTION','next_request_sha256':rh},'E22':'DERIVED_PENDING_E17_E18_E19_E20_E21','counts':'4demonstrated/12limited/6pending unchanged','Phase6':'PARTIAL_INDEPENDENT_OTHER_GATES','no_redundant_campaign':'Unchanged E17/E18 sources hash-match; target quantitative input reviewed and copied as reference, no invented values/new operational identity'},indent=2)+'\n',encoding='utf-8')
print('LIVE_MATRIX_UPDATED_WITHOUT_PROMOTIONS');print('REQUEST='+rh);print('EXE='+ex)
