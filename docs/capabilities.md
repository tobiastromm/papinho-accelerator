# Capabilities e configuração

O [ADR-0005](adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md)
é a autoridade arquitetural canônica para a separação entre capability,
disponibilidade de backend, policy e configuração efetiva. Este documento
permanece como índice e visão geral viva.

**Estado de implementação:** o framework portátil da Phase 4 está implementado:
Capability Key, registry/sets bounded, interseção fail-closed, snapshot de
Session e enforcement contextual. Negociação wire e execução de workloads não
estão implementadas. Os nomes abaixo continuam conceitos futuros e não recebem
IDs concretos nesta fase.

Capability Documents disponíveis:

- [Logging e diagnóstico operacional](capabilities/logging.md);
- [TLS Offload para conexões externas](capabilities/tls-offload.md);
- [Network Egress](capabilities/network-egress.md);
- [Client Pairing e Enrollment](capabilities/client-pairing.md).

Capabilities são unidades independentes, extensíveis, negociáveis e
versionáveis. O ADR-0011 fixa a forma da identidade como ID numérico estável +
major version, mas esta baseline não atribui números às capabilities futuras.

```text
NETWORK                 TLS_OFFLOAD
IMAGE_DECODE
VIDEO_DECODE            VIDEO_STREAMING
VIDEO_TRANSCODE         VIDEO_REMOTE_FRAMES
AUDIO_DECODE            AUDIO_STREAMING
AUDIO_TRANSCODE
HTML_PARSING            CSS_LAYOUT
DISPLAY_COMMANDS        FRAMEBUFFER
```

Os nomes são iniciais. Anunciar uma capability não implica que todas as opções, formatos ou versões dela sejam aceitos.

`TLS_OFFLOAD` é o nome conceitual, ainda não congelado, da capability que poderá auxiliar ou executar operações TLS para conexões do cliente com sites e serviços externos. Ela pertence ao Capability Framework e não representa a segurança da conexão PapinhoAccelerator Client–Server.

Os [ADR-0001](adr/ADR-0001-secure-principal-e-legacy-endpoint.md) e
[ADR-0006](adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md)
são as autoridades canônicas para os transport profiles e para o perfil de
Transport Security do Secure Principal.

```text
Transport Security != TLS Offload
```

Transport Security é uma propriedade da infraestrutura/protocolo PapinhoAccelerator, não uma capability de processamento normal. Ela protege Control Channel, Data Channels, autenticação, comandos, credenciais e payloads. Capability Negotiation não pode habilitá-la, desabilitá-la ou enfraquecer requisitos de segurança da Session.

## Regra de negociação

```text
SERVER_SUPPORTED
        ∩ SERVER_ENABLED
        ∩ USER_ALLOWED
        ∩ CLIENT_SUPPORTED
        ∩ CLIENT_PREFERENCE
        = EFFECTIVE_CONFIGURATION
```

Cada interseção é avaliada por capability. Ausência, versão incompatível ou
decisão ambígua resulta em não habilitação. O snapshot publicado é um upper
bound imutável: policy atual pode negar/reabilitar uma capability já presente,
mas não adicionar outra silenciosamente. Expansão exige nova Session ou futura
renegociação explícita, ainda sem wire definido.

`SERVER_SUPPORTED` vem dos backends presentes; `SERVER_ENABLED`, da configuração administrativa; `USER_ALLOWED`, da autorização; `CLIENT_SUPPORTED`, das habilidades do cliente; e `CLIENT_PREFERENCE`, da escolha solicitada.

## Local de execução

Quando suportado, cada capability pode selecionar independentemente:

```text
EXECUTION_LOCAL
EXECUTION_ACCELERATOR
```

Por exemplo, imagem, vídeo e `TLS_OFFLOAD` podem ter sua execução escolhida independentemente enquanto layout permanece local. O servidor pode negar execução remota por política, quota, carga ou ausência de backend. O cliente pode então degradar graciosamente ou usar fallback local, se suportado; nunca deve alterar silenciosamente uma propriedade de segurança.

Uma configuração válida é:

```text
PapinhoAccelerator Transport Security = REQUIRED
TLS_OFFLOAD capability                = DISABLED
```

Desligar `TLS_OFFLOAD` significa apenas não delegar TLS de conexões externas. Isso não torna o canal PapinhoAccelerator inseguro.

Local de execução é diferente de implementação: `VIDEO_DECODE` remoto pode usar CPU, GPU, biblioteca, decoder físico, FPGA ou ASIC sem mudar a identidade conceitual da capability. Também é diferente de network egress: computar remotamente não concede uso do IP do servidor.

## Dimensões ortogonais de uma solicitação

Uma futura solicitação ou configuração pode combinar quatro dimensões
conceitualmente independentes:

| Dimensão | Pergunta respondida |
|---|---|
| Capability | O QUE deve ser feito |
| Execution Location | ONDE o processamento ocorre |
| Network Origin / Egress Authority | QUEM abre e possui a conexão externa, quando houver |
| Output Profile | EM QUE representação ou formato o resultado deve ser entregue |

```text
Capability
        !=
Execution Location
        !=
Network Egress Authority
        !=
Output Profile
```

`Network Origin` só é relevante quando o fluxo possui conexão externa.
`Output Profile` pode não se aplicar a determinada capability. Execution
Location permanece independente do backend concreto que executa o trabalho.
Essas dimensões são um modelo conceitual: não definem campos, enums, IDs ou
encoding do Wire Protocol.

Exemplos conceituais, sem declarar suporte:

```text
Capability:      VIDEO_TRANSCODE
Execution:       ACCELERATOR
Network Origin:  CLIENT
Output Profile:  WIN95_BASE

Capability:      IMAGE_DECODE
Execution:       ACCELERATOR
Network Origin:  CLIENT
Output Profile:  ORIGINAL / compatible result
```

As quatro dimensões não substituem negociação, policy ou autorização. O
resultado continua sujeito à interseção do ADR-0005:

```text
SERVER_SUPPORTED
        ∩ SERVER_ENABLED
        ∩ USER_ALLOWED
        ∩ CLIENT_SUPPORTED
        ∩ CLIENT_PREFERENCE
        = EFFECTIVE_CONFIGURATION
```

## Política e quotas

Autorização deve ser granular (por exemplo, permitir `IMAGE_DECODE` e negar network egress). Futuras políticas podem limitar Sessions, CPU, RAM, banda, streams simultâneos, jobs e parâmetros por capability. Reserva e consumo deverão ser contabilizados e liberados no cleanup; ultrapassar limites deve falhar de modo controlado.
