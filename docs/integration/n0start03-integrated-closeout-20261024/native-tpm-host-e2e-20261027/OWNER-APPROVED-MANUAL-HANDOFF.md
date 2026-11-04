# Aprovação nativa TPM registrada — handoff manual definitivo

TASK=PACC-PHASE6-ED11-NATIVE-TPM-OWNER-APPROVAL-AND-MANUAL-HANDOFF
Data2026-10-27. Preparação autorizada concluída; execução exclusivamente pelo proprietário.

## Registro e conferência

A autorização explícita do proprietário no item1 do TASK foi registrada UMA vez em [OWNER-APPROVED-NATIVE-PROOF.json](manual-candidate/OWNER-APPROVED-NATIVE-PROOF.json), kind OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY. SHA256 `4a5f2e001feb29a0607333c40dd46e413b3e75a3957b1c527d7090b2eab1376c`.

Pedido exato SHA256 `2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3` e executável C nativo `95446475cad9b4666379b75d4b850b1a77e4fdf232d240d937416593387c6857` conferidos. [Registro e54 verificações](OWNER-APPROVAL-REGISTRATION-AND-PINS.json): fontes/artefatos/dependências on-disk/históricos correspondem aos pins. Nenhuma divergência técnica encontrada; builds/testes qualificados reutilizados, sem rebuild. Head/main f341985924d29870d8ff627ed37c8a0874ad6469 e WIP preservados. Não houve edição de fontes, binaries, pedido ou pins históricos.

As duas aprovações gerenciadas anteriores continuam históricas e usadas, sem reutilização. Receipt do novo candidato ausente nesta conferência estática. Isso não restaura autorização depois de invocação negada antes do receipt. Somente esta nova aprovação exata permite UMA invocação manual.

Host e coletor de checkpoints estão no mesmo executável C; não há launcher/coletor PowerShell adicional a executar. Identidade pública EK/certificado, protocolo salted HMAC e limites não mudaram. Não se criou challenge antecipado: salt e caller nonces novos serão gerados no processo manual, vinculados ao token/contexto atuais e ao Name qualificado.

## Comando definitivo

Windows PowerShell5.1 x64 já elevado, console do operador autorizado. Não abrir outro supervisor/pipeline/launcher, não contornar UAC, não alterar globalmente herança/flags para obter contagem favorável. O executável C não exige Set-ExecutionPolicy.

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\manual-candidate\NativeTpmProof.exe" --owner-manual-proof 95446475cad9b4666379b75d4b850b1a77e4fdf232d240d937416593387c6857 4a5f2e001feb29a0607333c40dd46e413b3e75a3957b1c527d7090b2eab1376c
```

Quando solicitado, digitar exatamente:

```text
PROVE_EK_SESSION 2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3
```

O guard real requer exatamente pai e prova como clientes e processo de janela de console distinto; a topologia modelada do fixture não é autorização para ignorar divergência. Os gates frescos não foram executados pelo agente e não estão declarados PASS: operador SID fixado, sem impersonation, LUIDs TokenId/AuthId/ModifiedId, host/boot237900/start fixados, PID/criação, objetos originais, hash/volume/FileID/tamanho, recursos124/128/1024 e confirmação ainda serão verificados pela única invocação manual. Qualquer divergência resulta DENY sem retry.

## Limites, STOP e retenção

No máximo um contexto TBS e uma sessão HMAC transitória, até12Submits diretos: GetCapability1, ReadPublic até9, StartAuthSession1, GetRandom1. Sem AK/key creation, NV, hierarchy, Clear, Q4, serviço, SCM/ACL ou autoridade produtiva. Internos do TBS e picos não amostrados UNKNOWN. O pico91 é do fixture com endpoint e autoridade modelados, não garantia de capacidade real.

Pré-observador<=124, processo próprio<=128 e administração pai+console<=1024, sem aumento/transferência/crédito. Escape solicita Stop entre chamadas síncronas; não promete cancelamento interno. Primeira falha preservada. Sessão sem fim autenticado ou fechamento incerto retém referências originais e dependências no processo vivo. Não matar, adivinhar FlushContext, repetir, limpar ou liberar reservas. Host perdido mantém UNKNOWN.

O CREATE_NEW local rejeita segunda execução ao alcançar o receipt já criado; é evidência administrativa suplementar, não ledger Q4 resistente a rollback. Aprovação é de uma invocação apenas mesmo se o guard negar antes de criar esse arquivo. Não usar sua ausência como permissão de retry.

## Reconciliação preparada

Devolver saída INTEGRAL: NATIVE_API_COUNTS, JSON e mensagens UNKNOWN, inclusive se negar antes de abrir contexto. Será conferido request/approval/live PID/criação, query_succeeded/count/error, amostras/pico128 e administração1024, contexto aberto/fechado, comandos efetivamente enviados, Name/challenge/HMAC, primeira falha, session_end, controlled_refs e retenção. Nenhum novo resultado nativo foi recebido neste TASK; não se inventa PASS ou posse.

Revogação/CRL permanece UNKNOWN; prova matemática não autentica geração física nem concede custódia/provisionamento/NV/Q4. E21=PENDING, ED11/Phase6=PARTIAL. E17/E18 e D4/D5/E19/E20 seguem independentes. Próximo gate: proprietário executa este comando UMA vez e envia a saída integral; depois reconciliação técnica autônoma, sem retry ou nova autoridade inferida.

CURRENT_TASK_COMPLETED=YES_PREPARATION_ONLY
OWNER_APPROVAL=REGISTERED_EXACT_ONE_MANUAL_INVOCATION
NATIVE_TPM_PROOF=NOT_EXECUTED
Q4=NOT_EXECUTED
NV_MUTATIONS=NONE
SCM_ACL_MUTATIONS=NONE
CURRENT_SIGNATURE_RECOVERY=DENY
