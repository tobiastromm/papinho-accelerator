---
capability: network-egress
last-updated: 2026-09-13
scope: project
status: partial
title: Network Egress
---

# Network Egress

## Objetivo

Documentar a autoridade/capability que controla se o PapinhoAccelerator pode
abrir conexões externas em nome de um cliente,
sem confundir compute remoto com permissão para usar a rede ou o
endereço público do servidor.

Este documento é a **documentação viva factual da capability**.

Direções arquiteturais futuras ainda não congeladas por ADR ficam
separadas em:

``` text
docs/future/network-egress-policy.md
```

## Escopo

### Inclui

-   distinção entre egress realizado pelo cliente e pelo Accelerator;
-   configuração, policy e autorização explícitas;
-   validação futura de destinos, protocolos, portas e redirects;
-   limites, quotas, observabilidade e cleanup futuros;
-   relação com Capability Framework e Session Capability Snapshot.

### Não inclui

-   proxy genérico ou interpretação de protocolo de aplicação;
-   concessão automática por outra capability;
-   Transport Security Client--Accelerator;
-   definição de DNS, HTTP ou TLS offload completos;
-   sintaxe wire ou API ainda não decidida;
-   firewall genérico do host/rede;
-   Web Proxy/Application Filtering;
-   TLS interception.

## Comportamento esperado

``` text
remote compute != network egress permission
```

Egress pelo Accelerator exige policy/autorização explícita. Backend
presente, pedido do cliente ou capability de compute autorizada não
concedem egress por si sós.

São fluxos distintos:

``` text
NETWORK_EGRESS_CLIENT
→ cliente abre a conexão externa

NETWORK_EGRESS_ACCELERATOR
→ Accelerator abre a conexão externa
```

A distinção descreve a origem real da conexão e da identidade de rede;
não congela IDs de capability ou wire format.

## Relação com a arquitetura de capabilities

A autoridade geral continua governada pelo ADR-0005:

``` text
SERVER_SUPPORTED
        ∩ SERVER_ENABLED
        ∩ PRINCIPAL_ALLOWED
        ∩ CLIENT_SUPPORTED
        ∩ CLIENT_PREFERENCE
        = EFFECTIVE_CONFIGURATION
```

Capability, backend e policy permanecem conceitos distintos.

O ADR-0011 define a identidade portátil de capabilities por key numérica
estável + semantic major version, mas **nenhum ID concreto para Network
Egress é atribuído por este documento**.

O ADR-0012 define o Session Capability Snapshot como upper bound
imutável.

Para Network Egress isso significa:

``` text
NETWORK_EGRESS ∈ snapshot
→ policy contextual pode negar imediatamente
→ policy contextual pode reabilitar depois
→ somente porque a capability já fazia parte do snapshot
```

Por outro lado:

``` text
NETWORK_EGRESS ∉ snapshot
→ policy não pode adicioná-la silenciosamente
→ nova Session
   OU
→ futura renegociação explícita
```

A renegociação futura permanece sem wire/protocolo definido.

## Arquitetura / fluxo atual conceitual

``` text
authenticated/authorized request
        ↓
server configuration + Principal policy + capability policy
        ↓
destination/protocol/port validation
        ↓
quota/resource admission
        ↓
external connection owned by an egress component
```

Falha ou ambiguidade deve negar egress. A conexão externa não pertence
implicitamente a um Compute Backend.

A arquitetura futura mais detalhada de Network Egress Policy e Egress
Profiles está preservada em:

``` text
docs/future/network-egress-policy.md
```

Esse documento futuro não declara implementação nem cria sozinho novas
decisões accepted.

## Exemplos de uso / configuração

O Configuration Model já contém o booleano `allow_network_egress`. Ele é
uma entrada administrativa parcial, não prova que egress esteja
implementado.

Exemplo conceitual:

``` text
allow_network_egress = FALSE
remote compute        = AVAILABLE
```

Essa combinação é válida.

## Estado atual

Status geral: `partial`.

### Marco Phase 6.E1b

O runtime Win32 possui agora a menor composição de establishment de
`TCP_PLAIN`: uma configuração explícita injeta Destination Policy, catálogo e
seleção server-owned de Egress Profile; somente depois da resolução completa o
servidor reserva um Stream oculto, admite um Job e inicia connect não bloqueante
para um endpoint IP numérico já validado. O sucesso só é publicado depois de o
socket ser transferido exatamente uma vez ao Stream reservado.

Esse marco ainda não implementava forwarding. O estado atual posterior possui
resolver injetável e, na composição Win32 habilitada, resolver WinSock explícito:
hostname é normalizado e autorizado antes do DNS; cada endpoint numérico é
filtrado antes do connect, sem segunda resolução. Sem configuração concreta
injetada, o servidor permanece deny-all.

### Marco Phase 6.E2

