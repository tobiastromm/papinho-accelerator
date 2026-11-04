# Pedido — UPDATE da imagem qualificada com caller P1

2026-11-01. READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION / NÃO AUTORIZADO.
Solicita seleção exata dos artefatos abaixo e autorização própria para UMA
execução MANUAL BINARY_UPDATE_CLOSED PERSONAL_LOCAL. Não é retry/rearm de ato
consumido. Preparar este pedido não autoriza invocar o procedimento ou o executor.

## Norma e efeito delimitado

U1–U7: docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md, incorporando
UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md SHA256
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234.
U2: docs/adr/DECISION-personal-update-executor-data-u2-20261101.md, incorporando
U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md SHA256
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Originais reconferidos integralmente/byte-exatos e preservados. Sem nova ADR,
especialização ou transporte de B3/R3/G3/P0 para UPDATE.

Atualizar SOMENTE papacc_server.exe para disponibilizar o caller P1 específico
TRANSPORT_ONLY já qualificado offline. Não executar o novo servidor, endpoint,
SCQ ou Start. P0–P7 e QUERY/SQB2/G3 continuam limitados aos respectivos domínios.
O diagnóstico histórico1816, retenção histórica e quiescência OS ficam UNKNOWN.

## Identidade e instalação selecionáveis

Operador: S-1-5-21-785699550-1440458426-1682014700-1108, host TCORP-TOBIAS.
Metadados não são autoridade. Exigir contexto administrativo nativo elegível,
declaração e confirmação atribuída OWNER_BINARY_UPDATE do plano original.
Sem autoelevação, respostas automáticas ou reutilização de autorizações.

Known folder ProgramFiles deve resolver C:\Program Files, conforme U2 UPDATE.
P0 não dispensa esse gate: sua especialização vale somente para TRANSPORT_ONLY.
Parent existente C:\Program Files\Papinho\Accelerator,
ID esperado417614100:8257536:71209. Reter/revalidar ancestrais e objeto original,
owner/DACL efetivos e protegidos, ausência de reparse, substituição e continuidade,
com distinção anchor/child accepted. Nenhuma ACL existente será modificada.

SCM PapinhoAccelerator: WIN32_OWN_PROCESS16, DEMAND_START3, ERROR_NORMAL1,
NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED1, comando exato:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Exigir SERVICE_STOPPED/PID0 e configuração original em gates frescos antes das
mutações e readback. Exit1077 histórico pré-Start não substitui nem invalida
sozinho STOPPED/PID0; não exigir exit0 sem contrato. Snapshot não é fencing
contra outro administrador. Não chamar Start/Stop nem corrigir configuração.

## Arquivos exatos

Executor INDEPENDENTE, não alvo desta substituição:
C:\Program Files\Papinho\Accelerator\papacc_update.exe,
104448bytes, ID417614100:30670848:57859,
SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Preparação/manutenção qualificada: ../phase6-utility-replacement-integration-20261101/UTILITY-UPDATE-RECONCILIATION.md.
Capacidade real UPDATE e correção RootDirectory=NULL já demonstradas nas
reconciliações SERVER-UPDATE e COLD-DIAGNOSTIC-UPDATE desta pasta. Revalidar
proteção estrita U2, processo/token/imagem, dependências e continuidade.
Não executar utilitário do workspace nem atualizar o utilitário protegido.

Anterior canônico esperado:
C:\Program Files\Papinho\Accelerator\papacc_server.exe,
763904bytes, ID417614100:1835008:134613,
SHA2567d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
Proveniência: COLD-DIAGNOSTIC-UPDATE-RECONCILIATION.md e saída manual
OWNER-MANUAL-COLD-DIAGNOSTIC-UPDATE-OUTPUT.txt,
SHA25682c982e44ea88460efb0289a964ecc93935f5a0d1ca08597fd9fc0892f6e72a9.

Novo EXE do workspace SOMENTE DATA; nunca executar/carregar como controller:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
772608bytes, SHA256
54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50.
P1-ORIGINAL-CALLER-INTEGRATION.md: quatro builds Selected/Legacy Debug/Release
sem warnings e60 pares CTest PASS. Hash/tamanho reconferidos nesta preparação.
Delta instalado+8704bytes. Não substituir por build posterior sem nova conferência.
Sua identidade factual será capturada pelo handle original na futura invocação;
não inventar ID de DATA ou do candidato exclusivo futuro.

