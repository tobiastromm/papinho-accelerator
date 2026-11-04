# Pedido candidato — uma abertura read-only do pedido P1

Status: READY_FOR_OWNER_DECISION; NOT_AUTHORIZED; NOT_EXECUTED.
Escopo: uma observação prospectiva independente, nunca retry de E/Q/S.

## Limitação que antecede a decisão

É possível reproduzir exatamente os argumentos da NtCreateFile, mas NÃO provar
equivalência integral com a execução histórica. O novo observador é outro
processo/imagem; token efetivo completo (grupos, privilégios, restrições,
AuthenticationId/ModifiedId), namespace DOS, filtros e referências concorrentes
da tentativa passada não foram integralmente capturados. Mesmo SID/elevação
não comprovam essa equivalência. A observação identifica seu próprio NTSTATUS;
se a falha não reaparecer, a causa histórica continua UNKNOWN. Se reaparecer,
há evidência da falha atual, não prova retroativa de identidade de causas.

Não atualizar o EXE instalado para instrumentação. Não repetir run-p1 ou
qualquer driver de sessão, criar originais ou reutilizar decisão consumida.
Sem aceitação explícita deste escopo limitado, não executar o procedimento.

## Operador e artefatos exatos

Host TCORP-TOBIAS, Windows x64/Windows10 ou posterior; SID do operador:
S-1-5-21-785699550-1440458426-1682014700-1108. Token elevado, sem impersonação
de thread. Falha de consulta/contexto ou de fechamento do token: não abrir alvo.
Isso confere o contexto atual, não atribui autoridade por token ou imagem.

Pasta dos artefatos:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101`

| Artefato | Bytes / SHA256 |
|---|---|
| p1-raw-open-observer.exe | 145408 / 7f1f8bddad246144803406a97f6b849c84d4a556d334fbee4448ee680638cf1d |
| p1-raw-open-observer.c | 8553 / 224e017b0fbc6ab0bd2b4e2a2afa7ea11edec3cdc8a001630a6ca476a0f805c3 |
| observe-p1-raw-open.MANUAL.CANDIDATE.ps1 | SHA256 087f5a164f770cd3d239e50765e18fda3577e9bef2c55004ea4e9b77de650c3c |
| build-p1-raw-observer-offline.cmd | SHA256 53b63c34d8984d5a7b0f193cc4378870a459afee225382f9d8be2aaf53d6395b |

Executável workspace exclusivamente diagnóstico/DATA, não executor de UPDATE,
não instalação nem aprovação administrativa. Pins on-disk não são atestado da
imagem mapeada. Rebuild invalida estes pins; não substituir silenciosamente.

## Uma única abertura do alvo

Caminho Win32 histórico exato:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-SESSION-REQUEST.ORIGINAL.md`

