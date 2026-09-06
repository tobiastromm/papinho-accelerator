---
capability: logging
title: Logging e diagnóstico operacional
status: partial
last-updated: 2026-09-06
scope: project
---

# Logging e diagnóstico operacional

## Objetivo

Aplicar no PapinhoAccelerator o contrato transversal de logging do ecossistema Papinho, mantendo logging estruturado, seguro, configurável e independente da interface funcional.

## Comportamento esperado

### Níveis e threshold

```text
OFF
ERROR
WARN
INFO
DEBUG
TRACE
```

Semântica cumulativa:

```text
ERROR → ERROR
WARN  → WARN + ERROR
INFO  → INFO + WARN + ERROR
DEBUG → DEBUG + INFO + WARN + ERROR
TRACE → TRACE + DEBUG + INFO + WARN + ERROR
```

`INFO` é o nível para operação normal relevante. Uma instalação saudável deve poder permanecer em `INFO` por longos períodos sem avalanche de mensagens.

`DEBUG` contém diagnóstico técnico. `TRACE` contém detalhe fino, repetitivo ou forense.

### OFF e saída funcional

`--log-level off` desliga o logger, não a interface funcional.

Continuam disponíveis, conforme aplicável:

```text
--help
--list-interfaces
resultados explicitamente solicitados
mensagens necessárias à operação
estado funcional da GUI
```

```text
logging output != functional output
```

Uma futura janela de Logs/Diagnóstico deixa de receber novos eventos quando `OFF`; uma área funcional como “Servidor: Em execução / Clientes: 2” continua funcionando.

## Configuração compartilhada

CLI e GUI são Configuration Sources do mesmo modelo:

```text
CLI --log-level debug ─┐
                       ├──► PAPACC_SERVER_CONFIG / Configuration Model
GUI Depuração (DEBUG) ─┘
                                  │
                                  ▼
                               Logger
```

Correspondência prevista:

| GUI | CLI |
|---|---|
| Desativado (OFF) | `--log-level off` |
| Erros (ERROR) | `--log-level error` |
| Avisos (WARN) | `--log-level warn` |
| Informações (INFO) | `--log-level info` |
| Depuração (DEBUG) | `--log-level debug` |
| Rastreamento (TRACE) | `--log-level trace` |

Não criar checkbox separado “Ativar logs”; `OFF` já representa isso.

Inicialmente, manter um único `log_level` global. Filtros por subsistema podem ser avaliados futuramente se houver necessidade real.

O nível operacional padrão implementado e validado na CLI é `INFO`.

## Classificação no Accelerator

### ERROR

- listener obrigatório não abriu;
- inicialização obrigatória falhou;
- erro fatal de backend.

### WARN

- recurso opcional indisponível;
- condição recuperável;
- configuração válida, porém degradada/problemática.

### INFO

- servidor iniciado/parado;
- listener aberto;
- conexão aceita/encerrada;
- Session criada/encerrada;
- backend selecionado;
- configuração efetiva operacionalmente relevante.

### DEBUG

- transições internas;
- resolução de Bind Target;
- decisões internas de configuração/policy;
- resultados intermediários relevantes.

### TRACE

- framing/mensagens internas;
- passos finos de state machine;
- operações repetitivas;
- investigação forense.

## Sinks e PAL

No Accelerator, sinks específicos de plataforma podem atravessar a PAL:

```text
Configuration Model
    log_level=INFO
         ↓
      Logger
         ↓
    PAL / Log Sink
   ┌─────┼──────┐
 Win32  Linux  embedded
```

A PAL/sink determina onde e como o evento aparece; nível e semântica pertencem ao logger.

## Integração com PapinhoSecureTransport

PST não cria um segundo sistema visual de logging:

```c
logging.callback = accelerator_on_pst_log;
logging.user_context = &accelerator_logger;
```

```text
PST
 │ PST_LOG_EVENT
 ▼
accelerator_on_pst_log(...)
 ▼
adaptador PST → contrato Accelerator
 ▼
logger Accelerator
 ├── GUI
 ├── arquivo
 └── console/PAL sink
```

## Endpoints de conexão

Quando útil ao diagnóstico de uma conexão aceita, registrar ambos:

```text
remote=192.168.1.20:53142
local=192.168.1.10:39999
```

