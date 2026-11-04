# Pedido operacional — uma manutenção UTILITY_UPDATE_CLOSED

Status READY_FOR_OWNER_SELECTION_AND_SPECIFIC_MANUAL_AUTHORIZATION / NÃO AUTORIZADO.
Data2026-11-01. UMA invocação manual futura, sem retry. Não executar agora.
Aceitação normativa não é aprovação desta operação nem seleção dos artefatos.

## Norma, propósito e evidência anterior

DECISION-personal-utility-replacement-20261101 incorpora original byte-exato
00d0cd8dca8ebea3d221a5e0d9b09281ab42a300b93d3dbd52a294ca804003b6.
Somente PERSONAL_LOCAL e UM utilitário no owner existente. Não reutiliza
autorizações de preparação/UPDATE/read-only consumidas ou confirmações antigas.
Não é UPDATE do servidor, retry, recuperação, self-update ou nova distribuição.

Consulta manual reconciliada em ../phase6-update-disposition-utility-delta-20261101/
DISPOSITION-RECONCILIATION.md. Evidência integral SHA256
e6d62172cb0f65813307b4a3a1c7132c525d8e3d05b27da06d81ce0b52bc14c9.
Snapshot confirma canônico antigo intacto, candidato742d8325…0f10b persistente,
previous do servidor ausente e utilitário antigo presente. Query close conhecido
NÃO é fechamento do owner histórico: retention/cohort/créditos e quiescência OS
permanecem UNKNOWN. Este ato não os libera/adota/reutiliza ou os transforma em PASS.
Nenhum retry de UPDATE servidor é autorizado por esta manutenção.

## Ator, parent e objetos intocados

Operador selecionável S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS, WinPS5.1 Desktop x64 já elevado, sem UAC automático.
São comparações DATA, não prova de propriedade/autoridade. Exigir contexto/token
originais elegíveis, declaração e confirmação atribuída nativa específica.

Known folder ProgramFiles deve resolver C:\Program Files e ser congelada.
Parent EXISTENTE C:\Program Files\Papinho\Accelerator,
volume417614100/high8257536/low71209. Reter cadeia/parent originais, owner/DACL
efetiva/identidade/reparse/continuidade estritos. Não criar/adotar diretórios.

Alvo antigo SOMENTE C:\Program Files\Papinho\Accelerator\papacc_update.exe,
volume417614100/high1507328/low540527,104448bytes, SHA256
6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87.
Observado owner BA, SD protegido AU RX, SY/BA full; revalidar fresco. Hash e
nome não concedem autoridade. Referência antiga deve sobreviver até disposição.

Não tocar nos três nomes do servidor: papacc_server.exe e candidate/previous da
tentativa742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.
Não executar/completar/adotar/apagar candidato anterior. Canônico antigo observado
417614100:6225920:71237/571392bytes/c0f27f94…806ecf permanece fora do alvo.
Não alterar DLLs, SCM/ACL/configuração, ProgramData, CA/CURRENT ou dados.

## Artefatos finais para seleção conjunta

Fonte fixa C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release.

| Papel | Arquivo | Bytes | SHA256 |
|---|---|---:|---|
| Controller de bootstrap |papacc_server.exe|762880|3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b|
| Candidato somente DATA |papacc_update.exe|104448|084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a|
| Dependência do controller |libssl-3-x64.dll|1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| Dependência do controller |libcrypto-3-x64.dll|7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Manifesto FINAL-ARTIFACTS.json SHA256
b3903c289bd2fadd290c70478d273b67f9f51e441ffd1760a8773d668135c43a.
Proveniência: main/HEADf341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado,
norma accepted e qualificação offline desta pasta. Controller e DATA diferem dos
pedidos anteriores; sua nova seleção NÃO é inferida. Não substituir por build
posterior sem conferir/selecionar novamente. Não executar o utilitário DATA ou
utilitário antigo para manter a si próprio. Nenhum outro binário é instalado.

## Bootstrap e aquisição de originais

Aceitar explicitamente apenas neste ato o controller server ainda no workspace,
conforme norma00d0cd8d. Não é prova de que a ACL do workspace satisfaz U2 do
UPDATE servidor, nem exceção transportável. Referências de cadeia/controller/
DATA/DLLs, pins, identidade/processo/token/PID/criação/módulos e proveniência são
adquiridas/revalidadas. Nenhuma autoridade deriva de dados/hash ou fixture.
Não transmite tokens/handles/ponteiros por argv; não cria grant/ledger/canal.

