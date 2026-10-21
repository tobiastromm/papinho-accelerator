# Prova de extração, equivalência e limites

API exata revisada/aprovada antes das fontes: API-AND-LIFETIME.md. Payload oficial25 arquivos novos, sem mudanças de fontes preexistentes ou Browser. Núcleo complementar em src/provisioning/shared_structure, biblioteca independente de papacc_protocol; não ligada ao servidor ou Browser nesta primeira etapa. Originais permanecem disponíveis até migração de consumidores autorizada, não são novos forks produtivos.

## Reúso comprovável

M3 vem do port Browser já derivado do parser Accelerator: helpers de CRC/DNS/UTF8/TLV e reader reutilizados com namespace, tipos32 checados e rótulos de resultado. M2 conserva predicados do reader Accelerator, com output local até sucesso e Stop observacional. Normalização copia o adapter Browser e wipe volatile existente. Não migrados KDF/AEAD, crypto providers, M1/trust, issuer, root, instalação, UI, storage ou import avançada. Nenhum algoritmo reescrito por estilo.

Oracles: original manual_package.c integral em bytes seguido de accessor de teste para metadata static; headers/frame Browser byte-exatos. Browser content usa prefixo até antes do hash/provenance tail, remove apenas include BearSSL hash não usado e acrescenta accessor de metadata. Corpos de parser/normalizer são preservados textualmente; essas extrações normalizam CRLF para LF, explicitamente distinto de cópia byte-exata do arquivo inteiro. origins-and-transforms.json fixa fontes e transformações; oracle-verification.json confere o conteúdo efetivo. Oracles não entram em distribuição produtiva.

## Equivalência observada

Moderno Debug/Release:19.815 verificações diferenciais por configuração entre original Accelerator, original Browser e novo núcleo, mais2.077 da regressão estrutural original Accelerator. VC6 real /Za /W3 /WX /MD Debug/Release:15.681 checks entre leitor Browser e core. Original Accelerator/stdint permanece referência moderna; não se afirma compilação VC6 desse original. Zero warnings nos builds finais. São execuções de parser C real sobre dados públicos sintéticos, não callbacks que retornam sucesso.

Corpus: ZIP válido em memória, bitflips em todos os bytes, todos truncamentos do sample, trailing, duplicatas/traversal/CRC/EOCD/local-central/flags, metadata/tag/campos/UTF8 válido e inválido/DNS253/trailing/numeric/wildcard, limites envelope SPKI1/1024,cipher1/65536, versões/flags/custo/total e truncamento, alias/output nulo, Stop nos checkpoints, campo presente-vazio, views e normalização ASCII/bounds/limpeza. O arquivo chamado cliente.pk8 contém somente ASCII123456789, sem DER/chave/certificado real. Envelope tem SPKI falso/assinatura zero e não é autenticável. Nenhuma geração de fixture protegida anteriormente negada, assinatura ou KDF ocorreu.

Nenhuma divergência de ACEITAÇÃO estrutural no corpus. Isso não é prova matemática de equivalência para todo input. Categorias/output em falha são deliberadamente distintos na API nova: Browser reduz limites a INVALID e deixa outputs; Accelerator classifica LIMIT e zera views após preflight. Core define LIMIT explícito e output inalterado; os testes comparam aceitação/offsets/lengths e conferem outputs segundo contrato, não escondem essas diferenças. Wrappers futuros precisam preservar contrato do caller. Normalização foi comparada diretamente ao adapter Browser, não ao provider KDF Accelerator.

## Lifetime

Views apontam para input do caller, sem cópia ou owner novo. Input/quarentena real deve permanecer privado/imutável durante todos os borrows. Stop somente observa; não fecha, cancela I/O nem reembolsa créditos. Não há retenção interna ou memória dinâmica. Retenção externa e fontes continuam sob os owners existentes e ModelE. Nenhum sucesso DATA pode entrar no pipeline M1 como TRUST/RESOLVED ou permitir instalação. O core não aceita issuer/CURRENT/Principal/provider/factory de autoridade.

## Falha de build preservada

Primeira execução VC6 compilou todas as fontes sem warning; script usava /Fe: e VC6 interpretou ':' como nome. Debug falhou LNK1201 no PDB, Release não encontrou o EXE esperado. Correção oficial exata só nos dois argumentos /Fe, mesma toolchain/flags/target. Logs iniciais preservados; retry1 passou. Nenhuma negativa oficial nesta tarefa foi contornada, nem fonte alterada para obter build.

## Limite operacional

Não executados Browser principal/GUI/import, rede, NT4 nativo, box/ISA, TLS ou qualificação operacional. Browser permanece byte-exato no escopo do baseline; isso não renova PASS histórico da aplicação inteira. Primeiro slice concluído como componente/testes; consumo no Browser NÃO autorizado nesta tarefa. Nome público/versão/licença/release/snapshot ingerido continuam pendentes.
