# Knownfolders/drive — reorganização mínima integrada offline

Não foi demonstrado vazamento de23 handles nem contrato que permita fechá-los.
Foi demonstrada uma redundância funcional: o controlador TRANSPORT_ONLY resolve
ProgramData/full-layout, embora essa operação não use alvo de dados/CA/CURRENT.
O modo binary-only já existe e é usado na preparação receptora/cadeia de imagem.
A seleção controladora e algumas revalidações exigiam NEW indevidamente para
esse propósito binário. Integração corrigida, sem mudanças normativas/quotas.

## APIs, dependências e ownership

| Grupo executado anteriormente | Recurso explicitamente entregue ao programa | Lifetime e tratamento |
| --- | --- | --- |
| SHGetKnownFolderPath ProgramFiles, DONT_VERIFY, hTokenNULL — Shell32 | Um PWSTR de task allocator | Copiar após limite de comprimento e CoTaskMemFree no caminho done, inclusive falha |
| SHGetKnownFolderPath ProgramData — Shell32 | Outro PWSTR | Também liberado; removida consulta e buffer do percurso TRANSPORT_ONLY binário |
| pip_copy/pip_join/wcslen/memcpy — código/CRT | Arrays locais e campos do packet | Sem HANDLE adquirido; validação de tamanho/caminho preservada |
| CoTaskMemFree — Ole32/allocator | Não devolve HANDLE nem resultado de close | Libera memória do caller, não demonstra desmontagem de caches/processo shell |
| GetDriveTypeW — Kernel32 | UINT de tipo de volume | Não devolve HANDLE que o caller possa fechar; verificação DRIVE_FIXED permanece antes da guarda |
| GetProcessHandleCount — Kernel32 | Contagem escalar do próprio processo | Sem referência enumerável ou permissão para fechar internos |

Documentação primária consultada:
[SHGetKnownFolderPath](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath),
[CoTaskMemFree](https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cotaskmemfree),
[GetDriveTypeW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypew).
Esses contratos não atribuem23 handles à aplicação nem fornecem API para liberar
um cache interno. Recursos internos/caches são possibilidades, não ownership
individual ou legitimidade/lifetime factual demonstrados. Não inventar
CoUninitialize, FreeLibrary, registry-close ou enumeração/CloseHandle arbitrários.
Este trecho não adquiriu interface COM ou apartment com obrigação de release.

O laço full-layout fazia duas queries distintas, não duas aquisições do mesmo
HANDLE. Não havia PWSTR explicitamente retido após done. A duplicação eliminada
é de ESCOPO: resolver dados de instalação completa para prova somente binária.
Não adiar ProgramFiles/drive para depois da guarda a fim de esconder seu custo;
ambos continuam antes de adquirir referências e antes dos efeitos admitidos.

## Integração e compatibilidade

scqc_plan agora usa papacc_personal_paths_binary. O observador temporário também
acompanha o caminho corrigido; seu antigo pedido/pin não autoriza nova execução.
personal_scq_paths_current permite BINARY_ONLY somente para TRANSPORT_ONLY,
via binary_current existente: knownfolder real revalidada, campos parent/data/leaf
vazios e protegidos contra preenchimento indevido. NEW existente permanece para
callers anteriores; EXISTING/READ não ganham esse atalho.

Callers efetivamente ligados: captura da seleção, current administrativo,
admin_prepare, profile_prepare_transport, acquire_contents e offer do transporte.
Receiver QUERY e image_chain já suportavam binary-only, mantendo identidade/
SD/ancestrais/ausência de reparse e referências originais. Registrations apontam
para arrays originais do packet; parent/data vazios são DATA não aplicável,
não novos paths autoritativos. Confirmações/consumo/SCM/SSO-B/receipt/close intactos.
Guardas genéricas de instalação completa e READ não foram afrouxadas.

TO0/TO3 mantém transporte sem KSP/CA/CURRENT; TO4/TO5/OHI/SSO-B exigem originais,
contexto e continuidade, não consultas ProgramData sem uso. G3/G4 exige contabilizar
custos reais, não criar aquisições desnecessárias. Nenhuma cláusula accepted
exige o modo NEW para esse alvo somente binário; trata-se de adaptação técnica
ao perfil já aceito, não nova autoridade. Não altera guards124/128/1024.

## Qualificação e limites

4builds Selected/Legacy Debug/Release PASS sem warnings;7 testes afetados por
configuração,28pares CTest PASS. Logs cold-binary-paths-*-build/tests/details.log
e KNOWNFOLDER-GROUP-OFFLINE-EVIDENCE.json. Paths59checks inclui falha ProgramData
que NÃO afeta binary-only,1query/1free por aquisição e revalidação, campos
indevidos/Stop/caminho divergente, full-layout ainda2queries e READ inalterado.
Seleção nativa20casos/231checks, incluindo binary-only legítimo/negativos;
cold_controller17casos/153checks com fixture binária; cold_open32/115,
QUERY34/1755, G3 assurance e censo também PASS. APIs mutantes modeladas.

Redução demonstrada:2knownfolder queries para1 por aquisição/revalidação desse
fluxo; remoção do PWSTR ProgramData e de sua dependência desnecessária.
Redução de HANDLES demonstrada: NENHUMA. ProgramFiles sozinho pode inicializar
os mesmos internos;136 histórico não se reclassifica. Não há teto de caches
ou provisão de opacos provado. Nenhum fechamento antecipado implementado.

Novo candidato765952bytes, SHA256
566ee26c8b02cee4e27294883d8ed6341e40c3ae035c76742a044be894c48e4f,
censo Selected2001440 inalterado. Não selecionado para Start nem instalado.
Canônico7d29...ca7d/763904bytes e utilitário protegido permanecem intocados.

Primeira lacuna: efeito nativo de retirar ProgramData, especificamente se a
fronteira pré-open corrigida ficará<124 e com recursos posteriores suficientes.
Antes de UPDATE/novo Start, há pedido read-only focal pronto para seleção:
ONE-COLD-BINARY-PATHS-OBSERVATION-REQUEST.CANDIDATE.md. Reutiliza observador e
wrapper existentes, só novos pins/pedido; sem serviço/canal/infraestrutura.
Não foi executado nem autorizado. Não promete que a coleta faça o Start passar.

Se ProgramFiles sozinho mantiver excesso e não houver handle próprio redundante,
não haverá CloseHandle legítimo demonstrado. Alternativas técnicas exigirão fonte
de destino igualmente legítima/contínua, sem getenv/caminho fixo/grant; não adiar
custo real ou mover para outro processo como desconto. Qualquer mudança de
124/128 seria revisão normativa explícita de G4/QUERY-G3 e vínculos SCQ, não
correção técnica autorizada aqui. Nenhuma proposta de quota foi criada.

Sem nova coleta, endpoint, SCQ, Start/Stop, UPDATE, SCM/ACL, CA/CURRENT, rede,
TPM/NV/Q4, limpeza/refund ou recuperação. Preservados HEAD/WIP/pins/retidos/EXEs
anteriores/decisões consumidas/PST0.6.4. DENY e UNKNOWN históricos/OS intactos.
