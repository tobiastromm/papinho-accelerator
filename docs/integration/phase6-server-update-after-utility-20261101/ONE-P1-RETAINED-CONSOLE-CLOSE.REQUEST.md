# Pedido específico — encerrar manualmente o console do observador retido

2026-11-02, America/Sao_Paulo. CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED.
Não constitui nova coleta, retry ou recuperação autoritativa. Não executar
este encerramento com a autorização já consumida da observação.

## Alvo e ação proposta, sem comando de seleção genérica

Uma única ação manual do proprietário: fechar pelo X somente a janela original
`Administrador: Windows PowerShell` mostrada na captura enviada, na qual foi
executado observe-p1-interval.MANUAL.CANDIDATE.ps1 e permanecem as linhas:

`P1_INTERVAL cut=4 known=1 handles=128 error=0 slots=0x03`

`P1_INTERVAL_RESULT first=1816 first_cut=4 close_known=0 identity_checked=1 final_guard=0 sample_bytes=144 observation_bytes=784`

A captura deve corresponder à MESMA janela ainda aberta e sem prompt retornado.
Não selecionar janela por título isolado. Não usar taskkill/Stop-Process por nome,
PID inferido, wildcard, árvore inteira ou script automático. PID não coletado;
nenhuma seleção por PID é autorizada por este pedido. Não realizar coleta extra.
Se a janela não puder ser identificada com certeza pela execução e linhas acima,
ou houver outras tarefas conhecidas usando esse mesmo console, STOP sem fechar.

Host esperado TCORP-TOBIAS; proprietário original
S-1-5-21-785699550-1440458426-1682014700-1108. Não executar novamente o observador.
EXE workspace observado779776bytes/hash
7b6d5e500723ba5db2f1d337cb1ca55e9fa7b3c5588ca6b31d4c7d81e693e726,
script22906613fd4ff0e9206e830160caac7109225d23c845c7aadf6020ec07e5ffd9;
pedido consumidoe6644ce7505c6ae54efd4daadd51b98b3b254a397d83ae77348b472db3bd345d.
Esses pins documentam o contexto histórico; não são seleção atual de processo.

## Efeito e risco residual para decisão explícita

Fechar um console envia CTRL_CLOSE_EVENT aos processos anexados a ele, incluindo
PowerShell pai, wrapper e observador; não é um sinal exclusivamente ao EXE.
Não foi enumerado o conjunto atual de processos anexados. A decisão é aceitar
esse escopo de console específico, não fingir prova de isolamento por PID.
Se esse risco não for aceito, manter a janela sem intervenção e o pedido não
aprovado. Não iniciar uma campanha adicional de coleta por inferência.

O handler próprio do projeto trata CTRL_C/CTRL_BREAK mas não CTRL_CLOSE;
o Windows encaminha o evento/terminação conforme o contrato de console.
Não prometer execução do finally do wrapper nem cleanup voluntário do owner.
Não converter desaparecimento da janela em close_known1, refund, prova de
quiescência OS ou resolução de qualquer retenção histórica.

O observador não entrou em decisão, endpoint, SCQ, SCM ou Start; não criou
arquivo-alvo e não fez escrita/rename. A terminação encerra processos desse
console e perde suas referências/memória/saída ainda não emitida; o tratamento
OS dos recursos internos permanece não demonstrado. Nenhuma exclusão,
restauração, alteração de ACL/SCM, UPDATE, Stop de serviço, cleanup filesystem,
CA/CURRENT/rede/TPM/NV/Q4/refund fica autorizado. Nenhum artefato histórico será
apagado ou autorização rearmada. Guard124/cap128/G3/PST0.6.4 inalterados.

## Necessidade e reconciliação

O branch da imagem antiga marcou unresolved pela presença do SD temporário e
estacionou em Sleep(INFINITE); não há comando legítimo de retomada/cleanup
in-process exposto. A correção offline não pode ser injetada nessa instância.
CTRL+C só marca Stop no handler próprio; não fornece um wake do Sleep(INFINITE).
Por isso não orientar Ctrl+C repetido, alteração de memória ou reuso do owner.

G5 accepted não concede autoridade implícita de terminar processo; P6 preserva
UNKNOWN. Este pedido solicita somente decisão operacional específica de fechar
o console identificado, reconhecendo a interrupção sem prova de close nativo.
Não muda política/retention nem aprova nova observação ou Start.

Após autorização futura e ação manual única, registrar apenas o relato factual
de fechamento da janela/interrupção. A autorização da observação continua
consumida. Retenção histórica, nativeClose da coorte e quiescência OS continuam
UNKNOWN na ausência de evidência própria. Sem retry, comandos adicionais ou
reconciliação de objetos por inferência.

Referência primária:
https://learn.microsoft.com/en-us/windows/console/handlerroutine
