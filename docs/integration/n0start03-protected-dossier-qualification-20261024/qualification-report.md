# Qualificação preparatória N0START-03 — parcial, colocação STOP

HEAD main 913c90cf2328f7939d8f5b927640601903ac6e91. Baseline fixa autoridades e ferramentas; nenhum código funcional, launcher, pin ou ADR accepted foi alterado. O binding anterior continua qualificado offline e fechado operacionalmente por pins N2 vazios/IDs não qualificados.

## Evidência e limites

A consulta original do arquivo Windows foi read-only, com handle retido e hashes antes/depois coincidentes. conhost.exe: ID417614100:327680:1591236,867840bytes,2links, SHA b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963. RAW1 mask7 nativo248bytes/control9404 SHA7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0. Catálogo externo Valid/Catalog/IsOSBinary=True. Ferramentas e saída integral em readonly-constituents-result.json. SACL/mandatory labels UNKNOWN; arquivo aberto não é prova de imagem mapeada nem de associação autenticada ao console. Política de rede da verificação de catálogo não foi qualificada como cache-only; nenhum teste de rede operacional foi realizado.

Raiz protegida: STOP por acesso negado no contexto não administrativo do coletor. C:\ e ProgramData revalidados, demais componentes não observados. Referências próprias desta consulta e do arquivo Windows fechadas. Não há evidência de alteração de ACL. Usar a consulta manual existente do placement-plan-STOP.md; não remediar permissões.

## Matriz de binding e freshness

| Categoria | Preparado/observado | Obrigação futura |
|---|---|---|
| Arquivo Windows estático | Identidade, conteúdo, RAW1 e catálogo reobservados nesta coleta | Revalidar mesmo objeto/aplicabilidade; duas amostras não são prova contínua |
| Específico da tentativa | V2 tentativa03/operação03, novos nomes, hashes atuais de runtime/qualificador/launcher e decisões | Colocação protegida exclusiva, recibo e IDs nativos novos |
| Host/boot/serviço/configuração | Pins normativos esperados no candidato, não promovidos a fatos atuais | Consulta factual autorizada, SCM readback/recovery/triggers e estado conforme |
| Operador/token/plano/confirmação | Operador requerido, futura tupla e aprovação exata ausentes | Binding nativo fresco, confirmação explícita e anti-replay |
| Auxiliar | Nenhum PID/creation futuro antecipado | Identidade/proveniência/lifetime/revalidação, N7 específico pendente |
| Recursos | Nenhuma amostra operacional nova | Métricas atuais, freshness60s/1s, orçamento independente/headroom |

O registro V2 encadeia SHA da nova evidência. Candidatos não reutilizam aprovação antiga nem claims consumidos. Planos/desafios/tokens futuros permanecem ausentes. Aprovação de colocação, aceitação N7 e aprovação operacional são decisões separadas.

## Obrigações históricas e ED11

A: referências próprias fechadas no alcance dos recibos. B: StartService não chamado pela tentativa histórica. C: processos administrativos compartilhados não recebem extinção/lifetime por inferência. D: intent consumido, imagem/sink e ImagePath preservados. E: kernel/cache/stack/lifetimes opacos UNKNOWN retidos. F: P3/P5 distingue marcador administrativo de reservas reais; não há refund nem reutilização de allowance. H0–H9 permite avaliar sem teto retrospectivo universal SOMENTE dos resíduos opacos; recurso controlado com lifetime incerto continua DENY.

Orçamento independente REQUIRED_NOT_GRANTED. Quatro papéis; nativos128handles cada; conjunto administrativo1024handles/256MiB/64threads/32MiB; qualificador/serviço64MiB/16threads/8MiB; freshness60s/1s; headroom5/9GiB e gates existentes preservados; HOST_CREDITS=NONE_GRANTED. Patch de console só tem redução em fixture, não crédito ou headroom LAB garantido. Nova admissão não pode sobrepor obrigações incompatíveis. Nenhum preflight/LAB executado.

## Testes executados

Busy diagnostic115 e B+D85 casos por configuração, Selected/Legacy Debug/Release:8 execuções PASS. RAW1 modelo13 casos por configuração:4 PASS com --model. Colocação existente ModelTest:50 checks PASS, somente IO sintético; não abriu/criou inputs. Cobrem identidade/token/boot/arquivo/hash/ACL/replay/ausência de prova/Stop/retenção/UNKNOWN e limites em seus modelos existentes. Não são prova positiva nativa da sucessora. Ver JSONs de resultados.

Quatro invocações iniciais do executável RAW1 sem argumento retornaram2, conforme CLI: erro de invocação do teste, não falha de produto. Preservadas no log; após conferir main, a invocação correta --model passou nas quatro configurações. Nenhuma captura/consulta operacional foi repetida. Fonte nativa inalterada: rebuild não requerido; warnings de novo build não aplicável. Verificações de bytes dos candidatos registradas separadamente; não são parser nativo/admissão factual.

CURRENT_TASK_COMPLETED=NO (candidatos preparados; qualificação contemporânea obrigatória da raiz pendente)
SUCCESSOR_BINDING=OFFLINE_QUALIFIED
PROTECTED_DOSSIER=CANDIDATE_BYTES_PREPARED_PLACEMENT_STOP
N7_OPERATIONAL_ACCEPTANCE=NOT_GRANTED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
SUCCESSOR_ADMISSION=DENY
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=PROTECTED_ROOT_NOT_REVALIDATED_ACCESS_DENIED_CURRENT_CONTEXT
NEXT=SUCCESSOR_PROTECTED_INPUT_PLACEMENT_APPROVAL_AFTER_AUTHORIZED_ROOT_READONLY_RESULT
