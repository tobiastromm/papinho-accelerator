# Capabilities e configuração

O [ADR-0005](adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md)
é a autoridade arquitetural canônica para a separação entre capability,
disponibilidade de backend, policy e configuração efetiva. Este documento
permanece como índice e visão geral viva.

**Estado de implementação:** Capability Negotiation e execução de capabilities
de processamento ainda não estão implementadas na baseline atual. Os nomes
abaixo são conceitos futuros e não possuem IDs numéricos congelados.

Capability Documents disponíveis:

- [Logging e diagnóstico operacional](capabilities/logging.md);
- [TLS Offload para conexões externas](capabilities/tls-offload.md);
- [Network Egress](capabilities/network-egress.md).

Capabilities são unidades independentes, extensíveis, negociáveis e versionáveis. Seus IDs futuros deverão ser estáveis; esta baseline não atribui números.

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

Cada interseção é avaliada por capability e por seus parâmetros compatíveis. Ausência, versão incompatível ou decisão ambígua resulta em não habilitação. O servidor devolve o resultado efetivo; o cliente não deve inferir concessão pelo que pediu. Mudanças durante uma Session exigirão regra/versionamento futuro.

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

## Política e quotas

Autorização deve ser granular (por exemplo, permitir `IMAGE_DECODE` e negar network egress). Futuras políticas podem limitar Sessions, CPU, RAM, banda, streams simultâneos, jobs e parâmetros por capability. Reserva e consumo deverão ser contabilizados e liberados no cleanup; ultrapassar limites deve falhar de modo controlado.
