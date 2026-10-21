# Matriz de precedencia, condicoes e efeitos

| Plano | Semantica vigente | Fonte | Codigo / limite |
|---|---|---|---|
| Composicao de governanca | accepted aplicavel restringe source/future/code; excecoes delimitadas | OrganizationEngineering/ADR0001-3; CON; decisoes locais | historico/proposed nao substitui norma |
| Fontes de configuracao servidor | normalizar para mesma root; nenhuma heranca universal de sources foi definida | PE4,A09,A10,A44 | nao importar precedencia Browser para servidor |
| Selecionar contexto/tabela | configuracao efetiva valida server-owned antes de avaliar | A43 contexto; A48B | profile/port/admission/key; nao context de outro listener |
| Condicoes de uma regra | TODAS devem corresponder com fatos validos | A43; A48H | Principal16 opcional mais key/contexto; sem geral OR/NOT |
| Ordem de regras | primeira terminal correspondente decide; sem sort por especificidade | A43 algoritmo2-4 | papacc_server_policy_evaluate loop com retorno imediato |
| ACCEPT anterior e DENY posterior | ACCEPT daquela decisao; nao procurar veto posterior na mesma tabela | A43 exemplos | nao escolher DENY_OVERRIDES arbitrario |
| DENY anterior e ACCEPT posterior | DENY termina a decisao | A43 exemplos | nenhuma regra posterior reverte |
| No-match/tabela sem terminal/capability nao habilitada | DENY | A43 passos5-6,A48H | decisao inicialDENY e ausencia de enabled key nega |
| Condicao desconhecida nao implementada | rejeitar modelo ou negar decisao; nao ignorar/virar wildcard | A48E,A43 | policy_validate mask~PRINCIPAL retorna NOT_SUPPORTED |
| Fato indisponivel em condicao suportada | nao produz match positivo; demais gates obrigatorios continuam | A43 | principal_valid falso nao satisfaz predicate; wrapper real exige AuthN; nao inventar geral UNKNOWN language |
| Erro/ref invalida/contexto ambiguo | fail-closed; sem guessed refs/fallback | A10,A26,A43,SITE | validators/resolvers retornam erro; formato futuro nao aceita erro silencioso |
| Gate em outra dimensao nega | efeito nega apesar de ACCEPT na tabela | A28,A43 separacao | AuthN, snapshot/grant, CURRENT/P, destination, TLS, ED11/lifecycle independentes |
| Exact destination v1 | union de allow rules positivas exatas+default deny+classes estruturais/sensiveis | A26,A21 | sem ordem terminal DENY/CIDR/regex na v1 |
| Grupos/heranca/prioridadenumerica | sem contrato completo | A28,FUT,A43 limites | nao criar group DENY/parent override/grupo aninhado |
| Managed Policy Browser | resolucao de config do consumidor, nao rules server | historical ordered rules spec, Browser policy | nao override de servidor; nenhum compartilhamento automatico |
| Atualizacao CURRENT | commit completo antes de publicar; conflito/unknown nao retorna old authority por conveniencia | A44,IRP3/P6 | pins antigos=memoria, nao grants |
| Revogacao operacional | policy atual pode negar efeitos, sem ampliar snapshot ou reescrever identidade | A12,IRP4 | cancel/final/teardown narrow nao concede novo trabalho |
| Revogacao de certificado em Session ativa | POLICY_PENDING | PROV7,IRP5.2 | nao presumir Stop-on-revoke nem grandfathering |

AND implicito e apenas conjuncao dos predicates de uma regra suportada; varias regras sao alternativas ordenadas, nao uma AST OR arbitraria. NOT, chains, jumps, efeitos nao terminais, prioridades por grupo/especificidade e calendarios exigem contratos proprios. Uma nova source nao pode selecionar essas semanticas por conveniencia.
