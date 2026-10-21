---
decision-id: MODE_C_ADMINISTRATIVE_SITE_TLS_PROFILE_AUTHORITY
status: accepted
decision-date: 2026-10-18
revision: 1
scope-level: project
scope-target: PapinhoAccelerator canonical Mode C
decision-makers: [Tobias Tromm]
supersedes: []
---
# Autoridade administrativa de perfis Site TLS Mode C

Autoridade explícita: PACC-PHASE6-MODE-C-SITE-TLS-PROFILE-AUTHORITY-1. A configuração administrativa governada do Accelerator é a única fonte de aprovação dos perfis Site TLS Mode C. Registrar esta decisão antecede implementação dependente. Não altera ADRs accepted anteriores nem PACC wire.

## Decisão e conformidade
C2 revision1.0 §7 exige tls_profile_ref opaco aprovado pelo servidor. ADR-0048 permite entrada programática no Configuration Model e extensão privada versionada na mesma transação. ADR-0044 exige commit factual antes de CURRENT/epoch. ADR-0022/0027 e PapinhoEngineering/ADR-0009 separam trust Site, canal mTLS e Network Egress. Não foi encontrada contradição; o escopo Mode B daqueles documentos não é promovido automaticamente para Mode C.

O catálogo tem identidade estável, versão/revisão, identidade de perfil, tls_profile_ref, origem administrativa, trust, hostname/SNI, versões TLS, ALPN, revogação/expiração, limites e integridade. O Browser solicita referência; não aprova trust. Referência0 somente resolve um default explicitamente cadastrado, nunca um default implícito.

A mesma transação PREPARE → VALIDATE → COMMIT → POLICY_CURRENT → EPOCH vincula o catálogo. Ausência, UNKNOWN, referência desconhecida, revisão/epoch divergentes, policy não CURRENT, identidade não autorizada, revogação ou expiração negam uso. Reload não migra operações nem promove referências antigas automaticamente.

O catálogo não cria grants. Operação exige Principal factual, grants, snapshot e policy contextual, limites e lifecycle vigentes. Nenhuma autorização decorre de certificado mTLS, Network Egress ou perfil existente isoladamente.

## Segurança e lifecycle
Certificado/chain/nome obrigatórios; SNI é o hostname normalizado da operação. TLS1.3 somente no perfil inicial, sem downgrade, fallback, client certificate Site, resumption ou early data. ALPN limitado e filtrado. Trust é explicitamente administrativo; nenhuma CA de mTLS é reutilizada implicitamente e nenhum trust vem do Browser. Recursos PST preparados são application-owned e preservados até quiescência ou retidos sob Model E; não introduzir cleanup alternativo.

## Implementação privada e limites
Capacidade inicial pode ser menor que capacidades futuras; limites e representação privada são escolhas documentadas/testadas, não limites novos de wire. A primeira implementação pode suportar apenas SYSTEM trust; CUSTOM não implementado deve ser rejeitado, nunca convertido em SYSTEM. Configuração vazia é ausência, não aprovação. Formatos históricos permanecem estritos, sem migração ou reinterpretação automática. Integridade deve cobrir o conteúdo do catálogo na persistência da configuração, não apenas um apontador externo mutável.

Reservar catálogo, cópias, snapshots, revisão e retenção sob ED11 managed2097152 inalterado. Provider/kernel UNKNOWN. Sem autorização para rede, admissão, produção, outro listener/root/owner ou alterações Browser/PST/API pública/wire/pin0.6.4.

## Gates
Builds Debug/Release e regressões offline autorizados. Testes de representação/fixture não são prova de Principal autenticado, TLS, Session ou capacidade operacional. CLIENT_ADMISSION=DISABLED, SITE_TLS_ADMISSION=DENY_UNTIL_APPROVED, ED11_ADMISSION=DENY. PST0.6.5 permanece candidato até o fechamento integral da Phase6.

## Relações
C2 accepted §7/§11; ADR-0012/0022/0027/0028; ADR-0044/0048 históricos e decisão accepted de consolidação canônica; Model E; ED08/ED11. Evidência e schema privado serão mantidos na documentação viva e no closeout, sem alterar contratos congelados.