Streams `TCP_PLAIN` ACTIVE realizam forwarding bidirecional de bytes opacos.
`NETWORK_DATA` fragmentado alimenta o TX bounded; writes remotos parciais
consomem somente o prefixo aceito. Reads remotos alimentam o RX bounded e são
interrompidos quando ele está cheio; a saída usa frames `NETWORK_DATA`
incrementais e só consome RX depois do frame completo.

O scheduler oferece por passe uma oportunidade bounded para Job e uma para
Stream, com rotação entre Streams e preferência por alternar Sessions. A
capacidade factual atual é 1 KiB por direção e por Stream e o quantum é 256
bytes; ambos são limites de implementação por instância, não constantes
arquiteturais globais.

### Readiness e throughput de Network Stream

Na composicao Secure Principal Win32, conexoes PST outbound publicadas por
Streams `TLS_REQUIRED` participam do mesmo wait-set usado pelo transporte
seguro inbound. Readiness outbound dispara progresso imediatamente, sem usar o
timeout periodico de 50 ms como relogio de payload.

Cada wake possui budget factual de 32 operacoes. Cada operacao preserva o
quantum de 256 bytes e a selecao round-robin/session-aware ja existente, logo
um wake processa no maximo 8 KiB antes de devolver controle ao scheduler.
Uma rodada completa sem progresso encerra o drain antecipadamente e retorna ao
wait. O RX/TX continua limitado a 1 KiB por direcao e por Stream; buffer cheio
interrompe reads upstream e preserva backpressure PACC. Esses valores sao
limites atuais de implementacao, nao constantes arquiteturais globais.

O registro no wait-set e removido antes do progresso bounded, permitindo que
cancel, terminal ou shutdown liberem a conexao PST sem deixar registro
pendente. Apenas Streams ainda vivos sao registrados novamente.

### Marco Phase 6.E3

O lifecycle produtivo de `TCP_PLAIN` está fechado. `NETWORK_HALF_CLOSE`
solicita o fechamento direcional de envio e aguarda o TX bounded drenar antes
de `shutdown(send)`; novos `NETWORK_DATA` do cliente são rejeitados. A direção
remota continua ativa.

EOF remoto preserva todo RX já recebido. Somente depois do último frame
`NETWORK_DATA` ser concluído e consumido o servidor emite
`NETWORK_HALF_CLOSE`; com ambas as direções encerradas, o Stream e seu DATA
Channel terminam deterministicamente. `NETWORK_CLOSE` realiza fechamento
terminal normal e `NETWORK_ABORT` teardown excepcional imediato, ambos sem
reuso silencioso do mesmo DATA Channel.

Perda de DATA fecha exatamente o Stream vinculado. Perda de CONTROL/Session
cancela establishments, fecha todos os Streams da Session e remove seu
Capability Snapshot. Shutdown fecha Jobs e Streams de forma bounded. Cleanup
zera comprimentos e storage dos buffers antes de devolver o slot ao pool.

### Resumo por área

  -------------------------------------------------------------------------------------------
  Área                     Estado                    Observação
  ------------------------ ------------------------- ----------------------------------------
  Capability Framework     implemented               key/registry/sets/snapshot/enforcement
                                                     e negociação inicial integrados

  Capability identity      implemented               `NETWORK_EGRESS_V1` e `NETWORK_EGRESS_V2`
                                                     possuem keys factuais; `TLS_OFFLOAD` permanece
                                                     ortogonal

  Session Snapshot         implemented               upper bound e enforcement contextual
                                                     aplicados antes de Network Open

  Configuration Model      partial                   `allow_network_egress` existe e é
                                                     validado/copiado

  CLI source               partial                   opção existente compõe o campo de
                                                     configuração

  Policy por               partial                   boundary concreta existe; produção sem
  Principal/Session                                  policy injetada permanece deny-all

  External connection      TCP_PLAIN implemented     server runtime possui connect, forwarding e
                                                     lifecycle completos para fixture/policy explícita

  Destination validation   partial                   hostname/IPv4/IPv6, DNS WinSock produtivo
                                                     e filtro de classes sensíveis; redirects ausentes

  Egress Profiles          partial                   catálogo bounded, seleção server-owned e
                                                     DEFAULT_ROUTE explícito

  DNS/redirect handling    partial                   DNS implementado; redirects ausentes

  Quotas/observabilidade   partial                   limites bounded existem; management e
                                                     accounting de produto permanecem futuros

  Wire negotiation         implemented               v1 e v2; cada Session continua limitada a
                                                     duas keys negociadas
  -------------------------------------------------------------------------------------------

## Implementado

-   campo portátil `allow_network_egress` no Server Configuration Model;
-   initializer, validation/copy e parsing CLI associados;
-   testes da composição/configuração desse campo;
-   harness opt-in isolado de DNS/TCP para `google.com:443`, usado
    somente pela prova PST/TLS 1.3;
