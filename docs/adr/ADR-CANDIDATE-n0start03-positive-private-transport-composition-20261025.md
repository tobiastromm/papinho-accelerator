---
status: proposed
revision: 1
date: 2026-10-25
scope-level: project
scope-target: N0START-20261024-03 positive composition private stdout only
supersedes: []
---
# Candidate — extensão focal T10–T14 à composição positiva N0START-03

## T10 — decisão pendente e precedência exata

Esta proposta NÃO é ACCEPTED nem aprovação operacional. A decisão T0–T9 revisão2 permanece integralmente vigente. A autorização técnica para qualificar a composição positiva não autoriza reinterpretar suas restrições read-only.

Solicitar, exclusivamente para N0START-20261024-03/tentativa `0b1de869e11493b7cb08fb329578f6ce`, extensão da MESMA exceção de stdout local privado também ao caminho positivo separado da única sucessora já fixada em L1. Mesmo serviço PapinhoAcceleratorLab20261023, mesmo owner/mutex, host/boot L1, supervisor elevado único, qualificador original e transporte de stdout; nenhum serviço recebe esse canal.

Se explicitamente aceita e registrada, esta decisão especializa SOMENTE: (a) a frase de T0 que mantém perfis/rotas positivas DENY nesta integração; (b) a expressão de T8 que restringe o adapter a opt-in read-only/positiva DENY; (c) a proibição de executar efeitos “nesta decisão”, que continua proibindo execução por esta aceitação normativa. A nova regra substitutiva para (a)/(b) é: permitir preparação, compilação e qualificação de entrada positiva separada e estritamente vinculada à identidade acima, usando o transporte T1–T6 já aceito; DEFAULT_PROFILE=EMPTY_DENY; execução positiva continua DENY até aprovação operacional exata distinta e todos os gates factuais. O registro accepted anterior não será reescrito. Nenhuma outra frase de T0–T9, N5, B+D, RAW1, N7, P3, L/H ou ED11 é dispensada.

## T11 — stdout não é autoridade

O mesmo endpoint exclusivo, local, não herdável, de sessão única e autenticado reciprocamente transportará todos os emissores stdout alcançáveis do qualificador, incluindo helpers positivos, primeira falha e recibos. Hello/ack continuam somente estabelecimento; bytes posteriores apenas qualificador→supervisor. Stdin, stderr, confirmação e Stop permanecem no console; não transportar comandos, intent, credenciais, autoridade ou solicitação de Stop pelo pipe. Nenhuma nova topologia, IPC genérico, helper, daemon ou autenticação independente.

A mudança de posição do dispatcher, já compilada/testada com positiva OFF, não ativa a rota. Antes de habilitar a entrada, qualificar a tradução completa dos helpers, falhas de escrita, truncamento e recibos, confirmar fechamento/retention e manter proteções contra replay/impersonation/TOCTOU. Não adotar endpoint/processo reaproveitado ou substituir o handle original por PID reaberto.

## T12 — recursos e lifetime

Preservar 128 handles por processo nativo, 1024 administrativos compartilhados, quatro papéis, freshness60s/1s, headroom5/9GiB, pisos4/8GiB, NONE_GRANTED, reservas reais indisponíveis e UNKNOWN históricos. O dispatcher não fornece créditos. Referências cujo último uso foi comprovado podem fechar de modo governado; close incerto preserva o original, primeiro erro, freeze, Stop e dependências, sem retry/double-close.

Os resultados read-only118/pico amostrado120/admin1005 são históricos. O fechamento de três referências após cycle_live foi qualificado em isolamento; não reduz o pico anterior dentro dessa função. A primeira consulta TCP no fixture adicionou uma referência observada, sem provar um teto universal de IP Helper. Custos de CreateProcess do trabalhador sintético não são custo comprovado de Start via SCM. Nada disso constitui reserva, orçamento fresco ou prova do processo real do serviço.

Aplicar a política empírica B4.C/D já aceita: o pico real concorrente e os recursos do serviço são saídas da única tentativa diagnóstica, não pré-condições circulares inventadas. Permanecem obrigatórios todos os caps controlados, compatibilidade das obrigações retidas, métricas dos participantes já vivos e admissão fresca antes de efeitos. Não consumir para contornar um gate já conhecido como insuficiente. Ausência de prova obrigatória ou UNKNOWN controlado => DENY.

## T13 — execução futura ainda separada

Nenhuma execução decorre desta proposta ou de sua eventual aceitação. Exigir pacote positivo exato com hashes e pins atuais, aprovação original específica ainda não existente, confirmação exata, orçamento fresco e exclusividade. Aprovações read-only consumidas não autorizam efeitos. Intent03 deve estar inequivocamente ausente antes de consumo; imagem e sink novos ausentes por handles originais; FILE_CREATE/CREATE_NEW finais; sem adoção de efeitos02. Preservar primeira falha, no máximo um Start e um Stop legalmente aceito, retenção sem retry/kill/rollback/delete. N7 específico continua limitado à identidade03 e aos fatos obrigatórios N2.

Não exigir outro read-only equivalente apenas porque o dispatcher mudou de posição, se nenhum novo fato discriminante o justificar. Após integração positiva tecnicamente qualificada, a operação necessária é a única diagnóstica positiva exata, sujeita à aprovação própria e aos gates frescos. Se uma lacuna técnica permanecer, resolvê-la dentro do escopo antes de solicitar execução.

## T14 — evidência e decisão solicitada

Evidências e limites: `docs/integration/n0start03-integrated-closeout-20261024/positive-composition-closure/`. Conferir relatório final, hashes, patch focal, tradução compilada, testes reais isolados, negativos e matriz ED11 antes de aceitar. Não converter fixture em prova operacional, saída0 em quiescência ou observação em máximo.

Alternativas: (A) manter T0/T8 atuais e positiva bloqueada; (B) aceitar esta extensão estrita, qualificar a entrada positiva e preparar aprovação própria; (C) abandonar o transporte e voltar à herança ampla, rejeitada por isolamento/concurrency e pressão de recursos. Recomenda-se B somente após revisão específica do owner; A permanece efetiva enquanto isso. Não modificar contratos de produção, PST/Browser/wire ou quotas.

NORMATIVE_EXTENSION=PROPOSED
POSITIVE_OPERATIONAL_AUTHORITY=DENY
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
