---
status: proposed
scope: positive evidence for INITIAL_BOOTSTRAP only; no implementation authority
---
# Evidência positiva de primeira inicialização — handoff focal

Owner confirmou nesta tarefa que nenhum registro/procedimento canônico aprovado foi identificado. INITIAL_BOOTSTRAP=DENY. Não criar marcador, sidecar, registry ou nova autoridade. Esta proposta não estabelece um mecanismo por existência deste documento, não altera D2/D4/D3 e não autoriza sua implementação. Decisão normativa específica e revisão oficial serão necessárias.

## Fato que falta

Provar positivamente que o alvo pertence a uma implantação nova autorizada e não é instalação anterior apagada, store parcialmente criado, backup/restore ou histórico desconhecido. Observações de arquivo ausente, CURRENT ausente, diretório vazio, identidade/digest válidos ou CREATE_NEW não demonstram esse fato. Uma declaração/flag/callback isolado também não vira autoridade pelo software. O init atual comprova somente parte da condição de armazenamento; não é resolvedor administrativo inicial.

## Procedimento administrativo explícito a decidir, usando a root/store existentes

1. Autorizar a operação de commissioning pelo ator/mecanismo administrativo canônico aplicável, vinculada à invocação da ÚNICA root e alvo exato. A autorização para operar não substitui a evidência de história. Não usar autenticação de cliente/Principal/grants como autorização administrativa implícita.
2. Selecionar uma fonte existente e governada de proveniência da implantação/armazenamento (por exemplo, registro de alocação/entrega de ambiente de um instalador ou inventário administrativo já governado, caso exista e seja qualificado). Exigir referência íntegra/protegida, cadeia de custódia, responsável e método que realmente demonstrem nova alocação e ausência de predecessora. Nenhuma dessas fontes concretas está identificada/aprovada; não criar automaticamente um inventário para preencher a lacuna. Se não houver fonte suficiente, DENY.
3. Vincular fatos nativos de identidade do alvo e caminho à mesma evidência administrativa; revalidar antes do efeito. Identidade nativa distingue objetos, não comprova histórico por si. Não escolher timestamp/nonce/epoch novo para fingir prova.
4. Classificar explicitamente NEW_INSTALLATION versus EXISTING/RESTORE/UNKNOWN. Evidência de predecessor/backup/revogação/resultado pendente implica caminho de recuperação/update aplicável, nunca bootstrap. Sem checkpoint/fencing quando exigidos, recuperação permanece DENY. Não excluir/renomear store para mudar a classificação.
5. Validar os participantes/referências/leases da mesma root (Secure/material/trust, policy/capabilities e Site TLS quando aplicável), fonte administrativa e suporte do target conforme D2.1. Fonte, parser, hash ou callback sintético não concede RESOLVED. Named provider/SELECTED/carrier seguem seus próprios gates.
6. Só com TODAS as provas positivas e contrato do procedimento accepted, executar criação exclusiva e commit pelo worker/store canônicos. A exclusividade protege o efeito contra corrida; não substitui passos1–4. Validar ausência de base/LST1/R/A/P anteriores para bootstrap sem fabricar epoch. Não publicar CURRENT antes do commit; A/P somente pela autoridade existente após fatos correspondentes.
7. Falha/incerteza preserva root/packet/handles/créditos/história e impede retry destrutivo, reset de bootstrap, nova CA ou reutilização de serial. Evidência/resultados devem referenciar os mecanismos de auditoria já existentes; nenhum segundo journal é autorizado. A eventual forma de persistir a vinculação no mesmo store exige contrato próprio se ainda não coberta, sem usar bytes reservados D3.

## Evidências que a decisão futura precisa designar

| Obrigação | Fonte/aprovação atual | Critério de aceitação futuro |
|---|---|---|
| Ator administrativo e escopo de commissioning | mecanismo concreto não vinculado para este fluxo | autorização verificada pela root existente, não flag de entrada |
| Instalação sem história anterior | NOT_ESTABLISHED | fonte positiva governada, protegida e vinculada ao alvo; não simples ausência |
| Origem/custódia de armazenamento novo | NOT_ESTABLISHED | cadeia de alocação verificável com responsável e limites explícitos |
| Identidade/caminho/exclusividade | componentes nativos existentes, composição de bootstrap ainda ausente | fatos nativos no mesmo efeito e revalidação sem fallback |
| Material/refs/participants | mecanismos existentes, inputs reais deste bootstrap não fornecidos | leases/proveniência válidas, nenhum fixture promovido |
| Restore/instalação anterior/UNKNOWN | DENY para bootstrap | encaminhamento ao gate real de update/recuperação, nunca adoção implícita |

Não há implementação diretamente autorizada desta proposta. O menor próximo ato é selecionar normativamente a fonte/procedimento concreto que fornece a prova positiva e seu responsável; depois revisar os hunks exatos da mesma root/worker, com regressões e ED11. Não é necessário novo coletor/supervisor/registry/CA. Não é um novo roadmap.
