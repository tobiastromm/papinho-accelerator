---
status: accepted
decision-date: 2026-10-18
last-revised: 2026-10-18
revision: 1
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
supersedes: []
superseded-by: []
---

# Production client provisioning — obrigação de fechamento da Phase 6

## Autoridade e contexto

Registro normativo designado da direção expressa do owner na tarefa
`PACC-PHASE6-PRODUCTION-CLIENT-PROVISIONING-CONSOLIDATION-1`,
`OWNER_DIRECTION=PRODUCTION_CLIENT_PROVISIONING_REQUIRED_BEFORE_PHASE6_COMPLETION`.
A [solicitação integral](../integration/production-client-provisioning-consolidation-20261018/OWNER-REQUEST.txt)
identifica a autorização. Este documento registra somente as obrigações expressas;
não aceita formatos, protocolos, algoritmos, interfaces ou parâmetros inferidos.
Nenhum ADR accepted anterior é modificado ou superseded.

O [ADR-0006](ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md)
separa credencial de Principal, exige chave nos endpoints, servidor verificável
sem silent TOFU, código contextual/single-use/rate-limited e lifecycle de
renovação/revogação. O [ADR-0028](ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md)
separa transporte, admissão, AuthN e AuthZ. O
[ADR-0012](ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md)
proíbe ampliar silenciosamente o snapshot de uma Session.
Browser ADR-0007, seção "Production Secure Provisioning: two official modes
(2026-10-07)", já exige dois modos permanentes e convergentes. Manual R1
CLOSED_CURRENT_HOST não implementa pacote, enrollment ou revogação remota.

## Decisão explicitamente aprovada

1. MANUAL e AUTOMATIC são obrigatórios antes do fechamento integral da Phase6.
   Manual permanece após enrollment automático; preservar importação granular
   avançada do Browser.
2. MANUAL: administrador autorizado gera pacote para novo cliente; Browser o
   valida/importa pelos mecanismos existentes. Exigir versão, identidade do
   servidor/destinatário, integridade verificável, limites e importação
   transacional; proteger segredos quando aplicável. Gerar pacote não autoriza
   gerar/exportar no servidor a chave privada do cliente. Custódia e CSR devem
   obedecer ao contrato aprovado. Formato permanece aberto.
3. AUTOMATIC: usuário informa endereço; Browser apresenta identidade verificável
   do servidor e realiza confirmação por código. Preservar vínculo ao
   servidor/contexto/chave ou request, expiração, single-use e anti-replay.
   Chave privada fica no cliente no fluxo normal; CSR/prova de posse conforme
   contrato. Não selecionar representação do código ou bootstrap por inferência.
4. OPEN/CLOSED no acesso ao enrollment: OPEN não exige username/password inicial
   para solicitar enrollment; CLOSED exige sua validação antes de aprovação
   automática ou manual. Password nunca trafega plaintext nem exige armazenamento
   plaintext. Ambos preservam verificação do servidor e confirmação de pareamento.
   Esta especialização não redefine Admission Mode geral do ADR-0028 nem elimina
   CLOSED por certificado nas demais operações.
5. Permitir AUTO_APPROVAL e MANUAL_APPROVAL administrativos. Ambos mantêm
   identidade, confirmação, autorização, expiração e anti-replay; decisão manual
   fica vinculada à solicitação exata. Pending/unapproved não obtém CONTROL/DATA/
   egress operacional. Aprovação também não concede capabilities/grants implícitos.
6. Adicionar, aprovar, renovar e revogar clientes sem reiniciar o serviço.
   Reutilizar a root administrativa e PREPARE -> COMMIT -> CURRENT -> EPOCH
   factuais. Não escrever diretamente na configuração committed, inventar epoch
   ou criar segundo listener/autoridade. Primeiro commit dormente existente não
   satisfaz sozinho o requisito de hot update.
7. Revogação persiste no servidor e nega novas conexões revogadas; definir política
   explícita para sessões ativas. Preservar renovação, trust rotation, safe
   retirement, leases, auditoria sem segredos e recuperação após restart.
   Unpair local não revoga no servidor. Não liberar dependências sobreviventes,
   fazer retry destrutivo ou fabricar DESTROYED.
8. Ambos convergem no mesmo cadastro administrativo, autoridade de emissão,
   Principal/policy, configuração de grants e contrato CredentialGeneration ->
   CredentialReference -> store -> PST/Secure Principal, com os mesmos leases e
   lifecycle. Não criar dois subsistemas, outro owner ou autoridade operacional.

## Gates e compatibilidade

As seis entregas no [roadmap Phase6](../phase6-network-tls-offload.md#production-client-provisioning--phase-6-gates)
permanecem OPEN até implementação e qualificação efetivas ou decisão normativa
explícita de alteração de escopo. Documento/fixture não fecha entrega. Phase6
integral não pode ser declarada encerrada com essas obrigações abertas. READY
histórico do escopo anterior/Mode B não comprova fechamento integral atual.

A decisão canônica de 18/10, Model E e ED08/ED11 continuam aplicáveis. Secure
Principal operacional mantém TLS1.3/mTLS/ALPN papacc/1. O mecanismo anterior à
primeira credencial exige decisão concreta própria: não inferir segundo listener,
perfil operacional server-auth ou reutilização de plaintext. Preservar Browser
C89/VC6/NT4 e backend substituível; NTFS+DACL não vira requisito universal nem
criptografia em repouso.

CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.
Produção064/API2 preservada; candidato065/API3 normal opt-in separado de R2 TEST_ONLY.
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION.
Nenhum código, SDK, PACC wire, quota ou gate muda por este registro.

## Escolhas não aprovadas

Cerimônia/apresentação do código; bootstrap anterior à primeira credencial;
formato/autenticidade/limites do pacote; seleção operacional de issuer e
custódia/CSR/PoP; CLI/GUI administrativa; expiração/limites numéricos; propagação
da revogação nas sessões ativas. A [matriz factual](../integration/production-client-provisioning-consolidation-20261018/RECONCILIATION.md)
detalha somente essas lacunas. Não reabrir chave local, anti-replay, dois modos,
ortogonalidade, grants separados ou Model E já accepted.

## Rationale e consequências

Só registrar backlog deixaria obrigação normativa dependente do roadmap;
considerar R1 como produto completo confundiria importação com emissão/enrollment.
Reusar mecanismos e separar obrigação/estado/escolhas preserva decisões anteriores.
Gates ficam abertos e decisões concretas precedem o código que delas depende.

## Histórico

| Revisão | Data | Descrição |
|---|---|---|
| 1 | 2026-10-18 | Direção explícita do owner registrada; nenhum formato/parâmetro inferido. |
