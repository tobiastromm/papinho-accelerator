---
status: proposed
revision: 1
date: 2026-10-21
scope: canonical dormant v3 UPDATE approval source only
---
# U0-U7 — ato administrativo da candidata e observação de status

Proposta focal requerida pela seção3 de PACC-PHASE6-V3-CANDIDATE-APPROVAL-STATUS-PRODUCER-1. Não ACCEPTED. Complementa somente a aquisição/origem do ato administrativo exigido por S1-S5 e F1-F5/G0-G7; não reabre seus formatos ou requisitos. Nenhuma API/estrutura funcional é aceita por referência.

## U0 Escopo e precedência

O produtor pertence ao owner/cohort UPDATE existente, na mesma root, policy e issuer. Esta especialização proposta permite capturar um ato explícito do administrador local para a imagem candidata imutável sob predecessor CURRENT factual. Não transforma o Windows token em policy persistente, não concede grants ou autoridade ao SDK e não permite usar a prova de bootstrap para UPDATE. Inicialização, recuperação, operação remota e mudanças durante runtime ativo permanecem fora deste primeiro perfil. Sem aceitação específica destas cláusulas, manter produtor dependente e resolução negados.

## U1 Origem administrativa Windows

No primeiro adaptador Windows, o ator SHALL ser o próprio solicitante local do processo administrativo, por token primário efetivo, sem impersonation. Token de outro processo, processo de serviço agindo em nome de terceiro, grupo apenas histórico/deny-only, token filtrado ou contexto de autenticação não demonstrado SHALL negar. Exigir token elevado, Administrators habilitado e identidade SID válida, capturados pelas consultas nativas existentes/reutilizadas; não habilitar privilégios, alterar ACL, elevar ou usar credenciais de terceiros. Ausência/erro/resultado incerto de consulta nega. Outros targets/adaptadores não herdam esta escolha.

Essa condição identifica e autoriza somente o administrador local para executar o ATO explícito U2 no escopo proposto; não comprova aprovação prévia de uma tupla, frescor global, fencing, autorização de assinatura ou confirmação Browser. A escolha de permitir esse ato pelo administrador local é a especialização que requer aprovação normativa, e não uma equivalência já estabelecida pelo bootstrap ou C8.

## U2 Ato explícito e vínculo integral

A entrada administrativa SHALL distinguir captura da disposição de mera validação/importação de bytes. Nenhum startup, leitura de PDG APPROVED, flag em papacc.conf, hash esperado ou sucesso de parser executa o ato. O operador SHALL receber e selecionar explicitamente a candidata completa e o predecessor base, com disposição solicitada para cada associação alterada; ausência ou ambiguidade nega. Uma opção textual é somente solicitação: o produtor nativo deve comprovar U1 e o vínculo factual, sem aceitar um boolean de autorização do chamador.

O owner SHALL fixar antes do ato: sua invocação/operação; ticket/serial do predecessor real; root e revisão-base; identidade/revisão candidata; slot/serial candidato; imagem imutável completa e seu digest de transferência; a lista completa e limitada de tuplas SNT/PDG aprovadas ou revogadas. Cada tupla inclui listener/security_ref/perfil/nome, policy identity, issuer authority/generation/CA SPKI, request/signer/certificado/SPKI exatos, finalidade, intervalo e disposição. Autorizar o conjunto exato completo não autoriza outra imagem, pacote ou nova operação. Não registrar aprovação em bytes reservados ou em segunda tabela persistente.

O ato autoriza uma disposição candidata para eventual transação, NÃO afirma que ela já está publicada. disposition_approval_root_R e revisões de fonte candidatas são propostas sob o contrato existente; não são R/A/P/I CURRENT. Uma aprovação histórica pode ser herdada somente se houver fonte factual protegida comprovando a mesma tupla e histórico sem invalidação; PDG herdado isolado não basta. Revogar/reaprovar continua ato explícito conforme G3, sem apagar histórico.

## U3 Identidade e revalidação

O produtor SHALL manter no mesmo owner a identidade observada, contexto nativo, invocação e correlação U2. No adaptador Windows comparar novamente SID, TokenId, AuthenticationId e ModifiedId, além de elevação, grupo habilitado e ausência de impersonation, antes de tornar o ato utilizável e imediatamente antes de qualquer futuro efeito autorizado. Não converter esses campos em identidade global ou novo epoch. Divergência, Stop, mudança de CURRENT/base/slot/imagem/tupla ou falha de consulta invalida aplicabilidade; nunca fallback para prova antiga.

O resultado é consultável pela operação já existente do cohort; não se cria ID global, worker, lease, registry ou journal de aprovação. Declarações privadas C89 e representação concreta terão revisão/hashes/censo próprios. Não armazenar a prova em pending.previous, committed, t.model/source publicados ou outro backing ainda necessário ao predecessor/rollback.

## U4 Status observado, separado de aprovação

