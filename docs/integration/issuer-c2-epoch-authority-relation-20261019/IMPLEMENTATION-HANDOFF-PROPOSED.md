# Handoff implementável após eventual aceitação específica

Status: PROPOSED. Nenhuma autorização de implementação, rede, emissão ou publicação decorre deste arquivo. Documento normativo candidato: CONTRACT-PROPOSED.md, P0–P7. Os18 hunks e o WIP atuais continuam preservados e não aceitos como solução live.

## Ordem de trabalho dependente

1. Owner verifica/aceita o texto exato e registra pelo processo governado, sem alterar silenciosamente decisões accepted. O gate R-ACTIVE de revogação de Sessions permanece distinto e não é requisito artificial para cadastro/renovação ISSUER_ONLY; bloqueia somente a funcionalidade/disposição que dele depende. Não ampliar a aceitação para CNG ou recuperação sem provas.
2. Auditar/propor um descriptor tipado de snapshot da MESMA root: identidade da root, seleção R/predecessor, referência I e autoridade/geração/status, confirmação A, referência ao contexto operacional P/projeção, incarnation de lifetime e pins/leases. Usar os slots/owner existentes. Definir alias/overflow/ausência e codec privado explícito onde for necessário; preservar BODYs históricos. Não congelar flags de prova a partir de dados de configuração.
3. Implementar classificação da alteração. ISSUER_ONLY deve provar conteúdo operacional e referências inalterados. A mudança de credential inventory não concede grant. Se incluir mudança autorizadora, tratar como transação operacional, sem publicar metade da configuração. UNKNOWN bloqueia o efeito dependente.
4. Integrar PREPARE/VALIDATE/CAS/commit com participantes reais, ator administrativo e ED11 aplicável. Confirmar CURRENT/A na root; manter P vigente em ISSUER_ONLY. Somente a autoridade root/policy produz P quando a projeção operacional muda. Nenhum valor vem do issuer/Browser. Ack perdido causa reconciliação idempotente, não nova assinatura.
5. Separar três provas na implementação: referência de Session ainda protegida; permissão para novo efeito no CURRENT real; autorização para a operação capturada exata sob policy atual. Reusar um helper puro para identidade onde seguro, mas não usar um booleano de held como resultado de autorização dessas três classes. O cleanup tem prova de quiescência própria, independente da perda de autoridade de DATA.
6. Admissão: adquirir pin CURRENT antes de expor Session e capturar I/R/A/P corretos. Operações novas revalidam status e contexto operacional vigentes. BIND valida identidade/token/expiry e policy atual. Continuidade preserva epoch/operação/grants e reavalia revogação/deadline/crédito; nenhuma exceção de epoch antigo. Garantir que uma nova Session usa nova R mesmo quando o listener continua segurando uma lease de R antiga validada contra a mesma projeção P.
7. Reaper: aplicar o caminho sem efeitos destrutivos adicionais. Retirar pin só após todas as provas nativas e referências quiescentes; falha mantém owner e crédito. O helper proposto H17 precisa distinguir A de P, além de ticket/serial/domínio/revisão/incarnation. Não inicializar pins para Sessions antigas por observação retrospectiva do novo CURRENT.
8. Recuperação: vincular checkpoint/fencing factuais aprovados à mesma cadeia root/issuer/policy antes de permitir autoridade recuperada. Proveniência de P anterior não restaura Session viva. Reserva consumida e assinatura incerta nunca retornam ao pool. Sem prova: DENY, com status consultável e retenção.
9. Preparar nova operação exata com preimagens e dependências completas, matriz de regressões e delta ED11. Submeter oficialmente. A negativa anterior permanece válida; não aplicar os18 hunks como atalho nem dividir a operação para evitar revisão. Compilar/testar somente após autorização de implementação e aprovação aplicável.

## Oracles mínimos da futura implementação (não executados)

| Caso | Resultado exigido |
|---|---|
| Cadastro/renovação ISSUER_ONLY com Session/operação antiga | I/R/A novos factuais; P continua vigente; S antigo e buffers/handles preservados; operação segue apenas sob gates atuais |
| Nova Session após publicação | referência R CURRENT nova, mesma autoridade de policy P se inalterada; autenticação/grants normais, sem retrofit na Session antiga |
| Novo OPEN em Session antiga, P ainda vigente | revalidação CURRENT/status/policy obrigatória; pin antigo sozinho não concede |
| Novo OPEN com P obsoleto | DENY; não copiar P novo para Session antiga |
| BIND com identity/P/token antigo indevido ou credencial/status revogado aplicável | DENY sem consumo indevido de autoridade/victim cleanup |
| Redução/revogação operacional | bloquear novo trabalho conforme policy atual; manter immutable grants upper bound e retenção |
| Aumento de grants | sem expansão de Session existente; nova admissão governada |
| Revogação administrativa com Sessions ativas sem R-ACTIVE | registrar status/new-admission denial conforme escopo accepted; não alegar active Stop/continuidade qualificada; gate dependente continua pendente |
| Policy/Principal/trust alterado apresentado como ISSUER_ONLY | rejeitar classificação ou exigir transação operacional completa |
| Slots/tickets esgotados, stale writer, alias ou reentrância | negar sem overwrite/wait ilimitado; resultado de propriedade preservado |
| Stop em cada fase e falha de fechamento | sem novos efeitos; pins/leases/crédito permanecem até finalização factual |
| Crash/rollback/clone/ack incerto | distinguir commit/CURRENT/A/P; high-water não decresce; sem checkpoint/fencing recuperação DENY |
| Apenas dados antigos íntegros, relógio ou PID | insuficientes para CURRENT, fencing, Principal ou epoch vigente |

Não usar fixtures como autoridade, nem testes sintéticos como qualificação nativa. Não escrever novas comparações permissivas para contornar os casos negativos. Não executar nenhum desses casos nesta tarefa documental.

## Decisões/gates remanescentes

- Aceitação específica da relação proposta I/R/A/P e da classificação ISSUER_ONLY.
- R-ACTIVE: efeito de revogação de credencial sobre Sessions/operações vivas, caso a funcionalidade dependa dele. Não reabrir regras já accepted de revogação de policy.
- Prova de transação live, lifetime/Stop/latência, reserva ED11 e recuperação com checkpoint/fencing; requisitos de implementação/qualificação, não fatos supridos por este contrato.
- CNG/proteção/backup/real issuance continuam gates próprios. Box/ISA e PST0.6.5 não são qualificados/promovidos pela proposta.
