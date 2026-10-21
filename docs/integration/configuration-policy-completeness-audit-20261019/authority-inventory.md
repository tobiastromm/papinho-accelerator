# Inventario de autoridades e aplicabilidade

HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Inventario documental completo dos diretorios ADR organizacional/familia/canonico/historico escolhido. Status nao equivale a aplicabilidade, implementacao ou qualificacao. Catalogacao nao afirma releitura integral de todo assunto alheio ao modelo de configuracao. As autoridades usadas nas matrizes possuem clausulas abaixo e localizacao/hash verificaveis.

## Autoridades aplicadas

| Alias | Documento | Clausula recuperada |
|---|---|---|
| A07 | docs/adr/ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md | Decision; identity separation and resolution |
| A09 | docs/adr/ADR-0009-perfil-de-transporte-explicito-por-listener.md | Decisao; Regras derivadas1-12 |
| A10 | docs/adr/ADR-0010-resolucao-de-configuracao-de-seguranca-do-servidor.md | Decisao; Regras derivadas1-10 |
| A12 | docs/adr/ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md | Decisao; Regras derivadas |
| A19 | docs/adr/ADR-0019-policy-de-egress-e-egress-profiles-server-owned.md | Decisao |
| A20 | docs/adr/ADR-0020-normalizacao-dns-e-binding-de-endpoint-validado.md | Decisao |
| A21 | docs/adr/ADR-0021-prevencao-de-abuso-e-destinos-sensiveis-de-egress.md | Decisao |
| A26 | docs/adr/ADR-0026-referencias-de-policy-profile-e-regras-exatas-de-egress.md | Decisao |
| A27 | docs/adr/ADR-0027-referencia-e-resolucao-de-outbound-tls-security-profile.md | Decisao |
| A28 | docs/adr/ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md | Decisao; Consequencias e implementacao futura |
| A43 | build/phase6-c2-c3-20261012/accelerator/docs/adr/ADR-0043-existing-policy-authority-and-ordered-decisions.md | Decisoes aprovadas; Limites (original isolated scope) |
| A44 | build/phase6-c2-c3-20261012/accelerator/docs/adr/ADR-0044-atomic-persistent-effective-configuration-and-c2-epoch.md | D1/D2/D3; Ownership/resources (original isolated scope) |
| A48 | build/phase6-c2-c3-20261012/accelerator/docs/adr/ADR-0048-minimal-administrative-policy-model-and-versioned-private-body.md | Approved decisions A-H; lifecycle (original isolated scope) |
| A51 | build/phase6-c2-c3-20261012/accelerator/docs/adr/ADR-0051-private-authenticated-principal-representation.md | Exact16-byte Principal; historical-version separation |
| PROV | docs/adr/DECISION-production-client-provisioning-20261018.md | Decisao1-8; Gates; Escolhas nao aprovadas |
| CA | docs/adr/DECISION-internal-client-issuer-and-manual-custody-20261018.md | Accepted decision; Delimited precedence |
| CAT | docs/adr/DECISION-internal-ca-technical-profile-20261018.md | X5091.1-1.6; serial2.1-2.5; key/backup3.1-3.7 |
| SITE | docs/adr/DECISION-mode-c-site-tls-profile-authority-20261018.md | Decisao e conformidade; Implementacao privada e limites |
| IR | docs/adr/DECISION-issuer-c2-epoch-authority-relation-20261019.md | P2/P3/P4/P5/P6/P7 |
| VALID | docs/adr/DECISION-mode-c-evidence-validity-semantics-20261018.md | Accepted rules1-6; Limits |
| C2 | docs/protocol/pacc-c2-specification-revision-1.0.md | sections4/6/7/11.2/11.3; authority ADR0034 |
| FUT | docs/future/network-egress-policy.md | Future decisions; preserved requirements3-13; not runtime authority |
| CON | docs/adr/DECISION-phase6-canonical-consolidation-20261018.md | clauses1-10; Compatibility |
| PE4 | C:/Projetos/PapinhoEngineering/docs/adr/ADR-0004-modelo-compartilhado-de-configuracao-e-separacao-core-frontends.md | Decisao; Configuration Model/Source/Storage |
| PORT | docs/adr/ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md | Decisao; portability boundaries |
| MODELE | C:/Projetos/PapinhoSecureTransport/docs/model-e-consumer-lifecycle-contract.md | Factual dispositions; universal invariants |

