---
adr: ADR-0025
title: Contrato público portátil PapinhoPACC e implementação autoritativa compartilhada
status: accepted
decision-date: 2026-09-20
last-revised: 2026-09-20
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0025 — Contrato público portátil PapinhoPACC

## Contexto

O servidor, o Reference Client e o futuro PapinhoBrowser precisam falar o
mesmo protocolo PACC. Copiar codecs para consumidores criaria implementações
divergentes e tornaria correções de validação inconsistentes.

## Decisão

`PapinhoPACC` é o nome canônico do componente que publica o contrato portátil
do protocolo PACC. Existe uma implementação autoritativa de framing, tipos de
mensagem, codecs e validação wire, compartilhada por servidor, Reference
Client e consumidores externos. O target lógico preferido é `papacc_protocol`.

O componente é protocol-only: não depende de sockets, Win32, PST, transporte,
runtime do servidor, autorização, policy, Session/Job/Stream managers ou
ownership interno. Os transports `SECURE_PRINCIPAL` e `LEGACY_ENDPOINT` usam
o mesmo wire; esta decisão não concede egress ao Legacy Endpoint nem cria
fallback entre perfis.

A versão da API C é distinta da versão do wire. Evolução da API não altera
automaticamente o wire. A versão wire existente permanece representada pelo
Envelope 1.0 e Control 1.0; o contrato externo identifica separadamente a
geração de mensagens `0x0001`–`0x000E`. Esta ADR não adiciona bytes ou
negociação de versão ao wire. Layout, ordem, largura, endianness, limites,
validação e semântica desses IDs permanecem inalterados.

Consumidores externos fixam uma versão factual do pacote. O pacote canônico
v1 contém headers públicos, a mesma implementação fonte portátil e metadata
de versões. Bibliotecas pré-compiladas podem ser específicas de target, mas
não substituem essa distribuição em fonte entre toolchains. A boundary
portátil deve ser C89/VC6-friendly onde aplicável, bounded, sem heap
obrigatório, com buffers/capacidades explícitos e sem tipos nativos ou de
provider. Internals de runtime nunca compõem a API pública do protocolo.

PapinhoPACC não é o SDK high-level previsto para Phase 12. Não implementa
reconnect, pool, discovery, GUI, bindings ou política de fallback.

## Consequências

- Correções de codec são compartilhadas por todos os consumidores.
- O build e o pacote precisam provar independência do runtime e de providers.
- Mudança de API pode requerer nova versão do pacote sem mudar o wire.
- A integração Browser pode consumir fonte portátil sem presumir ABI de `.lib`
  gerada por outro toolchain.

## Relações

Complementa ADR-0002 (Core portátil), ADR-0016 (negociação) e ADR-0017
(Network Offload). As especificações de framing, Control e DATA continuam
autoridade para os bytes do wire.
