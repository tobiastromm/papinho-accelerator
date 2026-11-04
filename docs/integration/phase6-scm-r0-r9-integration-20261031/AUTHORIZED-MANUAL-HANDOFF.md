# Autorização registrada — UM registro SCM parado manual

Task PACC-PHASE6-ONE-STOPPED-SCM-AUTHORIZATION, 2026-10-31. Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469. O proprietário autorizou explicitamente UMA execução MANUAL do pedido original SHA256 `7eb9e6912a3d2c4b3a4408d176bfcd356e511da3a8bfc240942ce82a7ad03a79`, condicionada aos contratos e gates frescos. Sem execução automática ou retry. Registro em OWNER-STOPPED-SCM-EXECUTION-AUTHORIZATION.json; é evidência documental do ato recebido, não novo grant/ledger/loader no runtime.

Pedido e script originais permanecem imutáveis. Seus dizeres históricos CANDIDATE/NOT_AUTHORIZED são anteriores a este ato, cuja precedência é exclusivamente operacional para a tentativa delimitada. O serviço NÃO foi declarado criado, ausente ou parado; execução, consumo interno e resultado factual permanecem pendentes da ação manual. Nenhuma aprovação anterior foi reutilizada.

## Conferências concluídas

- Pedido integral/hash, R0–R9 integral incorporado pela decisão accepted e limites F1–F6/M5/P3/conta virtual/SSO-B já aplicáveis: sem divergência identificada. Nenhum contrato accepted modificado.
- Script original SHA256 `55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571`; não alterado ou executado.
- Controlador Selected/Release 579072 bytes / `ca4e1fa707230510da19eb7aebd4ab0e39504661504c27ebecfc494c95ccec55`, DLLs1321472/7217664 bytes e pins3fb3cd78…/09eec573…: conferidos somente por leitura.
- Alvo C:\Program Files\Papinho\Accelerator: EXE571392 bytes / `c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf`; DLLs e respectivos bytes/pins também conferidos por leitura. Atributos observados Archive, sem reparse de arquivo indicado nesse snapshot.
- Essa verificação NÃO adquire FileIDs/handles originais ou comprova DACL/ancestrais atuais, imagem mapeada, token/host/boot elegíveis, capacidade fresca ou ausência do serviço. Esses fatos permanecem gates obrigatórios do wrapper/owner na tentativa, sem conversão de UNKNOWN em PASS.
- Nome/conta/comando fixos: PapinhoAccelerator, NT SERVICE\PapinhoAccelerator, DEMAND_START=3, SID type UNRESTRICTED=1, `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`. Não equivale a um comando de Start.

Nenhuma consulta SCM, CreateService/ChangeServiceConfig2, Start/Stop, staging, ACL, endpoint, chave/CA/CURRENT, rede operacional ou outra operação protegida foi executada nesta preparação. Não houve build nem repetição de testes inalterados. Quiescência OS e máximos opacos continuam UNKNOWN. O snapshot de arquivos não consome a tentativa nativa nem rearma qualquer autorização histórica consumida.

## Comando exato — uma execução manual

No Windows PowerShell 5.1 x64, console já aberto como administrador no host/operador aprovados, executar UMA vez o bloco abaixo. Conferências de hashes antecedem a chamada. Bypass é somente do processo filho; nenhuma política persistente é alterada. Se GPO/política superior negar, STOP, sem contornar ou repetir.

```powershell
& {
    $scmRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-r0-r9-integration-20261031\ONE-STOPPED-SCM-REQUEST.CANDIDATE.md'
    $scmProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-r0-r9-integration-20261031\register-stopped.MANUAL.ps1'
    if ((Get-FileHash -LiteralPath $scmRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne '7eb9e6912a3d2c4b3a4408d176bfcd356e511da3a8bfc240942ce82a7ad03a79') { throw 'REQUEST_HASH_DENY' }
    if ((Get-FileHash -LiteralPath $scmProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571') { throw 'SCRIPT_HASH_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $scmProcedure -ExpectedScriptSha256 '55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571'
}
```

Conferir o plano nativo e seus três pins staged contra o pedido antes da frase OWNER_SCM_REGISTER gerada naquela tentativa. Nenhum valor de confirmação é preenchido automaticamente. Divergência implica cancelamento; não trocar paths, arquivos, identidades ou permissões. O escaping `\x5c` do presenter representa barra invertida, sem alterar o path nativo.

## STOP e reconciliação

Toda falha/cancelamento encerra a única execução manual, mesmo sem consumo interno. Sem retry. Stop/perda de contexto, originais/proteção/pins divergentes, serviço existente/ausência incerta, margem abaixo de Q, observação indisponível, erro/overflow ou fechamento incerto negam novas submissões. Em parcial não remover serviço, corrigir conta/SID/ACL ou iniciar para testar.

Enviar somente a saída textual não secreta completa do controlador e wrapper. Reconciliar tentativa/consumo, primeira falha, cada disposição persistente, configuração, SERVICE_STOPPED, close e retenção. Wrapper exit0 sozinho não basta. Se o resultado for inconclusivo, preservar UNKNOWN e não repetir. Não solicitar credenciais ou tokens exportados.

Se a prova de registro parado for suficiente, o próximo marco será preparar separadamente Start diagnóstico fechado. Esta autorização NÃO permite iniciar/parar serviço, endpoint/SCQ, KSP/CA/CURRENT ou promoção produtiva. E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN; PST0.6.4 preservada.
