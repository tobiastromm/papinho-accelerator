# Aquisição nativa OHI — custódia implementada e fronteira de entrega demonstrada

Task PACC-PHASE6-OHI-NATIVE-ORIGINAL-ACQUISITION. Data: 2026-10-30.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP anterior preservado.

## Resultado

**A aquisição de autoridade e a entrega autenticada ao runtime NÃO estão
concluídas.** Foi implementada e qualificada a etapa nativa de custódia de
referências/metadados no owner existente, sem abrir caminhos arbitrários,
ler/exibir conteúdo ou transformar referências em aprovação.

O impedimento de entrega agora tem fundamento concreto: o transporte existente
não suporta contratualmente o par administrador/conta virtual nem transporte
de autoridade. OHI continua ACCEPTED; não se pede outra aprovação de OHI.
Não foi criado pedido operacional, comando ou aprovação em nome do proprietário.

A conclusão anterior de que faltava somente implementação estava incompleta:
não havia sido confrontado o handoff entre contas com as limitações específicas
T0/T2/T11 do único transporte privado implementado. Esta entrega corrige esse
diagnóstico com leitura das normas e ensaio do predicado real, sem relaxá-lo.

## Código integrado e prova delimitada

`personal_scq_inputs_win32.h/.inc` implementa custódia de três referências
originais já abertas pelo fluxo chamador. Não existe abertura por path ou
parser de arquivo que possa conceder autoridade. Captura a identidade antes
e depois da duplicação com direitos de leitura/READ_CONTROL, sem herança;
rejeita aliases, diretórios, reparse e múltiplos links; captura owner/DACL
protegida por handle e exige continuidade byte-exata do descritor. O perfil
de descritor é conservador: owner igual ao operador, ACEs simples sem flags,
escrita somente para operador/SYSTEM/Administrators. Isso não redefine RAW1
nem afirma que todo descritor assim observado seja autoridade legítima.

`papacc_configuration_personal_scq_capture_inputs` está ligado ao packet
canônico e aos predicados nativos administrativos existentes `personal_token`:
sem impersonação, token primário/elevado, papel habilitado/contexto interativo,
SID observado, sessão e continuidade de TokenId/AuthenticationId/ModifiedId.
Não interpreta NEW_INSTALL ou CA_INITIALIZE como aprovação SCQ. Exige estado
vazio, paths NEW previamente congelados/revalidados e ausência de worker,
CA, publicação, recuperação ou Stop. `scq_inputs_current` revalida token e
referências; uma inconsistência não pode ser rearmada por consulta posterior.

Retire/mutação do layout/fechamento não descartam custódia ativa. Falha de
close, LocalFree ou saída contraditória de API conserva referências e UNKNOWN;
não há retry/reset. O token original permanece no bootstrap-token do mesmo
packet e só seu close root o libera. Nenhum owner/serviço/root foi criado.

**OK nesta API significa somente custódia de metadados.** Não autentica
aprovação/conteúdo/proveniência/propósito dos arquivos, confirmação atribuída,
pins de artefatos, destino ou consumo durável. Mesmo os campos coerentes de
fixture não concedem autoridade: o root continua com fonte OHI ausente e
nega antes do KSP. Distingue `NATIVE_SOURCE_UNAVAILABLE` de
`ORIGINAL_PROVENANCE_UNAVAILABLE` quando há apenas metadados adquiridos.

## Impedimento exato de entrega

**GOVERNANCE CONFLICT para uma implementação que reutilizasse o transporte
N0 para este handoff sem especialização explícita.** Não há conflito que
invalide OHI: é a utilização proposta daquele perfil que contrariaria suas
limitações. Nenhuma alteração dependente desse conflito foi aplicada.

1. [T0/T2 accepted](../../adr/DECISION-n0start03-noninherited-local-transport-20261025.md)
   limita a exceção ao supervisor elevado e seu qualificador original, para
   stdout; exclui endpoint de runtime/serviço/outro componente e não autoriza
   IPC genérico. O par usa mesmo TokenUser/AuthenticationId/sessão.
2. [T11 accepted](../../adr/DECISION-n0start03-positive-private-transport-composition-20261025.md)
   determina: “não transportar comandos, intent, credenciais, autoridade ou
   solicitação de Stop pelo pipe”. A especialização positiva continua LAB
   específica; não é autorização de transferência SCQ.
3. `tools/qualification/scm-n0-private-stdout-native.c` linhas31–37 implementa
   `same_token`; Authenticate e Client o exigem. Seu API Create produz filho
   do supervisor, não uma referência ao runtime criado pelo SCM.
4. O receptor OHI contém ponteiros owner/original/runtime e handles locais.
   Copiar essa estrutura para outro processo não entrega referências válidas.
   O worker de thread mantém o processo/token; não se torna conta virtual.
   `personal_cng_probe_win32.inc` exige runtime PID igual ao próprio processo.
   Executar no controlador administrativo ou impersonar não resolve a prova.
5. O contrato OHI exige origem e destinatário distintos, mas não permite
   inferir novo wire/ROOT/CURRENT/IPC genérico nem herdar exceções LAB.
   A tarefa também proíbe mecanismo paralelo. Logo não criar outro canal,
   remover `same_token`, usar callbacks positivos ou adotar tokens de fixture.

