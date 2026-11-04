# Phase 6 — presenter nativo F0–F9 da primeira instalação

TASK: `PACC-PHASE6-NATIVE-FIRST-INSTALL-PRESENTER-OFFLINE`

Partir do closeout deste pacote. Reutilizar a persistência CurrentUser qualificada sem repetir criação de chave. Não excluir, sobrescrever ou promover a chave retida. Não retomar TPM/Q4, CAS ou formatos já aceitos.

A lacuna concreta está em `personal_native_ceremony_ready` (DATA guard fechado), enquanto `personal_native_open/current`, `personal_source_current` e `personal_handoff_worker` já conservam referências. Implementar no owner canônico o presenter/ato atribuído F0–F9: tentativa CSPRNG, política vigente fora do candidato, operador/token efetivo e parent/source retidos, declaração explícita NEW_INSTALL/CA_INITIALIZE, apresentação completa e confirmação do plano exato, invalidação por mudança/Stop, consumo único e lifetime até resultado factual. Não transformar elevação ou stdin isolados em autoridade; não usar callbacks positivos como prova nativa.

Qualificar o presenter com I/O/API substituídos somente em testes e as verificações nativas negativas read-only. Preservar a criação operacional negada até todas as fontes e gates reais; nenhuma execução NEW_INSTALL real é autorizada. Não alterar contratos accepted, quotas ou identidade operacional por conveniência. Reportar a cláusula exata se faltar decisão, sem criar outra família de ADRs.

Manter os caminhos CA nova e EXISTING_CURRENT distintos. Continuar M7/E8/enrollment não operacionais onde autorizado. Entregar código, testes afetados, hashes, matriz/live docs e primeira evidência factual restante. Não criar pedido operacional com placeholders.

Proibido criar CA/root/chave operacional, publicar CURRENT, assinar produtivamente, alterar SCM/ACL, executar rede/Start/Stop/TPM/NV/Q4, cleanup/refund ou recuperação. Preservar HEAD/WIP, históricos, pins, UNKNOWN, reservas e PST 0.6.4.
