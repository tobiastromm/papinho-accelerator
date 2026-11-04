# Recursos do primeiro Start diagnóstico — suficiência e fronteira

Escopo exclusivo: PERSONAL_LOCAL, SCQ TRANSPORT_ONLY, processo/controlador e runtime virtuais originais, sem KSP ou atividade produtiva. Não é R0–R9 (CreateService parado) nem B0–B8/B3 (staging). Não houve coleta, consulta SCM, endpoint ou Start nesta execução.

## Autoridade aplicável

DECISION-personal-scq-cross-principal-offline-20261030 incorpora B regras2–8; regra7 mantém os limites e a obrigação de fundamentar custos do próprio perfil. DECISION-personal-scq-closed-startup-s0-s7-20261030 incorpora S3, S5 e S6: readiness requer limites, Stop/retidos e quotas próprias, sem prazos inventados. DECISION-personal-scq-transport-only-to0-to9-20261030 incorpora TO3, TO5, TO6 e TO7. TO7 diz expressamente que TRANSPORT_ONLY não é exceção de envelope opaco e que UNKNOWN obrigatório continua DENY. F8 exige os custos transitivos/retidos e não apenas o nonce ou o sizeof do packet. Nenhum desses textos foi alterado.

124/128/1024 permanece nos participantes SCQ por incorporação EXPRESSA em B7/S6/TO7, sem estender esse envelope automaticamente a staging ou ao SCM/RPC de registro. Antes de aplicar o número ao novo processo ou controlador é preciso identificar o participante e sua contagem completa. Não se pode descontar console, herança ou loader por conveniência; não há atribuição nativa nova nesta tarefa. Os 132 handles de antigas coletas de staging não são observação do novo runtime SCM e não fundamentam um teto SCQ.

## Termos, fase e evidência necessária

| Recurso | Fundamento disponível | Gate prévio / prova posterior |
|---|---|---|
| Owner gerenciado e candidato | sizeof compilado; candidato PAPACC_SCQ_OWNER_DECISION 848 bytes, flag e padding incluídos no mesmo packet, sem heap/handle novo no construtor | Reserva gerenciada anterior aos efeitos no owner existente; não somar componente embutido outra vez |
| Owner SCM/ServiceMain | sizeof(native)+sizeof(host)=14928 bytes neste build Win64; não inclui todo o root nem stacks transitivos | Censo controlado, não provisão física/commit ou capacidade produtiva |
| Carrier | 308 bytes tipados, 622 posições ASCII/WCHAR incluindo NUL, buffers embutidos existentes | Validar formato/escopo e preservar os buffers; nenhum crédito por posse do carrier |
| Transporte | record1048, SQR11052, SQC1/SQB11076; buffers e OVERLAPPED no driver existente | Custódia limitada pelo desenho; pipe/event/IO kernel e polls não têm ceiling nativo demonstrado |
| Referências | Três originais request/decision/image; file[3] e reader[3] distintos; três referências DUP destino; processo/token/controlador/runtime/SCM/event/pipe e consultas temporárias | Censo por fase/participante, sem somar remote_files como handles locais ou chamar um alias de referência gratuita. Medir contagem fresca não prova limite de custo interno |
| Novo processo SCM | Imagem e DLLs conhecidas são conteúdo em disco, não working set, private bytes, stack, commit ou envelope de loader/CRT | Provisão/política anterior ao Start ainda UNKNOWN. PID, token, memória e término reais são obtidos DEPOIS do Start, não pré-condições impossíveis |
| Hash/SD/token | Verificações CNG públicas e SD/token existentes; construtor só empresta depois das verificações e não chama KSP | Transitórios de leitura, descritores, BCrypt e queries precisam fundamento próprio; TO3 não os torna zero |
| RPC e endpoint | OpenSCManager/StartService e pipe existentes, com acesso mínimo e consumo separado | Custos materiais opacos e admissão continuam prévios; resultado do RPC, conexão e entrega são provas posteriores |
| Host físico/commit | Censos e amostras históricas não são margem fresca nem teto | Piso/margem justificados e observação fresca antes dos efeitos; nenhuma importação de 3MiB/R0–R9 ou pisos/B3 como suficiência nativa |
| Freshness/duração | Janela original por GetTickCount64 e revalidações; sem novo default | Política específica de idade, duração, custo de coleta e validade ainda não fundamentada. Não importar 500ms/120s do staging |
| Stop/retenção/close | Stop governado, wake do evento emprestado, primeira falha e retenção no owner | Reservar o conjunto controlado enquanto vivo. Cancelamento/expiração não provam cancelamento nativo nem liberam crédito; receipt/close/término são posteriores |
| Disco | Não se autoriza copiar binários, criar arquivos de estado, CA ou CURRENT no Start fechado | Revalidar imagem original; não reutilizar 2R/R do staging. Pagefile/commit e custos do OS não viram zero por ausência de escrita de arquivo pelo aplicativo |

O censo Selected desta campanha reporta KNOWN=1990840 e PROCESS_LIMIT=3145728; Legacy reporta KNOWN=1822272 e PROCESS_LIMIT=2097152. São somas controladas compiladas e quotas existentes: não provam margem física, commit, stacks transitivos ou reserva Windows/RPC. Os logs de detalhe de sizeof não são coletas operacionais. Nenhum backend ou quota foi alterado.

## Admissão no código

PAPACC_SERVICE_DIAGNOSTIC_START_REPORT agora separa resource_pre_missing=63 em seis bits: envelope do processo(1), transitórios RPC(2), margem do host(4), política de relógios(8), envelope de retenção(16), handles/participantes(32). resource_effect_obligations=7 registra runtime(1), endpoint(2) e Stop/close(4). São estados de evidência, não capacidades concedidas. O SCM CLI imprime a distinção quando a origem falta. O resource bit8 de missing_pre_start continua obrigatório e DENY; não há setter ou callback para limpá-lo.

Não há prova de PID/token/receipt antes do processo existir como exigência de admissão. A impossibilidade de obter esses resultados antes do efeito não torna admissíveis os custos materiais prévios desconhecidos. A norma vigente separa essas duas classes e não aceita observar um pico posterior como provisão anterior.

## Primeiro pedido operacional

Ainda NÃO existe pedido byte-exato de Start ou endpoint autorizável. Uma nova amostra somente leitura pode observar contagem/margem/metadata, mas não resolve por si envelope opaco ou a política ausente; portanto não foi criada outra coleta. Para manter garantia integral, é preciso uma fonte de provisão/envelope e política SCQ específica fundamentada. Se o proprietário preferir posteriormente um ensaio com risco residual, TO7 precisará de decisão focal explícita própria do Start; B0–B8/R0–R9 não cobrem esse efeito. Nenhuma proposta normativa foi criada automaticamente, nenhuma redução de garantia foi inferida e nenhum número de risco foi selecionado.