DLLs no parent protegido, apenas leitura/validação; sem cópia, alteração ou rename:

| Arquivo | Bytes | SHA256 |
| --- | ---: | --- |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

DLLs do build reconferidas idênticas aos pins. Arquivos instalados/SD/identidades
e SCM acima são BASELINE HISTÓRICA esperada, não consultas atuais. Nenhuma coleta
nativa/ACL/SCM foi executada nesta preparação. Wrapper e owner devem readquirir
todos os originais atuais; divergência/UNKNOWN material nega. Pins em disco
não atestam páginas mapeadas ou continuidade histórica não observada.

## Disposição e retenção históricas preservadas

Não tocar/adotar/limpar nenhum objeto anterior, inclusive:

- papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.candidate,
  758272bytes, ID histórico417614100:1769472:537456,
  SHA2567612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f;
  coorte/fechamento/créditos UNKNOWN.
- papacc_server.638a92da338de9d8c3683be1b14e64d8f67cfdf89cd57fb9e0848d8dc3608ce6.previous,
  571392bytes, SHA256c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf.
- papacc_update.fcc29b1584d97bef8923a5fb7e7b5f4857316e70b157fcbbd50239525259230d.previous,
  104448bytes, SHA2566c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87.
- papacc_server.ae5730e2344b321bea92b110ce5335257c632b13f5e0b60877d0772e8c667558.previous,
  762880bytes, ID histórico417614100:1900544:539611,
  SHA2563d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.

Lista histórica, sem nova enumeração e sem alegar que seja exaustiva/atual.
Fechamento saudável do último UPDATE não resolve coortes antigas ou OS.
Não recuperar/reutilizar handles, créditos ou reservas históricos incertos.
Se UNKNOWN impedir prova obrigatória de identidade, capacidade, exclusividade,
retenção ou reserva da nova invocação, STOP: este pedido não o dispensa.

## Única tentativa e efeitos autorizáveis

Após autorização específica, o owner existente gera uma tentativa CSPRNG32bytes,
congela plano e apresenta os dois leaves exatos ANTES da confirmação/consumo:
papacc_server.<tentativa hex64>.candidate e papacc_server.<mesma tentativa>.previous.
Binding tardio nativo já qualificado, não placeholder de comando nem nome
escolhido para contornar conflito. Não adotar nome existente ou repetir falha.

1. Readquisição read-only do executor, DATA, anterior, DLLs, ancestrais, token,
   SCM, recursos e volume; referências originais retidas e revalidadas.
2. Declaração/confirmação BINARY_UPDATE_CLOSED e consumo irrevogável ANTES de
   qualquer criação. Autorização manual anterior permanece consumida.
3. Uma criação exclusiva FILE_CREATE no parent protegido, DACL nova protegida
   BA/SY full e AU RX. Copiar somente772608bytes da DATA retida, em buffer
   limitado existente, verificar hash independente/bytes/identidade/SD e flush.
4. Rename do EXE anterior para .previous exclusivo, sem overwrite, preservando
   seu objeto; RootDirectory=NULL e readback qualificados.
5. Rename do candidato verificado para papacc_server.exe, sem overwrite;
   readback relativo ao parent original, identidade nova/hash/tamanho/proteção,
   SCM ainda parado/configuração original e anterior retido.
6. Fechamento factual uma vez; falha/incerteza retém objetos/coorte/créditos.

No máximo um arquivo criado e dois renames. Não sobrescrever bytes do anterior,
editar ACL existente, excluir, restaurar, completar falha ou implementar rollback.
Wrapper retém executor/DATA/DLLs/cadeias sem share DELETE; somente parent de
criação permite share WRITE. Lock readonly do anterior é fechado ANTES da
chamada ao executor; owner o readquire com direitos de rename e provas nativas.
Não alegar continuidade no intervalo. Falha do close do preflight nega chamada.

## Recursos e riscos U4

Executará o MESMO utilitário084aa..., não o novo servidor DATA. Seu K compilado
e confirmado no último UPDATE=2027314bytes, orçamento Selected3145728 inalterado.
Código do executor e funções do wrapper não mudam; payload maior usa o mesmo
buffer limitado. Censo2002000 do servidor novo não é K do utilitário nem soma
de reserva: esse arquivo não será executado durante UPDATE.
Exigir física/commit disponíveis>=K em gates frescos antes das mutações.
Essa margem nominal fraca não é reserva física ou máximo de provider/kernel.

