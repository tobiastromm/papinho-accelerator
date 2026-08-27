# Portabilidade

## Estado atual

- `papacc_core`, `papacc_network`, Server Config e parser CLI são portáteis e não expõem tipos Win32.
- Win32 implementa PAL monotônica, discovery/catálogo de interfaces, WinSock, socket/bind/listen, Listener Set e o executable lifecycle.
- `papacc_server` é atualmente um composition root Win32 e só é criado pelo CMake em builds Windows.
- Não existe backend POSIX/Linux, transport local, GUI ou Compute Backend implementado.
- Identidade persistente em outras plataformas permanece um contrato opcional: futuros backends poderão fornecer token próprio ou marcar `is_valid = FALSE`; nenhuma estratégia Linux foi escolhida.

O Core e os modelos de rede são escritos em C portável e consomem contratos abstratos. Windows/Win32 é a primeira implementação disponível; não determina tipos, lifecycle ou semântica pública do produto.

## Possibilidades arquiteturais

```text
Windows / Win32
Linux / POSIX
Raspberry Pi
Embedded
Dedicated Hardware
PCI / PCIe
ISA
```

Esta lista não afirma suporte atual, compromisso de entrega nem implementação uniforme. Algumas plataformas poderão hospedar o servidor completo; outras poderão ser apenas Compute Backends; outras poderão usar transports locais. PCI/PCIe e ISA são possibilidades preservadas, sem protocolo elétrico, driver, DMA, memória compartilhada ou outro detalhe inventado nesta fase.

## Fronteiras portáveis

- APIs do Core usam tipos C de largura/semântica definida e handles opacos próprios.
- Endianness e serialização pertencem ao Wire Protocol, nunca à representação nativa de structs.
- Tamanho de ponteiro, alinhamento, packing, path, newline e encoding não podem atravessar a fronteira implicitamente.
- Tempo de protocolo deve distinguir duração monotônica de horário civil.
- Erros de plataforma são traduzidos para categorias portáveis sem perder diagnóstico local.
- Ownership, lifetime, cancelamento e cleanup são definidos no contrato, não inferidos da API do OS.

Tipos como `HWND`, `HANDLE`, `SOCKET` e `CRITICAL_SECTION` podem existir apenas dentro da implementação Windows apropriada. Equivalentes POSIX ou embarcados obedecem à mesma regra.

## Adaptações

A PAL encapsulará primitivas de plataforma; transports encapsularão canais; Compute Backends encapsularão mecanismos de execução. O build futuro deverá permitir selecionar implementações sem condicionais de plataforma espalhadas pelo Core. Capabilities serão descobertas em runtime/build e negociadas, portanto backend ausente causa redução explícita de funcionalidade, não falha arquitetural.

## Portabilidade não é equivalência

Uma plataforma limitada pode oferecer apenas um subconjunto, com quotas e desempenho diferentes. Compatibilidade significa negociar corretamente esse subconjunto, preservar segurança e produzir erros definidos. Fallback local pertence ao cliente e só é usado quando ele declara suporte; nenhuma capability remota é presumida.
