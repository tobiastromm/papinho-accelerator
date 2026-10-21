# API exata do primeiro slice estrutural

TASK=PACC-PHASE6-SHARED-PROVISIONING-API-MIGRATION-1. Autoridade S0–S8 ACCEPTED. Implementação independente de papacc_protocol, em src/provisioning/shared_structure. SP_* é namespace técnico privado deste slice; não nome público/API1.1.0 de PapinhoPACC nem versão de release nova. Header/source exatos em source-operations.json/exact-source.patch.

## Funções e resultados

- sp_envelope_inspect: somente forma M2 PACCMP02/version2/suite1, header92, lengths/flags/params/total. Não valida DER/SPKI matematicamente, assinatura, KDF, AEAD ou trust. Saída SP_ENVELOPE com quatro views emprestadas.
- sp_payload_inspect: ZIP32 STORE canônico, cinco nomes/ordem/CRC/local/central/EOCD e metadata M3 accepted. Não extrai arquivos. Entrada deve ser quarentena privada do caller já autenticada quando for conteúdo real. Testes usam somente bytes públicos sintéticos, não material DER/chave ou pacote protegido.
- sp_metadata_validate: kind0 config ou1 issuance, tags16/5 estritamente ordenadas, campos/caps/UTF8/DNS/tempos como no parser original. Valida sintaxe, não fontes factuais, perfil X509 ou CURRENT.
- sp_payload_field: revalida metadata, retorna view; inválido/ausente NULL,0; issuance tag5 presente-vazio nonNULL,0. Não cria proveniência ou autoridade.
- sp_password_normalize: mesma regra ASCII do adapter Browser, entrada<=128, saída32 Base32 A-Z2-7, remove só espaço/hífen e dobra lowercase. DATA_ONLY não é força/origem CSPRNG, confiança ou KDF. KDF/AEAD não migrados.

SP_DATA_ONLY=1; SP_INVALID=-1; SP_LIMIT=-2; SP_STOPPED=-3. Não existe TRUSTED, RESOLVED, CURRENT ou INSTALL sucesso. Parsers deixam outputs inalterados em toda falha/Stop; saída normalizer32 é apagada em falha após preflight válido/disjunto. Precondições inválidas/aliases não alteram input/output. Grammar rejection comum equivale aos originais; categorias de erro são API distinta: Browser conflui limites em INVALID e Accelerator zera view em algumas falhas. Isso não é mudança wire/gramática. Wrappers futuros deverão preservar contrato do caller; nenhum aplicado nesta tarefa.

## Buffers, lifetime e Stop

Core não aloca, escreve arquivo, retém referência, inicia thread ou acessa provider. Caller mantém input imutável, outputs disjuntos, serialização e referências próprias até fim factual de todos os borrows. Views não são cópias dos buffers. Stop observacional ocorre na entrada/entre membros ZIP/antes do resultado; callback não reentra, não libera input nem cria borrowers. Não há API de cancelamento/destruição nativa. Parâmetros Stop são observação local, nunca booleanos de autoridade. A função síncrona retorna sem retenção interna, mas isso não prova quiescência externa ou libera crédito do owner. Segredos só são apagados depois de fim factual de seus borrows.

M2 input<=131072, SPKI<=1024, cipher<=65536, tag16/signature256; M3<=49654, caps16384/16384/8192/4096/4096. ZIP STORE dispensa decompressor. Core de dados pode inspecionar arrays sintéticos sem confiança; chamadas com conteúdo real permanecem subordinadas ao pipeline M1, nunca uma entrada alternativa de instalação.

## Portabilidade e dependências

C89, somente stddef/limits/string, tipo32 checado por ULONG_MAX/UINT_MAX/sizeof, bytes8bits e flat-address sizeof(size_t)==sizeof(void*) verificados. Conversões de ponteiro para size_t são hipótese local de targets flat revisados, nunca wire. Não qualifica arquitetura segmentada/box/ISA sem adaptação/prova. Sem Win32/PST/BearSSL/OpenSSL/root/issuer no core. Wipe local copia a rotina C89 volatile já revisada; não é nova primitiva. Sem ABI binária obrigatória; release/name/licença própria/distribuição pública pendentes.

## Limites da primeira extração

Os originais dos dois projetos permanecem intactos até equivalência e autorização da migração de consumidores. Oracles congelam preimagens para comparação, não são forks produtivos: Browser somente prefixo parser e normalizer; include hash não usado e tail de proveniência excluídos, corpos de gramática intactos. Accelerator oracle original inteiro, acrescido de accessor de teste para metadata static. Original stdint é compilado no moderno; VC6 testa core C89 e original Browser. Nenhum KDF/AEAD/trust/instalação ou fixture anteriormente rejeitada é executado. UI/store/advanced/produção preservados. Sem original removido nesta entrega.
