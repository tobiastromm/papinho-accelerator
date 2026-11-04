# Autorização e preparação manual da observação nativa

Task PACC-PHASE6-STAGING-NATIVE-OBSERVATION-AUTHORIZED.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

Lido integralmente o pedido aprovado e conferido seu SHA256
9ffe2080ea29403555f64dff5d3b0f372aab4613f2054ef4c43efeb60765b00e.
Script1935b7dd... e os três arquivos/9.092.096 bytes conferem byte-exatamente.
Parsing estático do script PASS; código do ramo observação conferido: preparação
read-only termina antes de RNG/confirmar/take, sem chamar staging_apply.
Nenhum rebuild, execução de teste ou consulta nativa foi necessário neste handoff;
as evidências qualificadas anteriores permanecem históricas/reutilizadas.

Registro criado uma única vez com CreateNew no diretório documental existente:
OWNER-OBSERVATION-AUTHORIZATION.json. Hash literal
e16494d395e19fbbfaac833e7a9aa6470d62a69dcbe577a0f45237ad9fece90b.
Preservados pedido/script/código/artefatos originais sem alteração. O registro
documenta exatamente a aprovação do proprietário; não é aprovação normativa,
grant de runtime, prova de elegibilidade nativa ou novo ledger. Não registrar
consumo por preparação/handoff; nenhuma saída manual recebida nesta tarefa.

AUTHORIZED-MANUAL-HANDOFF.md contém o comando definitivo, com precheck do script
anterior à chamada e checagem dos três artefatos pelo wrapper anterior ao exe.
Sem execução automática, retry, elevação ou mudança de ExecutionPolicy. Identidade
e guards frescos continuam a cargo do caminho nativo e do contexto manual.

## Reconciliação posterior preparada

Após a saída, registrar evidência derivada separadamente, vinculada ao hash do
registro aprovado, preservando seu literal e os históricos. Conferir:

1. Procedência da saída/pins: fontes e wrapper previstos; hash de disco não
   prova mapeamento da imagem nem identidade autenticada por si só.
2. Estado da única tentativa: não inferir consumo ou ausência de execução da
   falta de receipt. Falha/UNKNOWN não rearma autorização nem permite retry.
3. Primeira falha/localização, predicado e erro nativo; frames alcançados;
   operador/contexto e recursos próprios/quota do volume, sem PID futuro.
4. Tempos/QPC e consistência dos dados, insuficiência versus query indisponível;
   snapshots e picos versus limites/reserva operacional.
5. Close controlado conhecido versus retenção/OS interno UNKNOWN. Ausência de
   output, interrupção ou erro de close não é quiescência/rollback demonstrado.
6. Alcance da coleta: o perfil não mede criação/escrita/flush ou metadados ativos
   do filesystem. A máscara saudável ainda preserva os custos materiais UNKNOWN;
   não preparar pedido de staging como se esses custos estivessem provados.

Obrigatoriedade: DECISION-ed11-preserve-incremental-resource-envelopes-20261012
exige capacidade factual além de quota; DECISION-ed11-windows-3mib-process-profile-
20261020 W4/W5 preserva custos transitivos/retidos e UNKNOWN. Só após examinar a
saída será determinado o próximo fato material e o menor pedido próprio; não há
autoridade para ampliar coleta, escrever, criar nova infraestrutura ou ADR.

Autorização da coleta: REGISTERED. Preparação manual: COMPLETE. Execução/consumo/
evidência: PENDING_OWNER_OUTPUT, sem alegação de sucesso. Admissão de staging
DENY; E21 PENDING; ED11/Phase6 PARTIAL. Nenhum efeito protegido, acesso a segredo
ou chave fixture, alteração accepted, pin de produção, HEAD ou WIP.
