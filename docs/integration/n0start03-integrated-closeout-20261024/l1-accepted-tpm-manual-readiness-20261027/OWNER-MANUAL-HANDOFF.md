# Prova transitória TPM — uma execução manual autorizada

A decisão operacional independente B está em [OWNER-APPROVED-NATIVE-PROOF.json](../tpm-l1-native-proof-and-parallel-closure-20261026/OWNER-APPROVED-NATIVE-PROOF.json). Pedido exato `357c33ceafb7e5a33c69c87f7a70e0db685a16547e0e7fe8741b33e0d07d77a1`. Aprovação SHA256 `d7417145a73a2b8ec03fd4b57df23ecc10b045178d02eac543557be3fa76a146`. Executável SHA256 `ffd02de63a9b8e95741707532b6e81b33307f7745741232ae91f4c35d4148334`. L1 normativa é decisão independente e não concede NV ou leitura Q4.

## Antes da única execução

Usar Windows PowerShell 5.1 x64 já elevado, no console interativo existente do operador autorizado. Não usar pipeline/redirecionamento de stdin, terminal aninhado ou outro usuário. O host exige a topologia exata de dois clientes do console (pai e prova) e processo distinto da janela; seus metadados não são associação autenticada de console.

Pins fixados: operador `S-1-5-21-785699550-1440458426-1682014700-1108`; boot RecordId237900/início2026-10-26T21:35:16.5000000Z; hashes distintos de MachineName e Computer do evento são os do pedido. Não atualizar qualquer pin para contornar DENY. Token/impersonation/PID/criação/boot e recursos são revalidados pelo executável. A ausência do recibo de uso único foi conferida apenas na preparação, não constitui reserva.

Executar exatamente uma vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\tpm-l1-native-proof-and-parallel-closure-20261026\NativeProofRelease.exe" --owner-manual-proof ffd02de63a9b8e95741707532b6e81b33307f7745741232ae91f4c35d4148334 d7417145a73a2b8ec03fd4b57df23ecc10b045178d02eac543557be3fa76a146
```

Quando solicitado, digitar exatamente:

```text
PROVE_EK_SESSION 357c33ceafb7e5a33c69c87f7a70e0db685a16547e0e7fe8741b33e0d07d77a1
```

Não há necessidade de Set-ExecutionPolicy para executar este EXE. Não executar outro coletor ou recriar desafios.

## Escopo e STOP

Uma sessão salted HMAC transitória com EK existente e correspondência pública exata, sem AK, chave persistente, autorização de hierarquia, NV ou Q4. Até12 Submit diretos, sem fallback/FlushContext por tentativa. O recibo administrativo CREATE_NEW é uso único desta prova; não é o ledger Q4 nem testemunha antirollback.

DENY em hash/operador/token/boot/host/console/identidade/atributos/limites divergentes, certificado incompatível, EK não acessível ou múltipla, erro de API/HMAC/fechamento. Preservar128 nativo/1024 administrativos; gate pré-observer124 reserva quatro referências próprias. São amostras, não picos universais nem concessão de orçamento.

Após erro ou UNKNOWN não repetir, apagar recibo ou atualizar pins. `retained=true` mantém referências originais e dependências; preservar o processo e informar a saída, sem kill/cleanup para fabricar fechamento. Perda do host permanece UNKNOWN. A ausência de uma saída final não prova sucesso nem fechamento.

## Retorno e alcance

Enviar a saída integral, inclusive JSON e mensagens de retenção. Sucesso limitado exige first=NONE, proof=true, sessão com continue-clear autenticado, contexto aberto/fechado confirmado, retained=false, limites e identidade consistentes. Isso demonstra conhecimento matemático do material privado no endpoint TBS observado; não prova localização física/pristina, revogação, geração NV ou capacidade operacional. CRL/revogação continuam UNKNOWN. Não autoriza provisionamento, Q4, Start/Stop, SCM/ACL, CURRENT, assinatura ou recuperação.
