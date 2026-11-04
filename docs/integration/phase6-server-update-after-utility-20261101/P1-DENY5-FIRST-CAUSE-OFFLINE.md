# Sessão P1 — investigação offline do erro5

2026-11-01. Primeira falha localizada, causa Windows exata UNKNOWN.
main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Sem nova execução/coleta.

## Resultado recebido, sem repetir sessão

Fonte OWNER-MANUAL-P1-SESSION-OUTPUT.txt, preservada byte-exata do anexo,
SHA256dd38d63f304b44ce5b505c23d1a4f793c2b2ce3e4466bbfe1199e0767f1bfaa0.
P1-SESSION-RESULT.json contém a reconciliação. Autorização manual UTILIZADA;
não repetir o driver, recriar originais, reenviar decisão ou rearmar E/Q/S.

O proprietário digitou a linha P1 correta no prompt correto. A execução passou
pela leitura/parse/constructor P1 até a primeira consulta pre-open; logo não
foi erro de colagem/formato da seleção. Dois originais criados e verificados:
request417614100:8978432:356716/pin86303...2a34,
decisão417614100:6815744:363445/pina7d3...8607, created_mask3/verified_mask3,
firstNONE/consumedTrue/close_knownTrue. Esses arquivos devem ser preservados.

O controlador retornou SCQ_COLD_ORIGINAL=DENY/WIN32=5, antes de E/Q/S/Start.
Sua query foi bem-sucedida, own_count113, query_reference0. Guard124 NÃO negou
essa primeira aquisição. Não é baseline universal, pico ou capacidade para
chegar aos readers/peers/runtime. Não atribuir a diferença frente aos136 antigos
unicamente a internals Shell/COM ou prometer margem da nova sessão.

## Primeira causa demonstrável por controle de fluxo

scqc_plan -> scqc_open_raw(owner,0,request_path). Na imagem pinada54d3...4a50:

1. GetDriveType/localpath e contexto precedem a consulta113, portanto passaram.
2. GetProcessHandleCount passou; guard124 teria retornado1816/stage2, não5.
3. GetModuleHandle/GetProcAddress faltantes retornariam50; Stop retornaria1223.
4. NtCreateFile é chamado com origem read-only0x00120089, shareREAD, FILE_OPEN1,
   OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE, options0x00200060 e nome absoluto
   construído como `\??\C:\...\FIRST-P1-SESSION-REQUEST.ORIGINAL.md`.
5. O ramo que rejeita NTSTATUS, IOSB, Information!=FILE_OPENED ou handle ausente
   retornava5 se não marcado unresolved, SEM registrar scqc_first_cause.
6. Falha posterior de tipo/ID retornaria6; SD query teria stage3; SD/hash ou
   allocator falho daria outro código. Abrir decisão/imagem atualizaria
   query_reference para1/2. Não aconteceu na saída.

Assim, sob o caminho qualificado do binário pinado, a negação é do resultado
da PRIMEIRA NtCreateFile do pedido. Não foi guard124 observado, reader quota,
G3/freshness/deadline, comparação de imagem/processo, LookupAccountName, receiver,
endpoint/SCM ou apresentação dos três planos: esses pontos não foram alcançados.
Nenhuma conclusão sobre o status atual do serviço é inferida do NO_SCM_START.

O diagnóstico que preparei estava incompleto: stage0/mapped0 significavam
ausência de registro nesse ramo, não ausência de erro nativo. A causa específica
Windows NÃO foi emitida; é impossível recuperar seu NTSTATUS do erro5 genérico.
Não declarar ACL inadequada, sharing conflitante, reparse ou defeito do Windows.

## Correção técnica comprovada, sem afrouxar abertura

personal_native_win32.h:32bytes privados de snapshot no MESMO owner.
scqc_open_raw agora registra estágio6 e preserva status retornado, IOSB status/
Information inicializados, referência e booleano de handle retornado, ANTES
de cleanup. Emissor SCQ_COLD_NATIVE_OPEN não expõe HANDLE/ponteiro/conteúdo/SD.
GetLastError NÃO é usado como tradução de NTSTATUS. Campos IOSB são snapshots;
zeros podem ser inicialização quando API falha, não prova de escrita/sucesso.
Flag attempted distingue ausência de chamada de NTSTATUS0.

