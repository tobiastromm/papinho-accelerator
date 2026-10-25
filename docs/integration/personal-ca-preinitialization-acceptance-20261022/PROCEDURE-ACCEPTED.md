# Personal CA preinitialization — accepted normative procedure, revision1

Authority: [DECISION-personal-ca-preinitialization-20261022](../../adr/DECISION-personal-ca-preinitialization-20261022.md). Effective only with that record's official write approval and hash verification. Original proposal unchanged: ../personal-ca-preinitialization-20261022/PROCEDURE-PROPOSED.md, complete SHA256 284f1fd1b246e1abdff246e5260667e75bfd2aea0362b35566cdc4f6842fd72d.

The following A–I bytes are copied exactly; segment SHA256 600d81ba1579da7909a73b14c87298640f7c57396b278fc592ed44c5fe0f502f. Historical proposal-status wording is controlled only by the subsequent decision's delimited acceptance. Candidate tools, unqualified recovery/isolation and absent installation inputs remain pending; none of the command templates may be executed under this documentary acceptance. No claim that a SAM account SID equals an SCM service SID.

## A. Três gates separados

A — Antes de criar chave produtiva: procedimento aprovado; escolhas concretas da instalação preenchidas e verificadas; ato NEW_INSTALL e CA_INITIALIZE específico; operador nativo elegível; alvo exclusivo; identidade operacional distinta; proteção/custódia e recursos admissíveis. O plano read-only não autoriza efeitos. Aprovar este texto não supre fatos nem libera o guard atual.

B — Depois da criação autorizada: backup inicial realmente realizado, consistente e protegido. Não se exige backup de uma chave inexistente em A. Nenhuma emissão ou recuperação é habilitada pela criação/finalização da chave.

C — Antes de habilitar emissão: restauração isolada verificada, correspondência chave/certificado/SPKI, propriedades e testes de acesso com tokens reais, histórico/counters/status consistentes e fencing/latest-state suficientes. Certificados, assinatura e CURRENT exigem suas próprias operações canônicas autorizadas; este procedimento não cria atalho para produzi-los. Se ainda faltarem certificado, reserva canônica ou fonte UTC para essa qualificação, C permanece DENY.

## B. Escolhas obrigatórias da execução

O owner registra no plano/referências e recibos administrativos existentes: instalação/parent por identidade nativa, caminho administrativo absoluto, volumes do OS e de TODOS os dados da root/issuer/history, edição/build Windows, versões dos utilitários, operador/SID nativo, conta/SID operacional, custodiantes, dois meios físicos de recuperação identificados por disco/volume nativos, local físico de cada meio, meio separado de desbloqueio, mídia WinRE e alvo/discos de restauração autorizados. Letras de unidade são resolvidas novamente por identidade em cada ambiente, nunca usadas como prova.

Esses valores não foram escolhidos para o host do owner nesta tarefa. Campos não preenchidos significam gate não demonstrado, não valores padrão. Não cria schema persistido, registry de identidade ou journal paralelo; recibos descrevem evidência, não autorizam resumption por si mesmos.

## C. Chave e custódia

Reutilizar papacc_ca_key_create/validate/borrow/return/close no owner existente: RSA2048/e65537, Microsoft Software KSP, MACHINE, export policy0, assinatura somente. Nome conforme authority16/generation16 do contrato. IDs e tentativa usam CSPRNG existente, sem relógio como substituto. Nunca reusar as duas identidades LAB ou uma identidade UNKNOWN; colisão/erro não autoriza escolher sucessivos nomes até funcionar.

DACL protegida: operador específico e SYSTEM necessário ao provider; serviço operacional explicitamente excluído do acesso privado. Validar a DACL e a correspondência pública no owner; não adotar chave por nome. A implementação atual usa GENERIC_ALL ao operador e deny ao serviço: é evidência do código, não prova de menor privilégio nem de isolamento; a adequação do conjunto de direitos e os testes reais de acesso continuam requisitos de 3.3. Não ampliar ACL automaticamente nem ajustar por tentativa e erro.

Custodiante administrativo local assume a chave via owner/lease, sem material privado em CLI/log/env/config. Custodiante de recuperação guarda os meios/chaves de desbloqueio fora do runtime; no perfil pessoal pode ser a mesma pessoa em papel explícito separado, sem alegação de controle por duas pessoas. A conta operacional não recebe meios, senha de recuperação, token admin ou handle de CA. Não exportar chave privada; acesso público não concede autoridade.

Create, propriedades, Finalize e close são fatos separados. Falha/Stop invalida novos efeitos e mantém referências/recursos necessários até quiescência factual. Timeout não libera. Finalize/cleanup incerto mantém disposição UNKNOWN e recibo/owner, sem delete/retry automático nem reutilização da autorização. NCryptFreeObject confirmado fecha handle, não apaga chave persistida. Não classificar a chave LAB antiga como ausente.