Nome NT: prefixo literal `\??\` concatenado a esse caminho, sem normalização,
QueryDosDevice, Shell, OpenFileById, caminho alternativo ou nova descoberta.
ID esperado, somente após sucesso: **417614100:8978432:356716** (volume/high/low).
Tamanho15529 bytes, SHA256
86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34.
Não abrir a decisão nem a imagem instalada. Não enumerar pastas/processos.

| Argumento NtCreateFile | Valor preservado do scqc_open_raw(index0) |
|---|---|
| DesiredAccess | 0x00120089 |
| RootDirectory | NULL |
| ObjectAttributes | 0x1040 = OBJ_CASE_INSENSITIVE / OBJ_DONT_REPARSE |
| SecurityDescriptor / SecurityQualityOfService | NULL / NULL |
| Unicode Length / MaximumLength | mesmos bytes UTF16, ambos sem NUL final |
| AllocationSize | NULL |
| FileAttributes | FILE_ATTRIBUTE_NORMAL (0x80) |
| ShareAccess | FILE_SHARE_READ (1), sem WRITE/DELETE |
| CreateDisposition | FILE_OPEN (1), jamais criar |
| CreateOptions | 0x00200060 |
| EaBuffer / EaLength | NULL / 0 |
| HANDLE / IOSB iniciais | NULL / zerado |

GetDriveTypeW C:\ deve ser FIXED. Consulta de handles no próprio observador
imediatamente antes da abertura: falha ou count>=124 nega; Stop também nega.
Não excluir handles nem deslocar guard para simular margem. O valor113 pertence
à tentativa passada, não é pressuposto para o observador.

## Execução manual futura e resultado

Após aprovação específica, preparar chamada PowerShell5 x64 com hashes reais
do pedido e procedimento. O driver confere pins sem abrir o alvo e chama o
observador uma vez. Confirmação manual própria:
`OBSERVE_P1_REQUEST_RAW_ONCE`
Não é confirmação de endpoint/SCQ/Start. Não fornecer resposta automaticamente.
O pedido não autoriza agora executar qualquer comando.

A única NtCreateFile emite NTSTATUS hexadecimal, snapshots IOSB.Status e
Information, flag de handle e count anterior, sem HANDLE/ponteiro/conteúdo/SD.
IOSB zerado em falha não comprova escrita/sucesso. Não usar GetLastError como
tradução. A API retorna NTSTATUS: [Microsoft NtCreateFile](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile).

Se falha definitiva sem handle: registrar status e terminar, sem fallback.
Se pending/status positivo/handle inesperado: UNKNOWN, sem wait/reopen/retry;
não declarar fechamento conhecido dessa coorte. Término do processo não prova
quiescência OS ou resolve retenção histórica.
Somente em sucesso integral (status0, IOSB.Status0, Information1, handle válido):
consultar ID/tamanho/atributos, rejeitar reparse/diretório/ID divergente, ler no
máximo15530 bytes pela MESMA referência, conferir15529/SHA256 esperado,
revalidar identidade/tamanho e fechar handle. Nunca imprimir conteúdo.
Hash por BCrypt SHA256 sem chave/KSP, disponível por pseudo-handle Windows10:
[Microsoft CNG](https://learn.microsoft.com/en-us/windows/win32/seccng/cng-algorithm-pseudo-handles).
Isso comprova conteúdo/ID atual quando bem-sucedido, não toda proteção/continuidade
histórica de ancestrais e não admissão administrativa. O protocolo não afrouxa
ausência de reparse da chamada; não faz AccessCheck ou certificação de ACLs.

Stop Ctrl+C/controle de console impede próxima etapa; não garante cancelamento
imediato de I/O síncrono. Se há handle válido concluído, tentar CloseHandle uma
vez e reportar o resultado; falha mantém UNKNOWN. Sem cleanup/recovery/retry.

## Custos, efeitos e compatibilidade

Custos controlados: processo diagnóstico/CRT/console; token temporário QUERY
fechado ANTES da abertura; um handle read-only de arquivo; buffer15530 bytes,
digest32, objeto hash transitório; consultas de metadados e uma leitura limitada.
Custos loader/kernel/provider/allocator/cache e latências continuam UNKNOWN.
Hash não dá autoridade; não aciona chave privada/KSP/TPM. Nenhum ceiling nativo
é deduzido desses tamanhos. O observador tem custos próprios e contagem própria.

Sem escritas solicitadas, criação/rename/exclusão/ACL/SCM; acesso/time/cache ou
auditoria mantidos pelo Windows não são prometidos ausentes. ShareREAD pode
temporariamente excluir escritor/rename, exatamente como na chamada original.
Não reter streams do wrapper sobre o alvo, pois isso perturbaria a reprodução.

G3/G4, TO0–TO9, QUERY/G3 e P0–P7 permanecem íntegros: guard124/cap128, crescimento
1994944 bytes privado/WS por participante, pisos7239172096/9823326208 bytes,
freshness100ms, janela240000ms, duração60000ms sem renovação. Esta observação
NÃO é participante/session READY nem prova capacidade G3; não usa a janela
consumida, não concede margem/capacidade ou novo Start. Não transportar G0–G9
como licença para ultrapassar quotas. Nenhuma especialização normativa nova.

Sem serviço/root/protocolo/canal, sem nova autoridade. Uso de stdout e observador
manual descartável, como os cortes anteriores; não infraestrutura permanente.
Preservar todos os objetos/originais/EXEs, HEAD/WIP/pins/históricos/PST0.6.4,
autorização E/Q/S consumida, retenção histórica e OS_QUIESCENCE UNKNOWN.

## Qualificação e primeiro gate

MSVC18 x64, build release do observador e build model /W4 /WX PASS, sem warnings.
23checks modelados: constantes/nome/argumentos, count113/123/124/128/query UNKNOWN,
Stop, sucesso, ausência de handle, falhas access/sharing/reparse, IOSB divergente,
pending e handle inesperado. Modelo não chama token/NtCreateFile/SCM/endpoints.
AST PowerShell PASS; driver e observador nativo NÃO executados.
Não repetir builds Selected/Legacy do produto: produto/formatos não alterados.

Primeiro gate: decisão específica do proprietário sobre esta única observação,
incluindo a limitação de equivalência. Após aprovação, registrar autorização e
preparar comando literal pinado; não executar automaticamente. Não reutilizar
a aprovação de E/Q/S nem afirmar resolução da causa histórica antes de evidência.
