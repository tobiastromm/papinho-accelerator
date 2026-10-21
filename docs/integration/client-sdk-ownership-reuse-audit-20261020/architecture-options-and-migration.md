# Alternativas e migração PROPOSED

| Critério | A: ampliar PapinhoPACC | B: biblioteca complementar | C: SDK composto |
|---|---|---|---|
| Reúso | mistura wire e provisão | domínio comum isolado | PACC + provisão + adapters opcionais |
| Norma | conflita protocol-only ADR0025 sem revisão aceita | preserva ADR0025/PE0011 | preserva ambos, sem mega-library |
| API/ABI | risco de dependências no público atual | API própria sem mudar wire | versões/manifest de compatibilidade por módulo |
| Portabilidade | crypto indevido para consumidores wire-only | core C89 bounded | subconjuntos por target/provider |
| NT4/VC6 | PACC atual não qualifica nova provisão | aproveitar adapter legado existente | source builds;sem ABI moderna imposta |
| Windows moderno | monolito mais simples mas acoplado | EVP/CNG somente onde apropriado | separar emissão admin de validação pública |
| box/ISA | dependências novas involuntárias | UNSUPPORTED se provider faltar | matriz factual por módulo, sem hardware agora |
| Tamanho/ED11 | crescimento global do link | crypto opcional | contar composição real, sem margem presumida |
| Manutenção/testes | ownership ambíguo | core e KAT/gramática isolados | CI/metadata extras, testes cruzados |
| Licença | gap atual se espalha | termos próprios + notices | obrigações explícitas por artefato |
| Migração/risco | alto; muda contrato consolidado | médio; wrappers bounded | médio;packaging extra, integração incremental |

Recomendação: C como composição e B como módulo de domínio. A não selecionada sob ADR0025 vigente. Não antecipar reconnect/pool/discovery/GUI/bindings Phase12. Inicialmente fonte canônica Accelerator; repositório novo só por decisão posterior. PST mantém TLS/certificates/ALPN/mTLS; .papacc/policy não pertencem automaticamente ao engine TLS.

1. Aceitar ownership delimitado, coordenar adoção Browser/family. Aprovação documental desta auditoria não aceita ADR candidate.
2. Designar módulo/API C89 de dados/resultados, bounds/versão/licença/manifest e adapters. Sem HWND/HKEY/root/provider no core. API de evidências exige revisão separada; booleano/string/digest não criam autoridade.
3. Extrair M2/M3 e normalização/suite sem alterar bytes. Usar parser canônico e port VC6 existentes, preservar preimagens/vetores. Resolver tipos stdint/VC6 num contrato único, sem fork. Wrappers temporários com owner e gate de remoção; não duplicação permanente.
4. Adaptar crypto: preservar BearSSL/NSS-derived/EVP e licenças; unificar quarentena/aliases/Stop sem primitiva nova. Isolar material validation de store Win32. PAIR_ONLY não é chain/status/trust.
5. Integrar Browser pinado no mesmo UI/store/gate/ConfigGeneration/CredentialGeneration/ActiveConfiguration/leases, preservando simple/advanced. Accelerator usa core sem expor root ao cliente. Remover cópia local somente após equivalência e patch oficial.
6. Testes cruzados de bytes/corpus/KAT/RFC8439/RSA/alias/Stop/retenção/UNKNOWN; M1 zero KDF sem trust; regressões UI negativa/advanced/storage/CAS e consumidor wire-only. Testes dependentes de fixtures negadas exigem resolução oficial prévia; não gerar por outro mecanismo.
7. Source release somente após API/licença/NOTICE/pin/hash/compatibilidade/consumidores/ED11. NT4 real não decorre de VC6 host moderno; box/ISA não qualificados sem provas. Nenhuma publicação nesta tarefa.

Etapas1-6 podem integrar Phase6 sem ampliar escopo funcional, mas cada mudança depende nova autorização e aprovação exata. Não foram iniciadas por esta auditoria. Gates de saída: owner definido, API/licença fechadas, equivalência parser, backend/lifetime aprovados, consumers integrados, regressões passantes e distribuição aprovada. Nenhuma etapa abre admissão, install, assinatura ou CURRENT automaticamente. WIP/evidências/testes existentes preservados.
