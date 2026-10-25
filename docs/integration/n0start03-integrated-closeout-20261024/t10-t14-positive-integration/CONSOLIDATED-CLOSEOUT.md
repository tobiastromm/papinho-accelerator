# T10–T14 — decisão registrada e integração positiva qualificada offline

Data: 2026-10-25. HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`, branch main. T10–T14 revisão1 foi aprovada pelo owner com resposta explícita “aprovo” ao pedido normativo exato. A decisão [ACCEPTED](../../../adr/DECISION-n0start03-positive-private-transport-composition-20261025.md) incorpora integralmente a proposta original, sem reescrever N5/T0 ou transformar stdout em autoridade. SHA256 accepted `1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5`. A aprovação não autoriza efeitos LAB.

## Integração e revisão de segurança

O target existente ganhou composição opt-in `PAPACC_ENABLE_N0START03_PRIVATE_POSITIVE`; default permanece EMPTY_DENY. CMake rejeita ausência de transporte ou entrada positiva, mistura com pins read-only e traces reservados a read-only. Não há novo owner, mutex, serviço, logger ou processo de produto. A composição moderna Legacy mantém seu perfil2MiB; Selected mantém3MiB. Não houve troca de toolchain.

O bootstrap canônico aceita somente argv fechado, modo Cycle, identidade03, endpoint estritamente local e tupla original do pai. Rejeita números não canônicos/overflow, identidade histórica, peer incorreto e modo read-only nesta compilação antes de dispatch. Mantém autenticação recíproca, bInheritHandles=FALSE, console para stdin/stderr/Stop e somente stdout no canal privado. O binding Framework e o transporte autenticado existentes são reutilizados. Os testes isolados não são uma execução do owner operacional.

O launcher canônico tem perfil positivo separado; read-only não pode Cycle e positivo não pode Prepare. A aprovação positiva deve existir no NOVO endereço fixo `EXACT-POSITIVE-OPERATION-APPROVED.json` deste pacote. Ela não existe. O launcher verifica tipo JSON estrito, campos escalares/booleanos, operação, tentativa, plano normativo e hashes/caminhos dos quatro artefatos no MESMO stream pinado. O nativo verifica hash integral, tamanho, tipo, links e prefixo141bytes no MESMO handle original, rejeitando aprovação read-only/histórica mesmo que seu hash seja fornecido. API de metadata/leitura preserva o erro Win32 original; divergência de tipo não encobre falha de I/O. Falha deixa referência rastreável para cleanup/retenção existente.

O plano canônico agora incorpora o hash accepted T10–T14. Formatter real isolado mediu1765bytes no limite positivo e1636 read-only, dentro do buffer2304; hashing/identidades simulados nesse ensaio não são prova operacional. O recibo positivo exige consistência entre starts/stops/quiescent/intent/effect/diagnostic; recibo vivo/ausente/contraditório mantém UNKNOWN. `close=0` pré-Start continua fechamento próprio, sem afirmar quiescência global. A1/B1/self-token e os três last-use closes C1 permanecem; nenhum handle obrigatório foi suprimido para produzir PASS.

## Builds e testes

Quatro builds produto Selected/Legacy Debug/Release finais PASS, zero warnings, MSVCx64 /W4 /WX. [Logs e resultados](build-results.json). Setenta e duas regressões/modelos finais PASS:60 existentes e12 modelos de perfil fechado. [Resultados](regression-results.json). Uma invocação inicial do teste de descritor sem seu argumento obrigatório `--model` retornou erro de uso; a campanha final usa a assinatura CTest correta e passou. Isso não foi um defeito funcional nem uma consulta de ACL.

Quatro ensaios nativos do bootstrap real com autenticação IPC real e corpo sintético PASS; não executam owner/SCM/LAB. Quarenta e oito casos PowerShell5.1 de encaminhamento/console PASS, incluindo positivo sintético, falha parcial1816, UNKNOWN e recibo contraditório. Cobrem ReadHost/ReadConsoleW, Escape, F10, stderr, volume/truncamento, falha de escrita/console e fechamento. Vinte e sete gates puros de launcher PASS. Quatro execuções do leitor real de aprovação com APIs simuladas:16 casos cada PASS, incluindo erro nativo original. Quatro ensaios do formatter real PASS. Seis configurações CMake inválidas DENY esperado. Review estático dos dois perfis PASS com hashes finais; Review não é Prepare nem consulta SCM/protegida.

As provas anteriores de autenticação adversarial, impersonation/peer incorreto, zero herança privada, Stop/freeze e composição68casos são reutilizadas onde o código não mudou. Não se somam campanhas de classes diferentes para alegar uma prova nativa operacional. Falhas iniciais de compilação do fixture (preprocessador default e `else if` do emissor sintético) foram corrigidas sem suprimir warnings; logs iniciais preservados. O último ajuste nativo preserva GetLastError no leitor e foi seguido de quatro rebuilds,72 regressões e64 casos do leitor. Nenhum teste gera intenção/arquivo protegido/efeito SCM real.

## Composição de recursos e alcance

O fixture anterior tem pico amostrado99; o LAB read-only histórico tinha118 estáveis,120 durante PSS e1005 administrativos. São evidências distintas. A projeção condicional de sobreposição (118−1 self-token+2 intent/imagem+1 processo+2 PSS+3 live+1 thread+1 primeira consulta TCP) chega125; não é pico medido nem teto universal, e C1 ocorre somente depois do último uso. Serviços/RPC/segurança/módulos/transitórios e picos não amostrados continuam explicitamente UNKNOWN. Não se transferem handles entre tabelas nem se contam PSS e referências controladas duas vezes. O trabalhador CreateProcess sintético não prova custo de StartService.

Esta integração acrescenta um pin persistente administrativo para a decisão T10–T14; não concede crédito. O gate JSON aloca bytes até16KiB, string UTF16 até32KiB, parser e overhead gerenciado/transitivo não quantificado; aparecem nas métricas frescas, não são custo zero. O leitor nativo usa141bytes locais e literal142bytes, preservando o buffer de plano2304; custo total de stack/memória não se reduz a esses números. Não houve nova referência persistente nativa de produto para parsing da aprovação. O contexto de transporte e ledger anteriores continuam aplicáveis. O custo real concorrente e do serviço é saída empírica B4.C/D da tentativa única, sem dispensar qualquer gate controlado obrigatório antes dos efeitos.

Limites permanecem128handles por processo nativo,1024administrativos compartilhados, quatro papéis,60s/1s, headroom5/9GiB e pisos4/8GiB, NONE_GRANTED. Reservas reais indisponíveis/UNKNOWN históricos e P3/H/N7 continuam. Se o orçamento fresco, identidade, ausência de colisão, RAW1, recovery/triggers ou lifetime controlado falhar, DENY; não se consome para contornar insuficiência conhecida.

## Hashes finais e histórico

Launcher `09558b27272995b2c98b5cee9cddc95d777ffde0f0a054b987cc8d7082aefddc`.
Qualificador Selected Release `1d998e74728ee3cfcf925a9082bc9f1e594bfe51f9c4f218c8d41fde076d89c2`.
DLL nativa `c351ecb559834d74ebe36dd735b6174b4ca6226526ae928b7a2cc5f44c4d792c`.
Binding Framework `513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4`.
Binário instrumentado do SERVIÇO `44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c` (diferente do qualificador).
Demais configurações/dependências: [pins finais](final-artifact-pins.json).

Os sete hashes históricos são preservados como evidência: o launcher original foi arquivado em preimage, seis outros artefatos/decisões/pedido/aprovação continuam byte-idênticos. O launcher atual mudou legitimamente e não tem o hash histórico. Aprovação read-only consumida não é reutilizada. Inputs protegidos/intent02/imagem02/sink02/ImagePath não foram alterados. Nenhum commit/push/worktree/clone.

## Gate restante e procedimento

Pedido positivo NÃO aprovado SHA256 `218f012c98d1f65e6216fb21023113d266d252a0acd5cd17cee201509ee44efb`: [pedido integral](POSITIVE-OPERATION-REQUEST.DRAFT.json). [Procedimento e riscos](MANUAL-PROCEDURE.NOT-AUTHORIZED.md). [Registro candidato](POSITIVE-APPROVAL-RECORD.CANDIDATE-NOT-APPROVED.json) tem tipo deliberadamente não executável. Não há hash de aprovação operacional fabricado nem comando positivo final utilizável.

Próxima ação: owner aprovar especificamente esse pedido, uma execução MANUAL Cycle03, consumo único/CREATE_NEW imagem/sink/rebind/um Start/até um Stop legal, retenção parcial e UNKNOWN, sujeito a todos os gates frescos. Só depois registrar a aprovação original no endereço exato, conferir hash integral e entregar comando final com SHA literal. Não repetir read-only equivalente nem reaproveitar aprovação consumida. Caso o owner aprove, a execução continua manual e não será iniciada automaticamente.

ED11 E02/E10/E11/E14/E15 têm qualificação fonte/offline ampliada; efeitos E15 e diagnóstico/serviço/Stop E16–E18 NOT_RUN. E20 (demais componentes Phase6) não é coberto por esse LAB. Matriz atualizada sem fechamento artificial.

CURRENT_TASK_COMPLETED=YES
T10_T14=ACCEPTED
TECHNICAL_POSITIVE_INTEGRATION=OFFLINE_QUALIFIED
DEFAULT_PROFILE=EMPTY_DENY
POSITIVE_OPERATIONAL_AUTHORITY=DENY
INDEPENDENT_BUDGET=NOT_GRANTED
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_SERVICE_EXECUTED=NO
OPERATIONAL_INTENT_CONSUMED=NO
SCM_MUTATIONS=NONE
EXISTING_ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_WITH_SCOPED_CHANGES_AND_PREIMAGES
FIRST_UNRESOLVED_DIVERGENCE=EXACT_POSITIVE_OPERATIONAL_APPROVAL_ABSENT
NEXT=OWNER_APPROVES_EXACT_MANUAL_N0START03_POSITIVE_DIAGNOSTIC
