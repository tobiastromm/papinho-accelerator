# Seleção original e admissão do primeiro Start — fechamento factual

Continuação de NEXT-NATIVE-ORIGINAL-SELECTION-AND-START-ADMISSION.md. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado. Sem nova ADR, owner, canal, root, ledger, formato persistido ou aprovação. Os hashes B, SSO-B e TO0–TO9 foram reconferidos e coincidem com as incorporações accepted.

## Integração funcional realizada

Antes desta execução, session_acquire_originals exigia um getter que devolvia NULL antes de qualquer captura. A nova aquisição administrativa deixa de exigir uma seleção previamente autorizada para CAPTURAR candidatos. Exclusivamente para o primeiro propósito TO/pre-readiness/waiting-runtime, a sequência é: referências originais recebidas pelo owner → captura nativa → verificação de conteúdo/descritores → personal_scq_selection_capture → scqa_capture. READ e outros perfis mantêm o requisito anterior e não herdam o novo caminho.

personal_scq_selection_win32.inc implementa o construtor interno no mesmo packet. Exige owner/thread/estado corretos, plano TO válido e não consumido, runtime futuro ainda não inventado, token administrativo original, processo/criação/TokenId/AuthenticationId/ModifiedId, SIDs, caminhos conhecidos congelados e referências de conteúdo/SD já verificadas e revalidadas. O candidato de 848 bytes e seu estado ficam no packet existente; não há alocação, arquivo/grant ou nova referência criada pelo construtor. Falhas não confirmam nem consomem nada.

admin_prepare aceita esse candidato interno apenas para chamar o capturador original existente. O getter de origem agora recebe o owner e empresta somente uma origem local corrente, validada por scqa_current ou scqr_current. Candidato sozinho e packet vazio continuam retornando NULL. Esse empréstimo QUERY não é autorização de Start: scqa_start_selection só retorna o original exato depois da confirmação independente OWNER_SCQ_DIAGNOSTIC_START, com identidade, escopo, janela, referências e consumo consistentes. O carrier e o owner SCM passam por esse produtor antes de usar o ato. Não há cópia remota de ponteiros nem concessão por flag.

Continuam separados: aquisição; apresentação/declaração e confirmação SCQ; apresentação/declaração e confirmação Start; consumo SCQ/Start; autorização própria de endpoint; submissão SCM; receipt/close e readiness diagnóstica. Nem o candidato nem a confirmação SCQ satisfazem a confirmação Start. Stop, alteração de identidade/plano/SD/conteúdo, expiração, replay e retenção negam novos efeitos. A seleção de endpoint e de KSP continua vazia; não foi derivada do Start.

A admissão passa a reportar custos específicos e distinguir pré-condições de provas posteriores. O CLI imprime essas causas antes de qualquer efeito quando não recebe fonte legítima. Ver RESOURCE-APPLICABILITY.md para termos, cláusulas, custos e limites. Não foi aplicado R0–R9/B3 ao Start.

## O que NÃO está concluído

O produtor administrativo interno está implementado, mas o caminho nativo completo da CLI/ServiceMain ainda não está fechado. Há dois cortes de aquisição precisos:

1. service_cli chama launcher_scq_start_carrier; não há caller nativo anterior que forneça, no mesmo controlador vivo, os três originais e o plano operacional byte-exato e invoque session_acquire_originals e as duas confirmações de console. Esses métodos não têm caller produtivo fora de suas definições. Arquivos staged, o ato de registro consumido e o carrier não são esses originais. O construtor implementado não inventa pedido ou decisão do proprietário nem escolhe parâmetros quantitativos ausentes.
2. No runtime vazio, scq_ingress_continue ainda precisa da seleção independente antes de scqb_open; o novo construtor é administrativo, não constrói uma seleção receptora a partir do carrier. O getter receptor só empresta origem já adquirida; não preenche essa primeira aquisição. O modelo anterior fornece uma seleção local distinta, e isso não prova seu producer nativo. Não se declarou impossibilidade normativa nem se pediu reaprovar SSO-B. Falta uma composição concreta de aquisição independente pelo mecanismo permitido; substituir o requisito por metadata seria incorreto.

