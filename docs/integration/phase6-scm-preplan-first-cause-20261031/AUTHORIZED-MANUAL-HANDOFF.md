# Autorização registrada — execução manual do registro SCM corrigido

Task PACC-PHASE6-CORRECTED-STOPPED-SCM-AUTHORIZATION, 2026-10-31. Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469. O proprietário aprovou explicitamente UMA execução MANUAL do pedido SHA256 `fd08f3ae7278ba50fe712769ff28134d3efbfdf7d100db181cb92bd97ce73d9d`, sujeita aos contratos e gates frescos. Não autoriza execução automática ou retry. Registro documental separado em OWNER-CORRECTED-STOPPED-SCM-AUTHORIZATION.json; não é grant/ledger/loader do runtime.

A autorização anterior7eb9e691… permanece UTILIZADA e não reutilizável. Seu resultado e os arquivos originais continuam preservados. Esta nova aprovação não altera o pedido corrigido, script, código ou contratos accepted; apenas estabelece o ato operacional específico posterior. As indicações históricas CANDIDATE/NOT_AUTHORIZED nos originais antecedem esse ato e não são editadas para manter seus hashes.

Execução e consumo nativo novos continuam pendentes. O serviço não foi consultado, criado ou declarado instalado/parado pelo agente. Seu estado atual permanece UNKNOWN; o erro exato nativo histórico anterior continua sem prova retroativa de RNG ou ACL.

## Conferência concluída

- Pedido integral e SHA256 fd08f3ae…, R0–R9 incorporado pela decisão accepted, limites de F1–F6/M5/P3/conta virtual e separação SSO-B: sem divergência identificada.
- Script2c43a4f6… imutável, preparado e parseado, NÃO EXECUTADO.
- Controlador Selected/Release: 582144 bytes /63291745746f9f7441bec0894ef2bf1fc2b57485cca38ece4e70a3536d511c83. DLLs1321472/7217664 bytes e hashes3fb3cd78…/09eec573… conferidos por leitura.
- Alvo staged C:\Program Files\Papinho\Accelerator: EXE571392 bytes /c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf; DLLs/pins/tamanhos também conferidos por leitura. Atributos de arquivo observados Archive, sem reparse indicado nas folhas. Detalhes em AUTHORIZATION-READONLY-FILE-SNAPSHOTS.json.
- Snapshot de path/bytes/hash não comprova FileID original, proteção efetiva dos ancestrais/descritores atuais, imagem mapeada ou contexto/token/capacidade frescos. Esses gates permanecem obrigatórios no wrapper/owner da tentativa.
- Conferida a correção: o caller que prepara BINARY_ONLY usa layout_binary_current, com validação binária estrita e revalidação nativa seguinte; o genérico continua rejeitando esse modo. Conferidos diagnóstico monotônico de primeira causa antes de close e evidências offline (quatro builds PASS/zero warnings, 72 pares CTest/configuração). Nenhum build/teste inalterado repetido nesta preparação.
- Nome PapinhoAccelerator; conta NT SERVICE\PapinhoAccelerator; DEMAND_START3; SID type UNRESTRICTED1; comando fixo `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`. Isso não é um comando de Start.

## Procedimento manual exato

Em Windows PowerShell 5.1 x64 já aberto como administrador no host/operador do pedido, executar UMA vez o bloco aprovado abaixo. As duas verificações de hash antecedem a chamada. Bypass somente no processo filho; não alterar política persistente, GPO ou permissões. Em negativa, STOP sem contorno/retry.

```powershell
& {
    $scmRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-preplan-first-cause-20261031\ONE-STOPPED-SCM-REQUEST.CANDIDATE.md'
    $scmProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-preplan-first-cause-20261031\register-stopped.MANUAL.ps1'
    if ((Get-FileHash -LiteralPath $scmRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'fd08f3ae7278ba50fe712769ff28134d3efbfdf7d100db181cb92bd97ce73d9d') { throw 'REQUEST_HASH_DENY' }
    if ((Get-FileHash -LiteralPath $scmProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '2c43a4f63fcdf1f2f2932bcf83d3bcbfcd399a649e008f3306a4d134f1676fe7') { throw 'SCRIPT_HASH_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $scmProcedure -ExpectedScriptSha256 '2c43a4f63fcdf1f2f2932bcf83d3bcbfcd399a649e008f3306a4d134f1676fe7'
}
```

O wrapper verifica novamente host/SID/contexto, known folder e os seis pins/tamanhos, sem registrar consentimento automaticamente. O owner adquire e revalida os originais protegidos, mostra o plano e requer declaração/confirmação atribuída específica. Conferir alvo, conta/comando/início/SID type e três pins staged antes de confirmar a frase OWNER_SCM_REGISTER gerada naquela tentativa. O escaping `\x5c` representa barra invertida; divergência real exige cancelamento, não mudança de arquivo/path/ACL.

## Limites, STOP e evidência futura

Somente registro local exclusivo parado, configuração do SID type, readback original de configuração e SERVICE_STOPPED e fechamento governado do pedido. Um consumo, no máximo uma CreateService e uma configuração SID. Sem Start/Stop/Delete, staging, diretórios/contas/CA, ACL, endpoint, CURRENT, rede operacional, TPM/NV/Q4, cleanup/refund ou autoridade produtiva. Risco nativo/RPC e quiescência OS UNKNOWN reconhecidos; pisos/monitoração R3 não são reserva ou garantia de conclusão.

Falha de identidade/proteção/pin/contexto/continuidade/observação, serviço existente ou ausência incerta, Stop, insuficiência segundo R3, erro/overflow, readback divergente ou fechamento incerto impede novas submissões. Se houver parcial, conservar consumo/cohort/evidência e não remover/corrigir/iniciar para testar. Qualquer falha ou cancelamento encerra a execução autorizada, mesmo se consumo interno0; não repetir.

Após receber a saída textual não secreta completa: registrar utilização desta autorização; identificar primeira causa (inclusive SCM_REGISTER_FIRST_CAUSE), consumo e alcance de Create/configuração; conferir conta/comando/início/error-control/SID type/SERVICE_STOPPED, disposições persistentes e close/retention. Exit0 isolado não basta, ausência de um frame não equivale a PASS. Não solicitar segredos/tokens exportados nem fazer consultas adicionais por inferência.

Somente depois de registro parado demonstrado preparar separadamente o Start diagnóstico fechado. Start não está autorizado aqui. E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN; HEAD/WIP, históricos/pins/autorizações consumidas, chave fixture retida e PST0.6.4 preservados.
