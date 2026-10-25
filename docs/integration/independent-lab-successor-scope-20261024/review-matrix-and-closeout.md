# Revisão focal L0–L9 e closeout

Proposta normativa somente. Revisão integral do agente executante, sem alegar parecer externo independente, implementação ou prova de capacidade. A proposta define requisitos completos de UMA sucessora sem gerar sua identidade. Isso evita tratar a aceitação do escopo como aceite de uma tupla inexistente.

## Matriz de revisão

| Cláusula | Requisito/resultado |
|---|---|
| L0 |Mesmo hostboot/OS/config/serviço/conta/SID/operador e pai como limites a revalidar; especialização de N0/B0 apenas nos campos de uma sucessora; P3/P5/P6 preservados.|
| L1 |Requisitos para identidade/intent/imagem/sink/dossiê/aprovação/pins/recorder novos; nada gerado. Anexo normativo exato aprovado pode ocorrer em etapas com gates pendentes explícitos, sem hash fictício ou autoridade automática.|
| L2 |Mesmo owner/mutex; anti-replay permanece; nenhum segundo owner, registry ou mutex por tentativa. Código atual não é declarado pronto.|
| L3 |P3 aplicado documentalmente; consumido/efeitos/quiescent0/UNKNOWN/reservas reais intactos; sem refund/segunda liberação.|
| L4 |Texto explícito de risco N7 proposto para UMA sucessora; aceitação anterior não estendida. Falta N2 nega mesmo com risco aceito.|
| L5 |Orçamento independente obrigatório; não garantido por RAM livre, fixture ou marcador disposto. Caps/freshness/headroom mantidos. Cobertura de UNKNOWN cumulativo não é implicitamente aprovada.|
| L6 |Novo binding protegido; provas estáticas antigas só referenciadas sob revisão/revalidação, nunca claims reaproveitadas. RAW1 original, SACL UNKNOWN, sem ACL remediation.|
| L7 |ImagePath histórico é pré-estado, não imagem nova adotada. Rebind só por aprovação própria; novos nomes e CREATE_NEW/FILE_CREATE; no retry após efeito parcial.|
| L8 |Stop/quiescência/refs separados; exactly-once/Model E/ED08 mantidos. Implementação/colocação/qualificação/execução exigem gates próprios.|
| L9 |PROPOSED até aprovação; exemplos não são testes. Norma de escopo não seleciona GUID ou aceita binários futuros automaticamente.|

## Relação com contratos vigentes

N0/B0 fixavam apenas75330d11...; L0/L1 propõem a possibilidade delimitada de vincular UMA identidade sucessora pelo mesmo processo normativo, não admissão genérica por string arbitrária. B4.A/B4.B.6 mantêm vinculação exata, novos pins e invalidação. Mudanças concretas de binário/leaf/política devem estar nos anexos aprovados e na implementação qualificada, sem editar retroativamente contratos.

P3 está registrado com hash892d51...5fad; P5 autoriza avaliação sem refund e P6 exige nova identidade/budget/autoridade. L3/L5 preservam esse efeito: não manter o marcador como bloqueio eterno nem convertê-lo em saldo. Reservas reais/UNKNOWN continuam retidos. Nenhuma reserva materializada é inventada para justificar a política, nem se presume que todas são zero.

N7 aceita classificação limitada do auxiliar no escopo antigo; L4 prepara sua aceitação explícita para a sucessora, sujeita à vinculação exata. N7 e o risco administrativo P7 não aprovam automaticamente sobreposição de custos. L5 exige uma revisão target-specific dessa categoria, se necessária, sem dispensar bound rígido, identidade, quota ou lifetime controlado.

ED11 mantém todas as cotas. Conjunto administrativo256MiB/1024handles/64threads/32MiB stack; cada nativo64MiB/128handles/16threads/8MiB stack; quatro papéis, freshness60s/1s, dois pre-effect generations, headroom5/9GiB/pisos4/8GiB e NONE_GRANTED. Modelo de reserva e métricas físicas não são confundidos; arquivos não são contados como handles vivos e custos compartilhados não são duplicados.

## Dependências e caminho mínimo após eventual aceitação

1. Aprovação e registro normativo do escopo L0–L9, com N7 proposto explícito e condições pendentes.
2. Seleção posterior autorizada de UMA identidade, anexo exato e plano de binding, sem criação automática de intent/recursos. Ratificar N7 para a tupla e fixar obrigações históricas/cobertura de custos.
3. Autorização separada de implementação focal, builds/testes pertinentes e pins reais; autorização específica para eventual nova colocação protegida.
4. Revisão de orçamento independente/censo/SCM/exclusividade e qualificação factual manual somente se autorizada; sem limites/risco comprovados, DENY.
5. Pacote e aprovação operacional exata posteriores. Nada deste relatório é comando positivo ou registro de Start aprovado.

Não foram feitos builds/testes nativos ou consultas operacionais: fonte/pins não mudaram, tarefa estritamente normativa. Foram verificadas integridade de autoridades/preimagens, cláusulas, referências e preservação de WIP; exemplos PASS/DENY são requisitos, não resultados de execução.

CURRENT_TASK_COMPLETED=YES
SUCCESSOR_SCOPE=PROPOSED
CANONICAL_OWNER=PRESERVED
SERVICE_IDENTITY=PRESERVED
N7_REACCEPTANCE=PROPOSED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
HISTORICAL_UNKNOWN=RETAINED
NEW_ATTEMPT=NOT_CREATED
IMPLEMENTATION_AUTHORIZED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=SUCCESSOR_SCOPE_L0_L9_NOT_ACCEPTED_EXACT_IDENTITY_AND_BUDGET_PENDING
NEXT=SUCCESSOR_SCOPE_NORMATIVE_ACCEPTANCE
