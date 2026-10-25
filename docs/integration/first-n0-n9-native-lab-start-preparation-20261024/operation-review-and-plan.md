# Preparação da primeira operação N0–N9 instrumentada

TASK=PACC-PHASE6-FIRST-N0-N9-NATIVE-LAB-START-PREPARATION-1

## Resultado da revisão

Preparação documental realizada; operação executável ainda NÃO qualificada. Não entregar o comando antigo --cycle-lab como sucessor N0–N9. Esta revisão do agente executor não é parecer de revisor independente. Nenhum ADR accepted foi alterado. HEAD/main permanece 913c90cf2328f7939d8f5b927640601903ac6e91. PROJECT-CONTEXT.md ausente. Continuidade da composição de governança já lida na sessão; sem mudança arquitetural nova.

As 18 preimagens do manifest qualificado anterior, incluindo decisões, fontes, script e quatro executáveis, conferem byte-exatamente (pin-verification.json). O resultado N0PF fornecido pelo owner está registrado em historical-preflight-result.json: first0/close0/exit0, qualificador120 handles; administrativo747+271=1018/1024. Margem6 é somente observação histórica, não reserva nem capacidade atual. Sem nova consulta protegida, preflight, ETW ou execução de serviço. Quiescent0 no recibo read-only NÃO é quiescência de serviço.

## Identidade exata candidata

Host SHA256 2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff; Windows10.0.19045 AMD64; NTFS serial417614100; boot Kernel-General12 record236441, StartTime2026-10-23T18:36:34.5000000Z. Operador SID S-1-5-21-785699550-1440458426-1682014700-1108: token primário elevado, sem impersonation, revalidado no original; não preencher AuthenticationId/token futuro.

Serviço existente PapinhoAcceleratorLab20261023; conta NT SERVICE\PapinhoAcceleratorLab20261023; SID S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451. Exigir DEMAND_START, own-process, SID UNRESTRICTED, STOPPED/PID0 antes dos efeitos. Estado atual ainda depende do readback da futura operação, não inferir do histórico.

Tentativa75330d11cc7e4123ae62eadc9a59c2f5. Não reutilizar a tentativa consumida563826bacc49449f954b20cc48ce9b36. Novo rótulo administrativo candidato N0START-20261024-01 não concede autoridade ou cria outro intent.

Config existente116 bytes, nativo417614100:393216:1120240, SHA256 a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44. Pai original417614100:1966080:79426 em build/scm-protected-installation-binding-20261022/Selected/Release. Imagem registrada histórica papacc_server_lab_host.exe SHA25662087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716: confirmar na operação, não substituir silenciosamente.

Fonte instrumentada build/session0-diagnostic-runtime-integration-20261023/Selected/Release/papacc_server_lab_host.exe, SHA256 eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b. Destino candidato EXCLUSIVO no pai: papacc_server_lab_host_ntstatus.exe. Nenhum arquivo copiado; identidade futura só após criação real.

Dois inputs existentes em C:/ProgramData/PapinhoAccelerator/Protected/N0: evidence5358/hashbabab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0/nativo417614100:196608:2841429; approved326/hash5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294/nativo417614100:131072:2841430. Descriptor152/hash0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814. Reutilizar bytes e referências originais, sem recriação. RAW1, N0–N9, B+D e ED11 hashes no manifest anexo continuam controlling.

Sink candidato no mesmo pai: papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.diag; CREATE_NEW, proteção explícita desde criação, arm1024 + um append1024, total lógico2048, sem overwrite/retry/delete. Não modificar ACL existente. Não antecipar file ID.

## Primeira divergência e menor slice necessário

1. scm-instrumented-cycle-lab.inc:182-185 bloqueia qualquer entrada diferente da forma read-only. Em :198-201, --prepare-n0 nunca define positive. O header scm-n0-context-pins.h habilita inputs somente nesse perfil. O script qualificado invoca exclusivamente --prepare-n0 (:98). Não há pins/script positivos qualificados; trocar argumento ou remover guard não constitui integração segura.
2. O caminho antigo :204/:219 espera dois PowerShell; :228 bind_launch usa o segundo. N0 usa esse campo para auxiliar. Deve conectar operação positiva ao MESMO binding protegido/owner N0–N9 e seu consumo existente; não duplicar PID, owner ou autoridade.
3. Readback de recovery actions/restart não está implementado no ciclo instrumentado revisado. Qualificar consulta bounded QueryServiceConfig2 de failure actions e respectiva flag, mais triggers/config relevantes: qualquer ação de restart/run-command/reboot ou resultado desconhecido => DENY, sem alterar configuração para passar. Não declarar ausência de restart com base em DEMAND_START. Nenhuma consulta SCM adicional foi executada nesta tarefa.
4. Provar no fluxo positivo original: freshness das referências N0, orçamento antes de consumir e antes de cada efeito; revalidação após cópia/rebind; inclusão do serviço como papel2 quando factual, sem substituir auxiliar papel3. Manter 60s desde observação mais antiga e final1s; no máximo duas gerações pré-efeito. Consumir antes do primeiro efeito; a cópia/arm/rebind podem consumir sem chegar ao Start. Sem rearm ou segundo Start.
5. Qualificar Stop em falha de observação/limite, inclusive START_PENDING sem controls: suprimir novos efeitos, uma solicitação segura quando aceita, retenção se indisponível. O ciclo antigo :288-301 solicita Stop ao atingir RUNNING, mas timeout/erro pode reter antes disso. Não prometer Stop aceito ou concluído em todos os ramos; nenhuma adaptação positiva N0 testada.

