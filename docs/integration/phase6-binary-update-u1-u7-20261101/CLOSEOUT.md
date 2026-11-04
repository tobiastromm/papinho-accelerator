# U1–U7 — UPDATE binário integrado e qualificado offline

2026-11-01; main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
Estado: IMPLEMENTED_AND_QUALIFIED_OFFLINE. Execução operacional NOT_EXECUTED;
autoridade atual DENY. HEAD/WIP, históricos, pins e atos consumidos preservados.

## Aceitação exata

Conferido SHA2569fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234
do candidato original imutável. O proprietário aprovou B/U1–U7 explicitamente.
[Decisão accepted](../../adr/DECISION-personal-binary-update-u1-u7-20261101.md)
registrada antes do código dependente. Não há redução de proteção, quota,
identidade, consumo, Stop/retidos ou readback. Somente a garantia opaca U4 é
especializada para este ato. Não se importam B3, R3 ou G3.

## Código e callers efetivos

`main.c.api3.inc` reconhece exclusivamente --owner-manual-binary-update e
encaminha ao owner existente. LAB rejeita; argumentos inválidos negam sem
fallback para iniciar o runtime. `papacc_server_launcher_binary_update_cli`
aceita source, seis componentes decimais de IDs de parent/EXE e quatro pins
canônicos como DATA, não como autorização. Não existe setter/grant positivo.

`server_launcher_staged_update_win32.inc` reutiliza captura original, root,
token/contexto, paths BINARY_ONLY, slots e fechamento do packet. O wrapper
read-only anterior continua sem mutação/consumo, missing15. O novo caller
privado constrói fonte atribuída nativa em `...staged_update_effects_win32.inc`:

1. Captura/revalida identidade nativa/admin/token, parent histórico e EXE antigo,
   ancestrais sem reparse, owner/DACL/proteção, fonte, DLLs e hashes. Reter
   referências, rejeitar alias e substituição. Source usa DACL efetiva; não
   se presume que seja objeto criado pelo stager. O EXE antigo e parent devem
   também corresponder aos IDs selecionados, não somente aos bytes.
2. Abre somente SCM_CONNECT/QUERY_CONFIG/QUERY_STATUS; confere serviço, conta,
   comando, tipo/start/SID, STOPPED/PID0 e configuração inalterada. Nenhuma
   chamada SCM mutante, Start/Stop ou DeleteService foi adicionada.
3. Instala o bridge de console F1–F6 existente. Callback só sinaliza Stop. Sem
   registro conhecido do bridge, nega. Após retenção não o remove como cleanup;
   após fechamento conhecido, unregister. Falha de unregister retém o cohort.
4. Gera tentativa32bytes no CSPRNG existente, congela nomes/plano/referências e
   seu digest transitório, mostra identidade/IDs/pins/bytes/comando/nomes/K/R,
   riscos e efeitos. Recebe declaração/confirmacão OWNER_BINARY_UPDATE exata.
   Nonce, token, imagem, CLI e elevação isolados nunca autorizam o UPDATE.
5. Revalida origem/plano e amostra U4; consome uma única vez antes da primeira
   submissão de criação. Falha não rearma. Cria UM candidato FILE_CREATE
   exclusivo, protegido, copia tamanho congelado com buffer4096 reutilizado,
   aceita I/O parcial sem repetir bytes, verifica hash/size/SD e flush.
6. NtSetInformationFile FileRenameInformation(10), ReplaceIfExists FALSE:
   primeiro o handle antigo para .previous, depois o candidato para o nome
   canônico. Dois renames no máximo, nenhum overwrite/Delete/rollback.
   Readbacks relativos ao parent retido conferem objetos/bytes/SD de ambos.
7. Confere serviço/configuração ainda fechados, fecha uma vez cada referência
   se factual, emite resultado e o file ID novo observado. Qualquer falha após
   submissão ou fechamento incerto retém/UNKNOWN. Status nativo pending retém
   IOSB, nomes e backing no mesmo packet; não espera timeout para liberar.

Direitos: raiz de volume apenas leitura/travessia no contexto UPDATE próprio;
demais perfis conservam sua aquisição original. Parent do alvo com ADD_FILE;
antigo read/DELETE; candidato read/write/DELETE; fonte/DLLs read. Não há
WRITE_DAC/WRITE_OWNER em objetos existentes. Readbacks só leem e compartilham
com os handles primários do próprio owner: a partilha READ dos primários
continua impedindo outros writers/deleters. Não é um bypass global de proteção.

O plano é privado, de mesma invocação e sem bytes persistidos/públicos novos.
Seu digest cobre tentativa, política exata, K/geometria, IDs, pins, paths/nomes,
token e referências originais. Não serializa handles como referências remotas.
Custódia não é reconstruída por esse digest após restart.

## Recursos demonstrados e opacos

| Perfil | ROOT | KNOWN estrutural | K do caminho | Limite gerido |
| --- | ---: | ---: | ---: | ---: |
| Selected |373752|2000800|2025386|3145728|
| Legacy |205184|1832232|1856818|2097152|

