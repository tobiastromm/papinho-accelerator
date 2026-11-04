# Pedido operacional — UPDATE diagnóstico de aquisição original FCA
CANDIDATE / MANUAL_ONLY / NOT_EXECUTED. 2026-11-03.

## Escopo independente
Uma execução manual BINARY_UPDATE_CLOSED PERSONAL_LOCAL U1–U7/U2 pelo updater protegido existente. Somente substituir papacc_server.exe, preservar anterior em .previous exclusivo, sem retry/rollback/limpeza. Este pedido não autoriza nova FCA e não reutiliza as autorizações usadas. Fonte de autoridade será o proprietário e confirmação nativa específica, não DATA/hash/argv.

## Motivo e limitação
A tentativa FCAae3b8ea2937a01bf2498b3c19c48b4058529c691c43d9f83b07e93df84073ec0 retornou stage4/operation_result4=PAPACC_RESULT_INVALID_STATE antes de apresentação; não é Win32/NTSTATUS. Fechamento conhecido, consumo nativo de efeito não observado; tentativa manual usada e não repetível. Flagszero não são ausência independente.
Correção exclusivamente diagnóstica:26 rejeições de personal_first_open_mode recebem número estável de predicado e retornam o MESMO resultado original. Nenhuma nova consulta/alteração de estado/guard/ordem/direito/fechamento ou perfil. Removendo helper e decoradores, a função/arquivo retorna byte-exatamente ao SHA25610dd573a745f186ce816842a7ef42b5fba384fc3b82a3ee8a1d180c25002c287. Logs não expõem SID/caminhos/bytes ou segredo adicional.
Identifica o ramo de rejeição; ramos compostos continuam tendo mais de um subpredicado. Não promete NTSTATUS exato nem causa histórica resolvida ou futuro PASS.
Quatro builds sem warnings e120 pares PASS; FCA-ACQUISITION-BRANCH-OFFLINE-QUALIFICATION.json. Sem operação real nesta qualificação.

## Objetos selecionados
TCORP-TOBIAS; operador S-1-5-21-785699550-1440458426-1682014700-1108; PowerShell5.1 Desktop AMD64/contexto administrativo original, não autoridade isolada.
Parent C:\Program Files\Papinho\Accelerator ID417614100:8257536:71209; âncora ProgramFiles417614100:131072:226316.
Executor C:\Program Files\Papinho\Accelerator\papacc_update.exe,104448bytes/417614100:30670848:57859/SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
OLD C:\Program Files\Papinho\Accelerator\papacc_server.exe,911360bytes/417614100:8716288:318363/SHA256747ba426d55af50947169a178c8366def46cb57ad4a8c29308ff44664aeaee96. Proveniência FCA-PREFLIGHT-STAGE-IMAGE-UPDATE-RESULT.json e evidência0170cfbbe94b34a73201ea09c8dc4c2f61ef728ead5e94d86a773a05e64ca07b.
DATA C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,912384bytes/SHA256f4d91013a34da09053dcb800dcd909f82ba544384f1430d9c22584d1d24ea82b. Congelado papacc_server.fca-acquisition-branches-912384.exe. Não executar DATA; ID original DATA adquirido pelo owner, novo ID canônico somente após criação/readback.
DLLs somente conferir/preservar: libssl-3-x64.dll1321472bytes/SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed; libcrypto-3-x64.dll7217664bytes/SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Preservar todos os .previous históricos, configuração417614100:2490368:315690 e Papinho417614100:1310720:452619, sem operações sobre ProgramData/chaves.

## Procedimento e gates
update-fca-acquisition-branch-image.MANUAL.CANDIDATE.ps1 SHA256d6013467022e83a25edb0ebe01dd90bd25942d8f602f2aaed208ee438fb9a115. Mesmos helpers/owner/16 argumentos do UPDATE qualificado, só pacote/pins/tamanhoDATA/OLD ID/banner mudam. Sem reconstruir updater/staging/serviço/protocolo.
Verificar hashes reais script/pedido/decisão futura OWNER-FCA-ACQUISITION-BRANCH-IMAGE-UPDATE-APPROVAL.txt antes da chamada. Após aprovação será registrado hash real e comando completo sem placeholders. Não fornecer respostas automáticas.
Normas accepted DECISION-personal-binary-update-u1-u7-20261101.md (incorpora9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234) e DECISION-personal-update-executor-data-u2-20261101.md (incorpora58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da), F1–F6 aplicáveis.
SCM original obrigatório PapinhoAccelerator/NT SERVICE\PapinhoAccelerator/OWN_PROCESS16/DEMAND3/ERROR_NORMAL1/SID_UNRESTRICTED1; comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed. STOPPED/PID0 antes dos efeitos e após readback; não chamar Stop.
Originais/token/processo/imagem do executor, DLLs, parent/ancestrais, owner/DACL/proteção efetiva, ausência de reparse, DATA finito, alias/hardlink/substituição/continuidade, plano imutável/declaração/confirmacão atribuída/consumo único/Stop obrigatórios.
Wrapper libera apenas OLD preflight antes da aquisição nativa DELETE; sem ampliar share DELETE. Hash de disco não é mapped-image attestation. Histórico não é lease nem quiescência OS.
K do domínioU4/censo do updater próprio inalterado; R recalculado pelo tamanho912384 e geometria factual, sem fixar R histórico. Física/commit>=K e caller/actualfree>=R, amostras/aritmética/Stop antes das submissões; sem importar envelope FCA/READ/G3 ou tratar opacos como zero/suficientes.

## Efeitos, riscos e proibições
Após confirmação/consumo irrevogável: UMA criação exclusiva protegida no parent, cópia finita912384bytes com buffer existente, hash independente/SD/flush/readback e no máximo dois renames sem overwrite: OLD para .previous exclusivo e novo para canônico, RootDirectory=NULL do protocolo qualificado. Leaves concretos produzidos/apresentados pelo owner antes da mutação. Preservar anterior e todos os históricos.
Riscos: opacos/exaustão, bloqueios longos, atualização parcial/intervalo sem canônico, retenção e nenhum rollback automático; sem atomicidade/durabilidade universal. Stop não cancela IO submetido. Falha/colisão/SCM divergente/substituição/readback ou close inconclusivo: STOP/UNKNOWN; não completar automaticamente, restaurar, delete/refund/retry.
Não autoriza FCA/coleta/novo ensaio/configuração/Papinho/Accelerator/chave/fixture, CA/CURRENT/assinatura/certificado/CSR/exportação privada/readiness, endpoint/SCQ/E/Q/S/P/StartStop/SCM-ACL/rede/TPM-NV-Q4, UPDATE de executor/DLLs, limpeza/kill/produção.
Retenção histórica/quiescência OS UNKNOWN, DENY, HEAD/WIP/EXEs/pins/consumos/PST0.6.4 preservados.

## Reconciliação
Receber saída integral, primeira falha/consumo/criação/dois renames, anterior preservado, novo ID/tamanho/hash/SD/readback, SCM parado, close/retenção. Exit isolado não basta. Não repetir.
Somente depois preparar ato independente mínimo para capturar ramo original; não reutilizar FCA histórica ou presumir causa resolvida.