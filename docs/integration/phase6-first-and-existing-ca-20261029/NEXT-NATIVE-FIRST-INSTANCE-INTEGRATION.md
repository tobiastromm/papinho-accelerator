# PapinhoAccelerator — entrada nativa da primeira instância

**TASK:** `PACC-PHASE6-NATIVE-FIRST-INSTANCE-ENTRY-INTEGRATION`

Projeto: `C:\Projetos\PapinhoAccelerator`.

Ler o closeout `phase6-first-and-existing-ca-20261029` e NATIVE-ENTRY-DECISION.md. Não repetir reconciliação de M3/F0–F9 nem procurar CA anterior para autorizar NEW_INSTALL. TBS inicial e EXISTING_CURRENT estão integrados; os positivos de criação/assinatura são modelos.

**Pré-condição:** decisão explícita para integrar o chamador nativo persistente descrito em NATIVE-ENTRY-DECISION.md. Este arquivo não registra essa aprovação. A revisão automática anterior rejeitou o chamador; não reaplicar nem contornar a rejeição sem resolver o escopo.

Após autorização válida de código, implementar a menor entrada nos owners existentes, com plano NEW_INSTALL/CA_INITIALIZE original, referências nativas, consumo único e retenção. Preservar o modo de CA existente, sem fallback. Guard DATA-only, proofs NORMAL ausentes ou recursos UNKNOWN não podem virar PASS por flags. Integrar fontes legítimas de admissibilidade antes de qualquer habilitação; nenhuma chamada real de criação/assinatura é autorizada.

Qualificar criação e falhas em fixture substituído, fontes ausentes, concorrência, Stop, incerteza, identidade/cert/key/generation, reserva e publicação/re-read. Reutilizar as campanhas válidas; compilar Selected/Legacy Debug/Release apenas conforme alterações. Não alterar formatos ou normas por inferência; demonstrar cláusula concreta se houver lacuna.

Entregar patch, testes/hashes, limites restantes e o pedido operacional somente se os bindings forem reais e completos. Não gerar comando com placeholders. Permanecem proibidos CA real, CURRENT produtivo, assinatura produtiva, rede, Start/Stop, SCM/ACL, TPM/NV/Q4, cleanup/refund, recuperação e release. Preservar HEAD, WIP, históricos, pins, UNKNOWN, reservas e PST 0.6.4.
