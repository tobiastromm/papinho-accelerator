# Sessão SQB3 — timeout e retenção reconciliados

Saída exata anexada preservada em OWNER-SQB3-SESSION-OUTPUT.txt; resultado em
SQB3-SESSION-RESULT.json. Duas cópias verificadas/fechadas: pedido417614100:917504:409120,
decisão417614100:589824:409139. Três confirmações nativas observadas e Start submetido.
Consumo documental explícito; contadores individuais de E/Q/S não impressos.
A sessão manual foi utilizada e não pode ser repetida, independentemente desses contadores.

SCQ_START_WAIT_FIRST_CAUSE step5/mapped1460, etapa212/start_submission_phase1,
prestart_close0/service_close5023/retained1. Não é falha de handles nem prova de
reabertura administrativa negada. 1460 é timeout da espera original, não NTSTATUS
novo de CreateFile/NtQueryObject. Falta operação/phase/legs/offset do transporte e
primeira rejeição do receptor. Nenhum desses valores pode ser reconstruído do log.

Auditoria focal: progress retorna pending em conexão/poll/delivery/continuação;
wait step5 não distingue qual deles. Ingresso do receptor também pode falhar/reter
antes de conectar ou depois de receber. Não selecionar hipótese como causa sem
observação original. Não aumentar quotas nem modificar a correção SQB3 às cegas.

Controlador4184/criação134381373154465967 é original observado; runtimePID não
impresso. Os campos runtime0 do plano são anteriores a Start e não significam
que o processo real tenha PID0. Estado atual SCM/processos não foi consultado.
Bilateral/receipt/SQC3/custódia terminal/RUNNING diagnóstico não comprovados;
application close, retenção histórica e quiescência OS UNKNOWN.

Preparada uma consulta mínima read-only de SCM e metadados de dois participantes,
reutilizando observador existente, sem SD/elevação/recursos/coleta interna ou produto.
30cenários offline/12edges simuladas/AST/core PASS; fonte real só compilada.
O candidato runtime é selecionado do único serviço fixo e deve continuar o mesmo
entre duas consultas; nunca se torna original por essa observação. Ela NÃO recupera
a causa histórica; apenas governa disposição/identidades atuais. Nenhuma operação real.

Pedido ONE-SQB3-PARTICIPANTS-READONLY-REQUEST.CANDIDATE.md,
SHA256 029503adb6642e1c7cffce9892c8ee1ce469051c0681fec16b82f4e069d4ba2c.
Aguardar autorização específica; nenhum comando executável emitido antes dela.
Não repetir sessão, criar originais, Stop/kill/UPDATE ou limpar. Não reutilizar
G5/UPDATE/consultas anteriores. G3/DENY/UNKNOWN/históricos/PST0.6.4 preservados.
