---
status: proposed
revision: 1
proposal-date: 2026-10-19
scope-level: project
scope-target: PapinhoAccelerator
owner-direction: SEPARATE_ISSUER_REVISION_FROM_C2_OPERATIONAL_EPOCH_WITH_FACTUAL_ROOT_BINDING
---

# Relação entre revisão issuer, CURRENT da root e policy_epoch C2

## 0. Estado, autoridade e precedência

P0.1 Este é o texto normativo EXATO PROPOSTO para avaliação do owner. SHALL/DEVE abaixo descrevem a decisão candidata, sem autoridade até aceitação específica pelo processo governado. A direção do owner autoriza esta preparação documental; não equivale à aceitação deste texto. Nenhum ADR accepted, código, hunk histórico ou formato é modificado.

P0.2 Restrições preservadas: C2 revision1 §4 (policy_epoch, identidade e BIND), §6 (revogação, dados já aceitos, CANCEL/FINAL); ADR-0012 Decisão/regras derivadas; ADR-0048 Lifecycle; decisões accepted issuer/custódia cláusulas2,7,8,11 e perfil técnico §2.1–2.5/§3.5–3.7; decisão accepted de coexistência de2026-10-19; Model E/ADR-0033; ED08 e ED11/C2 §11.3. O texto esclarece a relação entre identidades internas; não concede exceção de epoch, admissão, revogação ou transporte. Conflito material encontrado na aceitação exige STOP, não alteração silenciosa de norma anterior.

## 1. Identidades e suas fontes

| Identidade | Significado e fonte factual | Não substitui |
|---|---|---|
| issuer revision (I) | revisão de conteúdo administrativo issuer, vinculada à autoridade/geração CA, serial high-water, reservas, certificados/status, renovação e histórico; mantida pela transação da mesma root | revisão global R, epoch C2, Principal ou grant |
| root CURRENT revision (R) | seleção global efetivamente committed/publicada da única root, com predecessor e referências exatas aos conteúdos participantes | mera existência de arquivo, PREPARED, I ou uma permissão operacional |
| contexto de publicação administrativa (A) | identidade/epoch interno de confirmação da publicação R, emitido e vinculado pela MESMA autoridade root após commit confirmado | policy_epoch C2 ou autoridade escolhida pelo issuer |
| policy_epoch C2 (P) | contexto operacional vigente emitido pela autoridade existente de policy/root, com proveniência da revisão operacional e das dependências aplicáveis | I, R, A, relógio, PID, ordinal ou contador de teste |
| Session admission snapshot (S) | fatos imutáveis da admissão real: incarnation/owner, Principal autenticado, upper bound de capabilities/grants, I/R/A de referência, P admitido e leases/pins factuais | CURRENT futuro, nova autenticação, policy presente ou grant ilimitado |
| Principal | identidade interna estável factualmente resolvida; separada de certificado, serial, fingerprint/chave e endereço | credential ou autorização automática |
| grants | permissões efetivamente negociadas/publicadas e limitadas ao escopo Session/operação; upper bound imutável | qualquer certificado emitido ou valor configurado ainda não publicado |

P1.1 I/R/A/P são tipos semânticos distintos mesmo se a representação privada usar números de mesma largura. Igualdade numérica ocasional não comprova relação. A root DEVE fornecer vínculo factual entre I selecionada, R/CURRENT, A e a projeção operacional vigente que identifica P; o consumidor não deriva um deles de outro nem compara relógios como prova.

P1.2 Nenhum identificador operacional novo é enviado no PACC wire por esta decisão. O campo policy_epoch wire continua sendo P não zero, atribuído pelo servidor conforme C2. Layout/encoding privado novo, se necessário, exige revisão versionada própria, preservando BODYs históricos. Esta proposta não congela novos bytes, IDs públicos ou símbolos de API.

