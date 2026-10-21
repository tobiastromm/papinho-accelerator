# Prova focal — CURRENT, captura e topologia

## Associação implementada, qualificação delimitada

interface_bind_committed_current é chamado somente no ramo de sucesso do publish existente, após commit normativo e publicação confirmados. Não emite epoch, não altera CAS nem concede permissão. Sob o lock existente, confere invocation, listener preparado, cópia presente/não retida, revisão preparada igual ao predecessor e modelo comprometido igual; então copia domínio/R/A/P reais para metadados separados da cópia imutável. Falta de prova deixa bound falso. Recuperação selecionada, Stop, estado incerto ou modelo divergente não recebem binding. Metadados são correlação, não identidade operacional nova.

Identidade da captura é a mesma invocation e o membro embutido write-once do participante, com revisão preparada preservada. Não há novo contador, registry, sistema de snapshots/pins ou hash usado como autoridade. PersistentIDs selecionados são os do modelo existente, confrontado com o committed; a captura nunca concede grants/Principal/TLS/Network Egress.

A nova query papacc_server_launcher_configuration_interface_current exige owner/thread corretos e pin CURRENT factual pelo guard existente, correspondência exata do domínio/R/A/P/invocation/revisão da captura, participante preparado e não retido. Antes de limpar o output rejeita sobreposição com root/owner/pin/children; revalida Stop/CURRENT depois da cópia. Usa gate sizeof(output)+allowance16384 sobre combined_reserved factual. Topology_proven e resolved continuam zero por construção. Flags preenchidas por caller não substituem o guard da root; esse negativo foi executado.

A publicação issuer-only pode preservar P conforme contrato accepted, mas não torna R/A intercambiáveis: metadados antigos não são automaticamente rebased para nova R/A. Query divergente nega, sem fallback para snapshot histórico. Stale writer permanece limitado pelo CAS/guards existentes; nenhuma restauração/recovery/fencing é introduzida. Retenção invalida bound antes de reter a source e conserva bytes/crédito; cleanup/reaper não mudam.

## Topologia implementada e limite obrigatório

papacc_interface_snapshot_copy_matches compara observações para cada PersistentID selecionado. Reutiliza persistent_bind_selection_resolve e bind_targets_resolve; compara is_up/is_loopback e conjuntos IP/scope por identidade. InstanceID/índice/nome são detalhes voláteis, não fallback. Endpoints de interfaces diferentes não são misturados para ocultar troca de endereço entre IDs. Identidades ausentes/obsoletas, flags ou endpoints divergentes não dão MATCH. Capacidades são verificadas antes de percorrer entrada, com8IDs/8targets por interface e32interfaces/128endereços herdados do storage; excesso ou corrupção negam sem truncar.

O comparator não consulta SO, não autentica a source, não testa disponibilidade real de porta, não define freshness e não detecta mudanças que ocorreram e desapareceram entre observações. Dois resultados iguais não provam a vigência da captura no consumo administrativo. O backend atual executa GetAdaptersAddresses, mas não fornece vínculo de invalidação/revisão da topologia ao resolver CURRENT. Nenhuma captura adicional foi introduzida no owner potencialmente bloqueando Stop. Não criamos listener/observer/notificador/lease para suprir esse requisito por interpretação.

A condição STOP “topologia sem prova” impede a ligação a RESOLVED. Não é uma negativa do executor. A comparação está disponível como parte independente; não serve de recibo ou autorização. Um mecanismo factual de revalidação/invalidação na fronteira de consumo pode satisfazer o requisito, mas precisa ser demonstrado no componente existente antes de implementação dependente. Esta análise não impõe por conta própria um novo serviço, política quantitativa ou capacidade obrigatória de interface.

## O que foi executado

