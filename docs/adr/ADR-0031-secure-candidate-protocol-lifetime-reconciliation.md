---
adr: ADR-0031
title: Reconciliação do lifecycle de Secure Candidate e Protocol Slot
status: accepted
decision-date: 2026-10-08
last-revised: 2026-10-09
revision: 2
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0031 — Reconciliação do lifecycle de Secure Candidate e Protocol Slot

## Status e limite

Esta ADR está em estado `accepted`. A decisão foi aprovada explicitamente pelo
owner; sua implementação e qualificação permanecem sujeitas aos slices
governados e aos testes C1-C12 definidos neste documento.

A decisão não qualifica a implementação parcial do ADR-0030 e não muda API/ABI
pública, PST, PACC wire ou Browser.

## Contexto factual

PAPACC_SERVER_SECURE_IO_WIN32 aceita conexões Secure Principal, executa o
handshake PST, publica uma Connection no server I/O loop e associa o Secure
Candidate ao Protocol Slot por connection_instance_id. Depois dessa associação,
o candidate entra em PROTOCOL.

O reap do server I/O loop pode terminar o Protocol Slot, fechar e remover a
Connection e acionar o close-root do transport adapter. Esse close-root remove a
conexão PST do wait-set, libera o processor PST e fecha o transporte nativo
exatamente uma vez.

A divergência observada é que não existe reconciliação posterior do Secure
Candidate. Oito candidates permaneceram em PROTOCOL depois do desaparecimento
de seus Protocol Slots e Connections correspondentes. O pool de oito candidates
foi esgotado e o accept seguinte retornou PAPACC_RESULT_LIMIT_EXCEEDED.

ROOT_CAUSE:

    Secure Candidates permanecem em PROTOCOL após remoção do Protocol Slot e da
    Connection correspondente, mesmo quando o close-root já removeu a
    membership e liberou a conexão PST.

A ausência do Protocol Slot, isoladamente, não prova que a membership foi
removida, que PST foi liberado ou que callbacks e referências externas deixaram
de existir. Portanto, ela não autoriza marcar o candidate como CLOSED.

## Autoridades de lifecycle

O server I/O loop é autoridade para:

- estado terminal e shutdown do Protocol Slot;
- fechamento e remoção da Connection;
- lifecycle de Session e Channel;
- reap de recursos de protocolo.

PAPACC_SERVER_SECURE_IO_WIN32 é a autoridade para reconciliar e tornar um Secure
Candidate reutilizável. Essa decisão pertence exclusivamente à thread owner e
não pode ocorrer durante dispatch do scheduler.

O transport adapter é a autoridade factual para registered e closed. O secure
processor é a autoridade factual para a presença da pst_connection.

A reconciliação não cria uma segunda autoridade de ownership.

## Identidade e geração

A correlação obrigatória usa o connection_instance_id exato que foi publicado
para o candidate e para o Protocol Slot.

O índice do array de candidates é somente identidade física e diagnóstica. Ele
não autoriza teardown, release ou reutilização.

Connection, Session, Channel e Network Stream preservam identidades
independentes:

- Connection: connection_instance_id;
- Session: session_instance_id;
- Channel: channel_instance_id;
- Network Stream: stream_instance_id e sua geração;
- Secure Candidate: associação à geração factual da Connection.

A reconciliação não encerra recurso cuja identidade ou geração não corresponda.
A reutilização de um slot físico por uma nova Connection nunca autoriza cleanup
da nova geração em nome da anterior.

## Owner e trigger

OWNER:

    PAPACC_SERVER_SECURE_IO_WIN32, exclusivamente na thread owner.

TRIGGER:

- depois que dispatch_ready retornar;
- depois de maintenance/reap do server I/O loop;
- antes de aceitar novas conexões;
- durante o owner progress já definido pelo ADR-0030.

A função não modifica o scheduler durante dispatch e não adiciona worker,
locking ou retry policy paralela.

## Ordem segura

