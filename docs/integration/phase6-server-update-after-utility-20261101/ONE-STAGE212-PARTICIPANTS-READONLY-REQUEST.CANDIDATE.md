# Pedido — uma observação read-only dos participantes da etapa 212

Estado CANDIDATE. Requer aprovação operacional específica; não será executado automaticamente. Não autoriza disposição, Stop ou encerramento.

## Artefatos e escopo fixo

Host TCORP-TOBIAS, Windows PowerShell Desktop5.1 AMD64, console interativo sem redirecionamento, sem impersonação; operador original elevado S-1-5-21-785699550-1440458426-1682014700-1108.

Uma invocação manual de observe-stage212-participants.MANUAL.CANDIDATE.ps1, SHA256 4ff39b81d8ab16eaee50cd9224d5557f67fd0669c881e49bb1851e2db0fe2d8e. Fonte fixa stage212-participants-readonly.CANDIDATE.cs, SHA256 4eb1efdcbd9cf06f40c31321ae456eeb00a8c5f820864fca73969fe0cdfb2b9d. Ambos na pasta docs/integration/phase6-server-update-after-utility-20261101. O wrapper verifica seus pins e o pedido antes de chamar o observador. Nenhum argumento escolhe outro alvo.

Reutiliza os bindings e primitivas Win32 de consulta/identidade/fechamento do adaptador transitório existente, sem importar sua capacidade de TerminateProcess. Não cria serviço, owner, canal, protocolo, root, arquivo instalado ou mecanismo permanente.

## Por que esta consulta é necessária

A consulta anterior, já consumida, observou START_PENDING/PID18000 e controlador7580 com criação134381210166495538. Ela não obteve criação/token do runtime. As permissões excepcionais para PIDs148/15744 estão consumidas e não se aplicam. Identidade atual é necessária para delimitar qualquer futura disposição, mas não a autoriza e não recupera a identidade histórica interna do controller.

## Operações exatas e sequência

Após aprovação e confirmação manual OBSERVE_STAGE212_TWO_PARTICIPANTS_ONCE, consome a observação uma vez antes das consultas. Abre SCM local com SC_MANAGER_CONNECT(1) e o serviço exato PapinhoAccelerator com SERVICE_QUERY_STATUS(4). QueryServiceStatusEx exige WIN32_OWN_PROCESS(16), START_PENDING(2), PID18000 e códigos de saída zero; divergência cancela sem escolher outro processo.

Adquire somente PID7580 e PID18000 com PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE(0x00101000), sem herança. GetProcessId/GetProcessTimes e WaitForSingleObject(...,0) verificam PID, criação e existência viva; não há espera bloqueante. QueryFullProcessImageNameW(...,0) exige o caminho original C:\Program Files\Papinho\Accelerator\papacc_server.exe. Isso é conferência do nome retornado pelo processo, não atestado de imagem mapeada, hash ou proteção em disco.

O controlador deve corresponder à criação original134381210166495538, ao TokenUser original e aos TokenStatistics publicados na sessão: token126164448:0, authentication570063:0, modified126164115:0, groups17 e TokenPrimary. Runtime exige TokenPrimary e o SID virtual original S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099, derivado dos bytes SID do plano emitido, não de seleção arbitrária. Sua criação e estatísticas são observadas agora, sem inventar valores históricos; criação deve ser posterior ou igual à do controlador.

OpenProcessToken solicita somente TOKEN_QUERY(8). GetTokenInformation consulta somente TokenUser(1) e TokenStatistics(10), com buffers/ranges verificados. Revalida criação/processo/token pelos mesmos handles retidos; segunda consulta SCM deve manter o mesmo serviço/estado/PID. Não reabre os processos nem reutiliza PID isolado como identidade. Não há enumeração, leitura de memória, duplicação de token, impersonação, debug privilege ou alteração de permissões.

Registra somente role/PID/criação, SID e IDs/contagem de grupos já usados no mecanismo anterior. IDs não são tokens impersonáveis nem autoridade. Primeira falha/interrupção/mismatch mantém UNKNOWN e impede repetição. Não amplia automaticamente a consulta se o estado mudou ou processo saiu.

## Custos e fechamento

Até seis referências explícitas simultâneas do observador: SCM manager, serviço, dois processos e dois tokens. Buffer não gerenciado512bytes por consulta de token, liberado em finally, sequencial; buffers/objetos gerenciados e operações internas têm custos opacos. Nenhuma quota G3/B/R/U de outro domínio é importada e esses custos não provam capacidade. Compilação Add-Type pode usar artefatos temporários do compilador; nenhum destino instalado é criado ou atualizado.

Tenta fechar uma vez cada referência adquirida: token/processo runtime, token/processo controlador, serviço/SCM. Falha de fechamento é explicitamente UNKNOWN; não há retry ou compensação. observer_close_known não representa close dos aplicativos, SCQ ou reservas. Snapshot não garante estado futuro nem prova referência original histórica do runtime, autenticação bilateral, receipt/close ou quiescência OS.

## Limites absolutos e evidência futura

Uma execução manual, sem retry. Não autoriza Stop, kill/TerminateProcess, Start, endpoint/SCQ operacional, UPDATE, restauração, rollback, limpeza/exclusão, rename, ACL/SCM mutantes, CA/CURRENT, rede operacional, TPM/NV/Q4, refund ou autoridade produtiva. Essas APIs não estão presentes na fonte.

Preservar originais, EXEs anteriores, pins, autorizações consumidas, causa histórica212 UNKNOWN, retenção histórica e quiescência OS UNKNOWN, PST0.6.4. A saída integral futura será reconciliada antes de preparar qualquer pedido separado de disposição. Nenhuma aprovação desse pedido concede tal autoridade por inferência.

## Qualificação offline

Fonte nativa compilada sem executar RunFixed; versão modelo substitui integralmente as12 funções nativas e entrada de console.28 cenários PASS, incluindo caso válido, cancelamento, PID/criação/token/conta/path divergentes, insuficiência/erro de consulta, mudança SCM, token alterado na revalidação, quatro falhas de fechamento e replay. Ver STAGE212-PARTICIPANTS-OFFLINE-QUALIFICATION.json. Código operacional do produto não mudou nesta preparação; não repetir builds inalterados.
