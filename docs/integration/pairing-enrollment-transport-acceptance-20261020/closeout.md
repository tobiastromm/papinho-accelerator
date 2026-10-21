# Aceitação normativa de pareamento e transporte de enrollment

P0–P9 e T0–T7 Revision 2 registrados ACCEPTED no documento normativo designado após revisão conjunta e aprovação oficial de escrita. Incorporação byte-exata pelos intervalos de source-intervals.json e anexos congelados. Propostas originais e ADRs accepted anteriores preservados; arquitetura do listener separado L0–L5 continua vigente.

Precedência explícita fecha o transporte antes aberto em P7, distingue claim inicial de consumo posterior e limita o novo perfil a uma CONFIRM, mais restritiva que o teto de cinco de P4. ALPN papacc-enroll/1 e PAE1/v1.0 são contratos exclusivos do ENROLLMENT. Operacional continua TLS 1.3/mTLS/papacc/1. OPEN/CLOSED, aprovação, CSR, issuer e grants permanecem separados; chave automática exclusivamente no cliente.

Nenhuma implementação, build, teste, rede, CA, chave, certificado, assinatura, publicação CURRENT ou admissão foi executada. ED11 não tem reserva nova: Windows passa apenas a soma conhecida, com censo transitivo ainda pendente; Legacy excede no cenário adicional e mantém DENY. Provider/kernel UNKNOWN, total PARTIAL. Nenhum target qualificado por documento. Extensão versionada do modelo/codec, capacidade/runtime e fontes factuais continuam gates de implementação.

HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a, WIP preservado. Próxima etapa depende de autorização específica de implementação e patches exatos, sem ativação automática.

```ini
CURRENT_TASK_COMPLETED=YES
PAIRING_CODE_CONTRACT=ACCEPTED
INITIAL_TRANSPORT_CONTRACT=ACCEPTED
ENROLLMENT_LISTENER_ARCHITECTURE=ACCEPTED
ALPN=papacc-enroll/1
OPERATIONAL_MTLS=PRESERVED
IMPLEMENTATION_AUTHORIZED=NO
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_REVIEWED_NORMATIVE_SCOPE
NEXT=AUTOMATIC_ENROLLMENT_LISTENER_AND_PAIRING_IMPLEMENTATION
```
