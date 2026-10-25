# N0START-03 — falha pré-consumo e reconciliação factual

A operação manual autorizada retornou exit2, primeira falha1460 em PRE_METRIC_REVALIDATION/bd_census_predicate/process_snapshot. Intent0, effect0, diagnostic0, starts0, stops0, close0. O transporte confirmou seu fechamento local com first0/retained0. A aprovação exata foi utilizada nessa única execução; preservada, sem reuso ou retry. O intent não consumido não torna a autorização novamente disponível.

## Alcance da evidência

Origem: saída colada pelo owner na conversa; OWNER-OUTPUT-EXCERPT.txt é transcrição declarada dos recibos relevantes, não arquivo original byte-exato do console. Identidades e hashes declarados pelo resultado correspondem ao pedido. Artefatos atuais e dependências accepted foram recalculados e continuam nos hashes aprovados. Nenhum launcher nativo, SCM ou objeto protegido foi aberto/executado para repetir a observação. O resultado não equivale a nova verificação factual da ausência atual dos nomes ou de um intent persistente; os valores0 são fatos do recibo dessa execução.

Imagem e sink foram observados ausentes pelos handles originais antes da falha. Não são reservas atômicas. Não houve criação/rebind/Start/Stop reportado nesta execução. Os efeitos02 e P3 históricos permanecem preservados, sem inferência de rollback ou quiescência global. Não há amostra ED11 de handles/memória nesta saída: a execução nem alcançou PREFLIGHT_METRICS. Não reaproveitar118/120/1005 como valores atuais; tampouco atribuir a falha a128/1024.

## Cadeia causal localizada no código

`ig_project_census` faz revalidação B+D e inventário antes do snapshot. Em cada PROCESSENTRY32, chama `ps_bd_census`; este chama `ps_bd_inventory`, que chama `ps_bd_console`. Em N0, `ps_bd_console` invoca a callback `psa_bd_aux_current`, que verifica novamente token/identidade/sessão/console e termina em `PSA.bd.check(d)`, isto é, `ig_n0_current`.

Fora da janela de métricas, `ig_n0_current` executa hashes protegidos/configuração/binário/dependências/dossiê/proveniência/políticas/aprovação, RAW1/ancestrais, operador e readback SCM/recovery. Portanto essas provas completas são repetidas por entrada enumerada. B1 já corrigia uma duplicação dentro de `psa_bd_current`; não eliminou essa repetição B2 entre entradas do censo. A propriedade de source é demonstrável sem repetir LAB.

O1460 é propagado pelo predicado e rotulado como code/native1460; isso não é prova de timeout original de CreateToolhelp32Snapshot ou de API do kernel. Pode ser retorno sintético de freshness em subvalidação. O PID6380 é a entrada do snapshot no momento do erro. `role=4` é rótulo diagnóstico, não prova de quinto papel ou processo indevido. A falta de timestamps por callback impede identificar retrospectivamente a observação/condição exata que expirou ou o custo individual de hash/SCM. A janela global60s, o limite final1s e a posição pré-métricas são distintos: não é demonstrado que o gate final1s foi alcançado.

## Primeiro bloqueio técnico e menor correção a qualificar

A repetição completa por entrada é o primeiro mecanismo concreto a investigar, não um defeito de produção presumido. Uma correção B2 somente é válida se um censo limitado a uma geração mantiver provas obrigatórias, handles originais/imutabilidade, identidade/token/anti-impersonation, exclusividade, mudança durante enumeração e validação fresca no ponto de uso. Deve manter validação integral antes/depois do snapshot e negar mudança/Stop/expiry, sem resetar artificialmente observed/began, usar cache global, estender60s/1s ou transformar stale em PASS. A callback direta continua íntegra para outros callers.

A equivalência dessa consolidação ainda não foi demonstrada; não aplicar remoção simples de `ps_bd_inventory` por entrada como se fosse otimização já segura. Nenhum patch funcional, pin ou build foi modificado nesta reconciliação. Não repetir builds/testes sem mudança. Próxima frente técnica é qualificar B2 com callbacks reais/APIs simuladas e geração limitada: mudança de token/identidade/arquivo, Stop, reentrância, UNKNOWN/close incerto, expiry dentro do snapshot e fim de geração. Os intervalos e limites permanecem inalterados. Uma futura execução exige pacote atualizado e autorização original específica; não solicitar repetição idêntica da aprovação usada.

## Matriz e autoridades

E15 foi exercitado apenas até consulta de ausência; consumo/colocação/rebind NOT_RUN. E13 agora tem falha factual de freshness no censo, sem timestamps discriminantes. E16–E18 diagnóstico de serviço/Start/Stop/quiescência continuam NOT_RUN. Capacidade positiva NÃO demonstrada, ED11/Phase6 PARTIAL. Gates de outros componentes permanecem fora desta prova. CURRENT, assinatura e recuperação autoritativa DENY.

START_EXECUTED=NO
STOP_EXECUTED=NO
INTENT_CONSUMED=NO_REPORTED
SCM_MUTATIONS=NONE_REPORTED
ACL_MUTATIONS=NONE
LOCAL_CLOSE=CONFIRMED_IN_RECEIPT
GLOBAL_QUIESCENCE=UNPROVEN
RETRY=FORBIDDEN
NEXT=BOUNDED_CENSUS_REVALIDATION_EQUIVALENCE_QUALIFICATION
