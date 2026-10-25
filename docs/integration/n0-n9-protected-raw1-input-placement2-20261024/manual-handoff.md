# Handoff — N2P-20261024-02, somente preparação

Nenhuma colocação autorizada por este pacote. A raiz foi criada na operação anterior; nunca executar o criador N0 novamente. Não reutilizar N2P-01 ou o launcher com destino C:\Projetos.

Usar um único Windows PowerShell5.1 x64 já elevado e novo (sem PaccFileAcl/PaccN2PlacementNative já carregados). O script não cria outro PowerShell. Se a política local bloquear, eventual ajuste deve limitar-se a Scope Process; não modificar política permanente nem contornar GPO.

## Primeira prova restante: somente consulta

No contexto elevado do operador autorizado, o comando preparado para a consulta read-only é:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-raw1-input-manual.ps1" -Mode VerifyRoot
```

Não cria diretório, input ou recibo administrativo, não executa preflight. Verifica operador/host/boot, abre e retém os cinco originais, compara identidades e RAW, confere fechamento. Exigir ROOT_READONLY_FIRST_ERROR=NONE e RETAINED=False. Em erro preservar saída, sem retry/remediação. Essa consulta não constitui aprovação da colocação.

## Operação candidata futura — somente após aprovação específica

Em uma nova janela elevada única (para evitar reutilizar tipos compilados na consulta anterior), o comando exato candidato é:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-raw1-input-manual.ps1" -Mode Place
```

O plano solicita esta confirmação literal:

```text
PLACE_N2_INPUTS N2P-20261024-02 75330d11cc7e4123ae62eadc9a59c2f5
```

A operação revalida a raiz por handles originais antes de CREATE_NEW; uma consulta anterior nunca substitui essa validação. Cria exclusivamente os nomes abaixo em C:\ProgramData\PapinhoAccelerator\Protected\N0:

| Nome | Bytes | SHA256 |
|---|---:|---|
|papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence|5358|babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0|
|papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved|326|5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294|

Mais um recibo administrativo exclusivo neste pacote, manual-N2P-20261024-02.txt. Recibo em docs não é autoridade nem input protegido; pode existir após falha anterior à criação de inputs. Não conceder crédito/cleanup com base nele.

Escape solicita Stop entre chamadas síncronas; não significa conclusão ou cancelamento de chamada já iniciada. Não fechar console em UNKNOWN; preservar referências e objetos. Nenhum arquivo parcial será apagado. Nenhuma repetição após falha sem nova decisão. Aguardar NATIVE_OWNED_CLOSED, RETAINED, ADMIN_RECEIPT_CLOSE e N2_PLACEMENT_EXIT factuais; janela desaparecida não prova fechamento.

Pins completos: qualified-pins.json. Script de entrada SHA25627c17b69ae245411f4a2289c3ff519ba148d9bd090089e4f7ae286b2ad3ebc9f; launcher SHA2567b15a9d73420d9754596efc685501ea836bd98af346c3d3978a3f83356406f0c; nativo.cs323dd10c2331402ad3757dcc6c2170e0432777db339025721d763124bac5d4cb. O script de entrada de desenvolvimento requer integridade/conferência independente; sua autoverificação não prova proteção contra substituição de si próprio. Ele executa em memória os bytes originais hash-verificados do launcher mantido aberto. Helpers são compilados a partir dos mesmos bytes verificados. Não há alteração de pins da admissão LAB.

Operador SID S-1-5-21-785699550-1440458426-1682014700-1108, hostSHA2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff, boot236441/2026-10-23T18:36:34.5000000Z. Mudança, ausência, stale ou divergência nega sem repin. Inputs continuam históricos e não provam métricas atuais. Não há Start/Stop/SCM/preflight/ACL de objetos existentes/CA/cert/bootstrap/CURRENT. ED11 PARTIAL; autoridades DENY.
