# Pedido operacional — UPDATE da captura de etapa prévia FCA
CANDIDATE / MANUAL_ONLY / NOT_EXECUTED. 2026-11-03.

## Escopo
Solicita UMA execução manual PERSONAL_LOCAL BINARY_UPDATE_CLOSED U1–U7/U2 pelo papacc_update.exe protegido já qualificado. Atualizar exclusivamente papacc_server.exe, preservando o anterior por referência em leaf .previous exclusivo. Nenhuma execução automática ou retry.
Este documento/hash/argv são DATA, não autoridade. A decisão específica ainda não foi emitida. Confirmação atribuída BINARY_UPDATE_CLOSED no plano nativo permanece obrigatória.

## Alteração selecionada e evidência
Somente diagnóstico local de etapa1–12 no prefixo personal_present; captura de retorno PAPACC_RESULT das chamadas de aquisição4/plano11. Em rejeição anterior à apresentação, imprime FCA_PREFLIGHT_REJECT antes do close, sem nova consulta nativa/segredos.
operation_result é o último retorno capturado, não errno/NTSTATUS do predicado corrente nem prova de sucesso nas outras etapas. Localiza uma região; não resolve retroativamente a causa histórica, nem garante granularidade interna de uma API.
Sem alteração de guards, ordem, quotas, proteção, autoridade, owner, perfis ou mecanismo administrativo.
Quatro builds Selected/Legacy Debug/Release sem warnings;120 pares teste/configuração PASS. Manifesto FCA-PREFLIGHT-STAGE-OFFLINE-QUALIFICATION.json e relatório FCA-PREFLIGHT-REJECTION-OFFLINE-REPORT.md.
Nenhuma campanha inalterada foi repetida nesta preparação de UPDATE. O updater independente não foi recompilado.

## Objetos e pins completos
Host TCORP-TOBIAS, operador S-1-5-21-785699550-1440458426-1682014700-1108, Windows PowerShell5.1 Desktop AMD64, contexto administrativo original. Metadados/elevação isolados não são autoridade.
Parent C:\Program Files\Papinho\Accelerator ID417614100:8257536:71209; âncora ProgramFiles417614100:131072:226316.
Executor C:\Program Files\Papinho\Accelerator\papacc_update.exe,104448bytes,ID417614100:30670848:57859,SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe,911360bytes,ID417614100:2883584:1065605,SHA256c14dc2a0e2f3e51d19588c9930e354d512c9f3d3c41f0eaff7957dae0734f474.
Novo DATA C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,911360bytes,SHA256747ba426d55af50947169a178c8366def46cb57ad4a8c29308ff44664aeaee96.
Congelado offline em papacc_server.fca-preflight-stage-911360.exe, mesmos bytes/hash. Não executar o DATA. Identidade original do DATA será capturada pelo updater, não inventada. Novo canônico terá identidade futura só após create/readback.
DLLs somente conferir/preservar:
libssl-3-x64.dll1321472bytes/SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
libcrypto-3-x64.dll7217664bytes/SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

## Normas, procedimento e autoridade
U1–U7 accepted: docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md, incorporando proposta SHA2569fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234.
U2 accepted: docs/adr/DECISION-personal-update-executor-data-u2-20261101.md, incorporando proposta SHA25658493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Procedimento update-fca-preflight-stage-image.MANUAL.CANDIDATE.ps1 SHA25617bca20beb9e9e6f7b33bc779ed7162c323a250699e50f1382913eaffe1ecc24.
Reutiliza byte-exatamente os helpers qualificados da revisão3 anterior, uma chamada do mesmo executor e16 argumentos tipados. Somente nomes do pacote, pins/ID/tamanho anterior e novo payload mudam. A referência OLD preflight é encerrada uma vez antes da aquisição nativa com DELETE; sem ampliar share DELETE.
Verificar script/pedido/decisão reais antes da chamada. A decisão futura OWNER-FCA-PREFLIGHT-STAGE-IMAGE-UPDATE-APPROVAL.txt só será registrada após autorização deste pedido. Comando completo será emitido com hash real, sem placeholders e sem resposta automática.

