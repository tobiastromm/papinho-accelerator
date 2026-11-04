# Admissão específica do primeiro Start — sem capacidade concedida

Autoridade: F8, B7, SSO-B S3/S5/S6 e TO7. Staging B0–B8/B3 e registro R0–R9 não
cobrem este efeito. Nenhum accepted foi modificado. Headroom não deriva do owner
BA da imagem nem de um resultado de registro parado.

| Classe | Evidência própria | Alcance / momento |
|---|---|---|
| Owner gerenciado | Selected 1.993.744 bytes; Legacy 1.825.176; quotas já existentes 3.145.728 e 2.097.152 | sizeof/somas controladas; não margem física/commit ou orçamento nativo |
| Estado SCM | sizeof(native)+sizeof(host)=14.928 bytes | Armazenamento estático controlado; não todo o processo |
| Novo caller | um objeto local PAPACC_SCQ_PLAN_DATA de 1.048 bytes; nenhum handle/heap novo no wrapper; chamadas transitivas existentes | Tamanho exato do objeto, NÃO teto do stack, da cadeia ou do SO; não somar ao root como se embutido fosse |
| Carrier/transporte | carrier308; texto622 posições incluindo NUL; record1048; SQR1 1052; SQC1/SQB1 1076 | Buffers já embutidos/limitados; não somar componentes duas vezes nem chamar custo kernel de zero |
| Cadeia da imagem | oito slots de diretório + imagem; três file + três reader e descritores no input existente | Slots/ownership, não contagem total do participante; guard124 antes de cada aquisição; cap128 e quotas originais preservados |
| Handles dos participantes | processos/tokens/SCM/event/pipe, originais e temporários; remoto não é handle local | Conjunto vivo por fase ainda não integralmente fundamentado; não descontar herdados/console/loader sem prova |
| Novo processo/loader/RPC | StartService, criação SCM, CRT/stacks/loader, queries de token, endpoint e provider/kernel | Custos materiais opacos sem limite demonstrado; devem ter suficiência/política legítima ANTES do efeito; PID/token reais são posteriores |
| Host/relógios | censos/amostras antigas são históricos | Margem física/commit, freshness, validade/duração e custo de observação próprios continuam UNKNOWN; B3 500ms/120s/pisos não importados |
| Retenção/Stop | packet/buffers/referências retidos; handler retirado somente no fechamento conhecido | Cancelamento não é prova de término; UNKNOWN conserva reservas e nega; receipt/close/término só após operação |

Relatório mantém `resource_pre_missing=63` (processo, RPC, margem do host,
relógios, retenção e handles); `resource_effect_obligations=7` (runtime,
endpoint, Stop/close). O bit de recursos em `missing_pre_start` permanece
obrigatório. Novo campo de sizeof do plano não limpa nenhum desses bits.

Gates prévios: fonte própria de cada efeito, identidade/proteção dos originais,
consumo antes dos efeitos, suficiência própria de participantes/host/retidos e
política de janela. Gates posteriores: PID/criação/token/SCM originais do novo
runtime, autenticação bilateral, receipt/close e readiness fechada. Não exigir
um PID futuro antes de Start e não conceder capacidade por esse PID depois.

TO7 é explícito: não há exceção de envelope opaco e amostras finitas não são
ceilings. Portanto outra coleta isolada não resolveria a política. A alternativa
legítima vigente é fundamentar provisão/suficiência integral; uma especialização
de risco do primeiro Start precisaria de decisão própria e não pode herdar R0–R9.
Esta tarefa não seleciona números, não apresenta custos UNKNOWN como zero e não
aceita uma especialização por inferência. Não foi preparado pedido operacional
positivo. Essa é uma fronteira de garantia real adicional ao producer ausente,
não exigência de medir um serviço que ainda não iniciou.
