from pathlib import Path
import json,hashlib,csv,subprocess,re
root=Path.cwd();base=Path('docs/integration/n0start03-integrated-closeout-20261024');p=base/'real-console-role-binding-20261027';sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest();adr=Path('docs/adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md');ah=sha(adr);exe=p/'Release/ConsoleRoles.exe';eh=sha(exe)
report=f'''# Topologia real de console — evidência nativa e binding proposto

TASK=PACC-PHASE6-ED11-REAL-CONSOLE-ROLE-BINDING
Pacote iniciado2026-10-27; a pasta preserva a identidade documental da investigação. Não é nova operação TPM/LAB.

## Resultado técnico

Foi encontrado um auxiliar REAL distinto do pai/cliente HWND: conhost.exe PID20144, criação134376272949369467, referenciado por consulta nativa ao handle original do PowerShell17204. O bloqueio não demonstra ausência desse auxiliar: o locator HWND não o identifica neste ambiente. Foram implementados um observador nativo e testes reais de console clássico/ConPTY, concorrência, identidade, fechamento e UNKNOWN. Nenhum código funcional da prova, launcher, contrato accepted ou pin histórico foi modificado.

O alvo ainda NÃO está qualificado para outra prova. Associação autenticada completa/mapped image permanecem UNPROVEN; a lista de clientes atual do console elevado não pôde ser obtida pelo observador não elevado. Uma coleta herdada manual SEM TPM está pronta. [ADR focal C0–C9 PROPOSED](../../../adr/{adr.name}), SHA256 `{ah}`; não aceita, sem autoridade operacional.

## Topologia factual nova

[Dados por participante](PARTICIPANT-IDENTITIES.csv), [leituras concorrentes do alvo](FACTUAL-ROLE-AND-ACCOUNTING.json), [saída1](OWNER-FINAL-CONCURRENT-1.json) e [saída2](OWNER-FINAL-CONCURRENT-2.json).

| Papel | PID / criação FILETIME | Evidência atual |
|---|---|---|
| Operador, processo PowerShell seed |17204 /134376272948693901|Handle original, criação antes/depois, sessão1, TokenUser SID autorizado...1108, elevado, arquivo original e métricas|
| Processo associado ao HWND histórico |17204|O recibo anterior o iguala ao pai; não o confundimos com host real. HWND atual do console elevado ainda não observado por este coletor|
| Auxiliar candidato real |20144 /134376272949369467|Handle original estável, sessão1, SID...1108/elevado, parent metadata17204, conhost.exe, arquivo Windows volume417614100/FileID327680:1591236/hashb02ee54f...|
| Ancestral desktop observado |7200 /134375241536646020|explorer.exe, mesmo usuário, não elevado; observado separadamente como ancestral/baseline, não host do console|
| Prova anterior |10252 /134376273119221101 no recibo histórico|Processo indisponível agora; não reconstruímos seu token/associação ou custo por PID reutilizável|
| Observadores desta investigação |22036 e15780, criações próprias nos JSONs|Conta do proprietário, não elevados; originais distintos e refs fechadas. Não são a prova nem debitados no orçamento administrativo da futura prova|

Os nomes são descrições, não autenticação. Volume/ID/tamanho/hash do arquivo aberto, token/sessão, referências nativas e continuidade foram observados. Igualdade de arquivo não prova imagem mapeada; assinatura não é presumida. O seed esperado foi fixado por criação, não somente PID. O seed não ganhou autoridade por ser parâmetro do coletor.

NtQueryInformationProcess49 no seed retornou NTSTATUS0,8bytes, raw20145=0x4eb1. Máscara dos dois lowbits leva a20144, flag1. O handle original do candidato retorna raw17206=0x4336/flag2, metadado reverso ao pai; isso não é outro host. Não seguimos todo PID mascarado indiscriminadamente. Explorer raw6484/flag0 também não é host desse console. Raw/status/tamanho foram repetidos sobre os mesmos handles, sem mudança de criação ou término observado.

Essa informação é DISCOVERY METADATA, não autenticação. [phnt](https://github.com/winsiderss/phnt/blob/master/ntpsapi.h) identifica a classe e tipo; [libuv](https://github.com/libuv/libuv/blob/v1.x/src/win/tty.c) consulta a classe e separa os lowbits para descobrir conhost. [Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-wininterl-ntqueryinformationprocess) adverte que APIs nativas podem mudar (URL correta nos sources abaixo). Não usamos setter, debug injection, SeDebugPrivilege, substituição de token ou UAC bypass. O campo é metadado configurável, não vínculo criptográfico.

## Contabilização sem duplicação

Primeira leitura parcial: PowerShell711 + conhost266 =977handles. Duas leituras concorrentes finais: PowerShell618 + conhost266 =884, observadas sequencialmente mantendo handles de ambos vivos.140 até1024 naquele intervalo, não garantia futura. Não atribuímos711→618 a este patch; o processo alvo permanece dinâmico. Não somamos máximos históricos ou leituras de relógios diferentes para criar teto.

Conta-se o operador uma vez mesmo quando também é pai e HWND owner. Conta-se o auxiliar REAL20144 separadamente. O observador/próxima prova tem orçamento próprio128 e guard124; seu custo não é transferido para administração nem subtraído da tabela do processo. Os leitores concorrentes fazem observações independentes; não se somam884+884 como se fossem quatro administradores.

Explorer3297handles foi medido e aparece no relatório, não ocultado nem considerado zero. Pela composição B+D, baseline OS/desktop externo é distinto de processos relacionados à operação. Ser ancestral não o torna automaticamente actor administrativo; se uma futura seleção depender de processo adicional UI/broker como participante, deve identificá-lo e cobri-lo ou negar. Não afirmamos ter feito censo completo do desktop ou provado ausência de todo terminal/broker. Não excluímos processo necessário para fabricar PASS. Classificação/abrangência do orçamento completo ainda precisa do binding e lista de clientes reais.

Private Bytes e WorkingSet por processo estão nos JSONs. Threads/stack-VA/custos kernel/transitivos/unsampled peak continuam fora desta prova e não recebem crédito.884 é SUBTOTAL DOS DOIS OBJETOS DESCOBERTOS, não orçamento/admissão ED11 PASS. N7, recursos e freshness aplicáveis não foram relaxados.

## Observador e limites de contexto

[ConsoleRoles.c](ConsoleRoles.c) abre referências originais, coleta criação/token/sessão/arquivo por handle, hash streaming, métricas e revalidação de associação/lifetime. Slots bounded, reserva de temporário antes da aquisição; nenhuma aquisição após freeze; primeiro erro e referência incerta retidos. Contabilização por identidade única; alias de papel no mesmo objeto reutiliza slot, PID/criação divergente nega. Arquivo público current-on-disk não é mapped-image attestation. Fonte de autorização não é o JSON; todos os recibos dizem authority=DENY.

O sandbox executa sob outro SID (...1003), explicitamente distinto do operador. Execução fora dele usa SID...1108, porém sem elevação; não é o token elevado da prova. Não se autentica operador usando a identidade da conta restrita. Nenhum processo-alvo teve token, handles globais, memória ou flags alterados.

Na tentativa de associação externa, antes havia2clientes no console do observador; FreeConsole retornou TRUE/erro0 e depois0clientes. AttachConsole(17204) falhou5; o observador continuou sem janela/clientes desse alvo. [Discriminante integral](OWNER-FINAL-ATTACH-DISCRIMINANT.json). Não atribuímos conclusivamente o5 à elevação: a [documentação AttachConsole](https://learn.microsoft.com/en-us/windows/console/attachconsole) também descreve esse código para já-associado, e os nossos retornos não estabelecem toda a causa interna. Não contornamos UAC ou tentamos repetidamente anexar o mesmo processo.

O comando final usa console HERDADO, sem FreeConsole/AttachConsole. Precisa ser invocado pelo proprietário no PowerShell elevado já existente para fornecer a evidência que o contexto do agente não alcança. Não há nova aprovação TPM a consumir. A criação esperada17204 impede adoção silenciosa de PID reutilizado. Esse é o limite real de observação atual.

## Correções e custos efetivamente encontrados

A verificação opcional Authenticode/offline, aplicada inicialmente ao ancestral Explorer, carregou dependências e fez o observador atingir213handles. O guard negou; resultado preservado em ACTUAL-ROLE-OBSERVATION-OWNER-CONTEXT.json, nenhuma redução de quota ou atribuição de zero. Essa verificação foi separada da coleta de topologia, que não concede autoridade de assinatura: o recibo final marca trust_queried=false/trust=null e mapped image UNPROVEN. Não foi removida uma verificação obrigatória do host de prova — esse host e seus pins não mudaram. Qualificação de proveniência obrigatória permanece gate antes de futura admissão.

Outro ajuste focal do protótipo: preservar e interpretar lowbits como metadados tipados em vez de tratar backlink/ancestral como segundo auxiliar. Um host de console do processo ancestral observado é BASELINE_HOST64, não auxiliar do seed. A descoberta de hosts de outros contextos fica explícita, sem duplicar o subtotal do alvo.

Pico próprio final de coleta fora do sandbox85handles; leitores concorrentes87. Ambos abaixo de124/128 nesse escopo sem TPM. Primeiro protótipo103 com outros providers e213 no ramo Authenticode não são apagados. Não comparar diretamente com94 do host de prova nem declarar economia operacional. Refs originais finais0/retainedfalse; internos/caches/globais permanecem UNKNOWN. Aquisições após freeze bloqueadas. Falha de fechamento real reteria o processo coletor vivo, sem retry/fechamento forçado.

## Testes nativos novos

[ConsoleFixtures.c](ConsoleFixtures.c), [Debug](Debug-FINAL-v2-tests.log), [Release](Release-FINAL-v2-tests.log), [resultados estruturados](FINAL-FIXTURE-RESULTS.json).

Quatro builds finais: observador+fixture x Debug/Release /W4 /WX /GS /sdl, zero warnings, MSVC/x64 moderno. Não reconstruímos produto/launcher/prova TPM, nem repetimos os antigos testes de alias-denial.

40checks por configuração, dos quais26 do novo modelo de identidade/ownership: identidade única/dedup, geração divergente, flags/width/status/overflow da classe49, reserva BUSY, capacidade saturada, Stop antes de adquirir, fechamento incerto injetado sem chamar CloseHandle inválido e sem segundo fechamento. O original sintético incerto fica retido até perda do processo fixture; não se declara fechamento ou leak-free global por essa perda.

Oito filhos nativos finitos no total (quatro por configuração): CREATE_NEW_CONSOLE, CreatePseudoConsole real e dois leitores/console distintos concorrentes. Há referência original do processo e término aguardado; nenhum TerminateProcess/kill. Processo encerrado não é live, original conhecido fechado. Todos os JSONs finais first=NONE/refcount0/retainedfalse. Pico de cada observador-fixture77, controller87. Console clássico apresentou HWND do próprio cliente; ConPTY apresentou HWND do host. Isso é evidência experimental dos ambientes isolados, não reclassificação automática do terminal do proprietário.

ClosePseudoConsole retornou; seu retorno é void e fechamento interno não é totalmente observado. Pipes/process/thread refs controladas foram conferidas. Dois testes executáveis adicionais com criação errada negaram2/SEED_CREATION_MISMATCH, refs0: [resultados](NEGATIVE-EXIT-RESULTS.json). Falhas iniciais de build (quoting /Fo e nome bind colidindo com declaração WinSock) corrigidas; builds finais sem supressão de warnings ou troca de toolchain. Logs de etapas anteriores preservados.

## Arquitetura e próximo gate

Não precisamos criar um auxiliar artificial para substituir um inexistente:20144 existe e foi medido. A alteração necessária é o locator/binding; HWND continua metadado, não o identificador do auxiliar. C0–C9 propõe binding explícito por originais e descoberta nativa repetida, proteção por token/arquivo/sessão/lifetime, soma única de operador+auxiliar, lista exata de clientes e aceitação residual NOVA e restrita ao experimento matemático. Não se reutiliza a N7 histórica para uma identidade não coberta. Se esse risco não for aceito, DENY até vínculo autenticado suficiente.

O guard funcional e pedido consumido não mudaram. ADR C0–C9 PROPOSED SHA256 `{ah}`. Antes de integrar ao host da prova: decisão explícita, evidência de clientes/contexto herdado real, qualificação de todos os recursos exigidos e atualização prospectiva dos pins. NÃO foi preparado novo pedido TPM: topologia real integral ainda não qualificada. Aprovação4a5f permanece usada.

[Coleta manual read-only preparada](MANUAL-READONLY-HANDOFF.md), executável `{eh}`. O comando usa somente o observador e PID/criação já conhecidos; sem TPM/TBS/NV/Start/Stop/SCM/ACL, sem intent/Q4, sem aprovação consumida. Depois do JSON do console elevado, os testes e a integração técnica podem continuar dentro da decisão normativa efetivamente autorizada, e só então avaliar um novo pedido TPM.

E21=PENDING, E17/E18 serviço/lifecycle e E19/E20 alvo composto/D4/D5 independentes, E22 derivado.4demonstrados/12limitados/6pendentes. ED11/Phase6 PARTIAL. Históricos/pins/WIP e reservas/UNKNOWN preservados; nada de commit/worktree/limpeza/refund/CURRENT/assinatura/recuperação.

Sources: [Microsoft NtQueryInformationProcess](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess), [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow), [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid), [FreeConsole](https://learn.microsoft.com/en-us/windows/console/freeconsole). A ABI interna e limites de descoberta não foram transformados em promessa de autenticação.

CURRENT_TASK_COMPLETED=YES_TECHNICAL_INVESTIGATION_AND_QUALIFICATION_WITH_REMAINING_BOUNDARY_EXPLICIT
REAL_AUXILIARY=DISCOVERED_ORIGINAL_IDENTITY_OBSERVED_NOT_ADMITTED
ADMIN_PAIR_HANDLES=884_OBSERVED_977_EARLIER_NOT_FRESH_BUDGET
ROLE_ACCOUNTING=UNIQUE_PROCESS_OBJECTS_QUALIFIED_IN_FIXTURE
REAL_ELEVATED_CLIENT_LIST=NOT_OBSERVED
CONTRACT_REVISION=C0_C9_PROPOSED_NOT_ACCEPTED
BUILDS=4_PASS_ZERO_WARNINGS
TESTS=40_CHECKS_PER_CONFIGURATION_PLUS_CREATION_NEGATIVES_PASS
TPM_Q4_START_STOP=NOT_EXECUTED
SCM_ACL_MUTATIONS=NONE
NEXT=OWNER_INHERITED_READONLY_EVIDENCE_AND_EXPLICIT_CONSOLE_BINDING_DECISION
'''
report=report.replace('nf-wininterl-','nf-winternl-')
(p/'CONSOLIDATED-CLOSEOUT.md').write_text(report,encoding='utf-8')
# Live current state, no gate promotion or normative acceptance.
m=base/'ED11_GATE_MATRIX.csv';(p/'ED11_GATE_MATRIX.PREIMAGE.csv').write_bytes(m.read_bytes());rows=list(csv.DictReader(m.open(encoding='utf-8',newline='')));fields=list(rows[0]);g=next(x for x in rows if x['gate']=='E21');g['current_evidence']+='; actual original-handle class49 discovery PS17204/creation134376272948693901 -> conhost20144/creation134376272949369467; pair977 then884handles, desktop baseline separately observed; new classic/ConPTY/concurrent fixtures40checks/config and4builds zero warnings; elevated client list inaccessible/Attach5; real-console-role-binding-20261027/CONSOLIDATED-CLOSEOUT.md';g['closure_evidence_or_action']='Inherited owner-elevated nonTPM client/role observation then explicit narrow C0-C9 binding/risk decision and qualified integration before any new TPM request. No reuse of approval4a5f; actual possession/custody/NV/Q4 remain pending.'
with m.open('w',encoding='utf-8',newline='') as f:w=csv.DictWriter(f,fieldnames=fields);w.writeheader();w.writerows(rows)
with Path('docs/capabilities/canonical-phase6-consolidation.md').open('a',encoding='utf-8') as f:f.write('''\n\n## Real console role binding — native discovery after topology denial\n\nIndependent nonTPM observer found a distinct held conhost20144, creation134376272949369467, linked by native class49 discovery to PowerShell17204, creation134376272948693901. Both original identities/token/session/on-disk file/metrics stable; pair977 then884handles, not fresh admission; Explorer ancestry/baseline explicit. HWND is not auxiliary identity. Real target client list still inaccessible to non-elevated observer (FreeConsole TRUE then AttachConsole5); current reason not conclusively elevation. Inherited owner-console readonly command prepared, no TPM approval reused. Four modern builds zero warnings; classic/ConPTY/concurrent native fixtures40checks each configuration plus creation negatives PASS. C0-C9 candidate for explicit host binding and narrow risk acceptance remains PROPOSED; proof functional guard/pins unchanged. [New evidence and boundaries](../integration/n0start03-integrated-closeout-20261024/real-console-role-binding-20261027/CONSOLIDATED-CLOSEOUT.md). E21 pending; ED11/Phase6 PARTIAL; no TPM/Q4/SCM/ACL/Start/Stop or authority.\n''')
# Scope-specific pins and final verification; preserve older snapshots as historical.
files=[p/'ConsoleRoles.c',p/'ConsoleFixtures.c',p/'Debug/ConsoleRoles.exe',p/'Release/ConsoleRoles.exe',p/'Debug/ConsoleFixtures.exe',p/'Release/ConsoleFixtures.exe',adr]
(p/'FINAL-ARTIFACT-PINS.json').write_text(json.dumps({str(q).replace('\\','/'):dict(bytes=q.stat().st_size,sha256=sha(q)) for q in files},indent=2)+'\n')
assert subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()==(p/'HEAD-BEFORE.txt').read_text().strip()
(p/'WIP-AFTER.txt').write_text(subprocess.check_output(['git','status','--short'],text=True))
(p/'SHA256SUMS.json').write_text(json.dumps({str(q.relative_to(p)).replace('\\','/'):sha(q) for q in p.rglob('*') if q.is_file() and q.name!='SHA256SUMS.json'},indent=2)+'\n')
print('FINAL_REPORT='+str(p/'CONSOLIDATED-CLOSEOUT.md'));print('NORMATIVE_CANDIDATE='+ah);print('READONLY_OBSERVER='+eh)