Busca em apps/src/tools por named pipes/shared mappings/peer process IDs
encontrou somente o transporte N0 acima como implementação privada relevante.
O código dele e as decisões accepted permanecem intactos. A conta virtual
accepted não altera a política do transporte. A decisão focal proposta é
[especialização prospectiva B](ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md),
não uma nova aprovação da instalação ou reabertura de OHI.

## Qualificação e recursos

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings; 26 CTests
por configuração, **104 PASS**. Os builds iniciais encontraram um cast
signed/unsigned e include-path ausente no novo teste; ambos foram corrigidos
antes dos builds finais. Não houve troca de toolchain/backend.

- 21 casos de custódia: token/predicados reais source-linked, identidades,
  owner/DACL, writable Everyone, DACL não protegida, ACE fora do perfil,
  falhas de query/duplicação, retorno de erro com handle, substituição,
  alias, Stop, replay, invalidação, close e LocalFree UNKNOWN, thread alheia.
- Quatro casos do **predicado real do transporte**: par igual passa;
  TokenUser diferente, AuthenticationId diferente e sessão diferente negam.
  Nenhum Open/Create/Connect/Read/Write do transporte foi chamado; pipes=0.
- SID/security descriptor APIs e thread Windows são reais. Token, processo,
  arquivos e duplicações são modelados nos novos ensaios. Não são prova de
  token SCM real, ACL efetiva de filesystem/KSP ou entrega entre contas.
- O root real mantém negativos mesmo com DATA de metadados coerentes e
  continua com zero chamadas KSP. Emissão/crypto efêmera anterior é regressão
  isolada, não CA operacional. Receipt da chave persistente retida conferido
  sem abrir ou alterar a chave.

Subestado novo **248 bytes fixos** contabilizados no censo compilado:
Selected 1.974.984/3.145.728; Legacy 1.806.416/2.097.152. Descritores retornados
pelo OS têm custo dinâmico adicional: somar os três snapshots e a sobreposição
de `checking`, além de referências/stack/provider/kernel. Não são zero nem
qualificados para admissão real pelos tamanhos de fixture. UNKNOWN obrigatório
continua DENY; nenhum crédito nem limite 124/128/1024 foi alterado.

## Revisão automática e tratamento seguro

A revisão automática rejeitou um patch de producer/presenter que abriria
locators e apresentaria conteúdo bruto dos arquivos no console, além de SID
e identificadores, antes de comprovar origem/classificação dos inputs.
Motivo: possível exposição de material sensível e nova fronteira de autoridade
não especificamente autorizada. **O patch não foi aplicado.**

A alternativa implementada é restrita a referências já abertas e metadados:
não abre paths, não lê conteúdo, não imprime fonte/SID/descritores e não
confirma planos. Não há contorno da rejeição, segredo acessado ou permissão
operacional inferida. A candidata B exige projeção SCQ tipada não secreta e
rejeição de campos desconhecidos; não solicita autorização para exibir arquivos
arbitrários. A revisão automática não certifica os testes ou a segurança global.

## Estado e próxima decisão real

| Propriedade | Estado |
|---|---|
| OHI e conta virtual | ACCEPTED; registros/hashes preservados |
| Custódia administrativa de referências | Código nativo ligado; qualificação offline/modelada |
| Conteúdo/proveniência/decisão original autenticada | Não demonstrados; nenhuma fonte positiva criada |
| Confirmação SCQ por projeção tipada | Ainda não implementada; bruto rejeitado e descartado |
| Entrega administrador → virtual | Bloqueada pelo perfil do transporte existente |
| Consumo durável antes do primeiro efeito | Não demonstrado; contador local não o substitui |
| Parent/runtime/MACHINE real | Não adquiridos/observados nesta tarefa |
| Pedido/comando operacional SCQ | Não emitidos |
| E21/ED11/Phase 6 | PENDING/PARTIAL; autoridade DENY |

Decisão disponível: manter A/DENY ou aprovar B exclusivamente para integrar e
qualificar offline a especialização privada SCQ. O texto original T0–T14 fica
inalterado; não aplicar sua garantia de mesmo-token a peers distintos.
Depois de B: completar producer/confirmador tipado e transferência sob aquele
perfil, qualificar conjuntamente sender/receiver/consumo/Stop/UNKNOWN e custos;
somente então preparar pedido operacional byte-exato e solicitar aprovação
manual própria. Isso não promete que todas as provas nativas/recursos já passam.

Parent, se indispensável, requer autorização própria; não criar/adotar
Accelerator como fixture. NEW_INSTALL/CA_INITIALIZE/publicação têm autoridades
próprias após a sessão factual, não recebem aprovação implícita.

Nenhuma sessão, instalação, diretório, conta, CA, CURRENT, Start/Stop, SCM/ACL,
rede, TPM/NV/Q4, limpeza/refund ou recuperação foi executada. Não houve mudança
para Browser/E8/Legacy como frente; Legacy é somente o target exigido.
PST 0.6.4, HEAD/WIP/históricos/pins/chave retida/UNKNOWN/reservas preservados.

Evidências: ONA-QUALIFICATION.json, ONA-SOURCE-HASHES.json,
ONA-ARTIFACT-HASHES.json. [Próximo prompt focal](NEXT-ONA-DELIVERY-DECISION.md).
