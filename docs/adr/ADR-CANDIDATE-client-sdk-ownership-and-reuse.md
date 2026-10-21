---
status: proposed
revision: 1
date: 2026-10-20
scope-level: project
scope-target: PapinhoAccelerator shared client component boundary
supersedes: []
review-required: owner-and-official-executor
---
# ADR candidate — SDK cliente e provisão compartilhada

## Contexto

Proposta de PACC-PHASE6-CLIENT-SDK-OWNERSHIP-AND-REUSE-AUDIT-1. Não ACCEPTED e sem autorização funcional. Especializa lacuna local de ownership;adoção Browser/family exige seu processo próprio. Não modifica ADRs de outros projetos ou regras superiores. Multi-consumer já existe em BrowserADR0007,shared-owner-first em PEADR0011 e PACC protocol-only em AcceleratorADR0025.

## Proposta S0-S8

S0. Browser é primeiro/importante consumidor, não exclusivo. Capacidade comum não depende de HWND,BrowserProfile,abas,navegação,Registry ou estruturas privadas da root. Nenhum consumidor futuro é declarado suportado.

S1. PapinhoPACC mantém única implementação wire/framing/codecs/validação,protocol-only. API e pacote evoluem independentemente de wire. Não adicionar bytes/messages/ALPN ao wire para acomodar API local. Fixes upstream,snapshot pinado imutável.

S2. SDK composto com módulo complementar de provisão separado de papacc_protocol,fonte canônica inicialmente Accelerator. Extrair M2/M3/TLV/normalização/suite/pipeline comum comprovados sem reescrita semântica,após API/lifetime/target review. Nome público,pastas,númeroAPI/release/repositório não congelados. Sem plugin platform,trust service ou SDK high-level Phase12 por inferência.

S3. Core comum valida dados/evidências;estados DATA,crypto,confiança,autorização e instalação distintos. Não fabrica CURRENT/Principal/grants/trust por booleano,string ou digest. APIs exatas de autoridade/evidência exigem revisão própria. M1 STRUCTURE -> SIGNATURE -> TRUST -> KDF -> AEAD -> CONTENT -> RELEASE e recheck final intactos;quarentena exclusiva até conclusão integral.

S4. Backends opcionais e qualificados por target,reusando BearSSL/NSS-derived/EVP/PST com pins,licenças e testes. Não escrever primitiva ou emular capability profunda. PST mantém TLS/certificates/ALPN/mTLS;issuer/CNG privado não integra corecliente. C89/VC6 build não qualifica NT4,Windows/box/ISA por inferência.

S5. Consumidor mantém confirmação administrativa independente e autorização local de instalação,UI/Profile/storage/gate/CredentialGeneration/ActiveConfiguration/leases concretos. Accelerator mantém policy/delegação/issuer/serial/revogação/CURRENT/R/A/P/I/CAS/root/worker/emissão. Root não concede confiança local. Browser não conhece codec interno root. ProvenanceBrowser não vira formato universal obrigatório.

S6. Migração preserva owners factuais,Stop não bloqueante,ModelE/ED08/retenção/exactly-once/créditosED11. Não mover buffers para escapar de censo,não apagar segredo borrowed/retido. Simple/advanced mesmo pipeline;automático chave exclusivamente cliente;manual só exceção accepted. Sem segunda root/CA/truststore/registry/journal/autoridade ou fallback permissivo.

S7. Distribuição fonte versionada com manifest/hash/API/wire/target/backend/licença/NOTICE separados. Sem ABIbinária entre toolchains presumida,fetch no build ou dependência de checkout Browser. Wrappers temporários precisam owner/gateequivalência e remoção antes release conjunta. Não manter parsers/M1 paralelos permanentemente sem exceção normativa explícita delimitada;esta proposta não cria exceção.

S8. Owner aceita e registro oficial precede mudança dependente. Cada patch funcional exige aprovação própria. Fixture anteriormente negada continua bloqueada até revisão oficial própria;auditoria não é retry. Sem aceitar novos formatos/crypto/quotas/autoridade/release/gates por referência. Orçamento Windows Accelerator não aumenta limite Browser/outros targets;UNKNOWN mantém DENY.

## Alternativas/consequências

A ampliar PapinhoPACC conflita escopo protocol-only sem nova decisão. B componente complementar isola domínio. C composição de módulos utiliza B+PACC, recomendada. Custo:API/metadata/CI/migração. Benefício:uma gramática,correções comuns,providers preservados. Não prometer redução de memória ou segurança operacional não qualificada.

## Gate de aceitação e implementação futura

Owner designado;API boundedC89 sem tiposconsumer/root/provider;licença/proveniência resolvidas;equivalência golden/negativa e cross-provider;M1 sem KDF antestrust;fontes/lifetime factuais;simple/advanced intactos;consumer builds por target;censoED11;pin/hash atômicos. Sem exceção tácita ao ADR0025/PE0011. Conflito material exige STOP.

Relações:OEADR0001/0002/0003;PEADR0007/0009/0010/0011;AcceleratorADR0006/0009/0025/0028,M0-M8,D3,F1-F5/G0-G7,P0-P9/T0-T7;BrowserADR0007/0013/provenanceRevision2;PSTModelE. Auditoria em docs/integration/client-sdk-ownership-reuse-audit-20261020.

IMPLEMENTATION_AUTHORIZED=NO
ADR_STATUS=PROPOSED
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
