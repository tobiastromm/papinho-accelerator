# Resultado TPM reconciliado e lacuna de diagnóstico corrigida

Saída manual do proprietário para pedido357c33/approvald74171, boot237900/PID14512/criação134376111537279107. HEAD main f341985924d29870d8ff627ed37c8a0874ad6469 preservado. [Saída recebida](OWNER-MANUAL-OUTPUT.json) transcrita fielmente em JSON; [reconciliação tipada](RECONCILIATION.json).

## Resultado factual

A invocação foi **DENIED_BEFORE_TPM** no estágio OWNER_METADATA. `calls=0`, `context_opened=false`, `context_state=NOT_OPENED`, `proof=false`, `retained=false`, `observer_refs_retained=false`. Nenhuma sessão TPM foi criada pelo caminho reportado, nenhum Submit executado, nenhuma mutação NV ou leitura Q4. `context_close=false` não significa falha de fechamento: não havia contexto TBS aberto para fechar. As referências próprias do observer ainda não haviam sido adquiridas nesse primeiro guard. Isso não demonstra fechamento de todos os recursos internos CLR/Windows ou quiescência global/histórica.

O código usa uma única condição `!GetProcessHandleCount(...) || own>124`. Portanto **não é possível distinguir falha da API de contagem125+** pela saída original. Não foi impresso o número de handles nem o GetLastError. Não se pode afirmar que128 foi excedido, atribuir a falha à herança/CLR ou medir uma redução. Administração1024 não foi amostrada. Nenhuma observação histórica vira orçamento fresco.

A ordem real é identidade/elevation/impersonation, token e boot metadata, pins executável/aprovação/pedido, primeiro AdministrativeSample, confirmação, certificado, recibo CREATE_NEW e só depois abertura TBS. O bloqueio ocorreu antes da confirmação/recibo/TBS. A ausência atual de NATIVE-PROOF-ATTEMPT.ONCE.receipt é coerente com essa ordem, **não torna a autorização reutilizável**: a única invocação manual aprovada foi realizada e terminou DENY/no_retry. Aprovação e comando antigos permanecem históricos, não comandos ativos de retry. Não os sobrescrevemos.

## Correção técnica realizada

Novo [candidato isolado](NativeProofHost.DIAGNOSTIC-CANDIDATE.cs), derivado byte-exatamente da fonte aprovada; [patch focal](DIAGNOSTIC-ONLY.patch). O guard faz a mesma consulta, captura imediatamente sucesso/erro e contagem em quatro campos escalares, separa NATIVE_OBSERVER_QUERY_FAILED de NATIVE_OBSERVER_HEADROOM e imprime contagem nullable/erro/limite124 no resultado. Nenhuma consulta nativa adicional, enumeração, fechamento antecipado ou alteração de128/1024. A reserva124 continua reservando quatro referências antes do observer. Ausência de headroom continua DENY.

O diagnóstico guarda somente o último estado da consulta alcançada; não afirma pico nem explica a origem individual dos handles. Quatro campos escalares gerenciados foram adicionados; custo total CLR/layout/transitivo não foi medido ou tratado como zero/crédito. A serialização final ocorre após a decisão. Nenhum owner/serviço/transporte/IPC novo foi instalado. O código operacional C/root/launcher/pins históricos não mudou neste slice. L1 permanece ACCEPTED e sua qualificação anterior continua válida.

## Qualificação e hashes

- Guard extraído da fonte real e compilado em fixture sem APIs TPM:405 verificações por configuração Debug/Release,810 total PASS. Cobertura0..132, limite124/125, cinco códigos de erro incluindo retorno falso com erro0 e preservação da primeira falha. Sem chamadas reais GetProcessHandleCount nesse fixture: os retornos são simulados.
- Candidato completo Debug/Release: builds /warn:4 /warnaserror+ PASS, zero warnings; `--selfcheck` PASS_NO_TPM nas duas configurações. **Nenhum modo nativo foi executado pelo agente.**
- Dependências criptográficas originais copiadas sem alteração; não repetimos728/1092 checks anteriores sem alteração criptográfica. Os primeiros erros de link C# por dependências omitidas do comando foram corrigidos incluindo os três arquivos canônicos existentes; log inicial preservado.
- Executável/código/pedido/aprovação originais continuam com os quatro hashes anteriores, conferidos. [Hashes completos](FINAL-HASHES.json) e [builds/testes](BUILD-TEST-RESULTS.json).

## Novo pedido candidato e fronteira operacional

[Pedido corretivo candidato](NATIVE-POSSESSION-CORRECTIVE-REQUEST.DRAFT.json) SHA256:

`4aa1d74fc4dce21e17ae057b8b2c72ddcefc00a5a3a225b23c6cea24a78fe1d8`

A hipótese discriminante é obter o resultado da API/erro e a contagem do primeiro gate; se todos os mesmos guards frescos passarem, concluir a mesma única prova transitória delimitada. O novo executável é bound nesse novo pedido. Não há aprovação, comando executável autorizado, receipt criado, identidade Q4 reativada ou aumento de quotas. O nome candidato TPMPROOF-HEADROOM-20261027-01 é rótulo inerte, não intent/admissão.

É necessária **nova decisão explícita do proprietário para esse pedido exato**, após a tentativa negada. Só então registrar aprovação nova e preparar comando manual com hash literal. Não usar approvald74171 no novo candidato nem repetir o comando antigo. Se um count acima124 for observado, mantemos DENY e a próxima correção exigirá atribuição factual de referências; não autoriza relaxar a reserva ou fechar dependências para fabricar PASS.

A correção resolve a lacuna de observabilidade demonstrada; **não resolve nem mede a causa da pressão de handles**. Esse limite é explícito para evitar alegar uma otimização sem evidência.

## ED11 e caminhos paralelos

E21 permanece PENDING: posse não alcançada, revogação UNKNOWN, geração do dispositivo NOT_ESTABLISHED, custódia/NV durável/readback/disposição ainda pendentes. Zero efeito deste piloto não desfaz efeitos históricos Q4 nem libera reservas/refund. E17/E18 continuam exigindo ciclo independente elegível de identidade/acesso/readiness/Stop; E19/E20 continuam dependendo de D4/D5 justificados e composição real, sem esperar o TPM para preparação. Não há nova hipótese que justifique repetir suas campanhas inalteradas. E22 continua derivado;4 demonstrados/12 limitados/6 pendentes, ED11/Phase6 PARTIAL. CURRENT/assinatura/recuperação autoritativa DENY.

NEXT=OWNER_DECISION_FOR_EXACT_CORRECTIVE_TPM_REQUEST. NO_RETRY_OLD_APPROVAL. NV/Q4/Start/Stop/SCM/ACL/cleanup/refund=NONE. UNKNOWN/reservas/WIP/históricos preservados.
