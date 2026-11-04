# Pedido candidato — UPDATE da imagem corrigida do produtor SID G3-P1

2026-11-02. READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION.
NÃO AUTORIZADO / NÃO EXECUTADO. Solicita exclusivamente UMA execução manual
BINARY_UPDATE_CLOSED PERSONAL_LOCAL para trocar somente papacc_server.exe.
A aprovação normativa G3-P1 não autoriza este UPDATE nem uma sessão E/Q/S.

## Contratos e finalidade

Reutilizar U1–U7/U2 accepted e o executor independente protegido existente.
DECISION-personal-binary-update-u1-u7-20261101.md incorpora a proposta
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234;
DECISION-personal-update-executor-data-u2-20261101.md incorpora
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Disponibilizar a correção do produtor de tamanho SID do runtime, sem alterar o
envelope já instalado e aprovado em
DECISION-personal-scq-p1-handle-envelope-20261102.md, alternativa B, proposta
9559fbb0f82e875ad33a6dfc80c52fa4572ae3267338d6ff4401c2a901881abf.
192/188/entrada128 somente P1; READ/NEW_INSTALL e demais perfis inalterados.
Esses números NÃO se transportam para UPDATE. Não promete completar Start.
Fonte e qualificação: G3-P1-RUNTIME-SID-PRODUCER-QUALIFICATION.md, quatro builds sem
warnings, 56 pares teste/configuração afetados PASS. Nenhuma operação real.

Correção técnica focal: capacidade cbSid do buffer não é comprimento semântico
prometido no sucesso. Após a MESMA lookup original, validar limites/geometria,
IsValidSid e GetLengthSid e registrar o comprimento validado. scqp_unbound
permanece estrito; não limpar/truncar/normalizar SID, padding ou campos futuros.
Modelo reproduz SID32/capacidade68 => reason3 no produtor antigo; o novo usa32.
Isso demonstra o defeito de contrato, não os valores históricos não emitidos.
Sem novo owner/canal/autoridade, query adicional, retry ou tamanho wire.
Diagnóstico existente preservado. Histórico13/retensão/OS permanecem UNKNOWN.

## Seleção e originais esperados

Operador SID S-1-5-21-785699550-1440458426-1682014700-1108, host TCORP-TOBIAS.
Contexto administrativo legítimo, identidade/token originais e declaração/
confirmação atribuída OWNER_BINARY_UPDATE do plano nativo são obrigatórios.
Hash, DATA, imagem instalada e este candidato não constituem autoridade.

Parent C:\Program Files\Papinho\Accelerator,
ID417614100:8257536:71209. ProgramFiles x64 histórico
ID417614100:131072:226316. Readquirir/reter/revalidar objetos, owner/DACL
estritos, ancestrais, ausência de reparse e continuidade; sem alteração ACL.

Executor C:\Program Files\Papinho\Accelerator\papacc_update.exe,
104448 bytes, ID417614100:30670848:57859, SHA256
084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Não substituir nem executar outro updater do workspace. Guardas U2 permanecem.

Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe,
784896 bytes, ID417614100:4325376:164699, SHA256
0b00a21a148b4b015d2fc40d8f412441a8af78a77fcfa7bed93a67a60048fde3.
Origem histórica: G3-P1-FINAL-PLAN-IMAGE-UPDATE-RESULT.json e reconciliação; saída
OWNER-MANUAL-G3-P1-FINAL-PLAN-UPDATE-OUTPUT.txt, SHA256
41efee66e59ebf16e13e09ac92e8fb1db6ea033a42955242962ce936e16ad78e.
Autorização 43a5bad791d20ee5d48c7bd0efe90e25ca956a139c1f491bc8f7bc986720b4c7
consumida, assim como todas as anteriores. Não reutilizá-las.

Candidato Selected/Release exclusivamente DATA, não carregar nem executar:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe.
**784896 bytes**, SHA256 **201b0f01d0d6b33f73d653bd492567ff663f14e183170ced4d18a6a6f0ddb752**. Delta de tamanho 0 bytes; conteúdo e SHA256 diferentes.
Cópia de congelamento workspace papacc_server.g3-p1-runtime-sid-784896.exe tem os mesmos
bytes; não é executor ou infraestrutura de distribuição. Não selecionar build
posterior silenciosamente. Identidade de DATA capturada na referência original
retida durante a invocação; não inventada antecipadamente. Inspecionar descritor,
impedir escrita/substituição, conferir identidade/conteúdo/hash/continuidade;
rejeitar aliases/hardlinks entre executor, DATA e canônico.

DLLs inalteradas, validar somente:
- libssl-3-x64.dll:1321472 bytes, SHA256 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
- libcrypto-3-x64.dll:7217664 bytes, SHA256 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

SCM PapinhoAccelerator: OWN_PROCESS16, DEMAND_START3, ERROR_NORMAL1,
NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED1. Comando exato
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Readback SERVICE_STOPPED/PID0 e configuração originais frescos antes dos efeitos
e após substituição. Nenhum Start/Stop ou alteração de configuração SCM.
IDs, hashes instalados/proteção e estado acima são EXPECTATIVAS HISTÓRICAS,
não consultas atuais ou lease. Qualquer divergência => STOP.

## Efeitos e retenção

Uma chamada do owner existente, tentativa CSPRNG32 bytes gerada na execução.
Plano nativo mostra dois nomes concretos exclusivos da mesma tentativa:
papacc_server.<hex64>.candidate e papacc_server.<hex64>.previous. Binding tardio
já qualificado, não escolha de novo nome para contornar conflito. Conferir no
plano nomes, IDs, pins, K/R e efeitos antes de confirmar.

