# Handoff final — UMA execução manual read-only aprovada

Pedido integral e hashes fontes/artefatos/autoridades conferidos conforme exact-readonly-approval-review.json; aprovação real do owner registrada exclusivamente em EXACT-READONLY-APPROVED.json, SHA256 `ea272d2a4aa33f42f1cb1c12c8418fdd793fde1e529067325bd9ab8b430dcb7e`. Aprovação NÃO executada pelo Codex; nenhum preflight/SCM/serviço consultado ou executado. Código/launcher/pins não modificados nesta etapa. Draft original preservado como histórico; registro atual o sucede apenas quanto à autorização exata read-only.

Usar um único Windows PowerShell5.1 x64 NOVO, aberto manualmente já como administrador via mecanismo normal do Windows. Não criar outro powershell.exe dentro dele. O processo não pode ter carregado PaccFileAcl/reader/placer anteriores: Lock-QualifierPath exige FRESH_OPERATOR_PROCESS_REQUIRED. Não reutilizar a janela dos readers anteriores. Não trocar boot, operador ou pins. GPO/política impeditiva => STOP, sem bypass de segurança por tentativa.

Comando final, uma vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Prepare -Profile N0START03ReadOnly -ExpectedScriptSha256 a96ad14d8ded2d9033599579462af154046fdee738e2d804512e6c40779ec009 -ExactApprovalSha256 ea272d2a4aa33f42f1cb1c12c8418fdd793fde1e529067325bd9ab8b430dcb7e
```

Confirmações devem ser as exibidas nesta execução: PREPARE_N0_READ_ONLY com tentativa03/PID/criação contemporâneos e BIND_BD_READ_ONLY com desafio nativo novo. Não copiar valores de -02 ou outras coletas. PID/token/métricas futuros não preenchidos no registro. N7 aceita só a incerteza delimitada, não cria prova de console ou imagem mapeada.

A operação permite somente revalidação/consultas protegidas, SCM read-only, mutex/censo/métricas/confirmacões e fechamento/retenção. Resultado em console; enviar toda saída. Expectativa limpa: IG_PREPARED_READ_ONLY, IG_FINAL first0/close0/starts0/stops0/intent_consumed0/effect0/diagnostic_created0 e N0_READ_ONLY_EXIT0. Falha de recurso/identidade/pin/SCM/prova/freshness => DENY, sem corrigir/repetir. first/close/retention devem ser preservados como ocorrerem, sem converter erro em sucesso.

F10 somente se o launcher anunciar retorno factual/fechamento do qualificador; RETAINED/UNKNOWN => preservar janela/owner/evidências, não matar/fechar/forçar mutex. Sem retry, Start/Stop/rebind/intent/imagem/sink/arm/ACL/quotas/crédito/CA/certificado/bootstrap/CURRENT. ED11PARTIAL, orçamentoNOT_GRANTED, positivoDENY e autoridadesDENY permanecem. O resultado desta leitura não concede autorização positiva futura.
