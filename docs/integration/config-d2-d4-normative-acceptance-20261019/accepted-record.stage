---
status: accepted
revision: 1
decision-date: 2026-10-19
scope-level: project
scope-target: PapinhoAccelerator canonical configuration bootstrap and update
supersedes: []
decision-makers: [Tobias Tromm]
---
# Aceitação delimitada D2–D2.3 e D4 — bootstrap/update canônicos

## Autoridade, incorporação exata e precedência

Registro especificamente autorizado pela decisão do owner de preparar e submeter a aceitação delimitada D2/D4, em continuação de PACC-PHASE6-D3-CANONICAL-ROOT-TRANSACTION-1. Vigência condicionada à aprovação oficial da operação exata pelo executor e verificação SHA-256 pós-gravação. Matriz das oito condições em docs/integration/config-d2-d4-normative-acceptance-20261019/conformance-matrix.md.

Fonte: docs/integration/config-bootstrap-base-root-envelope-20261019/BOUNDARY-REVISION2-PROPOSED.md, Revision2, SHA-256 c06de2b1646595e3398d9259cacd81a9f352a400a27ab8912762cea28bb8e8ff. Somente os dois segmentos D2–D2.3 e D4 abaixo são incorporados sem alteração de bytes como requisitos ACCEPTED. A proposta original permanece integralmente PROPOSED como proveniência. A expressão histórica “Esta proposta não aprova patch funcional” permanece válida: aceitação normativa não aplica nem aprova hunks funcionais por referência.

Esta aceitação direta e delimitada complementa a decisão accepted D3 Revision3; não altera sua exclusão de aceitação automática de outras cláusulas, nenhuma cláusula D3.0–D3.8, LST1 Revision2 ou qualquer outra decisão vigente. D1/D3/D5 do documento-fonte não são incorporados. O antigo D3 proposto não substitui a envelope accepted. Nenhuma proposta/handoff/teste/WIP é promovido por referência.

Precedência restrita à distinção de autoridade e preparação de bootstrap/update aqui incorporada, sob papacc.conf N1–N8, ADR0007/0009/0028, root/C8, issuer/C2 I/R/A/P, Model E e ED08/ED11 existentes. Prova factual de instalação nova, ator, participantes, referências e lifetime continua obrigatória. Não cria resolvedor, procedimento de backup/fencing, novo provider, exceção de segurança, registry, worker ou root. Ausência de CURRENT ou arquivo, CREATE_NEW ou digest isolado não prova ausência de história. Sem mecanismo/evidência suficiente, a implementação nega.

O layout D3, BODY0.5/ROOT06, checkpoints e LST1 históricos ficam intactos. Aceitação não autoriza migração, carrier issuer, recuperação autoritativa, bootstrap produtivo, CURRENT/epochs operacionais, listeners/admissão, CA/certificados/credenciais reais ou rede. Carrier issuer continua UNSUPPORTED no caminho D3 corrente; issuer existente não pode ser removido. Freshness/headroom não são inferidos; quota ED11 não aumenta e provider/kernel continuam UNKNOWN.

## Cláusulas incorporadas byte-exatas

D2. Separar explicitamente BOOTSTRAP_INITIAL de UPDATE_CURRENT. Ausência de CURRENT não autoriza nenhum dos modos sozinha. BOOTSTRAP_INITIAL exige operação administrativa explícita e autorizada, instalação/store alvo factualmente nunca inicializado, nenhuma root/CURRENT/história anterior, nenhum resultado pendente/incerto ou estado retido e candidato validado por todos os participantes aplicáveis. Reutilizar a prova de caminho/identidade e a criação exclusiva já governadas; verificação de ausência antes da abertura não substitui a criação exclusiva no commit. Store existente, vazio/truncado/corrompido/inacessível/parcial, falha de consulta, resultado de criação incerto, backup restaurado ou histórico não reconciliado não são instalação inicial: DENY. Não renomear/apagar/substituir store para tornar bootstrap elegível. Nenhum pin, R/A/P anterior ou LST1 base é fabricado no bootstrap. Presença de contexto-base na envelope é explicitamente ausente nesse modo; isso não relaxa o header obrigatório do LST1 aceito.

