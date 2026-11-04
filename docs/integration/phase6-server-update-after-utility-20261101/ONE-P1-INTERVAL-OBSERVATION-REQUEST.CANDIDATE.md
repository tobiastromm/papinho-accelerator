# Pedido candidato — uma observação prospectiva P1 de nove cortes

2026-11-02, America/Sao_Paulo. READY_FOR_OWNER_DECISION / NOT_AUTHORIZED /
NOT_EXECUTED. Não é aprovação de E/Q/S, nova sessão Start ou decisão normativa.
Autorizações anteriores permanecem consumidas. Não executar este candidato sem
autorização específica do proprietário. Nenhuma decisão emitida é fabricada aqui.

## Operador, artefatos e chamada única

Host selecionado TCORP-TOBIAS; operador administrativo original
S-1-5-21-785699550-1440458426-1682014700-1108, sem impersonation.
Windows PowerShell 5.1 Desktop x64, console interativo sem redirecionamento.

Pasta exata do executável workspace:
`C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release`.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 779776 | 7b6d5e500723ba5db2f1d337cb1ca55e9fa7b3c5588ca6b31d4c7d81e693e726 |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Procedimento candidato nesta mesma pasta documental:
`observe-p1-interval.MANUAL.CANDIDATE.ps1`, SHA256
`22906613fd4ff0e9206e830160caac7109225d23c845c7aadf6020ec07e5ffd9`.
Após autorização, preparar invocação PowerShell com ExpectedScriptSha256 e
ExpectedRequestSha256 reais. O wrapper verifica esses pins, mantém referências
read-only sem share-write/delete aos três artefatos e exige digitação manual
`OBSERVE_P1_INTERVAL_ONCE`. Não fornece respostas automaticamente.

A única chamada nativa prevista é o EXE acima com exatamente um argumento:
`--observe-p1-request-decision-interval`. Não aceita paths/IDs/grants adicionais.
Não executar o binário instalado, o driver P1 consumido ou o baseline preservado.
Hashes em disco não são autoridade nem atestação da imagem mapeada.

## Alvos e parâmetros exatos

Único arquivo aberto pelo observador nativo, como DATA read-only:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-CHAIN-SESSION-REQUEST.ORIGINAL.md`.
Esperados volume417614100/file1441792:542844,16483bytes,um link,arquivo disk,
sem DIRECTORY/REPARSE. O SHA256 histórico do conteúdo é
`608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441`;
esta prova NÃO lê/hasheia o corpo. Verifica ID/tipo/tamanho atual e usa o mesmo
hash do DESCRITOR do percurso real, sem publicar descritor/digest/conteúdo.
Não recertifica OHI nem a autoridade da autorização histórica consumida.

Nome somente DATA, sem abrir o objeto da decisão:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-CHAIN-OPERATIONAL-DECISION.ORIGINAL.txt`.
ID histórico417614100:1179648:542846 não é adquirido ou revalidado pela prova.
Nenhuma abertura de imagem, ancestrais, SCM, duplicatas ou readers posteriores.

Resolver nativo P1 compartilhado: GetDriveTypeW/QueryDosDeviceW, somente nome
direto permitido; não aceita SUBST/UNC/aliases recursivos por conveniência.
NtCreateFile com access0x00120089,shareFILE_SHARE_READ(1),dispositionFILE_OPEN(1),
options0x00200060,attributesFILE_ATTRIBUTE_NORMAL,RootDirectoryNULL,
OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE. Sem criação/traversal de reparse.
Primeiro NTSTATUS, IOSB inicializado e flag de handle retornado são separados.
Zero em snapshot não prova que uma chamada não alcançada tenha sucesso.

## Nove cortes, na mesma execução e ordem

| Corte | Posição |
|---|---|
| 0 | Query original imediatamente antes de lookup ntdll/símbolo/open do pedido |
| 1 | Após GetModuleHandleW/GetProcAddress, antes da NtCreateFile |
| 2 | Após abertura síncrona validada, antes de tipo/identidade |
| 3 | Após GetFileType/GetFileInformationByHandle e conferência ID/tipo/tamanho |
| 4 | Após GetSecurityInfo e IsValidSecurityDescriptor, antes do hash |
| 5 | Após papacc_store_hash, incluindo DestroyHash/CloseAlgorithmProvider |
| 6 | Após LocalFree e checkingNULL |
| 7 | Após GetDriveTypeW do nome da decisão |
| 8 | Após resolver/QueryDosDeviceW da decisão, query terminal sem abri-la |

Cada corte known/count/error/slots. Flags slots: raw0=1,checking=2,hash=4,
provider=8,active=16,retained=32,Stop=64. São estados públicos próprios;
hash/provider não equivalem automaticamente a dois handles da tabela executiva.
Cortes não alcançados known0 permanecem UNKNOWN, nunca contagem zero válida.
first_cut9 designa falha terminal de fechamento fora dos nove cortes;
MAXDWORD indica nenhuma falha localizada. hash_called separa status não obtido.

## Equivalência e limites de interpretação

