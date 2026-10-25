# N0START-03 corrigida — censo aprovado, orçamento administrativo negado

Saída integral do owner copiada byte a byte em owner-manual-output.txt, SHA256 `ef2a1fe939bafc117f17606bc993dca8a9794cbf8aa69304b6a7d4f319b70e92`. Pedido `64f20a0fcbe94782b3178c9da27ce0c76a989e478802d7e2262d3cef04d0e18b` e aprovação `b1d4185c2c581c9c024621bf13adfb9156b516a51da0c074fdfee85c89a8697e` foram verificados, junto dos oito artefatos e dependências normativas. HEAD `913c90cf2328f7939d8f5b927640601903ac6e91` preservado. Aprovação agora usada uma vez; o arquivo original não foi modificado e NÃO autoriza repetição. Não houve nova execução pelo Codex, query SCM/objeto protegido, setter ACL ou alteração funcional.

## Resultado factual

Exit2, primeira falha1816 em PREFLIGHT_METRICS. Intent0/effect0/diagnostic0/starts0/stops0. Close0 e TRANSPORT_LOCAL_CLOSE=CONFIRMED/first0/retained0. Fechamento próprio confirmado pelo recibo não prova quiescência global/histórica. Imagem/sink foram consultados ausentes pelos handles originais, sem reserva atômica ou inferência de ausência futura. Nenhuma criação/rebind/Start/Stop reportada. Histórico02/P3/UNKNOWN permanece.

A correção de composição foi exercitada nativamente: dois censos retornaram first0;328callbacks compartilhados e2fronteiras completas em cada um. Tempos390ms e453ms; idades globais6250ms e13125ms. Com a implementação atual,328 é N+2 callbacks internos, compatível com326entradas; não são328participantes. Os checks voláteis por entrada continuaram. Há progresso operacional concreto além do antigo PRE_METRIC_REVALIDATION. Isso não reconstrói a causa exata do1460 histórico, prova todo censo futuro ou garante a janela final1s, que não foi alcançada.

## Primeiro impedimento demonstrado

| Papel | Handles amostrados | Limite aplicável |
|---|---:|---|
| PowerShell PID18372 |767|Compartilhado com auxiliar|
| Auxiliar PID24196 |270|Compartilhado com PowerShell|
| Administração conjunta |1037|1024 — excede13|
| Qualificador PID18948 |112|128 — margem amostrada16|

Somar767+270 é obrigatório nesse orçamento; NÃO somar o qualificador nem transferir sua margem. Threads administrativas25+8=33, private/commit77881344bytes, WS114409472bytes, stack11198464bytes, dentro dos respectivos limites impressos. Nenhum desses resultados concede allowance/reserva/headroom futuro.

Fonte: `ig_observe` coleta processos, depois chama `papacc_server_service_admission_bd_metrics` antes de imprimir IG_HOST. O owner revalida current/inventory e executa `ps_bd_costs`, cujo limite administrativo é h<=1024 e retorna ERROR_NOT_ENOUGH_QUOTA1816 se excedido. Assim, há uma violação numérica obrigatória que explica a rejeição esperada. O diagnóstico genérico gate_or_uninstrumented_api não é um trace distinto da instrução interna: não se afirma exclusão retrospectiva de toda outra API que pudesse retornar1816. Independentemente disso,1037já impõe DENY sem novo efeito.

Não há IG_HOST nessa saída, portanto valores de RAM/commit disponíveis não são conhecidos a partir do anexo. O código coleta host antes dos processos, mas falha antes de publicá-lo. Não preencher com dados anteriores.112não é pico total: há PSS/transitórios e picos não amostrados UNKNOWN; não atribuir redução causal de118→112 ao patch. O serviço não existe nesta amostra; seu PID/token/recursos/NTSTATUS continuam NOT_RUN.

## Atribuição e menor próxima frente técnica

Histórico read-only1005 (734+271) contra esta amostra1037 (767+270): diferença32 entre execuções, sendo+33PowerShell/−1auxiliar. São processos e instantes diferentes. Não é prova de leak, de custo de uma chamada ou de regressão do patch. O anexo não possui baseline anterior ao launcher, tabela de objetos ou instante de criação de cada handle. Não há base para fechar13handles arbitrários, remover pins obrigatórios, matar processos, aumentar1024 ou solicitar nova tentativa idêntica. A economia observada no qualificador não resolve administração.

O próximo trabalho discriminante é atribuição focal do conjunto PowerShell767: separar baseline do processo/host, originais de pin/lock, autenticação/transporte e supervisão, com lifetimes/último uso e medidas equivalentes nos fixtures existentes. Reutilizar as provas Sysinternals/console anteriores, sem repetir campanhas genéricas. Uma redução só será válida com referências dispensáveis identificadas e fechamento governado demonstrado; não com limpeza de caches, GC ou troca de orçamento para produzir PASS. Consultar o processo depois do retorno, caso ainda exista, não recria a tabela contemporânea e não deve ser vendido como causalidade histórica.

Nenhuma correção funcional foi inferida/aplicada nesta reconciliação, pois a evidência não identifica uma referência dispensável concreta. Não há novo pacote operacional executável. Antes de qualquer autorização futura, resolver a atribuição ou apresentar uma decisão arquitetural concreta; depois exigir orçamento fresco com margem para toda a composição. “Reduzir13” apenas chegaria ao limite nesta amostra passada e não garante viabilidade positiva.

## ED11 e Phase6

E13 incorpora censo corrigido PASS_AT_OBSERVATION nesse alcance; não o gate final1s. E11 tem FAIL_AT_OBSERVATION1037/1024; o gate global continua PARTIAL. E10 tem112/128amostrados, não pico/serviço. E15consumo/imagem/sink/rebind03 e E16–E18diagnóstico/serviço/Stop/quiescência continuam NOT_RUN. Phase6 inclui ainda componentes externos ao LAB. Nenhum fechamento global, refund/crédito ou capacidade positiva é declarado.

CENSUS_CORRECTION=PASS_AT_OBSERVATION
ADMIN_HANDLES=1037
ADMIN_LIMIT=1024
ADMIN_EXCESS=13
QUALIFIER_HANDLES=112
FIRST_FAILED_STAGE=PREFLIGHT_METRICS
FIRST_ERROR=1816
INTENT_CONSUMED=NO_REPORTED
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE_REPORTED
ACL_MUTATIONS=NONE
QUIESCENCE=UNPROVEN
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=TARGETED_ADMINISTRATIVE_REFERENCE_ATTRIBUTION_BEFORE_ANY_NEW_OPERATION
