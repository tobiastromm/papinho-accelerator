# Etapa10 — fechamento offline de readers e discriminação da falha

## Conclusão factual
O peer relatou conteúdo stage10/Win325/kind0. Ainda não demonstra qual arquivo,
qual chamada ou DACL causou a falha. Três saídas do código permitem esse5:
query de handles (sub31), ReOpenFile (sub15), validação final/deadline (sub24 ou
primeiro erro já latched pelo check final). CNG/leitura/hash usam outros mapeamentos.
Não se demonstrou defeito funcional que autorize relaxar proteção ou mudar cursores.

ReOpenFile abre outro objeto de arquivo para cursor independente; o desenho de
conteúdo depende dessa separação. Microsoft documenta que DuplicateHandle mantém
posição de arquivo compartilhada; portanto NÃO foi adotado como substituição
silenciosa de reader privado:
https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle
https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-reopenfile
A documentação não prova qual chamada falhou na sessão observada. A hipótese de
reabertura sob principal virtual sem concessão DACL continua hipótese delimitada,
não resultado de AccessCheck/coleta sob esse token, que não foram executados.

## Integração permitida por F1
Refinado SOMENTE catálogo técnico da primeira falha SQF1, sob F0–F7 accepted:
20consulta/guard handles;21ReOpenFile pedido;22decisão;23imagem;24check final/deadline.
Campo stage já existente/mesmo SQF1 de56bytes, sem novo campo/protocolo/owner/canal,
quota, consulta ou referência. Envelope G3/perfis/origem/bilateral/close intactos.
Índice é derivado do prefixo já retido de readers APENAS quando c.first==15;
bookkeeping inconsistente mantém stage10/UNKNOWN, sem índice inventado.
Kind2/raw usa SOMENTE erro original cached em c.error quando corresponde ao retorno
API. Sub31 também sintetiza quota1816: esse valor permanece kind0, não é promovido
à prova de erro nativo de consulta. Check final permanece kind0. Primeira falha
congelada não muda em chamada subsequente. Frame negativo continua somente DENY.
Nenhum GetLastError tardio, handle/pointer/descritor ou dado de conteúdo é enviado.

## Evidência offline
Corpo real personal_scq_inputs_contents agora testado com ReOpenFile retornando5
em cada um dos3arquivos, demonstrando prefixos de aquisição e primeiro erro15.
Mutação de descritor durante leitura produz mesmo5 no check final e primeiro erro5,
confirmando por modelo que o relato10/5 é ambíguo. Testes do refinador cobrem os
slots consistentes/inconsistentes, erro divergente, primeira falha e quota sintética.
São fronteiras nativas MOCKADAS, não reprodução ou causa histórica comprovada.
Quatro builds sem warnings;24testes afetados por configuração,96pares PASS.
Sem repetir campanhas antigas inalteradas; logs novos preservam provas SQF1 anteriores.
Footprint/root/known iguais ao SQF1 anterior, nenhum byte/handle/consulta adicional.

## Imagem e próximo gate
Candidato 809984bytes/SHA25678886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5, congelado papacc_server.sqf1-content-detail-809984.exe.

Instalado809472/ID417614100:2883584:329728/pin81a849dc… permanece inalterado.
Este delta é diagnóstico, NÃO correção funcional do conteúdo nem prova da causa5.
Não criar UPDATE executável enquanto disposição atual estiver desconhecida.
Sessão consumida; controlador20104/criação134381442604810139; runtime não impresso.
Pedido read-only pronto ONE-SQF1-STAGE10-PARTICIPANTS-READONLY-REQUEST.CANDIDATE.md,
SHA2565ed67e7c2b5c98005d8a451b00e67b19165687910cc14310bc5abe3d4b4f5add,30casos offline PASS.
Não autorizado/executado. Depois de saída atual, eventual disposição G5 própria,
UPDATE U1–U7/U2 específico e sessão nova E/Q/S, sem reutilizar autorizações.
Nenhum encerramento/limpeza/refund inferido; histórico/OS UNKNOWN/PST0.6.4/DENY intactos.