O mesmo produtor SHALL consultar os registros e histórico issuer/policy aplicáveis sob CURRENT/pin e suas fontes protegidas. Identidades de root, revisão, issuer/generation, certificado e SPKI devem coincidir com U2. O issuer fornece seu estado real; apenas estado PUBLISHED do certificado exato, não revogado, com perfil técnico adequado e intervalo válido, pode sustentar observação positiva de certificado. PREPARED, SERIAL_RESERVED, SIGN_REQUESTED, ISSUED_UNPUBLISHED, ABANDONED, RECONCILIATION_REQUIRED, ausência ou erro não equivalem a certificado válido publicado. PUBLISHED não concede delegação.

Avaliar separadamente a disposição da delegação: ato candidato válido U1-U3 para mudança explícita; ou aprovação anterior factual e idêntica para herança. REVOKED, intervalo inaplicável, status desconhecido/ambíguo ou falta de histórico nega consumo positivo. Uma candidata de revogação pode continuar a preparação de sua disposição, mas não gerar evidência positiva de PACKAGE_SIGNING; o resultado da transação de revogação continua gate separado. Nova delegação não pode conceder trust antes de CURRENT posterior factual.

UTC SHALL vir do backend issuer já aprovado e qualificado, com erro/indisponibilidade negados, não de campo caller nem de horário embutido no pacote. A existência de backend.utc ou leitura do relógio local não qualifica automaticamente sua confiabilidade. Verificar intervalos do CA/certificado/delegação e identidade/perfil por rotinas existentes qualificadas, sem tolerância/TTL/rede/fallback novos. Falta de fonte UTC qualificada permanece um gate factual explícito.

## U5 Ownership, Stop e retenção

Aprovação e observação são dados transitórios do cohort existente, não formato persistente novo. Owner mantém base, pin, candidato imutável, fontes/certificados e contexto de ato durante PREPARE/VALIDATE e qualquer trabalho pendente/retido. Antes da exposição, todos os externos requerem lease próprio ou cópia contabilizada; pin de root não protege ponteiro externo. Referências do candidato derivam de seu ownership G7, sem fabricar pin ou usar o pin predecessor para acessar bytes candidatos.

Stop é não bloqueante, impede novas observações/efeitos e preserva grafo e créditos. Falha de release/close, resultado incerto ou filho não quiescente mantém disposição alcançável no mesmo owner. Não reemitir close por timeout nem reutilizar proof/slot antes de fechamento e quiescência factuais. RELEASE_INVALID de E0 mantém sua semântica sem tentativa/efeito. Não existe acquire E0 assíncrono ou lease fabricado por este produtor.

## U6 Integração e transação

O produtor abastece somente a preparação/validação UPDATE existente. A marca RESOLVED exige todas as fontes administrativas, identidade, status, integridade, predecessor completo e lifetime comprovados, além do ato; nem ato isolado nem status isolado bastam. Revalidar imediatamente antes de futuro efeito. Usar mesmo worker/CAS; CONFLICT/UNKNOWN nunca convertem candidata em CURRENT, rollback presumido ou retry destrutivo. Nenhuma publicação, assinatura, E0 positivo, TRUST/M1 ou admissão decorre desta proposta.

Persistir disposições somente pelos campos e transações PACROOT8/v3/PDG1/CP3 accepted. ROOT06/BODY0.5/D3/LST1/SNT1/v2 e layouts históricos ficam byte-exatos. A prova de origem não precisa ser serializada para conceder validade operacional após restart; recuperação autoritativa permanece DENY. Não usar ausência da prova transitória após restart como razão para reconstruí-la de PDG.

## U7 Gate de implementação, ED11 e qualificação

Antes de hunks, fixar estruturas, owners e custo compilado da prova/observação/handles; mapear simultaneidade com token consultado, fonte UTC, issuer, DER/arenas, predecessor, candidato, slots, source borrow, encode/decode/hash, stack e todos retidos. Reutilização exige prova de lifetime. Não impor um tamanho arbitrário de SID/struct nativa nem reservar um buffer por estimativa nesta aceitação. Layout interno não serializado precisa revisão exata, não nova autoridade; qualquer novo formato persistente exigiria outra decisão.

Windows3145728 somente seleção explícita, Legacy2097152 e quotas individuais intactos. Censo fixo1948656/1790872 não é pico nem crédito. Cenário completo conservador3747601 excede3MiB em601873; ativação dependente permanece negada. Custos transitivos/token/backend/UTC/provider/kernel ainda não medidos não são zero; ED11 totalPARTIAL.

Implementação dependerá de aprovação exata de hunks e regressões Debug/Release sem warnings: ato presente/ausente, token inadequado/alterado, tuple/candidato/revisão divergentes, status desconhecido, revogação, issuer divergente, UTC ausente, Stop, abort, retenção/close incerto e ED11. Provas positivas sintéticas só testam correlação/máquina de estados; não qualificam autoridade nativa. Sem ambiente administrativo e CURRENT factual, registrar positivo pendente e preservar todos os gates.

IMPLEMENTATION_AUTHORIZED=NO_BY_THIS_PROPOSAL
NORMATIVE_STATUS=PROPOSED
CANDIDATE_APPROVAL_PRODUCER=DENY_UNTIL_EXACT_SCOPE_ACCEPTANCE_AND_IMPLEMENTATION
CURRENT_PUBLICATION=DENY
V3_COMMIT=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
