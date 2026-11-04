# P1 SD — originais fechados; sessão negada antes da imagem

2026-11-02, America/Sao_Paulo. P1_SD_SESSION_DENIED_PRE_IMAGE_GUARD124.
Fonte: saída colada pelo proprietário no chat. Linhas relevantes transcritas em
OWNER-P1-SD-SESSION-RECEIVED-EXCERPT.txt, SHA256
23d7471824ef1fe392af9e89a2e387d3d1bc1e8a47e9f34178c94403ed413b50.
É transcrição do conteúdo recebido, não preservação byte-exata da codificação
original do console. Resumo estruturado P1-SD-SESSION-RESULT.json.

## Materialização efetivamente concluída

Pedido16e0693541eed115886c59092ebac8f5b32dfe68ef2a893b5fda5d49fd7fb99a,
decisão019222b9ea2c4e2e0889639098c63ff5187b04de96b8ddc768a0b9fcd437b28b,
driver86a21d84647e00bce9654d699b95aadcbe8f2e737890ec36967194deee704e93
correspondem ao pacote aprovado. Confirmação documental e linha P1 DATA foram
digitadas conforme exibidas; não há erro de colagem demonstrado.

Novos originais no parent documental existente:

| Original | Identidade capturada | Bytes/pin vinculados |
|---|---|---|
| FIRST-P1-SD-SESSION-REQUEST.ORIGINAL.md | 417614100:9830400:40231 | 15241/pedido16e069...7fb99a |
| FIRST-P1-SD-OPERATIONAL-DECISION.ORIGINAL.txt | 417614100:1769472:228178 | 1860/decisão019222...37b28b |

preparedTrue,firstNONE,consumedTrue,created_mask3,verified_mask3,close_knownTrue
e MANUAL_P1_ORIGINALS_EXIT0 sustentam preparação/bytes/hash/SD/readback/fechamento
pelo caminho qualificado. Nenhuma consulta instalada nova pelo agente.
Preservar arquivos e identidades; não criar novamente, excluir ou alterar ACLs.

## Primeira falha real e o que avançou

SCQ_COLD_FIRST_CAUSE:stage2/reference2/mapped1816/native_error0,
query_attempted1/query_succeeded1/count_known1/own_count124/guard124/cap128.
No loop scqc_plan, ref0=pedido,ref1=decisão,ref2=imagem. A query bem-sucedida
negou ANTES da NtCreateFile da imagem. Não é falha de consulta, ACL ou nova
reparse;124 satisfaz cap128 mas NÃO a condição de adquirir com count<124.

Última abertura capturada ref1/NTSTATUS0/IOSBstatus0/info1/handle1 corresponde
à decisão, não à imagem. Última consulta SD:calls2/NTSTATUS0/needed136/
capacity136 é o sizing+fill nativo concluído desse original. Não é contagem de
handles nem prova de ausência de custos internos. O ponto alcançado demonstra
que pedido/decisão e SD passaram pelas operações anteriores do loop qualificado.
O diagnóstico conserva só os últimos snapshots: as contagens Q0/Q1 anteriores
não estão disponíveis. Não calcular baseline122, delta contra113/119/125 ou
economia de oito handles por subtração entre execuções distintas.

Assim, NtQuerySecurityObject funcionou no ponto observado e houve avanço da
falha antiga antes da decisão para a guarda antes da imagem. Isso NÃO prova
redução total de handles, causa histórica ou fechamento do envelope do Start.

## Efeitos e autoridade consumida

Autorização manual USED_NO_RETRY, inclusive por a fase nativa ter sido invocada
antes de qualquer confirmação. Os três confirmadores E/Q/S e seus consumos
internos não foram alcançados pelo caminho qualificado: scqc_plan falhou antes
de profile_prepare/session_acquire/first_controller. Endpoint/SCQ/Start não
executados por essa invocação; NO_SCM_START também consta na saída.
Não confundir uso da autorização manual com consumo interno dos três atos.
Nenhum runtime/PID/token/bilateral/receipt/close/RUNNING foi demonstrado.

Estado atual do serviço não foi consultado; ausência de Start pelo caminho não
é prova fresca de STOPPED contra outros atores. Imagem selecionada continua a
expectativa reconciliada781312/ID417614100:8716288:38019/hashdc4855...3076;
aquisição original nativa dessa imagem não foi alcançada nesta sessão.

## Ownership/lifetime no ponto negado, sem nova coleta

Raw0 e raw1 são as referências próprias de pedido e decisão, retidas durante
construção do plano para proveniência/continuidade OHI. Não há sucessoras
qualificadas nesse ponto; não fechar/reabrir por path/hash para obter margem.
SD temporário foi usado para hash e LocalFree antes da próxima iteração; o
helper papacc_store_hash só publica sucesso após DestroyHash e CloseProvider.
Seus slots públicos encerrados não provam encerramento de caches/internals.
HMODULE de ntdll é emprestado, sem FreeLibrary legítimo. Arrays/strings/plan são
memória, não handles dispensáveis. Cadeia de diretórios, duplicatas/readers,
SCM/pipe/carrier/peer ainda são posteriores, não recursos já abertos a fechar.

Não foi encontrado novo fechamento seguro ou leak demonstrado pela saída.
Internals/recursos herdados e inicializações precisam de ownership comprovado;
não tocar neles. O saldo completo da invocação não é conhecido sem Q0/Q1.
Não aumentar quota, subtrair overhead ou mover a guarda. Nenhum código alterado
nesta reconciliação; builds/testes inalterados não repetidos.

Fechamento documental tem marcador positivo. Controlador retornou ao PowerShell
sem marcador positivo próprio de close: seu caminho prevê closes síncronos e
finish, mas retorno/ausência de mensagem de retenção não serão registrados
como close_known1 observado. NativeClose dessa fase permanece UNKNOWN na
evidência recebida; não resolve coortes antigas ou quiescência OS.
Não solicitar outra consulta apenas para trocar UNKNOWN por PASS.

## Primeiro bloqueio e continuação mínima

Bloqueio demonstrado: count124 antes de adquirir imagem, guard124 aplicável e
inalterada. Funcionalidade de SD nativo passou, admissão total NÃO. Nenhum
novo pedido de sessão/coleta/UPDATE ou comando é autorizado por essa saída.

Antes de nova operação, aprofundar offline somente o custo/lifetime do hash de
descritores que permanece entre essas queries e os contratos dos backends já
existentes. Duas execuções do helper no loop não são prova de que CNG causou
o124. Não fazer warm-up, fixar provider, adotar pseudo-handle/one-shot/novo
backend, trocar representação SD ou usar SHA de outro domínio por conveniência.
ADR-CANDIDATE-canonical-storage-sha256-backend.md consta PROPOSED, não é autoridade
para reutilização administrativa. Corrigir só defeito/equivalência demonstrados;
se faltar decisão normativa real, citar cláusula antes de proposta focal única.
Sem reabrir ownership já auditado dos originais ou outra coleta global redundante.

NEXT-P1-PREIMAGE-124-HASH-BOUNDARY-OFFLINE.md delimita esse próximo passo.
Preservar originais novos/históricos, atos consumidos/EXEs/pins/HEAD/WIP,
PST0.6.4/124/128/G3/DENY/UNKNOWN. Sem nova coleta, UPDATE, endpoint/SCQ/Start/Stop,
SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund/rollback/recuperação.