R=ceil(772608/U)*U, U da geometria real congelada/revalidada; aritmética verificada.
Com U4096, R774144bytes. Exigir caller-free/actual-free>=R antes da criação e
payload restante arredondado antes das escritas. O exemplo não substitui amostra
atual. Retidos existentes não são novas cópias; espaço/metadados opacos não são
provados por R. Preservar reservas/multiplicidades e contar todos os handles.
Guard124/cap128/G3 permanecem em SCQ/Start; não são transplantados ao UPDATE.

Freshness por evento: revalidar imediatamente antes de cada efeito, após
confirmação e chamadas anteriores; nenhum TTL/duração arbitrário novo.
Loader/CRT/wrapper/alocador/hash/SD/RPC/SCM/filesystem/cache/metadados/flush/
rename/kernel/retidos opacos continuam UNKNOWN sob U4. Risco de exaustão,
bloqueio, latência sem máximo e variação entre amostras explicitamente assumido
somente se o proprietário aprovar este pedido. Monitoramento não garante teto.

Esc cancela confirmação; Ctrl+C/Ctrl+Break sinaliza Stop do controlador, nunca
ControlService. Antes de cada criação/escrita/flush/rename: Stop, contexto,
originais, SCM e recursos. Divergência, replay, conflito, falha ou UNKNOWN material
impede próximo efeito. Não elevar direitos, matar processo ou afrouxar sharing.
Stop não cancela I/O já submetido. Não presumir atomicidade entre renames,
rollback ou durabilidade de metadados. Falha parcial pode deixar candidato
incompleto, intervalo sem EXE canônico ou canônico novo sem readback.
Reter sem exclusão, limpeza, restauração, refund, timeout cleanup ou retry.

## Procedimento e evidência futura

update-p1-image.MANUAL.CANDIDATE.ps1, SHA256
89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9.
Windows PowerShell5.1 Desktop x64, NoProfile, contexto legítimo; nenhuma
autoelevação. Exige ExpectedScriptSha256/ExpectedRequestSha256 reais, verifica
integridade/norma/contexto/cinco arquivos e IDs esperados antes da ÚNICA chamada
ao utilitário protegido com os mesmos16argumentos. Nenhuma resposta fornecida.
Ainda NÃO EXECUTAR. Após aprovação byte-exata será preparado o comando manual
literal com hashes reais da decisão/procedimento/pedido, sem placeholders.
ExecutionPolicy, se necessário, só no processo filho; sem alteração persistente/GPO.

Qualificação offline estática PASS: AST Windows PowerShell5.1, oito substituições
fechadas de pins/bytes/ID/nome do pedido/banner, seis funções helper byte-exatas,
mesma chamada nativa única; procedimento e helpers NÃO invocados. Builds/testes
inalterados da imagem reutilizados, não repetidos. Evidência:
P1-IMAGE-UPDATE-OFFLINE-EVIDENCE.json. Integridade não concede autoridade.

Reconciliação requer saída integral, first0, consumed1, created1, renames3
(bitmask dos dois renames), canonical_verified_closed1, close_known1 e
LIMITED_UPDATE_VERIFIED_CLOSED; confirmar identidade/hash/proteção e retenção
do anterior, novo canônico e SCM parado/configuração original. Exit0/wrapperclose
isolados não bastam. UNKNOWN histórico/OS permanece mesmo após sucesso.
Invocar o procedimento utiliza a única tentativa manual; consumed interno0 ou
erro de shell não autoriza retry. Não repetir a operação após falha.

Após sucesso qualificado, nova identidade real permitirá preparar NOVOS originais
P1 e nova sessão E/Q/S, com seleção DATA no console e três confirmações próprias.
Este pedido NÃO emite nem reutiliza essas decisões. PID/token do runtime,
bilateral/receipt/close e recursos G3 ainda exigirão provas nos respectivos gates.
Não antecipar readiness diagnóstica nem produtiva.

Proibidos: update de utilitário/DLLs, outros arquivos/diretórios, SCM/ACL,
Start/Stop, endpoint/SCQ, CA/CURRENT/ProgramData, rede, TPM/NV/Q4, limpeza/refund,
restauração/rollback/exclusão/recuperação. Preservar HEAD/WIP, históricos/pins,
originais/EXEs anteriores, reservas/retidos/atos consumidos e PST0.6.4.
DENY até autorização específica. E21 PENDING; ED11/Phase6 PARTIAL.