A ordem normativa proposta é:

    PROTOCOL_SLOT_TERMINAL
    → SLOT_SHUTDOWN
    → CONNECTION_CLOSE_ROOT
    → WAITSET_REMOVE
    → PST_RELEASE
    → CONNECTION_REMOVE
    → OWNER_RECONCILIATION
    → CANDIDATE_CLOSED

A reconciliação acontece depois do close-root. Ela não repete wait-set removal,
PST release ou native close já executados.

## Pré-condições para CLOSED e reutilização

Um candidate PROTOCOL somente pode passar a CLOSED quando todas as condições
abaixo forem comprovadas para a mesma connection_instance_id/generation:

1. o Protocol Slot correspondente está ausente;
2. a Connection correspondente está ausente do Connection Manager;
3. transport_adapter.registered == PAPACC_FALSE;
4. transport_adapter.closed == PAPACC_TRUE;
5. secure_processor.connection == NULL;
6. não existe callback válido pendente nem referência externa válida;
7. dispatch_active == PAPACC_FALSE;
8. todas as identidades e gerações observadas correspondem ao candidate.

Somente depois dessas verificações o owner pode liberar os contextos de
segurança ainda pertencentes ao candidate e marcar o estado CLOSED.

UNUSED e CLOSED são reutilizáveis. HANDSHAKING, AUTHORIZED, PROTOCOL,
SHUTTING_DOWN e FAILED não são reutilizáveis sem completar seu teardown
governado.

A condição “pendente de reconciliação” é derivada dos fatos existentes. Esta
proposta não adiciona um lifecycle concorrente nem presume um estado CLOSING que
não existe atualmente no enum do candidate.

## Falha de remoção

Se wait-set removal falhar:

- o candidate permanece não reutilizável;
- adapter, processor e pst_connection permanecem preservados;
- a identidade da Connection e a causa factual permanecem preservadas;
- PST release não é executado;
- o candidate não é marcado CLOSED;
- nenhum fallback destrutivo é permitido;
- a recuperação usa REMOVE_PENDING, retry governado e deadline absoluto do
  ADR-0030.

A reconciliação não cria retry independente e não limpa identidade para ocultar
falha.

## API interna proposta

A implementação futura requer uma função privada result-returning, por exemplo:

    papacc_server_secure_io_win32_reconcile_candidates(secure_io)

Contrato mínimo:

- valida secure_io, owner thread e dispatch inativo;
- examina somente candidates com estado elegível;
- correlaciona connection_instance_id e gerações;
- retorna OK somente para reconciliações concluídas ou ausência de trabalho;
- retorna estado pendente/erro factual sem executar fase destrutiva posterior;
- executa trabalho bounded por chamada;
- preserva exactly-once remove, release e native close.

Nenhuma API/ABI pública é alterada.

## Invariantes

- CLOSED implica Protocol Slot ausente, Connection ausente, adapter não
  registrado e fechado, processor sem pst_connection e nenhuma referência
  externa válida.
- Nenhum PST release ocorre enquanto registered for verdadeiro.
- Uma falha de remoção preserva o último ponteiro válido.
- O owner nunca modifica membership durante dispatch.
- Reutilização reinicializa o candidate e associa uma nova
  connection_instance_id.
- Callback tardio valida identidade e geração antes de acessar estado.
- Nenhuma transição CLOSED pode ressuscitar Connection, Session, Channel,
  Stream ou candidate.
- Pool cheio com oito candidates legitimamente ativos continua sendo uma
  condição factual de limite, não autorização para eviction.

## Plano de qualificação C1–C12

### C1 — candidate elegível encerrado

Preparar candidate PROTOCOL cuja geração teve Slot e Connection removidos pelo
close-root. Confirmar todas as pré-condições, uma transição CLOSED e nenhuma
segunda remoção/release/close.

PASS: candidate CLOSED e reutilizável; remove, PST release e native close
permanecem exatamente uma vez.

### C2 — slot ativo preservado

Manter Protocol Slot e Connection ativos para o mesmo connection_instance_id.

PASS: candidate permanece PROTOCOL; zero remove, release ou mudança de causa.

### C3 — reutilização segura por geração

Reutilizar o slot físico de protocolo com nova connection_instance_id enquanto
há evidência tardia da geração anterior.

