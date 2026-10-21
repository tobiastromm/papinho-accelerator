# PROPOSED — diretiva → autoridade → enforcement → suporte

Toda sintaxe nesta pasta é PROPOSED; nenhum parser existe. B significa componente factual existente, C obrigação/integração pendente; não significa que a diretiva já pode ser usada. Aliases/cláusulas e caminhos completos: [authority-aliases](../configuration-policy-completeness-audit-20261019/authority-aliases.json), [enforcement-map](../configuration-policy-completeness-audit-20261019/enforcement-map.md). Este catálogo é fechado; campos ausentes da tabela são desconhecidos. Blocos têm id e `end`; repeatability conforme gramática.

| Bloco / diretivas e valores | Autoridade | Enforcement factual / suporte e condição de aplicação futura |
|---|---|---|
| global: format_version 1 | PE4 + proposta lexical | Parser ausente; versão desconhecida rejeita, sem autodetect BODY |
| client_admission DISABLED; site_tls_admission DENY; ed11_admission DENY | SITE, VALID, C2§11.3, ED11 | server_admission_preflight_win32.inc; restrições negativas, nunca overrides de aprovação |
| network_egress DISABLED | A19/A26 | master gate e network_authorization.c; nenhum ENABLED proposto neste slice |
| expected_root_ref string; expected_current_revision u64 | A44 D1-3, IR P3/P6 | root transaction/pending CAS; precondições factuais, não publicação/geração |
| binding: mode ALL ou SELECTED; interface_ref string repetível apenas SELECTED | A07/A09 | persistent_bind_selection.c, bind_target.c; SELECTED exige pelo menos uma ref, ALL nenhuma; ausência/ambiguidade nega; B |
| listener: binding_ref id; port u16 nãozero; transport SECURE_PRINCIPAL ou LEGACY_ENDPOINT; channel_tls_ref id (só Secure, obrigatório); admission OPEN ou CLOSED; authentication_ref id; policy_ref id | A09 Decisão/regras1-12, A28/A48 | config/CLI singulares em conflito; coleção/associações exigem reconciliação/codec antes de aplicar; refs obrigatórias; Legacy não ganha AuthN/egress por esta proposta; C |
| access_auth: method NONE ou CERTIFICATE_IDENTITY; provider_ref string | A10/A28 | authentication_authorization.c, lab source limitada; NONE só compatível com OPEN e nunca remove canal mTLS; CERTIFICATE exige provider; CLOSED não aceita NONE; outras modalidades representáveis ADR28 mas sem sintaxe executável aqui; B/C |
| capability_policy: enable key major; rule id ACCEPT ou DENY key major [principal hex16] | A12/A43/A48H/A51 | server_policy_model.h, policy_validate/evaluate, C2 root binding; keys suportadas registry, no-match DENY, Principal exato16; quatro regras/três keys são limite atual, não arquitetura; cobertura todos paths ainda C |
| destination_policy: kind EXACT_ALLOWLIST; hostname string port; endpoint string port | A20/A21/A26 | destination_resolve_and_filter, egress_endpoint_allowed; apenas exact positivos, normalização/deduplicação, DNS AND endpoint; sem regras DENY/CIDR; core B, adapter DNS produtivo C |
| egress_profile: profile_ref string; destination_policy_ref id | A19/A24/A26 | egress_runtime_resolve, server_egress_source_win32.c só default-route; ref resolve catálogo/rota/interface/source, nunca herda bind; vínculo durável dos snapshots/source ainda C |
| channel_tls: security_ref string | A06/A10 | server security resolver + leases; TLS1.3/mTLS/papacc1 fixos, source lab não cadastro produtivo; B/C |
| outbound_tls: profile_ref string | A22/A27 | server_outbound_tls_source_win32.c; conteúdo TLS/ALPN/trust/nome resolvido e versionado; sem client cert/0RTT/resumption/fallback v1; B/C |
| site_tls: catalog_ref string; catalog_revision u64 nãozero; profile_ref u32 (0 só default explicitamente cadastrado); profile_revision u64 nãozero | SITE + C2§7 | server_site_tls_profile.c, root binding/dispatch; ref→BODY05 completo, SYSTEM inicial; refs/revisões exatas, policy/P/Principal/grants/UTC/limites obrigatórios; B/C |
| provisioning: issuer_ref id; manual REQUIRED; automatic REQUIRED; enrollment OPEN ou CLOSED; approval AUTO_APPROVAL ou MANUAL_APPROVAL; authentication_ref string quando CLOSED | PROV1-8, CA | Contrato accepted, frontend completo ausente; rejeitar ativação antes de fluxo autorizado/verificado. Não altera modos gerais de acesso; C |
| issuer: authority_ref string | CA/CAT/IR | portable issuer/root persist/live/checkpoint: B parcial; ref existente não inicializa CA ou autoriza assinatura; CNG/recovery/generation real gates |
| resources: profile_ref string; evidence_ref string | ED11/C2§11.3/VALID | preflight/resource owner/local ledgers; conteúdo governado e target vinculado, sem quota inventada ou UNKNOWN→PASS; B/C |
| audit: sink_ref string | PROV/CAT, A43, Model E | logger durável existente; audit efetiva/ator/regra/contexto ainda incompleta; rejeitar suporte alegado sem produtor necessário; B/C |
| end | proposta lexical | fecha exatamente um bloco, zero argumentos; nenhuma herança |

Bindings de SiteTLS/outbound/egress/provisioning são refs a componentes resolvidos, não um registry paralelo. Seleção por operação/perfil reutiliza configuração normativa existente; associação arbitrária por Principal/grupo/hostname não nasce da simples declaração do bloco. Quando não houver ligação durável e enforcement para uma ref, RESOLVE/PREPARE rejeita: bloco órfão não pode ser anunciado como aplicado. Relatório effective deve distinguir referência resolvida de recurso não instalado.

## Famílias deliberadamente sem diretivas

Source IP/subnet/range/arrival-interface, usuários/grupos/RBAC/nesting, horários/calendários/feriados/DST: A28/FUT preservam representabilidade, A48E exige rejeitar sem suporte. Não definir pseudo `acl`, `group` ou `schedule`. Principal não equivale a usuário ou grupo; janela UTC de certificado não é calendário civil. Rotas por interface existem como intenção do profile mas adapter atual não comprova execução; não expor comandos de rota fictícios. PUBLIC_INTERNET não é implementado com classificador parcial. Sem novos protocolos, TLS interception, firewall/NAT, backup/migração universal.

## Cobertura do modelo auditado

R001–012 interfaces/binding/listener; R013–024 source-context com Principal parcial e restantes futuros; R025–035 admissão/AuthN/provisionamento; R036–054 egress/destino/profile/recursos/audit; R055–070 TLS separado; R071–084 e R162 sujeitos/admin/grants com RBAC geral futuro; R085–099 tempo; R100–124 composição/precedência; R125–133 enforcement; R134–151 persistência; R152–161 administração; R163–167 portabilidade permanecem rastreados na matriz integral de 167 requisitos. Esta proposta não reclassifica nenhum requisito como implementado. Autoridade administrativa e isolamento do operador CA continuam obrigatórios mesmo sem RBAC geral.
