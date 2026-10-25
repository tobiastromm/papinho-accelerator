---
status: proposed
revision: 4
date: 2026-10-24
scope-level: project
scope-target: explicit child-handle inheritance for the existing single N0START-03 LAB successor launcher
supersedes: []
---
# Candidate — herança explícita no nascimento do qualificador N0START-03

## Contexto factual

Observação manual N0HT-20261024-01:71→105→126 handles no qualificador;1→9→27 slots controlados identificados;70→96→99 entradas sem owner mapeado. As12 entradas com HANDLE_FLAG_INHERIT incluem stdin/stdout/stderr e9 candidatas não stdio (6Section,1Event,2File). Flag não prova origem, identidade entre capturas ou dispensabilidade. Não declarar que remover9 produzirá117, nem que todo custo restante veio de SCM. O fixture anterior provou herança de8 eventos controlados e sua ausência quando não herdáveis, no mesmo mecanismo ProcessStartInfo.

Ver [reconciliação](../integration/n0start03-integrated-closeout-20261024/handle-attribution/MANUAL-RESULT-AND-ARCHITECTURAL-DECISION.md) e o registro original; não repetir a observação consumida.

## I0 — status e escopo da decisão proposta

Somente direção PROPOSED para a criação do qualificador, na operaçãoN0START-20261024-03/tentativa0b1de869e11493b7cb08fb329578f6ce e host/boot/serviço exatos já fixados em L1/N0. Não constitui admissão, novo serviço, novo owner, registry, quota, intent ou aprovação operacional. Não editar decisões accepted. Não afirmar que o contrato vigente proíbe toda implementação de herança seletiva: a escolha durável de excluir capacidades implícitas e sua qualificação são o objeto desta proposta.

## I1 — contrato fechado de capacidades no nascimento

Propor que o único PowerShell5.1 x64 já elevado passe explicitamente ao filho somente os canais stdio necessários: stdin/console existente, stdout no pipe supervisionado e stderr conforme o console existente. Qualquer outro handle necessário deve ser identificado por finalidade, direitos, owner e lifetime e revisado explicitamente; não autorizá-lo por nome, número, pré-existência ou flag. Lista vazia, canal inválido ou necessidade não prevista => DENY. Isso não autentica o console auxiliar nem amplia N7.

A implementação candidata usaria CreateProcessW/STARTUPINFOEX/PROC_THREAD_ATTRIBUTE_HANDLE_LIST no MESMO supervisor e contexto de token, sem segundo PowerShell/launcher/processo de autorização. A lista conteria somente referências reais privadas criadas/duplicadas para transferência, nunca pseudo-handles. Não alterar indiscriminadamente flags dos handles preexistentes do PowerShell. Não fechar handles desconhecidos dentro de um filho já iniciado para produzir PASS.

## I2 — autoridade e identidade preservadas

Owner/mutex canônico, operador elevado, ausência de impersonation, confirmação exata, control thread nativa, criação do processo original, dossiê/RAW1/políticas/plano e provas N0–N9 permanecem obrigatórios. O mecanismo de nascimento não pode conceder admissão. Preservar referência original retornada pela criação, vincular PID+creation e provar que a supervisão/Wait/exit-code/final receipt correspondem ao mesmo processo. Não substituir a referência por uma reabertura de PID sem prova.

## I3 — lifetime, Stop e falhas

Não criar janela de herança global: somente duplicatas privadas de transferência e pipe têm flags ajustadas, com ownership anterior à aquisição e fechamento verificado. Canal compartilhado é borrow ou transferência explícita, nunca dois owners fechando a mesma referência. Término antecipado, falha parcial de CreateProcess, falha de fechamento ou resultado contraditório preservam primeira falha/UNKNOWN e referências existentes, sem retry, kill, rollback ou liberação de mutex. Console de confirmação/Stop deve continuar interativo. Perda do console não equivale a cleanup. Stop do serviço continua separado de Stop do qualificador/supervisor.

## I4 — custos e compatibilidade

Preservar128 nativo,1024 administrativo, quatro papéis,60s/1s,headroom e NONE_GRANTED. Custos novos de pipe/duplicatas/atributos/processo/stream/managed interop pertencem ao processo correto e devem ser medidos, inclusive picos transitórios e falhas. Não deslocar handles do qualificador para outro papel para mascarar custo. Nenhum budget é concedido por esta proposta.

O alvo deste LAB é Windows10.0.19045AMD64. HANDLE_LIST é API Vista+, mas stdio/console no Windows7 exigem qualificação própria; disponibilidade não prova equivalência no PowerShell5.1. Não portar esse mecanismo a VC6/NT4 nem mudar o serviço Windows7+ para contornar uma falha. PSS do instrumento atual já exige8.1+ e não qualifica o target legado.

## I5 — qualificação antes de qualquer ativação