## Inventario ADR

| Origem | Status documental | Documento | Aplicabilidade |
|---|---|---|---|
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0001-modelo-de-composicao-e-aplicabilidade-da-governanca-de-engenharia-sem-cliente-nomeado.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0002-adocao-e-governanca-de-architecture-decision-records.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0003-taxonomia-de-documentacao-de-engenharia-e-direcoes-futuras.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0004-identidade-canonica-e-nomenclatura-de-produtos-projetos-e-artefatos.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0005-governanca-viva-e-revisao-de-validade-de-decisoes.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0006-workspace-temporario-externo-e-segregacao-de-artefatos-sensiveis.md | Current applicable accepted clauses only |
| ORGANIZATION | accepted | C:\Projetos\OrganizationEngineering\docs\adr\ADR-0007-template-canonico-de-bootstrap-de-projetos-da-organizacao.md | Current applicable accepted clauses only |
| FAMILY | superseded | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0001-adocao-e-governanca-de-adrs.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0002-identificacao-factual-e-duravel-de-build-targets.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0003-contrato-estruturado-e-desacoplado-de-logging.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0004-modelo-compartilhado-de-configuracao-e-separacao-core-frontends.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0005-governanca-da-documentacao-viva-de-capabilities.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0006-carregamento-obrigatorio-de-contexto-arquitetural-e-governanca-de-agentes.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0007-fronteiras-portaveis-entre-core-plataforma-e-backends-substituiveis.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0008-localizacao-e-internacionalizacao-desde-a-origem.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0009-separacao-entre-identidade-local-autenticacao-confianca-e-nome-do-peer.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0010-fronteira-de-adaptacao-de-providers-e-honestidade-de-capabilities.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0011-fronteiras-de-reutilizacao-e-extracao-de-componentes-compartilhados.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0012-template-canonico-de-bootstrap-de-projetos-papinho.md | Current applicable accepted clauses only |
| FAMILY | accepted | C:\Projetos\PapinhoEngineering\docs\adr\ADR-0013-semantica-terminal-de-truncamento-tls-apos-entrega-de-dados.md | Current applicable accepted clauses only |
| FAMILY | proposed | C:\Projetos\PapinhoEngineering\docs\adr\ADR-TEMPLATE.md | Current applicable accepted clauses only |
| FAMILY | concept | C:\Projetos\PapinhoEngineering\docs\adr\CAPABILITY-TEMPLATE.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0001-secure-principal-e-legacy-endpoint.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0003-connection-session-channel-ownership-e-lifecycle.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0008-fixacao-de-dependencias-papinho-por-release.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0009-perfil-de-transporte-explicito-por-listener.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0010-resolucao-de-configuracao-de-seguranca-do-servidor.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0011-identidade-de-capability-e-modelo-portatil-limitado.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0013-lifecycle-de-job-ownership-de-session-e-cancelamento-cooperativo.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0014-contrato-portatil-incremental-de-compute-backend.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0015-establishment-jobs-e-lifecycle-de-network-stream.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0016-negociacao-inicial-de-capabilities-no-wire.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0017-protocolo-wire-de-network-offload-em-data-channel-dedicado.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0018-registry-de-capabilities-network-egress-e-tls-offload.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0019-policy-de-egress-e-egress-profiles-server-owned.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0020-normalizacao-dns-e-binding-de-endpoint-validado.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0021-prevencao-de-abuso-e-destinos-sensiveis-de-egress.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0022-perfil-tls13-client-outbound-via-pst.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0023-tls-required-v1-nao-suporta-network-half-close-direcional.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0024-boundary-concreta-de-policy-e-egress-profile.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0025-contrato-publico-portavel-papinhopacc.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0026-referencias-de-policy-profile-e-regras-exatas-de-egress.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0027-referencia-e-resolucao-de-outbound-tls-security-profile.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0029-contrato-mode-c-c2-e-autorizacao-laboratorial.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0030-stop-global-phase-aware-para-teardown-outbound-pst.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0031-secure-candidate-protocol-lifetime-reconciliation.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0032-result-returning-pre-close-boundary.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md | Current applicable accepted clauses only |
| CANONICAL | proposed | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-CANDIDATE-ed11-total-resource-admission.md | Current applicable accepted clauses only |
| CANONICAL | proposed | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-CANDIDATE-pacc-c2-ed08-record-end-and-ed11-admission.md | Current applicable accepted clauses only |
| CANONICAL | proposed | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-CANDIDATE-pacc-c2-ed11-measured-admission-refinement.md | Current applicable accepted clauses only |
| CANONICAL | proposed | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-CANDIDATE-pacc-c2-exact-encoding-and-security.md | Current applicable accepted clauses only |
| CANONICAL | proposed | C:\Projetos\PapinhoAccelerator\docs\adr\ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-internal-ca-technical-profile-20261018.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-internal-client-issuer-and-manual-custody-20261018.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-issuer-c2-epoch-authority-relation-20261019.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-issuer-live-session-coexistence-20261019.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-mode-c-evidence-validity-semantics-20261018.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-mode-c-site-tls-profile-authority-20261018.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-pacc-c2-conditional-semantic-approval-20261012.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-phase6-canonical-consolidation-20261018.md | Current applicable accepted clauses only |
| CANONICAL | accepted | C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-production-client-provisioning-20261018.md | Current applicable accepted clauses only |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0001-secure-principal-e-legacy-endpoint.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0003-connection-session-channel-ownership-e-lifecycle.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0008-fixacao-de-dependencias-papinho-por-release.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0009-perfil-de-transporte-explicito-por-listener.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0010-resolucao-de-configuracao-de-seguranca-do-servidor.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0011-identidade-de-capability-e-modelo-portatil-limitado.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0013-lifecycle-de-job-ownership-de-session-e-cancelamento-cooperativo.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0014-contrato-portatil-incremental-de-compute-backend.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0015-establishment-jobs-e-lifecycle-de-network-stream.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0016-negociacao-inicial-de-capabilities-no-wire.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0017-protocolo-wire-de-network-offload-em-data-channel-dedicado.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0018-registry-de-capabilities-network-egress-e-tls-offload.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0019-policy-de-egress-e-egress-profiles-server-owned.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0020-normalizacao-dns-e-binding-de-endpoint-validado.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0021-prevencao-de-abuso-e-destinos-sensiveis-de-egress.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0022-perfil-tls13-client-outbound-via-pst.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0023-tls-required-v1-nao-suporta-network-half-close-direcional.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0024-boundary-concreta-de-policy-e-egress-profile.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0025-contrato-publico-portavel-papinhopacc.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0026-referencias-de-policy-profile-e-regras-exatas-de-egress.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0027-referencia-e-resolucao-de-outbound-tls-security-profile.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0029-contrato-mode-c-c2-e-autorizacao-laboratorial.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0030-stop-global-phase-aware-para-teardown-outbound-pst.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0031-secure-candidate-protocol-lifetime-reconciliation.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0032-result-returning-pre-close-boundary.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0033-model-e-consumer-close-completion.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0035-governed-isolated-c2-c3-implementation.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0036-isolated-empirical-diagnostic-admission.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0037-separate-completed-child-scheduling.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0038-private-e10-progress-intervals.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0039-finite-e10-capture-and-post-quiescence-streaming.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0040-versioned-e10-composite-evidence.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0041-isolated-c8-implementation-authority.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0042-durable-launcher-retained-dependencies.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0043-existing-policy-authority-and-ordered-decisions.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0044-atomic-persistent-effective-configuration-and-c2-epoch.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0045-single-server-local-configuration-implementation.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0046-normal-local-store-custody-and-offline-selection.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0047-isolated-real-owner-normal-recovery-integration.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0048-minimal-administrative-policy-model-and-versioned-private-body.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0049-isolated-closed-admission-attachment-and-pst-internal-wake.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0050-explicit-bootstrap-and-offline-admission-preflight.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0051-private-authenticated-principal-representation.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0052-scoped-c8-supervised-diagnostic-uncertainty.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-0053-private-two-stage-evidence-association.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | NO_EXPLICIT_FRONTMATTER_STATUS | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-browser-renderer-resource-accounting.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-c2-completed-child-readiness-suspension.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-c2-diagnostic-target-admission.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-c8-independent-site-tls-authorization-source.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-c8-minimal-administrative-policy-profile.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-e10-finite-capture-and-post-quiescence-streaming.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-e10-scoped-post-collection-completeness.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-ed11-total-resource-admission.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | NO_EXPLICIT_FRONTMATTER_STATUS | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-effective-configuration-recovery-authority-profile-revision-2.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | NO_EXPLICIT_FRONTMATTER_STATUS | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-effective-configuration-recovery-authority-profile.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-effective-configuration-revision-and-c2-epoch.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-pacc-c2-ed08-record-end-and-ed11-admission.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-pacc-c2-ed11-measured-admission-refinement.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-pacc-c2-exact-encoding-and-security.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | proposed | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-e10-minimal-s1-private-schema-20261018.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-e10-scoped-post-collection-completeness-20261017.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-e10-v5-no-operation-denial-erratum-20261018.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-hybrid-s1-factual-observation-20261017.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-pacc-c2-conditional-semantic-approval-20261012.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |
| HISTORICAL_ISOLATED | accepted | C:\Projetos\PapinhoAccelerator\build\phase6-c2-c3-20261012\accelerator\docs\adr\DECISION-phase6-c7-governed-eligible-execution-20261013.md | Original isolated scope; only explicit canonical references/consolidation, no automatic qualification |