Isso permite saber não apenas quem conectou, mas por qual endereço/interface local a conexão entrou.

## Segurança

Mesmo em `TRACE`, não registrar automaticamente senhas, private keys, tokens completos, credential material, segredos de autenticação, payload arbitrário, material privado de certificados/tickets ou dumps indiscriminados.

Preferir IDs diagnósticos, resultados normalizados e metadados seguros.

## Estado atual

Status geral: `partial`.

A auditoria factual de 2026-09-06 confirmou a implementação atual abaixo.
O logger ainda é parcial diante da gramática transversal completa.

| Área | Estado | Observação |
|---|---|---|
| `--log-level off` | validated | Implementado/validado no histórico recuperado |
| OFF vs saída funcional | validated | Regra validada |
| callback + contexto explícito | validated | Sink e contexto fornecidos ao logger |
| ausência de logger global obrigatório | validated | Logger é passado explicitamente |
| eventos efêmeros síncronos | validated | Callback imediato; strings emprestadas durante a chamada |
| OFF, ERROR, WARN, INFO e DEBUG | validated | Enum, parser CLI, help e testes |
| TRACE | not-implemented | Não existe no enum nem na CLI |
| threshold cumulativo | validated | Testado em cada threshold implementado |
| default INFO | validated | Initializer da request CLI |
| CLI `off|error|warn|info|debug` | validated | Parser rejeita valores desconhecidos |
| structured events | partial | Level, component, message e timestamp existem |
| `EVENT_ID`, `CATEGORY`, `RESULT`, `OPERATION` | not-implemented | Campos ainda ausentes do record |
| nível global | implemented | Um `log_level` por execução do servidor |
| GUI | planned | Futura GUI |
| PAL/sinks | partial | Timestamp usa PAL; console sink pertence à aplicação |
| PST adapter | not-implemented | PapinhoSecureTransport ainda não está integrado |
| local+remote endpoint | partial | `remote` existe; `local` ainda não é registrado |

## Pendências

- [ ] Adicionar `TRACE` ao contrato, configuração, sinks e testes.
- [ ] Evoluir o record com `EVENT_ID`, `CATEGORY`, `RESULT` e `OPERATION`.
- [ ] Implementar integração PST por callback/adaptador quando o componente for integrado.
- [ ] Registrar `local=` e `remote=` nos eventos apropriados.
- [ ] Revisar INFO para evitar debug disfarçado.
- [ ] Validar ausência de segredos em DEBUG/TRACE.
- [ ] Integrar futura GUI ao mesmo Configuration Model da CLI.

## Ideias futuras

- filtros/categorias por subsistema se houver necessidade real;
- múltiplos sinks simultâneos;
- janela de diagnóstico na GUI;
- exportação estruturada de eventos.

Não são automaticamente decisões ou implementações atuais.

## ADRs relacionadas

- `PapinhoEngineering/ADR-0003` — Contrato estruturado e desacoplado de logging.
- `PapinhoEngineering/ADR-0004` — Modelo compartilhado de configuração, fonte única de verdade e separação entre Core e Frontends.
- `PapinhoEngineering/ADR-0005` — Governança da documentação viva de capabilities.

## Código relevante

- `src/runtime/log.h`
- `src/runtime/log.c`
- `src/runtime/runtime.c`
- `apps/server/server_cli.h`
- `apps/server/server_cli.c`
- `apps/server/main.c`
- `apps/server/server_run_win32.c`
- `apps/server/server_io_loop_win32.c`

## Testes / evidências

- `tests/log_test.c` — callback/contexto, timestamp, argumentos, thresholds e OFF.
- `tests/server_cli_test.c` — default INFO, DEBUG, OFF e rejeição de nível inválido.
- `tests/runtime_test.c` — logger opcional e lifecycle do runtime.
- `tests/server_run_win32_test.c` — eventos operacionais e execução sem eventos em OFF.
- CTest de 2026-09-06: 41/41 testes passaram, incluindo
  `papacc.runtime.logging`, CLI, runtime e server execution.

## Histórico de mudanças

| Data | Descrição |
|---|---|
| 2026-09-06 | Documento inicial consolidando decisões e estado conhecido do logging do PapinhoAccelerator. |
| 2026-09-06 | Auditoria factual sincronizou níveis, threshold, OFF, callback, CLI, sinks, testes e pendências estruturadas. |