D2.1 No bootstrap, referências só podem vir dos mecanismos administrativos canônicos existentes, com autorização do ator/configuração e identidade/integridade/lifetime factuais: configuração Secure selecionada e validada pelo source/material loader existente, refs e leases exatas de trust/certificados, catálogo Site TLS administrativo já aprovado quando aplicável e policy/capabilities do modelo validado e limitado pelo target. Esses mecanismos não dispensam prova de autoridade por estarem presentes. Falta de prova do ator, loader, material, catálogo, referência ou lease mantém DENY; não usar fixture, callback sintético, hash esperado ou texto como autorização. Não criar provider, CA, Principal ou grants. ALL pode ser intenção de binding sob os validators existentes; SELECTED mantém DENY por aplicabilidade topológica; provider nomeado PENDING; CLOSED/CERTIFICATE_IDENTITY permanece sem suporte no primeiro codec. O contrato não declara implementado nenhum resolvedor de bootstrap.

D2.2 UPDATE_CURRENT exige CURRENT factual na root correspondente, revisão-base exata, pin/lease válido durante validação/uso, conteúdo e referências atuais e CAS contra a seleção esperada. Proteger coexistência de gerações/Sessions e revalidar autoridade/Stop antes do efeito. Snapshot obsoleto, pin apenas histórico, conflito/UNKNOWN, referência divergente ou insuficiência de slots/reserva implica DENY/retenção conforme contrato; nunca fallback para bootstrap ou CURRENT anterior. Não enfraquecer source_resolve existente: ele continua sendo leitor de CURRENT, não fonte de autoridade inicial. Preparação do candidato utiliza os mesmos participantes e owner com autoridade do modo explicitamente demonstrada, sem promover MODEL_BOUND por flag do chamador.

D2.3 Ambos os modos preservam PREPARE -> VALIDATE -> COMMIT durável comprovado -> CURRENT -> emissão/vínculo de A/P pela autoridade existente. R/predecessor de candidato não são CURRENT; A/P novos nunca são preenchidos antecipadamente para satisfazer encode. LST1 continua fragmento histórico offline. No bootstrap não há LST1 de base; após publicação factual ele poderá representar contexto comprovado, sem autorizar acesso. Atualização só preserva P se a projeção operacional for comprovadamente idêntica conforme decisão issuer/C2; nenhuma concessão implícita de grants. Falha pós-commit ou resultado incerto conserva dados, histórico e recursos, sem recriar bootstrap/reutilizar serial. Nenhum modo concede admissão ou executa publicação nesta tarefa.

D4. Preparação sem persistência distingue SOURCE/PARSED/VALIDATED/MODEL_BOUND/RESOLVED/PREPARED. Para avançar além de MODEL_BOUND, exige prova factual dos participantes e da autoridade do modo D2: bootstrap inicial comprovado ou base CURRENT válida, nunca CURRENT fictício no bootstrap. Sem contrato aprovado da envelope, o candidato pode ser inspecionado offline, mas não serializado/inserido como transação completa na root nem tratado como COMMITTED/CURRENT. Não criar caminho paralelo de owner, pins ou persistência. Esta proposta não aprova patch funcional.

## Limites desta operação normativa

FUNCTIONAL_PATCH_APPLIED=NO
CONFIG_CURRENT_PUBLICATION=DENY_UNTIL_QUALIFIED
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

Nenhum código, build, teste ou rede executado nesta operação. A autorização anterior de integração permanece em seu escopo e exige revisão oficial de cada patch funcional protegido; este registro não qualifica implementação, reserva ou produto.