1. Aquisição/revalidação de executor, DATA, anterior, DLLs, parent/ancestrais,
   token, SCM e recursos; declaração/confirmação atribuída do plano imutável.
2. Consumo único antes dos efeitos; criação FILE_CREATE exclusiva de candidato
   com DACL protegida BA/SY full/AU RX. Copiar 784896 bytes da DATA retida,
   verificar bytes/hash/identidade/proteção e flush.
3. Rename anterior para .previous sem overwrite, RootDirectory=NULL.
4. Rename candidato para nome canônico sem overwrite. Readback original do
   novo objeto, bytes/hash/SD/SCM STOPPED/PID0 e anterior preservado.
5. Fechamento uma vez quando factual. Falha/close incerto preserva objetos,
   coorte, reservas e retenção UNKNOWN; nenhum rollback/cleanup automático.

Preservar todos os candidatos e .previous anteriores, os originais e decisões
consumidas; inclusive candidato742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b,
ID417614100:1769472:537456. Preservar os novos originais417614100:4587520:222537 e417614100:1376256:285871
e a decisão/sessão e3c579ff64a62aabedb34bd06d914f50184bdadeca7f4b09a28c893aab595f69
consumida, sem recriar cópias. Preservar também os originais417614100:1114112:290114 e417614100:1048576:290115
e sessão bd8bd383513415fa21470f8d324f985e0343cf651c53fc3204f665b6d0744447
consumida. Preservar também a retenção histórica do observador
e das sessões; retorno do wrapper/update posterior não comprova sua resolução.
Não modificar, excluir, adotar, restaurar ou limpar objetos históricos.
UNKNOWN material/conflito/lock que impeça o ato => STOP, sem retry ou refund.
Pode haver intervalo sem canônico e falha parcial entre renames. Não presumir
atomicidade ou recuperação. DLLs/updater/SCM/ProgramData/CA/CURRENT intactos.

## Recursos, STOP e riscos U4

K esperado do MESMO updater protegido:2027314 bytes, Selected3145728 inalterado.
Não usar o censo do servidor recém-compilado como K do executor já instalado.
Divergência de K no plano => STOP. Buffer existente, sem novo subsistema.
Físico/commit disponíveis>=K, margem fresca antes de cada submissão mutante.
R=ceil(784896/U)*U com geometria real adquirida/congelada/revalidada e overflow
checado. Exemplo U4096: R786432 bytes; não é observação atual. Caller-free e
actual-free>=R antes de criar e payload restante arredondado antes de escrever.
Não importar 2R/B0–B8/R0–R9/G3. Retidos continuam ocupando espaço/reservas.
Metadados filesystem, loader/CRT/wrapper/alocador/SD/hash/SCM/RPC/cache/flush/
rename/kernel permanecem opacos; nenhuma margem prova máximos ou latência.

Revalidar imediatamente antes de cada efeito e após confirmação/outra chamada;
sem TTL inventado. Esc cancela; Ctrl+C/Break sinaliza Stop do owner, não
ControlService. Stop, identidade/proteção/SCM/recursos insuficientes/UNKNOWN
material impedem próximo efeito. I/O pode não cancelar imediatamente.
Sem quotas ampliadas neste domínio, ACL/share relaxado, kill, retry ou rollback.
Retenção histórica e quiescência OS permanecem UNKNOWN.

## Procedimento e decisão solicitada

update-g3-p1-runtime-sid-image.MANUAL.CANDIDATE.ps1, SHA256 **adb80e3f24fa5ebf4b55914c43fc505c46ce223e4d07f5f0e44a8e1a51a773ff**.
PowerShell5.1 Desktop x64 legítimo, sem autoelevação. Seis helpers preservados,
mesmos 16 argumentos e UMA chamada ao updater protegido. Wrapper retém DATA/
executor/DLLs/cadeias; fecha lock readonly do anterior antes da chamada para
não impedir DELETE do owner. O owner readquire e confere IDs/pins/proteção;
não alegar continuidade no intervalo desses locks. Falha de close nega chamada.
AST analisada offline, nenhuma definição nativa ou preflight executada.

Solicita-se seleção explícita dos bytes/hash acima e autorização de UMA execução
manual exclusivamente deste pedido, reconhecendo riscos de custos opacos,
substituição parcial, intervalo sem EXE, retenção persistente e ausência de
rollback. Após aprovação registrar decisão/hash real e preparar comando exato.
Este candidato não concede autorização operacional e não deve ser executado.

Reconciliação futura exige saída integral, primeira falha/consumo/efeitos/dois
renames, anterior retido, novo ID/hash/bytes/SD/readback, SCM STOPPED e fechamento.
Resultado inconclusivo permanece UNKNOWN; não repetir. Depois de UPDATE
qualificado, nova sessão P1 E/Q/S com originais e aprovação independentes;
não reutilizar a sessão consumida nem inferir readiness produtiva.

Proibidos coleta, endpoint/SCQ/Start/Stop, UPDATE adicional, limpeza/exclusão,
restauração/rollback, alterações SCM/ACL, CA/CURRENT, rede operacional,
TPM/NV/Q4, refund ou autoridades produtivas. Preservar HEAD/WIP/históricos/pins,
EXEs anteriores, PST0.6.4, DENY e UNKNOWN.
