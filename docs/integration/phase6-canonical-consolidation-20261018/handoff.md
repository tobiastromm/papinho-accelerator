# Handoff mínimo — blocker de ownership do wrapper

1. Retomar do WIP canônico atual e da baseline/hash/preimagens desta pasta; não substituir a árvore.
2. Corrigir exclusivamente o caminho comprovado de alias nativo após criação de pst_win32_socket_transport, distinguindo wrapper consumer-owned de child PST-owned. Preservar o grafo durável e impedir retry/reuso após incerteza; sem alteração PST.
3. Revisar também o caminho equivalente outbound TLS e falhas de aquisição de accepted socket, acceptor e connector. Não declarar resolvidos por esta nota.
4. Concluir propagação result-returning até a root antes de liberar arrays/dependências. Preservar as correções canônicas de logger e lifetime. Isolar main/application_lifetime para não mudar produção inadvertidamente.
5. Compilar Debug/Release normal e produção em outputs novos, sem sobrescrever históricos; executar somente regressões offline permitidas. Reconciliar armazenamento real, stack, multiplicidade e retenção antes de alegar ED11.
6. Após conformidade de ownership, continuar a integração seletiva real dos componentes C2/C3/root factual existentes. Não usar ordinal, epoch fixo, factory diagnóstica ou callback sintético como autoridade. Não habilitar admissão ou executar rede.

Não há negativa atual do executor: as operações desta retomada foram aprovadas. A parada é motivada pelo defeito factual de lifecycle descrito no closeout, conforme o STOP do owner. O build Debug existente antecede as últimas alterações nativas e não qualifica o estado atual.