Este é trabalho focal no owner/ciclo/launcher existentes, não uma nova arquitetura. Não alterar decisões nem quotas. O escopo atual prepara a operação; não implementar implicitamente a remoção da barreira read-only. Próximo slice deve qualificar essas conexões e produzir novos hashes, preservando o artefato read-only histórico.

## Sequência candidata futura e efeitos que exigem aprovação

1. Um PowerShell5.1 x64 já elevado; conferir hashes do sucessor e serviço/imagens/dependências, host/boot/operator/inputs originais. Confirmar plano e desafio nativo exatos vinculados à tentativa. Sem segundo PowerShell.
2. Adquirir exclusividade canônica, validar estado/conta/SID/DACL/dependências e ausência de recovery automático; conferir ausência de colisão de intent/imagem/sink. Ausência ou falha de qualquer evidência => DENY.
3. Censo N0–N9 e métricas atuais; administrativo PS+aux <=256MiB/1024handles/64threads/32MiB stack; qualificador e serviço cada64MiB/128/16/8MiB. Quatro processos máximos; serviço ausente não ganha PID fictício. RAM>=5GiB/commit>=9GiB, pisos4/8GiB; créditos NONE_GRANTED. UNKNOWN kernel/cache/stack/picos não amostrados persistem.
4. Após confirmação explícita, consumir intent canônico uma vez antes de efeito; criar exclusivamente imagem e arm protegidos, verificar bytes/identidade/fechamento, executar UM rebind explícito do ImagePath somente para a imagem instrumentada. Preservar tipo/start/conta/SID/DACL/recovery; sem restauração automática. Cada efeito parcial permanece registrado e consome tentativa.
5. Revalidar referências/SCM/censo e amostra final <=1s e dossier<=60s; recursos excedidos ou stale => nenhum Start. UM StartService, sem retry. Capturar START_PENDING/status/PID factual e token quando observáveis; amostras alvo100ms/gap<=1s, métricas do serviço e host. Não garantir captura de toda transição ou pico.
6. Preservar primeira falha e NTSTATUS válido do recorder existente; coletar API/objeto/access/disposition/options pelo registro e inventário estático referenciado. Erro de binding do sink anterior ao recorder pode deixar captura indisponível: não afirmar que NTSTATUS sempre existirá. SCM5/5023 não substitui código nativo. Não adicionar outro logger ou canal SCM nesta tarefa.
7. Um Stop seguro quando controls permitirem; distinguir solicitação/aceitação/STOP_PENDING/STOPPED, término pelo handle original, quiescência do runtime e fechamento. Falha inicial pode já terminar o serviço sem Stop enviado. Nunca kill, force release, delete, rollback ou retry. Console perdido/timeout não é cleanup; reter UNKNOWN e charges.

Nenhuma rede/listener/CA/emissão/bootstrap/CURRENT. Aprovação futura deve incluir expressamente criação exclusiva de imagem/arm/intent, rebind preciso, um Start e até um Stop, retenção de efeitos parciais e riscos N7/ED11. Aprovar somente Start sem esses preparativos não basta.

## Testes e entrega executável pendentes

Sem alteração de fonte nesta tarefa: builds não repetidos; resultados anteriores não certificam o caminho positivo. Matriz obrigatória no slice: administrativo1025/qualificador129, token/boot/hash/file-ID divergente, replay, quinto processo, restart automático/flag desconhecida, sink existente, SCM divergente, freshness60s/final1s, Stop-before-effect/START_PENDING/RUNNING, falha parcial/fechamento incerto/UNKNOWN/retention. Selected/Legacy Debug/Release /W4 /WX zero warnings. Modelos separados de provas nativas. Nenhum teste operacional executado aqui.

COMANDO_MANUAL_POSITIVO=NOT_AVAILABLE_UNTIL_SUCCESSOR_IMPLEMENTATION_AND_OFFLINE_QUALIFICATION. Não usar --cycle-lab antigo, nem editar pins/guard manualmente. Confirmação estática candidata: START_N0_SINGLE_LAB N0START-20261024-01 75330d11cc7e4123ae62eadc9a59c2f5, acrescida de desafio/plano nativos futuros. Não executável ou autorização. O comando final e hashes somente poderão ser entregues depois de compilar e revisar o sucessor real. Não solicitar aprovação de uma operação sem esses artefatos concretos.
