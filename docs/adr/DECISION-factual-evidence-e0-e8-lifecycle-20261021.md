---
status: accepted
revision: 1
decision-date: 2026-10-21
last-revised: 2026-10-21
scope-level: project
scope-target: shared provisioning E0–E8 corrected factual evidence lifecycle
supersedes: []
decision-makers: [Tobias Tromm]
---
# Evidências factuais E0–E8 — aceitação delimitada da correção de lifecycle

## Autoridade e incorporação byte-exata

TASK=PACC-PHASE6-FACTUAL-EVIDENCE-LIFECYCLE-ACCEPTANCE-2. Owner concede aceitação condicional da correção exata E1/E6/E7 e E0–E8 reconciliado, sujeita a conformidade e aprovação oficial do executor. Este registro só entra em vigor após aprovação oficial destes bytes, gravação e verificação dos hashes. Não é autorização de implementação, migração ou qualificação factual.

Aceitar SOMENTE E0–E8 corrigidos, incorporados pelo anexo [E0-E8-corrected-frozen-clause-bytes.txt](../integration/factual-evidence-lifecycle-acceptance2-20261021/E0-E8-corrected-frozen-clause-bytes.txt), 11192bytes, SHA256 `73d9bc6d3759d5b157bf07c8d922d2097c6f5443fa9a2fb284ae4adb8c073d38`. Fonte original preservada: `docs/integration/shared-m1-factual-evidence-20261021/FACTUAL-EVIDENCE-CONTRACT-PROPOSED.md`, SHA256 `742088da6c361c277d423dbcb5cc3bc67b8bc638760d0f420a2242ccc416d937`, 9209bytes. A correção é EXATAMENTE o patch histórico `docs/integration/factual-evidence-e0-e8-acceptance-20261021/exact-lifecycle-corrections-PROPOSED.patch`, SHA256 `437e647b3753e3032d9b575f19cd7a39216e6da9c097a99640cfd3ead2dbf196`, 6007bytes, derivado de `docs/integration/factual-evidence-e0-e8-acceptance-20261021/LIFECYCLE-CORRECTION-PROPOSED.md`, SHA256 `761b5d90daaa192f7d513424ad2ffa3ee60beefe1b856727a62861b8417d97b4`. Patch reconstruído somente em memória para compor o anexo; não aplicado ao arquivo original. Intervalo corrigido incorporado [370,11562), a partir de ## E0 até EOF, sem título/introdução/status da proposta. Sem normalizar encoding/quebras de linha das cláusulas. [incorporation-and-preimages.json](../integration/factual-evidence-lifecycle-acceptance2-20261021/incorporation-and-preimages.json) fixa fontes, três hunks e invariantes.

E0/E2/E3/E4/E5/E8 permanecem byte-exatos em relação à proposta original. Somente E1/E6/E7 recebem as substituições históricas delimitadas. Texto preparatório, alternativa de aquisição assíncrona, patches funcionais e futuras versões não são aceitos por referência. Proposta original, correção candidata, relatório STOP anterior e ADRs accepted permanecem intocados como história.

## Precedência e fronteira aceita

Este registro encerra SOMENTE R1/R2 da revisão anterior para o perfil borrow-only expressamente descrito na correção aceita. Os termos históricos PROPOSED/necessita registro dentro das cláusulas incorporadas são superados apenas quanto à aceitação normativa desses bytes e assinaturas privadas; todos os gates de implementação dos tipos/fábricas/fontes, autorização própria, captura independente, qualificação e recursos permanecem. Nenhum header/fábrica/API funcional passa a existir por este registro.

S0–S8, M0–M8, D3, F1–F5/G0–G7, issuer/policy/root/C8 e Browser provenancev2 mantêm autoridade. O SDK verifica correlação/aplicabilidade, não adquire autoridade própria. Accelerator fornece observações administrativas; destinatário fornece confirmação independente; BrowserProfile decide instalação local depois da validação completa. Browser não conhece formato interno da root. Não criar trust store, registry, nova CA, worker, journal, root, provider geral ou sistema de pins. Persistências e wire permanecem byte-exatos.

## Leitura delimitada de tentativa e consulta

