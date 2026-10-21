---
status: accepted
revision: 1
decision-date: 2026-10-19
scope-level: project
scope-target: PapinhoAccelerator configuration source
supersedes: []
decision-makers: [Tobias Tromm]
---
# Contrato papacc.conf — comentários, ACLs futuras e listeners

## Autoridade e aceitação delimitada

Direção explicitamente aprovada pelo owner em PACC-PHASE6-CONFIG-CONTRACT-RECONCILIATION-2. Este registro somente vigora após a aprovação oficial da operação exata de criação pelo executor e verificação da gravação. Não altera nem substitui ADRs accepted. Aceita as cláusulas N1–N8 abaixo, não todos os detalhes propostos por referência. Documentos anteriores e preimagens permanecem como proveniência. A aprovação normativa não aplica patches funcionais nem qualifica produto.

## N1 — comentários e linhas desativadas

`#` fora de aspas inicia comentário comum integral ou inline; dentro de string é dado. `;` como primeiro caractere não branco desativa toda a linha, inclusive diretivas, campos ou delimitadores de bloco. `;` dentro de string é literal; em outra posição fora de string/comentário é erro lexical. Conteúdo de comentário não é tokenizado. Linhas desativadas não produzem AST, referências, regras, listeners ou autoridade. Preservar localização de origem e ordem física de regras ativas. Remover o marcador somente torna a linha candidata à validação; não aplica configuração. Bloco ativo parcialmente desativado que fique estruturalmente incompleto é rejeitado, sem preenchimento permissivo ou descarte de predicates. Uma regra ativa não pode desativar só um predicate por `;` inline. Mudança estruturalmente válida segue diff, validação e transação normais, sem inferir intenção de texto desativado.

## N2 — ACLs futuras formalmente preservadas

A arquitetura preserva IP de origem/subnet/intervalo/família IPv4/IPv6; interface de chegada/identidade persistente; usuário/grupo/associação/hierarquia; Principal autenticado distinto de usuário; dias/horários/calendários/feriados/timezone/DST; associação de policies a listeners/transportes/capabilities/TLS/Network Egress/provisionamento; composição/precedência e auditoria de regra/identidade/contexto/motivo. Sua semântica detalhada deve ser definida normativamente antes de implementação, sem inventar OR/NOT, herança, especificidade ou prioridade automática.

Classificar explicitamente SUPPORTED (somente no componente/target/enforcement demonstrado), REPRESENTABLE_NOT_IMPLEMENTED (dimensão preservada, execução ausente) e FUTURE_UNSPECIFIED (detalhes sem contrato). Nenhuma classificação concede autoridade. Parser aceitar léxico não prova suporte operacional. Configuração ativa que depende de capacidade sem contrato/enforcement é integralmente rejeitada; não ignorar, aproximar ou converter em wildcard. Não criar diretivas executáveis fictícias. RBAC geral/calendários/ACLs avançadas não se tornam novos gates da Phase6. Autorizações administrativas já exigidas continuam obrigatórias.

## N3 — listeners e ADR-0009

O modelo canônico é listeners[], mesmo quando o target inicialmente suporta apenas um. Cada listener tem identidade administrativa estável no escopo da root, binding, porta, perfil explícito, referências de segurança aplicáveis e contexto de admissão/policy. Identidade administrativa não substitui Principal, Session, socket ou epoch. Múltiplos sockets derivados de um binding não são múltiplos listeners lógicos. Perfil ausente não se converte em Legacy. Legacy exige opt-in administrativo explícito; coleção acima da capacidade é rejeitada sem truncamento. Nenhuma migração silenciosa da produção0.6.4 ou alteração de perfil em lifetime. ADR0009 permanece intacto; o modelo singular atual exige reconciliação funcional própria.

## N4 — decisões e dimensões independentes

Preservar primeira regra terminal correspondente, conjunção das condições, ordem configurada e DEFAULT_DENY. ACCEPT não elimina gates independentes de autenticação/admissão/Principal/snapshot/grants/CURRENT/epoch/destination/TLS/recursos/lifecycle. Não importar essa linguagem para exact destination v1 nem tornar OPEN ausência de TLS/mTLS. Canal, SiteTLS e Network Egress são distintos, sob as autoridades existentes. Sem fallback permissivo.

## N5 — intenção, root e transação

Arquivo é SOURCE/intenção: PARSE → VALIDATE → RESOLVE → PREPARE → COMMIT factual → CURRENT → EPOCH conforme contratos accepted. Arquivo editado não é estado aplicado; não editar BODY/ROOT/serial/high-water diretamente. Uma única root, sem registry/autoridade paralela. Preservar identidades distintas I/R/A/P, CAS, conteúdo/versionamento/proveniência, snapshots/pins/leases, Model E, ED08/ED11, Stop/quiescência/retenção. Pin antigo protege lifetime, não nova autoridade CURRENT. Resultado incerto não permite fallback ou reutilização. Recuperação sem checkpoint/fencing comprovados permanece DENY.

## N6 — versão, portabilidade e falha fechada

Formato tem versão explícita, diretivas desconhecidas rejeitadas, diagnósticos de localização/motivo e presença explícita. Target que não implementa requisito ativo rejeita antes de efeitos. Núcleo/parser/AST/validação/codec independentes de Win32/CNG; adaptadores não oferecem fallback silencioso. Não congelar limites da primeira implementação como arquitetura definitiva. Detalhes lexicais restantes, layout/integridade do codec, bounds e deltas de armazenamento ainda propostos exigem revisão específica do patch antes de aplicação.

## N7 — próximo slice e autorização separada

Preparar parser limitado portável, AST imutável, validação semântica/suporte por target, coleção normalizada e codec privado versionado, com regressões Debug/Release, negativas, portabilidade e censo ED11. Submeter alterações funcionais exatas oficialmente antes de aplicar. Este registro não implementa config apply produtivo, não publica CURRENT/EPOCH e não aceita patches por referência. Quotas não aumentam; armazenamento/coexistência/retenção contabilizados sem duplicação. Provider/kernel UNKNOWN e parâmetros pendentes não se tornam PASS.

## N8 — limites

CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY. Nenhuma CA/credencial/certificado real, rede, alteração Browser/PST/PACC wire, commit/push/tag/release. Production pin0.6.4 preservado; PST0.6.5 candidato até encerramento integral Phase6. WIP e evidências preservados. Não implementar novos protocolos, TLS interception, firewall/NAT, backup universal, hardware ou calendário/RBAC geral para preencher exemplos.

## Precedência e referências

ADR0009 Decisão/regras1–12, ADR0007/0010/0012/0026/0027/0028; ADR0043/0044/0048 históricos somente nos escopos aplicáveis preservados pela consolidação canônica; decisões accepted SiteTLS, provisionamento, issuer/C2, C8, Model E, ED08/ED11 permanecem integrais. N1 reconcilia a escolha documental anterior de rejeitar `;` inicial; isso não altera nenhum parser implementado ou ADR accepted. N2 não cria nova semântica runtime de ACL. N3 reafirma o modelo accepted, sem dispensar sua implementação. A gramática proposta anterior e o handoff não são aceitos integralmente por referência. A matriz de revisão em docs/integration/config-contract-reconciliation-2-20261019/exact-review.md delimita esta aceitação.
