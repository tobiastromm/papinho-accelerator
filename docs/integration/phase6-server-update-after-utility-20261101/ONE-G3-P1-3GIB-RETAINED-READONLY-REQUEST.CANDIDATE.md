# Pedido — uma observação read-only da disposição retida G3-P1 3GiB

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-02.
Uma chamada manual em OUTRO Windows PowerShell 5.1 x64, no host TCORP-TOBIAS,
operador SID S-1-5-21-785699550-1440458426-1682014700-1108, console interativo.
Não fechar ou interromper a janela retida existente.

Objetivo: observar estado/configuração/SID type do serviço PapinhoAccelerator e
verificar se o PID148 ainda corresponde à criação original. Não é identificação
do predicado interno5, prova de identidade integral/token, fechamento ou recuperação.
Origem: sessão2a34a063563cd9fdf12ec5742b6c6d45ede377fa4501b104779c33205e31cfc0,
tentativa241a0c96391265c83a5482c3f178c4f0367e68908b4d80c10eea6b994caf8082,
SERVICE ERROR5/CLOSE0/RETAINED1, Sleep(INFINITE), wrapper sem retorno.

Operações exatas, após integridade/contexto e confirmação manual independente:
1. sc.exe queryex PapinhoAccelerator.
2. sc.exe qc PapinhoAccelerator.
3. sc.exe qsidtype PapinhoAccelerator.
4. Get-Process -Id148; ler StartTime e, somente com FILETIME original igual,
HasExited; Dispose apenas do objeto de observação deste script, nunca do
processo observado. Criação esperada high31288046/low3049392229, não PID isolado.
Erro em consulta SCM interrompe restante; erro/ausência/reuso PID=>UNKNOWN/DENY.
Não enumerar módulos, token, handles, conteúdo de memória ou command line.
Nenhuma espera, loop/retry, endpoint, SCQ, Start/Stop ou mutação.

Ferramentas do Windows e .NET podem adquirir recursos locais/RPC internamente;
custos e handles internos opacos, sem promessa de custo máximo ou quiescência.
Saída textual no console, sem arquivo protegido criado. Não transporta G3 para
este observador nem transforma a observação em capacidade/autoridade. Não é
snapshot atômico das três consultas; estado pode mudar entre leituras.
Get-Process em processo diferente não prova estado interno nem autenticidade
bilateral. Erro5 histórico continua UNKNOWN mesmo se serviço estiver STOPPED.

SCM esperado: OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, conta
NT SERVICE\PapinhoAccelerator, SID_UNRESTRICTED1, comando fechado:
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.
Não corrigir nenhuma divergência. Imagem selecionada786944bytes,
ID417614100:4718592:287758, SHA256
bf2f8c67a3f5de3a5a4b88f28b69eb8573aec04db1df8e36c7907f999a6a748a;
nenhuma nova abertura/hash da imagem é parte desta coleta.

Arquivo observe-g3-p1-3gib-retained.MANUAL.CANDIDATE.ps1
SHA256 ec4824cd671066221dfdde93b9e5b7045ad6f99b109f1b6e7b6c42874729dd1c
Script verifica hash próprio/pedido antes de consultas. O hash não é autoridade;
exige aprovação específica deste pedido e confirmação OBSERVE_RETAINED_G3_P1_ONCE.
Nenhum comando de execução emitido antes dessa aprovação. Uma única invocação,
sem retry mesmo com falha/cancelamento. Não reutiliza autorização E/Q/S consumida.

Preservar processo/console/referências, objetos/originais/históricos/pins/EXEs,
HEAD/WIP/PST0.6.4, UNKNOWN e DENY produtivo. Não autoriza fechar a janela,
kill/Ctrl+C/Stop, UPDATE/staging/SCM-ACL/CA-CURRENT/rede/TPM-NV-Q4/limpeza/refund/
rollback/restauração/recuperação. Retenção e OS UNKNOWN permanecem.
Após saída reconciliar primeira falha, sequência temporal, configuração/estado,
criação original ou reuso/ausência PID, preservando desconhecidos. Consulta não
libera retenção nem autoriza nova sessão. Qualificação: parsing AST e inspeção
estática da lista de operações; nenhum script/coleta foi executado.
