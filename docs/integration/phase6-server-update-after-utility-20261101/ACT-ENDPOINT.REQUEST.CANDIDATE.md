# Ato E — endpoint privado da única sessão SCQ

Status CANDIDATE / NOT_AUTHORIZED. PERSONAL_LOCAL / TRANSPORT_ONLY.
Integra somente o pacote ONE-FIRST-DIAGNOSTIC-THREE-ACTS-REQUEST.CANDIDATE.md.
Não concede Start ou empréstimo SCQ por implicação.

Controlador: operador S-1-5-21-785699550-1440458426-1682014700-1108 em TCORP-TOBIAS,
processo original do EXE canônico protegido, confirmado por token/contexto,
PID/criação e imagem. Destinatário planejado: runtime SCM do serviço exato
PapinhoAccelerator, NT SERVICE\PapinhoAccelerator. Processos reais são adquiridos
e revalidados na tentativa, sem PID/token futuro fabricado.

No máximo UMA criação exclusiva do namespace e UMA abertura receptora local,
sem adoção, segunda conexão, forwarding ou retry. Nome calculado pelo código
pinado: prefixo `\\.\pipe\PapinhoAccelerator.SCQ.` seguido dos32 hex chars
dos primeiros16 bytes da tentativa original de32 bytes. Não se antecipa o nonce.
Pipe byte/duplex/overlapped, FIRST_PIPE_INSTANCE, uma instância,
PIPE_REJECT_REMOTE_CLIENTS; evento local auxiliar do mesmo transporte.
SD protegido: owner administrativo original; ACEs para SYSTEM, controlador e
runtime originais, máscara0x00120183; sem herança de handle. Não é IPC genérico.

Confirmador existente OWNER_SCQ_PRIVATE_ENDPOINT, independente de OWNER_SCQ
e OWNER_SCQ_DIAGNOSTIC_START. Consumo do ato endpoint precede criação; namespace,
contexto/identidade e G3 revalidados antes de efeitos. Bindings/carrier não são
autoridade por presença. QUERY só adquire candidatos não autoritativos.

G3, tentativa/lifetime e riscos do pacote são indivisíveis; menor deadline vence.
Close/receipt/incerteza mantêm semânticas distintas. Perda de peer, replay,
nome/identidade/proveniência divergentes, Stop, expiração, quota/recursos
insuficientes ou UNKNOWN material negam progressão e preservam retenção.
Nenhum KSP, rede produtiva, serviço adicional, ControlService Stop, cleanup,
recuperação, refund ou prontidão produtiva. A autorização futura deste ato,
isoladamente, não é suficiente para iniciar a sequência dos três atos.