P1.3 Revisões são atualizadas sob CAS/recovery da mesma root, sem wrap/reuso ou inferência revision-as-epoch. I pode permanecer igual quando outro participante muda; R não é sinônimo de I. Um lote issuer produz a revisão de conteúdo efetivamente committed, sem presumir correspondência1:1 entre registros, I e R. A e P jamais são inventados pelo chamador. Indisponibilidade de qualquer vínculo obrigatório implica ausência explícita/DENY.

## 2. Classificação factual da mudança

P2.1 Uma mudança é ISSUER_ONLY apenas se a transação demonstrar que sua projeção operacional permaneceu idêntica: policy efetiva e versão aplicável, habilitação de capabilities, semântica de Principal e mapeamentos autorizadores, grants/restrições, regras de revogação operacional, perfil/trust de segurança, leases e demais inputs que condicionam o contexto P. Comparação usa representação canônica governada/conteúdo e vínculos, não memcmp de padding de struct, proximidade temporal, nome de arquivo ou igualdade de hash isolada como prova de autoridade. A abrangência da projeção e sua proveniência DEVEM ser auditáveis.

P2.2 Inclusão de credencial ou renovação sob mapeamento Principal/policy já aprovado pode alterar o conjunto factual de credenciais disponíveis à autenticação CURRENT sem alterar permissões de Principals/Sessions existentes. Isso não cria Principal privilegiado, grant ou exceção de admissão. Se o pedido também alterar mapeamento autorizador, policy, grants ou trust operacional, não é ISSUER_ONLY; separar/classificar o efeito operacional na mesma transação, sem publicar configuração parcial.

P2.3 Incerteza de classificação, referência divergente ou dependência não validada não autoriza preservar P para contornar uma mudança operacional. O efeito dependente permanece negado até validação. O próprio evento de criação/renovação/emissão não substitui autorização administrativa.

## 3. Transação e regra de epochs

P3.1 Toda publicação usa PREPARE -> VALIDATE -> COMMIT factual -> CURRENT -> EPOCH/ack de publicação. PREPARE fixa ator autorizado, request/correlação, predecessor R, revisão I esperada, conteúdo integral e recursos/pins necessários; VALIDATE confirma participantes e classificação antes de efeitos irreversíveis; COMMIT durável usa CAS factual contra R/I/high-water e conteúdo, protegido entre instâncias. Cada fase mantém ownership e resultado incerto explícitos. Componente comprometido no disco não comprova a transação normativa completa.

P3.2 Depois do commit confirmado, a MESMA root seleciona e confirma CURRENT e emite/vincula A; não há autoridade de epoch paralela no issuer. Observadores não podem receber CURRENT positivo para uma combinação cujo contexto/ack obrigatório ainda esteja incompleto. A implementação pode publicar uma seleção imutável indivisível e depois confirmar o ack, mas nenhum epoch novo pode ser concedido antes do commit e nenhum estado parcial pode autorizar efeitos. Falha após commit impede declarar publicação completa; preservar história/serial, reportar UNCERTAIN/RECONCILIATION_REQUIRED conforme os estados existentes, sem rollback destrutivo ou fabricação de ack.

P3.3 Para ISSUER_ONLY validado, I e R avançam conforme o conteúdo committed e A identifica a nova publicação; P permanece o contexto operacional vigente anterior, com vínculo factual revalidado ao novo CURRENT. Preservar P não significa reutilizar epoch encerrado: o mesmo contexto operacional continuou vigente e suas leases/restrições continuam válidas. Não emitir artificialmente novo P por mera mudança de cadastro, reserva de serial, certificado ou status administrativo sem efeito operacional.

P3.4 Uma mudança efetiva da projeção operacional exige a transição P correspondente pela autoridade existente, após o commit normativo confirmado. Não copiar A/I/R para P; não aceitar P do cliente/issuer; não alterar P capturado em Session/operação para fazê-la parecer recém-admitida. O significado e a igualdade exata do campo C2 wire são preservados.

P3.5 Um ACK perdido não autoriza repetir assinatura, reservar outro serial, refazer publicação às cegas ou voltar a high-water anterior. Consultar/reconciliar a seleção e correlação exatas sob autoridade factual; ausência de prova mantém DENY. Reservas duráveis precedem qualquer assinatura, inclusive quando P não muda. PUBLISHED só descreve publicação factual do resultado, nunca só commit de metadados ou assinatura retornada.

