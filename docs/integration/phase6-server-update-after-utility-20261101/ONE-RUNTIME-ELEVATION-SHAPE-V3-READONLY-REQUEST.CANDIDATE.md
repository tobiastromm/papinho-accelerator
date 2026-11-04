# Pedido — uma caracterização read-only corrigida V3 do token do runtime retido

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução manual sem retry. Não autoriza admitir token elevado nem alterar guards ou encerrar processos.

## Correção técnica focal da classe21 e versões anteriores

As execuções anteriores permanecem preservadas/encerradas sem retry automático: V1 falhou24 em20; V2 teve um cancelamento e depois execução reportada com SHAPE_SCALAR_LENGTH_CLASS_21/consumedTrue/completeFalse/observer_close_knownTrue/wrapper2. O tamanho devolvido pela execuçãoV2 não foi impresso e continua UNKNOWN: não afirmar retroativamente que era1.

Esclarecimento primário: https://github.com/microsoft/win32metadata/discussions/2308 . O mantenedor esclarece que a saída21 é BOOLEAN com ponteiro DWORD-aligned; confirma byte0/ReturnLength1 e trata DWORD como erro documental. Não há matriz formal universal de versões/arquiteturas; não alegar essa garantia. Esta é correção técnica do decoder do observador, não aceitação de privilégio ou mudança dos contratos do produto. Não herdar o perfilQ4/LAB.

V3 mantém capacidade defensiva4 para chamadas escalares e dados variáveis8192. Somente classe21: verifica endereço alinhado a4 e não nulo, zera4bytes do buffer próprio, exige sucesso/retorno1 e lê apenas byte0; não lê bytes1–3. Zero significa falso e qualquer byte não zero conserva seu valor bruto e significa verdadeiro. Capacidade4 não é apresentada como mínimo garantido. Não aceita retorno4/0/2/8 ou outro tamanho para21. Demais escalares20/18/12 continuam exigindo4, sem aceitar1 globalmente.

Registra classe/capacidade/sucesso/comprimento original/erro nativo capturado uma vez antes de testar o retorno, sem segunda consulta. Comprimento em falha é apenas snapshot, não prova de escrita. Valor escalar só é lido depois de largura válida. Capturas escalares são provisórias e explicitamente NÃO são revalidação final/autoridade; detalhes completos de grupos/contexto só após os gates de revalidação. Falha da impressão não substitui a primeira falha nativa ou o erro de comprimento. Não modifica interpretação/admissão de TokenIsElevated.

Uma nova execução só sob aprovação específica destes bytes. Sem reutilizar permissões consumidas, redimensionar/repetir consulta, fallback de tipo ou liberação de UNKNOWN. Nenhum novo serviço/canal/root/owner/infraestrutura e nenhum UPDATE para instrumentação.

## Causa e lacuna precisas

A sessão801214904f87a0209c3d4f1777d6924fe4a6b9dfa773ed7b4235082f7280e569 produziu site30/error5 na fonte qualificada5300ea54...: TokenIsElevated diferente de zero. A consulta TokenElevation teve sucesso; o5 foi gerado pelo predicado do owner, não por uma falha nativa de ACL. Essa evidência não prova causa da sessão anterior nem grupo Administrators ENABLED. TokenElevationType/grupos/restrições ainda não foram consultados pelo caminho que negou. O validador SCQ também exige elevated0. Não ignorar bit, inventar0 ou substituir por TokenElevationTypeDefault para passar.

Precisa-se distinguir a forma factual do token atual antes de classificar correção coberta ou especialização normativa. A observação não concede elegibilidade, autoridade ou prontidão. G0–G9 G5/G7/G9, M5/P3/virtual, OHI/SSO-B/P0–P7 continuam inalterados. Nenhuma proposta permissiva ou nova ADR está sendo aceita aqui.

## Artefatos e contexto