-   Capability Framework genérico da Phase 4;
-   Capability Key, registry/sets bounded e identidade por `(id, major)`
    genéricos;
-   Session Capability Snapshot e enforcement contextual genéricos.
-   keys/registry de `NETWORK_EGRESS_V1` `(1,1)`, `NETWORK_EGRESS_V2` `(1,2)`
    e `TLS_OFFLOAD` `(2,1)`, com storage global para as três keys e limite
    por negociação preservado em duas;
-   snapshot imutável por Session: v1 e v2 são alternativas negociadas, não
    uma troca de major em runtime;
-   Destination Policy e Egress Profile como autoridades distintas;
-   `allow_network_egress` como master gate, nunca como allow-all;
-   ausência de policy/profile e destinos sensíveis sem allow explícito negados;
-   reserva oculta, Job de establishment, connect Win32 não bloqueante e
    publicação failure-atomic do Stream;
-   `NETWORK_OPEN_RESULT` emitido após a publicação bem-sucedida;
-   cleanup por perda de DATA/Session e shutdown.
-   half-close TCP direcional após drain de TX;
-   EOF remoto com drain de RX antes do half-close reverso;
-   `NETWORK_CLOSE` normal e `NETWORK_ABORT` excepcional;
-   truncamento TLS mapeado conforme o major negociado: v1 emite
    `NETWORK_ABORT` vazio; v2 preserva todo `NETWORK_DATA` já produzido e
    termina uma única vez com `NETWORK_ABORT_REASON(TLS_TRUNCATED=1)`, payload
    big-endian `00 01`; o Stream terminal não é reutilizado;
-   reap, zeroização e reutilização bounded de slots;
-   isolamento das cascatas DATA e Session.
-   integração wire/runtime de `0x0007`..`0x000E`, com registry sem conflitos,
    parsing fragmentado/coalescido, validação estrutural bounded, matriz de
    estado, descarte seguro de frame parcial e reuso limpo do slot;
-   ausência de IDs runtime e de material interno de policy/security nos
    payloads serializados.

Esses itens, junto às Phases 6.F–6.I, fecham o lifecycle outbound produtivo de
`TCP_PLAIN` e `TLS_REQUIRED` Mode B sob policy/profile explicitamente injetada.

## Parcialmente implementado

-   configuração operacional além do profile server-owned atual;
-   quotas/accounting e management de produto;
-   integração com consumidores reais além do cliente de referência.

## Não implementado

-   proxy;
-   redirects;
-   explicit egress failover;
-   IDNA/Unicode;
-   Browser integration e Mode C;
-   Web Proxy/Application Filtering;
-   TLS interception.

## Configuração

`allow_network_egress` é `FALSE` por padrão. `TRUE` apenas habilita avaliação;
o modelo normalizado também exige `egress_policy_ref` e
`egress_profile_ref`, resolvidas pela aplicação antes de RUN. A implementação
Win32 atual publica snapshot imutável e bounded de até 16 regras, rejeita
duplicatas e aceita somente hostname ASCII normalizado + porta exata e endpoint
IPv4 canônico + porta exata. Wildcards, suffix, CIDR, subnet, port range e
explicit deny não fazem parte da v1.

Para hostname, a autorização do nome ocorre antes de uma única resolução; cada
candidato numérico precisa também corresponder a uma regra endpoint+porta exata.
Permissão de classe sensível nunca substitui essa correspondência. Para literal,
somente endpoint+porta exata se aplica. `Destination Policy != Egress Profile` e
`LEGACY_ENDPOINT_NETWORK_EGRESS_V1=NO` permanecem invariantes.

O Configuration Model também possui `outbound_tls_profile_ref`, governado pelo
ADR-0027. Ele resolve separadamente o trust/security profile PST CLIENT para
`TLS_REQUIRED`; Destination Policy continua respondendo apenas “onde”, Egress
Profile apenas “por qual link” e Outbound TLS Security Profile “como autenticar
e proteger o site remoto”. `INBOUND_CLIENT_TRUST != OUTBOUND_SITE_TRUST`.
não concede egress sem capability, policy, profile e autorização adicionais.

CLI e futuras interfaces devem convergir para o mesmo Configuration
Model, conforme a governança transversal de configuração.

O campo atual não substitui a futura Capability Key nem a policy
contextual.

## Compatibilidade

O build Win32 PST-enabled validado fornece Network Egress produtivo v1. Isso
não declara suporte universal a outros targets ou integração com Browser.

O harness foi testado no host do target PST
`win32-x64-msvc-19.51-openssl3`; isso não constitui backend de egress
nem suporte da capability.

A Phase 4 é framework genérico e também não constitui suporte de Network
Egress.

## Limitações conhecidas