Esses são débitos de implementação/composição, não gates que uma simples aprovação no chat limparia automaticamente. A ausência de autorização operacional não foi usada como justificativa para deixar o construtor administrativo vazio. A composição inteira, contudo, permanece incompleta e não recebeu declaração de sucesso nativo.

Há ainda incompatibilidade determinística entre o descritor que o stager produz (`O:BA`) e a exigência `owner==ator` do consumidor SCQ para a imagem. Isso foi confirmado executando a guarda INALTERADA sobre o SD somente em memória. Não é coleta da ACL atual. O patch de produção para tratar a imagem separadamente foi rejeitado pela revisão automática como redução de fronteira de autorização não explicitamente aprovada; não foi aplicado nem contornado. A proposta técnica limitada a papel3/TO está em STAGED-IMAGE-OWNER-GUARD.CANDIDATE.md, para decisão explícita, sem nova ADR. Request/decisão, READ e todos os demais predicados devem conservar a guarda. Este é o primeiro predicado de proteção incompatível conhecido para a aquisição dos bytes staged sob a política histórica.

## Recursos

O gate de recursos permanece UNKNOWN/DENY por seis termos prévios materiais; provas de processo, endpoint e close são posteriores e não exigidas como PID/receipt futuros antes de Start. Censo controlado e riscos conhecidos não se transformaram em provisão nativa. B7/S6/TO7 continuam vigentes; TO7 não contém exceção de custo opaco. Não foram inventados ceilings, freshness/duração ou margem do host. Nenhuma coleta inconclusiva foi proposta como solução para todos esses termos.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings de compilação; 26 regressões afetadas por configuração (104 pares PASS). Após encontrar o predicado de owner, executado apenas o novo teste negativo em cada configuração (4 pares adicionais), sem repetir as 104 execuções inalteradas. Total108 pares PASS; logs e pins em EVIDENCE.json. Novos modelos: scq_native_selection_capture executa o corpo do construtor e o getter nativos, 16 casos/176 verificações; scq_start_selection, 18 casos/211 verificações; origem/Stop/token/conteúdo/descritor/plano/owner/consumo divergentes negam. Candidato sem captura, origem sem confirmação Start e confirmação SCQ isolada não concedem Start. O modelo nativo usa os tipos reais de owner, mas modela aquisição, token/SD/CNG/paths/processo: NÃO é aquisição nativa real. O teste de owner usa apenas SID/SD em memória e mantém a rejeição atual. Os modelos mantêm zero endpoints, SCM/Start/Stop, RNG ou KSP reais.

## Imagem, pedido e caminho mínimo

O EXE de qualificação mudou novamente; DLLs sem delta de conteúdo. O EXE staged histórico permanece intocado. STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md registra pins e delta, sem selecionar uma imagem operacional ou permitir overwrite. Como os cortes de aquisição ainda podem mudar código, não se congelou nem se pediu seleção antecipada do artefato final.

Primeiro pedido operacional autorizável: nenhum neste estado. Start/endpoint exigem os cortes de aquisição fechados e uma política de suficiência nativa aplicável. Update exige imagem final estável e uma primitiva governada para objeto existente; o stager exclusivo não permite sobrescrita. Não há comando prematuro ou placeholders.

Ordem mínima: concluir os dois callers de aquisição original → fechar a política aplicável de suficiência sem provas futuras impossíveis → qualificar os callers completos com APIs modeladas → congelar uma imagem → pedido separado de update/reconciliação → pedido específico de endpoint/Start diagnóstico → observações frescas e fechamento sob autorização posterior. Não repetir staging, CreateService ou autorizações consumidas.

E21 PENDING; ED11/Phase6 PARTIAL; autoridade produtiva DENY; OS_QUIESCENCE UNKNOWN. Nenhuma consulta/alteração SCM, update, Start/Stop, endpoint, filesystem protegido, CA/CURRENT, rede, TPM/Q4 ou cleanup/refund executado. HEAD/históricos/pins/reservas/chave fixture/PST0.6.4 preservados.
