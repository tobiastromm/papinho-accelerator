# Pedido operacional condicional — imagem SQB3/SQC3

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Não é comando executável agora.
Uma única execução manual BINARY_UPDATE_CLOSED PERSONAL_LOCAL, sem retry.
BLOQUEADO até disposição legitimamente autorizada/reconciliada dos participantes
WS3M e readback fresco SCM STOPPED/PID0. A aprovação normativa SQB3 não autoriza
TerminateProcess, UPDATE, nova sessão, criação de originais ou cleanup.

## Imagem e escopo

Instalar exclusivamente o EXE Selected/Release que integra a entrega das duas
referências originais somente consulta SQB3 e o close SQC3, com custódia Q6 retida.
Decisão normativa: DECISION-personal-scq-p1-query-references-sqb3-20261102.md,
base aprovado726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c
e errata aprovada b8d73c7541df2363e6f02489d5143a2becec78fdd315003806010a0303bbe810.
Reutilizar U1–U7/U2 accepted (9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234;
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da),
mesmo owner e updater protegido, sem novo mecanismo. Nenhuma mudança de DLL/SCM.

Novo EXE exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe
804352bytes; SHA25611cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815.
Cópia congelada na integração: papacc_server.sqb3-query-804352.exe, mesmos bytes.
Delta de tamanho contra o último canônico conhecido: +5632bytes.
Somente o caminho build acima é passado ao updater; a cópia congelada é evidência.
Adquirir DATA por referência original retida, identidade/continuidade, inspeção de
SD, hash e conteúdo. Não executar DATA, promover sua ACL a autoridade nem exigir
que seu ID futuro seja conhecido antes da aquisição.

Canônico anterior esperado C:\Program Files\Papinho\Accelerator\papacc_server.exe:
798720bytes; ID417614100:4128768:327703;
SHA2566ad6e378ea5ec074ad6af44327b6fd1a22880a8cf18a8f02a3ecc28baf0edb9e.
Esta é a última evidência reconciliada, não um lease ou readback atual.

Executor independente esperado C:\Program Files\Papinho\Accelerator\papacc_update.exe:
104448bytes; ID417614100:30670848:57859;
SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Não atualizar executor. Parent instalado ID417614100:8257536:71209; âncora
histórica417614100:131072:226316. Reaquisição/owner-DACL/ancestrais/ausência de
reparse/continuidade e proteção efetiva obrigatórios. Hash/ID/path isolados não
são autoridade nem atestação de imagem mapeada. Rejeitar aliases/hardlinks.

DLLs apenas conferir, sem modificar:
libssl-3-x64.dll1321472bytes, SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
libcrypto-3-x64.dll7217664bytes, SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Bytes de build conferidos offline; os instalados dependem de gates frescos nativos.

## Sessão anterior e impedimento factual

WS3M9249509a3a942554defa8142bb6b78252e1c5ff9cc39db7bb11082548867653b/
decisão2161c0f085438c1e217d30063df9b5856d3e01733b2d33a8e29a3c1f6891dd69
está consumida, sem retry. Timeout1460/etapa212 e retenção5023, sem close SCQ provado.
Última observação: controlador19860/criação134381311017119087; runtime19836/
criação134381311235391902, SCM START_PENDING. A consulta c51072d5… terminou com
close do observador conhecido; não terminou a aplicação nem resolveu retenção.

Não executar UPDATE enquanto esses participantes estiverem retidos ou sua disposição
for inconclusiva. Eventual término excepcional exige autoridade específica G5 para
os objetos fixos, gates frescos e procedimento próprio; este pedido não o permite.
Não reutilizar disposições antigas. Após eventual saída, application close, bilateral
original, retenção histórica e quiescência OS continuam UNKNOWN. A constatação de
barreira DACL atual não comprova o erro nativo histórico nem sucesso da correção.

SCM original exigido fresco: PapinhoAccelerator, OWN_PROCESS16/DEMAND_START3/
ERROR_NORMAL1, NT SERVICE\PapinhoAccelerator, SID_UNRESTRICTED1;
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.
STOPPED/PID0 antes dos efeitos e após substituição. Não exigir apagamento de1067
histórico nem tratá-lo como sucesso diagnóstico. Sem Stop ou alteração de configuração.

## Efeitos permitidos após autorização futura e remoção legítima do bloqueio

