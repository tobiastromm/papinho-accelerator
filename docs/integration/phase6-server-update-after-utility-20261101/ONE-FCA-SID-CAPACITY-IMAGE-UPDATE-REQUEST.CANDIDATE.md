# Pedido operacional — UPDATE da validação de capacidade SID FCA
CANDIDATE / MANUAL_ONLY / NOT_EXECUTED. 2026-11-03.

## Objeto e proveniência
UMA execução manual PERSONAL_LOCAL BINARY_UPDATE_CLOSED U1–U7/U2 pelo papacc_update.exe protegido já qualificado, exclusivamente para substituir papacc_server.exe preservando anterior em .previous exclusivo.
O proprietário aprovou reduzir também o piso de RAM física para3GiB, decisão registrada antes do código em docs/adr/DECISION-personal-fca-pdp-physical-floor-3gib-20261103.md SHA256c8ef29c776d02f36b3badcfdc40058b1313d14be705730a3b3b481b4e2e78d9f.
Depois autorizou contextualmente "autorizo update e start" durante a qualificação. Esse histórico não equivale a hash de candidato digitado antes de existir. Registrar decisão contextual separada com seleção dos bytes finais e exigir confirmação nativa específica do plano antes de qualquer mutação. Não executar automaticamente.

## Delta e qualificação
TentativaFCAf00f5fadbefac3246aee25907e37b70dd9321a424fc16616d0e4c30457b14a00 rejeitou predicate23 (consulta/validação SID candidato) com budget_first0/close conhecido, antes de confirmação/consumo de efeito observado. Causa nativa histórica individual ainda UNKNOWN.
Defeito de contrato demonstrado: igualdade GetLengthSid==cbSid rejeita SID válido32bytes quando capacidade68 é mantida. A documentação LookupAccountNameW descreve cbSid como capacidade de entrada e atualização de tamanho necessário em buffer insuficiente; não exigir garantia adicional de escrita de comprimento no sucesso. Fonte https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupaccountnamew .
Corrigido somente produtor FCA: capacidade>=8 e<=buffer, SID válido e GetLengthSid<=capacidade, preservados tipos1/5, autoridadeNT,6subauthorities/RID80, desigualdade com operador e demais gates; depois comprimento real GetLengthSid é associado ao plano. Alinhado aos produtores existentes serviço/runtime, sem dispensar lookup/SID ou criar fonte por string.
Modelo percorre função real e plano/confirmação com SID32/capacidade68; capacidade truncada31/excedente69 continuam negadas antes do RNG. Quatro builds sem warnings/120pares PASS; FCA-SID-CAPACITY-OFFLINE-QUALIFICATION.json.
Física/commit3GiB,128/188/192, private/working/freshness/deadlines e outros perfis intactos. Não prova retrospectivamente qual subpredicado falhou nem promete sucesso futuro.
## Objetos selecionados
HostTCORP-TOBIAS; operador S-1-5-21-785699550-1440458426-1682014700-1108; PowerShell5.1 Desktop AMD64/contexto administrativo original, não autoridade isolada.
Parent C:\Program Files\Papinho\Accelerator ID417614100:8257536:71209; âncoraProgramFiles417614100:131072:226316.
Executor C:\Program Files\Papinho\Accelerator\papacc_update.exe104448bytes/417614100:30670848:57859/SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
OLD C:\Program Files\Papinho\Accelerator\papacc_server.exe912384bytes/417614100:851968:606522/SHA256c4419f97a2e4499da4e30b8a5316a16e55097d18384fc70a0db58870c4188556, proveniência FCA-PHYSICAL-3GIB-IMAGE-UPDATE-RESULT.json.
DATA C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe912384bytes/SHA256a3adc6a253bae9fd6589c7e6f74a9b2d4cc04ea17dcf13882f2e84ef28767f8c.
Congelado papacc_server.fca-sid-capacity-912384.exe mesmos bytes/hash, não executar DATA. Identidade DATA original adquirida pelo owner; novo ID canônico somente após create/readback.
DLLs somente conferir/preservar: libssl-3-x64.dll1321472bytes/SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed; libcrypto-3-x64.dll7217664bytes/SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Preservar todos os .previous/históricos, configuração417614100:2490368:315690 e Papinho417614100:1310720:452619. Sem operações sobre ProgramData/chaves.