## 4. Sessions, operações e enforcement

P4.1 Na admissão de cada nova Session, capturar S a partir de CURRENT real e dos cinco inputs de negociação/autorização aplicáveis, com identidade/Principal e I/R/A/P factuais. Adquirir pin/lease antes de expor a Session/registro; falha de aquisição não publica Session. Falha de liberação conserva referência/metadata e crédito. Não reconstruir S retrospectivamente a partir do CURRENT encontrado depois.

P4.2 Sessions anteriores retêm S e seu pin da revisão admitida. Uma publicação ISSUER_ONLY comprovada não substitui S, Principal, grants, leases ou P vigente. Referências antigas continuam válidas como memória e proveniência; não precisam ter R igual à última R para continuar existindo. Isso não as torna CURRENT para nova admissão. Um predicado de pin retido é prova de lifetime, não decisão de autorização.

P4.3 Nova Session e novo OPEN exigem a autoridade operacional CURRENT no instante de admissão: Principal/credencial/status atuais aplicáveis, snapshot membership, grants, policy/contexto, P vigente, recursos, deadline e gates de segurança. Para uma Session antiga cujo P permanece vigente e permissões ainda válidas, um novo OPEN pode ser admitido só após revalidação CURRENT completa; o pin antigo sozinho é insuficiente. Se P capturado ficou obsoleto, não aceitar novo OPEN por comparação permissiva de gerações. Exigir nova Session ou futura renegociação expressamente governada; nenhuma renegociação é criada aqui.

P4.4 BIND é uma nova reivindicação de role para operação existente: exige identidade/grant/epoch capturado exatos e verificação de policy CURRENT, validade, Session/Principal, token e slot conforme C2 §4. Não aceitar P arbitrário, atualizar identidade da operação ou usar R histórica para contornar revogação/expiração. Sem vínculo autorizado entre operação e contexto vigente, negar BIND. Um role já committed não é novamente admitido por esta proposta.

P4.5 Continuidade de operação já concedida preserva request/operação/generation/P capturado, channels/roles, fences/prefixos e grants. Cada efeito continua sujeito à policy CURRENT, estado local de revogação, expiração e crédito conforme C2. Snapshot histórico não garante progresso ilimitado nem cria novo trabalho após Stop. Se uma transição P real impede demonstrar a continuidade conforme o contrato existente, negar novo payload e seguir a terminação governada; não criar exceção silenciosa para epoch antigo. Bytes previamente aceitos pela camada inferior permanecem fatos rastreados, não nova autorização.

P4.6 Desabilitação/revogação operacional atual pode negar imediatamente efeitos nos termos ADR0012/C2. Aumento de grants não amplia snapshots existentes: requer nova Session ou futura renegociação explícita aceita. Não reescrever IDs, upper bounds ou epochs em registros existentes. Destruição/cleanup e narrow CANCEL/FINAL têm suas próprias precondições e não restauram DATA/OPEN/BIND.

P4.7 Reaper libera pin só após ausência factual da Session nativa e todas as referências/callbacks/borrowers/Connections/channels/processors/writes/operações/candidates quiescentes e disposições Model E confirmadas. Falha/retido conserva registro/lease/crédito. Stop, timeout, FINAL remoto e saída de processo não provam DESTROYED. Slot pinned/retained nunca é sobrescrito; falta de capacidade nega sem esperar indefinidamente nem expulsar dependências.

## 5. Matriz de operações administrativas

