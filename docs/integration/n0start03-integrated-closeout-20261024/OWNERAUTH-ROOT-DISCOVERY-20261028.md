# Investigação factual da root para entrega STORAGE_20

Estado: UNKNOWN / DENY. Investigação somente por leitura em 2026-10-28.
Ref verificada: main, f341985924d29870d8ff627ed37c8a0874ad6469.

O proprietário declarou não possuir root administrativa operacional identificada e excluiu a promoção automática de roots, configurações e manifestos de desenvolvimento/teste. Nenhuma publicação, adoção, recuperação ou operação protegida foi executada.

## Evidência encontrada

| Fonte | Fato observado | Limite |
|---|---|---|
| apps/server/server_configuration_root_transaction_win32.inc:44 | A entrada prepare_store recebe caminho do chamador; exige primeiro commit dormente, expected zero e owner ainda não inicializado; comentário proíbe adoção de store existente | Não é um leitor nem uma rota de descoberta/adoção operacional |
| apps/server/server_configuration_commit_win32.c, papacc_configuration_commit_init | Requer destino ausente e cria domínio novo em memória | Executá-la fabricaria uma identidade nova, fora da investigação |
| Busca prepare_store em apps e tests | Definição, declaração e chamadas em tests/canonical_root_transaction_test.c:247/251 | Não demonstra inexistência de publicadores externos ou root fora do escopo pesquisado |
| tests/canonical_root_transaction_test.c:235–239 | Caminho fornecido pelo teste ou build/issuer-session-live-publication-20261019/root-*.bin, build/issuer-live-root-recovery-20261019/app-root-*.bin e build/phase6-canonical-consolidation-20261018/root-tx-*.bin | Nomes contendo live/publication não concedem autoridade operacional |
| canonical-root-c2-c3-wiring-20261018/closeout.md, lido integralmente | Commit e CURRENT/epoch qualificados nativamente em escopo local offline/dormente; startup de produto negado | Persistência de teste não é publicação operacional |
| initial-bootstrap-custody-authorization-20261022/closeout.md, lido integralmente | BOOTSTRAP_EXECUTION=NOT_RUN; CURRENT_PUBLICATION=DENY | Não há recibo positivo de publicação nesse registro |
| personal-first-install-uac-launcher-20261022/manual-readonly-success-20261022.md, lido integralmente | Cerimônia manual read-only; bootstrap não executado, CURRENT negado | Não fornece root administrativa publicada |
| tools/qualification/n0-protected-root-creator.c:12 | Caminhos C:\ProgramData\PapinhoAccelerator\Protected\N0; criação governada de diretórios N0 | Namespace protegido LAB não equivale ao owner/root C8 |
| Listagem de C:\ProgramData | Diretório PapinhoAccelerator presente | Identidade original e conteúdo não verificados |
| Listagem de C:\ProgramData\PapinhoAccelerator | Access denied | Não houve escalonamento, bypass, leitura protegida ou inferência de ausência |

## Resultado e fronteira autenticada

Owner de código identificado: PAPACC_CONFIGURATION_COMMIT embutido no PAPACC_SERVER_LAUNCHER_OWNER_WIN32. Não foram identificados caminho operacional C8, domínio publicado, manifesto original verificável, processo owner vivo, pin/lease factual ou produtor nativo ativo da fronteira autenticada para entrega STORAGE_20.

O helper DATA e o guard isolado existentes não substituem esses fatos. SID, digest, elevação administrativa ou nome de arquivo declarados não autenticam destinatário, propósito ou publicação. Nenhuma credencial real foi acessada. Nenhum binding histórico foi promovido a binding atual.

Não se conclui que inexiste root no equipamento. Conclui-se que as fontes e caminhos revisados não comprovam uma root operacional legítima; o diretório protegido permaneceu inacessível.

## Primeira evidência necessária e sequência mínima

1. Localizar um registro não secreto de publicação autorizada que identifique caminho C8, owner responsável e proveniência da publicação. Um manifesto de fixture não satisfaz isso.
2. Se tal registro existir, preparar verificação própria de objeto original/RAW1/conteúdo e correlação ao owner/pin atual, sob a autoridade aplicável. Não executar o readback Q4 como substituto.
3. Se a root nunca foi publicada, a decisão real é sobre o bootstrap/publicação governados e seus pré-requisitos; não é outra aprovação de custodiante ou STORAGE_20. Nenhuma publicação foi proposta como já autorizada aqui.
4. Somente depois vincular contexto/token/host/boot, módulo original da fonte e fronteira nativa autenticada; finalizar pedido único de entrega em memória e obter sua autorização operacional própria.

Pedido anterior permanece histórico e não executável: custody-storage20-acceptance-20261028/CONTROLLED-STORAGE20-MEMORY-REQUEST.DRAFT.json. Não foi gerado comando com placeholders ou nova aprovação.

## Qualificação e continuidade

Nenhum código, binário, pin operacional ou ADR accepted alterado. Builds e campanhas anteriores não repetidos, pois não houve alteração funcional. As buscas de call sites e os documentos acima constituem a evidência nova; não são testes nativos de identidade protegida.

E17: prova real de identidade/acesso/readiness do serviço pendente. E18: lifecycle/Stop/quiescência pendentes. E19: justificativas quantitativas D4/D5 e provider/kernel ativos pendentes. E20: composição real e bindings root/CURRENT/epoch/SiteTLS/backend/destino pendentes. Essas frentes não recebem créditos desta investigação e não dependem de repetir a consulta STORAGE_20. E21 continua PENDING; E22 derivado pendente; ED11 e Phase 6 PARTIAL, autoridades DENY.

UNKNOWN, reservas, históricos e WIP preservados. Sem ownerAuth, TPM/NV, Q4, Start/Stop, SCM/ACL, limpeza, refund, CURRENT, assinatura ou recuperação autoritativa.
