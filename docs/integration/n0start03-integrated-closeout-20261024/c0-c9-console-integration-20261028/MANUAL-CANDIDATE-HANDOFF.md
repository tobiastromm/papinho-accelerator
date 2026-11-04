# Próxima prova TPM: candidato C0–C9

PREPARED_NOT_APPROVED_NOT_EXECUTED. C0–C9 foi aceita normativamente; isso NÃO autoriza a prova.

Pedido exato [NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json](manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json), SHA256 `b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b`.
Host C Selected Release [NativeTpmProof.exe](manual-candidate/NativeTpmProof.exe), SHA256 `59c67f6d81d88c49c1d676bde2553aa8a843a1925c76a33deb14acbab3424898`.
[Demais pins](MANUAL-CANDIDATE-PINS.json); certificado público DER965f56aa... inalterado. [Registro normativo](../../../adr/DECISION-tpm-proof-explicit-console-host-binding-20261028.md). O parser C real aceitou549nodes/19651bytes sem executar TPM.

## Aprovação ainda necessária

Uma nova decisão explícita do proprietário para UMA invocação manual conforme os bytes deste pedido. As aprovações anteriores, inclusive4a5f, estão usadas. Não existe novo OWNER-APPROVED-NATIVE-PROOF.json no candidato. Não foi preparado comando operacional com hash de aprovação inventado.

Após decisão específica: registrar UMA aprovação kind OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY com request/exe exatos, one_manual_execution=true e ack_unknown_revocation/ack_no_hardware_admission/ack_internal_tpm_accounting_unknown=true. Calcular SHA literal do novo registro; conferir novamente todos os artefatos e dependências e gerar a linha --owner-manual-proof com hashes completos. Não executar automaticamente.

## Procedimento futuro

Windows PowerShell5.1 x64 já elevado e operador/host/boot atuais aprovados; sem wrapper, pipeline, UAC child ou alteração artificial de console. A descoberta usa o PAI REAL do próprio processo novo, dois clientes desse console e um auxiliar distinto; não usa seed17204/20976/13944 nem os PIDs dos testes como pins. Novo console implica nova identidade factual, nunca adoção silenciosa de histórico.

O programa apresenta C0_BIND com PID/criação de pai e auxiliar, HWND metadado e risco UNPROVEN. Só confirmar a linha INTEGRAL que o programa exibir: PROVE_EK_SESSION, o hash literal deste pedido e os quatro valores LIVE de PID/criação do pai/auxiliar. Não preencher esses valores antecipadamente nem copiar confirmação antiga. Eles só existirão na execução manual.

Antes de TBS, exigir token atual/elevado/sem impersonation/LUID continuidade, originais PID/criação/liveness/sessão e tokens de papéis, class49 repetida no pai/native com perfil válido e auxiliar distinto, arquivo Windows original/volume/ID/size/hash/OWNER-GROUP-DACL para parent/helper/ABI, exact client census,124/128/1024 e demais obrigações. Qualquer divergência => DENY;982 histórico não concede folga futura.

Um contexto TBS, uma sessão HMAC transitória, no máximo12Submits diretos. Nenhum AK/CreatePrimary/NV/hierarchy/Clear/Q4/LAB/Start/Stop/SCM/ACL/CURRENT/assinatura/recuperação. Internos TBS/kernel/transitivos e unsampled peak UNKNOWN. Um resultado matemático não autentica geração física ou revogação/custódia.

Escape/Stop entre chamadas síncronas; não há cancelamento garantido dentro de chamada. Incerto => freeze e referências/dependências originais retidas no processo vivo, sem kill/retry/FlushContext adivinhado. Não apagar receipt/resíduos. A invocação usa a aprovação MESMO SE negar antes do CREATE_NEW administrativo; sua ausência não permite retry e esse marcador não é ledger Q4 resistente a rollback.

Enviar saída INTEGRAL NATIVE_API_COUNTS, C0_BIND e JSON, inclusive negativa/UNKNOWN. Reconciliação verificará first/stage, artefatos originais, papéis, amostras/contagens/calls, challenge/HMAC/Name/session_end/context_close/retained; nenhuma conclusão antecipada de E21/ED11. Não são necessárias novas coletas equivalentes de topologia antes desta decisão: a integração nova e testes estão no pacote; os gates reais serão frescos na futura invocação autorizada.

E21=PENDING; ED11/Phase6=PARTIAL; pedido TPM aguarda aprovação operacional própria.
