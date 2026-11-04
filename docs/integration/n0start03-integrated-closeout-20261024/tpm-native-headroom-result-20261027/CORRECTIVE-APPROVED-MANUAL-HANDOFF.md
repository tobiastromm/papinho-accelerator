# Prova TPM corretiva — autorização registrada e procedimento manual final

TASK PACC-PHASE6-ED11-TPM-CORRECTED-PROOF-AUTHORIZATION. A autorização explícita do proprietário foi registrada separadamente da aprovação anterior. Uma única invocação manual, condicionada integralmente ao pedido e guards frescos. Nenhuma execução nativa automática, retry, consulta TPM ou operação persistente foi realizada.

## Conferência

Pedido corretivo lido integralmente e SHA256 exato conferido: `4aa1d74fc4dce21e17ae057b8b2c72ddcefc00a5a3a225b23c6cea24a78fe1d8`. A cópia com basename fixo que o host lê tem os mesmos bytes. Executável, fonte, seis dependências, certificado público e quatro artefatos históricos:15 pins PASS. [Verificação pré-registro](CORRECTIVE-AUTHORIZATION-PREWRITE-VERIFICATION.json). O host é standalone: qualificador C/launcher operacional não participam desse procedimento e não tiveram pins alterados. Não repetimos quatro builds/810 checks já qualificados, pois nenhum código mudou.

Registro novo [OWNER-APPROVED-NATIVE-PROOF.json](OWNER-APPROVED-NATIVE-PROOF.json), SHA256 `df36112d56b152f2f91e5609c85128770d47c809e8f49567a76d44e219cf7331`. Executável SHA256 `58e70df53c9beec30bc23f85d3dd936eabeedc99193cf41d907d51eef0a4270f`. [Registro de vinculação](CORRECTIVE-AUTHORIZATION-REGISTRATION.json). Tentativa candidata `TPMPROOF-HEADROOM-20261027-01_INERT` vinculada ao pedido; não é identidade Q4/intent reutilizado.

A autorização anterior d7417145 foi usada na invocação bloqueada e continua histórica; não se tornou disponível pela ausência de recibo. A aprovação nova foi criada exclusivamente, sem sobrescrever a anterior. Recibo corretivo ainda ausente na revisão: não é reserva ou prova de admissão futura. Não há novo intent/objeto NV/readback Q4.

## Pré-condições e única execução

Usar Windows PowerShell5.1 x64 já elevado, console interativo existente do operador `S-1-5-21-785699550-1440458426-1682014700-1108`. Não usar pipeline/redirecionamento de entrada ou terminal aninhado. O host exige pai+filho como os dois clientes do console e processo distinto da janela; associação do console permanece metadata, não autenticação completa.

Host/boot continuam fixados no pedido: boot237900/início2026-10-26T21:35:16.5000000Z; MachineName SHAfc2d00eb95bdc15ee7f56c7abd78f1c88670ba8359a0786ab4b49523fd91f482 e Event Computer SHA2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff. TokenId/AuthId/ModifiedId, ausência de impersonation e PID/criação são vinculados/revalidados pelo código. Essas condições não foram declaradas frescas pela revisão documental; precisam passar no momento manual. Divergência => STOP, sem atualizar pins.

Executar exatamente uma vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\tpm-native-headroom-result-20261027\NativeProofCandidateRelease.exe" --owner-manual-proof 58e70df53c9beec30bc23f85d3dd936eabeedc99193cf41d907d51eef0a4270f df36112d56b152f2f91e5609c85128770d47c809e8f49567a76d44e219cf7331
```

Quando solicitado, digitar exatamente:

```text
PROVE_EK_SESSION 4aa1d74fc4dce21e17ae057b8b2c72ddcefc00a5a3a225b23c6cea24a78fe1d8
```

Não é necessário Set-ExecutionPolicy para esse EXE. Não usar o executável/aprovação antigos e não recriar desafios. Mesmo DENY anterior à confirmação/TBS encerra esta invocação autorizada: não repetir sem decisão nova.

## Limites e STOP

Mesmos128 handles nativos, guard pré-observer124 para quatro referências reservadas e1024 compartilhados administrativos. Sem concessão de orçamento, créditos ou teto universal de pico. Consulta de contagem falha => NATIVE_OBSERVER_QUERY_FAILED, contagem desconhecida e código nativo separado. Consulta válida com count>124 => NATIVE_OBSERVER_HEADROOM. Não fechar referências obrigatórias nem aumentar o limite para passar.

Se admitida: uma sessão HMAC salted transitória sobre EK existente exata; até12 Submit diretos (GetCapability<=1, ReadPublic<=9, StartAuthSession<=1, GetRandom<=1). Sem AK, NV, hierarquia, Clear, Q4, Start/Stop, SCM/ACL, CURRENT, assinatura ou recuperação. CRL/revogação UNKNOWN explicitamente preservada; prova matemática não estabelece geração/localidade física, dispositivo íntegro ou admissão hardware.

Escape é observado antes de chamadas síncronas; não há garantia de cancelamento dentro delas. Qualquer erro/inconsistência/falha de fechamento => primeira falha preservada, nenhum retry/fallback. Em retained=true, preservar o processo e as referências/dependências originais; informar a saída sem kill/remoção do recibo/FlushContext adivinhado. Perda abrupta do host deixa UNKNOWN. Não fazer limpeza/refund.

## Evidência e reconciliação posterior

Enviar toda a saída original, incluindo JSON final e eventual mensagem UNKNOWN. Reconciliar observer_query_attempted/succeeded, observer_own_count (nullable), observer_query_error, first/first_stage, limite124/128, amostras administrativas1024, live binding, calls, public_area_name, proof, session_end, context_opened/state/close e retenção. Campos desconhecidos não viram zero. Context_close=false com NOT_OPENED não é falha de close; contexto aberto/fechamento incerto é outra condição.

Sucesso limitado exige prova HMAC válida, identidade/Name exatos antes/depois, first=NONE, fechamento factual e retained=false. Não prova picos não amostrados, quiescência global ou custos internos/transitivos. Nenhuma disposição Q4 ou provisionamento decorre do resultado. [Template](CORRECTIVE-NATIVE-RESULT.TEMPLATE.json).

E21=PENDING, ED11/Phase6=PARTIAL. Próximo gate é esta única execução pelo proprietário e sua reconciliação; nenhuma nova autorização técnica intermediária é necessária. E17/E18 e D4/D5/E19/E20 seguem independentes; históricos/UNKNOWN/reservas permanecem. L1 ACCEPTED preservada. CURRENT/assinatura/recuperação autoritativa DENY.
