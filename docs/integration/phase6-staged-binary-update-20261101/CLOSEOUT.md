# UPDATE governado do EXE staged — preparação integrada, admissão DENY

Atualização posterior: U1–U7 foi explicitamente aprovada, registrada e integrada
offline. Ver [fechamento sucessor](../phase6-binary-update-u1-u7-20261101/CLOSEOUT.md).
O estado abaixo preserva a preparação anterior e seu gate normativo histórico;
a proposta original não foi alterada. Autorização operacional continua pendente.

2026-11-01. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
HEAD/WIP e todos os históricos/pins/atos consumidos preservados. Nenhum efeito
operacional executado, inclusive consultas SCM/filesystem do novo procedimento.

## Resultado e callers reais

Integração offline concluída **para preparação nativa somente leitura**.
A substituição positiva não está implementada/qualificada. Não se declara a
task operacionalmente pronta por passagem de builds.

O entry privado `papacc_server_launcher_staged_update_prepare` está no owner
canônico, incluído em `server_launcher_owner_win32.c` por
`server_launcher_staged_update_win32.inc`. Usa o packet pessoal existente e os
16 slots/buffer já censados do stager como custódia exclusiva da invocação;
não altera FILE_CREATE do stager nem transforma sua confirmação em UPDATE.
Fora do perfil compilado aplicável, retorna negação. Não há novo dispatcher CLI
ou caller operacional; o caller executado é o teste com APIs nativas modeladas.

O procedimento implementado:

1. Obtém a exclusividade do root existente; captura thread/invocação e congela
   os pins e volume/file ID histórico selecionado do EXE anterior como DATA.
2. Resolve known folder pelo mecanismo existente, captura token/admin e raiz,
   abre cada ancestral relativo ao original retido, sem reparse, mantendo a
   correção anchor/child. Não adota diretório ausente nem cria filhos.
3. Adquire EXE staged e DLLs originais relativos ao destino protegido e o novo
   EXE por NT FILE_OPEN somente leitura, sem WRITE/DELETE/ACL. Verifica owner,
   DACL, proteção dos arquivos staged (fonte com DACL efetiva, sem exigir que
   tenha sido criada pelo stager), identidade/continuidade, tamanho não vazio,
   SHA256 e ausência de alias entre as quatro referências. O EXE antigo deve
   corresponder também ao ID selecionado, não apenas ao mesmo hash.
4. Obtém somente SC_MANAGER_CONNECT e SERVICE_QUERY_CONFIG/QUERY_STATUS.
   Confere comando/destino exatos, conta virtual, OWN_PROCESS, DEMAND_START,
   ERROR_NORMAL, SID UNRESTRICTED, SERVICE_STOPPED e PID zero. Strings retornadas
   devem estar terminadas dentro do buffer; tamanho desconhecido nega, sem
   realloc/retry. Conteúdo/identidade são revalidados depois da consulta SCM.
5. Fecha uma vez cada contexto/referência, ou mantém RETAINED_UNKNOWN e impede
   reutilização. O close genérico agora rejeita UPDATE vivo/retido. Nenhuma
   confirmação é capturada nem consumida; efeitos permanecem zero.

O retorno zero significa apenas preparação/snapshot e fechamento controlado
conhecidos. `missing=15` continua explícito: ato próprio, suficiência nativa,
margem e protocolo de substituição não demonstrados. Nenhum getter de Start ou
QUERY foi relaxado. A captura não é fencing contra outro administrador e não
prova quiescência OS ou imagem mapeada.

## Primeira lacuna concreta e fundamento

**Falta o contrato específico de BINARY_UPDATE_CLOSED e sua garantia aplicável.**
Não há conflito literal entre accepteds: seus escopos são diferentes. A ausência
de cobertura não pode ser resolvida por promover o ato NEW_INSTALL ou UPDATE v3.

- F0 distingue NEW_INSTALL de UPDATE; F1/F3/F5 fixam confirmação/consumo para o
  propósito exato. Não fornecem um grant genérico de substituição.
- U0–U7 de `DECISION-v3-update-approval-u0-u7-u6-correction-20261021.md`, seção
  “Precedência delimitada”, restringem UPDATE à base v2/CP2 ou v3/CP3/CURRENT e
  tuplas de configuração. PC_ROOT8 não é EXE Windows.
- F8 e ED11 incremental, parágrafo “Admission MUST consider”, conservam DENY
  para evidência material requerida desconhecida. W4 não considera custo opaco
  zero. Uma observação de pico não demonstra suficiência integral.
- B2 da decisão B0–B8 só especializa B0/B1; R2 só SCM_REGISTER_CLOSED; G0–G9 só
  Start. Nenhuma dessas aceita a exceção para UPDATE binário.

