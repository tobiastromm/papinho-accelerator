# Matriz de referências — PARSED / VALIDATED / MODEL_BOUND / RESOLVED

| Referência/fato | Fonte e prova no código | Resultado / disponibilidade |
|---|---|---|
| Estrutura e referências entre blocos | pc_parse / pc_validate / pc_view existentes, strings owned | VALIDATED não é RESOLVED |
| Contexto listener/porta/profile | PAPACC_SERVER_CONFIG existente; igualdade exata e exatamente um match por listener | MODEL_BOUND só quando correspondência única; ausência/ambiguidade negam |
| Coleção | PC_MODEL_BINDING.listeners[4], count explícito | Nunca truncada; wrapper da root atual exige exatamente1 |
| Bind ALL | Modelo existente + papacc_bind_targets_resolve | Dois wildcard targets IPv4/IPv6 de um listener lógico; não abre sockets |
| SELECTED/interface_ref textual | Não existe tradução governada de string para PersistentID localizada | UNSUPPORTED, inclusive nome alegado de interface; não usa presentation/index/IP como identidade |
| PersistentID tipado existente | papacc_persistent_bind_selection_resolve já implementado, mas sem fonte textual aprovada para alimentá-lo | Não foi reimplementado nem alimentado com valor inventado; lookup textual positivo pendente |
| Security ref | igualdade de bytes com config atual e source.reference PUBLISHED validada; source_validate existente | Resolver não lê environment mutable nem prepara fonte nova |
| TLS/mTLS e provider | PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION da fonte já mantida pelo owner; validador existente e callbacks presentes | Não inicia PST ou autentica peer; material/root positive nativo não executado aqui |
| access_auth NONE/OPEN | método textual explícito + admission da policy existente iguais | Não remove mTLS do Secure; sem autenticação de operação por PC_OK |
| CLOSED/provider_ref nomeado | Interface de callback existe; não há identidade/revisão governada de provider nomeado localizada | UNSUPPORTED; não toma security_ref como alias nem fornece autenticação fictícia |
| Capability/policy | papacc_server_config_validate e conteúdo normalizado exato: enabled keys, ordem, ações, predicates e bytes Principal16 | Sem aplicar policy textual; identidade32 copiada só do modelo existente; alteração/ausência nega |
| Principal/grants | predicate deve coincidir byte-exato com policy existente; source PrincipalResolver/AuthZ permanecem factuais e separados | Não chama callbacks com identidade inventada nem concede grants; grants de Session só nos caminhos operacionais atuais |
| I/R/A/P/CURRENT | papacc_server_launcher_configuration_current com pin/ticket/projeção e provenance da única root, antes/depois | Único wrapper nativo pode marcar resolved; não cria epoch/commit/publish |
| Expected revision | argumento factual obrigatório comparado com pin atual, não novo campo de papacc.conf | Divergência nega; contador não vira autoridade |
| Stop/retenção | threadowner; root stopped/unresolved/authoritylost/current false; source PUBLISHED/valid; console stop interlocked | Saída invalidada/zerada; nenhum pin/lease/crédito adquirido/liberado pelo resolver |

Os labels locais dos blocos não criam registry. O match do conteúdo com snapshot existente prova correspondência, não que o label seja identidade global administrada. Output contém valor/correlação histórica; não é ticket de admissão ou prova de currency futura. Todos os blocos declarados precisam participar do contexto: órfão não é silenciosamente aplicado/ignorado. Sem resolver real de SELECTED ou provider nomeado, o candidate dependente falha fechado antes de RESOLVED.
