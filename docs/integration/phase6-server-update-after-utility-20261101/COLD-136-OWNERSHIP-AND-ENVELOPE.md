#136handles — ownership e aplicabilidade do envelope

Task: análise offline focal, 2026-11-01. Branch main,
HEAD f341985924d29870d8ff627ed37c8a0874ad6469 e WIP preservados.
Nenhuma coleta nativa, sessão, tracing, alteração de código ou efeito protegido.

## Conclusão

A guarda é aplicável EXPRESSAMENTE ao controlador SCQ frio desta sessão, não
um limite universal de executáveis Windows. A contagem136 é fato; não foi
demonstrado defeito de contabilização, duplicação dispensável ou transposição
indevida do envelope. A elegibilidade operacional foi corretamente negada.
Também não foi demonstrado que136 seja o mínimo necessário do desenho: origem
individual e lifetime do residual permanecem UNKNOWN. Não concluir que a quota
é tecnicamente adequada a todo host ou que precisa aumentar por conveniência.

## Cláusulas determinantes

- G3 exige participantes e contagem completa, distinguindo custos demonstrados,
  amostras e tolerâncias. G1 especializa apenas máximos opacos, não quotas.
- G4: “Contabilizar handles completos de cada participante, sem descontar
  console/herdados/loader ou aliases por conveniência.” Preserva124/128/1024
  nos domínios SCQ e guardas de cada referência.
- INITIAL-QUERY-AND-G3.CANDIDATE.md, incorporado accepted pelo SHA256
  5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca:
  tabela G3 handles128/guard124; parágrafo de gates exige negar “antes de nova
  referência com count>=124”; declara que contagem inicial maior não pode ser
  subtraída. O texto menciona expressamente novo controlador, não só receptor.
- SSO-B S6, alternativa B cross-principal regra7 e TO7 preservam esses limites.
  QUERY B3 preserva guarda anterior a novas aquisições, não reduz autoridade.
- Portanto pré-READY/pré-confirmação não exclui o controlador do domínio.
 1024 não substitui128 do participante por ser execução administrativa.
- A análise anterior de staging identificou ausência de extensão accepted a
  um ato diferente. D0–D7/B0–B8 não dispensam SCQ e não se transportam aqui.

Fontes primárias lidas: FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md G0–G9,
SHA256 d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b;
INITIAL-QUERY-AND-G3 e respectivos DECISION accepted; DECISION de SCQ B,
SSO-B e TRANSPORT_ONLY e cláusulas incorporadas. Hashes G/QUERY reconferidos.
Não há conflito normativo demonstrado. Nenhum accepted foi alterado.

## Corte factual exato e inventário

Transcript010193...864a9e reconferido. stage2/reference0/query_succeeded1/
own_count136 registra GetProcessHandleCount(GetCurrentProcess()) imediatamente
antes de NtCreateFile do primeiro request. Não conta handles de outro processo.

| Componente | Estado antes desse corte | Ownership e lifetime |
| --- | --- | --- |
| CRT/loader/DLLs e handles herdados | Já podem existir antes de main; quantidade/tipos desconhecidos | Não são handles descartáveis do owner por simples presença; não atribuir a Windows/OpenSSL/PowerShell sem fato |
| stdout/stderr/console padrão | Streams emprestados ao CLI, não abertos/duplicados pela entrada fria | Pode haver aliases; não contar3 por pressuposto nem fechar com CloseHandle/fclose; necessários ao diálogo/evidência |
| owner_acquire | Root estático, contadores/reserva, inicializadores inertes; empty network/config têm zero listeners/bind IDs | Neste caminho não entra nas alocações condicionadas a count/listener_count; não cria lote de handles de rede/SCM |
| console Stop bridge | SetConsoleCtrlHandler registrado antes do plano | Sem HANDLE aplicativo devolvido; internos opacos; uninstall no encerramento conhecido, nunca remover Stop para passar |
| unexpected_thread | OpenThreadToken deve falhar ERROR_NO_TOKEN para continuar | Sucesso abortaria antes; nenhum token de impersonação aceito no corte |
| personal_group x3 | SID local e CheckTokenMembership(NULL,...) já executados | Buffers locais; possíveis recursos internos da API não são referências adquiridas para fechamento pelo produto |
| bootstrap_token | Um OpenProcessToken(TOKEN_QUERY) bem-sucedido, retido no pending | Um handle próprio explicitamente identificável; token original necessário à prova/revalidação posterior, fecha uma vez no native_close |
| check_token | Só em personal_token(capture=0), posterior | Não adquirido pela chamada capture=1 neste corte; não é duplicação ativa a eliminar |
| known folders | SHGetKnownFolderPath ProgramFiles e ProgramData, DONT_VERIFY, executados e retornados | PWSTR liberados por CoTaskMemFree, não HANDLEs; efeitos internos de shell/allocator opacos, sem CloseHandle legítimo correspondente |
| três scq_cold_raw | Todos ainda vazios, falha ANTES do primeiro NtCreateFile | ZERO handles de arquivos originais desse laço adquiridos até o corte |
| descritores/hash/CSPRNG do plano | Aquisição do SD e hash após open; nonce após os3 arquivos | Não alcançados; não atribuir o pico a esses buffers/providers posteriores |
| processo/thread correntes | GetCurrentProcess/GetCurrentThread usados como pseudo-handles | Não criar contagem adicional ou fechar; não são handles reais adquiridos pelo produto |
| input duplicates e empréstimo SCQ | session_acquire_originals só após scqc_plan completo | Não alcançados; duplicações posteriores têm direitos/lifetime próprios, não causam este136 |
| diretórios protegidos/readbacks | O plano frio resolve strings; aquisições subsequentes não chegaram | Não atribuir a handles do UPDATE ou wrapper, que é outro processo/invocação |
| SCM, peer/process/token runtime, pipe/event e namespace | Só em estágios posteriores ao plano/atos | Não alcançados; nenhum Start ou endpoint autorizado por esta análise |

