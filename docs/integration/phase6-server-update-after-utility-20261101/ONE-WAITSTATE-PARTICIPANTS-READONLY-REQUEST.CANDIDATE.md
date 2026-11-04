# Uma consulta manual read-only — participantes WAIT_STATE

Procedimento transitório existente; uma chamada sem retry. Não é E/Q/S, UPDATE
ou disposição. Selecionado sob autorização atual do proprietário para prosseguir
com a preparação e enviar comandos; nenhuma consulta executada pelo agente.

Sessão consumida5bf09792d603754b94e9117cb6514895081f16414c48482e731108ad6ba29618,
tentativa44da67d2ca5029977b4fbbde3e226f88ec4b0e2929e2bbad9c82848bfe1bc571.
Espera receipt1052 operation2/pending1/offset0, timeout1460/212/retained1.
Isso não identifica rejeição do receptor. Corrigida offline dependência circular
de pre_readiness na seleção P1; não prova causa histórica nem permite UPDATE agora.

Controlador fixo PID12976/criação134381401233458253, token200516335:0,
auth570063:0/modified200515986:0/groups17, User originalSID1108.
Runtime PID não impresso e NÃO inventado. Imagem instalada805376/ID417614100:4194304:339059/
pin044f10de34010b83075976ba39599a92381bce483ade9f780960b0fcc73010b1.

Reutiliza observador anterior com substituições somente do alvo fixo/nome/confirmador.
Host TCORP-TOBIAS, PowerShell5.1Desktop AMD64, administrador SID
S-1-5-21-785699550-1440458426-1682014700-1108 original sem impersonação e console.
Confirmar manualmente OBSERVE_WAITSTATE_CURRENT_PARTICIPANTS_ONCE; nenhuma resposta
automática. Consumo antes da primeira aquisição; nenhuma retomada após falha.

SCM CONNECT1/serviço PapinhoAccelerator QUERY_STATUS4, QueryServiceStatusEx36bytes.
Exige OWN_PROCESS16/START_PENDING2/PIDnãozero distinto do controlador/códigos0.
Se divergir, imprime estado e nega sem fallback/enumeração/adotar outro processo.
Não apagar códigos para passar. Só então adquirir ambos os processos0x00101000
QUERY_LIMITED_INFORMATION|SYNCHRONIZE, sem herança, tokens QUERY8.
Controlador: PID/criação/estatísticas/User exatos acima e processo vivo. Runtime:
PID atual indicado pelo SCM, criação positiva não anterior ao controlador,
TokenPrimary e SID virtual exato
S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099.
Ambos: imagem C:\Program Files\Papinho\Accelerator\papacc_server.exe,
revalidação dos mesmos handles/criação/TokenId/Auth/Modified/grupos/User. Segunda
consulta exige mesmo PID SCM. Mudança nega sem substituto. Imprime apenas metadados
atuais; caminho/token/PID não constituem autoridade nem atestação original/bilateral.
Não mede hash/SD da imagem mapeada nem inspeciona memória/estado interno do receptor.

Fechar uma vez apenas seis referências do observador: SCM/serviço/2processos/2tokens.
Buffer512 sequencial e strings1024. Custos loader/.NET/compilação/kernel opacos,
sem importar quotas G3/B/R/U ou afirmar máximos nativos. Falha parcial/close incerto
mantém UNKNOWN; observer close não é SCQ/application close/quiescência OS.

Contratos P0–P7/OHI/SSO-B/QUERY/G3-P1/G0–G9/SQB3/Q6 preservados. Sem nova autoridade,
serviço/root/protocolo. Proibidos Start/Stop/kill/UPDATE/ACL/SCM edit/SeDebug/
impersonação/duplicação/ajuste de token/memória/endpoint/CA/CURRENT/rede/TPM/NV/Q4/
limpeza/refund/rollback. Preservar todos os objetos, históricos, PST0.6.4 e DENY.
Autorizações antigas consumidas não se reutilizam. Qualquer disposição posterior
exige plano próprio de objetos exatos e confirmação manual, não inferida pela consulta.

Artefatos SHA256:
- waitstate-participants-readonly.CANDIDATE.cs: b5d7ca68a11a69ba54e31335a37cba029547ae5db7d7f5fd376d98808ac70dae
- observe-waitstate-participants.MANUAL.CANDIDATE.ps1: f89b5d9b223169821252f0fffd555571297516d263e2040d5419864832a30083

Qualificação offline: fonte compilada sem chamar RunFixed; modelo sem P/Invoke,
30cenários com12fronteiras simuladas, cancelamento/replay/PID/token/criação/path/
SCM trocado/close UNKNOWN. AST sem respostas automáticas; nenhuma consulta real.
Aguardar saída integral. Sem retry mesmo se processo já não existir.
