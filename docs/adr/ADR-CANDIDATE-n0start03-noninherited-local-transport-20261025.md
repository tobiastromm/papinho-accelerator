---
status: proposed
revision: 2
date: 2026-10-25
scope-level: project
scope-target: N0START-20261024-03 local noninherited stdout bootstrap
supersedes: []
---
# Candidate — transporte local sem herança T0–T9

## T0 — escopo e decisão pendente

Propor, exclusivamente para N0START-20261024-03 / tentativa0b1de869e11493b7cb08fb329578f6ce, transporte stdout local sem herança. Mesmo serviço PapinhoAcceleratorLab20261023, owner/mutex, quatro papéis, RAW1/N0–N9/L/H/P3 e decisões N7 existentes. Nenhuma admissão, aprovação executável ou mudança em produção0.6.4. Esta proposta não está ACCEPTED.

A autorização técnica atual permitiu o protótipo isolado e testes. N5 accepted determina “no ... new IPC ... is authorized”; a exceção focal de transporte ainda exige decisão explícita. Não promover essa autorização técnica a revisão implícita do contrato accepted. Histórico e decisões anteriores não são modificados.

### Emenda exata proposta à primeira frase de N5

Se esta revisão2 for explicitamente aceita e registrada pelo processo autorizado, SOMENTE para a operação N0START-20261024-03/tentativa0b1de869e11493b7cb08fb329578f6ce e o tuple host/boot/serviço normativamente fixado em L1, substituir a primeira frase de N5 pela redação:

> Require usable interactive confirmation/Stop console before effects; no detached/unattended topology, replacement actor or background launcher is authorized. No new IPC is authorized, except the single ephemeral local stdout transport between the existing elevated PowerShell supervisor and its exact original native qualifier, strictly under T0–T9 of this decision.

A segunda frase de N5 (“Console is transport...”) e todos os demais parágrafos de N5 permanecem integralmente controladores. B+D R0–R7 não são dispensados ou reescritos. Nenhuma permissão de controle independente de console é inferida.

A exceção autoriza somente recursos de bootstrap/transporte entre os MESMOS dois participantes: conexão única autenticada, hello/ack de estabelecimento e, após esse estabelecimento, bytes stdout no sentido qualificador→supervisor. Não autoriza comandos de admissão/Stop no pipe, IPC genérico, RPC, endpoint de uso por Browser/PST/runtime/serviço/outro componente, comunicação remota, daemon ou quinto papel. O canal não autentica ou autoriza um operador, não assina um recibo e não substitui nenhuma prova N0/B+D/RAW1.

A criação transitória do endpoint/evento pode preceder a inicialização do owner nativo do filho, assim como a criação do processo e o antigo pipe stdout, SOMENTE depois das verificações do launcher e da autorização exata da operação de qualificação. Ela é aquisição preparatória explicitamente contabilizada, não consumo do intent nem autorização de efeito LAB. Nenhum efeito governado positivo precede o consumo único vigente; perfis/rotas positivas permanecem DENY nesta integração. Falha/UNKNOWN em transporte não autoriza recriar endpoint, relançar filho ou armar outra tentativa.

A revisão1 aprovada como baseline técnico permanece byte-exata no pacote de revisão, SHA256045b3edd03cb5841036eb047a62f54adee9763055429cfac6fc062d131257395. Nenhuma aceitação desta revisão2 é presumida.

## T1 — escolha técnica

Substituir a necessidade de handles privados herdáveis por named pipe local com criação exclusiva, endpoint aleatório criptográfico de128 bits, apenas uma instância e rejeição de clientes remotos. CreateProcessW do qualificador usa bInheritHandles=FALSE. Todos os novos handles de endpoint, eventos e processos são criados não herdáveis; nenhum flag de handle preexistente do operador é alterado. Cliente deve validar namespace estritamente local, prefixo fechado e32 caracteres hexadecimais minúsculos ANTES de qualquer abertura do endpoint; UNC remoto, aliases, sufixos, caminhos alternativos ou IDs com lixo/overflow são rejeitados sem tentativa. Nome aleatório não é credencial nem substitui autenticação. Sem helper-processo/serviço/owner de autorização adicional.

## T2 — proteção e identidade dos pares

DACL protegida explícita desde a criação do endpoint: SYSTEM e operador exato; direitos do cliente0x00120183, sem FILE_CREATE_PIPE_INSTANCE, WRITE_DAC, WRITE_OWNER ou DELETE explícitos. Owner privilegiado/administrador permanece no threat model confiável vigente. Não conceder escrita a Users/Everyone/Authenticated Users. FILE_FLAG_FIRST_PIPE_INSTANCE e limite de uma instância rejeitam colisões; nenhuma adoção, retry ou alteração de ACL existente.

