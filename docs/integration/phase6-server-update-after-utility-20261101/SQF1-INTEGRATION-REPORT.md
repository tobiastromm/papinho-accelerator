# SQF1 — fechamento de integração e qualificação offline

F0–F7/B registrada ACCEPTED antes do código em
DECISION-personal-scq-p1-receiver-first-failure-sqf1-20261102.md. Proposta preservada,
SHA25696b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643.
Não altera accepted anteriores: especialização focal Q2/errata Q6 para negativo
SQF1/56bytes, no mesmo owner/pipe/tentativa, somente P1 PERSONAL_LOCAL TRANSPORT_ONLY.

## Código e comportamento
Transport guarda buffer56 separado e8DWORDs, +88bytes alinhados, zero HANDLEs novos.
Freeze mantém primeira etapa/erro antes da retenção. Primeira resposta negativa
não sobrescreve bootstrap/originais/custódia e não é receipt/close ou autoridade.
Envio: pipe/evento originais, runtime/processo/token retidos/correlacionados, scope
carrier/tentativa/policy/P1, Stop/menor prazo/freshness/recursos. Nenhuma aquisição
pós-falha para diagnosticar; consultas usam referências retidas. Uma WriteFile56,
polls da mesma submissão, sem reenviar resto em short write. Erro/completion
incerta conserva pending/backing e UNKNOWN; erro secundário não altera o primeiro.
Sem prova/pipe/evento/recursos ou com pending/Stop/expiração, envio suprimido.

Controlador distingue SQF1 do SQR1 durante leitura fragmentada, rejeita tamanho/
versão/escopo/attempt/kind/replay/excesso observado; identidade runtime/pipe original
exigida antes de registrar RECEIVER_REPORTED_FIRST_FAILURE. Metadado remoto NÃO é
erro nativo próprio. Código remoto inclusive997/234 nunca controla espera/progresso:
frame válido retorna DENY local. Ack/close/legs/readiness não são concedidos.
Os formatos legados/públicos/persistidos e limites dos outros perfis permanecem.
O novo ramo não afirma ausência de bytes extras não observados no stream: nenhum
frame posterior é aceito e toda a sessão fica negada/retida.

O corpo REAL ingress e predicado wait foram extraídos sem duplicação para
personal_scq_ingress_win32.inc. Guarda unresolved continua bloqueando admissão;
permite SOMENTE espera do negativo já submetido e validado, com referências
originais e deadline. Resultado terminal/Stop/timeout mantém retenção sem close.
Nenhum novo serviço/root/endpoint/canal, callback de autoridade ou comando CLI.

## Catálogo e precisão da evidência
1 contexto/guard anterior ao ator (pode não haver envio);2captura runtime;
3policy;4resource;5open QUERY;6read bootstrap;7candidates;8paths;
9input-capture/cadeia/SD;10contents/hash;11runtime observation;
12image-data/correlação;13controller-image;14bind-query;15deadline;
16first-prepare;17promote;18continuation;19continuation na fase pré-receipt3.
Etapas18/19 incluem os gates internos dessa chamada, não identificam uma API
interna não capturada. Todos os retornos compostos são native_kind0/raw0;
parser aceita kind1/2 bem-formados, mas não se inventou status nativo no remetente.
Assim um próximo stage9/erro5 ainda poderá exigir análise focal do subpredicado.
Não prometer que stage de componente elucide toda causa nativa.

## Qualificação e recursos
Quatro builds Selected/Legacy Debug/Release sem warnings;20 testes afetados por
configuração,80pares PASS. Incluem cold_chain positivo/negativo existente,
codec/mutações,16casos envio/14recepção, sentinel997/234, short write/poll incerto,
recursos/Stop/expiração/peer divergente/replay/fragmentos. Teste adicional usa corpo
REAL ingress:18falhas em fronteiras e ordem positiva; predicado wait real com
unresolved, stopped e serviço divergente. Fronteiras de aquisição/validação nesse
teste são mockadas individualmente; não se copia um pacote inteiro de digests
aprovados como aquisição. Os testes independentes dos componentes continuam parte
das regressões. Não é execução nativa do pipeline inteiro ou prova histórica.

Transport1152->1240;driver2336->2424; root Selected375256/known2002304 sob3145728;
Legacy206688/known1833736 sob2097152. Controlados44 e quotas/G3 intactos.
Custos/latência/IRPs/alocadores internos permanecem opacos, não máximos demonstrados.
Nenhum teste de alvo instalado, coleta, UPDATE ou E/Q/S executado pelo agente.

## Imagem e primeiro gate
Candidato Selected/Release 809472bytes/SHA25681a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86,
congelado papacc_server.sqf1-qualified-809472.exe.

Instalado permanece805376/ID417614100:2097152:411753/pin174db00a….
Necessário UPDATE futuro, mas nenhum pedido executável de UPDATE preparado com
disposição atual desconhecida. Última sessão consumida: controlador7400/criação
134381413716812022; runtime PID não impresso; SCM atual não consultado.

Primeiro gate: UMA observação read-only específica pelos mecanismos existentes,
para SCM e controlador fixo/candidato runtime. Pedido
ONE-RECEIVER-PATH-PARTICIPANTS-READONLY-REQUEST.CANDIDATE.md,
SHA2563e32bbc632b580e2b00740d1e0ff33259ee169effde2e379c09c57861af84ad8,
30cenários offline PASS/12fronteiras simuladas. A autorização F0–F7 NÃO o concede.
Depois da evidência, se retido, disposição G5 específica; somente depois preparar
UPDATE U1–U7/U2 e sessão E/Q/S nova com decisões independentes. Sem retry/Stop/kill/
limpeza/refund inferidos. Histórico/OS UNKNOWN, todos os objetos/PST0.6.4/DENY preservados.