## Gates frescos
PapinhoAccelerator original: OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1/SID_UNRESTRICTED1; conta NT SERVICE\PapinhoAccelerator; comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.
Exigir SCM STOPPED/PID0 e configuração inalterada antes dos efeitos e após readback. Não executar Stop para conseguir isso. Nenhuma nova consulta ao SCM pelo agente; histórico não é lease nem quiescência global.
Owner/DACL efetiva, ancestrais protegidos, ausência de reparse, imagem/processo/token/contexto e originais do executor/DLLs/OLD/DATA, rejeição de alias/hardlink/substituição, continuidade, confirmação/consumo irrevogável e Stop permanecem no owner nativo.
U4 domínio do updater: K calculado pelo censo qualificado próprio, composição e buffer inalterados; não usar2880836 FCA como K. O tamanho DATA é igual ao UPDATE anterior, mas R deve ser recalculado da geometria factual do volume; não fixar valor histórico. Observações frescas, física/commit >=K e caller/actual-free >=R, antes das submissões e aritmética sem overflow obrigatórios. Custos opacos UNKNOWN, sem suficiência universal.
Não importar envelopes P1/FCA/READ como fallback para UPDATE.

## Efeitos permitidos e disposição histórica
Depois da confirmação/consumo: UMA criação exclusiva protegida no mesmo parent; cópia finita911360bytes; hash independente/SD/flush/readback; no máximo dois renames sem overwrite, OLD para .previous exclusivo e novo para canônico, RootDirectory=NULL conforme protocolo qualificado. Nomes concretos apresentados pelo owner antes dos efeitos, não inventados.
Preservar todos os .previous/candidatos/fontes/históricos. O anterior já retido905728bytes/ID417614100:2555904:1047601/leafpapacc_server.eca6172e632933912004b5a3a14fc321d3a63bf9da585c8e820df8eab85abf3d.previous não é alvo.
Preservar configuração protegida417614100:2490368:315690 e Papinho417614100:1310720:452619; não abrir/criar/alterar ProgramData ou chave nesta operação.
Última tentativa FCA: sem confirmação nativa exibida, prepared0/flagszero/close conhecido e wrapper retornado. Não se observou consumo de efeito; tentativa manual usada e não reutilizável. Flagszero não são ausência independente de objetos. Nenhuma recuperação/limpeza autorizada. Essa tentativa não deixou worker ativo demonstrado pelo percurso anterior à confirmação; isso não resolve coortes/histórico/quiescência OS UNKNOWN.

## Riscos, falhas e limites
Exaustão/custos opacos, chamadas longas, falha parcial e intervalo sem canônico, retenção persistente e nenhum rollback automático. Sem atomicidade/durabilidade universal. Stop impede novas submissões, não cancela IO submetido. Qualquer divergência/colisão/SCM não parado/substituição/readback ou close inconclusivo: STOP/UNKNOWN, sem retry/delete/adoção/completar/restaurar/refund.
Não autoriza FCA/recriar configuração/Papinho/Accelerator/chave, fixture, CA/CURRENT/assinatura/certificado/CSR/exportação privada, READ/E/Q/S/P/endpoint/StartStop, SCM/ACL, rede, TPM/NV/Q4, atualização do updater/DLLs, limpeza/kill ou readiness.
Preservar HEAD/WIP, contratos accepted, históricos/pins, autorizações consumidas e PST0.6.4.

## Reconciliação e próximo marco
Receber saída integral, identificar primeira falha/consumo/criação/dois renames; verificar anterior preservado, novo canônico por ID/tamanho/hash/SD/readback, SCM parado, close/retenção. Exit não basta; não repetir.
Só depois preparar o menor ato independente que permita obter a etapa original FCA, sem reutilizar a tentativa anterior. Se necessário refinar diagnóstico interno após localização, preservar UNKNOWN: não se promete que esta instrumentação isoladamente identifica o NTSTATUS exato ou conclui a preparação.