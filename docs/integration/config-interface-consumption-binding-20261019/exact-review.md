# Revisão dos hunks exatos

Quatro operações em operations.json e diffs0–3. Fonte de autoridade: tarefa do owner, decisões accepted/ADR0007/0009, Model E/ED08/ED11. Nenhum parâmetro ou regra accepted alterada.

0 owner.h: declara guard privado de consumo SELECTED, sem estado/autoridade nova.

1 server_c8_resource_root_win32.inc: guard read-only chama relatório CURRENT existente, exige current_bound/topology_proven/resolved factuais. O relatório atual não fornece prova topológica e o guard nega. activate exige o guard para SELECTED antes de reservar recursos/alocar/inicializar WinSock e novamente imediatamente antes de bind_owned. Não retirar gates ED11/READY/admissão existentes. Falha no segundo ponto mantém o caminho RETAINED já existente. Sem consulta nativa ou callback novo no owner, nenhum worker/registry/snapshot/counter novo. ALL não usa interface_ref e não é promovido a RESOLVED por esta função.

2 config_binding_native_test: NULL/fake owner não podem passar o novo guard. Não é prova de CURRENT real.

3 canonical_root_transaction_test: modo novo snapshot-consume usa PersistentID realmente enumerado antes de adquirir a root, configuração diagnóstica/denyrules e material TLS TEST_ONLY existente. Depois da preparação, guard/activate negam sem qualquer recurso/listener/WinSock adquirido. Cópia/lifetime continuam reais e rollback fechado termina sem commit/publish. Ramo snapshot-* retorna antes de store/commit/CURRENT; nenhum sucesso nativo de consumo é inventado.

O vínculo do guard ao ponto real é comprovável por fonte e negação no consumidor real. Positivo de bind/listen dependeria de rede/admissão e fica NÃO EXECUTADO. CURRENT positivo novo também não será executado sob CONFIG_PUBLICATION=DENY e decisão accepted I4. Comparação de observações isoladas não configura topology_proven. Não criamos prova por timestamps, sinais ou flags do caller.

Fato do consumidor: bind_owned consome IP/porta/scope, não PersistentID; interface_instance_id não é usado no bind nativo IPv4/IPv6. SO_EXCLUSIVEADDRUSE não atesta identidade de interface. Sem mecanismo factual de aplicabilidade/invalidação, mudança entre validação e esse consumo não pode ser declarada segura. STOP mantém SELECTED negado. Provider nomeado PENDING; nenhum efeito nos gates de TLS/grants/Network Egress.