## Procedimento e gates
update-fca-sid-capacity-image.MANUAL.CANDIDATE.ps1 SHA25661b77275d33005a95e41a9eef382f00eaf768b2b1ec6b7a7d1a1b3ed4dff81c6.
Mesmos helpers/readlocks/owner/16argv qualificados; somente pacote/pins/OLD ID/tamanhos/banner e pin da nova decisão física. Sem reimplementar updater/staging/CLI/SCM/protocolo. Conferir hashes reais de script/pedido/decisão emitida OWNER-FCA-SID-CAPACITY-IMAGE-UPDATE-APPROVAL.txt antes da chamada; nenhuma resposta automática.
U1–U7/U2 accepted: DECISION-personal-binary-update-u1-u7-20261101.md e DECISION-personal-update-executor-data-u2-20261101.md incorporam propostas9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234 e58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
SCM PapinhoAccelerator/NT SERVICE\PapinhoAccelerator/OWN_PROCESS16/DEMAND3/ERROR_NORMAL1/SID_UNRESTRICTED1; comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed. STOPPED/PID0 antes dos efeitos e após readback; não chamar Stop.
Gates originais de executor/processo/token/imagem/dependências, parent/ancestrais/owner-DACL/proteção/semreparse, DATA finito/hash/alias-hardlink/substituição/continuidade, plano/declaração/confirmação/consumo único/Stop intactos. Hash em disco não é mapped-image attestation; histórico não é lease/quiescência.
OLD wrapper readlock é encerrado antes da aquisição nativa DELETE, sem ampliar shareDELETE. K próprio do updater/censo e buffer inalterados; R recalculado pelo tamanho/geometria factual. Física/commit>=K, disco caller/actualfree>=R e observações/aritmética/Stop antes das submissões. Novo piso FCA não é fallback ou mudança de U4 do updater.

## Efeitos, risco e proibições
Depois da confirmação atribuída e consumo irrevogável: UMA criação exclusiva protegida no parent, cópia finita912384bytes, hash independente/SD/flush/readback, no máximo dois renames sem overwrite OLD->.previous e novo->canônico, RootDirectory=NULL do protocolo. Leaves concretos gerados/apresentados antes das mutações.
Riscos: opacos/exaustão/chamadas longas, atualização parcial/intervalo sem canônico, retenção e ausência de rollback automático; sem atomicidade/durabilidade universal. Stop não cancela IO submetido. Falha/colisão/SCM divergente/substituição/readback/close inconclusivo: STOP/UNKNOWN sem retry/adoção/delete/completar/restauração/refund/kill.
Autorizações FCA/observação/UPDATE anteriores usadas/consumidas não são reutilizáveis. Sem novos diretórios/originais/keys/fixture. Sem CA/CURRENT/assinatura/certificado/CSR/exportação privada, endpoint/SCQ/E/Q/S/P/StartStop/SCM-ACL/rede/TPM-NV-Q4, UPDATE de executor/DLLs, limpeza/produção/readiness neste ato.
A autorização contextual para avançar ao Start é preservada como intenção de continuidade, mas este procedimento só atualiza a imagem; não concede afirmação de pré-requisitos/identidades/confirmações E/Q/S ou preparação FCA PASS. Comandos futuros exigem escopo/gates/confirmacões originais completos, sem solicitar reaprovação redundante quando a autorização já cobrir o ato concreto.
Retenção histórica/quiescência OS UNKNOWN, HEAD/WIP/EXEs/pins/históricos/PST0.6.4 e DENY preservados.

## Reconciliação
Receber saída integral, primeira falha/consumo/criação/dois renames, anterior preservado, novo ID/tamanho/hash/proteção/readback, SCM parado, close/retenção. Não inferir sucesso pelo exit. Não repetir.
Após resultado suficiente preparar o próximo ato concreto usando configuração/Papinho originais, nova tentativa e confirmação nativa, sem prometer que o novo piso elimina falha interna da aquisição ou garante Start. Sem execução automática.
Autorização atual de continuidade: proprietário declarou "autorizo sempre os updates chat, n tenho outra escolha". Registrar honestamente para este UPDATE necessário da investigação, sem inventar hash aprovado anteriormente. Preservar execução manual/confirmacão nativa/gates originais, uma tentativa e nenhum retry, sem ampliar efeitos/proteção/produção.