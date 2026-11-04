# Reconciliação — leitura focal do parent FCA

Resultado recebido do proprietário: autorização consumida; first=NATIVE_OPEN index2 ntstatus0xc0000034; complete=False; observer_close_known=True. Nenhuma criação ou efeito mutante. Saída transcrita em OWNER-FCA-DATA-PARENT-READONLY-OUTPUT.txt como UTF-8/LF normalizado do chat, sem alegar bytes originais do terminal. Resultado estruturado: FCA-DATA-PARENT-READONLY-RESULT.json. Registro de autorização atualizado como consumido, sem retry. Não foi executada nova coleta.

## Dois bloqueios distintos

1. O resolvedor retornou C:/ProgramData/Papinho. Volume e ProgramData abriram; a abertura relativa Papinho retornou0xc0000034 sem handle. A ausência é evidência daquela consulta, não lease de inexistência ou autorização para criar/adotar. ID de Papinho NOT_ACQUIRED. Nenhum leaf Accelerator foi consultado.
2. O descritor capturado de ProgramData foi projetado como REJECT na ACE4 BUILTIN Users mask0x116, mesmo com owner confiável e DACL protegida. Causa da rejeição de política demonstrada nesses bytes; não é falha de recurso, alteração de ACL necessária ou defeito de OpenFile. O SD bruto não foi exportado. Não alcançou revalidação: não anunciar continuidade comprovada de toda a cadeia.

Volume417614100:327680:5, hashSDdd12fd358f9aec2929310ef1c7601e65a84c9960da68a7e46fd1a3a5f1a924c2, projeção match. ProgramData417614100:131072:227641, hashSDf20ee8791a5de98b673aea4ee150ac1380c08c64ec536309f699668fe06864f2, projeção reject. Hashes de capturas históricas OWNER|GROUP|DACL não são diretamente comparáveis com esta OWNER|DACL; a diferença não demonstra mudança de ACL. Projeção do observador não é admissão nativa FCA.

## Confronto com contratos

F0/F1/F2 exigem administração legítima/direitos e originais; F3/F5 delimitam plano/continuidade e uma criação por ato. FCA1 conserva proteção/ancestrais/semreparse, FCA4 permite um contêiner novo e uma chave, não um parent extra. `personal_first_open` exige parent original existente e a política conservadora `scqi_directory_sd` rejeita escrita por não confiáveis fora da exceção de volume. Não existe autorização atual para contornar isso. Código está cumprindo o gate conservador; não foi demonstrado defeito funcional que autorize relaxamento autônomo.

Não criar Papinho por fora, pois isso não resolve a rejeição do ancestral. Não alterar ACL de ProgramData, redirecionar instalação, importar identidade histórica, desligar OBJ_DONT_REPARSE, criar owner/CLI universal ou aumentar recursos. Não há razão para repetir a leitura nem os builds inalterados.

## Proposta focal única

FCA-DATA-PARENT-PDP0-PDP7.CANDIDATE.md, SHA256e68969bed2a8d84ab366378cedc991bc1f105964cfcfff4c7e099d56cd8acd14. AlternativaA mantém DENY; B propõe integração/qualificação OFFLINE de um ato preparatório distinto para criar exclusivamente Papinho no mesmo mecanismo administrativo/owner existente, sem CA/Accelerator, e exceção focal à ACE0x116 no objeto KnownFolder ProgramData somente nessa composição e FCA posterior. Nada de remediação de ACL ou mudança de local. Papinho novo tem proteção final estrita na criação, consumo único e readback; a futura FCA continua independente, um contêiner/uma chave, com originais readquiridos e nenhuma adoção de resultado incerto.

A exceção é proposta normativa, não correção de bug nem garantia de segurança nativa já comprovada. Retenção/compartilhamento/reparse/rights devem passar na qualificação; qualquer necessidade de ampliar escopo mantém STOP. É a menor decisão conjunta para os dois bloqueios observados: só autorizar uma criação não remove a restrição do ancestral; só alterar a projeção não materializa o parent ausente.

Primeiro gate restante: aceitação explícita dessa alternativaB, caso desejada. Nenhuma implementação dependente foi aplicada. Depois registrar norma designada, implementar/qualificar no mecanismo existente e determinar delta real; UPDATE e criação reais permanecem pedidos operacionais próprios. Source mínimo43bytes segue DATA, original não materializado. Imagem905728/ID417614100:2555904:1047601 e hashes/históricos preservados. PST0.6.4, readiness DENY, P4/K5 não qualificados, retenção histórica e quiescência OS UNKNOWN.