Fonte runtime-elevation-shape-v3-readonly.CANDIDATE.cs, SHA256 42840a1f93801e962775770bc4362070fcb89b1ee54fc9695eea7913845bcb42.
Procedimento observe-runtime-elevation-shape-v3.MANUAL.CANDIDATE.ps1, SHA256 ed6b3347a115783ac8ef2025a1655b36445c0fb2dead617b9a940371f904bda7.
Pasta C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101. Wrapper verifica hashes do pedido, procedimento e fonte antes das consultas.

Host TCORP-TOBIAS, PowerShell5.1 Desktop AMD64, console interativo sem redirecionamento, operador original elevado S-1-5-21-785699550-1440458426-1682014700-1108, sem impersonação ou autoelevação. Uma confirmação manual OBSERVE_RUNTIME_ELEVATION_SHAPE_V3_ONCE somente após aprovação específica; nenhuma resposta automática.

## Alvos e correlação limitada

Controlador fixo PID6120/criação134381245905057064/TokenUser original/token139600747:0/auth570063:0/modified139600167:0/groups17/TokenPrimary, conforme plano desta sessão. Não reutilizar alvo7580/18000 de sessão consumida.

Runtime: somente o PID retornado AGORA pelo SCM local para PapinhoAccelerator, OWN_PROCESS16/START_PENDING2/códigos de saída0/PID não zero e diferente do controlador. O PID desta nova sessão não foi impresso; não inventá-lo. A localização é exclusivamente QueryServiceStatusEx do serviço fixo, sem parâmetro CLI ou enumeração de processos. Não escolher outro serviço/conta/path por fallback. Segunda consulta deve manter o mesmo estado/PID; divergência interrompe sem perseguir outro processo.

Abre manager SC_MANAGER_CONNECT1/serviço SERVICE_QUERY_STATUS4. Retém cada processo por PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE0x00101000, sem herança, e token por TOKEN_QUERY8. Consulta GetProcessId/GetProcessTimes/WaitForSingleObject(...,0), QueryFullProcessImageNameW e TokenUser/Statistics; requer nome C:\Program Files\Papinho\Accelerator\papacc_server.exe, controlador original acima e runtime TokenPrimary/TokenUser igual ao SID virtual S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099. Criação do runtime atual não pode preceder a do controlador. Revalida criação/token pelos MESMOS handles, com nova consulta SCM após a captura. Isso é correlação atual, não prova do original histórico interno do owner nem atestação de imagem mapeada.

Contexto documental da imagem:797696/ID417614100:3014656:330305/SHA256bf7d0793428f5f751e87b007b28d676aa10235e6decc36d33bdd1de434508d60. Não reabre/hasha o EXE ou muda a instalação nesta consulta. Nome de processo é conferência limitada, não substituto da proteção/origem/continuidade exigidas para qualquer futura operação.

## Consultas somente no token do runtime

Pelo token retido, GetTokenInformation consulta (capacidade4 explícita nos escalares; retorno exigido1 somente para21 e4 para20/18/12; demais classes da forma com buffer8192):
- TokenElevation20: valor bruto, sem exigir0 ou fabricar1.
- TokenElevationType18: valor1/2/3; Default não é ausência de elevação.
- TokenGroups2: enumeração integral limitada ao buffer, todos os SIDs/atributos, count deve corresponder a Statistics.
- TokenRestrictedSids11: enumeração de SIDs/atributos restritos.
- TokenHasRestrictions21: valor bruto BOOLEAN de byte0, campo has_ever_been_filtered_raw, distinto da lista atual de restricting SIDs. TokenSessionId12: DWORD. Não são defaults de admissão ou concessão.
- TokenPrivileges3: todos os LUIDs/atributos; LookupPrivilegeNameW local obtém os nomes com buffer fixo, sem habilitar/ajustar privilégios.
- TokenIntegrityLevel25: SID/atributos do mandatory label, sem alterá-lo.