O supervisor mantém o handle ORIGINAL retornado por CreateProcess, PID/criação/liveness e token primário do filho. GetNamedPipeClientProcessId deve corresponder a esse processo original, não a um PID reaberto ou nome. Verificar TokenUser, AuthenticationId, sessão e ausência de impersonation inesperada; elevação e identidade exata continuam sob N0/B+D, não são concedidas pelo IPC.

O filho abre e mantém processo do supervisor, verifica criação/lifetime/contexto contra o vínculo aprovado e confere GetNamedPipeServerProcessId. A abertura inicial por PID é aquisição candidata, NÃO equivale ao handle original do pai: é aceita como identificação somente dentro do plano protegido e das verificações canônicas posteriores. Claims textuais/argv não constituem autoridade. O protótipo não autentica sozinho o arquivo de imagem do operador/qualificador; pins, arquivo original e controle de substituição existentes permanecem obrigatórios antes da admissão.

O endpoint só aceita um cliente. Um processo errado que chegue primeiro causa DENY, sem reconectar para contornar a falha. Cliente usa SECURITY_ANONYMOUS para não conceder impersonation ao servidor. Nenhuma rotina chama ImpersonateNamedPipeClient.

## T3 — bootstrap e canais

Conectar uma vez, autenticar servidor no filho, emitir hello sem autoridade; supervisor valida cliente original e só então confirma canal. Hello/ack não carregam aprovação, intent, plano de controle ou autorização de Stop. Bytes de sessões anteriores e conexão de outro processo são rejeitados pela identidade do destinatário original e pelo endpoint de uma única sessão; o canal não fornece uma segunda exactly-once authority. Nenhum intent/SCM/serviço pode depender apenas desse hello/ack. Depois, o mesmo endpoint transporta stdout. Conteúdo ASCII/UTF16 hexadecimal/protocolo atual, truncamento e recibos precisam permanecer equivalentes no adapter futuro.

Usar writer FILE/descriptor explícito via _open_osfhandle(_O_NOINHERIT) e _fdopen, ou WriteFile com ownership equivalente. Proibir _dup2: a implementação UCRT inspecionada duplica com herança TRUE. SetStdHandle sozinho não redireciona a estrutura FILE stdout do CRT. O adapter deve encaminhar TODOS os emissores relevantes ao stream explícito e preservar fflush/erros/primeira falha, sem logger novo ou alteração do wire.

Stdin/stderr permanecem no console já compartilhado; não duplicar handles de console entre processos, nem criar outro console operacional. Qualificar Read-Host/Console.ReadKey, confirmações, Escape/F10, console loss e stderr no launcher existente antes de ativar. A prova nativa de console isolado não substitui essa qualificação PowerShell.

## T4 — ownership e UNKNOWN

Owner de transporte é contexto limitado dentro do supervisor já existente, sem autoridade paralela. Pipe e evento de overlapped têm ownership persistente antes de adquirir. Handle de processo ORIGINAL é retido até prova de término. Controlar pending I/O; CancelIoEx não prova fechamento. Esperar conclusão factual por GetOverlappedResult antes de reutilizar evento/buffer ou fechar. Timeout/close contraditório mantém UNKNOWN/refs e bloqueia novas aquisições; sem retry, kill ou close duplicado.

No filho, ownership pipe nativo → descriptor → FILE é explícito; aliases SetStdHandle/borrow não são segundos owners. fclose falho é incerto e não permite CloseHandle adicional. Em falha de hello/ack, não registrar canal aceito; preservar erro original e resolver pending/fechamento por caminho governado, sem saída direta que descarte o inventário. Revalidar a identidade/lifetime no ponto de uso; peer encerrado invalida. Propagar saída incompleta, erro de escrita e falha de recibo ao mecanismo canônico existente. Término de processo, stream fechado e console perdido não significam quiescência global.

## T5 — orçamento

Manter128 nativo/1024 administrativo, quatro papéis,60s/1s, headroom, NONE_GRANTED e UNKNOWN históricos. Supervisor requer processo+endpoint+evento em steady state; evento é custo adicional frente ao reader de pipe anterior, não crédito deslocado. Cliente adiciona endpoint e referência transitória do supervisor, além de tokens temporários de autenticação. Fechar apenas referências cujo último uso seja demonstrado. Não reutilizar allowance histórica ou presumir redução da contagem real.