Host TCORP-TOBIAS; operador SID S-1-5-21-785699550-1440458426-1682014700-1108,
contexto administrativo legítimo, console original PowerShell5.1Desktop AMD64.
Declaração e confirmação atribuídas OWNER_BINARY_UPDATE do plano exato. Não fornecer
respostas automaticamente. Consumo único antes de qualquer efeito protegido.

Uma chamada do updater existente com os16argv qualificados. O owner gera uma
única tentativa CSPRNG32 e apresenta os dois leaves exclusivos concretos:
papacc_server.<hex64>.candidate e papacc_server.<hex64>.previous. Não antecipar
nonce/nomes/IDs nem adotar objetos existentes para satisfazer o plano.

1. Revalidar executor/operador/DATA/canônico/DLLs/parent/ancestrais/SCM/recursos.
2. Uma criação exclusiva protegida BA/SY full e AU RX. Copiar804352bytes da DATA
retida, hash/flush/readback, revalidar originais antes dos renames.
3. Rename canônico->previous exclusivo, sem overwrite, RootDirectory=NULL.
4. Rename candidato->canônico exclusivo, sem overwrite; readback de identidade,
conteúdo/proteção, EXE anterior retido e SCM STOPPED/PID0.
5. Fechar uma vez quando demonstrado. Stop/falha parcial/close incerto mantém
objetos/consumo/reservas/UNKNOWN, sem retry/rollback/restauração/delete/limpeza.

Todos os anteriores previous/candidatos/originais e efeitos históricos permanecem.
Não modificar DLLs, updater, ProgramData, configuração SCM, CA ou CURRENT.

## Recursos e risco

U4 próprio do updater, sem importar G3. K2027314bytes do mesmo binário inalterado;
Selected3145728. Físico/commit e margens frescas conforme gates U4 existentes.
R=ceil(804352/U)*U, geometria original revalidada e overflow checado; exemplo
U4096=>806912bytes, cálculo somente, não consulta atual. Espaço livre caller/actual
>=R antes da criação e restante arredondado antes de escrever; retidos ocupam espaço.

Custos loader/CRT/alocador/SD/hash/provider/SCM/RPC/kernel/cache/filesystem/flush/
rename continuam opacos. Não presumir atomicidade, latência, cancelamento ou
rollback. Risco de substituição parcial, intervalo sem canônico e retenção persistente.
Não converter margem em máximo nativo demonstrado. Consulta insuficiente, Stop,
divergência ou UNKNOWN impeditivo => DENY. Nenhum ajuste de permissões ou quotas.

## Procedimento preparado e decisão futura

update-sqb3-image.MANUAL.CANDIDATE.ps1,
SHA25631533c135d19cef6d36badca8ed51fcf446fb61b9a80638c04ade71dbf461db5.
Helpers de leitura idênticos ao wrapper anterior; alteração fechada de nomes, pins,
tamanhos e identidade do canônico. Qualificação AST e22bindings,16argv, uma chamada
futura; wrapper/helpers não executados. O preflight fecha sua referência OLD antes
de DELETE/rename nativo, sem ampliar sharing; close incerto bloqueia a chamada.
O owner adquire/revalida OLD novamente; snapshot não substitui originais.

SQB3-OFFLINE-QUALIFICATION.json contém a qualificação funcional e os hashes finais.
Pedir autorização específica deste documento somente distinguindo o bloqueio anterior.
A decisão real deverá ser registrada em OWNER-SQB3-UPDATE-APPROVAL.txt com seu hash
real após recebida; esse arquivo ainda não existe, não é fabricado nesta preparação.
Nenhum comando de execução é emitido enquanto o bloqueio persistir.

Após execução manual autorizada: reconciliar primeira falha, consumo, criação,
dois renames, retenção anterior, novo ID/hash/SD/readback/SCM/fechamento.
Exit0 não basta. Depois, nova sessão E/Q/S e novos originais, nunca retry de WS3M.
Não exigir PID/token futuro antes do processo; bilateral/receipt/close/readiness
continuam provas posteriores, não garantidas pela imagem/modelo.

Não autoriza automaticamente UPDATE, disposição, coleta, Start/Stop, endpoint/SCQ,
SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4, limpeza/refund ou autoridade produtiva.
Preservar HEAD/WIP/históricos/pins/EXEs/UNKNOWN/DENY/PST0.6.4.