| Operação | I/R/A | P C2 | Sessions/operações existentes | Nova autenticação/Session/OPEN/BIND |
|---|---|---|---|---|
| Reserva serial/registro de resultado | publicação factual da revisão aplicável; serial consumido antes da assinatura | preservado se projeção operacional idêntica | nenhuma alteração de grants/identidade; histórico/ownership retido | não concede acesso; todos os gates CURRENT |
| Inclusão de cliente sob mapping aprovado | nova revisão de credencial e CURRENT; não grant automático | preservado se ISSUER_ONLY | S e referências permanecem imutáveis | autenticação real e CURRENT decidem; novo cliente pode receber DENY |
| Renovação explícita, mesma identidade/mapping | novo request/chave/serial vinculado ao predecessor; histórico preservado | preservado se ISSUER_ONLY | sem troca de Principal/credential na Session admitida; não revoga predecessor implicitamente | novo certificado somente após publicação e validação; grant não herdado por rótulo |
| Revogação de credencial/cliente | status persistente e CURRENT factual | não avançar P apenas para simular Stop; se combinado com AuthZ, aplicar a regra operacional | efeitos sobre Sessions ativas pertencem ao gate R-ACTIVE abaixo; não declarar encerramento nem continuidade como consequência implícita | negar credencial revogada em nova autenticação/admissão; novo OPEN/BIND em Session afetada exige status CURRENT válido, não pin histórico |
| Redução/revogação de policy/grants | publicar participantes consistentes e revisão operacional | transição operacional factual pós-commit | aplicação imediata do enforcement vigente; snapshot upper bound preservado, não reescrito | DENY quando contexto/epoch/grant não satisfaz CURRENT |
| Aumento de policy/grants | nova revisão operacional | transição operacional factual pós-commit | sem silent add; snapshots antigos não ganham capabilities | nova Session/renegociação futura aceita, com todos os gates |
| Alteração de identidade/mapping/trust | não classificar como simples renovação/cadastro | transição conforme projeção operacional; sem reinterpretação | preservar identidade capturada e validar efeitos explicitamente | nenhuma conversão retroativa ou fallback |

P5.1 Revalidar imediatamente status e autorização nas fronteiras de nova autenticação/admissão, OPEN e claim BIND aplicáveis. Status revoked/indeterminado ou provenance não comprovada não concede nova autoridade. A verificação de status de credencial permanece distinta da policy de capabilities e do trust TLS; não substituir uma dimensão por outra.

P5.2 Gate R-ACTIVE permanece separado: decidir o efeito específico da revogação administrativa de credencial sobre Sessions já autenticadas/operações em andamento, incluindo eventual Stop por Session ou Principal, escopo, momento, casos de cancelamento simultâneo e auditoria. A decisão accepted de issuer cláusula8/Delimited precedence deixou essa política pendente. Esta proposta NÃO escolhe Stop-on-certificate-revoke nem grandfathering de Sessions revogadas. Um recurso que dependa dessa disposição não pode ser ativado/qualificado sem a decisão e a prova próprias. É possível persistir status e reportar bloqueio de novas admissões sem alegar revogação ativa completa, conforme a cláusula8; não liberar recursos existentes.

P5.3 Revogação operacional explícita já definida pela policy CURRENT continua obrigatória e não espera R-ACTIVE. Nenhuma dúvida sobre cleanup permite conceder novos efeitos. UNKNOWN permanece DENY no gate dependente, sem fabricar uma ação terminal ou identidade de alvo.

## 6. Recuperação e consistência

P6.1 Uma seleção recuperável deve relacionar a mesma root/domain e identidade de armazenamento, R/predecessor/CURRENT, I e autoridade/geração, prefixo/serial high-water, reservas/correlações/estados/DER/hash/revogações, projeção de policy e proveniência do vínculo P/A. Um digest isolado não comprova última versão. O checkpoint protegido e fencing devem vir dos mecanismos/procedimentos aprovados existentes da root/backup, não de segundo registry/journal.

P6.2 Restore exige fencing de TODAS as instâncias emissoras anteriores e prova de high-water mais recente, inclusive reservas/assinaturas de resultado incerto e certificados externamente publicados, conforme perfil técnico §2.4 e3.5–3.6. Sem essas provas, recuperação autoritativa e assinatura/publicação dependentes permanecem DENY. Não usar timeout, relógio ou arquivo íntegro como fencing. Não clonar geração emissora ativa.