## D. Mecanismo de backup candidato e capacidade demonstrada

Direção proposta: imagem de volumes do Windows por wbadmin com -allCritical e inclusão explícita dos volumes de dados, em dois discos externos distintos criptografados por BitLocker e mantidos offline. É capacidade documentada da ferramenta, não seleção qualificada para a CA desta máquina. Não usar cópia manual de arquivos KSP, export PKCS8, ZIP ou System Restore como substitutos.

A Microsoft documenta imagem bare-metal e inclusão de volumes para Windows10/11 em [wbadmin start backup](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wbadmin-start-backup). A interface WinRE para restauração está em [wbadmin start sysrecovery](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wbadmin-start-sysrecovery). Isso não garante recuperação desta chave KSP nem portabilidade entre equipamentos. BitLocker protege o volume do destino, não é criptografia automaticamente fornecida pelo wbadmin; capacidade de encriptar volumes de dados/removíveis está no [guia oficial](https://learn.microsoft.com/en-us/windows/security/operating-system-security/data-protection/bitlocker/operations-guide).

Antes de aceitar seleção para uma instalação: demonstrar em LAB separado a imagem/restore completa no OS/hardware/WinRE e mídia pretendidos, incluindo estado protegido MACHINE de chave descartável autorizada e correspondência pública. Nenhuma nova chave LAB nem restore é autorizado aqui; não tocar na chave LAB existente. Falta desse ensaio mantém o mecanismo como CANDIDATE, e não se declara o gate produtivo fechado. Nenhuma outra biblioteca/ferramenta é escolhida como fallback.

## E. Backup inicial e manutenção — roteiro manual futuro

1. Conferir autorização específica, escolhas de B, energia/espaço e ausência de outra operação. Inspecionar BitLocker no destino pelo painel Windows: criptografia concluída, proteção ativa, desbloqueio independente previamente testado. Desabilitação/suspensão não constitui proteção. Os segredos ficam no meio separado, nunca no recibo ou histórico de comandos. Nenhuma configuração de disco/BitLocker é realizada nesta tarefa.
2. Pelo owner canônico, suspender novos efeitos administrativos/assinatura, drenar borrowers e obter quiescência factual. Se o código não fornece essa fronteira ou houver CAS/signature/cleanup UNKNOWN, STOP: não alegar que VSS torna o aplicativo consistente. Manter KSP, SAM/SECURITY, OS e TODOS os volumes da root/issuer na mesma imagem/corte consistente. Não desmontar owner retido para conseguir backup.
3. Conferir identidades dos discos e seleção de volumes. Um comando futuro, EXPANDIDO e revisado por execução, terá esta forma (metavariáveis não são comandos executáveis):

```text
wbadmin start backup -backupTarget:<DESTINO_IDENTIFICADO>: -include:<VOLUMES_OS_E_DADOS> -allCritical -vssCopy
wbadmin get versions -backupTarget:<DESTINO_IDENTIFICADO>:
wbadmin get items -version:<VERSAO_REAL_DO_CATALOGO> -backupTarget:<DESTINO_IDENTIFICADO>: -machine:<ORIGEM_REAL>
```

Não usar -quiet, -noVerify, exclusões, credenciais na linha de comando ou -vssFull que possa truncar logs históricos. Confirmar o conjunto real de volumes/artifacts no catálogo e códigos/eventos; ausência de erro não prova restore. [get versions](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wbadmin-get-versions) e [get items](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wbadmin-get-items) fornecem descoberta de catálogo/conteúdo, não prova de latest-state.
4. Guardar imagem, catálogo e recibos não secretos: IDs de mídia/corte/CA/root, revisões, CURRENT quando existente, predecessor, issuer reservations/high-water, certificados/DER/hashes/status/revogações e checkpoint canônico existente. Ausência de CURRENT inicial é registrada como tal, sem predecessor fictício. Incluir o volume inteiro de proteção OS/KSP, não apenas lista de arquivos públicos. Só marcar recuperável após comprovação exigida.
5. Enquanto o corte ainda está protegido, produzir e verificar segunda geração de recuperação em meio físico independente. Guardar os dois offline em locais fisicamente separados escolhidos pelo owner, com acesso restrito. Durante substituição, preservar as duas gerações válidas até verificar a nova; se necessário usar terceiro meio temporário, sem sobrescrever a única geração recuperável. Espaço de imagem não é quota de heap.
6. Após cada estado de emissão/reserva/status/rotação relevante, arquivar duravelmente o estado/checkpoint canônico consistente antes de reportá-lo recuperável. Política conservadora desta proposta: pausar novos efeitos até esse arquivamento ser confirmado. O produto atual não comprova esse ciclo automaticamente; não inventar acknowledgement. Se o custo/tempo/espaço for inviável, manter DENY e revisar explicitamente o procedimento, sem reduzir histórico.
7. Fechar trabalho/handles somente de forma factual, bloquear/ejetar mídia pelo Windows e confirmar inacessibilidade sem desbloqueio antes de armazenar. Erro de fechamento/ejeção preserva retenção; não presumir offline por timeout. Retomar somente os efeitos já autorizados e qualificados; backup não promove autoridade.

## F. Recuperação e teste antes de emissão

1. Autorizar separadamente alvo destrutivo de restore e cada disco. Parar emissão, conter fisicamente todos os antigos emissores/cópias conhecidas (desligados, mídias retiradas e sob custódia); guardar registro de custódia independente no acervo administrativo existente. Um serviço parado, hostname, hash, timestamp ou flag local não cerca clones desconhecidos. Se todos os emissores anteriores não puderem ser cercados, NUNCA retomar assinatura da mesma geração.
2. Iniciar WinRE de mídia escolhida em alvo isolado sem rede. Desbloquear destino BitLocker por prompt nativo ou chave externa em meio separado, sem capturar segredo. [manage-bde unlock](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/manage-bde-unlock) suporta prompt de senha e chave .bek; não colocar recovery password na linha de comando ou em transcrição.
3. Conferir catálogo, IDs físicos, origem e versão real. Revisar comando futuro:

```text
wbadmin start sysrecovery -version:<VERSAO_REAL> -backupTarget:<DESTINO_NO_WINRE>: -machine:<ORIGEM_REAL> -restoreAllVolumes
```

Executar só na operação de recuperação especificamente aprovada, a partir de WinRE. Não usar -recreateDisks ou restauração sobre host produtivo por inferência. Conferir partições/dados incluídos antes de aceitar qualquer operação destrutiva. Se o layout exigir outra flag, nova revisão exata. No alvo isolado, primeiro boot continua sem emissão/autoridade automática.
4. Pelo mecanismo canônico autorizado, abrir a chave selecionada sem geração, conferir authority/generation/SPKI e certificado quando existente, export policy0, uso, DACL/SIDs e propriedade de chave pública. Testar negação real de acesso privado com token operacional e operações permitidas somente com token administrativo/purpose/lease. Testes de assinatura/exportação negativa exigidos por 3.1 são operações separadamente autorizadas; não assinar mensagem artificial, emitir certificado sem reserva ou exibir bytes privados. Falta de certificado/UTC/serial reservation não é suprida por fixture.
5. Comparar root/history/reservations/serial burned/high-water/revogações/DER e correlações UNKNOWN com o checkpoint mais recente sob custódia independente da imagem e com saídas públicas posteriores. Uma geração antiga válida não prova ser a última. Se o acervo independente estiver incompleto, assinatura da geração antiga permanece DENY; preservar validação pública/histórico. Eventual nova geração/trust rotation requer sua própria decisão e operação, nunca regeneração automática.
6. Verificar proteção de disco/OS restaurado e acessos antes de qualquer uso; não presumir que os volumes restaurados permanecem criptografados. Comparar propriedades e byte hashes públicos, registrar resultados e disposição. O restore de ensaio não pode virar segundo emissor. Encerrar owners factualmente e conter o alvo restaurado sob custódia; não alegar exclusão física por fechamento de handles.
7. C só passa com todas as provas reais e ED11/host admission aplicáveis; teste isolado não ativa produção. Não se promete recuperação em outro equipamento: se o estado de proteção MACHINE não se recuperar, preservar DENY, não tornar a chave exportável.

## G. Rotação e aposentadoria

Preparar rotação explicitamente até 90 dias antes de expirar CA (perfil técnico 1.4), ou por comprometimento, perda de proteção/recuperabilidade, mudança de custodiante/SID relevante ou esgotamento/risco de serial. Risco não autoriza novo nome para contornar UNKNOWN. Confirmar plano/autoridade da mesma root, nova geração/chave e proteção/backup/restore, reservas/certificado autorizados, publicação canônica e transição de trust por gates próprios. Não reinicializar na partida.

Preservar certificados anteriores, DER, revogações, referências de clientes, counters/burned reservations e predecessor. Revogação não apaga história ou revoga automaticamente Sessions sem política accepted. Antiga chave permanece enquanto trabalho retido precisar dela; nova emissão antiga fica desabilitada conforme transição autorizada. Aposentadoria exige ação distinta, nenhuma lease/borrow/worker ativo, quiescência/close factuais e política de retenção/recovery satisfeita. Não estipular prazo inventado nem apagar automaticamente chave/imagem única. Meios antigos permanecem protegidos e não autorizam restart/assinatura. Falha de rotação/retirement retém estado e mantém DENY, sem rollback presumido.

## H. Identidade operacional proposta

Direção mínima proposta para o host console existente: conta LOCAL padrão dedicada exclusivamente à instalação, distinta do operador admin; SID do usuário da conta (SAM), não SYSTEM, Administrators ou LocalService. Nenhum nome/SID é selecionado automaticamente. Não é nova autoridade de aprovação: SAM autentica o contexto; a política F0–F9 e o ato específico continuam decidindo autorização.

Preparação futura pelo owner: escolher nome local novo, criar via interface nativa de Contas do Windows com credenciais mantidas exclusivamente pelo OS/operador, sem privilégio administrativo ou grupos de backup/restauração; rejeitar colisão ou adoção silenciosa. Resolver nome para SID local e domínio/máquina factual por LookupAccountName/LookupAccountSid e consultar a conta; reter o SID binário e sua proveniência no owner/plano. A capacidade Windows de criar conta local padrão é documentada por [NetUserAdd](https://learn.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netuseradd); não foi chamada aqui.

A execução operacional deve usar logon nativo separado para essa conta e o MESMO executável/owner operacional, sem herdar handle/token administrativo. Verificar TokenUser real, grupos/privilégios, configuração exata e objetos de instalação; não aceitar nome configurado como prova. A conta recebe apenas acesso necessário aos artefatos públicos/runtime; key DACL exclui seu SID e não lhe dá acesso aos meios/segredos/recibos administrativos protegidos. Não dar acesso geral de escrita ao estado de autoridade. Confirmar readback e teste com o token efetivo; não há binding implementado desse contexto neste trabalho.

Avaliação de alternativas: SID próprio de serviço e conta virtual NT SERVICE\<nome> são mecanismos Windows reais; CreateServiceW documenta conta virtual sem senha fornecida e SERVICE_SID_INFO documenta SID no token do serviço. Porém buscas no código apps/src/tools não encontraram CreateService/OpenSCManager/StartServiceCtrlDispatcher/ServiceMain/ChangeServiceConfig. Não criar SCM host/supervisor só para obter SID. Conta dedicada serve à separação do processo console existente; futura hospedagem SCM, se autorizada, pode associar explicitamente essa conta ao serviço e consultar sua configuração/token. SID de grupo de serviço não deve ser confundido com TokenUser. [Contas de serviço](https://learn.microsoft.com/en-us/windows/win32/services/service-user-accounts), [CreateServiceW](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicew), [SERVICE_SID_INFO](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_sid_info).

Não negar logon interativo e simultaneamente presumir que o atual host console conseguirá iniciar; direitos/logon e ACLs mínimos dependem da hospedagem realmente qualificada. Não criar launcher de credenciais próprio nem guardar senha no Accelerator. Exclusão/recriação de conta produz novo SID e exige nova associação/qualificação explícita, nunca atualização silenciosa da DACL. A direção é concreta e implementável, mas conta, hosting e isolamento ainda NÃO foram implementados/qualificados nem alterados nesta tarefa.

## I. ED11 e decisão de compatibilidade

Custos adicionais a medir: TokenUser/grupos, buffers de SID/resolução SAM, handles/logon, descritores/DACL, owner480 bytes, borrows/planos imutáveis, worker/parent/child, heap/stack/CRT, KSP/kernel, recursos retidos e concorrência. wbadmin/VSS/BitLocker e restore são operações OS externas com custo próprio/host admission; não se transferem suas despesas para margem do processo nem se assume coexistência zero sem medir quiescência. Imagens/duas gerações têm orçamento de armazenamento separado não qualificado. Não instalar tarefas agendadas ou aumentar quotas.

Sem conflito lógico entre não exportar por NCrypt e recuperar o estado completo protegido do OS. Isso é uma hipótese de recuperação a provar, não garantia de migração de chave. Se o ensaio falhar, 3.6 exige DENY/alternativa governada. Não há autorização para relaxar export policy ou substituir backup.

F0–F9/P1, cláusula6 de custódia, perfil técnico 2.4/3.3/3.5–3.7, Model E e ED08/ED11 são preservados. Processo normativo pode aprovar o roteiro sem afirmar execução; o mecanismo só se torna qualificado após provas especificadas. Permanecem pendentes aprovação humana/oficial, valores concretos de instalação, ensaio de capacidade KSP/imagem/WinRE, binding da conta operacional, latest-state/fencing e ED11. Não registrar ACCEPTED nesta revisão. Não interpretar este procedimento como autorização para executar seus comandos.
