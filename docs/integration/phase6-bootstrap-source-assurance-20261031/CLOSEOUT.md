# Phase 6 — fonte endpoint e escolha de garantia do primeiro Start

Task: PACC-PHASE6-BOOTSTRAP-SOURCE-AND-START-ASSURANCE.
Estado: **PARTIAL; ingresso frio ainda incompleto; Start UNKNOWN/DENY**.
Pedido operacional NOT_READY. Nenhuma operação protegida executada.

## Código efetivamente integrado

1. Ato endpoint próprio no MESMO PAPACC_SCQ_ADMIN_SOURCE: present/confirm/current/
   selection/take, propósito TO, owner/thread/token/originais/tentativa/janela,
   primeira falha e consumo único. Prefixo específico OWNER_SCQ_PRIVATE_ENDPOINT,
   diferente de OWNER_SCQ e OWNER_SCQ_DIAGNOSTIC_START. Não é aprovação normativa
   em código nem callback positivo. Sem original/confirmacão, getter continua
   NULL. A confirmação não chama endpoint ou Start.
2. Presenter nativo existente mostra a projeção tipada e os efeitos específicos
   antes da confirmação endpoint. first_controller liga SCQ -> endpoint -> Start,
   três atos distintos. Ato confirmado não significa consumido nem recurso admitido.
3. Getter endpoint deixou de ser NULL constante no controlador: recebe o owner e
   empresta somente a fonte local original/atribuída. No receptor frio continua
   negando; não reconstrói autoridade por carrier, ack ou endpoint.
4. Antes da primeira criação TO, transport_enter exige admissão de recursos e
   consumo endpoint próprio antes de scqt_reserve. Caminho já bound não recebe
   fallback permissivo. Conexão/entrega e namespace-current exigem consumo.
   Como os termos TO7 permanecem desconhecidos, não consome nem entra no provider.
5. Correção funcional demonstrada: session_acquire_originals já faz scqa_capture
   (fase1). first_controller recusava qualquer fonte não vazia, interrompendo a
   sequência legítima existente. Agora aceita SOMENTE essa captura original não
   confirmada do mesmo packet/seleção, revalida-a e não repete capture. Fonte
   substituída, confirmada/replay, consumo, Stop ou UNKNOWN não recebem continuação.
6. Close desconhecido marca também o ato endpoint como UNKNOWN. O fechamento
   pré-Start recusa liberar endpoint consumido. Não há grant, ledger, root,
   novo serviço, IPC genérico ou mudança persistida/wire.

## Fontes e cadeia — alcance honesto

[COLD-INGRESS-STATUS.md](COLD-INGRESS-STATUS.md) identifica o corte restante.
A composição de aquisição já capturada até as confirmações/session prepare está
qualificada, incluindo o percurso session_acquire_originals fase1 corrigido.
**O producer inicial do plano e dos handles no controller frio NÃO foi concluído.**
Não se gerou decisão operacional do owner nem se criou um loader de grants.

**A fonte independente inicial do receptor/endpoint NÃO foi concluída.**
ServiceMain captura SCQ1 DATA; scq_ingress_continue ainda não dispõe de seleção
original independente para a primeira aquisição. Não se removeu essa guarda.
Validações nativas posteriores do adapter e receipt/close existem, mas seu
percurso positivo ainda depende de seleção receptora modelada. Nenhuma
impossibilidade universal sob os accepted foi alegada para encobrir trabalho
faltante; a ordem atual do código não é uma norma. Aceitar a proposta de recursos
não supre esses producers nem autoriza endpoint sem fonte própria.

## Admissão e decisão focal

[RESOURCE-APPLICABILITY.md](RESOURCE-APPLICABILITY.md) confronta cada custo com
F8/W4, envelopes incrementais, OHI/B7, SSO-B e TO7. Censo controlado agora:
Selected1993760/3145728; Legacy1825192/2097152. Delta16 bytes pelo ato endpoint,
sem novo handle/heap na máquina do ato. Stack transitivo, RPC/processo/loader,
provider/kernel, margem e relógios continuam sem suficiência integral.
resource_pre_missing63; resource_effect_obligations7. Nada foi convertido em PASS.

