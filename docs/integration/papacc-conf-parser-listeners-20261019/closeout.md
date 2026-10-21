# Closeout — PACC-PHASE6-PAPACC-CONF-PARSER-LISTENERS-1

Concluído o slice independente autorizado de parser/AST/modelo/validação offline. A definição posterior do owner resolve o bloqueio anterior: end explícito, indentação visual. Codec continua adiado expressamente. O [closeout parcial anterior](closeout-before-owner-delimiter-resolution.md) permanece como histórico, sem representar o estado atual.

## Autoridade e operações oficiais

[Gramática focal accepted](../../adr/DECISION-papacc-conf-textual-grammar-20261019.md), SHA256 d8eb2892dc6d430417246f30ec71a4aacda7d56fedc819bfaeb185416e5dc990, registrada por operação oficial aprovada. Não alterou decisão accepted anterior. Patch funcional inicial de4arquivos e suplemento de2arquivos foram submetidos separadamente com hashes/pré-imagens e aprovados. Não houve negativa nem mecanismo alternativo. [Hunk review](source-hunk-review.md), exact-source-operation.json/diffs e exact-supplement-operation.json/diffs preservam as operações exatas. Suplemento restringe CR a CRLF, corrige coluna factual e amplia negativos; não amplia autoridade.

## Implementação e limites

Biblioteca portável C90 opt-in no CMake, default OFF; novos src/config/config_source.h/.c e tests/config_source_test.c. Não ligada ao produto. AST versionado com strings próprias, linhas/colunas, refs e ausência; const view somente após validação e ownership exclusivo do caller. Coleção listeners[], perfil explícito sem fallback, refs tipadas por objeto/contexto e suporte de target com rejeição de excedentes. Ver [handoff exato](administrative-integration-handoff.md).

O subset ativo implementa binding/channel_tls/access_auth/capability_policy/listener e gates negativos. Known blocos restantes e globals administrativos ainda não suportados rejeitados. Não existe parser aceitando campo para ignorá-lo depois. PC_OK não comprova resolução factual de referência externa, identidade, grants, policy/root ou ED11. Endpoints mesma porta conservadoramente rejeitados até resolução. O modelo/CLI produtivo singular NÃO foi migrado: ADR0009 implementado no candidate offline, integração operacional pendente. Nenhum listener ativado.

Codec binário novo, root persistence/publicação e config apply não implementados por decisão do owner. Não alegar roundtrip/corrupção de codec nem executar migração histórica. BODY/ROOT e API2produtiva/PST/wire/Browser preservados.

## Builds e testes

Debug/Release com MSVC real, /W4 /WX, zero warnings finais e linkage completo dos componentes afetados. CMake usa C_STANDARD90; source/core depende só de headers C padrão, sem Win32/CNG. Auditoria de portabilidade é isolamento de código/build MSVC, não prova executada em box/ISA/NT4 nem outro compilador.

Primeiras execuções:108 verificações cada,0falhas. Após suplemento, finais121 verificações cada,0falhas (242 verificações em duas execuções). Contagem inclui checks auxiliares de fail-closed e não equivale a121casos independentes. [Debug final](Debug-tests2.log), [Release final](Release-tests2.log). Cobertura: versão/UTF8/BOM/CRLF, # e ;, strings/escapes, desativação parcial, duplicatas/refs, perfil e ausência Legacy, Principal/ordem/defaultdeny, capacidade do target e limites token/string/block/linhas, coleção/sameport e sample failclosed. Testes sintéticos de configuração, não autenticação/rede/qualificação de par. Nada de assinatura/CA/certificados/credenciais.

## ED11 e preservação

[Censo compilado](ed11-census.json): PC_DOCUMENT59880, input8192, diagnóstico24,target8 =>68104 bytes, strings/linhas/listeners embutidos contados uma vez. Assembly Release mede prologues locais (parse104/validate152bytes), não o stack total transitivo/CRT nem provider/kernel. Produto não linka componente ou aloca workspace; delta persistente no produto0. A futura composição precisa reserva própria/censo antes de integrar; ED11totalPARTIAL/provider-kernelUNKNOWN/quotas inalteradas.

WIP preservado fora dos hunks exatos e appends factuais das duas live/future docs. Preimagens, verificação e manifesto adjacentes. O baseline não inclui SDKs/binários ignorados pelo Git: nenhum foi sobrescrito e não alegamos novo rehash completo desses arquivos. Não houve clean/clone/worktree/Gitpublish. Build outputs novos e isolados. Diferença de ref não ocorreu: main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a.

```ini
CURRENT_TASK_COMPLETED=YES
PAPACC_CONF_PARSER=IMPLEMENTED_BOUNDED_OFFLINE_SUBSET
PAPACC_CONF_AST=VERSIONED_OWNED_STRINGS_CONST_VALIDATED_VIEW
LISTENER_COLLECTION=IMPLEMENTED_OFFLINE_TARGET_CAPACITY_EXPLICIT
EXPLICIT_PROFILE=REQUIRED_NO_IMPLICIT_LEGACY
ADR0009_IMPLEMENTATION=OFFLINE_CANDIDATE_MODEL_ONLY_RUNTIME_INTEGRATION_PENDING
SEMANTIC_VALIDATION=OFFLINE_STRUCTURAL_AND_TARGET_SUPPORT_EXTERNAL_AUTHORITY_UNRESOLVED
CODEC_OFFLINE=DEFERRED_BY_OWNER_PENDING_EXACT_LAYOUT_REVIEW
UNSUPPORTED_DIRECTIVES=REJECT
OFFICIAL_EXECUTOR_REVIEW=APPROVED_NORMATIVE_AND_EXACT_FUNCTIONAL_OPERATIONS
BUILDS=DEBUG_RELEASE_PASS_AFFECTED_COMPONENTS
WARNINGS=0
OFFLINE_TESTS=121_CHECKS_EACH_DEBUG_RELEASE_ZERO_FAILURES
NETWORK_TESTS=NOT_RUN
CONFIG_PUBLICATION=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_OUTSIDE_EXACT_HUNKS_AND_DOCUMENTED_LIVE_APPENDS
FIRST_UNRESOLVED_DIVERGENCE=OFFLINE_LISTENER_COLLECTION_NOT_YET_BOUND_TO_SINGULAR_RUNTIME_MODEL_AND_FACTUAL_RESOLVERS
NEXT=CANONICAL_ADMIN_CONFIGURATION_INTEGRATION_WITH_CODEC_REVIEW_AND_PUBLICATION_DENIED
```
