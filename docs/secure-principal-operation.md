# Secure Principal listener — operação de laboratório

Este documento descreve a fonte de configuração **lab-only** atualmente
implementada no `papacc_server.exe` Win32. A arquitetura normativa permanece
nos ADRs 0001, 0006, 0009 e 0010. Este procedimento não implementa pairing,
enrollment, armazenamento persistente de credenciais ou uma fonte de
configuração de produção.

## Perfil efetivo

O perfil é escolhido explicitamente por execução:

```text
--listener-profile legacy-endpoint
--listener-profile secure-principal --security-configuration-ref <reference>
```

Não há detecção por porta, certificado ou ClientHello e não há fallback entre
os perfis. `secure-principal` exige TLS 1.3, mTLS, ALPN `papacc/1`, credencial
de servidor, CA de clientes e enrollment explícito por fingerprint SHA-256.
Somente depois dessas gates uma Connection PACC pode publicar CONTROL ou DATA.

CONTROL e todas as conexões DATA usam o mesmo listener e a mesma porta. Cada
operação usa uma conexão DATA nova e um ticket one-time; CONTROL pode permanecer
aberto entre operações.

## Gerar a PKI descartável

Execute em uma máquina de desenvolvimento com OpenSSL disponível. O diretório
de saída deve ficar fora do checkout:

```powershell
$lab = 'C:\papacc-lab\secure-principal'
powershell -NoProfile -ExecutionPolicy Bypass -File `
  .\tools\new-secure-principal-lab-pki.ps1 `
  -OutputDirectory $lab `
  -ServerDnsName papacc-test-server
```

O script cria RSA-2048/SHA-256, uma CA de laboratório, certificado de servidor
com SAN DNS e EKU `serverAuth`, e certificado de Browser com EKU `clientAuth`.
Ele imprime os valores que devem alimentar os ambientes do servidor e do
cliente. Não reutilize esse material em produção.

## Fonte de configuração do servidor

A referência da CLI é resolvida pela fonte de configuração de laboratório
proprietária da aplicação. O valor de `PAPACC_SECURITY_CONFIGURATION_REF` deve
ser idêntico ao argumento da CLI. Os demais valores são caminhos para arquivos
Base64 que contêm DER ou PKCS#8 DER sem senha:

```powershell
$env:PAPACC_SECURITY_CONFIGURATION_REF = 'nt4-lab-secure'
$env:PAPACC_SERVER_CERTIFICATE_DER_B64 = 'C:\papacc-lab\secure-principal\accelerator\server.der.b64'
$env:PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64 = 'C:\papacc-lab\secure-principal\accelerator\server.pk8.b64'
$env:PAPACC_CLIENT_CA_DER_B64 = 'C:\papacc-lab\secure-principal\ca\ca.der.b64'
$env:PAPACC_ALLOWED_CLIENT_CERT_SHA256 = '<sha256 impresso pelo script>'
```

O fingerprint é uma chave de lookup da credencial, não o Principal. A fonte
lab mapeia somente o certificado exatamente matriculado para um Principal
opaco e concede as ações usadas pelo runtime atual. Certificado válido emitido
pela mesma CA, mas não matriculado, falha fechado.

Inicialização por todas as interfaces:

```powershell
.\build\ninja\papacc_server.exe `
  --port 4433 `
  --all-interfaces `
  --listener-profile secure-principal `
  --security-configuration-ref nt4-lab-secure `
  --allow-network-egress `
  --egress-policy-ref nt4-lab-controlled `
  --egress-profile-ref default-route `
  --outbound-tls-profile-ref system-tls13 `
  --log-level info
```

Sem `--allow-network-egress`, egress permanece negado. A flag liga apenas a
avaliação da policy e do Egress Profile server-owned; não significa allow-all.
O listener Secure Principal continua operacional para CONTROL/DATA mesmo com
egress negado.

O source Win32 de laboratório exige que a referência de policy coincida com
`PAPACC_EGRESS_POLICY_REF` e que
`PAPACC_EGRESS_PROFILE_REF=default-route`. Cada regra exata usa o mesmo índice:

```text
PAPACC_EGRESS_RULE_0_HOST=papacc-lab-http.test
PAPACC_EGRESS_RULE_0_IPV4=127.0.0.1
PAPACC_EGRESS_RULE_0_PORT=8080
```

`HOST` é opcional para uma regra de literal IP; `IPV4` e `PORT` são
obrigatórios. A source aceita no máximo 16 entradas, rejeita duplicatas e não
implementa wildcard, subnet, range ou fallback. O resolver usa exclusivamente
os endpoints do snapshot publicado; não executa uma segunda resolução DNS.

O profile outbound `system-tls13` usa o trust store do sistema. Para target de
laboratório assinado por CA privada, configure uma referência distinta tanto em
`--outbound-tls-profile-ref` quanto em `PAPACC_OUTBOUND_TLS_PROFILE_REF` e aponte
`PAPACC_OUTBOUND_TLS_CA_DER_B64` para a CA DER em base64. Isso configura somente
trust de sites externos; a CA de clientes inbound permanece independente.

Para bind específico, copie sem alteração o `Persistent ID` decimal exibido
por `--list-interfaces` para `--interface-id`. O parser preserva todos os 64
bits e não interpreta Runtime ID como Persistent ID.

## Perfil Legacy explícito

O comportamento existente permanece disponível e plaintext:

```powershell
.\build\ninja\papacc_server.exe `
  --port 4433 `
  --all-interfaces `
  --listener-profile legacy-endpoint `
  --log-level info
```

Legacy Endpoint não aceita `--security-configuration-ref`, não possui
identidade criptográfica e não recebe segurança retroativa de TLS Offload.

## Material do Browser / NT4

Somente estes artefatos gerados pertencem ao pacote manual do Browser:

```text
browser/client.der.b64
browser/client.pk8.b64
ca/ca.der.b64
```

Além deles, copie o executável PapinhoBrowser e as DLLs do target PST exigidas
pelo pacote do Browser. A configuração factual do Browser deve usar o mecanismo
que ele já implementa; este repositório não define nem altera essa interface.

Nunca copie ao NT4:

```text
ca/ca.key.pem
accelerator/server.key.pem
accelerator/server.pk8
accelerator/server.pk8.b64
```

O script não grava material no repositório. O operador é responsável por ACLs,
retenção e descarte do diretório de laboratório. O logger não registra chaves,
certificados DER, fingerprints completos ou payload arbitrário.

## Rede e firewall

Permita:

```text
NT4/Browser -> Accelerator:<CONTROL port>  (CONTROL e DATA, mesmo listener)
Accelerator -> destinos autorizados       (somente quando egress está habilitado)
```

Não existe porta DATA separada nem range dinâmico inbound. DNS, endpoint
normalizado, anti-SSRF e policy de destinos continuam sendo aplicados no
Accelerator.

## Limites atuais

Para aceitação manual contra o servidor real sem a Secure Browser fixture, use
os [standalone controlled lab egress targets](standalone-lab-egress-targets.md).
Eles são sites HTTP/HTTPS simulados em loopback, com portas selecionadas pelo
operador; não são servidor PACC nem participam do trecho Browser → Accelerator.

- esta fonte é intencionalmente transitória e lab-only;
- não há pairing, emissão, renewal, revocation, recovery ou GUI;
- existe um único fingerprint matriculado por execução;
- não há persistência/precedência entre fontes;
- nenhuma private key é aceita pela CLI;
- `Transport Security != TLS_OFFLOAD` continua válido.