Incremento estrutural +5584 bytes sobre a preparação anterior. K usa o censo
real do serviço/bridge e frames/buffers/caller nomeados conservadores. O mesmo
getter somente de sizeof alimenta a reserva do caminho e o teste de censo
ligado à implementação real; não é setter nem prova de capacidade do host.
O censo mantém builds sem serviço/sem esse target independentes do getter.

O gate U4 coleta handles totais (inclusive herdados), privado/working set,
física/commit e relógio; física/commit >=K antes de cada mutação. Não inventa
ceiling de handles nem usa o guard124 do sampler legado. Este continua
intacto em seus domínios. Readbacks também são referências contabilizadas.
R depende da geometria factual congelada/revalidada e do novo tamanho; free
caller/actual >=R antes da criação, payload restante arredondado antes de
escritas. API/proc e parâmetros são preparados antes do último gate de efeito.

Custos de alocação Windows/descritores/CRT/loader/hash/provider/RPC/filesystem,
cache/metadados/flush/rename/kernel, cancelamento/latência e retenções internas
continuam opacos. Tamanho validado de descritor não limita a alocação nativa
anterior à validação. Somar K não limita esses custos e observar margem não
reserva o host. Não há coleta operacional nem medição de pico nesta task.
missing2 (native opaque) permanece inclusive no sucesso limitado; não foi
reescrito como provisão integral ou ED11 PASS. W1–W3/W5 e quotas inalterados.

## Qualificação e seus limites

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings. Treze CTests
afetados por configuração (52 pares distintos) PASS. O modelo de observação
que inclui o censo também foi recompilado/testado nas quatro configurações
(+4 pares distintos: total56). Ver logs `*-build/tests/details.log`,
`*-observer-*` e `*-census-final-*`. Reexecuções de desenvolvimento/expansão
não são contadas como requisitos adicionais.

UPDATE: 77 cenários e382 verificações por configuração:33 de preparação e44
de efeito/fonte. Caminhos positivos percorrem o ato real e o caller CLI sobre
APIs modeladas; não são apenas um receipt DATA. Testam originais/pins/parent
divergentes, serviço não parado, nome exclusivo existente, copy/flush, erros
antes/entre/depois dos renames, pending, readbacks errados, Stop, replay/take
duplicado, recursos/volume desconhecidos ou insuficientes, alteração do plano,
token/proteção e close/unregister incertos. I/O parcial positivo preserva bytes;
as falhas retêm sem rename inverso ou escrita no antigo. Dois renames sempre
sem ReplaceIfExists; colisão no nome retido/canônico nega sem adoção.

Controle de console, SCM, RNG e filesystem mutante são simulados nesses testes.
Somente parsing/crypto de hash/censos sizeof e montagem local dos fixtures são
reais. Nenhum SetConsoleCtrlHandler do novo fluxo, SCM/query operacional,
NtCreate/NtSet/Write/Flush real do destino, Start/Stop, endpoint ou KSP foi
executado. Não existe prova bilateral nativa nova. OS_QUIESCENCE UNKNOWN.

## Artefato estabilizado e pedido operacional

Selected/Release final:737280bytes,
42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca.
Preservado por hash em build/private-bootstrap-preserved-20261031. Snapshot
pré-U1–U7 c1f52f…b6c6 preservado; snapshots intermediários não são seleção.

Staged histórico continua571392bytes/c0f27f…806ecf, ID417614100:6225920:71237;
parent417614100:8257536:71209. Não foi relido/alterado operacionalmente aqui.
Delta EXE +165888 bytes, não medida de memória. DLLs permanecem idênticas:
ssl1321472/3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
crypto7217664/09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Nenhuma cópia de DLL ou alteração de serviço é necessária/permitida.

[Pedido byte-exato](ONE-BINARY-UPDATE-REQUEST.CANDIDATE.md), SHA256
baa501ad97fb1c5cbf65c96d2ecfc338ea0f7c55a872ad0443b74580f08f4726,
READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION, NÃO AUTORIZADO.
Procedimento manual candidato85531965578ba126200bfab6d2168226dfdaadd9c4e1b96135411d01d1e6ac55,
somente AST validado, nunca executado. Não foi emitido comando de efeito.

Primeiro gate restante: aprovação operacional específica desse pedido/hash,
incluindo seleção do artefato e risco U4. IDs atuais/refs, token, recursos,
geometria e disposição são gates frescos DENTRO dessa única sessão autorizada,
não exigência de outra coleta antecipada. Sem essa aprovação, DENY.

Após execução manual e reconciliação positiva limitada: selecionar o novo file
ID factual e reacquirir originais, preparar endpoint/Start diagnóstico com seus
três atos e G3 independentes. Nenhum dado deste UPDATE restaura esses grants.
Não executar Start/Stop, CA/CURRENT ou outras etapas automaticamente.

E21 PENDING; ED11/Phase6 PARTIAL; autoridade produtiva DENY. Backup/recuperação
do future pessoal continuam deferred; reter EXE não implementa recuperação da
CA/identidade. PST0.6.4, quotas, fixture, UNKNOWN e reservas preservados.
[Próximo prompt](NEXT-ONE-BINARY-UPDATE-AUTHORIZATION.md).
