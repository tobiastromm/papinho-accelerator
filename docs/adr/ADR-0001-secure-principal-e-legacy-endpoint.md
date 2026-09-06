---
adr: ADR-0001
title: Secure Principal e Legacy Endpoint
status: accepted
decision-date: 2026-08-30
last-revised: 2026-09-06
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0001 — Secure Principal e Legacy Endpoint

## Contexto

O PapinhoAccelerator precisa manter TLS 1.3 mTLS como perfil normal de
segurança e, ao mesmo tempo, preservar a possibilidade futura de atender
plataformas extremamente antigas incapazes de executar TLS moderno. Essa
compatibilidade não pode transformar falha de segurança em downgrade, atribuir
identidade criptográfica a dados de rede ou misturar plaintext com a futura
abstração segura.

A decisão surgiu no fechamento da Phase 3.A2B-R3 e foi originalmente registrada
em `docs/phase3-transport-profiles.md`. Este ADR a formaliza posteriormente sem
reescrever a cronologia daquele documento.

## Forças da decisão

- segurança e resistência a downgrade;
- compatibilidade explícita com endpoints muito antigos;
- identidade e autorização semanticamente corretas;
- isolamento operacional e auditabilidade;
- separação entre PACC, transport e segurança;
- futura integração por abstração criptográfica madura;
- distinção entre Transport Security e `TLS_OFFLOAD`.

## Alternativas consideradas

### Alternativa A — Somente perfil TLS

**Vantagens**

- superfície conceitual menor;
- todo endpoint possui proteção criptográfica forte.

**Desvantagens / trade-offs**

- exclui plataformas que não conseguem executar TLS moderno mesmo em redes
  administrativamente isoladas.

### Alternativa B — Auto-detecção ou fallback TLS/plaintext

**Vantagens**

- aparente simplicidade operacional em um único endpoint.

**Desvantagens / trade-offs**

- permite ambiguidade e downgrade acidental;
- uma falha de TLS pode ser confundida com solicitação de plaintext;
- dificulta isolamento, logging e auditoria.

### Alternativa C — Perfis explícitos Secure Principal e Legacy Endpoint

**Vantagens**

- mantém o caminho seguro fail-closed;
- torna o risco do modo legado explícito;
- permite isolamento e política próprios;
- preserva o mesmo protocolo PACC.

**Desvantagens / trade-offs**

- exige configuração explícita e futura disciplina operacional;
- o perfil legado não oferece confidencialidade nem identidade criptográfica
  no hop cliente–Accelerator.

## Decisão

O PACC permanece um único protocolo. Seu transporte poderá futuramente usar
dois perfis explicitamente selecionados por listener/configuração.

### Secure Principal

```text
TCP
  -> TLS 1.3 mTLS
  -> authenticated cryptographic Principal
  -> PACC
  -> CONTROL / DATA
```

`SECURE PRINCIPAL` é a direção normal e segura de produção. TLS 1.3 e mTLS são
obrigatórios. A identidade criptograficamente autenticada resolve um Principal
interno estável, conceitualmente separado dos bytes de certificado e chave.
Autenticação e autorização são decisões distintas. Falha de TLS, identidade ou
integridade é fatal e nunca causa fallback automático para plaintext.

### Legacy Endpoint

```text
TCP
  -> PACC
  -> CONTROL / DATA
```

`LEGACY ENDPOINT` é um perfil futuro e explícito de compatibilidade. Ele não
possui Transport Security no hop cliente–Accelerator e não representa strong
cryptographic identity. Deve ser explicitamente habilitado, permanecer
desabilitado por padrão, apresentar warning visível e poderá receber política
de autorização própria ou mais restrita.

Endereço IP, endpoint TCP, password transmitida em plaintext e ticket DATA não
podem ser representados como Secure Principal ou autenticação criptográfica
forte.

### Seleção e downgrade

```text
NO AUTOMATIC DOWNGRADE:
SECURE PRINCIPAL -> LEGACY ENDPOINT
```

A seleção do transport profile ocorre por listener/configuração. Ela nunca
ocorre por falha de TLS, payload sniffing ou tentativa automática de downgrade.

### Fronteiras

```text
Transport Security != TLS_OFFLOAD
```

Transport Security protege o hop cliente–Accelerator. `TLS_OFFLOAD` é uma
capability futura para TLS entre o cliente e serviços externos e não torna
seguro um hop Legacy Endpoint plaintext.

Somente Secure Principal atravessa o futuro `PapinhoSecureTransport`. Legacy
Endpoint permanece diretamente sobre Transport/PACC e não pode ser apresentado
como backend "no crypto" do `PapinhoSecureTransport`.

