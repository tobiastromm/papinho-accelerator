# Console forwarding — closeout e preparação de sucessora LAB

## Conclusão

Patch preservado e qualificado somente para o protocolo atual ASCII/UTF16 hexadecimal. SHA256 do launcher: `deb0ab304110e5675057261df09e2057199db11884be5718c77a86ca1d382cec`. Todos os 54 artefatos indexados da qualificação anterior foram conferidos sem divergência. Não há alteração de encoding, fonte funcional, binário ou contrato accepted nesta tarefa. Unicode arbitrário em CP850, transcript, Information Stream e UI de host personalizado continuam fora da equivalência demonstrada. Redução de handles no LAB não comprovada.

**Nova tentativa: viável apenas condicionalmente, ainda não criada nem autorizada.** A primeira barreira é normativa: N0 fixa a tentativa75330d11cc7e4123ae62eadc9a59c2f5 e declara que mudança de tentativa/candidato/config/boot exige nova decisão exata. B0/B4 e B4.B.5 preservam a reserva e cargas retidas sem reutilização antes de fechamento factual. Uma nova aprovação operacional isolada não amplia essa exceção nem libera a reserva histórica.

## Efeitos antigos — leitura atual e histórico separados

Consultas sequenciais, não atômicas, de arquivos e SCM foram executadas sem chamar launcher, preflight, Start/Stop ou setters. Logs brutos sc-* e current-readonly-observations.json preservam as consultas. A consulta inicial do caminho manual histórico não era o intent do owner compilado; a leitura de server_service_admission_win32.inc resolveu psa_path e a consulta correta confirmou o consumo. Não interpretar aquele nome ausente como intent ausente. A leitura protegida que o sandbox não permitiu foi realizada somente por leitura fora dele, sem mudança de ACL.

| Objeto | Disposição atual |
|---|---|
| Intent canônico |103 bytes; ID `0000000000000000002000000023fb51`; SHA256 `9e54d105d484c2201b5b280f9354038e9373f355f137c7e8a0debbca1ca45895`; conteúdo CONSUMED/NO_RETRY_NO_RECOVERY/UNKNOWN_UNTIL_FACTUAL_CLOSE|
| Imagem antiga |434688 bytes; ID `0000000000000000000a00000024dfd7`; SHA256 `eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b`; preservada, sem atributo reparse observado|
| Sink/arm antigo |1024 bytes; ID `0000000000000000001000000024e0ad`; SHA256 `240d658c0cd8fdc2fdd6980f81c3a6419b1c8427876b766a664d686ebdd661e0`; mesmo conteúdo do arm anteriormente validado, sem append final|
| SCM ImagePath |`"C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server_lab_host_ntstatus.exe" service host`|
| Serviço |PapinhoAcceleratorLab20261023; STOPPED/PID0;1077/specific0; DEMAND_START; conta virtual NT SERVICE/PapinhoAcceleratorLab20261023; SID UNRESTRICTED|
| Recovery/triggers |Nenhuma ação listada; reset0; mensagem/comando vazios; non-crash FALSE; nenhum trigger registrado nas consultas bem-sucedidas|
| Referências/quiescência |close0 histórico apenas para referências próprias do qualificador; quiescent0 preservado. Quiescência histórica UNPROVEN; nenhuma liberação, refund ou cleanup inferido|

Owner/DACL de intent e sink continuam protegidos e coerentes com os registros anteriores nas consultas Get-Acl; SDDL é observação descritiva, não revalidação operacional RAW1. Os objetos e ImagePath não foram adotados como novos. Histórico: starts0/stops0, intent_consumed1/effect2/diagnostic_created1 e primeira falha1816. Nenhum NTSTATUS psr_prepare foi capturado nessa tentativa.

## Menor caminho e isolamento obrigatório