Há duas fronteiras distintas. A preparação/aquisição nativa factual pertence ao owner existente e não é implementada ou ampliada por este contrato. sp_trust_acquire é apenas ligação síncrona all-or-nothing sobre o grafo já adquirido/protegido/contabilizado. Não inicia I/O, aquisição nativa/assíncrona, nova alocação ou aquisição parcial de ref/pin/lease. Se tais efeitos forem necessários dentro da função, este perfil é inaplicável: DENY, sem fabricar lease ou reinterpretar RETAINED como erro final.

A identificação usa a identidade e o lifetime da operação existentes no owner produtor e a fase correspondente, preservadas antes de efeitos. Não é criado identificador global, nonce, contador, ABI de tentativa ou catálogo no SDK. Para uma preparação nativa anterior, a implementação dependente deverá demonstrar vínculo factual à operação original e sua consulta de resultado/disposição pelo MESMO owner, sem depender de lease SDK devolvido. Sem essa localização/consulta factual, nenhum efeito dependente está autorizado. A ausência de lease SDK nunca comprova ausência de recursos, inclusive numa preparação anterior retida.

| Fronteira/fato | Identificação e consulta | Ownership e ação |
|---|---|---|
| Preparação nativa anterior em curso, incerta ou retida | operação/lifetime original no owner; consulta factual desse owner | conservar grafo, pins, buffers e créditos; não chamar acquire como se estivesse pronta; binding DENY |
| Preparação anterior com falha definitiva | resultado factual da mesma operação, distinto de quiescência | não gerar lease; cleanup só após prova de quiescência/fechamento por seu owner |
| Preparação anterior concluída/protegida e condições E2–E5 válidas | referências/resultado factual já mantidos pelo owner | elegível para ligação síncrona, sem transferir autoridade ou mover owner |
| acquire APPLICABLE | retorno síncrono correlacionado à mesma operação e output completo | borrow completo; referências e lifetimes continuam no grafo dos owners reais |
| acquire DENIED/INVALID | retorno síncrono; output intacto | não adquire/transfere/libera ownership; recursos anteriores não são descartados |
| acquire pendente/RETAINED | NÃO ADMISSÍVEL neste perfil | não há consulta de pendência própria porque a função não a pode iniciar; necessidade disso exige contrato distinto antes de implementação |
| revalidate RETAINED | lease já conhecido da mesma operação | impedir KDF/conteúdo; conservar lease/grafo/créditos até encerramento governado; nenhum novo lease |

Consultar a preparação anterior é obrigação factual do binding, não nova API/protocolo aprovado nesta decisão. Este registro não afirma que essa função produtora já está implementada ou qualificada. Nenhuma implementação pode substituir a prova por enum, boolean, string, digest, linha PDG decodificada ou callback arbitrário. Retornos síncronos da ligação não apagam a história do produtor nem fazem retry de efeitos anteriores.

## Release INVALID e reconhecimento

A API só recebe borrow previamente fornecido pelo produtor, com ponteiro/lifetime conhecido e preservado pelo owner. Não é decoder ou descoberta de endereços arbitrários. INVALID isolado não autentica/reconhece um ponteiro desconhecido. Nunca chamar com ponteiro/lifetime desconhecido ou já invalidado; ausência de lease não autoriza liberar o grafo original.

Para borrow previamente válido/conhecido, RELEASE_INVALID é exclusivamente precondição sem tentativa de liberação: nenhum efeito parcial, sem mudança de ownership, dependências/créditos ou estado de tentativa anterior. O borrow continua protegido como memória; isso não restabelece aplicabilidade de trust/M1 se houve Stop, revogação ou invalidação de contexto. Só o owner factual pode comprovar ausência de tentativa e permitir corrigir a precondição antes da primeira tentativa efetiva. Não deduzir essa ausência de timeout, erro ou output vazio.