-   somente configuração parcial;
-   nenhuma conexão externa de produção;
-   nenhum contrato congelado de destination policy;
-   nenhum Egress Profile implementado;
-   nenhum modelo concreto de quotas/observabilidade de egress;
-   nenhum wire negotiation para a capability;
-   future architecture directions deliberadamente separadas da
    factualidade deste Capability Document.

## Pendências

-   [ ] Definir, na fase apropriada, Network Egress Policy concreta.
-   [ ] Definir Egress Profiles e autoridade de seleção.
-   [ ] Definir validação de hostname/endereço/subnet e porta/range.
-   [ ] Tratar localhost, loopback, private, link-local e redes de
    management.
-   [ ] Tratar DNS rebinding e mudanças de resolução.
-   [ ] Definir política de redirects e revalidação de cada destino.
-   [ ] Definir quotas de conexões, bytes, tempo, bandwidth e
    concorrência.
-   [ ] Definir ownership, cancellation e cleanup da conexão externa.
-   [ ] Definir observabilidade sem expor dados sensíveis.
-   [ ] Definir relação produtiva com `TLS_OFFLOAD` sem concessão
    implícita.
-   [ ] Definir a futura Capability Key concreta somente quando a
    capability amadurecer.
-   [ ] Definir wire negotiation somente por specification/decisão
    apropriada.
-   [ ] Avaliar ADRs futuras indicadas em
    `docs/future/network-egress-policy.md`.

## Ideias futuras / direções preservadas

As direções aprovadas mas ainda não congeladas como implementação estão
em:

``` text
docs/future/network-egress-policy.md
```

Entre elas:

-   Network Egress Policy contextual;
-   Egress Profiles;
-   destination policy;
-   no-silent link fallback;
-   SSRF/proxy-abuse protections;
-   fronteira contra firewall genérico;
-   separação de Web Proxy/Application Filtering;
-   separação de TLS interception.

O documento futuro também identifica quais pontos merecem futura
avaliação para ADR e quais devem permanecer decisões abertas.

## Questões em aberto

-   Quais protocolos externos serão permitidos?
-   Como nomes e endereços serão normalizados e revalidados?
-   Qual será a semântica concreta de DNS e DNS rebinding?
-   Redirects serão permitidos em quais condições?
-   Como Egress Profiles serão persistidos/configurados?
-   Como routing/failover será implementado?
-   Qual componente possuirá a conexão externa?
-   Qual será a Capability Key concreta?
-   Como será o wire negotiation?
-   Como a futura renegociação explícita do ADR-0012 será representada
    no protocolo?

## ADRs relacionadas

-   `PapinhoAccelerator/ADR-0002` --- Core portável entre deployments e
    backends substituíveis.
-   `PapinhoAccelerator/ADR-0005` --- Negociação de capabilities,
    disponibilidade de backend e autoridade de policy.
-   `PapinhoAccelerator/ADR-0006` --- Perfil de Transport Security e
    credenciais do Secure Principal.
-   `PapinhoAccelerator/ADR-0011` --- Identidade de capability e modelo
    portátil limitado.
-   `PapinhoAccelerator/ADR-0012` --- Snapshot de capabilities da
    Session e enforcement contextual.
-   `PapinhoAccelerator/ADR-0015` --- Establishment Jobs e lifecycle de
    Network Stream.
-   `PapinhoAccelerator/ADR-0017` --- protocolo Network Offload em DATA.
-   `PapinhoAccelerator/ADR-0018` --- registry de Network Egress/TLS Offload.
-   `PapinhoAccelerator/ADR-0019` --- Network Egress Policy e Profiles.
-   `PapinhoAccelerator/ADR-0021` --- destinos sensíveis e prevenção de abuso.
-   `PapinhoAccelerator/ADR-0024` --- boundary concreta de configuração.
-   `PapinhoEngineering/ADR-0004` --- Modelo compartilhado de
    configuração e separação entre Core e Frontends.
-   `PapinhoEngineering/ADR-0005` --- Governança da documentação viva de
    capabilities.

## Código relevante

-   `apps/server/server_config.h`
-   `apps/server/server_config.c`
-   `apps/server/server_cli.c`
-   `src/capability/`
-   `src/network_egress/`
-   `src/platform/win32/tcp_outbound_win32.c`
-   `apps/server/server_io_loop_win32.c`
-   `tests/pst_tls13_outbound_proof.c` --- prova isolada, fora do
    servidor.

## Testes / evidências

-   `tests/server_config_test.c` --- initializer, validação e cópia do
    campo;
-   `tests/server_cli_test.c` --- parsing e default da opção CLI;
-   `tests/server_network_test.c` --- composição aceita a configuração
    sem implementar egress;
-   testes da Phase 4 --- evidenciam o framework genérico de capability,
    snapshot e enforcement, não Network Egress de produção.
-   `tests/phase6_portable_test.c` --- policy/profile, protocolo, Job e Stream;
    inclui volumes deterministas de 64 KiB e 1 MiB, fairness entre dois
    Streams, observacao explicita de `WOULD_BLOCK`, backpressure bounded e
    cancel/cleanup;
