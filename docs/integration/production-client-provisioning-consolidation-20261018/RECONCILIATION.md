# Reconciliação factual — Production Client Provisioning

Data: 2026-10-18. Accelerator canônico; Browser somente leitura.
Nenhum código implementado, build/teste/rede/runtime/PKI executado.

## Baseline e referências exatas

Accelerator main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a;
Browser main/1eb028554f622672377953225e162ed9b6564a40. Ambos já tinham WIP extenso.
[BASELINE.json](BASELINE.json): status integral, hashes dos arquivos versionados/
não ignorados existentes e hash do diff HEAD de cada árvore. SDKs/artefatos
ignorados não são automaticamente incluídos; manifests e evidências selecionados
constam de AUTHORITIES-SHA256.json. Preimagens byte-exact dos três documentos
vivos em preimages.zip. Nenhum SDK/build foi alterado ou executado.

| Ref | Autoridade/evidência exata | Uso e limite |
|---|---|---|
| A1 | [Accelerator ADR-0006](../../adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md), Pairing e first-use trust / Cert lifecycle / Logging e secrets | TLS1.3/mTLS, chave nos endpoints, credential != Principal; código contextual single-use/short-lived/rate-limited; renewal/revoke; sem secrets. |
| A2 | [Accelerator ADR-0028](../../adr/ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md), Admission Mode / Authentication / Combinações canônicas | OPEN/CLOSED independente de TLS/AuthN/AuthZ; CLOSED geral admite certificado e/ou password. |
| A3 | [Accelerator ADR-0012](../../adr/ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md), Decisão | Snapshot imutável; policy pode negar/revogar, nunca acrescentar capability silenciosamente. |
| A4 | [Browser ADR-0007](../../../../PapinhoBrowser/docs/adr/ADR-0007-integracao-policy-e-ux-do-papinhoaccelerator-no-papinhobrowser.md), Production Secure Provisioning: two official modes (2026-10-07), linhas 1885–1933 da baseline | Dois modos permanentes/convergentes, pacote não congelado, blocker de final handoff. Accelerator ADR-0007 governa bind de interfaces, não este contrato Browser. |
| A5 | [Browser roadmap](../../../../PapinhoBrowser/docs/architecture-roadmap.md#production-secure-provisioning), linhas 18–71 | Granular/R1 current-host fechado; pacote/export/enrollment/bootstrap/re-pair/unpair/revoke abertos. |
| A6 | [Browser ADR-0007](../../../../PapinhoBrowser/docs/adr/ADR-0007-integracao-policy-e-ux-do-papinhoaccelerator-no-papinhobrowser.md), Minimum Secure Provisioning R1 closeout (2026-10-07), linhas 1807–1883; build/provisioning-closeout/REPORT.txt | Import DER/Base64/PKCS8 não cifrado, validação/commit/leases anteriores; não issuer/pacote/enrollment/revoke servidor ou NT4. Relato preservado, não rerun. |
| A7 | [Proposta/handoff Browser 07/10](../../../../PapinhoBrowser/build/diagnostics/consolidated-intermediate-handoff-proposal-20261007.md#8-permanent-provisioning-paths-and-revocation), seção 8, linhas 187–212 | CSR/public-key preferido, bounded package/import, remote revoke separado; proposta não congela formato, issuer ou parâmetro. |
| A8 | [Factual handoff 07/10](../../../../PapinhoBrowser/build/diagnostics/mode-c-c2-browser-factual-handoff-20261007.md), Baseline; [pre-handoff audit](../../../../PapinhoBrowser/build/diagnostics/pre-handoff-readiness-audit-20261007.md), Executive finding | Histórico source-established; não importa cleanup antigo nem nega integrações canônicas posteriores. |
| A9 | [Decisão canônica 18/10](../../adr/DECISION-phase6-canonical-consolidation-20261018.md) | Mesma root/listener, Model E/ED08/ED11, pin064 e publicação065 diferida; preservar WIP/SDK. |
| A10 | [Provisioning handoff 18/10](../mode-c-administrative-tls-input-binding-20261018/provisioning-handoff.md), Exact configuration and storage interfaces | BODY0.5/root/source/Browser APIs; materiais/input bindings não aprovados; primeiro commit apenas; fonte lab não issuer de produção. |
| A11 | [Future policy](../../future/network-egress-policy.md), Estado implementado / Governance classification | Ortogonalidade e egress separados; não autoriza enrollment, outra CA ou emissão. |
| A12 | [Decisão explícita atual](../../adr/DECISION-production-client-provisioning-20261018.md) / OWNER-REQUEST.txt | Seis gates antes do fechamento integral, password no enrollment CLOSED, aprovação auto/manual e hot update; escolhas concretas não inferidas. |

## Matriz por requisito

EXISTING = contrato/mecanismo encontrado, apenas no escopo descrito.
PARTIAL = parte reutilizável existe; produto integral não implementado/qualificado.
MISSING = implementação produtiva correspondente não localizada nos caminhos
canônicos inspecionados. Contrato não equivale a código ou qualificação.
Todos os seis gates de entrega permanecem OPEN.

| Requisito | Contrato | Implementação | Evidência / lacuna |
|---|---|---|---|
| Dois modos permanentes antes do fechamento Phase6 | EXISTING A4/A12 | PARTIAL | R1 granular existe; pacote e automático faltam. |
| Importação granular avançada | EXISTING A4/A6 | EXISTING R1/current-host | Browser src/papinho_credential_options.inc: papinho_security_dialog_proc -> material_import -> store_prepare; Apply/OK/Cancel preservados. NT4 não qualificado. |
| Admin Export new client | EXISTING obrigação A4/A12 | MISSING | Não há gerador/issuer/cadastro produtivo identificado nos apps/server, src/include inspecionados. Fonte lab não substitui. |
| Importação de pacote único | EXISTING obrigação A4/A12 | MISSING | Browser importer lê três materiais; reutilizar validator/store/generation, não anunciar parser de container existente. |
| Versão/servidor/destinatário/integridade/bounds | PARTIAL: invariantes A12; formato aberto | MISSING pacote | Aprovar encoding/autenticidade/limites/replay/duplicates/secret protection antes de serializer/parser. Não escolher ZIP/JSON/PEM/PKCS12. |
| Chave privada local, CSR/PoP | EXISTING custódia A1; PARTIAL fluxo concreto | PARTIAL | R1 importa chave fornecida pelo usuário; não gera CSR nem autoriza exportar chave de cliente no servidor. Issuer/algoritmo/CSR/PoP concretos pendentes. |
| Address -> identidade verificável -> código | EXISTING obrigação A1/A12; PARTIAL cerimônia | MISSING automático | Endpoint/código não autentica servidor; bootstrap/transcript/apresentação não congelados. |
| OPEN sem login inicial | EXISTING A2/A12 | MISSING enrollment | Não dispensa TLS/confirmação/policy, não concede uso operacional. |
| CLOSED password antes de approval | EXISTING A12, escopo enrollment | MISSING enrollment | CLOSED geral A2 continua representável por certificado. TLS obrigatório para password; sem armazenamento plaintext. |
| AUTO_APPROVAL/MANUAL_APPROVAL | EXISTING A12 | MISSING | Ambos validam tudo; manual request-bound; admin frontend não identificado. |
| Expiração/single-use/limites/anti-replay | EXISTING invariantes A1/A12 | MISSING enrollment | Request, clocks, replay state e valores não aprovados. Nenhum default inventado. |
| Pending sem CONTROL/DATA/egress | EXISTING A1/A12 | PARTIAL | Gates deny e AuthN/policy existentes; bootstrap/approval/issuer não implementados. Não usar Session operacional como atalho. |
| Mesmo cadastro e issuer | EXISTING A12 | MISSING produto | server_security_source_win32.h/.c é lab, single leaf digest; não registro multi-cliente, emissor ou admin de produção. Não criar nova CA. |
| Principal/policy/grants separados | EXISTING A1/A2/A3 | PARTIAL | server_policy_model.h e C2/root têm Principal16/policy; enrollment/revoke/approval pendentes. Não converter fingerprint em Principal. |
| CredentialGeneration/Reference/store | EXISTING A4/A6 | EXISTING Browser R1 | papinho_credential_store.h/.c e papinho_accelerator_config_generation.inc, metadados/ActiveConfiguration. Frontends novos pendentes. |
| Leases/retirement/Model E | EXISTING A6/A9 | EXISTING mecanismos; PARTIAL ciclo novo | papinho_credential_lifetime.inc e finalização canônica; Accelerator source prepare/borrow/unborrow/retain. Não liberar dependência sobrevivente. |
| PREPARE -> COMMIT -> CURRENT -> EPOCH | EXISTING A9/A12 | PARTIAL | server_configuration_root_transaction_win32.inc: apply_dormant/transaction_prepare_store/commit/publish/current_pin; primeiro commit, não hot reload. |
| Add/approve/renew/revoke sem restart | EXISTING A12 | MISSING caminho live do cadastro | prepare_store rejeita expected != 0 e root/store inicializados; root_dormant_owned exige I/O inativo; publish declara transição/reload posterior unsupported. Estender mesma autoridade, não contornar preconditions. |
| Revoke persistente/deny novas conexões | EXISTING obrigação A1/A12 | MISSING cadastro produtivo | Allowlist lab de um leaf não prova revogação persistente multi-cliente/live/recovery. |
| Revogação sobre sessões/requests ativos | PARTIAL: obrigação A3/A12, disposição aberta | PARTIAL base | Snapshot admite deny; não determina encerramento/propagação/ack das sessões já vivas. Política concreta pendente. |
| Renovação/trust rotation | EXISTING obrigação A1/A12 | PARTIAL | Browser troca geração/retém leases; issuance/registro servidor/overlap/rollback/rotação verificada pendentes. |
| Recovery após restart | EXISTING obrigação A12 | PARTIAL | Store/recovery e Browser gerações existem; não provam recovery de pending/approval/revoke/issuer registry inexistente. |
| Unpair local != revoke remoto | EXISTING A4/A6/A12 | EXISTING distinção; MISSING remoto | papinho_options_security_remove avisa explicitamente; remover referência não revoga servidor. |
| Auditoria sem segredos | EXISTING A1/A12 | PARTIAL | Logger não implementa auditoria do enrollment ainda inexistente; sem chave/password/token/proof bruto. |
| Interface admin CLI/GUI | MISSING decisão concreta | MISSING produto | APIs privadas root não comprovam CLI/GUI instalada ou workflow autorizado de cadastro. |
| Seis gates Phase6 OPEN | EXISTING A12 | EXISTING registro documental | Atualizado roadmap Phase6 existente; nenhuma conclusão funcional nova. |

## Somente decisões concretas ainda faltantes

| Lacuna | Parte accepted, não reabrir | Decisão mínima pendente |
|---|---|---|
| D1 cerimônia/apresentação do código | Código contextual/single-use/bounded obrigatório; não é identidade | Quem compara/confirma, transcript/request/public-key binding, relay/replay/cancel/re-pair, pending/recovery; sem escolher número de dígitos. |
| D2 bootstrap | Servidor verificável, sem silent TOFU; normal Secure Principal mTLS | Como verificar antes da primeira credencial na autoridade/listener existente; trusted channel/anchors/name. Sem reduzir mTLS ou criar listener. |
| D3 pacote | Versão/integridade/destinatário/servidor/bounds/commit | Schema/encoding, autenticidade/verificador, IDs/validade/replay/duplicates, limites e confidencialidade se houver segredos. Primeiro gate do gerador. |
| D4 issuer/custódia concreta | Chave normal no cliente; mesmo issuer/Principal; CA key fora da credencial runtime | Issuer aprovado, interface CSR/PoP, identidade/SAN/EKU/algoritmo/validade/operadores/referências. Export excepcional de chave exige autoridade própria. |
| D5 interface admin/live | Uma root/autoridade; auto/manual approval e hot update obrigatórios | CLI/GUI/callers reais, permissões e contrato de publicação live, retorno factual; sem inventar comando. |
| D6 parâmetros | Expiração/single-use/rate limit obrigatórios | Valores/escopos de tempo/tentativas/pending/concurrency/replay e custos ED11; clocks apropriados. Não copiar margem de 512 MiB ou quota de outra árvore. |
| D7 revogação ativa | Persistir/deny novas conexões/Model E/leases | Política para sessões/ops ativas, checkpoints/bound/ack/revisão/epoch/falha/recovery/retirement sem ressurreição. |

Não foram aceitas escolhas dessas sete lacunas, nem criadas sete ADRs.
Submeter somente o necessário ao próximo slice, preservando o já accepted.

## Divergências reconciliadas

NORMATIVE_CONFLICTS=NONE_IDENTIFIED_WITH_SCOPED_ENROLLMENT_INTERPRETATION.
Password CLOSED novo é requisito do enrollment, não revisão universal de A2.
Manual permanente de A4 não autoriza private-key export; A7 é proposta, não
prevalece sobre A1/A4/A9/A12.

O READY/complete no cabeçalho antigo de phase6-network-tls-offload.md era do
escopo anterior/Mode B. O documento agora separa esse resultado histórico da
Phase6 integral OPEN. Nenhuma evidência histórica foi apagada/requalificada.
Não existe docs/architecture-roadmap.md no Accelerator nesta baseline; o arquivo
com esse nome é do Browser. Atualizou-se o registro Phase6 já existente no
Accelerator, sem criar outro roadmap concorrente.

FIRST_UNRESOLVED_DIVERGENCE=MANUAL_PACKAGE_SCHEMA_ISSUER_RECIPIENT_CONTRACT_NOT_FROZEN.
Hot update é lacuna funcional independente: primeiro commit não prova update live.