P6.3 Epochs runtime não ressuscitam Sessions após restart. Recuperar a proveniência histórica de P/A não concede um contexto vivo. A mesma autoridade pode emitir um contexto runtime factual de recuperação só após os gates normativos correspondentes; novas Sessions passam por autenticação/negociação normais. Não reabrir antigo snapshot de disco como Session ativa, nem reemitir certificado/CA automaticamente.

| Janela/falha | Fato que deve ser preservado | Resultado seguro requerido |
|---|---|---|
| Antes do commit | proposta não selecionada, recursos factualmente adquiridos | nenhuma autoridade nova; rollback somente do comprovadamente não consumido |
| Reserva committed, sem assinatura | serial high-water e correlação | serial consumido; sem reutilização; confirmação de CURRENT/publicação conforme transação |
| SIGN_REQUESTED ou assinatura incerta | marker durável, possível resultado/certificado e leases | RECONCILIATION_REQUIRED; sem re-sign/retry automático |
| Commit confirmado antes de CURRENT | novo conteúdo durável e predecessor | seleção/ack pendente; não declarar PUBLISHED nem fabricar P/A |
| CURRENT selecionado, epoch/ack incompleto | estado exato de seleção e resultado incerto | impedir efeitos dependentes, reconciliar; não voltar a serial antigo |
| Após CURRENT/EPOCH confirmado, resposta perdida | correlação e resultado já publicados | consulta idempotente do resultado existente; sem nova assinatura |
| Stale writer/conflito entre processos | expected R/I/high-water e identidade | CAS falha sem publicar; sem retry até PASS |
| Rollback/backup antigo/clone | identidade íntegra pode ainda ser stale | sem checkpoint/fencing mais recente: DENY; história antiga não vira autoridade |
| Revogação/renovação recuperada | status persistente, predecessor e Principal mapping aprovado | não apagar revogação, serial ou história; status incompleto nega efeitos dependentes |
| Geração antiga pinned/retained | snapshots, leases/callbacks e crédito | não sobrescrever/destruir; capacidade insuficiente nega |

P6.4 Falha em uma confirmação não prova ausência de efeito. A seleção administrativa anterior só pode continuar sendo usada onde sua autoridade atual permaneça positivamente comprovada; não presumir fallback CURRENT. Perda de prova do contexto operacional também nega trabalho dependente. Um estágio novo desconhecido não permite rollback manual de estado committed.

## 7. Portabilidade, ED11 e gates de implementação

P7.1 O contrato comum expressa identidades tipadas, revisão/commit/publicação, CAS, classificação, status, pins/leases e resultados; não NCrypt, CNG, SID, DACL, handles, Registro, serviço Windows ou filesystem específico. Adaptadores realizam os fatos de plataforma. Windows é um target; box e ISA mantêm representabilidade sem serem declarados qualificados. PapinhoTrustStore é fronteira substituível futura, não dependência Phase6.

P7.2 Contabilizar três slots, todas as gerações/snapshots, pins de root/listener/attachment/Session, buffers staging/commit, journal e serial/status/DER/história, stack e coexistência/retidos. Não somar armazenamento embutido duas vezes, elevar quotas, expulsar pin ou liberar crédito antes de destruição factual. Provider/kernel UNKNOWN; ED11 PARTIAL. Esta proposta não fixa novos thresholds, capacidade física, headroom ou freshness.

P7.3 Esta proposta não aceita os18 hunks anteriores nem remove a negativa do executor. Após aceitação específica, revisar os hunks como uma operação completa coerente com os tipos distintos e as quatro classes de efeitos, e submeter oficialmente antes de qualquer aplicação autorizada. Source/Tests/Build devem provar estado factual; texto normativo não é implementação.

P7.4 Nesta tarefa: IMPLEMENTATION_AUTHORIZED=NO; LIVE_PUBLICATION=DENY; AUTHORITATIVE_RECOVERY=DENY; CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY. Sem builds/testes/rede, CA/certificados, Browser/PST/wire, pin produtivo ou publicação Git. Produção0.6.4 preservada; PST0.6.5 candidato até encerramento integral da Phase6.