-   `tests/server_io_loop_win32_test.c` --- negociação, DATA, policy loopback
    exata, connect local real, publicação, 32 KiB de forwarding binário
    bidirecional fragmentado, accounting bounded e cleanup.
-   `tests/server_secure_io_win32_integration_test.c` --- CONTROL/DATA seguros,
    `TLS_REQUIRED` produtivo via PST, 32 KiB binários e negativos TLS 1.2-only,
    trust, hostname, ALPN, stall e reset com cleanup e recovery.

O mesmo E2E comprova `NETWORK_CLOSE` de `TLS_REQUIRED` com shutdown PST
incremental, `close_notify` recíproco e cleanup bounded. A Phase 6.F2 também
possui harness hermético com múltiplos Streams TLS produtivos, coexistência
TLS/`TCP_PLAIN`, fairness por Session e por Stream, isolamento de falhas,
cascatas de DATA/Session/server shutdown e reutilização de capacidade. Assim,
a integração PST outbound da Phase 6.F está concluída; Phase 6 permanece aberta
para as etapas posteriores, sem antecipar 6.G/6.H.

Code/Tests evidenciam somente os estados descritos acima.

### Marco concluído Phase 6.H

O cliente de referência seguro executa em processo independente e usa apenas
os codecs/framing portáveis do PACC e a API CLIENT pública do PST. Pela
composição real do servidor ele comprova CONTROL seguro, negociação exata,
DATA seguro independente e Network Offload em `TCP_PLAIN` e `TLS_REQUIRED`.

A matriz hermética inclui 32 KiB binários fragmentados, half-close TCP com
resposta após EOF, close e abort. Cenários CTest isolados, cada qual com nova
instância do servidor, comprovam ausência de negociação, ausência de
`TLS_OFFLOAD`, destino negado e hostname TLS incorreto. Uma única Session
adicional comprova recuperação depois de DATA terminal, dois DATA Channels e
Streams simultâneos sem mistura de bytes, isolamento de abort e novo DATA
depois do terminal. O cliente não recebe Session, Job ou Stream runtime IDs e
não escolhe profile, source IP, resolver, trust ou policy.

O harness opt-in evidenciou DNS e TCP para destino fixo
`google.com:443`, sem aceitar destino arbitrário do cliente, sem proxy e
sem integrar um caminho Network Egress de produção.

### Marco concluído Phase 6.I

A matriz adversarial portátil valida de forma isolada os gates de autorização,
configuração e destino. Sem Secure Principal, snapshot publicado,
`NETWORK_EGRESS`, policy contextual favorável e capability ainda habilitada no
servidor, a abertura falha fechada. `TLS_REQUIRED` exige adicionalmente
`TLS_OFFLOAD`. O Session Capability Store é limitado, não publica parcialmente
ao esgotar e não permite lookup cruzado por identidade de Session.

`allow_network_egress`, Destination Policy e Egress Profile são três gates
independentes. Ausência ou erro em qualquer um falha sem default route ou troca
silenciosa de profile. A policy é aplicada ao hostname normalizado e a cada
endpoint numérico da única resolução DNS. IPv4-mapped IPv6 é canonicalizado
antes da classificação. Loopback, link-local, RFC1918/ULA, CGNAT e metadata
exigem autorização explícita de todas as classes aplicáveis. Endpoints de
management, listeners do próprio Accelerator e public-unicast continuam sob a
callback administrativa deny-by-default; ser público não concede acesso.

Os testes E2E existentes permanecem os gates para isolamento entre DATA
Channels, tickets/Principal, lifecycle, malformed wire, capacidade de
Jobs/Streams e matriz TLS real. A revisão de wire confirma que o cliente não
seleciona profile, interface/source IP, resolver, trust source ou SNI separado;
o nome TLS deriva do destino normalizado. A revisão de logging e resultados de
wire não encontrou serialização de payloads, credenciais, certificados DER,
diagnósticos brutos do provider ou IDs internos de Stream/Job.

## Documentos relacionados

### Fixture externa Mode B para consumers

O executavel de teste `papacc_secure_browser_fixture` expoe o cenario
`mode-b-egress`. Ele publica um hostname e uma porta efemera, configura o
resolver controlado ja usado pela matriz Phase 6 e aceita `NETWORK_OPEN` com
`TLS_REQUIRED` somente apos a negociacao de `NETWORK_EGRESS` e `TLS_OFFLOAD`
por um Secure Principal real. O target HTTPS local aceita request HTTP
fragmentado e devolve uma resposta deterministica sobre o stream PACC.

O cenario complementar `mode-b-tls-name-failure` prova falha fechada de
hostname, sem fallback plaintext. Ambos sao fixture-only: nao alteram listener,
resolver, defaults ou policy do servidor de producao.