Reutilizar os fixtures existentes para comparar criação atual/seletiva com referências conhecidas, stdio, linhas/protocolo ASCII/hex, confirmação e console, processo vivo/encerrado, Stop/cancelamento, falhas em cada aquisição/transferência/fechamento, UNKNOWN/retention, identidade divergente, allowlist inesperada e ausência de herança de eventos privados não autorizados. Medir separadamente tabelas de supervisor/filho e memória/tempo. Preservar todas as provas e o patch Console.Out. Exigir builds/tests/pins factuais do sucessor e novo aceite exato antes de execução real; nenhuma aprovação histórica autoriza bytes novos.

Redução em fixture não é redução medida do LAB ou capacidade positiva demonstrada. Qualquer futura operação positiva exige admissão fresca e aprovação distinta; se a capacidade independente não couber, DENY. Não criar comando positivo nesta decisão.

## I6 — risco e decisão do owner

A restrição pode retirar uma capacidade implícita de componente de inicialização/Windows/software externo, ocasionando falha ou UNKNOWN antes de binding. Os dados atuais não provam que isso seja necessário nem que seja inofensivo. A direção proposta escolhe um contrato explícito de lançamento e comportamento fail-closed, em vez de prometer compatibilidade silenciosa com dependências implícitas. Não aceitar esse risco automaticamente. É uma decisão de direção arquitetural e deve ser registrada conforme governança se o owner a aprovar; a proposta não declara nenhum dos9 handles dispensável.

Alternativa: consolidar quatro objetos de política em um objeto protegido único reduziria3 referências controladas em projeção, mas troca a prova de originais independentes, exige revisão N0/RAW1/binding e não resolve por si o pico interno/cycle_live. É mais intrusiva na cadeia de provas e não é a recomendação inicial. Manter DENY é a opção caso a direção explícita não seja aceita ou qualificada; não elevar quotas.

DECISION=PROPOSED
IMPLEMENTATION_ACTIVATED=NO
OPERATIONAL_APPROVAL=ABSENT
NATIVE_EXECUTION=NOT_AUTHORIZED
POSITIVE_CAPACITY=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY


## Revisão 2 — evidência isolada e limites de ativação

A direção foi autorizada como hipótese experimental, sem aceitação normativa. Revisão1 preservada em explicit-inheritance/ADR-CANDIDATE-n0start03-explicit-child-handle-inheritance-20261024.md.preimage, SHA256457f79568a251622e7f7b53db1d7bbb384e9e3e03afc80b1ca9d92339379d20a. Esta revisão permanece PROPOSED.

DLL experimental no mesmo supervisor isolado usa STARTUPINFOEX/HANDLE_LIST com três referências privadas: duplicata de stdin, escritor de stdout e duplicata de stderr. Não está ligada ao produto nem ao launcher LAB. APIs ausentes causam falha fechada, sem fallback global. Duas execuções por variante/configuração: Debug68→56 e Release52→40 handles de entrada. CompareObjectHandles confirmou oito objetos Event presentes no controle e nenhum na lista limitada; a diferença total12 não autoriza atribuir os outros4. Não exclui por prova as nove entradas históricas reais.

Pico controlado do helper6, estável2, contexto4408 bytes e allocation lógica de atributos48 bytes; heap/loader/CLR/transitivos não são custo zero. Stdout, stderr, confirmação e Stop de protocolo sintético passaram, com referência original de processo/PID/criação. Falhas de close/lease/output congelam e retêm, sem retry/kill. Console interativo real, F10/Escape, transcript e host personalizado não estão qualificados por esses testes com pipes.

HANDLE_LIST restringe o filho desta chamada, mas não impede que outra criação global concorrente herde os transfers privados enquanto herdáveis. O fixture é serializado; não provou a exclusão de tais chamadas em um PowerShell real com outros componentes/runspaces. Não criar helper-processo adicional para ocultar esse risco. A ativação exige revisão dessa fronteira e prova dos canais/Stop/retention no supervisor existente. Não reinterpretar essa lacuna como autorização para LAB.

Sem herança global é alternativa para filho sem dependências; não é substituto comprovado do contrato atual com pipe stdout e stdin/stderr necessários. Nenhum fechamento especulativo das nove referências históricas é autorizado. Projeção geral: pré-Start129−R+U; primeiro monitor130−R+U; R é redução real futura comprovada, U custo interno adicional no filho ainda desconhecido. Não inserir R=8/12/9 a partir do fixture. ED11/Phase6 PARTIAL; positive DENY.

Ver [closeout único](../integration/n0start03-integrated-closeout-20261024/explicit-inheritance/CLOSEOUT.md), resultados e hashes. A API é Vista+ e a comparação de objetos do teste Windows10+; nada foi colocado no runtime NT4.


## Revisão 3 — resultado adversarial da integração (2026-10-25)