Wrapper adquire cadeias/arquivos e mantém arquivos e fonte sem share WRITE/DELETE;
somente parent protegido de criação permite WRITE sharing para FILE_ADD_FILE,
nunca DELETE. Faz preflight do utilitário antigo e FECHA apenas essa referência
do wrapper antes da aquisição DELETE pelo owner. Não manter lock incompatível
nem ampliar compartilhamento para forçar rename. Owner readquire antigo por
parent original/ID/hash/SD, retendo-o até fechamento/retention. Não alegar
continuidade do antigo através do intervalo entre os dois opens; o gate nativo
é obrigatório. Outros originais permanecem retidos durante a chamada.

## Plano, efeito e sequência única

Uma chamada ao controller existente --owner-manual-update-utility,13 argumentos
mais argv0: caminho utility DATA, IDs3 do parent e antigo, pins antigo/novo/
DLL2/controller. Não contém PID/token futuro ou resposta automática.
No owner: contexto/serviço originais, tentativa CSPRNG32bytes, plano imutável,
apresentação de IDs/SD/pins/custos/efeitos/riscos e declaração/confirmacão própria
OWNER_UTILITY_UPDATE seguida do nonce mostrado. Não confirmar outro propósito.

Dois leaves fechados derivados da tentativa original são mostrados no plano,
ANTES de qualquer efeito, no mesmo parent:
- papacc_update. +64 hex minúsculos da tentativa + .candidate;
- papacc_update. +os MESMOS64 hex + .previous.
São regras de binding tardio, não placeholders de comando. Não inventar nonce
ou ID do novo filho. Colisão nega, sem outro leaf, overwrite ou adoção.

Depois de revalidar plano/ator/refs/SCM/recursos/Stop, consumir irrevogavelmente
antes dos efeitos. Permitir exclusivamente:
1. Uma FILE_CREATE relativa exclusiva de candidato protegido BA/SY full, AU RX.
   Não editar descritor/ACL preexistente. Copiar104448bytes por DATA original,
   com buffer limitado/I/O parcial governado, hash independente, flush/verificação.
2. Primeiro rename por handle antigo para previous exclusivo; RootDirectory NULL,
   leaf simples, same-directory e ReplaceIfExists FALSE. Não é overwrite direto.
   Readback por parent original/ID/hash/SD. Original antigo permanece retido.
3. Fechar somente referência temporária de readback conhecida; reutilizar slot
   bounded apenas depois de sucesso. Se close incerto, reter cohort e parar.
4. Segundo rename por handle do candidato para papacc_update.exe, mesmo formato
   sem overwrite; readback canônico/ID/bytes/hash/SD e SCM ainda parado.
5. Fechamento governado conhecido, ou retenção UNKNOWN. Não executar o utilitário.

Antigo permanece como objeto .previous. Sem delete, restauração, rollback,
cleanup/refund ou completar automaticamente passo falho. Pode faltar o nome
canônico do utilitário entre renames ou após falha parcial. Arquivos do servidor,
tentativa anterior e DLLs não são alvos dessas chamadas.

## SCM, recursos e garantia limitada própria

Queries apenas: PapinhoAccelerator WIN32_OWN_PROCESS, DEMAND_START3,
ERROR_NORMAL1, NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED1,
comando "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed,
SERVICE_STOPPED/PID0. Snapshot read-only anterior coincide, mas deve ser
revalidado nativamente antes de efeitos e no readback. Não chamar Start/Stop.
Não é fencing contra outro administrador ou prova universal de quiescência.

Own K_UTILITY_UPDATE Selected2026432bytes, quota gerida3145728 mantida.
Charge estrutural aplicável + coexistência SCM/rename/readback censados nesta
composição; não K histórico importado, pico nativo ou reserva física. Label
K_PREP do relatório compartilhado contém esse K do modo ativo; census imprime
UTILITY_UPDATE_K separado. Original retenção não é refund/crédito disponível.
Contar handles totais e inventariar originais; sem novo ceiling arbitrário ou
importação124/128/1024/B3/R3/G3. Quotas de outros domínios permanecem intactas.

Física/commit disponíveis >=2026432 em amostra fresca antes dos efeitos;
R=ceil(104448/U)*U, U da geometria real/coerente do volume. Se U4096, exemplo
R106496, não substituto da consulta. Caller/actual-free>=R antes de criar,
>=payload restante arredondado antes de escrever. Metadados/rename/flush não
são cobertos por R; aritmética/overflow e mudança de geometria negam.
Freshness por evento/contexto depois da confirmação/chamadas anteriores,
imediatamente antes da submissão dependente, sem renovar ato/plano/lifetime.
Nenhum TTL/duração numérico ou pico de outro domínio é inventado.

