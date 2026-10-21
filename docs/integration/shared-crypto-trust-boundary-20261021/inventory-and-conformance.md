# Inventário funcional e conformidade

Revisão2026-10-21, tarefa PACC-PHASE6-SHARED-CRYPTO-TRUST-BOUNDARY-1. HEAD Accelerator c50b8fb9a7e3293b5e6355fc08459bf85b56857a e Browser1eb028554f622672377953225e162ed9b6564a40, branches main, WIP preservado pela baseline. S0–S8 é autoridade de ownership, não aprovação automática de API de autoridade. M0–M8/D3/F1–F5/G0–G7 permanecem intactos. A normalização/gramática já consumida pelo Browser não é reextraída nem seu snapshot editado.

| Regra | Accelerator | Browser atual | Destino/restrição |
|---|---|---|---|
| Normalização ASCII/Base32 | manual_package_crypto_openssl.c agora wrapper sp_password_normalize | papinho_papacc_crypto_password já wrapper do snapshot estrutural | Fonte comum; precondições distintas preservadas |
| KDF | EVP PBKDF2-HMAC-SHA256600000/16/32 | NSS-derived PBKDF2/BearSSL HMAC SHA256 | Primitivas/backend distintos legítimos; macro comum600000 no adapter EVP |
| Entrada derive | aceita senha a normalizar | exige32 bytes previamente normalizados | Não unificar silenciosamente assinaturas/erros |
| AEAD | EVP ChaCha20-Poly1305; q e staging disjuntos | BearSSL ChaCha20_ct/Poly1305_ctmul32; q in-place privada | Mesmo RFC8439/key32/nonce12/tag16; layout de buffers específico |
| AAD | adapter exige ponteiro nãoNULL, mesmo tamanho0 | NULL permitido se0 | Predicado comum de tamanho não substitui preflight de ponteiros/aliases |
| Domínio assinado | signature_exact consome SP_SIGNED_PARTS | public_check mantém string+sizeof | Mesmos26bytes incluindo NUL e prefix excluindo256; Browser migração posterior |
| RSA/DER/SPKI | EVP DER roundtrip/RSA2048/e65537/PKCS1v1.5SHA256 | decoder estrito + BearSSL i31 PKCS1verify | Diferenças legítimas, hash/assinatura não concedem trust |
| Certificado/chave | mp_crypto_key_match; perfil público separado | material_pair_only -> credential_material_validate | Correspondência não equivale a chain/EKU/validade/status/autoridade; não extrair Win32 para core |
| M1 | validation.c com port nativa fixa | UI nega antes de senha/KDF; helpers CRYPTO_ONLY fora da UI | Não criar novo M1 paralelo nem gateway permissivo |
| Quarentena | q,staging,output disjuntos; única cópia final | q privada depois de tag; instalação existente ainda negada | Não compartilhar backing antes de fim factual |
| D3/F1–F3 | seleção PDG DATA nega SOURCE_ABSENT | confirmação independente ainda sem fonte ligada | Dados decodificados/callbackbool nunca evidência factual |
| Stop | fronteiras síncronas/provider KDF não interrompível | loop PBKDF2 com observação/callback restrito | Stop não implica cancelamento/destruição |
| Retenção | abort/finish retém contexto e buffers; query-only, nunca late publish | owner/generation/leases específicos preservados | Não mover owner Browser para core |
| Limpeza | OPENSSL_cleanse backend; normalizer comum volatile reviewed | papinho_c2_wipe e normalizer comum | Somente memória própria quiescente; sem segredo em disco/log |
| Provenance/import | nenhum efeito runtime/CLI | provenancev2,writer,simple/advanced intactos | Não alterar Browser nesta tarefa |

Não foi encontrada divergência de algoritmo ou domínio assinado nos caminhos auditados/corpus executado. Isto não é equivalência universal. Não foi demonstrado um adaptador factual de trust; sua ausência é gate conhecido, não conflito normativo resolvido por esta revisão. A fonte atual privada MP_AUTH_INPUT/OPERATION inclui config_root8_v3.h e não é API cliente compartilhada elegível sem separação explícita.

Patch oficial01: cinco arquivos novos de suite DATA/teste/API. Patch oficial02: somente adapter EVP e CMake offline passam a consumir normalização/domínio comuns. Nenhum header de autoridade, M1 produtivo ou Browser modificado. Hunks/preimagens/hashes anexos. Máquinas M1 ou parsers antigos não foram copiados para criar implementação paralela. Arquivo suite.h é API privada de dados; shared M1 e evidência factual continuam PENDING_REVIEW_AND_BINDING.

Regressões novas usam1611 checks DATA por configuração moderna/VC6; M1 existente146 sintéticos +1189 crypto por Selected/Legacy Debug/Release; oracle Browser940 por Debug/Release. Vetores backend conhecidos KDF/AEAD e comparação BCrypt permanecem, não um novo harness diferencial simultâneo EVP/BearSSL. Plano assinado compara offsets com oracle Browser congelado; RSA usa fixtures públicas anteriores em cada suíte. Não alegar uma cobertura diferencial que não foi executada.

Preservada negativa histórica da geração de fixtures protegidas Browser; não houve nova geração, pacote operacional ou retry. Regressão Accelerator reutiliza seu teste histórico com assinatura TEST ONLY em memória, aprovado especificamente, não assinatura produtiva nem geração em disco de fixture protegida. Novos testes contêm só arrays estruturais públicos sem DER/chaves/crypto. Não houve negativa oficial nesta tarefa.
