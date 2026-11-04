# Decisão focal proposta — intenção e ack de credencial na mesma root

**PROPOSED. Nenhuma aprovação registrada. Revisão 2 da proposta focal de persistência A/P.**

## Escopo e fundamento

Complementar somente a representação de PUBLISHED da decisão issuer/C2 de 2026-10-19, P1.1/P1.2/P3.1–P3.5. Preservar M3/M7, issuer/custódia, CURRENT/CAS, Model E e ED08. Não criar owner, serviço, root, journal ou autoridade paralelos.

A é o contexto administrativo de uma publicação confirmada; P é o contexto operacional de policy. Commit, publicação e ack são fatos distintos. Os vínculos originários e atuais não são intercambiáveis mesmo quando os números coincidem. Nenhum A futuro pode preencher um registro antes de sua existência factual.

## Decisão solicitada

Se aprovada, admitir uma representação **explicitamente versionada**, na história canônica do issuer e no checkpoint do mesmo arquivo/root, de:

1. intenção autorizada de publicar uma credencial, sem admissão positiva;
2. confirmação original de publicação dessa intenção, capturada depois do commit/seleção/A;
3. confirmação CURRENT da história que contém o ack, com R/I/A/P atuais independentes dos contextos históricos da intenção.

O registro lógico PUBLISHED só poderá ser projetado após as três condições verificadas por fonte original. O estado de intenção ou a presença de um checksum nunca será apresentado como PUBLISHED. A alteração não aprova o conteúdo de qualquer credencial ou operação.

## Campos mínimos e reutilização

| Informação | Reutilização permitida | Complemento indispensável |
|---|---|---|
| Root/CA/geração/prefixo/policy, request/device/CSR/key/predecessor, certificado/digest, serial/counter/high-water | Campos já existentes no issuer image/registro e no conteúdo protegido da mesma root. | Não duplicar valores nem inferir sua autoridade pelo hash; preservar vínculo ao objeto original e à custódia. |
| Identidade da intenção | request existente + ordinal/revisão de evento existente + digest canônico do evento integral. | Discriminador versionado de intenção de PUBLICAR; correlação ao ato administrativo autorizado e P observado nesse ato. Não reutilizar automaticamente o P da preparação inicial se mudou. |
| Publicação original | R/I/predecessor e P da intenção, vinculados ao conteúdo originalmente committed. | A original, capturado somente após publicação factual, e referência inequívoca à intenção confirmada. |
| Ack retido no histórico | Mesmo histórico append-only e CAS; mesmo writer/pins/leases. | Evento versionado de confirmação que vincule a intenção e A/P originais. Ele não escolhe novo serial nem repete assinatura. |
| CURRENT que contém o ack | Tupla R/I/A/P, hashes de root/issuer/predecessor já tratados pelo checkpoint ROOT07 e identidade nativa do mesmo arquivo. | Dispatch de versão que distinga o ack de credencial do checkpoint genérico. O consumer valida origem por handles/custódia; File ID ou hash isolados não a substituem. |

Não é necessário armazenar um segundo certificado, uma segunda key, grants de Session, relógio como epoch, segredo ou nova tabela de Principal. O vínculo integral à intenção evita duplicar todos os seus campos no ack. A representação exata deve documentar quais bytes são referenciados e como o parser rejeita referências ambíguas.

## Sequência e término finito

- Preparar/validar e commitar a intenção sob o ato autorizado, mantendo admissão DENY.
- Selecionar a revisão da intenção e capturar seu A original factual. Preservar P somente quando ISSUER_ONLY efetivamente demonstrado.
- Persistir o evento de confirmação no **mesmo histórico/arquivo**, sob o CAS seguinte, retendo a referência ao A original e aos bytes da intenção. Publicar e confirmar a seleção que contém esse evento.
- Usar o mecanismo de checkpoint do mesmo worker para confirmar R/I/A/P dessa seleção e releitura original, sem tratar o checkpoint como outra mudança de policy/seleção que gere novo A por si. Isso evita uma recursão de ack do ack.
- A confirmação ao core distingue explicitamente A da publicação original e A do CURRENT que contém o ack. Nunca compara o primeiro com P ou presume que seja numericamente igual ao segundo.
- Somente então projetar PUBLISHED e permitir os gates já existentes de autenticação/admissão. A existência do ack não dispensa CA/key/custódia, história original, exclusividade, freshness, recursos ou policy.