1. Preparar proposta normativa focal de sucessão, preservando integralmente decisões antigas. Deve delimitar nova identidade de tentativa/operação, nomes novos de intent/imagem/sink/arm, serviço existente, host/boot, configuração e hashes novos efetivos. Não escolher/criar GUID ou nome aqui. Reaprovar explicitamente o risco N7 para a nova tupla. Reconciliar N0, N8, B0/B4.B.6 e seus pins; não criar exceção geral ou novo owner.
2. Antes de qualquer nova reserva ou operação, decidir a disposição das cargas/UNKNOWN antigos sob Model E/B4.B.5. Não liberar a reserva histórica por STOPPED/PID0 ou por novo nome. Se a disponibilidade canônica não puder ser comprovada sem reutilizar cargas retidas, permanecer DENY; eventual revisão dessa condição exige decisão normativa explícita, não mera nova aprovação de Start. Esta tarefa não oferece mecanismo de recuperação nem nova reserva paralela.
3. Após aprovação normativa e autorização de implementação, integrar coordenadamente a nova identidade no owner canônico (psa_path/registro), ciclo/confirmadores/planos, launcher, dossier/contexto protegido e recorder do serviço. server_service_psr_file_win32.inc fixa psrd_attempt/psrd_leaf e texto da tentativa; não basta apontar ImagePath para outro nome. Não criar recorder ou mecanismo de autorização. Renomear somente o arquivo .diag deixaria binding divergente.
4. Rebuild dos quatro targets e testes negativos pertinentes, novos hashes/pins e revisão oficial. O hash do binário instrumentado provavelmente muda ao alterar a identidade incorporada; calcular após build, nunca inventar. Preservar o patch de console como delta único em relação ao sucessor; manter todas as confirmações Write-Host e Stop/retention existentes.
5. Novo dossiê protegido deve vincular tentativa, plano, política e binários novos. Evidências estáticas Windows podem ser referenciadas somente após revisão de aplicabilidade e revalidação do mesmo objeto; os arquivos evidence/approved antigos não são autorização da sucessora. Não copiar/relabelar claims consumidos. Qualquer nova colocação protegida requer escopo e autorização próprios.
6. Pacote exato posterior deve fixar serviço/conta/SID/recovery/triggers/config/parent originais, novos nomes exclusivos, sink/arm, intent novo, hashes e autorização. Exigir consulta nativa pré-consumo dos nomes pelo pai original e CREATE_NEW/FILE_CREATE final. Consulta não reserva atomicamente: colisão posterior consome a nova tentativa e preserva efeitos. Não adotar nem substituir imagem/sink anteriores.
7. Somente após revisão final solicitar aprovação exata para operação manual: um consumo, criação exclusiva, um rebind expressamente delimitado do serviço existente, um Start, até um Stop seguro quando aceito, observação e retenção. O ImagePath inicial já instrumentado antigo deve ser validado como estado prévio e qualquer troca futura explicitamente autorizada; não restaurá-lo automaticamente. Não fornecer comando positivo executável antes dessa aprovação.

## Plano de recursos e Stop

O resultado1022→1032 e qualificador124/128 continua histórico. O fixture não concede oito handles de margem, crédito ou redução garantida. Nova admissão deve observar PowerShell e auxiliar separadamente, somando apenas esses dois no orçamento administrativo1024; qualificador e serviço têm128 cada. Incluir todos os custos obrigatórios, quatro papéis, UNKNOWN e reservas retidas; sem quinto processo/monitor durante admissão.

Preservar geração inicial e final, idade global60s e final1s desde a observação obrigatória mais antiga, headroom5/9GiB e pisos4/8GiB, demais limites e NONE_GRANTED. Antes de consumo e antes do Start, exigir orçamento válido; medir novamente nos pontos existentes após cópia, sink e rebind, registrando identidade/geração/estágio. Limite excedido ou medição indisponível nega novos efeitos; depois do consumo preserva tentativa e efeitos. Não introduzir nova geração/retry para alcançar PASS. Nenhuma margem histórica mínima é garantia contra crescimento entre amostras.

Stop solicitado, aceito, concluído, quiescência e referências fechadas permanecem fatos separados. Após efeito, suprimir efeitos novos e usar somente o Stop já autorizado quando legalmente aceito. START_PENDING pode não aceitar controles. Perda do console, timeout ou saída de processo mantém UNKNOWN; nenhum kill, mutex forçado, rollback, exclusão ou cleanup automático.

## Aprovações necessárias e riscos

Primeiro: decisão focal normativa de sucessão e tratamento das cargas retidas. Depois: implementação/qualificação e eventual colocação protegida. Por último: aprovação de operação exata manual, com hashes literais, efeitos e confirmações contemporâneas. Este relatório não é pedido para executar nem cria registro EXACT-OPERATION-APPROVED.

Riscos residuais: N7/associação e imagem mapeada não autenticadas; kernels/cache/stack e picos entre amostras UNKNOWN; custo administrativo pode voltar a ultrapassar1024; consumo anterior ao Start; colisão posterior à consulta; falha parcial de rebind/sink; indisponibilidade de Stop; ausência de NTSTATUS se serviço/sink falhar cedo; reserva histórica não liberada. Nenhum desses riscos é convertido em PASS.

CURRENT_TASK_COMPLETED=YES (revisão/preparação concluída; nova operação continua bloqueada).
CONSOLE_FORWARDING=QUALIFIED_FOR_CURRENT_PROTOCOL
HISTORICAL_ATTEMPT=CONSUMED_PRESERVED
NEXT_ATTEMPT=NOT_CREATED_DENY_PENDING_NORMATIVE_SCOPE_AND_RETAINED_CHARGE_DISPOSITION
RESOURCE_HEADROOM=UNPROVEN
QUIESCENCE=UNPROVEN
START_EXECUTED=NO; STOP_EXECUTED=NO; SCM_MUTATIONS=NONE; ACL_MUTATIONS=NONE; PROTECTED_WRITES=NONE
ED11_TOTAL_STATUS=PARTIAL; CURRENT_PUBLICATION=DENY; SIGNATURE_AUTHORITY=DENY; AUTHORITATIVE_RECOVERY=DENY
NEXT=NEW_LAB_ATTEMPT_EXACT_APPROVAL_PREPARATION
