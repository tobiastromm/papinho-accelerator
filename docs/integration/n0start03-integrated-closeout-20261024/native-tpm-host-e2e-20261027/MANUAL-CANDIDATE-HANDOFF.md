# Pedido candidato: prova EK no host C nativo

Estado: PREPARED_NOT_APPROVED_NOT_EXECUTED. Este arquivo não autoriza execução.

Pedido exato: [NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json](manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json).
SHA256 `2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3`.
Executável Selected Release copiado byte a byte: [NativeTpmProof.exe](manual-candidate/NativeTpmProof.exe), SHA256 `95446475cad9b4666379b75d4b850b1a77e4fdf232d240d937416593387c6857`.
Certificado público DER: `965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761`.
[Demais pins](MANUAL-CANDIDATE-PINS.json). O parser/contrato C real validou o pedido:450 nodes,14032bytes. Não existe OWNER-APPROVED-NATIVE-PROOF.json neste diretório.

## Decisão necessária

Uma NOVA aprovação específica do proprietário para uma única invocação manual deste pedido. As aprovações dos dois hosts gerenciados foram usadas; ausência de receipt antes do guard não as restaura. O identificador do candidato é inerte, não novo intent/identidade operacional Q4.

Após aprovação: registrar separadamente o kind OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY, request_sha256 e executable_sha256 exatos, one_manual_execution=true e os três reconhecimentos explícitos de revogação UNKNOWN, ausência de admissão de hardware e accounting interno UNKNOWN. Calcular o SHA256 literal do novo registro; revalidar os três arquivos e os pins compilados. Só então gerar a linha final --owner-manual-proof com ambos os hashes literais. Não foi criado registro aprovado nem comando operacional ativado neste pacote.

## Procedimento futuro condicionado

Usar Windows PowerShell5.1 x64 já elevado e o mesmo operador/host/boot237900 fixados. Não contornar UAC. Mudança de boot/token/artefatos exige revisão; não editar pins durante a execução. A execução manual será feita pelo proprietário uma única vez, sem retry.

O console deve satisfazer o guard REAL: exatamente pai e prova como clientes e um processo de console distinto. No console privado do ensaio, HWND pertencia ao cliente; o guard real negou corretamente. O ensaio positivo usou papéis administrativos modelados, SEM comprovar1024. Não alterar flags globais/herança/topologia para fabricar contagem. Se o ambiente manual não satisfizer o guard, DENY; não repetir para procurar ambiente favorável sob a mesma aprovação.

Confirmar exatamente `PROVE_EK_SESSION 2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3` quando solicitado pelo executável aprovado. Escape solicita Stop entre chamadas síncronas; não promete cancelamento interno. O receipt local CREATE_NEW é apenas evidência administrativa suplementar, não ledger Q4 resistente a rollback. Qualquer invocação usa a aprovação mesmo que ainda não crie receipt.

## Efeitos e STOP

No máximo um contexto TBS, uma sessão HMAC transitória com EK EXISTENTE e12 Submits diretos: GetCapability1, ReadPublic até9, StartAuthSession1, GetRandom1. Nenhum AK/CreatePrimary/NV/hierarchy/Clear/FlushContext. A contagem não abrange chamadas internas opacas do TBS.

Exigir pré-observador<=124, próprias<=128, pai+console<=1024, identidade PID/criação e LUIDs, nenhum impersonation, hash/ID/objeto original íntegros. Nenhum crédito/concessão de orçamento. Divergência, Stop, erro ou fechamento incerto congela aquisições/comandos; primeira falha preservada. Sessão sem fim autenticado/contexto incerto retém referências e dependências no processo vivo. Não matar, repetir ou adivinhar FlushContext; perda do host deixa UNKNOWN. Não limpar receipt/resíduos.

Devolver a saída integral NATIVE_API_COUNTS e JSON e qualquer mensagem UNKNOWN. Conferir query/count/error, fases/limites, Name, prova, context/session end, retained, artefatos e fechamento. Mesmo um PASS matemático não autentica geração física, revogação, custódia NV ou readback Q4.

Provisionamento=NOT_READY. E21=PENDING; ED11/Phase6=PARTIAL. Proibidos Q4/Start/Stop/SCM/ACL/CURRENT/signature/recovery/refund/cleanup.