Riscos residuais a reconhecer nesta autorização própria: bootstrap/controller
de workspace/loader e mapeamentos anteriores não atestados universalmente,
CLR/CRT/interop/alocador/hash/SD/RPC/SCM/filesystem/cache/metadados/flush/rename/
kernel/retention opacos, exaustão/pressão entre amostras, I/O bloqueante, atraso
ou ausência de cancelamento imediato, estado parcial/intervalo sem utilitário e
retenção persistente sem rollback. Custos continuam UNKNOWN, não zero/ceiling.
Monitoramento é margem observacional fraca, não garantia ou reserva.
Essa aceitação de risco é limitada à manutenção, não restaura capacidade produtiva.

## STOP, lifetime, fechamento e consumo

Esc cancela confirmação; Ctrl+C/Break sinaliza Stop local do controller, nunca
ControlService. Stop/perda de continuidade, consulta desconhecida/insuficiente,
divergência de ator/serviço/SD/ID/pin/plano, alias/reparse/colisão, replay ou
close incerto impede novos efeitos. Não alterar ACL/direitos/share, encerrar
outro processo, trocar leaf ou conceder flags para obter PASS. I/O já emitido
pode não cancelar imediatamente. Não matar console como procedimento de correção.

Autoridade utilizada ao iniciar a invocação manual, inclusive wrapper DENY/
INVOKED=False/consumed0. Não há retry, rearm por restart, refund ou fallback.
Fechar somente referências próprias quando factual; conservar cohort/objetos/
créditos UNKNOWN em falha/fechamento incerto. Não limpar o candidato anterior
nem liberar créditos da operação consumida como efeito deste ato novo.

Sucesso limitado requer UTILITY_UPDATE_VERIFIED_CLOSED, first0/created1/consumed1/
close_known1, UTILITY_UPDATE_RENAMES mask3/canonical_verified_closed1 e wrapper
WRAPPER_CLOSE_UNCERTAIN=False. mask3 são DOIS renames confirmados, não três.
Conferir o novo ID reportado e anterior preservado via gates/readbacks, não
supor pelos paths ou exit0. historical_owner_close/OS_QUIESCENCE continuam
UNKNOWN quando não provados; sucesso da manutenção não é sucesso de UPDATE servidor.

## Procedimento candidato e autorização futura

update-utility.MANUAL.CANDIDATE.ps1 SHA256
cf8ea1c17316bbb8809b5e88982dcd5362a0ed0fd794acc2cb33a180bc09847b.
Mesmos bytes do draft qualificado; NÃO EXECUTADO. Inclui hashes explícitos do
próprio script/pedido/norma/manifesto e pins de arquivos por originais antes da
chamada. Aprovação deste pedido exato autorizará preparar comando literal e
aguardar UMA execução manual; não executar automaticamente.
WinPS5.1 Desktop x64 -NoProfile, Bypass somente no processo filho se necessário,
sem alterar CurrentUser/LocalMachine/GPO/ACL ou contornar política superior.
Sem UAC, resposta automática, retry ou placeholders.

Evidências offline: quatro builds zero warnings/68 pares afetados PASS;
manutenção29 cenários/145 checks, UPDATE106/634, preparação53/295 e entry21 por
configuração. Wrapper17 checks reais apenas workspace/loader de fixture stub,
argc14/old-lock release/AST; não prova nativa positiva da manutenção no alvo.
Não repetir campanha inalterada para autorizar por build. Reconciliar a saída
manual completa posteriormente, mantendo desconhecidos reais.

Proibidos UPDATE servidor/retry, qualquer alteração nos objetos da tentativa
anterior, preparação adicional/diretórios/contas, exclusão/restauração/limpeza/
refund/recuperação autoritativa, Start/Stop, endpoint, SCM/ACL mutante, CA/CURRENT,
rede operacional, TPM/NV/Q4 ou produção. Preservar históricos/pins/atos consumidos,
EXE anterior/DLLs/SCM/ProgramData/reservas/HEAD/WIP e PST0.6.4.
E21 PENDING; ED11/Phase6 PARTIAL; produção DENY; OS_QUIESCENCE UNKNOWN.
