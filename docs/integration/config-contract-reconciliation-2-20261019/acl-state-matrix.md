# ACLs — arquitetura preservada e suporte factual

Estados não são níveis de autorização. SUPPORTED significa somente a semântica/componente explicitamente delimitado na coluna de evidência. Nenhuma diretiva papacc.conf está implementada. REPRESENTABLE_NOT_IMPLEMENTED preserva dimensão arquitetural accepted/direção aprovada sem enforcement completo; FUTURE_UNSPECIFIED preserva requisito cujo contrato detalhado não está definido. Em ambos, dependência ativa deve ser rejeitada integralmente, nunca ignorada/aproximada. Os três estados não criam novas obrigações Phase6.

| Dimensão | Estado | Fonte, fronteira e lacuna |
|---|---|---|
| Principal autenticado exato16 | SUPPORTED (componente C2 limitado) | ADR0010/0051; c8_authenticated_control e c2_capability_policy; não usuário/grupo; source lab não cadastro produtivo |
| Ordem física, conjunção, primeira terminal/default deny | SUPPORTED (avaliador bounded) | ADR0043/0048H, server_policy_evaluate; não cobre todos paths ModeB; sem OR/NOT/herança |
| IP origem, subnet, intervalo, família IPv4/IPv6 | REPRESENTABLE_NOT_IMPLEMENTED | ADR0028/FUT; endereço socket não é predicate operacional da tabela |
| Interface chegada/identidade persistente como condição | REPRESENTABLE_NOT_IMPLEMENTED | ADR0007/0028; binding persistente implementado não é arriving-interface AuthZ |
| Usuário, grupo e associação | REPRESENTABLE_NOT_IMPLEMENTED | ADR0028/direção owner; nenhum resolver geral qualificado; Principal não se converte em usuário |
| Hierarquia/nesting e resolução de conflitos | FUTURE_UNSPECIFIED | Preservar requisito; sem herança escolhida ou algoritmo de precedência implícito |
| Dias e janelas de horário | REPRESENTABLE_NOT_IMPLEMENTED | ADR0028/FUT; monotonic deadline/UTC de certificado não implementa agenda civil |
| Calendários, feriados, timezone, DST, intervalos cruzados | FUTURE_UNSPECIFIED | Requisito preservado; decidir semântica/ambiguidade antes de código |
| Associação policy listener/transporte/capability | SUPPORTED (subset do modelo) | ADR0009/0048; listener collection/codec e source ainda pendentes; não anunciar coleção implementada |
| Associação Source ACL a TLS/Network Egress/provisionamento | REPRESENTABLE_NOT_IMPLEMENTED | ADR0028/PROV; gates independentes existentes não criam selector universal |
| Novas combinações/precedência de futuras condições | FUTURE_UNSPECIFIED | Semântica normativa deve anteceder implementação, sem estender operadores atuais por inferência |
| Auditoria de regra, identidade, contexto e motivo | REPRESENTABLE_NOT_IMPLEMENTED (integral) | logger existente, matched index parcial; trilha administrativa completa ausente |

Bindings, destination exact e SiteTLS conservam seus contratos separados. Não inserir ACL fictícia no arquivo para satisfazer a matriz. Revogação ativa de certificado continua POLICY_PENDING. IPs/grupos/tempo não se tornam novos gates obrigatórios da Phase6. Autoridade administrativa real e isolamento do operador permanecem gates existentes.