É possível continuar a preparação estrita sem fabricar autoridade, como o código
demonstra. Um caminho positivo dependente de fonte/garantia não aceita seria
uma ampliação normativa, não correção técnica ordinária de QUERY/SQB2.

Uma única [proposta focal](UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md) delimita a
decisão necessária, sem nova ADR ou registro ACCEPTED. Alternativa A mantém a
garantia integral/DENY corrente. Recomenda-se B, ensaio independente com original
antigo retido e custos opacos explicitamente reconhecidos, sujeito a aceitação
e posterior implementação/qualificação. A proposta não autoriza operação.

## Recursos e evidências

Novo estado privado: 224 bytes, no mesmo packet, sem pool ou ledger adicional.
Censo compilado Selected: ROOT 368.168, KNOWN 1.995.216 / Q 3.145.728 bytes.
Legacy: ROOT 199.600, KNOWN 1.826.648 / Q 2.097.152. Incremento sobre o fechamento
pré-Start: +224 bytes em ambos. São somas estruturais, não pico nativo ou RAM.

Preparação reutiliza buffer de 4.096 bytes e vetor de 16 referências; adquire
até oito ancestrais, quatro arquivos originais, raiz/token já governados e dois
contextos SCM de consulta. Não usa SC_HANDLE como contagem de handle kernel.
A reserva adicional no código soma frames/buffers/pins/paths simultâneos do
caller e hash, sem retirar charges opacos. Não há cópia de payload, espaço
temporário ou arquivo novo **nesta preparação**. O protocolo de substituição
futuro precisará censar o novo arquivo/retidos e geometria/free space do volume.

Continuam UNKNOWN: custos transitivos de CRT/alocador/descritores, RPC/SCM,
filesystem/cache/metadados/flush/rename/kernel, suficiência de margem e duração,
disposição de uma substituição parcial e quiescência OS. Nenhuma coleta foi
repetida. Quotas 124/128/1024 não foram alteradas nem importadas como política
de UPDATE. B3 e G3 permanecem inalterados em seus próprios domínios.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings. Treze CTests
afetados por configuração, 52 pares distintos PASS; logs finais principais
(dez por configuração) e de domínio (quatro, com um teste compartilhado) registram
56 execuções. Iterações anteriores de desenvolvimento não são novos requisitos.

O componente novo tem 33 cenários/81 verificações por configuração: snapshot
parado, serviço em execução/PID, comando/conta/start/SID divergentes, consulta
incompleta/ponteiros inválidos, EXE/pins/DLL divergentes, owner/DACL/proteção,
reparse, identidade substituída antes/durante aquisição, token/continuidade,
Stop, fonte pending, concorrência/reentrada e fechamento de arquivo/SCM incerto.
Todos verificam zero criações, escritas, flush, RNG, consumo e efeitos. Guardas
de close ativo/retido também foram exercitadas na biblioteca real.

APIs mutantes e consultas nativas são simuladas nos testes. Substituição parcial,
readback pós-substituição e rollback não foram exercitados como UPDATE: o writer
ainda não existe, e não se apresenta sua ausência como prova de protocolo
qualificado. Não há rota de rollback/cleanup automático na preparação.

## Artefato e caminho mínimo

Staged histórico intacto: 571.392 bytes,
c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf,
volume417614100/high6225920/low71237. Não foi reinspecionado operacionalmente
nesta task; estado SCM atual permanece não observado nesta execução offline.

Selected/Release de workspace após esta qualificação: 710.656 bytes,
c1f52f79c9e389ad55afe60fec5c41addb881f2209b66e11eb654bda23e0b6c6.
Delta de tamanho +139.264 bytes, não custo de execução. DLLs permanecem
1321472/7217664 bytes e pins ssl3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed /
crypto09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
O EXE pré-task 08ac…c043 foi preservado no diretório de snapshots existente.

O candidato atual NÃO é congelamento operacional final: faltam ato/mutação e
admissão próprios. Não se solicitou seleção de um binário que ainda mudará.
Pedido operacional de update NOT_READY; não foi emitido comando ou aprovação.

Sequência mínima: decisão focal do contrato/garantia; concluir writer e ato no
mesmo owner, qualificar falhas parciais e recursos; congelar EXE final; pedido
byte-exato de uma atualização; autorização própria e reconciliação manual;
somente depois pedido independente de endpoint/Start diagnóstico. Não repetir
staging, CreateService, QUERY/G3/SQB2 ou confirmações já aceitas.

Autoridade DENY; E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN.
PST0.6.4, reservas, fixture e todos os históricos preservados.
[Próximo prompt](NEXT-STAGED-UPDATE-CONTRACT-DECISION.md).