Verificar comprimentos/counts/offsets/SIDs no buffer original. Consulta incompleta/tamanho inválido/cap excedido/nome de privilégio indisponível => falha/UNKNOWN, sem redimensionar/repetir/ignorar campos. Revalida TokenStatistics/criação após captura; alteração invalida o conjunto. Capturas escalares originais podem aparecer antes da revalidação, com marca de não autoridade; só publica o conjunto detalhado após revalidação e segunda consulta SCM. Uma falha de close posterior mantém dados observados mas impede resultado completo com close conhecido.

O indicador Administrators ENABLED é derivado apenas da entrada SID S-1-5-32-544 com SE_GROUP_ENABLED e sem DENY_ONLY; presença/atributos são impressos sem transformá-los em prova completa de acesso efetivo ou autoridade. Não invoca CheckTokenMembership com token primário, não duplica/instala token de impersonação, não consulta chave/CA/arquivo privado. Um token incompatível pode ser observado como DATA sem ser admitido pelo produto. first0 da consulta não significa segurança/elegibilidade/PASS de SCQ.

## Custos, fechamento e limites

Até6 referências explícitas do observador (SCM/serviço/dois processos/dois tokens); buffer512bytes para consultas básicas e8192bytes sequencial para a forma do token, mais nomes de privilégios de256caracteres e dados gerenciados limitados pelos comprimentos verificados. Sem TOKEN_LINKED_TOKEN e sem adquirir handle ligado. Lookup de nomes é local. Custos internos/gerenciados/compilador/kernel permanecem opacos; não importar G3/B/R/U nem alegar máximo nativo/capacidade. Compilação Add-Type pode usar temporários do compilador, sem arquivo instalado/protegido novo.

Libera buffers em finally; tenta fechar uma vez apenas referências próprias, sem retry. Close do observador não é close das aplicações, SCQ, borrows ou reservas. Snapshot não é lease nem prova retrospectiva. Retenção histórica/quiescência OS permanecem UNKNOWN.

Proibidos: Start/Stop, endpoint/SCQ, TerminateProcess/kill, UPDATE, leitura/escrita de memória, debug privilege/AdjustTokenPrivileges/AdjustTokenGroups, impersonação, duplicação de token, escrita de arquivo-alvo, ACL/SCM mutantes, CA/CURRENT, rede operacional, TPM/NV/Q4, limpeza/refund/rollback/restauração/recuperação ou autoridade produtiva. Nenhuma dessas operações existe no observador. Não fechar janelas/processos retidos nem reutilizar autorizações consumidas.

## Qualificação e evidência futura

Fonte real compilada sem invocar RunFixed. Modelo substitui13 APIs e a entrada de console,47 cenários,20 verificações de capacidade planejada,1 de alinhamento e3 de preservação de erro sob falha de output PASS: elevação0/1, tiposDefault/Full/Limited, Administrators ENABLED/DENY_ONLY, restrições, privilégios/nomes/labels, consultas incompletas/count/pointer inválidos, token alterado, SCM/PID divergente, cancelamento/replay e close UNKNOWN. Ver RUNTIME-ELEVATION-SHAPE-V3-OFFLINE-QUALIFICATION.json.

Depois da única execução autorizada, preservar saída integral/primeira falha/consumo, criação/PID/token atual, valores de elevação/grupos/restrições/privilégios/integridade, continuidade e close. Não modificar guards ou preparar novo UPDATE/Start por inferência. Com dados suficientes, confrontar forma efetiva com accepted e preparar somente correção já coberta ou uma decisão focal realmente necessária. Qualquer disposição dos participantes é independente e requer autorização específica.

Fontes primárias: https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_elevation ; https://learn.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-token_elevation_type ; https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership .

A referência Win32 ainda descreve21 como DWORD, divergência tratada pelo esclarecimento primário acima. Não assumir IsTokenRestricted equivalente: sua semântica limita-se à lista de restricting SIDs. Este observador não substitui uma API pela outra.

Fonte para erro24: https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499- . A falha do observador não muda o predicado TokenIsElevated não zero demonstrado na sessão diagnóstica anterior.
