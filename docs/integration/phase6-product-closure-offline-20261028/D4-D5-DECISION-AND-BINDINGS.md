# D4/D5 — conclusão técnica possível sem efeitos

O anexo JSON contém 30 ingredientes reconciliados, não uma política aplicável. D2 já está ACCEPTED apenas para preparação. Não se solicita novamente escolher B ou aceitar D2. Nenhum valor quantitativo novo foi aprovado ou inventado.

## Fontes, unidades e coexistência

| Categoria | Produtor/campo atual | Uso permitido / falta |
|---|---|---|
| Reserva Browser | papinho_c2_budget.inc e sizeof/context das provas históricas | Cap2097152; charge1858020 histórico; diferença239132 é aritmética gerenciada, não headroom físico. Charge transferida job/caller/result vive até última utilização e join/close. Duas charges3716040 não cabem. |
| Accelerator | ed11_process_profile.h / C8 layout/combined_reserved | Selected3145728 e default2097152 distintos. Macros históricas1965128/1799632 não substituem novo censo total; estão marcadas como histórico no JSON. Cada instância do processo tem seu orçamento, nunca somar tabelas como se fosse um processo. |
| C2/C3 buffers | c2_operation_win32.h; c3_bridge_win32.h | Quatro flows65536 e duas filas65536 são subconjuntos embutidos dos owners. Não cobrar novamente por cópia de referência. Um par de filas C3 não inclui toda memória do provider/kernel. |
| Browser application | C2_HTTP_CAP524288/C2_FLOW_CAP65536/C2_FRAME_CAP16400 | Conteúdo/contexto e saída terminada/resultados têm lifetimes distintos. Sem redirecionamentos/subrecursos na candidata uma operação; UI não pode criar trabalho extra silenciosamente. |
| Raw C8 | target_collect/process_provenance/target_view | started_ms/finished_ms, process creation, series/sequence, observation/validation/expiry originais.100ms conclusão/cadência e1000ms expiry são mecanismos; policy ABSENT e final RESOURCE_UNKNOWN permanecem no validador. |
| Browser clocks | resource_observe / native receipt | GetTickCount U32, processo/criação/thread e presence/error. Não comparar ticks com PAL do Accelerator; ausência de API NT4 não é medida zero. |
| Provider | c3_bridge_provider_snapshot / papinho_pst_owner_provider_snapshot | initialized/provider_id/version sob owner original, só em janela válida e sem novo acquire. Layout60 histórico é valor de metadados, não footprint. Prova de módulo mapeado/bytes exige observação própria. |
| Host | GlobalMemoryStatusEx e observador existente | Available physical e commit disponíveis separados. Windows/cache/driver/shared e alocação transitória opaca permanecem UNKNOWN. Working set/private/pagefile/pool não se somam. |
| Retidos | credits/leases/root/Model E e resultado Browser | Reservas permanecem cobradas até fatos de encerramento. Histórico Q4 não dá crédito por retorno de processo/fechamento local. |

## Restrições quantitativas já demonstráveis

O intervalo útil de um sample não pode exceder sua expiração original nem a validade aplicável da política alvo. Cópia passiva não renova intervalo. A política alvo precisa ser explícita e pode impor condições mais estritas; o raw1000ms não a instala. O cap estrutural de duas operações não prova capacidade para duas.

O timeout Browser de progresso30s reinicia com progresso; handshake10s, pump60s, setup/drain5s não se somam para formar limite total. Logo o mecanismo atual não fundamenta número finito de duração máxima global por essa soma. Interrupção de I/O síncrona/retained owner também não vira garantia de fechamento no deadline.

Uma contagem de handles ou pico privado LAB não é envelope de Browser/C2/C3. Headroom físico/commit não deriva dos239132 bytes aritméticos nem do limite gerenciado Selected. Nenhum piso LAB60s/1s/5GiB/9GiB foi transportado ao alvo.

## Bindings necessários por caller

1. Publicação/configuração: current_pin e root_operational_reference_current fornecem domínio/revisão/context_epoch/operational_epoch/invocation e leases; não obter de timestamp ou arquivo N0.
2. Política e SiteTLS: referência efetivamente aprovada do catálogo/CURRENT, trust, hostname e destino. c2_site_template não aceita trust arbitrário como SYSTEM. Browser original hostname/endpoint snapshot e epoch do OPEN_RESULT são fatos distintos.
3. Proveniência: PID/criação e owner thread de cada processo, SDK/backend normal correto, módulo original efetivamente mapeado, identidade do arquivo e receita. Hash de artefato no build não é atestação do módulo vivo.
4. Fase/lifetime: baseline antes da preparação; inicialização do provider; handshake; atividade; drainage; encerramento/retained. Histórico finito C8 de oito entradas pode ficar incompleto: overflow não apaga amostras nem vira cobertura de todas as fases.
5. Recursos: per-process total/baseline/increment/peak e shared atribuído uma vez; reservas da mesma root/owner e retidos. Quem fornece cada campo está em CALLER-MAP.csv. Todos os bindings operacionais continuam null neste pacote.

## Primeira escolha quantitativa ainda necessária

Não existe uma política completa a aprovar hoje. É necessário delimitar o suporte do diagnóstico alvo (artefatos/backend/host/workload e risco residual) e fundamentar cinco parâmetros: idade máxima, validade da coleta, duração global, piso físico e piso de commit. O executor pode reunir ingredientes e propor limites, mas não converter máximos amostrados ou números LAB em escolha normativa.

Alternativa estrita mantém DENY até envelope completo. Alternativa diagnóstica B mantém D2 e só permite futura execução após anexo quantitativo justificado e aceitação operacional própria; não concede capacidade produtiva. A aceitação residual de execução não foi suprida pela exceção preparation-only. Sem esses dados não há comando nem pedido positivo byte-exato pronto. Não abrir outra ADR para reiterar essa mesma ausência.
