# Modelo de mídia

“Aceleração de vídeo” não é uma operação única. A negociação deve distinguir os três modelos abaixo e seus custos de rede/cliente.

## Streaming Assist

O Accelerator pode obter, bufferizar, demultiplexar, remultiplexar ou manipular um stream mantendo o codec original. Isso pode reduzir trabalho de transporte, mas não garante remover do cliente o custo de decode. Origem da obtenção depende separadamente da política de network egress.

## Transcoding

O Accelerator converte um formato de entrada para outro adequado ao cliente:

```text
AV1 / VP9 / H.264 -> Accelerator -> formato adequado ao cliente
```

O cliente poderá solicitar um Target Profile, como `WIN311_BASE`, `WIN95_BASE`, `WIN98_BASE`, `NT4_BASE` ou `CUSTOM`. Perfis são contratos futuros de restrições, não listas de codecs nesta baseline; nenhum codec definitivo é associado a eles agora.

## Remote Decode / Remote Frames

O Accelerator decodifica por completo e transmite frames, framebuffer ou pixel data prontos ou quase prontos para apresentação:

```text
stream moderno -> Accelerator -> frames/pixels -> cliente
```

Negociação futura deverá cobrir pixel format, resolução, frame rate, dirty rectangles, áudio, sincronização A/V, buffering e backpressure. Também deverá estabelecer ordering, timestamps, perda/recuperação, limites e cancelamento. Frames remotos podem consumir muita banda e memória; quotas e flow control são obrigatórios antes de implementação.

## Áudio

Decode, streaming e transcoding de áudio são capabilities separadas. Em mídia audiovisual, o contrato deverá coordenar relógios, latência e sincronização sem presumir que áudio e vídeo usem o mesmo backend ou canal.

## Aplicações web modernas

Transcoding isolado não torna necessariamente uma página moderna utilizável. Players podem depender de JavaScript, manifests, APIs, DOM dinâmico e streaming adaptativo. Futuras soluções poderão negociar HTML parsing, CSS/layout, JavaScript remoto (ainda não listado como ID congelado), display commands, framebuffer ou uma representação intermediária. Esses recursos têm superfícies de segurança e lifecycle próprios e não são implementados nem especificados em detalhe aqui.