PASS: nova geração permanece intacta; evidência antiga é rejeitada e não acessa
nem encerra o novo recurso.

### C4 — falha de remoção preservada

Injetar falha factual de wait-set removal.

PASS: registered permanece verdadeiro; candidate, processor, pst_connection,
identidade e causa permanecem; zero PST release, native close ou CLOSED.

### C5 — callback tardio rejeitado

Entregar callback da geração anterior após terminal/reconciliação.

PASS: callback é rejeitado por identidade/generation; nenhuma publicação,
liberação, alteração de causa ou resurrection.

### C6 — perda de CONTROL

Encerrar CONTROL com DATA/Streams dependentes reais.

PASS: cascata definida pelo contrato ocorre; cada Connection é reconciliada
pelo owner correto; nenhum DATA órfão e exactly-once teardown.

### C7 — Stop global integrado ao ADR-0030

Solicitar Stop durante candidate pendente de reconciliação e, separadamente,
durante falha de removal.

PASS: owner reutiliza STOP_REQUESTED/REMOVE_PENDING/DRAINING/ERROR_TERMINAL,
deadline não reinicia e nenhuma retry policy paralela aparece.

### C8 — múltiplos candidates

Terminar várias gerações em uma mesma janela de manutenção.

PASS: reconciliação bounded progride sem starvation; cada candidate é fechado
uma vez e o scheduler não é mutado durante dispatch.

### C9 — pool legitimamente cheio

Manter os oito candidates em estados ativos e não elegíveis.

PASS: novo accept retorna limite; nenhum candidate ativo é evicted ou fechado.

### C10 — pool recuperável

Encher o pool com pelo menos um candidate que satisfaça todas as pré-condições.

PASS: owner reconcilia o elegível antes do accept; nova conexão usa nova geração
sem afetar candidates ativos.

### C11 — exactly-once nativo

Observar pontos reais de remove, provider/PST release e native close.

PASS: para cada recurso adquirido, uma tentativa efetiva bem-sucedida de cada
fase aplicável; zero segunda liberação ou close. Recurso não adquirido tem zero
release.

### C12 — nenhuma resurrection

Entregar readiness, callback, frame ou retry tardio após CLOSED e após
reutilização do slot físico.

PASS: nenhuma operação, membership, Connection, Session, Channel ou Stream é
recriada e nenhum objeto liberado é acessado.

## Compatibilidade com ADR-0030

Esta proposta especializa o progresso da mesma thread owner e preserva:

- REMOVE_FROM_WAITSET antes de PST_RELEASE;
- proibição de mutação durante dispatch;
- preservação integral quando removal falha;
- retry e deadline governados por REMOVE_PENDING;
- ERROR_TERMINAL não destrutivo;
- exactly-once teardown.

Não altera o ADR-0030 nem cria política concorrente.

## Relações normativas

- ADR-0017 — lifecycle de CONTROL/DATA e terminais de Network Offload.
- ADR-0023 — shutdown TLS_REQUIRED, terminal imutável e exactly-once teardown.
- ADR-0030 — owner thread, remove-before-release, retry e falha terminal segura.
- PapinhoSecureTransport ADR-0005 e docs/lifecycle-ownership.md — wait-set
  empresta connections; o consumer remove membership antes de release.

## Consequências

Positivas:

- elimina retenção de candidates PROTOCOL já encerrados;
- preserva ownership PST e exatamente um close-root;
- separa capacidade legitimamente ocupada de capacidade recuperável;
- impede cleanup por índice ou geração errada;
- integra recuperação ao ADR-0030.

Trade-offs:

- adiciona uma fronteira privada de reconciliação;
- exige observabilidade de geração e close-root;
- exige matriz C1–C12 antes de qualificação;
- mantém candidates não reutilizáveis quando a prova de teardown é incompleta.

## Não objetivos

Esta proposta não implementa a função, não altera enums públicos, não muda
PACC wire, não modifica PST, não requalifica ADR-0030, A–I ou S1–S12 e não
autoriza commit, push ou release.