## Principios e historico

Principios de autoridade unica, ortogonalidade, fail-closed, identidade factual, bounded work e ownership sao recuperados dos ADRs0001/0002/0003/0004/0005/0006/0007/0009/0010/0012 e das decisoes posteriores. Nenhum PRINCIPLES.md/PROJECT-CONTEXT.md canonico foi localizado na descoberta; isso nao elimina os principios dos ADRs.

ADR0043/0044/0048/0051 continuam em docs/adr da referencia isolada, accepted no escopo original. A consolidacao e as decisoes canonicas Site TLS/issuer referenciam esses contratos; preservamos suas restricoes no componente reconciliado, sem declarar toda a arvore historica produtiva ou promover ADR0052 CONTROL para ModeC.

docs/architecture.md, docs/networking.md e trechos antigos de docs/future/network-egress-policy.md misturam marcos Phase3/4/ModeB com o estado posterior. Sao HISTORICAL quando divergem de source/decisoes recentes; READY antigo nao fecha provisioning/ModeC/ED11. Nenhum documento historico foi apagado ou corrigido nesta auditoria.

Browser e PST foram consultados somente como referencia: Browser importer/store/lease e ADR0007 de provisioning; PST Model E. Browser AGENTS.md no caminho canonico nao foi localizado. Nenhuma escrita ou comando Git de confianca foi feito nessas arvores.

## Aliases adicionais usados nas matrizes

| Alias | Documento | Clausula |
|---|---|---|
| A01 | docs/adr/ADR-0001-secure-principal-e-legacy-endpoint.md | Decisao; endpoint identity and no downgrade |
| A02 | docs/adr/ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md | Decisao; deployments/backends |
| A03 | docs/adr/ADR-0003-connection-session-channel-ownership-e-lifecycle.md | Ownership and lifecycle |
| A04 | docs/adr/ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md | Bounded scheduling; monotonic deadlines |
| A05 | docs/adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md | Decisao; support/enabled/authorized/requested/effective |
| A06 | docs/adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md | Transport Security; Credential/Principal; enrollment; scoped CA/custody supplements |
| A22 | docs/adr/ADR-0022-perfil-tls13-client-outbound-via-pst.md | Decisao: ModeB TLS13 and separate placements |
| A24 | docs/adr/ADR-0024-boundary-concreta-de-policy-e-egress-profile.md | Decisao: injected policy/profile and no implicit default route |
