# Closeout — contrato textual inspirado em squid.conf

TASK=PACC-PHASE6-SQUID-INSPIRED-CONFIGURATION-CONTRACT-1. Documentação PROPOSED, sem implementação. Branch ativa main, HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a confirmado; nenhuma troca de ref. Continuação focal da auditoria existente; não promove histórico/propostas a autoridade corrente.

## Entregas

- [Reconciliação ADR0009](adr0009-reconciliation.md): coleção lógica, identidade, bind, porta, profile explícito, security ref e contexto administrativo. Múltiplos sockets não são múltiplos listeners. Conflito factual F01 permanece no código; proposta não declara conformidade implementada.
- [Gramática proposta](grammar-proposed.md): versão1, comentários #, sem comentário por ;, regras lexicais, campos obrigatórios, presença, diagnóstico com linha/coluna/motivo, suporte por target e falha integral.
- [Exemplo papacc.conf](papacc.conf): famílias completas documentadas, cinco linhas ativas somente para versão/restrições negativas. Nenhuma credencial, Principal, grant ou epoch real; exemplos funcionais comentados. Predicados futuros descritos sem diretivas fictícias executáveis.
- [Matriz de diretivas](directive-matrix.md): autoridade, função/componente, suporte factual e lacunas; source proposto não equivale a enforcement.
- [Handoff mínimo](implementation-handoff.md): parser/modelo/codec/resolução/transação existentes, comandos administrativos propostos e regressões futuras.
- [Verificação](verification.json), [preimagens](baseline.json) e [hashes](final-manifest.json).

## Decisão focal e limites

A menor direção coerente é uma source legível que normalize o modelo accepted, com coleção explícita e DEFAULT_DENY; não um dump textual do BODY0.5 limitado. A sintaxe é proposta e necessita revisão/aceitação própria antes de código dependente. ADR0009 não precisa ser enfraquecido para caber no modelo singular. Nenhuma troca silenciosa de produção ou criação de listener ocorreu.

Preservados primeira terminal correspondente, condições conjuntivas, ordem declarada, gates independentes e exact destination v1 separado. Sem OR/NOT/herança/sort automático, fonte secundária de autoridade, scripts de apply ou relógios/epochs escolhidos pelo arquivo. SOURCE/PARSE/VALIDATE/RESOLVE/PREPARE/COMMIT/CURRENT/EPOCH são fases distintas. Edição do arquivo não publica configuração. I/R/A/P e pins continuam distintos; UNKNOWN/CAS conflict não concede fallback.

MANUAL/AUTOMATIC, admin autorizado, lifecycle e ED08/ED11 continuam gates accepted; catálogo geral de usuários/grupos/calendários e backup universal não ampliam Phase6. Contabilização incremental de futura implementação permanece pendente: nenhum armazenamento operacional foi adicionado ou novo censo compilado alegado. Provider/kernel UNKNOWN; freshness/headroom PENDING. Admissão permanece negada.

## Preservação e verificações

2183 arquivos preexistentes rastreados/não ignorados preservados byte a byte, incluindo WIP e documentos anteriores. HEAD inalterado. Não foram feitas escritas em fontes funcionais, Browser, PST, SDKs, manifests anteriores ou artefatos históricos. A verificação cobre arquivos enumerados pelo Git sem ignorados; não alega rehash independente de todos os binários/SDKs ignorados. Manifest novo identifica esta entrega, não substitui manifests históricos.

Inspeção documental do exemplo confirmou somente cinco linhas ativas negativas/header. `git diff --check` retornou0, com avisos de conversão LF/CRLF do WIP existente; nenhum arquivo foi convertido. Verificações documentais não são testes de parser (inexistente) ou regressões runtime. Não houve build, testes ou rede. Não houve submissão de patch funcional nem necessidade de escalonamento para a documentação no workspace autorizado.

```ini
CURRENT_TASK_COMPLETED=YES
ADR0009_RECONCILIATION=PROPOSED_REPRESENTATION_IMPLEMENTATION_CONFLICT_REMAINS
CONFIG_FORMAT=PROPOSED
SQUID_STYLE_COMMENTS=HASH_ONLY_OUTSIDE_QUOTES_SEMICOLON_NOT_COMMENT
LISTENER_COLLECTION=EXPLICIT_LOGICAL_COLLECTION_PROPOSED
LEGACY_EXPLICIT_OPT_IN=REQUIRED_NO_ABSENCE_FALLBACK
DEFAULT_DENY=PRESERVED
POLICY_PRECEDENCE=PRESERVED
UNSUPPORTED_DIRECTIVES=REJECT
SAME_ROOT_AUTHORITY=PRESERVED
PHASE6_SCOPE=UNCHANGED
IMPLEMENTATION_AUTHORIZED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
FUNCTIONAL_SOURCE_CHANGED=NO
ADR_ACCEPTED_CREATED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_2183_PREEXISTING_FILES_HASH_UNCHANGED
DIFF_CHECK=PASS_WITH_EXISTING_GIT_LINE_ENDING_WARNINGS
FIRST_UNRESOLVED_DIVERGENCE=ADR0009_COLLECTION_AND_EXPLICIT_PROFILE_VS_SINGULAR_LEGACY_INITIALIZER_AND_OPTIONAL_CLI_PROFILE
NEXT=OWNER_REVIEW_OF_SQUID_INSPIRED_CONFIGURATION_CONTRACT
```
