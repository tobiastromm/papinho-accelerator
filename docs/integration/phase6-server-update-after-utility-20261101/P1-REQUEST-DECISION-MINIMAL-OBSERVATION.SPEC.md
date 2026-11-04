# Especificação técnica — cortes do intervalo request0 -> guard decisão

PREPARATION_ONLY / NOT_IMPLEMENTED / NOT_AUTHORIZED / NOT_EXECUTED.
Não é nova norma/ADR nem pedido operacional completo. A revisão offline
P1-REQUEST-DECISION-OWNERSHIP.md não demonstrou fechamento adicional legítimo.

## Questão exata e suficiência

Medir Q0 da MESMA execução e separar o saldo por NtCreateFile, metadados/SD,
hash com cleanup e resolver da decisão. Um novo total125 ou68 isolado não basta.
Nove cortes em UMA execução, mesmos dados/APIs, com flags de recursos próprios,
localizam o grupo que mudou. Não prometem atribuição individual dos internals,
equivalência retroativa ou prova de ausência de atividade assíncrona.
Se o saldo surgir apenas em recursos internos após cleanup público correto,
concluir que fechamento arbitrário não é legítimo; não abrir uma campanha de
enumeração de todos os handles para tentar justificar CloseHandle sem ownership.

## Disponibilização mínima, antes de solicitar autorização

Reutilizar o percurso de observação frio já existente no launcher/owner; NÃO
invocar scqc_plan/cold_controller ou o driver E/Q/S consumido, pois avançariam
para decisão/imagem/nonce/autoridade. Isolar somente a sequência abaixo sob
modo diagnóstico read-only fixo e qualificação offline com APIs simuladas.
Não criar novo serviço/root/canal/owner/subsistema nem converter modo em entrada
administrativa genérica. Não transplantar o último corte antigo de knownfolders:
startup/P1/resolver devem corresponder ao percurso corrigido, sem Shell extra.

O binário diagnóstico poderá ser workspace somente leitura/sem autoridade,
com mesma composição CRT/toolchain/imports relevantes do controlador, sem
UPDATE de imagem instalada apenas para instrumentar. Antes de pedir execução,
auditar essa equivalência e diferenças do observador. Não alegar que processo
independente/token diverso reproduzem integralmente o125 histórico.
Se a arquitetura não permitir isso sem violar fonte/ownership, delimitar a
limitação e manter NOT_READY; não emitir comando/artefato fictício.

## Alvos e operações estritamente limitados

Request DATA/read-only existente:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-CHAIN-SESSION-REQUEST.ORIGINAL.md`,
ID histórico esperado417614100:1441792:542844,16483bytes,
SHA256608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441.
Não promover aprovação consumida em autoridade: uma NOVA autorização de
observação será necessária. Não criar/adotar/recriar esse arquivo ou decisão.

Nome da decisão só como DATA para GetDriveType/QueryDosDevice; NÃO abrir:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-CHAIN-OPERATIONAL-DECISION.ORIGINAL.txt`.
Sem imagem/ancestrais novos, SCM/RNG/endpoint/SCQ/Start/Stop ou leitura de segredo.

Uma NtCreateFile(request) com direitos00120089/shareREAD/FILE_OPEN1/options00200060,
RootDirectoryNULL, CASE_INSENSITIVE|DONT_REPARSE e o resolver nativo P1 atual.
Mesmas consultas de tipo/ID/SD e hash SHA256 do descritor pelo helper existente,
sem trocar provider/pseudo-handle, ordem de cleanup ou exclusividade. Verificar
identidade/limites de objeto conforme escopo autorizado, sem publicar conteúdo/SD.
Stop/guard124/falha anterior negam novas aquisições; não remover/adiar guard.
No máximo um HANDLE de arquivo próprio retido até fechamento diagnóstico.

## Cortes necessários

| Corte | Local |
|---|---|
| 0 | Query original imediatamente anterior a GetModuleHandle/GetProcAddress/request NtCreateFile |
| 1 | Após lookup ntdll/símbolo, antes da NtCreateFile |
| 2 | Após NtCreateFile síncrona validada, antes de consultas de identidade |
| 3 | Após GetFileType/GetFileInformationByHandle, antes de GetSecurityInfo |
| 4 | Após SD adquirido/validado, antes de papacc_store_hash |
| 5 | Após retorno do hash, incluindo DestroyHash/CloseAlgorithmProvider originais |
| 6 | Após LocalFree e slotcheckingNULL |
| 7 | Após GetDriveType da decisão, antes de resolver seu nome |
| 8 | Após QueryDosDevice/resolver da decisão: query terminal equivalente ao guard; NÃO abrir decisão |

Cada amostra: DWORD known/count/error/flags. Flags apenas presença de raw0,
checking, hash/provider, active/retained/Stop; sem HANDLE/ponteiro/SID/token
exportado ou descritor/conteúdo. 9*16=144bytes de armazenamento das amostras,
não quota ou teto nativo. Usar pseudohandle próprio; não abrir processo/arquivo
de output, enumerar handles ou imprimir entre os cortes. Emissão só ao final.
Mesmas APIs mutantes não são necessárias; a única abertura é FILE_OPEN read-only.

O custo de GetProcessHandleCount, novas branches/buffer/CRT/loader do observador
deve ser declarado; chamadas extras não são observações atômicas nem garantia
de zero perturbação. Não subtrair suposto overhead para obter PASS ou mover
recursos necessários a outro processo. O resultado é diagnóstico, nunca G3READY.

## Falhas e interpretação

Preservar primeira falha/corte não alcançado como UNKNOWN, não preencher zeros
como sucesso. NTSTATUS/IOSB/handle flag separados dos counts. Pending/resultado
incerto mantém retenção; não esperar/reabrir/tentar cleanup não qualificado.
Handle próprio concluído fecha uma vez; DestroyHash/CloseAlgorithmProvider/
LocalFree mantêm seus contratos. Falha de close preserva UNKNOWN, não retry.
Autorizações anteriores/retidos históricos não são usados ou liberados.

Incremento ao redor de uma API localiza uma fase, não necessariamente um leak
ou posse individual. Flags provider/hash0 no corte5 comprovam estado público
do helper, não fechamento universal dos internos Windows. Se todos os cortes
forem inconclusivos, declarar resultado, sem repetir ou alterar quotas.

Qualificar offline antes de pedido operacional: sucesso modelado, count123/124/
128/queryfalha, Stop por corte, SD/hash/cleanupfalho, reparse/ID divergente,
proibição de abrir decisão e ausência de RNG/SCM/endpoint/Start, retenção/close
UNKNOWN. Só então congelar pins e solicitar UMA execução manual própria.
Nada disso autoriza coletar agora. Preservar124/128/G3/DENY/UNKNOWN/PST0.6.4.
