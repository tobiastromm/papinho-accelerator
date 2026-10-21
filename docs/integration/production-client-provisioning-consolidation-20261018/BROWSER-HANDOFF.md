# Handoff Browser — dois modos permanentes de provisionamento

Status: ACTIVE / DOCUMENTARY_HANDOFF. Data: 2026-10-18.
Destino: Codex Browser em C:\Projetos\PapinhoBrowser.
Preparado no Accelerator; Browser permaneceu somente leitura.

## Autoridade e baseline

[Decisão explícita](../../adr/DECISION-production-client-provisioning-20261018.md),
ADR-0006/0028 Accelerator e ADR-0007 Browser, seção Production Secure Provisioning:
two official modes (07/10). [Matriz](RECONCILIATION.md): referências históricas,
estado real e lacunas. Browser HEAD1eb028554f622672377953225e162ed9b6564a40;
Accelerator HEADc50b8fb9a7e3293b5e6355fc08459bf85b56857a, ambos com WIP.
Usar canônico atual; não importar cleanup/snapshots de 07/10.

## Integração mínima após contrato aprovado

MANUAL permanente: preservar granular/avançado. Adicionar apenas frontend de
pacote aprovado e validação bounded, convergindo no pipeline existente:

```text
pacote verificado / materiais aprovados
 -> papinho_credential_material_validate
 -> papinho_credential_store_prepare
 -> draft CredentialReference + configuração validada
 -> gate e commit ConfigGeneration / ActiveConfiguration
 -> papinho_credential_candidate_committed
 -> consumidores existentes, leases e safe retirement
```

Pontos reais: src/papinho_credential_options.inc (dialog/import/draft),
papinho_credential_import.inc (DER/raw Base64), papinho_credential_store.h/.c
(validator/candidate/reference), papinho_accelerator_config_generation.inc
(commit_metadata/selector) e papinho_credential_lifetime.inc (leases/retirement).
Preservar ordem real das chamadas e seus contratos, não copiar o diagrama como
nova implementação. PKCS8 não cifrado de R1 não autoriza pacote com chave
privada gerada/exportada pelo servidor. Formato e CSR/key generation concretos
precisam de decisão. Não anunciar PEM/PKCS12/encrypted container como existente.

Não publicar candidato antes do flip factual. Cancel/failure preserva committed
anterior. Sem copiar material diretamente para gerações, editar HKCU manualmente
ou apagar dependência retida. Mudança no Browser/roadmap pertence a tarefa
própria autorizada; nenhuma edição Browser ocorreu aqui.

## Automatic e ciclo servidor

Address + servidor verificável + código + approval configurado. OPEN dispensa
login inicial; CLOSED do enrollment valida password antes de auto/manual approval.
TLS/confirmação/AuthZ/anti-replay obrigatórios. Pending/unapproved sem Session
CONTROL/DATA operacional. Bootstrap anterior à primeira credencial, cerimônia e
wire não congelados: sem silent TOFU, atalho operacional ou novo listener.
Chave normal fica no cliente; CSR/PoP conforme contrato.

Ambos convergem no mesmo cadastro/issuer/Principal-policy/grants e
CredentialGeneration/Reference. Não criar owner/scheduler/controller/store paralelo.
Proveniência MANUAL_IMPORT/ENROLLMENT pode ser metadata, sem bifurcar PST/routing/
lifetime. Unpair local não revoga servidor. Hot update/revoke persistente e
política para sessões ativas permanecem pendentes. Primeiro commit dormente do
Accelerator não prova add/revoke sem restart.

## Gates para tarefa própria

Os seis IDs no [roadmap](../../phase6-network-tls-offload.md#production-client-provisioning--phase-6-gates)
ficam OPEN. Aprovar D3/D4 e interface mínima D5 antes do parser/gerador.
Validar tamper/wrong server/recipient, CSR/key mismatch, replay/expiry,
limites/malformed/traversal, duplicate/cancel, crash antes/depois de commit,
stale writer, leases/retention/Stop. Distinguir sintético/nativo/NT4.

Preservar Site Pump, Session/owner duráveis, callbacks, quiescência, Model E,
ED08/ED11 e leases canônicos. C89/VC6/Win32/NT4; produção064/API2;
candidato065/API3 normal opt-in e R2 TEST_ONLY isolado. Sem alteração wire/PST/
quota/segredo ou admissão por esta consolidação.
CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION.
NEXT=IMPLEMENT_CANONICAL_MANUAL_CLIENT_PACKAGE_GENERATOR.
O primeiro gate é aprovar D3/D4/interface mínima; NEXT não permite inventar
formato, exportar chave ou executar PKI sem autoridade própria.
