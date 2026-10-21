# Revisão delimitada D2–D2.3 e D4

Owner: decisão explícita após pergunta call_4f878d00997f461198ebd5483aa42758, na continuação de PACC-PHASE6-D3-CANONICAL-ROOT-TRANSACTION-1. Aprovação do owner condicionada às oito condições e ao executor; nenhum patch funcional incluído. HEAD/ref registrados em baseline.json. Fonte original preservada integralmente, incluindo status PROPOSED e D1/D3/D5 não incorporados.

| Condição do owner | Texto exato aplicável / autoridade preservada | Conformidade |
|---|---|---|
| 1 Bootstrap exige store novo, autorização e ausência de história | D2: operação explícita/autorizada; factualmente nunca inicializado; nenhuma root/CURRENT/história anterior; criação exclusiva no commit | CONFORME; ausência de arquivo isolada não basta |
| 2 Update exige CURRENT/base/pin/CAS | D2.2: CURRENT factual na root correspondente, revisão-base exata, pin/lease válido e CAS contra seleção esperada | CONFORME |
| 3 CURRENT ausente não prova store novo | D2: ausência de CURRENT não autoriza nenhum dos modos sozinha | CONFORME |
| 4 Corrupção/parcial/incerto nega bootstrap | D2: store existente/vazio/truncado/corrompido/inacessível/parcial, consulta falha, criação incerta, backup/história incerta DENY | CONFORME |
| 5 Mesma root/transação, sem autoridade paralela | D2.2 mesmos participantes/owner; D2.3 mesma sequência; D4 proíbe caminho paralelo de owner/pins/persistência; N5 e D3.6 mantidos | CONFORME |
| 6 CURRENT/epochs pós-commit | D2.3 COMMIT durável comprovado -> CURRENT -> A/P da autoridade existente; valores antecipados proibidos; D3.6/P3.1–P3.4 preservados | CONFORME |
| 7 Conflict/UNKNOWN sem fallback/retry destrutivo | D2.2 nega/retem sem bootstrap/CURRENT anterior; D2.3 conserva história/recursos; D3.6 e issuer/C2 P3.5/P6.4 já proíbem retry/rollback destrutivo | CONFORME por composição explícita das autoridades vigentes |
| 8 Bootstrap não concede admissão/grants/listeners automáticos | D2.1 sem Principal/grants/provider/fixture como autoridade; D2.3 sem admissão/publicação nesta tarefa; D4 distingue estágios; N3–N5 e ADR0009 preservam gates de listener | CONFORME; aceitar contrato não ativa listener |

## Compatibilidade exata

- D3 Revision3 continua integralmente accepted e byte-exato. Seu registro exclui promoção AUTOMÁTICA de D2/D4: esta é outra aceitação direta especificamente autorizada, sem editar aquela exclusão nem a norma D3. A condição de D4 sobre envelope aprovada é atendida quanto ao layout pela aceitação D3; não dispensa referências, participantes, CAS, lifecycle ou implementação factual.
- LST1 Revision2 C1–C5: fragmento histórico com R/A/P nãozero não é candidato/bootstrap; D2 não fabrica header/base. N5 e issuer/C2 mantêm I/R/A/P distintos, epochs pela única root após commit. Pin protege lifetime, não autoridade futura.
- BODY0.5/ROOT06/LST1/D3: nenhum byte, leitor, tamanho, hash, offset ou migração muda. Carrier issuer permanece UNSUPPORTED no caminho corrente; não apagar issuer.
- Model E/issuer-C2 P4.7 e ED08: referências/borrowers/callbacks/leases/pins retidos até quiescência/destruição factual; Stop/timeout não libera. D2.2/D2.3/D4 mantêm essas obrigações, não as substituem.
- ED11: não altera quota2097152, reserva ou censo2089095. Provider/kernel UNKNOWN; nenhuma aprovação de headroom/freshness/retention por texto. Censo da nova root transacional ainda necessário.
- Autoridade administrativa/proveniência de instalação nova não são comprovadas por CREATE_NEW, hash, ausência de CURRENT, callback de teste ou esta decisão. O mecanismo factual deve ser implementado/qualificado; sem prova, DENY.
- Sem conflito material identificado no escopo incorporado. Os dois modos são os descritos literalmente na proposta; rótulos de interface futura não criam nova autoridade ou wire.

## Operação exata

Criar exclusivamente docs/adr/DECISION-config-bootstrap-update-boundary-20261019.md, destino ABSENT, conforme normative-operation.json. Proposta integral copiada para preimagem; chunks D2–D2.3 e D4 incorporados sem alterar bytes. D1/D3/D5 e artefatos/hunks não são aceitos por referência. Registro entra em vigor somente com aprovação oficial e hash verificado. Não inclui escrita funcional, build/teste/rede, CURRENT/epoch ou provisionamento real.
