# Phase 6 — handoff de custódia e decisão focal do ack (2026-10-29)

## Resultado funcional

Foi implementada a passagem de uma borrow CA **já existente e legitimamente adquirida**, da cerimônia para o backend CNG existente, sem intervalo em que a key fique sem holder. O backend distingue custódia de lifetime e lease de assinatura. A transferência não produz proof flags NORMAL, certificado, CURRENT ou autoridade de emissão.

O produtor original ainda não foi materializado nesta tarefa: a prova positiva da origem continua condicionada à existência de uma cerimônia/root/key/cert legítimas. Os testes positivos de handoff são modelos explícitos; o teste nativo continua negativo. NORMAL completo e fanout nativo positivo permanecem NOT_DEMONSTRATED/DENY.

A proposta focal foi consolidada numa decisão normativa exata, sem nova família de ADRs. Ela continua **PROPOSED**, com pedido de aprovação separado e SHA256 literal. Não se alterou PACIS601, BODY/ROOT/CURRENT ou ROOT07.

## Código integrado

1. `papacc_ca_key_transfer_borrow` verifica thread, estado READY, handles já detidos, ausência de close e o holder original exato. Troca diretamente borrower de origem para destino; não faz return/borrow intermediário nem native key call. Holder divergente, mesmo holder, outro thread, estado retido ou ausência de borrow negam sem transferir.
2. `papacc_configuration_personal_ca_handoff` usa destinatário tipado `PAPACC_ISSUER_CNG_BACKEND`, verifica sua key/owner/estado e revalida a origem por `personal_ca_current`: attempt, autoridade/geração, objeto/source/custódia e validação da key existentes. Só depois transfere a referência e publica a custódia no backend. O source perde o direito de devolver a borrow e permanece one-shot. Um core RECOVERY já inicializado ou outro binding de core/backend é rejeitado.
3. CNG acquire continua validando identidade, root/view, key/ACL e SPKI antes da assinatura. Com custódia recebida, usa a referência que já pertence ao próprio backend, sem tentar outra borrow exclusiva. A custódia transferida não pode servir a assinatura RECOVERY.
4. O fim da lease de assinatura limpa apenas a lease/active; não devolve a custódia principal. Perda do holder ou incerteza retém o backend. Stop não devolve automaticamente a referência.
5. `papacc_issuer_cng_backend_release_custody` só devolve a referência após backend Stop e core factualmente terminado, ou cancelamento de preparação com core nunca inicializado e idle. Busy, lease, output/pending/retained ou erro de devolução impedem sucesso. Não fecha a key/provider ou libera créditos. A CA original continua pertencendo ao seu owner físico.

Não há novo owner, serviço, queue, callback de autorização positivo ou setter de proofs. O destino permanece o backend existente, que deve ter lifetime suficiente; sua borrow impede o fechamento da key pelo source enquanto detida. A ausência de fonte original nunca é preenchida a partir desses estados de lifetime.

## Fontes originais e primeira lacuna factual

O mecanismo de entrega deixou de ser a lacuna técnica anteriormente identificada: ele já conecta o source da cerimônia ao receptor CNG. **A primeira aquisição real continua faltando**: uma CA/key/cert/generation originais, sob custódia e root verificáveis, para chamar essa rota com sucesso factual. O native negativo não obteve esses fatos. N0 não foi reutilizada como C8.

História, high-water e CAS locais existentes permanecem válidos nos seus escopos; não demonstram sozinhos original-history/prefix/exclusividade de emissão. O handoff não transforma estado de teste em fonte NORMAL. Não se abriu/gerou uma key, criou root, alterou ACL, pediu segredo ou executou cerimônia para obter o fato ausente. A aprovação normativa do ack também não suprirá essa aquisição.

## Revisão do ack A/P e alternativas

A é contexto de publicação administrativa após commit; P é policy operacional. A/P privados já estão separados e sua qualificação anterior é preservada. A questão restante é a história persistida de um ato específico de publicação de credencial.

Reutilizáveis: identidades root/CA/geração/policy, request/certificado/serial, high-water, R/I/predecessor, digests de conteúdo e os campos A/P do checkpoint existente. Complementos mínimos: intenção de PUBLICAR discriminada/versionada e vinculada ao ato/P observado; identidade inequívoca do evento; confirmação que referencia essa intenção e seu A original; confirmação CURRENT da história que contém o ack. Valores originais e atuais são distintos e não são derivados um do outro. Não há necessidade de outra key/cert, tabela de Principal, root ou ledger.

Alternativas examinadas:

- **v1 intacto:** preserva DATA/DENY. Não representa a intenção e o ack específico pós-commit com as regras atuais: PUBLISHED exige epoch antes do append, e o A relevante ainda não existe.
- **A de uma publicação anterior, com receipts separados:** o A do conteúdo anterior não confirma por si o ato posterior de publicar a credencial. Falta associar/preservar o ato e os dois contextos. Reinterpretar published_epoch histórico para fazê-lo significar outro ato não é uma reutilização inalterada de v1.
- **ROOT07 atual:** possui vínculos de publicação de conteúdo, mas não identifica sozinho o ato de credencial nem retém todo o histórico de acks por credencial; não pode conceder PUBLISHED automaticamente.
- **Ack só em memória:** pode permanecer um fato do owner corrente, mas não satisfaz a persistência/continuidade pedida; crash/reinício sem prova mantém DENY.
- **Segundo ledger:** desnecessário e fora do contrato da mesma root.

Portanto, **para a representação persistida solicitada e sem reinterpretar formatos históricos**, é necessária uma revisão focal versionada. Não se demonstrou que todo projeto de encoding possível é impossível em v1; demonstrou-se que as alternativas examinadas não fecham o requisito mantendo sua semântica atual.

## Pedido normativo exato

[PUBLISHED-ACK-FOCAL-DECISION.PROPOSED.md](PUBLISHED-ACK-FOCAL-DECISION.PROPOSED.md), revisão 2:

`3fcce389e58299aaa161779b9c7976f00f8e79d76dc5622d40d60b091e34f684`

[OWNER-DECISION-REQUEST.json](OWNER-DECISION-REQUEST.json) vincula a decisão a esses bytes. Status: PENDING_OWNER_DECISION; nenhum registro de aprovação foi criado. A necessidade vem de issuer/C2 P1.2: “Layout/encoding privado novo, se necessário, exige revisão versionada própria, preservando BODYs históricos.” Não se pede novamente M3/M7/CAS.

A decisão proposta cobre a semântica limitada de intenção+ack na mesma história/root/worker, incluindo vínculo original/atual, replay, substituição, retenção, compatibilidade, rejeição de versões e término finito da confirmação. A implementação deverá documentar/revisar o encoding versionado exato e seus bounds antes de integração, sem aumentar quotas ou editar formatos antigos. Desvio semântico exige decisão própria; operações reais permanecem independentes.

## Fanout M7

O source de status, port concreto e cohort exato do pacote anterior permanecem integrados. A passagem de custódia evita que finalizar uma lease de assinatura devolva prematuramente a referência da CA enquanto o backend/issuer ainda é responsável pelo lifetime. Não libera recursos de Sessions nem altera identidade/grants de credenciais não relacionadas.

As regressões de re-read A/R/P e cohort M7 passaram novamente. Não houve nova credencial PUBLISHED/REVOKED legítima para exercitar o wrapper nativo positivo; esse fato não foi substituído por callbacks de fixture. Receipts continuam sendo de invalidação/Stop/wake, não quiescência/close. UNKNOWN continua retido.

## Builds, testes e limites da prova

- Selected/Legacy Debug/Release: **quatro builds PASS, zero warnings finais**.
- **52 execuções CTests PASS**, treze por configuração: dez regressões anteriores e CA lifetime/source native negativo/source model.
- A primitiva de lifetime executa 15 verificações por configuração, com handles sintéticos que não são dereferenciados. Não há geração/import/open/free de key nativa nessas verificações.
- O modelo da cerimônia testa destinatário/key/attempt divergentes, origem indisponível, troca de objeto, falha de validação, busy, RECOVERY, core/backend divergente, handoff contínuo e rejeição de replay/devolução pelo source. A variante nativa continua **NATIVE_NEGATIVE**, sem materializar fontes.
- O modelo CNG passou **614 verificações por configuração**: inclui lease com custódia mantida, assinatura sintética única, release somente após término, cancelamento antes de init, perda de holder, falha de devolução e rejeição de RECOVERY. Public DER e assinaturas sintéticas não são operações privadas reais nem posse de uma CA.
- O core A/P permanece com 279 verificações do modelo por configuração. Não se repetiram os 32/68 fixtures nativos anteriores do CAS sem alteração de suas funções.

O backend é 728 bytes neste build; a reserva usa sizeof dos objetos e continua carregando o custo real. O censo managed/root não mudou: Selected 1971896/3145728 e Legacy 1803328/2097152. Nenhum limite foi ampliado; provider/kernel e pico não amostrado permanecem UNKNOWN. Não há concessão de headroom/capacidade por estes testes.

## Estado e próximo passo

HEAD/WIP, históricos, pins e formatos accepted preservados. Nenhuma operação CURRENT real, assinatura produtiva, bootstrap, serviço/Start/Stop, SCM/ACL, rede operacional, TPM/NV/Q4, cleanup/refund ou recuperação foi executada. PST 0.6.4 preservado.

O código de handoff e seu consumo estão prontos/qualificados nos escopos acima. O próximo gate normativo é decidir a proposta exata. A aquisição original é uma dependência factual separada; não se afirma que a aprovação deixará todas as provas disponíveis. Issuer NORMAL: DENY; fanout nativo positivo: NOT_DEMONSTRATED; ED11/Phase 6: PARTIAL. [Próximo prompt](NEXT-ACK-DECISION-AND-CUSTODY-INTEGRATION.md).