Reutilizados owner, console, token, resolver, helper NtCreateFile, aquisição SD,
papacc_store_hash e cleanup originais. O caminho operacional sem observador
mantém NULL e não recebe as sete queries extras. Nenhuma operação foi movida
para depois da guarda ou outro processo para fabricar admissão.

O seletor operacional exige módulo instalado. A prova workspace NÃO contorna
essa guarda, não instala paths_existing e não simula fonte administrativa.
Executa GetModuleFileNameW como locator e mantém install_paths.phase0; usa
localizador DATA fixo somente nesta entrada diagnóstica. O ritual de seleção
administrativa P1 não é executado. O percurso de operações do intervalo é
compartilhado, mas processo/token/loader/localização/ambiente/tempo são outros.

A composição é o server completo Selected/Release, mesmo toolchain/CRT e imports
PE estáticos sem diferenças de nomes DLL/APIs contra o baseline774656/981658...0220.
Isso não demonstra equivalência de estado interno, módulos dinamicamente
carregados, token, timing, provider ou histórico125. Delta entre cortes localiza
uma fase nesta observação; não prova leak, ownership de internals ou causalidade
exclusiva. Atividade assíncrona e perturbação do próprio observador são UNKNOWN.
Se apenas surgir saldo interno após cleanup público correto, não fechar handles
Windows arbitrariamente e não iniciar enumeração genérica para justificar closes.

## Custos, gates, Stop e retenção

9*16=144bytes de amostras dentro de estrutura privada784bytes no ABIx64,
incluindo identidade, nome do módulo, drive, digest e status; cobrada à reserva
gerenciada existente do launcher. Não aumenta packet/formato público/persistido.
Sete GetProcessHandleCount adicionais em relação às duas queries do intervalo
operacional, pseudohandle próprio, sem enumeração/abertura de processo/output.
Nenhuma impressão entre cortes. Um novo HANDLE próprio de arquivo no máximo;
token/console/owner e SD/CNG seguem lifetimes existentes. Buffers/heap/CRT/kernel/
provider/cache/tempo têm custos opacos, não teto demonstrado nem zero assumido.
O wrapper retém três FileStreams no processo pai, não subtrai custos do filho;
composição/inheritance/carregamento não são atestados por essa retenção.

Guard124 original antes da primeira abertura; cap128 preservado. Queries extras
também negam >128; cortes1/4 negam >=124 antes da nova aquisição conhecida.
Isso pode interromper a prova mais cedo, nunca permite aquisição adicional.
Corte8>=124 registra final_guard1/1816, não abre decisão. Sem G3READY/capacidade.
Demais valores G3 intactos:1994944bytes de crescimento privado e WS por
participante; físico7239172096bytes;commit9823326208bytes;freshness100ms;
janela240000ms;global60000ms. Não são relaxados/importados como licença da prova;
esta entrada não admite atos operacionais nem renova sessão/lifetime consumido.

Pins/contexto/identidade divergentes, Stop, query desconhecida, reparse,
resultado pending/handle incerto, SD/hash/cleanupfalho: parar na primeira falha,
sem retry, nova conexão, tolerância improvisada ou troca de APIs/providers.
Original do pedido permanece retido até encerramento diagnóstico comprovado.
Fecha somente referências próprias pelo cleanup existente uma vez. Fechamento
incerto mantém owner/recursos em retenção; pode estacionar o processo, sem timeout
que mate/limpe/recupere automaticamente. Não interpretar wrapperClose como nativeClose.
Após falha/UNKNOWN não repetir nem fechar handles internos por conveniência.

## Efeitos e decisão requerida

Autorizar, se aprovado, UMA execução manual prospectiva da prova acima.
Não cria/modifica documentos, arquivos, ACLs, diretórios ou serviço; observação
read-only pode gerar efeitos internos/auditoria/acesso do Windows não limitados
por promessa de zero metadata. Não endpoint/SCQ/Start/Stop/RNG operacional,
UPDATE/rename/rollback/limpeza/CA/CURRENT/rede/TPM/NV/Q4/refund.
Não resolve retenção histórica ou quiescência OS UNKNOWN. Não reaproveita a sessão
P1 consumida, não recria originais, não promove hash/DATA/token a autoridade.

Qualificação offline: quatro builds Selected/Legacy Debug/Release sem warnings;
60 pares CTest PASS; raw-open98casos/758checks (34novos modelados), entrypoint9casos.
APIs-alvo simuladas; nenhum observador nativo executado. Falha de objeto COFF de
teste após reinício regenerada somente em buildLegacyRelease; suíte afetada PASS.
Procedimento PowerShell analisado sintaticamente sem execução da coleta.

Evidência futura: nove linhas com known/count/error/slots, native snapshots,
first/first_cut/identity_checked/final_guard/close_known e resultado do wrapper.
Exit0 isolado não é autoridade, capacidade, causalidade histórica ou readiness.
Caso incompleto: delimitar UNKNOWN; caso completo: comparar apenas cortes da
MESMA execução. Preservar todos os históricos/pins/objetos/PST0.6.4 e limites.