O target controlado permanece aceitando conexoes sequenciais enquanto a
fixture estiver ativa. Cada nova Session cria seu proprio site TCP/PST e pode
concluir independentemente `NETWORK_OPEN`, HTTP, `NETWORK_DATA` e
`NETWORK_CLOSE`; o stop event encerra o accept loop de forma limitada.

### Fixture externa TCP_PLAIN para consumers

O cenario test-only `tcp-plain-egress` de
`papacc_secure_browser_fixture` expoe o caminho produtivo Phase 6 sem TLS no
site transport. O primeiro hop permanece Secure Principal real com TLS 1.3,
mTLS e ALPN `papacc/1`; cada operacao usa ticket e DATA Channel novos sobre uma
CONTROL Session persistente.

O ready-file publica hostname controlado `papacc-plain-test-server`, porta
efemera, `target_scheme=http`, `network_mode=TCP_PLAIN`, resposta esperada
`papacc-tcp-plain-ok` e contagem sequencial tres. O resolver controlado mapeia
o hostname ao endpoint loopback validado. O target aceita tres conexoes TCP
plaintext e nao cria contexto PST para o site transport.

``` text
PACC Secure Transport PST used = yes
site TLS PST used              = no
site TLS location              = none
TCP_PLAIN requires TLS_OFFLOAD = no
```

O Accelerator encaminha bytes opacos e nao interpreta HTTP. A fixture e uma
superficie de teste para consumers; defaults, listeners, resolver e policy de
producao permanecem inalterados.

### Fixture externa mista para consumers

O cenario test-only `mixed-egress` combina, sob uma unica CONTROL Session de
Secure Principal, operacoes sequenciais `TLS_REQUIRED`, `TCP_PLAIN` e
`TLS_REQUIRED`. `NETWORK_EGRESS` e `TLS_OFFLOAD` sao negociadas uma unica vez;
cada operacao recebe ticket novo e usa DATA Channel e Network Stream novos.

O ready-file v2 publica separadamente os targets controlados TLS e plaintext,
suas portas efemeras, schemes, transport modes e respostas esperadas. O alvo
TLS usa PST, validacao de certificado e hostname `papacc-test-server`; o alvo
plaintext usa `papacc-plain-test-server` e nao cria contexto PST no site
transport. O resolver e a policy continuam controlados e limitados a esses
endpoints loopback.

Essa composicao comprova que a Session Capability Snapshot permanece estavel
entre operacoes heterogeneas e que `TLS_OFFLOAD` autoriza apenas a operacao
externa `TLS_REQUIRED`. Ela nao transforma `TCP_PLAIN` em TLS, nao confunde
Transport Security com TLS Offload e nao altera comportamento de producao.

### Fixture de terminal TLS para consumers HTTP

`papacc_secure_browser_fixture` oferece cenarios test-only que atravessam o
caminho real Secure Principal, negociacao `NETWORK_EGRESS` major 2,
`TLS_OFFLOAD`, DATA Channel e origin HTTPS controlado:

``` text
tls-truncated-content-length-complete
tls-truncated-content-length-incomplete
tls-truncated-chunked-complete
tls-truncated-chunked-incomplete
tls-truncated-close-delimited
tls-clean-close
tls-truncated-content-length-complete-multi-frame
tls-truncated-content-length-incomplete-multi-frame
```

Nos cenarios `tls-truncated-*`, o origin entrega os bytes autenticados e
encerra o transporte sem `close_notify`; o Accelerator produz
`NETWORK_ABORT_REASON(TLS_TRUNCATED)`. `tls-clean-close` executa shutdown TLS
autenticado e produz `NETWORK_HALF_CLOSE`. As variantes sem sufixo cabem em um
unico `NETWORK_DATA`; as variantes `multi-frame` preservam exatamente os mesmos
bytes HTTP e fragmentam a escrita controlada para exigir multiplos frames,
sempre com o terminal posterior ao ultimo DATA.

O ready-file versao 3 preserva os campos de conexao existentes e acrescenta
major negociado, grant de TLS Offload, framing, comprimentos, completude
esperada, intencao de `close_notify`, terminal, razao e fragmentacao. Esses
campos descrevem somente a fixture; a decisao HTTP permanece no Browser.

O corpo esperado e publicado como marcador textual em `expected_HTTP_body` e
o LF final factual e declarado separadamente por
`expected_HTTP_body_trailing_lf=YES`. `expected_HTTP_body_length` e
`delivered_body_length` incluem esse LF. Resposta e metadados provem do mesmo
descritor test-only, cuja consistencia com Content-Length, chunk framing,
completude e terminal e validada antes da publicacao do ready-file.

### Fixture Secure de fallback de major