O contexto NT_OWNER atual mede144 bytes e a guarda nova de contagem de aberturas tem4 bytes escalares próprios no fixture; isso não é limite de custo total nem orçamento concedido. Estado compilado do protótipo, buffers/frames, allocations de segurança, FILE/CRT, BCrypt, PSS e kernel devem ser distinguidos. Dados de fixture não concedem orçamento. Falta de capacidade na admissão fresca causa DENY antes de consumo.

## T6 — invariantes e threat model

Exclusividade canônica, exactly-once, anti-replay, Stop/UNKNOWN/retention e identidade/config/plano/aprovação/dossiê protegidos continuam obrigatórios. IPC transporta bytes, não autorização. Não ampliar N7 para autenticação do console ou imagem mapeada. Confiar no Windows e operador autorizado como contratos atuais; não afirmar isolamento contra kernel comprometido, administrador equivalente ou injeção no processo confiável.

A segurança contra herança demonstrada limita-se aos novos objetos privados não herdáveis. Não transforma handles históricos arbitrários do PowerShell em seguros nem permite fechá-los. Confirmação interativa e Stop não são autorizados por nome do pipe, nonce, PID ou flag.

## T7 — evidência isolada

Protótipo executável em tests/noninherited_transport_test.c usa IPC real, DACL lida do objeto original, identidade recíproca e canais CRT sem _dup2. Concorrente global do supervisor e descendente do filho não recebem os objetos privados: comparação por objeto, não apenas contagem. Casos corretos/incorretos, criação divergente, restrição de token simulada, servidor divergente, colisão, CreateProcess falho, fechamento do servidor, saída precoce, close incerto e console são cobertos. Logs/hashes/resultados em noninherited-transport no pacote integrado.

O protótipo é processo isolado; não é qualificação do PowerShell integrado, de outro usuário real, de ACL negativa com credenciais diferentes ou do ciclo LAB. Restrição token divergente no teste é fault injection da condição, não execução cross-account. A ausência de impersonation do thread cliente deve ser verificada antes da abertura e depois do ack; a igualdade dos tokens primários não a substitui. O teste novo usa ImpersonateSelf real no próprio processo para provar negação antes do endpoint, sem credencial de outro usuário. Novas provas da revisão2: validação pura de namespace/IDs antes de I/O, replay de bytes de sessão antiga por processo errado, saída do peer entre hello/ack, falha de escrita do ack e cleanup factual. Casos de replay não provam resistência a código comprometido dentro do processo já autorizado; TokenId/ModifiedId/elevação e plano continuam com revalidação canônica, não são substituídos pela igualdade de AuthenticationId. Não inferir redução positiva real nem toda equivalência de printf/wprintf/fflush a partir do payload fixo.

## T8 — integração condicionada

Somente após decisão normativa focal permitir este novo transporte: adapter mínimo no mesmo launcher/qualificador, opt-in read-only e default EMPTY_DENY, positiva DENY. Preservar fonte/pins/approvals históricos; novos hashes e pedido específico. Exigir quatro configurações Selected/Legacy Debug/Release conforme B+D R7 para as fontes afetadas, sem warnings, e qualificar dispatcher completo stdout, confirmação/Stop PowerShell, endpoint novo e custo administrativo antes de solicitar observação manual. Nenhum novo pedido deve ser aprovado por inferência ou usar registros consumidos.

Não instalar infraestrutura paralela. Não executar Prepare/Start/Stop, intent ou SCM/ACL nesta decisão. Mudança de transporte não autoriza alterar os inputs RAW1 existentes ou a identidade normativa da sucessora.

## T9 — alternativas e decisão solicitada

HANDLE_LIST foi rejeitado para esta necessidade: exposição inversa reproduzida. DuplicateHandle não autentica destinatário, requer canal de bootstrap para informar o valor remoto e acrescenta lifetimes; handles de console têm restrições cross-process. Helper de transferência viola a topologia atual. Serialização local não coordena criadores externos.

Recomendação técnica: aceitar T0–T9 como exceção focal de transporte a N5/B+D somente após revisão oficial integral, preservando todos os demais contratos. Esta é proposta preparada para decisão do owner; não há registro ACCEPTED nem integração ativada. Para futura aceitação, registrar a revisão aprovada em decisão normativa designada, preservando integralmente N5 accepted e esta proposta original como histórico; a precedência é somente a emenda textual e o escopo acima. Não considerar este documento ou seu hash uma aprovação executável. Enquanto pendente, manter launcher/pins operacionais intactos. Depois do aceite, aplicar e qualificar o adapter no escopo técnico autorizado, sem operação LAB automática.

DECISION=PROPOSED
CANONICAL_INTEGRATION=NOT_ACTIVATED
OPERATIONAL_APPROVAL=ABSENT
POSITIVE_CAPACITY=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
