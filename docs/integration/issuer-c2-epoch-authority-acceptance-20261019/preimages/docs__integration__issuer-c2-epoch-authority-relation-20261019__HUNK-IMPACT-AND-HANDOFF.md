# Impacto nos18 hunks — PROPOSED / nenhuma aplicação

Referência imutável: ../issuer-session-exact-review-20261019/hunk-matrix.md e JSON. Esta matriz complementa o contrato proposto, sem editar os hunks, suas preimagens ou a negativa histórica. Nenhum conjunto está liberado para aplicação por este documento.

| Hunk | Conteúdo anterior | Impacto da relação proposta | Obrigação antes da submissão futura |
|---|---|---|---|
| H01 | CURRENT AND held em preflight | preservar CURRENT verdadeiro para nova admissão; lifetime do listener antigo deve ter predicado distinto, sem marcar R histórica como global CURRENT | separar check de composição/lease operacional de check de nova seleção; provar P vigente e projeção operacional atual, não igualdade permissiva de gerações |
| H02 | comparação de intervalos para alias | princípio permanece, backend-local, sem autoridade | vetores de overflow, adjacência, overlap nas duas direções e tipos do target |
| H03 | proteção de alias na cópia issuer | permanece proteção de dados e ownership | não permitir que saída sobrescreva owner/pin/slot; nenhum bit de autorização emitido |
| H04 | pin_read view + epoch | exige tipagem explícita do epoch retornado: A administrativo não é P C2 | retornar vínculo de revisão/dados sem transformar identidade administrativa em policy; rever assinatura privada e caller com preimagens atuais |
| H05 | bloqueio de publish06 normal | manter DENY enquanto faltar transação normativa root completa | aceitação do contrato não autoriza retirar guard; não tomar publicação de componente por CURRENT da aplicação |
| H06 | fechamento do guard de compilação | mesma dependência de H05 | perfis de build separados; teste/normal não intercambiáveis |
| H07 | ponteiro para pin de Session | manter uso do storage do mesmo owner | descriptor lógico deve identificar R/I/A admitidos e P capturado separadamente, com incarnation/proveniência; sem segundo registry |
| H08 | membership por endereço factual da Session | manter restrição à Session pertencente ao owner | verificar incarnation e ticket/serial contra slot reutilizado; índice sozinho não é identidade |
| H09 | checks de held + comparações originais | H09 original não é solução live completa; comparar R da Session com R do listener confunde lifetime com CURRENT | conferir Session contra sua própria referência admitida; preservar CURRENT/P vigente para novos efeitos e policy atual para continuidade; não remover genericamente o guard de epoch |
| H10 | variável admitted em adopt | manter candidato local, sem autoridade autônoma | não publicar Session nem reter stackview; contextos root-owned e tipados |
| H11 | pin antes da exposição e igualdade ao listener | aquisição antecipada permanece; igualdade absoluta R-admitida==R-listener não é válida como regra geral após ISSUER_ONLY | novo pin deve ser da R global CURRENT real; listener/leases podem referir geração antiga protegida somente com vínculo comprovado à mesma projeção operacional/P vigente; não aceitar listener apenas porque está pinned |
| H12 | s.policy_epoch=admitted.epoch | NÃO copiar campo administrativo A para campo wire/Session P | capturar P operacional vigente proveniente da root na admissão; guardar A/R/I em referência administrativa distinta; nenhuma conversão por largura/igualdade numérica |
| H13 | domínio da root vindo do pin | manter domínio exato da mesma root | não confundir domínio root, autoridade/geração CA e Principal |
| H14 | revisão admitida vindo do pin | manter R factual protegida | registrar I selecionada e vínculo de A/P sem inventar IDs nem congelar layout por este texto |
| H15 | falha de attach/unpin conserva retenção | manter integralmente | falha de liberação conserva Session/ticket/crédito; não substituir por cleanup/reset ou retry destrutivo |
| H16 | declaração privada de unpin | contrato semântico permanece interno | tipos de referência/retorno revistos como unidade; nenhuma API/wire pública nova |
| H17 | helper unpin com full quiescence | manter identidade, membership e TODOS os guards; não exigir CURRENT para destruição-only | igualdade epoch retornado por pin_read==s.policy_epoch precisa ser revista porque A!=P; validar cada um contra seu campo/proveniência capturados, sem relaxar ticket/serial/revision/domain/incarnation; falha retém |
| H18 | unpin após barreiras antes do erase | manter posição exata | não limpar manager/Session antes de unpin factual; erro conserva registros; validação de cada referência individual e exactly-once |

## Dependências fora dos18 hunks

Os18 hunks não cobrem a transação live completa. Revisar server_configuration_root_transaction_win32.inc (classificação/CAS/root ack e A/P), server_admission_preflight_win32.inc (nova autoridade versus lease existente), server_listener_composition_win32.inc (composição operacional versus R global), server_c2_root_binding_win32.inc (admissão/Session binding), c2_session.c e c2_operation_win32.c (novo efeito versus operação capturada), C8 policy adapter (P admitido/efetivo) e seletores Site TLS (perfil CURRENT e lease protegido). Não alterar o wire ou limpar referências para fazer comparações passarem.

O campo WIP context_epoch e PAPACC_ROOT_CURRENT_PIN.epoch não são aprovados como se já representassem todas as identidades distintas. O WIP parcial armazenou P inicial no contexto do slot. Nenhum valor antigo vira A ou novo descriptor tipado por renomeação, cast ou migração automática. Fonte/codec e evidência de origem devem resolver significado/ausência antes de consumo. Esta proposta não fornece patch de conversão nem aceita o WIP como implementado.

## ED11

Não há mudança física de armazenamento nesta tarefa. A implementação deverá precificar vínculos A/P/R/I, descriptor de Session, três gerações,32 tickets compartilhados e os pins já existentes de root/listener/attachment/Session. Reusar a mesma raiz/slots; não criar tabela de autorização paralela. Somar apenas delta não embutido; cópias temporárias/stack e coexistência são custos distintos. Sem quota maior, sem refund de crédito retido. Valores compilados de composições anteriores não provam esta futura representação; provider/kernel UNKNOWN e total PARTIAL.
