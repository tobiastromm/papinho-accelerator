# UPDATE individual — fronteira DATA/executor não demonstrada

2026-11-01. Revisão offline, sem alteração funcional ou execução operacional.
Decisão do proprietário: manter EXE individual, DLLs idênticas preservadas,
sem distribuição adicional nem troca de pastas. Ato anterior utilizado.

## Contratos e limite da interpretação

U1 classifica arquivos/pins candidatos como DATA e mantém o ato atribuído
F1–F6 no owner existente. U2 exige owner/DACL efetiva e originalidade/continuidade
de cada objeto; pins não substituem identidade nem atestam imagem mapeada.
U3 exige candidato novo protegido antes da substituição. U5/U6 mantêm consumo,
Stop, renames exclusivos, readback e retenção. Nenhuma dessas cláusulas permite
que classificar o payload como DATA atribua confiança ao executor.

O stager possui tratamento DATA por descritor válido, referências retidas e
hashes. Reutilizar essa regra no slot8 UPDATE seria seguro apenas se o papel
de executor fosse demonstrado independentemente. O pedido anterior selecionou
o MESMO arquivo como controller e payload. Logo, o tratamento DATA de outro
caminho não demonstra sozinho compatibilidade com U2 neste caller concreto.

Foi considerada alteração apenas do predicado do slot8 para descritor válido,
mantendo os demais objetos estritos. A revisão automática de aprovação REJEITOU
a ação antes de aplicar: permitir qualquer SD válido nesse slot enfraquece a
guarda do EXE controlador, sem prova de separação DATA/executor ou cobertura U2.
Não contornar a rejeição com outro helper, script, patch indireto ou novo path.

A afirmação preliminar de que o tratamento DATA existente bastaria para essa
correção era incompleta. U1 resolve a natureza dos bytes; não resolve a confiança
no programa que recebe a confirmação e executa o efeito. Token/contexto legítimos
e confirmação continuam necessários, mas não atestam a imagem mapeada.

## Três propriedades e estado factual

| Propriedade | Evidência disponível | Lacuna |
|---|---|---|
| Autoridade administrativa | Ato nativo independente, token/contexto, plano/declaração/confirmação e consumo implementados | Não prova independente de integridade/proveniência do executor quando ele é a mesma fonte rejeitada |
| Bytes candidatos | Handle original read-only/share READ, identidade/SD/digest, pins e cópia/hash/readback implementados | Tratamento DATA distinto não está condicionado a executor independentemente qualificado |
| Candidato no destino | Criação exclusiva, SD novo protegido, hash/flush e guarda estrita implementados | Só existe após efeito autorizado; não legitima retroativamente o executor |

Não foi demonstrada fonte de confiança alternativa já accepted no caller
UPDATE que permita retirar o predicado hoje. O subpredicado histórico108
continua UNKNOWN; o snapshot atual não é o handle da tentativa histórica.

## Menor decisão realmente necessária

Não reabrir U1–U7 integralmente. Se a entrada de workspace deve deixar de
satisfazer a guarda estrita atual, delimitar somente a especialização de U2
para fonte DATA, junto da prova independente exigida do executor. Preservar
captura original, recusa de escritores/delete concorrentes, continuidade e
SD observado/digest, pin selecionado e verificação independente do candidato.
Nenhuma redução de request/decisão/READ/SCQ, destino/DLLs, consumo, Stop ou close.

O critério independente do executor não pode ser um booleano, nome, hash
isolado ou mera execução elevada. Enquanto não estiver especificado e coberto
normativamente, qualquer getter/efeito continua DENY. Este documento NÃO fixa
um novo mecanismo ou aceita especialização. Não há proposta exata pronta para
aceitação nem comando operacional legitimamente completo.

## Verificação e continuação

Verificado após rejeição: guarda original do slot8 intacta; teste continua com
77 cenários; build solicitado não iniciou e seu log não existe. EXE workspace
preserva SHA25642d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca.
Nenhuma campanha histórica repetida. Nenhum arquivo instalado, SCM, ACL,
ProgramData, DLL, EXE anterior ou autorização histórica alterado.

Próximo passo é uma decisão focal dessa fronteira, sem infraestrutura ou nova
família de ADRs. Depois de cobertura byte-exata e implementação qualificada,
preparar UM novo pedido UPDATE individual, nunca rearmar baa501…4726.
Sem UPDATE/Start/Stop/endpoint/SCM/ACL/CA/CURRENT/rede/TPM/Q4/limpeza/refund.
Pedido NOT_READY; DENY; OS_QUIESCENCE UNKNOWN; E21 PENDING; ED11/Phase6 PARTIAL;
HEAD/WIP, históricos/pins e PST0.6.4 preservados.