### Recomendação arquitetural

A topologia recomendada usa listeners separados:

```text
Secure Listener -> TLS 1.3 mTLS obrigatório
Legacy Listener -> plaintext explicitamente configurado
```

Quando possível, o listener legado deve usar porta, interface, rede ou VLAN
dedicada. Isto é recomendação arquitetural; configuração, política e listeners
concretos ainda são trabalho futuro, não comportamento implementado.

## Justificativa

Perfis explícitos preservam compatibilidade sem enfraquecer o caminho seguro.
A seleção fora do handshake impede que erros, tráfego malformado ou atacantes
induzam downgrade. Separar Principal criptográfico de informações de rede evita
atribuir confiança inexistente. Manter plaintext fora do
`PapinhoSecureTransport` preserva a semântica da abstração segura.

## Consequências

### Positivas

- TLS falha fechado;
- o risco legado permanece explícito;
- PACC não precisa de versão segura e insegura;
- isolamento e auditoria podem ser aplicados por listener;
- identidade criptográfica não é confundida com endereço ou ticket;
- a futura abstração segura não precisa de backend plaintext fictício.

### Negativas / trade-offs

- Legacy Endpoint transmite PACC sem confidencialidade ou integridade
  criptográfica no primeiro hop;
- operação legada exigirá warnings e configuração cuidadosa;
- poderão existir políticas e superfícies operacionais adicionais.

### Neutras ou operacionais

- nenhum listener, perfil ou configuração é implementado por este ADR;
- nenhuma mensagem ou byte PACC é alterado;
- `PapinhoSecureTransport` continua trabalho futuro separado.

## Regras derivadas

1. Secure Principal exige TLS 1.3 mTLS.
2. Falha de segurança no perfil Secure Principal é fatal e fail-closed.
3. Não existe downgrade automático de Secure Principal para Legacy Endpoint.
4. Legacy Endpoint é desabilitado por padrão e exige habilitação explícita.
5. Legacy Endpoint não pode alegar strong cryptographic identity.
6. IP, endpoint TCP, password plaintext e ticket DATA não identificam Secure
   Principal.
7. A seleção do perfil ocorre por listener/configuração, nunca por sniffing ou
   falha TLS.
8. Transport Security permanece distinto de `TLS_OFFLOAD`.
9. Somente Secure Principal atravessa `PapinhoSecureTransport`.
10. Legacy Endpoint não é backend "no crypto" de `PapinhoSecureTransport`.
11. Autenticação e autorização permanecem decisões distintas.
12. Listeners separados são recomendados, mas ainda não estão implementados ou
    concretamente especificados.

## Impacto

### Código

Nenhuma alteração funcional nesta decisão documental. Integração futura deverá
preservar as regras acima.

### Build e toolchains

Nenhum impacto atual.

### Documentação

Este ADR passa a ser o registro canônico. O documento original da Phase 3 é
preservado como fonte histórica e explicativa.

### Compatibilidade

O perfil legado preserva uma possibilidade futura sem declarar suporte a uma
plataforma específica nem tornar plaintext seguro.

### Testes

Testes futuros devem provar seleção explícita, defaults seguros, ausência de
downgrade e isolamento dos perfis quando estes forem implementados.

## Verificação de conformidade

- não existe caminho automático de TLS falho para plaintext;
- Legacy Endpoint não é habilitado por default;
- nenhuma informação de rede é tratada como Principal criptográfico;
- `TLS_OFFLOAD` não controla Transport Security;
- nenhum backend plaintext existe dentro de `PapinhoSecureTransport`;
- documentação distingue norma vigente, recomendação e trabalho futuro;
- código atual continua sem alegar que os perfis já foram implementados.

## Relações

### ADRs relacionadas

- `PapinhoEngineering/ADR-0001` — adoção e governança de ADRs.
- `PapinhoEngineering/ADR-0004` — modelo compartilhado de configuração.
- `PapinhoEngineering/ADR-0006` — carregamento de contexto e governança de agentes.

### Capability Documents relacionadas

Nenhuma criada especificamente para transport profiles nesta fase.

### Documentos relacionados

- `docs/phase3-transport-profiles.md` — registro histórico/original da decisão.
- `docs/phase3-transport-security-profile.md` — perfil TLS 1.3 mTLS.
- `docs/phase3-nss-mtls-nt4-proof.md` — prova do backend legado.

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização como primeiro ADR local da decisão explicitamente aceita sobre Secure Principal, Legacy Endpoint e ausência de downgrade automático. |