Mesmo retorno5/ERROR_IO_INCOMPLETE, mesmas condições de retenção, direitos,
share/flags/caminho, guard124/cap128/G3/consumo/Stop. Não remover OBJ_DONT_REPARSE,
acrescentar shareWRITE/DELETE, elevar acesso, fechar referências ou dar fallback.
Primeiro erro registrado permanece imutável diante de falha posterior.
Não alterar formatos públicos/SCQ1/SQB1/SQB2/receipts/close ou accepted.

## Hipóteses examinadas, sem falsa atribuição

- ACL/direitos: não demonstrado; materialização e metadata do wrapper passaram,
  mas isso não substitui o resultado NtCreateFile sob o contexto do controlador.
- Sharing/antivírus/filtro/referência externa: possível, não evidenciado; não
  atribuir culpa ou fechar handles externos.
- Parsing de caminho `\??\C:` versus nome de dispositivo e OBJ_DONT_REPARSE:
  hipótese focal porque o updater existente resolve QueryDosDevice antes da
  abertura. Essa diferença NÃO prova o status deste evento nem justifica trocar
  a API/flag com objetivo de PASS. Não repetir pesquisa de known folders.
- Sintaxe/selector P1: excluída como primeira causa pelo progresso até raw0.

Fontes primárias: [NtCreateFile](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile)
documenta o retorno NTSTATUS, IOSB, compatibilidade de sharing e FILE_OPEN.
[QueryDosDeviceW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-querydosdevicew)
descreve mapping/junctions do namespace DOS; consulta/mapping isolado não é
prova de objeto ou continuidade. Não foi encontrada fonte primária suficiente
para atribuir a falha histórica especificamente à interação DOS/OBJ_DONT_REPARSE.

## Qualificação offline

Quatro builds Selected/Legacy Debug/Release PASS, /W4 /WX, zero warnings finais.
15 CTests/configuração,60 pares PASS; logs p1-deny5-<perfil>-<config>-build/tests.log.
COLD_RAW_OPEN52cenários/189checks. Cinco novos casos com count113/query0 modelam
NT access denied, sharing, reparse e resultados IOSB/handle inconsistentes que
colapsavam em5; agora distinguem snapshots/estágio6 e preservam primeira falha.
Pending/handle inesperado, guard123/124/128, query falha, SD, Stop/retention e
source/perfil/replay/consumo/close regressões preservados. Todas as aberturas
NT/SCM/endpoint/RNG protegidas nesses testes são modeladas, sem nova coleta real.

Censo Selected2002032/Legacy1833464: +32bytes frente a P1, sem aumentar quotas
ou alegar privado/WS/margem host. Artifact offline Selected/Release773632bytes,
SHA25655c3d770a765c60843fc2df85dcdca040c1c0d72e2ff8f71864e79fe2ee02d56.
NÃO selecionado/instalado. Imagem instalada continua a seleção772608/54d3...4a50
do UPDATE reconciliado; não foi alterada nem consultada novamente nesta tarefa.
O artefato novo não altera o passado nem pode substituir pins de pedido consumido.

## Primeira lacuna e próximo gate

Falta EXATAMENTE o NTSTATUS/IOSB da abertura raw0 sob contexto nativo relevante.
O log antigo não os contém; não mascarar UNKNOWN com um teste injetado.
Uma prova futura deve limitar-se a essa abertura read-only e conferir contexto/
objeto/flags, com autorização própria, sem E/Q/S, Start ou recriar documentos.
Se depender de disponibilizar diagnóstico no processo original, usar somente
UPDATE governado existente e autorização específica; não instalar por esta task.
Não concluir que probe em outro processo/token equivale à tentativa histórica.
Se não reproduzir, a causa histórica permanece UNKNOWN.

Próximo NEXT-P1-RAW-OPEN-STATUS-GATE.md delimita essa única prova, não campanha
genérica de handles/known folders/tracing ou recomendação de nova sessão.
Sem retry, UPDATE, coleta, alteração ACL/SCM, Start/Stop/endpoint/SCQ, limpeza,
refund/recuperação nesta investigação. Retenção histórica/OS UNKNOWN. Close do
par documental conhecido não prova close de outra coorte; retorno ao prompt
não é marcador universal de fechamento do controlador. Preservar todos os
originais/EXEs/pins/decisões consumidas/HEAD/WIP/reservas/PST0.6.4.
