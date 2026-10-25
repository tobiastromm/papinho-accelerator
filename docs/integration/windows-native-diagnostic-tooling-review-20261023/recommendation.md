# Windows native diagnostic tooling review

2026-10-23. Revisão técnica; nenhuma mudança normativa ou funcional.
Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. WIP preservado.

## Recomendação

Priorizar WPR/ETW existente no Windows 10 e análise offline WPA antes de ampliar instrumentação própria. VMMap e RAMMap complementam perguntas específicas. Ferramentas permanecem externas ao produto. Nenhum trace, instalação, launcher, preflight ou serviço foi executado nesta revisão; apenas documentação, código e metadados de perfis foram consultados.

| Ferramenta | Uso recomendado | Compatibilidade e limite |
|---|---|---|
| WPR/ETW + WPA | Eventos de processos, imagens, handles e arquivos | WPR local10.0.19041.6093; perfis Handle/FileIO presentes. WPA não encontrado no PATH nem no caminho convencional consultado; análise pode ocorrer separadamente. Validar schema, cobertura e perdas. |
| Process Monitor | Operações de arquivo/registro, resultados e stacks | Página atual v4.12 exige Windows11+. Não qualificar esse download para Windows10. Versão anterior requer suporte documentado, proveniência e hash; nenhuma selecionada. |
| Process Explorer | Handles e DLLs por processo | Página atual v17.2 exige Windows11+. WPR é alternativa nativa imediata; versão anterior somente após qualificação. |
| VMMap v3.4 | Commit por tipo, working set e mapa virtual | Suporta Windows10+. Snapshot não prova máximo universal ou todo custo kernel. |
| RAMMap v1.63 | Memória física, cache e contexto kernel | Suporta Vista+. Somente observar/exportar; não limpar listas/cache. Não atribui necessariamente cada custo a um processo. |

