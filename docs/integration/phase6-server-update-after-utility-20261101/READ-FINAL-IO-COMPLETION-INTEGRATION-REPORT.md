# Integração offline — conclusão local da I/O final READ

2026-11-03. OFFLINE_QUALIFIED / UPDATE_NOT_EXECUTED / PRODUCTIVE_DENY.

A disposição específica do controlador 5824 foi reconciliada: first=0, consumed=True, submitted=True, exit_observed=True. Não constitui application-close SCQ. A autorização está consumida. Retenção histórica, reservas e quiescência OS permanecem UNKNOWN.

## Correção e limite da evidência

O modelo demonstrou a intercalação na qual o runtime consome os bytes da escrita final 16 e encerra antes de o controlador observar sua conclusão OVERLAPPED. O guard de peer vivo rejeitava a readquisição original antes desse polling. Isso explica um defeito reproduzível; não prova por si só todos os detalhes internos da tentativa histórica.

personal_scq_machine_read_continue_win32.inc integra scqkm_retained_peer e scqkm_return_inconclusive. personal_scq_transport_continue_win32.inc encaminha exclusivamente a conclusão READ inconclusiva estrita já submetida, antes da readquisição que exige liveness. A submissão inicial continua usando scqt_start_io e os guards de peer vivo. Não há nova escrita após o encerramento, reconnect, reopen por PID ou retomada de sessão consumida.

A continuação usa os mesmos processo, delivery-process, token, pipe, evento, OVERLAPPED e buffer retidos. Confere PID/criação, TokenId/authentication/modified, SID/grupos/logon e vínculo ao plano; exige owner/thread, fontes/admin originais, READ explícito, P consumido, controle positivo, resultado BADKEYSET estrito, close original aceito, janela, G3 e Stop. Somente bytes completos e frame SQC3/record/IDs originais permitem concluir leg16. Incomplete, erro, substituição, Stop ou expiração preservam DENY/UNKNOWN/retenção.

A consulta pelos handles retidos não concede nova autoridade. Os guards globais de processo vivo permanecem para novos efeitos. Exit original, SCM terminal e fechamento dos owners continuam provas separadas exigidas pelo lifecycle I0–I6. ERROR_NOT_FOUND/CLI3 significa inconclusivo, nunca READ PASS ou readiness produtiva. TRANSPORT_ONLY, READ ordinário e NEW_INSTALL mantêm seus percursos e quotas.

Não foram adicionados canais, wire, owners, threads ou handles de diagnóstico. As consultas pelos originais retidos têm custo opaco UNKNOWN; a qualificação em modelo não demonstra teto nativo ou suficiência universal.

## Qualificação

Quatro builds Selected/Legacy Debug/Release, zero warnings; 22 testes por configuração, 88 pares PASS. Evidência: READ-FINAL-IO-COMPLETION-OFFLINE-QUALIFICATION.json e logs read-final-io-completion-final9-*. Os logs anteriores, inclusive falhas intermediárias de modelo, foram preservados.

scq_machine_read_chain_test: 27 cenários, 1604 checks por variante SD, intercalação com processo sinalizado e 17 negativos, incluindo identidade/token, frame, truncamento, pending, erro, replay, Stop e deadline; nenhuma escrita adicional. O modelo de token foi completado com grupo/logon de serviço. A restauração de checkpoint usada para comparar o guard antigo é somente técnica de teste, nunca recuperação operacional.

scq_transport_continue_wrapper_test: 35 checks; wrapper real e fronteiras modeladas verificam encaminhamento antes do getter de peer vivo e preservação dos demais branches. O helper real e polling/frame são exercitados pelo teste de cadeia; não se afirma que isso substitui uma sessão Windows real.

AST do procedimento UPDATE PASS; helpers byte-idênticos ao procedimento anterior; mesmos 16 argumentos e uma chamada do executor. Nenhum procedimento operacional foi invocado. git diff --check global também encontrou linha vazia final em docs/future/network-egress-policy.md, fora desta correção; WIP preservado.

## Imagens e próximo gate

Instalada permanece 880128 bytes, ID417614100:524288:605924, SHA256 7ccb080b564ae331a5e212bd455b3b5aff636cae28089c5a4c306268d8687acc, conforme última evidência reconciliada, não lease atual.

Candidato Selected/Release congelado: 882688 bytes (+2560); SHA256 e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7. Não instalado. Executável protegido do updater, DLLs e anteriores preservados.

Primeiro gate: autorização específica de ONE-READ-FINAL-IO-COMPLETION-IMAGE-UPDATE-REQUEST.CANDIDATE.md, SHA256 5da25da5938aef7eaa289e320393051fd7dfdb1f5fed05db364b6c94487e1d86. A aprovação I0–I6 cobre integração offline, não este UPDATE. Nenhuma decisão deste pedido foi fabricada.

Caminho mínimo: decisão específica → comando manual com hashes reais → uma execução U1–U7/U2 com gates frescos → reconciliação de renames/readback/novo ID/SCM/close → novos originais e decisão de sessão READ independente → quatro confirmações E/Q/S/P → provas nativas de bilateral, probe, receipt/close e terminal. Reutilizar a fixture qualificada, sem recriar chave. Nenhum PASS futuro é presumido.

Norma aplicável: docs/adr/DECISION-personal-scq-read-inconclusive-i0-i6-20261103.md, além de K0–K9, S0–S5 e U1–U7/U2 accepted. Sem alteração normativa ou de direção futura de rede.
