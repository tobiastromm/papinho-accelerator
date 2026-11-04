# Handoff original SCQ — aceitação e integração offline do receptor

Task PACC-PHASE6-OSR-SCQ-HANDOFF-ACCEPTANCE-AND-INTEGRATION, 2026-10-30.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP anterior preservado.

## Resultado e limite

A proposta SHA256 `34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561`
foi conferida integralmente e preservada. A aprovação explícita do proprietário
foi registrada na [decisão accepted](../../adr/DECISION-personal-scq-original-handoff-20261030.md)
antes da implementação. Não se solicita reaprovação dessa norma.

**A integração nativa completa ainda não foi concluída.** O receptor de
empréstimo e seu lifecycle estão integrados ao packet canônico e qualificados
offline; aquisição e entrega autenticada dos originais administrativos ainda
não têm um produtor/caller nativo implementado. A entrada real chama o
receptor com fonte ausente, reporta ABSENT/NATIVE_SOURCE_UNAVAILABLE e não
alcança o KSP. Esse NULL não é apresentado como implementação da fonte.
Não há pedido operacional pronto nem comando executável.

Esta entrega não demonstra o critério completo de handoff original nativo.
O resultado positivo de referências modeladas prova o receptor, não a origem
administrativa da autorização. A primeira lacuna continua técnica, agora
delimitada ao produtor/aquisição/transferência, e não normativa nem falta de
outro hash de aprovação.

## Código e callers

- `personal_scq_handoff_win32.h/.inc`: estado privado de empréstimo,
  destinatário/owner/thread, associação, runtime PID/criação/TokenId,
  AuthenticationId/ModifiedId, chave exata, propósito SCQ, deadline e
  referências originais; nenhuma factory pública ou callback de autorização.
- `papacc_configuration_personal_scq_receive_original`: entrada do mesmo
  owner/packet; nega sem o produtor original. Não recebe approved, token de
  NEW_INSTALL, hash ou objeto serializado que possa conceder autoridade.
- Dispatcher: ordem preparada para consumir o original antes do probe e
  terminar o empréstimo somente após close factual. O gate independente de
  origem permanece fechado, pois sua prova ainda não foi implementada.
- `pending_blocks`, retire, runtime/native close: impedem apagar ou fechar
  dependências emprestadas/UNKNOWN; cancelamento não rearma o original.
- NEW_INSTALL/CA_INITIALIZE, `personal_native_ceremony_ready` e
  `ps_binding_ready` não concedem autoridade SCQ e continuam preservados.

O componente verifica identidade/tamanho/tipo de arquivo, ausência de reparse,
vida do processo de origem e estatísticas do token retido. Essas verificações
**não comprovam conteúdo aprovado, descritores protegidos, ligação entre token
e processo, papel administrativo ou confirmação atribuída**. Esses requisitos
pertencem ao produtor de originais ainda ausente. Não habilitar o gate usando
somente este componente ou reinterpretar sua estrutura privada como entrada
autorizada. Source view, self-pointer, enum ou metadados não são autoridade.

O consumo modelado é local ao original controlador. Não prova receipt durável,
ordem real anterior a Start, transferência entre processos, resistência global
a rollback ou retomada após crash. Uma sessão futura precisa consumir a decisão
no controlador antes do primeiro efeito protegido; o empréstimo de consulta
ao runtime não substitui essa etapa. Sem essa prova o gate deve continuar DENY.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings, 24 CTests
por configuração, 96 PASS. O build inicial encontrou uma incompatibilidade
signed/unsigned no teste novo; foi corrigida e os quatro builds finais foram
executados com warnings tratados como erro.

24 casos de receptor exercitam referências válidas modeladas, ausência,
identidades/tipo/reparse/query divergentes, processo encerrado, propósito
NEW_INSTALL recusado, deadline expirado, consumo anterior, exclusividade,
PID/criação/token divergentes, associação/chave divergentes, Stop, cópia de
original, mutação entre recepção/consumo, replay e close UNKNOWN. Cópia do
receptor não pode consumir/fechar o empréstimo original. Thread Windows real
alheia não pode consumir. A fonte administrativa e APIs de referência são
modeladas; nenhuma aprovação operacional é criada pelos testes.

Os negativos do root exercitam a entrada real sem fonte, repetição e zero
chamadas de provider. As regressões CNG efêmeras existentes não são prova de
SCM/token/MACHINE ou de uma origem administrativa. Não foi aberta a chave
fixture persistente retida; somente seu receipt histórico é comparado.

O receptor final ocupa **96 bytes** fixos adicionais no packet, incluídos no
censo compilado: Selected 1.974.736/3.145.728; Legacy 1.806.168/2.097.152.
Structs de originais no ensaio são backings de teste emprestados, não uma fonte
nativa provisionada ou storage operacional qualificado. Custos transitivos,
provider/kernel e picos nativos continuam UNKNOWN. Não há novos créditos ou
alteração dos limites 124/128/1024.

## Bindings e próximo gate

| Item | Estado |
|---|---|
| Contrato focal SCQ | ACCEPTED, original preservado |
| Receptor/borrow/consumo/close | Integrado e qualificado offline; não prova origem |
| Aquisição/entrega autenticada nativa | Não implementada; primeiro trabalho técnico restante |
| Pedido/decisão operacional SCQ | Não emitidos nem aprovados nesta tarefa |
| Operador, host/boot, runtime/imagem e key MACHINE | Sem novos fatos desta tarefa; não reutilizar histórico |
| Parent `%ProgramData%\Papinho` | Não adquirido/criado; não presumir ausência ou autoridade |
| Serviço/token/KSP real | Não observados |
| ED11/Phase 6 | PARTIAL; autoridade DENY |

Não basta tornar `personal_scq_native_source_status` positivo: o próximo código
deve adquirir os objetos protegidos originais, validar conteúdo/SD/proveniência
e confirmação atribuída, controlar o consumo anterior aos efeitos, e entregar
somente o empréstimo exato ao runtime. Precisará qualificar o sender/receiver
com os mesmos owners, sem grants sintéticos, novo serviço ou IPC genérico.
Essa implementação já está no escopo técnico autorizado; não se pede nova
aceitação normativa por esta entrega. A evidência atual não permite alegar que
só falta aprovação operacional.

Depois: bindings completos → pedido SCQ byte-exato → aprovação operacional
específica → sessão manual factual/reconciliação → cerimônia NEW_INSTALL e
CA_INITIALIZE/publicação sob autoridades próprias. Se criação do parent for
necessária, seu efeito requer autorização própria; KSP não exige esse parent
por si só. Nenhuma criação/adoção antecipada de Accelerator.

Sem instalação, diretório/conta/CA, CURRENT, Start/Stop, SCM/ACL, rede,
TPM/NV/Q4, limpeza/refund ou recuperação reais. Não houve mudança de frente.
PST 0.6.4, HEAD, históricos, pins, chave retida, UNKNOWN e reservas preservados.

Evidências e hashes: `OHI-QUALIFICATION.json`, `OHI-SOURCE-HASHES.json`,
`OHI-ARTIFACT-HASHES.json`. [Próxima continuação técnica](NEXT-OHI-NATIVE-ORIGINAL-ACQUISITION.md).
