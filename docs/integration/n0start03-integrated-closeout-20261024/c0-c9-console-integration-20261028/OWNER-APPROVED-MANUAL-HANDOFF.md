# Autorização C0–C9 e handoff manual TPM

Autorização específica registrada a partir da decisão explícita do proprietário nesta tarefa. Uma invocação manual, sem execução automática ou retry, inclusive se houver DENY antes do receipt. Pedido e relatório históricos permanecem byte-exatos; seus campos de preparação anteriores são históricos.

Pedido SHA256: `b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b`.

Aprovação: [OWNER-APPROVED-NATIVE-PROOF.json](manual-candidate/OWNER-APPROVED-NATIVE-PROOF.json), SHA256 `94f9ae80a9d8c5bc6ef179a97e43244e8237cbfd235131828d94b2af82bd8afc`.

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\manual-candidate\NativeTpmProof.exe" --owner-manual-proof 59c67f6d81d88c49c1d676bde2553aa8a843a1925c76a33deb14acbab3424898 94f9ae80a9d8c5bc6ef179a97e43244e8237cbfd235131828d94b2af82bd8afc
```

Usar Windows PowerShell 5.1 x64 já elevado, no console interativo do proprietário. Não usar wrapper/pipeline nem copiar PIDs históricos. Confirmar somente a linha integral exibida pelo programa, com request SHA e PID/criação LIVE do pai e auxiliar. Os gates de operador/token/host/boot, originais, RAW/proteção, topologia e recursos permanecem frescos e obrigatórios; não foram substituídos pela conferência estática.

Conferência: 35 entradas dos manifestos, 18 fontes do pedido e históricos reutilizados correspondem aos hashes; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado. Aprovações anteriores usadas e preservadas; receipt novo ausente antes do registro. Sem rebuild ou testes repetidos, pois não houve alteração funcional. Builds Debug/Release e 18 CTests anteriores permanecem evidência offline.

Limites: pré-observer124/próprio128/administração1024, máximo12 submits diretos, um contexto e uma sessão transitória. Revogação, pico não amostrado e internos permanecem UNKNOWN; nenhum orçamento ou confiança de hardware concedido.

STOP: divergência de identidade/pin/contexto/contagem/consulta, Escape, primeira falha ou fechamento incerto. Não repetir, apagar receipt, forçar fechamento ou limpar resíduos. Falhas incertas congelam aquisições/comandos e preservam originais/dependências; perda do host UNKNOWN. Sem NV/AK/hierarquia/Clear/Q4/LAB/Start/Stop/SCM/ACL/CURRENT/assinatura/recuperação.

Enviar saída integral C0_BIND, NATIVE_API_COUNTS e JSON, inclusive falhas. Reconciliação posterior verificará participantes originais, recursos, comandos, challenge/HMAC/Name, sessão/contexto, primeira falha e retenção. Prova TPM NOT_EXECUTED; E21 PENDING; ED11/Phase6 PARTIAL. Próximo gate: única execução manual pelo proprietário e recebimento da evidência.
