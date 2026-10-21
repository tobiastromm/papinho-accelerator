---
status: proposed
scope: future exact acceptance only
---

# Minuta de aceitação futura delimitada

NÃO É REGISTRO ACCEPTED. Arquitetura L0–L5 tem registro separado, da decisão já explícita do owner e revisão oficial de escrita. A nova escolha de ALPN/frames/limites permanece proposta.

Uma revisão futura deve incorporar os bytes exatos de T2 e T6 em COMPLEMENT-REVISION2-PROPOSED.md, conforme clause-digests.json, e reconhecer somente ajustes de coerência T0/T3/T7 mostrados em exact-T2-T6-and-coherence.patch. P0–P9 original deve ser preservado; sua eventual aceitação exige decisão própria do conjunto, não referência automática.

Precedência candidata a aprovar: endpoint separado L0–L5 sobre a antiga proibição deT2, papacc-enroll/1 somente no novo listener; umaCONFIRM no perfil inicial mais restrita que teto5 deP4; T5claimúnico complementaP6; tamanho/tempo/rate/subtetos T2/T6 não alteramcodeTTL300000, quota individual, PACCpapacc/1 ou limitesWindows/Legacy. Não modifica formatos persistentes históricos nem aceita enum/codec novo. Recortar claramente nenhuma divulgação de chave privada, assinatura operacional, CA/CURRENT, recuperação, admissão, patchfuncional ou release.

Antes do registro futuro conferir byte-exatidão, todos os offsets/somas da tabela, estado/direção/erros, compatibilidade com trust/issuer/CSR, limites de recursos e condições de suporte. Registro normativo não é prova nativa. Testes/ED11/codec/Browser permanecem gates de implementação/qualificação, não autorização implícita.