Builds finais5 Debug/Release /WX passam, linkage completo, zero warnings.9CTests+3modos de root por configuração =24/24invocações finais PASS. Repetições anteriores preservadas, não somadas como novos cenários. Root snapshot-copy/stop/retain retornam antes de prepare_store/commit/publish. CONFIG_PUBLICATION=DENY e decisão PersistentID I4 foram respeitados. Nenhum CURRENT/A/P foi publicado nesta tarefa. Portanto o novo ramo positivo pós-commit NÃO foi exercitado nativamente; PASS anterior não é reutilizado como prova do hook novo.

| Caso pedido | Evidência final | Limite |
|---|---|---|
| Commit confirmado/CURRENT correspondente | ordenação e guard na fonte compilada; snapshots metadata só copiados após publish com sucesso | ramo positivo novo NÃO EXECUTADO; precisa execução administrativa específica autorizada |
| CURRENT divergente/pin falso | fake owner/pin e flag bound fabricada rejeitados; root real sem commit nega query; output sobreposto ao pin nega sem modificá-lo | não prova ramo interno de divergência sob pin CURRENT real |
| Topologia idêntica | fixture selecionada e duas enumerações nativas;4IDs/4MATCH/0DIFFERENCES em ambas configurações | duas observações, não prova de intervalo/recency |
| Topologia alterada/interface removida/IDobsoleto | fixtures determinísticas de endereço, ausência de interface, token e is_up distintos retornam false | nenhuma interface SO foi removida ou alterada |
| Renumeração | mesma identidade persistente com nova instanceID/index/endereço inalterado mantém MATCH | não usa índice como identidade; renumeração SO não executada |
| Rollback/Stop/retenção | três modos novos da tarefa anterior recompilados/executados na fonte atual; zero-child cleanup, Stop pipeline e retenção real de preparação conservam storage | Stop no meio de memcpy e falha nativa de release não induzidos |
| Stale writer/CAS conflict/unknown | CAS e guards de publicação permanecem inalterados; hook rejeita unresolved/stopped/recovery/model mismatch por fonte | nenhum commit/CAS foi executado; cobertura nativa nova não declarada |
| Disponibilidade/conflito físico de endpoint | apenas validação semântica de IP/scope/binding e modelo existente | sem bind/listener/rede, não qualificação de porta/provider/kernel |

Native discovery usa PersistentIDs reais; invocation/revisão dos testes unitários são fixtures. Modos da root usam root real dormente e material TLS histórico TEST_ONLY, nunca configuração produtiva ou autenticação de cliente. Não criamos credenciais/certificados/CA. Preservados provider nomeado PENDING, CLOSED/provisionamento, Model E/ED08 e todos gates DENY.

## Hunks oficiais e preservação

operations.json/diffs0–7 aprovados antes da aplicação; suplementos bounds, report-guard, alias e report-budget também aprovados com preimagens. Um suplemento de preparação converteu LF para CRLF no include; newline-operation.json restaurou byte-exatamente LF original com igualdade dos tokens/linhas verificada. A restauração foi oficialmente aprovada antes dos builds finais. Nenhuma supressão de warning, reset/clean ou negativa contornada. Código final conserva o WIP fora dos hunks aprovados.

## Handoff concreto

Antes de fechar a tarefa geral: (1) obter autorização específica de execução administrativa diagnóstica que permita commit/publish CURRENT com stores isolados para exercitar hook/pin, revisão divergente, CAS e Stop pós-commit sem admissão; (2) demonstrar a fonte factual nativa e a revalidação/invalidação da topologia na fronteira de consumo, com lifetime, orçamento e Stop preservados. Não substituir isso por timestamp/threshold inventado ou flag do caller; enquanto ausente, DENY. O código comparador não fecha esse gate sozinho.

Codec permanece PROPOSED; não serializar structs/ponteiros nem usar campos reservados BODY0.5/ROOT06. Respeitar I/R/A/P separados, CURRENT/pins e retenção. Nenhum novo roadmap ou arquitetura paralela é proposto.
