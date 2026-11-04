# Ato S — primeiro Start diagnóstico fechado, finito

Status CANDIDATE / NOT_AUTHORIZED. PERSONAL_LOCAL / TRANSPORT_ONLY.
Integra somente o pacote ONE-FIRST-DIAGNOSTIC-THREE-ACTS-REQUEST.CANDIDATE.md.
Não concede endpoint ou SCQ por implicação.

Serviço existente PapinhoAccelerator, NT SERVICE\PapinhoAccelerator,
WIN32_OWN_PROCESS0x10, DEMAND_START3, ERROR_NORMAL1, SID UNRESTRICTED1,
comando exato `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
SCM deve ser revalidado STOPPED/PID0 e configuração exata antes da chamada.
Sem CreateService, ChangeServiceConfig, ACL ou substituição de imagem.
Canônico762880bytes/417614100:1900544:539611,
SHA2563d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.
Controller original é o mesmo EXE instalado, não workspace/papacc_update.exe.

No máximo UMA StartServiceW com carrier privado/contexto original e deadline
G3. Os quatro argumentos de Start são produzidos pelo owner (`scq-to`, carrier,
`g3-until`, deadline), nunca nonce/PID/token futuro inventado ou aprovação viaargv.
Confirmador existente OWNER_SCQ_DIAGNOSTIC_START, independente dos outros dois;
consumo antes da chamada, inclusive se freshness posterior negar o efeito.

Após chamada, exigir START_PENDING e PID real, adquirir processo/token/criação,
grupos/restrições, SID virtual e imagem/SCM originais; autenticar ambas as pontas.
START_PENDING não é readiness. Após admissão/receipt/close, RUNNING apenas
diagnóstico fechado; a sessão é finita e o caminho exige término normal do
processo/SCM STOPPED/PID0/exit0 e readback. Esse fechamento natural NÃO concede
ControlService Stop. Nenhum Start persistente/produtivo ou promoção automática.

G3 e riscos do pacote preservados. Janela/deadline não renovam; expiração não
garante retorno de API, cancelamento imediato ou fechamento. Falha/Stop/perda
do peer/UNKNOWN retém, podendo estacionar processo para preservar referências;
não terminar/recuperar/refundar por inferência. Se ControlService Stop for
necessário, depende de decisão específica posterior, não incluída aqui.
Sem retry, restart, endpoint extra, CA/CURRENT, KSP, rede operacional, TPM/NV/Q4,
UPDATE, limpeza ou autoridade produtiva.