| Chamada/resultado | Tentativa e reconhecimento | Estado consultável e ação permitida |
|---|---|---|
| release INVALID, borrow conhecido | nenhuma tentativa nova; precondição inválida | preservar o mesmo owner/borrow/grafo; corrigir apenas precondição comprovadamente sem tentativa; não limpar nem reusar |
| release RELEASE_RETAINED | tentativa efetiva iniciada, conclusão segura não provada | mesmo handle e dependências retidos, somente release_query; nenhum novo release/revalidate/M1 |
| release/query RELEASED | fechamento factual confirmado pelo owner, não resultado guessed | invalidar alias uma vez; não consultar nem repetir; créditos conforme fechamento factual |
| query INVALID | consulta com precondição inválida, sem tentativa/efeito novo | conservar a disposição anterior; se estava retido, permanece query-only; nunca reabre release |
| query RELEASE_RETAINED | consulta factual sem reemitir close | conservar referências/créditos, aguardar resolução factual sem timeout destrutivo |

Depois de tentativa efetivamente iniciada, seu resultado válido é RELEASED ou RELEASE_RETAINED, nunca INVALID para esconder incerteza. Uma query inválida não altera esse resultado anterior. Não transplantar automaticamente dispositions PST para esta API: Model E é preservado pelos fatos de tentativa/ownership/fechamento/quiescência, não por igualdade numérica de enums.

## Stop, versões e recursos

Stop impede avanço/efeitos e solicita encerramento, sem esperar/cancelar I/O/KDF no caminho de Stop. A ligação não cria trabalho assíncrono; qualquer trabalho prévio continua alcançável no owner e retido sob os contratos próprios. Quiescência factual de TODOS os filhos/borrows e resultado de fechamento são necessários antes de limpar segredos, liberar referências ou restituir crédito. Timeout/FINAL/exit não provam isso. Exactly-once, ausência de fallback e retenção continuam obrigatórios; lifecycle inválido nunca vira confiança positiva.

As declarações são C89, nomes privados e enums sem novo tamanho wire/serializado ou ABI binária entre toolchains. Este é o contrato reconciliado de revisão2; a revisão1 da proposta e o registro1 anterior de STOP são história, não contratos accepted concorrentes. Tipos opacos e identificação factual de operação reutilizam os domínios/lifetimes existentes; representação concreta e dimensões precisam revisão/censo na implementação. Nenhum novo layout de root, pacote, Browser provenance ou versão PapinhoPACC é autorizado. Sem builds/testes nesta tarefa; VC6/NT4/box/ISA não são qualificados por texto.

Referências previamente contabilizadas não significam reserva aprovada agora. Custos de evidência, backing, snapshots/pins, buffers, stack, coexistência e retidos permanecem sob ED11; desconhecidos não são zero. Cenário conservador conhecido3626009 é maior que3MiB, não pico operacional/reserva. Windows3145728 só por seleção explícita, default2097152/Browser e quotas individuais intactos. Sem aumento, desconto artificial ou economia presumida. ED11 totalPARTIAL/provider-kernel UNKNOWN; ativação dependente DENY.

## Resultado normativo e gates

FACTUAL_EVIDENCE_CONTRACT=ACCEPTED
ACQUIRE_RETAINED_LIFECYCLE=NO_PENDING_ACQUIRE_BORROW_ONLY_PRIOR_OWNER_RETAINS
RELEASE_INVALID_SEMANTICS=NO_ATTEMPT_NO_EFFECT_OWNER_AND_BORROW_PRESERVED
SHARED_M1_STATE_MACHINE=IMPLEMENTED_OFFLINE
IMPLEMENTATION_AUTHORIZED=NO
FACTUAL_SOURCE_BINDING=DENY
BROWSER_MIGRATION=NOT_AUTHORIZED
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO

Aceitação não é implementação/qualificação nem aceita patches funcionais por referência. Sem fontes funcionais, Browser/PST/wire, CA/chaves/certificados/pacotes/rede, bootstrap/CURRENT/assinatura, clone/worktree, commit/push/tag/release. Preservar WIP e quatro LAB. Próximo escopo: implementação própria da API sob revisão oficial e prova de fontes/owners/ED11, sem fonte sintética.

[Conformidade](../integration/factual-evidence-lifecycle-acceptance2-20261021/conformance-matrix.md) · [closeout](../integration/factual-evidence-lifecycle-acceptance2-20261021/closeout.md) · [preimagens](../integration/factual-evidence-lifecycle-acceptance2-20261021/incorporation-and-preimages.json).
