# Pedido — uma observação manual da disposição UPDATE

Status READY_FOR_SPECIFIC_MANUAL_AUTHORIZATION / NÃO AUTORIZADO.
Data2026-11-01. Não executar automaticamente. Uma única invocação manual,
sem retry, após aprovação byte-exata. Não é recuperação, UPDATE ou novo grant.

## Finalidade e origem

Reconciliar exclusivamente a disposição persistente da tentativa consumida
UPDATE cujo first0xC0000043/location701, created1/consumed1/renames0/close_known0.
Fonte histórica: OWNER-MANUAL-UPDATE-OUTPUT.txt em
phase6-corrected-update-executor-20261101, SHA256
4a7e4dbc5e6022a1c3978c89f6aaa14c667258b1c2ec395085107928c2f9dfd4.
U1–U7/U2 exigem identidade/proteção/readback/retenção; não autorizam retry de
ato consumido. Esta leitura não altera as decisões ou estados históricos.

Operador esperado S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS, Windows PowerShell5.1 Desktop x64 já elevado. Metadados/token
não criam autoridade de UPDATE. Contexto divergente nega; sem elevação automática.
Known folder ProgramFiles deve resolver C:\Program Files. Sem qualquer diretório
de distribuição, novo serviço, owner, root, ledger, grants ou canal.

## Escopo fechado

Adquirir root C:\ e cadeia existente Program Files\Papinho\Accelerator;
parent esperado417614100:8257536:71209. Parar se aquisição/ID/reparse divergir.
Somente quatro arquivos nesse parent:

| Leaf | Expectativa histórica, NÃO presença atual presumida |
|---|---|
| papacc_server.exe |417614100:6225920:71237;571392bytes;c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf|
| papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.candidate |417614100:1769472:537456;758272bytes;7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f|
| papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.previous |planejado, nenhum rename confirmado; consultar somente presença/ausência e metadados se presente|
| papacc_update.exe |417614100:1507328:540527;104448bytes;6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87|

Não enumerar arquivos/subdiretórios, DLLs, outros processos ou tabela de handles.
Não abrir candidato/previous para executar, completar, adotar ou excluir.
Presença/ausência inesperada é dado factual para reconciliação, não permissão.

Somente consultas SCM locais sc.exe qc/queryex/qsidtype PapinhoAccelerator,
uma de cada, sem modificar serviço, abrir endpoint ou iniciar runtime. Capturar
configuração/conta/comando/tipos/SID type e STOPPED/PID com códigos de retorno.
Saída localizada é evidência snapshot; não prova continuidade de serviço.

## Procedimento e direitos

observe-disposition.MANUAL.CANDIDATE.ps1, SHA256
46b0e72f909d09d93a3f9f921d67900dcff8a02667a2cab93e407957f7abd96e.
Exige hashes explícitos do próprio script e deste pedido antes da consulta.
Reutiliza helpers de originais com interop emitida somente em memória; não
compila DLL/helper nem grava sink/log operacional. Nenhum binário de produto
é executado. Console/stdout é a única saída solicitada, a enviar integralmente.

CreateFileW OPEN_EXISTING(3), OPEN_REPARSE_POINT/BACKUP_SEMANTICS, share READ
somente. Diretórios READ_CONTROL/READ_ATTRIBUTES/TRAVERSE/SYNCHRONIZE0x1200a0;
arquivos READ_CONTROL/READ_ATTRIBUTES/READ_DATA/SYNCHRONIZE0x120081.
Nunca WRITE/DELETE/WRITE_DAC/WRITE_OWNER. Capturar ID/volume/tamanho/atributos,
owner SID, proteção, ACEs/flags/direitos, bytes/digest de SD e SHA256 por handle
retido; revalidar identidade/continuidade antes/depois e fechar originais uma vez.

SetLastError da interop é capturado para distinguir resultado de abertura.
Apenas ERROR_FILE_NOT_FOUND2 com parent já adquirido vira ABSENT_SNAPSHOT.
ERROR_PATH_NOT_FOUND3, ACCESS_DENIED5, SHARING_VIOLATION32 e demais falhas são
UNKNOWN, nunca ausência/zero. Sucesso ignora last-error stale. Reparse/tipo
incorreto/SD não obtido/continuidade quebrada impede prosseguir. Não trocar
share flags, ACL, privilégio, nome, timeout ou processo para contornar falha.

Se algum arquivo resultar UNKNOWN, manter resultados parciais, parar consultas
subsequentes e fechar somente as referências próprias quando conhecido. Não
esperar/repetir nem fechar handles de terceiros. Close incerto mantém UNKNOWN
e não tenta novamente ou presume quiescência pela saída do processo.

## Garantia, efeitos e riscos

Somente leitura de conteúdo necessário ao hash e metadados; não expor conteúdo
bruto/segredos. Há custos transitórios de PowerShell/CLR/interop/hash/ACL/SCM e
I/O de leitura, sem garantia de pico, latência ou reserva. O método aceita falha
e emite UNKNOWN; não é admissão de capacidade para mutação. Não inventa quotas
nem transporta B/R/G ou picos de staging para esta consulta.

Acesso pode falhar por cohort retido/antivírus/terceiros sem atribuição causal.
Hash/ACL/ausência são snapshots sob refs retidas, não atestação universal de
páginas, durabilidade ou defesa contra administrador/kernel comprometido.
Efeitos incidentais OS/cache/last-access não são prometidos como zero; nenhum
efeito persistente de aplicação é solicitado. Não criar/copiar/renomear/apagar
arquivos, editar ACL/SCM, atualizar/pôr em produção ou liberar crédito.

No procedimento futuro, Bypass pode ser usado exclusivamente no processo filho
Windows PowerShell5.1x64 -NoProfile para este script, sem mudar CurrentUser/
LocalMachine/GPO/ACL ou contornar política superior. Bloqueio exige STOP.
Não emitir comando operacional nesta fase de preparação não autorizada.

## Consumo, STOP e reconciliação

Uma autorização própria futura, usada ao iniciar a invocação manual mesmo se
submissão=False/exit2. Autorizações UPDATE/preparação anteriores permanecem
utilizadas. Nenhum retry, resposta automática ou inferência de aprovação.

Após a única saída, conferir pins/contexto, primeira falha, registros PRESENT/
ABSENT_SNAPSHOT/UNKNOWN, IDs/tamanhos/hash/SD/continuidade e status SCM; manter
objeto divergente como evidência, não adotá-lo. query_close_known refere-se
APENAS à consulta, jamais ao antigo owner. historical_owner_close UNKNOWN
permanece explícito; leitura/exit0 não resolve crédito/quiescência universal.

STOP em divergência de pedido/script/contexto/known folder/parent/reparse,
erro de abertura/query/SD/hash ou perda de continuidade/close. Não executar
utilitário, UPDATE, retry, recuperação, exclusão, restauração, limpeza/refund,
Start/Stop, endpoint, SCM/ACL mutante, CA/CURRENT, rede operacional ou TPM/NV/Q4.

Procedimento qualificado somente offline:21 checks WinPS5.1x64 sobre workspace
com funções isoladas (erros2/3/5/32, refs/hash/SD/continuidade/close e escopo).
Consulta completa/target/SCM NÃO executados nesta tarefa. Necessita aprovação
específica e ação manual. E21 PENDING; ED11/Phase6 PARTIAL; produção DENY;
OS_QUIESCENCE UNKNOWN. Preservar objetos/EXE anterior/históricos/pins/reservas/
autorizações utilizadas/HEAD/WIP e PST0.6.4.