O cenario test-only `secure-v2-unavailable-v1-fallback` desabilita v2 somente
no conjunto efetivo da fixture, preservando v1 e `TLS_OFFLOAD`. Uma primeira
Secure Principal Session que oferece v2 recebe um resultado valido sem o grant
de Network Egress; apos seu encerramento, uma nova Secure Principal Session
pode oferecer v1, recebe o grant e completa uma operacao HTTPS controlada.
Nao existe renegociacao in-place nem participacao do endpoint Legacy.

Os cenarios factuais `secure-v2-protocol-error-no-fallback`,
`secure-v2-auth-failure-no-fallback`,
`secure-v2-transport-failure-no-fallback` e
`secure-v2-egress-started-no-fallback` publicam os controles negativos para o
consumer. A fixture canonica nao consegue emitir um resultado de capability
malformado: o processor de servidor sempre usa o codec validado. O ready-file
versao 4 declara esse gap explicitamente; ele nao e aproximado por Legacy,
falha de autenticacao ou indisponibilidade valida.

As realizacoes negativas sao igualmente test-only: no caso de protocolo, o
sink sincrono da fixture instala uma gate interna e nula por default na
instancia do processor durante `CONTROL_ESTABLISHED`, antes de a transicao
devolver controle ao scheduler. A gate e observada dentro do processamento de
capabilities somente depois do decode integral da oferta v2 valida e antes da
publicacao do snapshot ou construcao do resultado, retornando
`PROTOCOL_ERROR`. Os marcadores de decode e realizacao sao publicados apenas
nesses boundaries factuais; o caso de
autenticacao usa um certificado valido da CA, mas nao enrolled, e falha antes
de CONTROL; o caso de transporte solicita shutdown limitado da composicao
Secure depois da autenticacao e antes de CONTROL; o caso pos-egress concede v2,
inicia o Network Stream real e recebe `TLS_TRUNCATED` do origin controlado.
`papacc.browser_fixture.fallback_negative_matrix` comprova cada primeira
fronteira factual e confronta-a com o ready-file. O teste
`papacc.browser_fixture.protocol_error_repetition` repete cem execucoes
concorrentes independentes sem atraso artificial no client.

-   `docs/capabilities.md`
-   `docs/capabilities/tls-offload.md`
-   `docs/future/network-egress-policy.md`
-   `docs/networking.md`
-   `docs/security-model.md`
-   `docs/phase4-capability-framework-policy-engine.md`

## Princípio

``` text
Capability Document
→ estado factual e vivo da capability

docs/future/
→ direção arquitetural adiada ainda não congelada como ADR

ADR
→ decisão durável aceita
```

## Histórico de mudanças

  -----------------------------------------------------------------------------
  Data                                Descrição
  ----------------------------------- -----------------------------------------
  2026-09-06                          Criação inicial do Capability Document
                                      com estado factual de configuração
                                      parcial e egress não implementado.

  2026-09-06                          Registrada prova outbound isolada para
                                      destino fixo, sem promover Network Egress
                                      de produção a implementado.

  2026-09-12                          Alinhado à Phase 4, ADR-0011/0012 e ao
                                      novo
                                      `docs/future/network-egress-policy.md`;
                                      preservado Network Egress como capability
                                      ainda não implementada em produção.

  2026-09-12                          Atualizado para a Phase 6.E1b: boundary
                                      concreta fail-closed e establishment
                                      TCP_PLAIN com fixture local real.

  2026-09-12                          Atualizado para a Phase 6.E2: forwarding
                                      TCP_PLAIN bidirecional, buffers bounded,
                                      backpressure e fairness session-aware.

  2026-09-12                          Atualizado para a Phase 6.E3: half-close
                                      direcional, EOF/drain ordenado, close,
                                      abort, cascatas, reap e reuse.
  -----------------------------------------------------------------------------

## P10 qualification checkpoint — 2026-10-09

The cumulative evidence is in [p10-resume-20261009.txt](p10-resume-20261009.txt).
R1 with removal injection OFF passed with the TEST_ONLY TLS write gate BLOCK.
P10 once and normal-removal checks passed; persistent Stop removal remains
unqualified because NORMAL unregister can bypass REMOVE_PENDING retention.
Qualification stopped due to the observed integrity risk under ADR-0030.
The once fixture does not prove all ADR-0032 P10 pre-close pending obligations.

## P10 persistent bypass correction — 2026-10-09

The secure owner now retains resources throughout Stop removal failure and
`ERROR_TERMINAL`. NORMAL unregister, late dispatch, candidate close and the
generic Connection pre-close cannot replace successful STOP removal. The owner
continues pending close intent after dispatch; no API/ABI, PST or wire change was
made. These are implementations of ADR-0030/0031/0032, not new decisions.

