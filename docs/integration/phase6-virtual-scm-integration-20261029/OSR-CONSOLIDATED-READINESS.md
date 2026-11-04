# Primeira instalação — causa exata de bloqueio da sessão SCQ

TASK: PACC-PHASE6-PERSONAL-INSTALL-ONE-SCQ-READINESS. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Foco exclusivo na primeira instalação e sua sessão factual. Nenhuma frente MANUAL/E8/Browser/Legacy foi iniciada; Selected/Legacy são apenas os targets de qualificação requeridos.

## Veredito

**SESSION_NOT_EXECUTABLE — SCQ_NATIVE_AUTHORITY_HANDOFF_UNDEFINED.** Não foi emitido pedido operacional nem comando prematuro.

A única dependência imediata que impede preparar uma sessão executável é **definir e qualificar a fonte original da autorização SCQ e seu handoff administrador → runtime**. Não é falta de mais um hash, de reaprovar destino/conta ou de localizar uma root histórica. Não é correto dizer simplesmente que bastaria o proprietário aprovar uma operação: o caminho atual não tem receptor de autorização original que torne esse efeito admissível.

Essa é a dependência anterior aos gates factuais. Não significa que os gates pós-handoff já estejam demonstrados: parent, token real, chave/isolamento e recursos precisam ser observados/revalidados pela futura sessão. Não são novas decisões de arquitetura a pedir separadamente ou resultados que possam ser inventados agora.

## Prova factual do bloqueio

1. NEXT-BDI-ONE-SCQ-REQUEST.md e BDI-FIRST-OPERATION-SCOPE.md foram lidos integralmente. O segundo já identifica que não há fonte diagnóstica original entregue; não contém autorização executável.
2. O argumento association[32] e o nome congelado são explicitamente DATA em personal_scq_dispatch_win32.h; não existe nessa interface uma fonte de aprovação, seu empréstimo original ou consumo de aprovação operacional. Metadata validada não supre isso.
3. A entrada SCQ usava personal_native_ceremony_ready, que retorna incondicionalmente zero e pertence à primeira instalação. Não há combinação de SID/path/hash ou uma mensagem adicional de aprovação que altere esse código.
4. F0–F9 estabelece ato local para NEW_INSTALL; F7 separa CA_INITIALIZE. M5 separa administração e serviço: o serviço não confirma a cerimônia interativa nem cria CA. A decisão virtual resolve a identidade primária, não entrega autorização diagnóstica ao processo.
5. O probe exige PID próprio e TokenUser virtual, ausência de impersonação e contexto contínuo. Executá-lo no administrador observador não testa o acesso do runtime. Converter sua confirmação administrativa em credencial do serviço seria uma alteração de autoridade não definida, não uma correção de argumento.
6. ServiceMain pessoal e operações SCM continuam guardados por ps_binding_ready, também falso. Aprovação normativa de paths/identidade não é um bypass desses guards. A mesma ausência de handoff explica os endpoints administrativos/runtime; não são dois pedidos de layout ou duas identidades a criar.

## Correção técnica concluída

SCQ deixou de consultar a cerimônia NEW_INSTALL como se fosse sua autorização. personal_scq_native_source_status identifica explicitamente a ausência do adaptador original e devolve PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE. gate_reason é diagnóstico de negação, não um bit de autorização; valores escritos pelo caller não concedem efeito porque o root recalcula o status.

Ambas as entradas SCQ preservam NOT_SUPPORTED/DENY, antes de provider/key. A fonte de NEW_INSTALL permanece intacta para seu propósito. Não foi implementado caminho positivo sem contrato, callback permissivo ou setter que autorize operação. O teste do root prepara paths/association/contexto coerentes, observa a causa explícita e confirma zero chamadas KSP. Stop, associação divergente e replay conservam suas causas anteriores próprias.

A [proposta focal](OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md) preenche a definição ausente: decisão operacional byte-exata + operador elegível + confirmação atribuída + referências originais protegidas; empréstimo diagnóstico de leitura ao destinatário exato; consumo/controlador; no implicit parent creation. PROPOSED, sem aceitar nada em nome do proprietário e sem nova família normativa ou infraestrutura.

## Estado dos bindings

| Binding | Estado | Tratamento |
|---|---|---|
| Norma de destino NEW_INSTALL | ACCEPTED, hash conferido | Não reaprovar; EXISTING preservado |
| Norma de conta virtual | ACCEPTED | Sem herança LAB ou mudança de conta |
| Paths | Código resolve/congela/revalida | DATA, não objeto adquirido |
| Aprovação operacional SCQ | Não concedida por esta tarefa | Só após pedido completo e decisão específica |
| Fonte/handoff original SCQ | Não definido/implementado | Primeiro gate não satisfeito |
| Operador/contexto administrativo | Observação elevada anterior, não vinculada à nova sessão | Capturar/revalidar original na fase pré-efeito; não reutilizar SID/contexto histórico como binding atual |
| Host/boot | Sem binding fresco desta sessão | Capturar e congelar no mesmo procedimento, sem microcoleta para tentar desbloquear o gate constante |
| SCM configurado/token/PID/criação | Collector/binding offline qualificados | Serviço pessoal real não observado |
| KSP MACHINE | Owner/probe offline qualificados | Sem chave persistente nova ou acesso real ao fixture retido |
| Consumo único | Dispatch local impede replay no mesmo owner | Não é consumo de autorização ou prova durável após reinício |
| Recursos/freshness/headroom | Gates preservados | Sem ceilings/valores/waivers copiados de LAB; UNKNOWN continua DENY |