Estado PROPOSED preservado. A hipótese de que referências privadas tornam a janela segura por si só foi REFUTADA no fixture: um segundo thread cria filho sintético com bInheritHandles=TRUE durante NX_BEFORE_CREATE, depois de preparar os três transfers. Snapshot do competidor suspenso e DuplicateHandle/CompareObjectHandles identificam os mesmos três objetos do contexto proprietário. O thread principal aguarda a observação mantendo as referências abertas; isso torna a janela determinística, não mede probabilidade ou exploração real no LAB. Com bInheritHandles=FALSE no competidor: zero correspondências. Três repetições de cada controle em Debug e Release deram o resultado esperado. O concorrente é retomado uma vez para término natural, nunca morto.

I3 é um REQUISITO ainda não satisfeito, não uma garantia fornecida pela implementação de revisão2. A lista restrita protege a seleção de handles do filho pretendido, mas não exclui a herança inversa por outro criador global. Um lock local só coordena callers que o respeitam; não obriga CLR, módulos, perfis ou outro runspace. Não alegar que o teste mostrou concorrência no LAB; mostrou uma limitação concreta do mecanismo.

Console isolado real: stdin ReadConsoleW, stderr WriteConsoleW, confirmação, Escape e F10 com teclas sintéticas; pai e filho exibem o mesmo HWND, referências próprias fechadas e término do filho comprovado. Console nativo funcional não prova Read-Host/Console.ReadKey do launcher canônico, transcript, console loss ou StopService. Nenhuma integração canônica foi aplicada porque o gate anterior de concorrência falhou.

### Decisão arquitetural necessária

A. Preservar a topologia atual e exigir contrato demonstrável de coordenação de TODAS as criações globais durante transferências. Não basta lock do helper ou amostra de processos. Se for adotada uma hipótese de confiança em componentes não coordenados, ela altera a garantia de I3 e exige decisão específica com risco de exposição de stdin/stdout/stderr, não coberto pelo N7 existente. Não recomendada sem prova adicional concreta de cobertura dos criadores.

B. Transferências fora do pai via processo auxiliar dedicado, conforme alternativa oficial Windows: evita expor transfers herdáveis no PowerShell, mas adiciona processo/handles/lifetime e contraria quatro papéis/topologia vigente. Não implementar sem revisão normativa própria; não disfarçar auxiliar como quinto papel gratuito ou novo owner.

C. Nascimento com herança FALSE e bootstrap stdout por endpoint explícito autenticado pelo owner existente; stdin/stderr reabertos no console herdado somente se a identidade e semântica forem qualificadas. Elimina transfers herdáveis criados pelo supervisor, mas muda transporte/protocolo inicial e exige revisão focal de N5/B+D/identidade/retention/fail-closed. Não usar nome ou PID isolado como autenticação. Deve vincular processo ORIGINAL/criação/token, endpoint protegido, finalidade, lifetime, exclusividade e custos; a criação do endpoint torna-se efeito a governar. Não implementado nesta decisão. Não é redução de quotas nem autorização para nova autoridade.

D. Manter STOP é a única disposição qualificada sob os contratos atuais enquanto A não for demonstrada ou B/C não forem especificamente decididas. Não preparar pedido operacional que pressuponha a segurança refutada; não repetir N0HT01 como substituto dessa decisão.

Recomendação: D agora; se o objetivo continuar sendo eliminar tecnicamente a janela sem presumir cooperação do PowerShell e preservar quatro papéis, revisar C como mudança explícita de transporte no mesmo owner. B é incompatível com os limites atuais. Uma aprovação de C ainda exigiria fixtures de bootstrap/identidade/console e revisão normativa antes de integração operacional. Nenhuma alternativa está ACCEPTED aqui.

Ver [relatório integrado desta etapa](../integration/n0start03-integrated-closeout-20261024/explicit-inheritance-integration/CLOSEOUT.md). OPERATIONAL_INTEGRATION=BLOCKED; POSITIVE_CAPACITY=NOT_DEMONSTRATED; ED11/Phase6=PARTIAL. Não há novo pedido read-only qualificado nem aprovação executável.


## Revisão 4 — alternativa não herdada implementada isoladamente

A falha de herança inversa permanece comprovada. O protótipo de transporte local sem herança foi implementado e testado, sem helper-processo de produto ou flags herdáveis privadas. A direção recomendada agora está detalhada em [candidate T0–T9](ADR-CANDIDATE-n0start03-noninherited-local-transport-20261025.md). Essa proposta substitui a recomendação técnica de ativar HANDLE_LIST, mas não supersede nenhuma norma accepted. O aceite T0–T9 ainda está pendente pela fronteira explícita N5 de novo IPC. Este documento e todas as evidências históricas permanecem preservados; nenhuma ativação ou admissão decorre dos testes. Ver noninherited-transport/CLOSEOUT.md no pacote integrado.