The governed PST 0.6.5 diagnostic NORMAL, FAIL_ONCE and FAIL_PERSISTENT scenarios
passed, including native per-token counters and negative oracles. Both normal
PST 0.6.4 and diagnostic builds passed with zero warnings. Full P10 qualification
remains open: the normal 0.6.4 TLS-transfer baseline still emits NETWORK_ABORT
(the exact pre-task owner snapshot reproduces it), and concurrent fixture B is
unstable despite its isolated PASS. Broader independent baseline failures remain
recorded. P11/P12 were not executed as separate qualification slices.

See [the cumulative report](p10-resume-20261009.txt) for hashes, test scope,
per-token evidence, failures and the next governed investigation. Relevant future
direction was reviewed; it remains future direction and does not prove these
gates qualified.

### Baseline reconciliation checkpoint — 2026-10-09

The fixture listener had a preexisting test harness defect: it examined the
FD_ACCEPT error slot when the event mask did not contain FD_ACCEPT. Winsock
leaves that slot untouched. The fix checks the event bit and initializes the
record; genuine listener failures still fail. Four event-mask negatives pass.
Diagnostic A/B/C each passed 100 concurrent repetitions; production PST 0.6.4
B passed 100 repetitions. A/B/C/F also pass in both full suites.

Pinned PST 0.6.4 still fails the upstream clean-close transfer. Public traces
show read CLOSED/source OK followed by reciprocal shutdown INVALID_STATE,
stream cleanup and PACC NETWORK_ABORT. Release commit
9e32c84dad6d8a84d6f1119ddde88ffe57583c89 confirms read makes the core terminal and
shutdown does not accept that state. This contract mismatch predates the P10
bypass fix. It cannot satisfy ADR-0023's reciprocal completion gate, so it is
not converted to success. Production pin, PST and the normative oracle remain
unchanged.

All six P10 modes passed again, including persistent ERROR_TERMINAL retention
and the native/state negatives. Both full builds pass without warnings.
Full CTest results remain diagnostic 88/92 and production 84/91. Independent
baseline failures and a concurrent owner IO failure (serial retry passed) remain
visible; full P10/ADR-0032 qualification is not claimed. See the cumulative
[reconciliation evidence](p10-resume-20261009.txt), including hashes and logs.
The related [future direction](../future/network-egress-policy.md) was reviewed;
this fixture correction does not change its implementation status or decisions.

### Remaining regression triage and P10 boundary - 2026-10-09

The previous eleven suite failures map to eight test identities and five cause
groups. Four contained harness defects are corrected: the DATA association test
now initializes its optional pre-close callback; portable Stream tests assert
retention and explicit owner finalization while keeping the original
idempotence checks; owner IO waits for actual upstream byte delivery instead of
a fixed call count; and the fixture launcher performs real HTTP work before
requesting a successful Stop. Normal and diagnostic focal checks pass.
Portable/DATA each pass twenty repetitions, owner IO passes ten, and impacted
Mode-B and negative regressions pass. Full builds pass with zero warnings.
The earlier full-suite results are preserved; no new whole-suite run is claimed.

Four production identities still share one PST 0.6.4 integration cause. A current
clean-close trace confirms read CLOSED/CLEAN followed by shutdown INVALID_STATE.
The consumer's PEER_CLOSED interpretation assumes reciprocal shutdown remains
possible, while pinned064 has already made its core terminal. The provider read
and PACC error mapping are not proved defective. A consumer-only success path
cannot be justified using the available contract; upstream lifecycle/dependency
reconciliation requires owner action within the existing restrictions.

The six P10 and four-hundred fixture qualification executions were not repeated.
They remain valid in their declared configurations. Project P10 qualification is
still open because those065 diagnostic Stop/pre-close positives do not establish
equivalent pending-TLS behavior for production064. The
[cumulative report](p10-resume-20261009.txt) records every test's causal relationship,
the positive/negative evidence matrix, mandatory blocker, hashes and trace IDs.
Separate P11/P12 and full ADR-0032 qualification are not inferred.
The [future direction](../future/network-egress-policy.md) remains unchanged.

### PST 0.6.5 normal candidate reconciliation (2026-10-09)

The governed normal x64 OpenSSL SDK (API 2.2.0 / SPI 3.0) completes reciprocal
shutdown after peer CLOSED/CLEAN and delivers NETWORK_CLOSE. Equivalent fixed
endpoints reproduce INVALID_STATE with normal 0.6.4. The normal 0.6.5 Release
consumer uses /MD and passes 91/91 regressions with zero build warnings.

Consumer-only diagnostic P10 fail-once passes its existing negatives. Persistent
removal reaches ERROR_TERMINAL with owner structures and membership retained.
These observations do not supply the actual native-lifetime oracle available
only in the separate correlated diagnostic. The old pending-TLS OFF fixture
also depends on a test-only write gate absent from the normal package.
Candidate qualification and project P10 qualification remain open; production
pin remains 0.6.4 and promotion has not occurred. The cumulative
[report](p10-resume-20261009.txt) records package/component hashes, current-run
traces, normal/correlated evidence boundaries, harness corrections and open gates.
