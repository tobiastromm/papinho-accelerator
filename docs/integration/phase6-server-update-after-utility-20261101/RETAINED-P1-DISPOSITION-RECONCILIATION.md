# Disposição P1 — saída observada, sessão abandonada

Proprietário executou uma vez o pedido18d4d848... aprovado. first0,
consumed/submitted/exit_observed True, wrapper exit0. Sem retry.
Prova localizada de saída da instância148/criação134381137375135845 pelo handle
validado. Código de abandono0xE0000001 e SCM STOPPED/PID0 antes/depois são gates
do procedimento fixo: first0 implica sucesso nesse caminho qualificado, mas não
há raw status/code na saída e não se declara consulta fresca futura.
Referências do observador sem falha registrada; não equivale a close SCQ.

Outra janela terminou com P1_SESSION_FAILED_OR_UNKNOWN_NO_RETRY, como esperado
após abandono. Linha com o native exit numérico não foi fornecida; não inventar
esse valor como observado. Erro5 original continua primeira falha histórica e
UNKNOWN em seu predicado. Sessão original FAILED/ABANDONED, nunca QUALIFIED.

O impedimento específico de controlador148 ainda vivo foi removido por saída
observada. Isso não resolve retenção histórica, objetos de outros processos,
quiescência OS, receipt/close ou capacidade. Originais/EXEs/retidos preservados;
sem cleanup/refund. Não repetir a sessão ou a disposição.

Próximo passo: pedido independente de UPDATE U1–U7/U2 para candidato qualificado
787968/a2626bf759cfe5e10f7add6515be2e2b8523c8cdc86bf6d49773010a44bf8a94.
Nenhum UPDATE autorizado por esta saída. Gates frescos de original/parent/
executor/DATA/SCM/recursos/Stop continuam podendo negar em caso de conflito.