Código auditado: main.c.api3.inc entrada SERVICE_CLI, launcher_owner_acquire,
server_launcher_scq_cold_start_win32.inc, personal_scq_cold_start_win32.inc,
personal_scq_cold_acquisition_win32.inc, server_personal_token_checks_win32.inc,
personal_install_paths_win32.inc, server_console_win32.c e native_close.
O dispatch frio precede o caminho normal de configuração/rede; inicializadores
de loader anteriores a main continuam fora do inventário estático observável.

Não se conclui que os outros135 sejam herdados, vazamentos ou dispensáveis:
apenas um handle de token tem aquisição/lifetime explícitos demonstrados nesse
trecho. Recursos internos e estado de entrada não foram separados na evidência.
Coortes de processos anteriores não são incorporadas como handles deste processo
por mera retenção histórica; suas obrigações tampouco são consideradas resolvidas.

## Redução segura e alterações

NENHUMA redução segura demonstrada. Fechar bootstrap_token romperia identidade
original; remover Stop alteraria garantias; fechar std/handles herdados/CRT ou
internos não possui prova de ownership. Não foi encontrado DuplicateHandle
alcançado nem aquisição repetida do token nesse corte. Reordenar só a consulta
para antes dos custos reais esconderia recursos e não atenderia gate pré-efeito.

ProgramData é resolvido pelo helper full-layout compartilhado mesmo que este
trecho use binary. Isso não prova que sua remoção elimine handles: nenhum
delta por chamada foi medido, e mudar o mode NEW para BINARY_ONLY pode contrariar
os callers/revalidação seguintes. Não executar uma troca especulativa só para
tentar PASS, nem afirmar que CoTaskMemFree descarrega estado interno de shell.

Sem defeito funcional comprovado, nenhum código/pin/imagem foi alterado;
nenhum build/teste repetido. Evidência já qualificada: quatro builds sem warnings,
20pares CTest; cold_open25casos/81checks distingue123 permitido,124/128 negados,
falha de query/SD/times/exit e UNKNOWN. Isso não atribui a origem real dos136.
Future docs descobertos não contêm contrato que substitua G3/G4 para este corte.

## Primeira lacuna e menor evidência útil

Faltam os estados de contagem do MESMO processo antes dos três grupos já
executados: registro do bridge, captura de token/grupos e resolução knownfolders.
A linha atual não possui esses cortes; não reconstruí-los de amostras de outro
processo, picos de UPDATE ou coletas antigas132. Não há prova de efeito de console
ou shell suficiente para autorizar fechamento de seus objetos internos.

A menor observação futura útil deve distinguir entrada pré-owner, pós-bridge,
pós-token e o corte pré-open já existente. O quarto deve REUTILIZAR a consulta
original registrada, não criar um novo número como se fosse o mesmo instante.
Consultas escalares não concedem autoridade nem prova de ausência de pico entre
elas; acrescentam custo temporal. Executar somente com autorização específica,
em percurso estritamente read-only encerrado antes de originais/RNG/atos/SCM/
endpoint, sem promover o percurso observado a sessão aprovada. Preparar desenho
offline somente se solicitado; não instalar instrumento ou nova imagem agora.

Se já exceder no primeiro corte, essas diferenças não bastam para decidir quais
handles fechar: será necessário atribuir ownership de recursos de entrada em
escopo restrito, antes de propor fechamento. Se aumentar num grupo, atribui a
fase, não um leak ou dispensabilidade de cada handle. Não prometer que outra
contagem resolverá o problema. Não iniciar tracing global ou criar infraestrutura.

Alternativa normativa só existe mediante decisão explícita: especializar G4 e
a seleção QUERY/G3 e sua interação com B/SSO-B/TO, preservando contabilização,
headroom e riscos. Não há base para escolher outro número, aplicar1024 ou abrir
essa proposta como atalho sem decisão do proprietário. Nenhuma proposta nova
foi criada; inaplicabilidade não foi comprovada.

Caminho mínimo: distinguir ownership no corte exato; somente correção segura
demonstrada e qualificada pode alterar binário; se isso exigir disponibilização,
UPDATE tem autorização independente. Depois preparar nova sessão E/Q/S legítima,
nunca reutilizaraf8529...67bc9 ou recriar decisãoef8b...e199. Recursos/bilateral/
receipt/close permanecem gates reais. Causa1816 histórica anterior, retenção
histórica e OS_QUIESCENCE UNKNOWN. Readiness produtiva DENY; ED11/Phase6 PARTIAL,
E21 PENDING, PST0.6.4 intacto. Nenhum retry, coleta ou efeito real autorizado.