Os participantes e recibos pertencem ao owner existente. Não se instala callback positivo que aceite um bool, digest ou tuple fornecida pelo chamador como prova de publicação.

## Replay, falha e retenção

Negar ack de outra intenção/request/credencial/geração/root/objeto, revisão/predecessor/P divergentes, conflito de writer, evento duplicado, versão desconhecida ou referências inconsistentes. Preservar todo o histórico necessário; não descartar ack/tombstone para ganhar capacidade. Limites atuais continuam vigentes; falta de espaço nega a operação.

PENDING não é sucesso ou cancelamento. Falha antes/depois de qualquer commit, Stop, close incerto ou perda de writer conserva serial, intenção, ack que eventualmente exista, pins/leases e primeira causa; retorna UNKNOWN/UNCERTAIN e não repete assinatura/CAS automaticamente. Não destruir Session, desfazer bytes aceitos ou conceder crédito por mera solicitação de Stop. M7 continua separado e baseado na credencial exata.

NORMAL não se torna RECOVERY. Estado restaurado/clonado/contraditório ou origem não demonstrada permanece negado nos contratos existentes. A extensão não promete proteção geral contra rollback administrativo e não altera as obrigações Q4/TPM históricas.

## Compatibilidade e autoridade da implementação

PACIS601/v1 e PACICP01/ROOT07 históricos permanecem imutáveis, com suas regras atuais. Não utilizar seus bytes reservados como extensão silenciosa. Versões novas devem ter dispatch explícito, tamanhos/bounds documentados e rejeição fechada pelos leitores que não as conhecem. Leitura histórica de DATA não produz A/P atuais, proofs NORMAL ou admissão.

A aceitação desta decisão, se ocorrer, cobre esta semântica limitada e sua implementação/qualificação técnica offline no mesmo mecanismo. O patch deverá registrar a revisão versionada exata de encoding, offsets, checksums, compatibilidade e limites conforme P1.2 antes de integrá-la; não pode alterar estes requisitos ou outros contratos accepted por escolha de layout. Não é necessário reaprovar M3/M7 nem criar uma família de ADRs. Desvio semântico ou aumento de autoridade/limites exige decisão específica, não inferência.

Nenhuma aceitação autoriza criar fonte original inexistente, key/CA, bootstrap, CURRENT produtivo, assinatura/emissão, rede, serviço/Start/Stop real, SCM/ACL, TPM/NV/Q4, limpeza/refund, publicação PST ou recuperação autoritativa. Essas autoridades permanecem independentes.

## Alternativas examinadas

| Alternativa | Conclusão |
|---|---|
| Reutilizar v1 sem novos bytes | Pode manter DATA e DENY; não expressa intenção + ack pós-commit específico sem mudar a semântica existente. |
| Preencher published_epoch com P/revisão/A futuro | Rejeitada: não prova publicação original. |
| Tratar ROOT07 atual como autorização de credencial | Rejeitada: checkpoint não distingue um ato específico de publicação e não retém sozinho todos os acks por credencial. |
| Usar A de uma publicação anterior e continuar comparando-o com o A atual | Rejeitada: falta vínculo explícito entre os dois atos e seus contextos; igualdade numérica não supre esse vínculo. |
| Guardar ack só em memória | Insuficiente para a representação persistida/continuidade exigida; falha ou reinício mantém DENY. |
| Segundo ledger/arquivo de autoridade | Não necessário e fora da decisão da mesma root. |
| Extensão versionada acima | Recomendada: reutiliza conteúdo/histórico/checkpoint/worker existentes, adicionando somente intenção e confirmação específicas. |

## Evidências de implementação exigidas

Antes de alegar caminho positivo: intenção sem commit, commit sem publicação, ack atrasado/ausente/substituído, R/I/A/P divergentes, concorrência, replay, Stop/crash antes/depois de cada fase, persistência/close incertos e preservação de formatos antigos. Demonstrar source original, assinatura única, ack histórico retido e negação antes do resultado final. Separar modelo, arquivo nativo isolado e operação real. A aprovação normativa não torna essas evidências já demonstradas.
