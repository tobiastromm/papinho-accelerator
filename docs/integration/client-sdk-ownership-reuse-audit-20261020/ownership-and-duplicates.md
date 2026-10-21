# Fronteiras e duplicações

Letras A-F seguem o pedido: protocolo, provisão compartilhada, adapter, Browser, Accelerator, indefinido. Não confundir com A/B/C/D do PEADR0011. Neste último, gramática comum exige shared owner C até designação, D depois; UI é product-specific A; fronteira indecisa B/review.

| Grupo | Correspondência | Tratamento |
|---|---|---|
| PACC wire | sete fontes canônicas e snapshot20arquivos | reúso correto;sem fork observado |
| M2 | mp_envelope_inspect vs papinho_papacc_frame_read | mesma gramática;consolidar sem reescrita |
| M3 | mp_payload_inspect/metadata/dns/text/crc32_zip vs Browser pc_* | port explícito com tipos/namespaces/Stop diferentes;duplicação semântica |
| Password/M1 suite | mp_password_normalize/derive vs papinho_papacc_crypto_* | invariantes comuns;adapters diferentes legítimos |
| RSA/X509 | EVP versus BearSSL/credential_import | compartilhar predicados/perfil/vetores,não engine ou CNG para NT4 |
| M1/D3 | pipeline Accelerator vs prefixo/raw Browser | não são dois pipelines completos;ambos negam autoridade ausente |
| PDG1 vs provenanceBrowser | PACROOT8/PDG vs PBPRV002 | domínios diferentes;NÃO duplicação;Browser não conhece root |
| Enrollment | PAE1 helpers Accelerator;cliente Browser não localizado | prevenir nova cópia;claim/admin/RNG servidor permanecem exclusivos |

Dependência atual Browser -> PACC/PST/BearSSL e Accelerator -> PACC/PST/issuer/admin; não foi identificado ciclo efetivo no pacote. Publicar material.h/credential_store.h ou validation.h/root8 privado introduziria dependência indevida. Header client Browser mistura Win32/Winsock/PST/HTTP e não é API de SDK portátil extraível inteira. Extração do que é comum deve preservar adapters/lifetimes e não arrastar UI/store/root. Inventário CSV e source-symbols.json fornecem arquivos/funções/hashes.