Fontes Microsoft: [WPR](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/windows-performance-recorder), [CLI WPR](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/wpr-command-line-options), [ProcMon](https://learn.microsoft.com/en-us/sysinternals/downloads/procmon), [Process Explorer](https://learn.microsoft.com/en-us/sysinternals/downloads/process-explorer), [VMMap](https://learn.microsoft.com/en-us/sysinternals/downloads/vmmap), [RAMMap](https://learn.microsoft.com/en-us/sysinternals/downloads/rammap).

Não introduzir Sysmon residente. Handle pode fornecer snapshots, mas sua página não estabelece aqui suporte Windows10; nunca usar fechamento forçado. Sigcheck atual também exige Windows11+. Get-AuthenticodeSignature é alternativa nativa para assinatura/catálogo, com comportamento de rede a qualificar separadamente. Assinatura do arquivo atual não autentica automaticamente a imagem mapeada.

## Auditoria do censo

Em tools/qualification/scm-instrumented-cycle-lab.inc, ig_project_census rejeita !participant && (child || project_prefix). child usa parent PID; project_prefix é prefixo papacc. ERROR_BUSY assim agrega filho arbitrário, infraestrutura de console e possível concorrente; não demonstra segunda admissão. Contudo a rejeição conservadora de filho adicional corresponde ao B4.B.4 vigente. Não cabe whitelist de conhost, remoção de custos ou alteração silenciosa do contrato.

A exclusividade administrativa pertence ao owner canônico, com mutex, identidade, consumo único, Stop, quiescência e retenção. Árvore de processos e ferramentas externas não substituem essa autoridade.

A topologia exige dois PowerShell distintos, qualificador e serviço futuro. Isso acopla B4 ao wrapper/UAC. O conhost capturado nasceu antes do qualificador; herdar console no último salto não o eliminou.

Alternativa PROPOSTA: uma sessão Windows PowerShell5.1 já elevada invoca o launcher na própria sessão; este cria o mesmo qualificador e governa o serviço futuro. Elimina um intermediário PowerShell. Exige revisão focal de B4, launcher, identidade e lifetime antes de implementação. Não passar o mesmo PID nos dois papéis existentes nem retirar referências retidas para contornar guards. Remover um salto sozinho não corrige o predicado child nem autentica associação de console.

Coletor externo e infraestrutura devem ser contabilizados explicitamente em plano diagnóstico revisto. Iniciar antes do launcher não torna recursos gratuitos. Não ocultar observador ou conhost, conceder quinto slot ou créditos.

## Evidência e pergunta focal

Preservar [captura contemporânea](../contemporary-console-host-proof-20261023/manual-contemporary-capture-result.md). Recibo manual-prepare-134372761903030040-final.json SHA256 9ef63149e6eaf7eeb8ebed4b7deb25948ba0a60508d2110aadbc21436ae2abe4: primeira falha PREFLIGHT_CENSUS170; zero Starts/Stops/efeitos/consumo.

O qualificador apresentou147 handles, acima do guard128. Investigar baseline, handles transitórios da coleta e retenção via trace externo; uma amostra não prova leak. Não elevar threshold. Associação autenticada de console e imagem continuam UNKNOWN; parent, sessão e lista de clientes não bastam. image_stable=0 não prova substituição: a comparação inclui metadados mutáveis, sem detalhamento por campo no log.

## Fluxo futuro mínimo

1. Fixar ferramentas/hashes, host/boot, sujeitos por PID+criação, owner da sessão, destino externo de evidências, limites e duração. Rever custos e efeitos do observador. Esta revisão não autoriza captura ou novo preflight.
2. Qualificar perfil existente Handle.Light em modo arquivo sobre fixture descartável, conferindo eventos e perdas. Propor depois uma única captura da topologia real, mediante autorização específica. Correlacionar nascimento/imagem e create/duplicate/close dos handles.
3. Analisar ETL offline no WPA compatível. Encerrar somente a sessão adquirida e registrar disposição factual ou UNKNOWN; não cancelar sessões alheias nem fechar handles investigados.
4. FileIO em janela separada se necessário. [FileIo_OpEnd](https://learn.microsoft.com/en-us/windows/win32/etw/fileio-opend) oferece NtStatus e correlação IRP, mas habilitação/schema/cobertura/perdas do perfil concreto devem ser verificados. ProcMon compatível pode substituir essa etapa. Nenhum deles garante observar todo erro ou predicado interno.
5. VMMap para memória/stack observável e RAMMap para físico/cache, preferencialmente separados. Registrar baseline, pico observado e final; amostras não são teto universal.

A coleta read-only não exercita psr_prepare do serviço. Capturar a falha original nesse caminho requer futura autorização específica de runtime; não contornar Start bloqueado.

Consulta local wpr -profiledetails Handle.Light -filemode mostrou dois coletores com20 buffers de1024KiB cada:40MiB configurados no perfil isolado. Keywords incluem Handle, Loader e ProcessThread. FileIO.Light.File tem a mesma configuração no perfil isolado. Não somar perfis sem analisar coletores compartilhados;40MiB não é custo total máximo nem quota nova. Defaults Verbose.Memory têm muito mais buffers. Nenhuma sessão iniciada.

Preservar primeira falha/NTSTATUS e recibos canônicos. Só retirar collectors próprios após demonstrar cobertura externa e revisão oficial; ETW não substitui semântica privada de autorização/Stop/retenção.

## Disposição

CUSTOM_INSTRUMENTATION_EXPANSION=DEFERRED
RECOMMENDATION=WPR_EXISTING_PROFILE_FIRST
LAUNCHER_SIMPLIFICATION=PROPOSED_REQUIRES_REVIEW
B4_B4=UNCHANGED
CROSS_ADMISSION_EXCLUSIVITY=PRESERVED
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
START_EXECUTED=NO
PREFLIGHT_RETRY=NOT_RUN
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
NEW_START_AUTHORIZED=NO
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY

Código/pins/ADRs accepted preservados; sem builds necessários para revisão documental. WIP, PST, Browser, PACC wire, produção0.6.4 e quotas preservados. Sem novo serviço, runtime, clone/worktree, commit/push/tag/release, bootstrap, CA, chave ou certificado.
