# Closeout — preparação da clarificação focal

Task PACC-PHASE6-STAGING-ENVELOPE-FOCAL-CLARIFICATION.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Apenas documentação nova e atualização da capability; WIP anterior preservado. Nenhuma aceitação ou mudança operacional.

## Proposta para decisão

STAGING-ENVELOPE-DOMAIN.CANDIDATE.md, regras D0–D7 integrais.
SHA256 literal: 5cad769d78c7f731653022ca00c9e80f8263177393424793f0d745459e8ed71c.
Estado PROPOSED. A direção do proprietário autoriza preparar, não aceitar. Não se registra nova aprovação nem se reedita ADR accepted.

Decisão pedida: aceitar a não generalização automática dos números de LAB/TPM/SCQ para o ato independente de staging, exigindo política própria fundamentada e DENY enquanto ausente. Isso não remove guards do código nem aprova número/pedido/execução. A aprovação, se ocorrer, deve identificar esse documento/hash exatos.

## Política proposta e evidências reutilizáveis

| Item | Evidência aproveitável | Lacuna / alcance |
|---|---|---|
| Fontes normativas | ENVELOPE-APPLICABILITY.md e fontes D6 | Nenhuma ponte específica ao staging localizada; não prova dispensa universal |
| Pacote candidato mais recente | Exe555.008 bytes/hash4a68ce6bdb2737221824ba2f70930fbb8534ea5dfda3d9cfa460eaef22c546f9 reconferido; DLLs/pins históricos no pedido3acf...; total9.094.144 | Não é imagem instalada, capacidade ou seleção operacional nova; verificar novamente pacote completo no pedido futuro |
| Layout estático | Packet7.360 bytes; buffer cópia4.096 e hash1.024;16 slots do staging além de token/volume/contexto; Selected1.988.704/Legacy1.820.136 geridos segundo último closeout qualificado | Não duplicar buffers já incluídos; managed não é total process/host. Valores anteriores de COSTS-AND-ADMISSION.md são históricos e não novo baseline |
| Handles | Último resultado109 entrada/pós-frame,132 pós-layout; queries válidas | Pico instantâneo/transitórios e suficiência de margem UNKNOWN; nenhum teto novo derivado |
| Memória | Última coleta: private2.826.240→3.551.232; WS6.582.272→8.192.000; mínimo físico6.367.141.888/commit9.160.630.272 | Histórico da execução, não disponibilidade atual ou reserva; falta envelope ativo/transitivo/retido e política própria de margem |
| Disco | Codec/query pelo volume original e aritmética já qualificados offline | Execução não alcançou volume; espaço caller/global, unidade e custo ativo FS UNKNOWN |
| Originais/proteção | Known-folder BINARY_ONLY com revalidação; aquisição/SD/close modelados | Ancestrais/token/volume reais não alcançados; identidade/proteção factual pendentes |
| Consumo/Stop/falhas | Regressões offline anteriores de staging/recursos/close e guards | Modelos não concedem autoridade real ou provam quiescência do OS |
| Fechamento observado | known=1,retained=0,effects=0 na coleta | Contexto ainda não adquirido; não prova close completo de operação com efeitos |
| Tempo | QPC e ordenação disponíveis na coleta | Janelas globais/freshness/margem não fundamentadas para efeito ativo; F4 por contexto não vira TTL arbitrário |

D4 da proposta define os critérios de suficiência por recurso. Não há valores de margem/limite de handles, teto de memória nativa, reserva de metadados FS ou duração aprovada. Não preencher esses campos com132, valores LAB, máximos de fixture ou zeros.

## Continuação independente concluída

Organizada a matriz de evidência e política sem alterar implementação dependente da clarificação. A especificação de atribuição permanece disponível como instrumento eventual para uma correção que dela dependa, não bloqueador universal já imposto. Não há implementação independente segura adicional demonstrada nesta etapa: qualquer retirada/alteração do guard depende da decisão e da política ainda não fundamentada.

Builds e testes históricos reutilizados apenas no alcance documentado; nenhum teste repetido sem mudança. Sem código/artefato novo a congelar. Nenhuma coleta/tracing, staging, SCM/ACL ou outro efeito protegido executado. E21 PENDING; ED11/Phase6 PARTIAL; staging DENY.

## Caminho mínimo

1. Decisão explícita sobre D0–D7/hash; sem aprovação manter proposta e guards atuais.
2. Depois de aceitação registrada em documento normativo designado, completar política aplicável com evidências, critérios de suficiência e originais; identificar o menor fato ainda indispensável, sem impor tracing por padrão.
3. Qualificar offline apenas a alteração autorizada pelo proprietário; aceitação desta clarificação não é autorização automática de remoção do guard.
4. Se necessário, preparar coleta factual distinta e mínima com scope/pins/gates próprios para aprovação; nunca reutilizar autorizações consumidas. Não prometer que coleta read-only prova custos de escrita/FS.
5. Com política/bindings/evidências completos, preparar pedido específico de staging protegido e aguardar aprovação operacional. Nenhum comando executável prematuro.
