# Histórico — sessão manual utilizada; NÃO EXECUTAR NOVAMENTE

Pedido25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4;
decisão REALc6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f.
Autorização registrada em THREE-ACTS-OPERATIONAL-AUTHORIZATION.md.
Execução realizada pelo proprietário: cópia auxiliar concluída; controlador
frio negado com1816, sem Start. Ver FIRST-DIAGNOSTIC-1816-RECONCILIATION.md.
Autorização utilizada, sem retry. O bloco abaixo é somente histórico;
NÃO repetir. Nenhum comando desta página foi invocado pelo agente.

No PowerShell elevado do operador S-1-5-21-785699550-1440458426-1682014700-1108
em TCORP-TOBIAS, usar UMA vez o bloco abaixo. Ele confere o script antes da
chamada; o script confere os demais pins, contexto e metadados antes dos efeitos.
Bypass é só do processo filho, sem alteração persistente de política ou ACL.
Não usar ISE, pipeline de saída/entrada, Tee-Object ou redirecionamento: os
confirmadores exigem console original. Não usar Services UI/sc.exe start ou
service start diretamente, pois isso contornaria a composição original.

```powershell
& {
    $firstDiagnosticScript = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-first-diagnostic.MANUAL.ps1'
    $firstDiagnosticScriptSha256 = '244531d47ddbebddddbf304cfa2466caae53fbac11127fee4b70a9fbd3d34f35'
    if ((Get-FileHash -LiteralPath $firstDiagnosticScript -Algorithm SHA256 -ErrorAction Stop).Hash -ne $firstDiagnosticScriptSha256) {
        throw 'FIRST_DIAGNOSTIC_SCRIPT_PIN_DENY'
    }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $firstDiagnosticScript -ExpectedScriptSha256 $firstDiagnosticScriptSha256
}
```

Etapa1: confirmação explícita da UMA cópia auxiliar, texto exibido pelo wrapper.
Esperado first=NONE, created_mask=1, verified_mask=1, close_known=True e novo ID
da decisão operacional. Não há recriação do request ou da decisão de preparação.
Qualquer erro impede chamada do controller. Se o arquivo já existir, não adotar,
apagar ou repetir; preservar e enviar a saída para reconciliação.

Etapa2: o EXE canônico adquire request/decisão/imagem e pede TRÊS confirmações
independentes: OWNER_SCQ, OWNER_SCQ_PRIVATE_ENDPOINT, OWNER_SCQ_DIAGNOSTIC_START.
Responder somente se os planos forem exatos: requestID417614100:1310720:120518,
imagemID417614100:1900544:539611 e decisãoID igual ao reportado na etapa1, pins
exatos e G3 aprovado. As linhas de confirmação usam a tentativa produzida ao vivo;
não foram inventadas/preenchidas pelo agente. Antes de Start, runtime PID/token
podem estar legitimamente não vinculados; isso não é autenticação ou PASS.

SCM STOPPED/configuração e margens G3 são gates frescos internos. Após Start,
identidades/token/bilateral/empréstimo/receipt/close são obrigações reais, não
conclusões antecipadas. RUNNING, se alcançado, é só diagnóstico fechado. O caminho
é finito e exige retorno normal/STOPPED; não acrescentar ControlService Stop.

Enviar a saída INTEGRAL das duas etapas, incluindo cada plano/confirmador e
primeira falha ou marcador final. SERVICE=REQUEST_SUBMITTED e exit0 isolados
não qualificam a prova. Se houver falha, não repetir; preservar objetos/consumo.
RETAINED/UNKNOWN pode estacionar o processo indefinidamente: não forçar término,
limpar, recuperar ou encerrar serviço para fabricar fechamento. Enviar a última
saída e aguardar análise/decisão específica se intervenção for necessária.

Essa sessão não resolve por si só a retenção histórica, OS_QUIESCENCE, capacidade
produtiva ou NEW_INSTALL. Não editar/remover .previous/candidatos/pins/arquivos.
