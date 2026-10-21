# Backings v3 no owner canônico — prova delimitada G7

TASK=PACC-PHASE6-V3-IMMUTABLE-BACKINGS-PIN-LIFETIME-1. Implementação privada Windows opt-in, DATA somente. Autoridades F1–F5/G0–G7, ROOT08/v2/SNT1/CP2, issuer, root/C8, E0–E8, Model E e ED08/ED11 permanecem accepted sem revisão. Não há conflito normativo ou nova decisão durável: a implementação faz cópia própria no MESMO owner conforme G7, sem criar autoridade de status.

## Inventário e ownership

| Backing | Origem/identidade/revisão | Owner, armazenamento e lifetime |
|---|---|---|
| SNT1 | componente do pacote v3 codec-validado, root/R e listener/security_ref | bytes dentro de packet30592 em cada um dos3 slots canônicos; nenhuma referência ao input do caller |
| PDG1 | mesmo pacote, root/R, signer/issuer/purpose/status; inclusive REVOKED como DATA | mesmo packet, sem catálogo ou cópia lateral de delegação |
| Issuer/CA | carrier completo no packet com I/root/authority/generation; SPKI DER da CA correlacionado por SHA256 nativo | carrier no packet; CA DER público por valor8192/slot; snapshots issuer históricos8192/slot continuam existentes e contabilizados |
| Policy/listeners/TLS | coleção/BODY/descriptor/SNT/PDG do mesmo packet; projeção do primeiro BODY comparada ao view do slot | coleção1..4 preservada integralmente, sem truncamento; target operacional singular e resolução permanecem gates separados |
| Predecessor/R/A/P/I | header/CP3/LST1 são DATA histórica | packet preserva bytes completos; não reconstrói CURRENT/epochs; base_link/CAS e aprovação de mudança não são substituídos |
| Snapshot nativo | slots existentes com view, serial, leitores, retained e issuer_snapshot_valid | PAPACC_CONFIGURATION_COMMIT já existente; nenhum owner ou pin novo |
| Pin | ticket/index/serial emitidos por papacc_configuration_pin | os mesmos32 tickets e3 slots; nenhum contador/tabela alternativo |
| CURRENT | exige root/projeção/publicação factual próprios | não fornecido por captura ou pin held; PENDING_EXTERNAL_ENVIRONMENT; nenhuma publicação executada |

PAPACC_CONFIG_V3_BACKING só existe no perfil Windows3MiB explicitamente selecionado. Cada slot contém packet30592+CA8192+metadata88=38872 bytes x64;3 slots116616. A extensão não é outro armazenamento persistente: não abre arquivo, journal ou registry. Não duplica o issuer como autoridade. As cópias físicas já existentes de issuer e as novas contidas no packet são cobradas, sem desconto por igualdade dos bytes.

## Captura e coerência DATA

papacc_configuration_v3_capture_data usa o owner/thread original e somente slot preparado livre, não atual, sem leitores/retained, Stop, incerteza ou pendência. A geração existente precisa ser não zero; uma captura selada para aquele mesmo serial não pode ser substituída. Inputs/arena disjuntos e capacidades verificadas antes dos efeitos. Hash/codec/DER síncronos são preparação anterior, nunca o caminho de Stop ou sp_trust_acquire. Callers serializam owner/Stop e respeitam os ports existentes não reentrantes.

O codec v3 completo existente é chamado sem mudar seus predicados; seus ports PC_ROOT8_COMPONENT continuam validação pública DATA, NÃO callbacks booleanos de confiança. O teste usa ports explicitamente MODEL; o produtor operacional futuro precisa dos adaptadores qualificados existentes. A API não transforma esse port em fonte factual ou em delegação. Não foi ligado kind8 ao dispatch canônico.

A captura verifica issuer root/I, todas as linhas SNT contra authority/generation/CA-SPKI, projeção do primeiro BODY contra o slot, identidade root/R/predecessor e, se presentes, a revisão/cópia issuer daquele slot. O codec mantém a coleção inteira e vínculos PDG/SNT/BODY. CA DER candidato é decodificado/reencodificado com CryptoAPI existente; SPKI DER canônico deve coincidir com o issuer. Isso comprova correlação DATA, NÃO perfil CA accepted, assinatura/validade corrente, identidade administrativa ou autorização de CA. Perfil/certificado/UTC/status factuais continuam obrigatórios antes de autoridade.

Após terminar os borrows temporários de decode, o mesmo caller arena é reutilizado para BODY decode; nada aponta para ele no output. O owner revalida slot/serial/estado e copia imagem e CA para armazenamento por valor; hashes completos e comprimentos ficam associados ao mesmo serial. Falha anterior não sela novo backing. Falha nativa de hash conserva os guards/handles/uncertainty do owner existente; não descarta recursos para obter sucesso.

## Pin e borrow

papacc_configuration_v3_borrow_data só entrega ponteiros const para armazenamento do owner sob ticket canônico já mantido, slot/serial/leitores correspondentes. Revalida integridade de packet e CA, root/R/predecessor/I e eventual snapshot issuer. Output permanece intacto na falha. Serial antigo, ticket inexistente, corrupção, ausência, revisão divergente, Stop, authority_lost ou unresolved negam nova obtenção. Não há pin/lease novo, transferência de ownership ou prova de CURRENT.

O caller mantém o MESMO pin até terminar TODOS os usos/filhos/callbacks dos ponteiros; não pode fazer unpin concorrente e continuar usando o borrow. O contrato C não autentica ponteiros arbitrários nem revoga aliases por timeout. Nenhum CERT_INFO raso escapa: o backing é DER público por valor. O futuro caller de kind8 deverá decodificar esse DER em arena própria, contabilizada e protegida durante toda a chamada, mantendo também o pin canônico. Não usar provider/CA do pacote como autoridade.

Preparações históricas só reutilizam slots sem leitores/retained e incrementam serial. Os novos borrows rejeitam backing de serial anterior mesmo quando os bytes antigos ainda existem. Nenhum setter de CURRENT/published/epoch foi acrescentado. Captura é memória DATA preparada, não commit; obter ticket histórico não comprova que packet v3 foi efetivamente commitado. Essa prova continua no worker/root factual, fora deste resultado.

## Stop, retenção e liberação

Stop existente continua somente sinalização/estado; não decodifica, espera, libera ou zera os backings. Referências já obtidas e pin mantido conservam memória após Stop, embora deixem de ser aplicáveis a novos efeitos. Não há novo I/O/worker ou cancelamento presumido. Hash/DER ocorrem apenas nas chamadas síncronas de preparação/obtenção, não no caminho de Stop; nenhum caller operacional foi habilitado.

unpin existente deve ocorrer uma vez após quiescência factual de todos os usos. unpin(retained) conserva o slot, sem reutilização ou finish. finish permanece negado com leitores, retained, pendência, prepared, commit incompleto ou unresolved. Só após quiescência/fechamento factuais o owner pode ser destruído. Retenção não é limpa por prazo, FINAL, erro ou ausência de lease. E0–E8/RELEASE_INVALID permanecem byte-exatos: nenhuma tentativa/efeito ou liberação por esta entrega.

G7 comprovado aqui é ownership/imutabilidade/lifetime DATA da cópia própria sob pins canônicos. Não é F2/E2–E5 positivos, CURRENT, fencing, fonte administrativa da CA, trust ou qualificação operacional do grafo completo.