## Parent original — procedimento mínimo e efeito próprio

Destino de dados: known folder ProgramData + Papinho; leaf de root Accelerator. Não foi adquirido, reservado ou criado parent. A antiga observação de path ausente não é prova de ausência atual e não é repetida como coleta equivalente. O uso do KSP MACHINE, por si só, não exige que esse diretório exista; tornar sua criação pré-requisito automático do probe seria uma dependência não demonstrada.

Na preparação legítima da instalação: resolver/congelar ProgramData por API sem CREATE; adquirir a âncora por handle original, validar cadeia não-reparse, Volume/FileID, owner/descritor e direitos; consultar Papinho de modo relativo/read-only, distinguindo ausente, colisão e UNKNOWN. Esses fatos pertencem à fase pré-efeito da mesma preparação, não a uma nova root de qualificação.

Se presente: não adotar silenciosamente, não alterar ACL; exigir proveniência/seleção explícitas como parent e revalidação, sem chamá-lo root publicada. Se ausente e sua criação for necessária ao procedimento final: solicitar autorização **própria** para um create exclusivo de Papinho sob a âncora original, com descritor exato qualificado/aprovado, sem mkdir recursivo/Force/OPEN_ALWAYS. Reter a referência retornada e revalidá-la. Colisão, reparse, perda de identidade, efeito parcial ou close incerto => STOP/UNKNOWN, sem retry, delete ou refund.

Não criar Accelerator para um teste e depois adotá-lo como root NEW_INSTALL: P3 exige o novo objeto criado pela cerimônia correspondente. A confirmação F3 de NEW_INSTALL ocorre depois da aquisição legítima do parent e é distinta de prepará-lo. Não foi gerado pedido de criação com IDs, SD ou hashes futuros inventados. Uma sessão manual pode conter decisões independentes, mas autorização SCQ não inclui automaticamente o create do parent.

## Autoridade necessária e escopo da sessão

Proprietário: decide a autorização operacional específica e, se necessário, a criação do parent em ato próprio. Administrador nativo: comprova identidade/contexto/direitos e fornece a confirmação atribuída. Runtime virtual: só recebe a capacidade diagnóstica restrita validada pelo handoff; não é administrador nem recebe chave privada. KSP: continua impondo sua proteção; elevação administrativa não prova isolamento e negação local não prova KSP.

A futura sessão deverá observar configuração SCM, TokenUser/grupos/restrições/tipo SID, PID/criação/continuidade, criação/uso administrativo de chave MACHINE isolada, tentativa de abertura pelo runtime, Stop e closes separados. Sem CA produtiva, CURRENT, rede ou recuperação. Nome de chave/assinatura matemática e hashes de fixture não são identidade original. NTE_BAD_KEYSET, falha de query/provider e API não alcançada são inconclusivos; abrir inesperadamente nega o resultado, sem assinar/exportar sob runtime. Assinatura/exportação não executadas não recebem retornos fictícios.

Nenhuma aprovação histórica é reutilizada. O teste local de replay não permite rotular um artefato como aprovação nova ou restaurar consumo após restart. Não se cria ledger/serviço genérico para compensar o contrato ausente.

## Qualificação e entrega

Houve mudança funcional apenas no diagnóstico/admissão por propósito; por isso foram executados quatro builds Selected/Legacy Debug/Release e 24 CTests/configuração: **96 PASS**, zero warnings. Paths39, dispatch59, probe95, identity91 e presenter38 continuam passando. SCM e probe usam APIs modeladas nesses testes; SID helpers e a thread concorrente são reais. A regressão CNG efêmera existente é matemática real, não MACHINE persistente ou token real. Nenhuma chave persistente foi criada/aberta; receipt da chave retida foi conferido sem acessá-la.

O novo reason ocupa padding: confirmar tamanhos reais em OSR-QUALIFICATION.json/layout logs, sem presumir crédito. Nenhum limite mudou; provider/kernel seguem UNKNOWN, autoridade DENY. Não houve trabalho em outras frentes. Sem instalação, diretório de instalação, conta, SCM/ACL, serviço, Start/Stop, TPM/Q4, CURRENT, limpeza/refund ou recuperação.

**Pedido operacional: NÃO_EMITIDO.** Não há comando executável honesto enquanto o contrato/receptor de handoff estiver ausente. A proposta focal é definição de fonte, não uma solicitação operacional incompleta para ser executada. O próximo passo é resolver essa definição única e qualificar seu receptor no mesmo owner; não outra auditoria de paths, contas, TPM ou root histórica.

Caminho restante NEW_INSTALL: contrato/handoff SCQ → pedido operacional exato com gates/objetos próprios → aprovação específica → sessão factual manual/reconciliação → cerimônia NEW_INSTALL/CA_INITIALIZE e publicação próprias somente quando seus gates forem satisfeitos. Sem presumir execução bem-sucedida, parent criado ou dispensa de recursos pela aprovação do contrato. ED11/Phase6 PARTIAL.

[Próximo prompt focal](NEXT-OSR-SCQ-HANDOFF-CONTRACT.md). Evidência: OSR-QUALIFICATION.json, OSR-SOURCE-HASHES.json e OSR-ARTIFACT-HASHES.json. Proposta e relatório possuem hashes reais, mas nenhum hash é apresentado como autoridade operacional.