Existe uma fronteira normativa real adicional ao ingresso: TO7 exclui a exceção
opaca; B0–B8/R0–R9 não autorizam Start. Preparada UMA proposta focal, não uma nova
família de ADRs: [FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md](FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md).
SHA256 **d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b**.

A mantém garantia integral e DENY até provisão suficiente. Recomenda-se B/G0–G9
para UM ensaio fechado, com redução explícita da garantia prévia de custos
opacos, originais/quotas/consumo/Stop/close preservados e aprovações operacionais
separadas. Não é aceitação, capacidade concedida ou seleção de números ausentes.
Não foi implementado perfil positivo dependente dessa proposta.

## Qualificação

Quatro builds Selected/Legacy Debug/Release finais PASS, /W4 /WX, zero warnings.
24 regressões afetadas por configuração, **96 pares distintos PASS**. Nova máquina
endpoint:18 cenários/208 checks; composição controladora:24 cenários/242 checks
por configuração. Inclui separação de atos, identidade/propósito/fonte ausente
ou divergente, consumo antes de efeitos, replay, Stop, retenção/close UNKNOWN,
captura fase1 legítima e substituída, falta de recursos apesar de confirmação.
Demais regressões verificam carrier/ServiceMain, receiver/receipt/close,
transporte, source e lifecycle. N0/READ/owner guard permanecem estritos.

As máquinas produtivas de fonte/atos e o caller são os corpos compilados reais;
as fronteiras de aquisição/console/token/IO/SCM/endpoint são MODELADAS. SID/SD e
hashing público em memória dos modelos não constituem KSP ou prova operacional.
Nenhum endpoint/RNG protegido/SCM/Start real foi executado. Não é evidência de
autenticação bilateral nativa ou proveniência original operacional. A primeira
compilação do novo teste rejeitou escapes C de paths incorretos; corrigidos
apenas os literais do modelo. Log inicial preservado, finais limpos. Reruns não
foram somados como pares adicionais. [EVIDENCE.json](EVIDENCE.json) e logs completos.

## Imagem e efeitos

[Delta final de workspace](STAGED-IMAGE-DELTA.md):683520 bytes,
1d900e6a3e08e40d556666fff9b80229e14310b48b3c2061ad65df38dfcc35ec.
Snapshot anterior preservado. DLLs sem delta. Imagem staged histórica intacta,
não revalidada; update necessário futuramente e prematuro enquanto ingresso
incompleto. Nenhum artefato final selecionado ou comando operacional emitido.

Nenhum Start/Stop, endpoint, SCM/ACL, staging/update, CA/CURRENT, rede operacional,
TPM/Q4, cleanup/refund ou recuperação. main/HEAD
f341985924d29870d8ff627ed37c8a0874ad6469, WIP/históricos/pins/reservas,
autorizações consumidas e PST0.6.4 preservados. Nenhum accepted alterado/criado.
OS_QUIESCENCE UNKNOWN. E21 PENDING; ED11/Phase6 PARTIAL.

## Primeiro bloqueio e continuação

Primeiro bloqueio factual: a entrada fria ainda não adquire a seleção original
que alimenta as primitivas e o receptor ainda não recebe admissão independente
inicial. Correções locais não equivalem a fechar essa cadeia. A escolha A/B de
garantia é uma decisão adicional exclusiva do proprietário, não um defeito a
contornar. Não é correto preparar pedido de execução antes de ambos fecharem.

[Próximo prompt executável](NEXT-ORIGINAL-ENTRY-AND-ASSURANCE-DECISION.md) mantém
esse foco, sem repetir staging/registro, campanhas ou pedir aprovação de SSO-B.
