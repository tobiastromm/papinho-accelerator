# Alternativa B SCQ — revisão focal para decisão do proprietário

Task PACC-PHASE6-ONA-SCQ-ALTERNATIVE-B-REVIEW. Data: 2026-10-30.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469.

## Resultado

**APTA PARA DECISÃO NORMATIVA LIMITADA; NÃO ACEITA; NÃO QUALIFICADA.**
A conferência integral confirmou SHA256
`e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1`
da [proposta original](ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md).
Seus bytes foram preservados. Este prompt solicita revisão, não contém
aprovação explícita de B. Nenhum registro ACCEPTED foi criado.

A proposta é suficiente como direção normativa restrita para implementar e
qualificar offline um perfil prospectivo SCQ. Não prova as garantias que
exige nem permite declarar o handoff funcional. Aprovar B não promove os
resultados atuais de custódia a prova de autoridade original.

Há uma limitação literal importante na regra1: “sem criação de endpoint real
nesta decisão”. A aceitação do texto exato não deve ser usada para criar ou
conectar endpoints nativos reais, mesmo sob o rótulo de fixture offline.
Compilação, modelos, fault injection e helpers nativos sem endpoint ou efeitos
protegidos permanecem dentro do escopo técnico. Caso o
proprietário pretenda também autorizar ensaio de endpoint real isolado,
isso não deve ser acrescentado silenciosamente ao aceite deste hash.
Não se exige alterar a candidata para decidir sobre seu escopo atual.

## Fontes e precedência

Foram confrontados integralmente a proposta, o [contrato OHI accepted](../../adr/DECISION-personal-scq-original-handoff-20261030.md)
e seu corpo original, [T0–T9](../../adr/DECISION-n0start03-noninherited-local-transport-20261025.md),
[T10–T14/T11](../../adr/DECISION-n0start03-positive-private-transport-composition-20261025.md)
e a [especialização SCM virtual](../../adr/DECISION-personal-scm-virtual-account-20261029.md).
F4–F6 da [autorização pessoal](../../adr/DECISION-personal-first-install-authorization-f0-f9-20261022.md)
mantêm ato contextual, consumo monotônico e proibição de restaurar autoridade
por arquivo, token, CLI ou restart. M5/P3/P4 e as decisões de layout não são
reabertos nem recebem efeitos por referência.

T0/T11 regulam o canal N0 supervisor/qualificador: stdout e estabelecimento,
sem autoridade, comandos, serviço ou runtime. No perfil de T2, o código atual exige igualdade de
TokenUser/AuthenticationId/sessão. B regra1 mantém esse perfil intacto;
regras2–3 propõem OUTRO perfil explícito no mecanismo existente para peers
administrativo/virtual distintos. Compatibilidade é, portanto, prospectiva e
por escopo, não autorização atual de usar o endpoint N0 para SCQ.

**GOVERNANCE CONFLICT** existiria se a implementação simplesmente alargasse o
dispatcher N0, retirasse `same_token`, reutilizasse seu endpoint/aprovação ou
passasse a transportar autoridade naquele perfil. Não existe contradição
intrínseca entre OHI e T11: seus perfis e finalidades são distintos. Depois
de eventual aceitação, o registro designado deve preservar essa separação e
não editar os documentos accepted históricos.

## Matriz dos requisitos

| Requisito | Cobertura normativa na candidata | Evidência e lacuna exata |
|---|---|---|
| Exclusividade SCQ | Preâmbulo; regras1–3,8: PERSONAL_LOCAL, uma tentativa, serviço/conta/imagem/chave/processo exatos | Política proposta; falta dispatcher SCQ que negue outros serviços/operações e confusão de perfil antes do efeito |
| Autenticação bilateral original | Regra2 exige referências originais, PID/criação, token/grupos/restrições, continuidade e imagem/serviço | Não demonstrada entre processos. Metadados e `same_token` modelado não são essa prova |
| Operador, destinatário, alvo, propósito | Regras2–4 e OHI vinculam todos ao pedido/decisão/attempt/policy | Faltam producer/decodificação/projeção e confirmação atribuída SCQ; arquivos abertos e SID resolvido não preenchem a lacuna |
| Sem autoridade administrativa reutilizável | Regra3 limita leitura/query e proíbe ação administrativa, token impersonável, chave privada, segredo e admissão | Lista exata de tipos/direitos ainda não existe. Deve ser demonstrada em código; “somente leitura” não prova impossibilidade de reutilização/redelegação |
| Consumo/replay/exclusividade | Regra5: controlador consome antes do primeiro efeito, inclusive Start; um empréstimo da tentativa governada | Falta prova do consumo nesse ponto e de sua continuidade. O LONG local do receptor não é persistência nem aprovação nova |
| Stop e UNKNOWN | Regras5–6: invalidação por perda do controlador/continuidade; referências e consumo retidos; sem retry/reset | Lifecycle do componente está modelado; faltam as duas pontas e I/O real. Não confundir cancelamento, encerramento de processo e close factual |
| Ausência de uso por outros serviços | Preâmbulo e regras2–3 limitam peers/serviço/conta/imagem e propósito | Faltam negativos cross-service/cross-operation/downgrade; nome de endpoint ou tag SCQ não bastam |
| Sem exposição de conteúdo | Regras3–4: não transferir segredo; projeção tipada, campos ativos conhecidos, desconhecido/secreto/ambíguo nega | Falta schema/decoder/projetor estrito. Arquivos originais inteiros não podem ser emprestados ao runtime se contiverem material fora desse domínio não secreto |
| Separação admin/runtime | Regras2–3: identidades distintas; runtime recebe só empréstimo; sem direitos administrativos | Falta ponte nativa de referências válida entre processos. Chamar o probe no administrador ou impersonar não comprova a negação KSP ao runtime |

As lacunas acima são gates de implementação/qualificação mantidos pela própria
proposta, não resultados PASS ou autorizações implícitas. Não é necessário
inventar valores quantitativos, uma nova root ou prova TPM para aceitar uma
direção técnica que mantém esses gates fechados. B não concede tais garantias.

## Limites e riscos residuais que a implementação deve resolver

1. **Direitos exatos.** Antes de habilitar o empréstimo, demonstrar por tipo
   quais direitos cada destinatário recebe. Não inferir acesso a memória de
   processo, duplicação/impersonação de token, escrita de objeto, alteração de
   segurança, assinatura/exportação ou administração SCM de “leitura/query”.
   Provar rejeição/ausência de cada direito desnecessário, não só o nome do
   perfil. Isso concretiza a regra3; não amplia sua autorização.
2. **Uso após fim e forwarding.** Identidade de processo destinatário, tentativa
   e consumo devem ser revalidados no uso. Copiar um identificador ou duplicar
   uma referência não pode transferir o ato SCQ para outro owner/processo.
   Encerrar o pipe não prova que todas as referências foram fechadas nem
   revoga fisicamente toda cópia possível. Preservar o fault model trusted-OS
   e código controlado; não prometer isolamento contra administrador equivalente,
   injeção no processo confiável, clones ou fencing global.
3. **Dados de entrada.** A projeção precisa ser completa e estrita: tipos,
   limites, duplicatas, campos desconhecidos, truncamento e dados após o fim
   devem ser tratados antes de confirmar. Erro de parse não deve ecoar o valor
   rejeitado ou causar dump do arquivo. Referência de leitura a fonte sensível
   continua proibida mesmo se o console imprimir somente um resumo seguro.
4. **Ordem dos efeitos.** Separar consumo da tentativa pelo controlador antes
   de Start do consumo local do empréstimo pelo receptor antes do probe. O
   segundo não conserta ausência do primeiro. Receipt é evidência de auditoria,
   nunca credencial transferível/restaurável. Não criar ledger paralelo ou
   retomar a cadeia TPM/Q4/C8 por inferência.
5. **Recursos.** Representação privada, tamanho compilado, SDs, duplicações,
   buffers, stack, I/O e retenção precisam entrar no censo. Valores de freshness,
   headroom e limites ainda desconhecidos não serão copiados do LAB. A regra7
   conserva DENY; a norma não transforma o custo fixo observado em capacidade.

Não há lacuna textual que obrigue aceitar autoridade genérica ou alterar
NEW_INSTALL/CA_INITIALIZE. Há obrigações normativas deliberadamente ainda
sem prova de execução. A recomendação é submeter o hash atual ao proprietário
somente como aceitação de direção/implementação offline, com os limites acima
expressamente informados. Se a intenção for qualificá-la como garantia já
demonstrada ou criar endpoint real por esse aceite, a recomendação é **não**.

## Plano de integração offline após aprovação válida

1. Registrar a decisão normativa designada incorporando o corpo exato da
   candidata por hash, preservando original, T0–T14 e OHI. A
   [minuta preparada](ONA-B-ACCEPTANCE.DRAFT.md) ainda não é decisão vigente.
2. Mapear os pontos existentes: `scq_inputs` conserva referências/metadados;
   o producer deve comprovar conteúdo/proveniência e confirmação atribuída;
   `scq_receiver` governa o empréstimo; `scq_dispatch` continua separado da
   admissibilidade. Nenhuma flag, callback positivo ou self-pointer será a fonte.
3. Definir em código a representação privada tipada e o mapa exato de direitos,
   vinculados ao owner/peers originais. Manter o N0 inalterado e testar seleção
   explícita de perfil; mensagem de outro perfil ou operação nega sem fallback.
   Não serializar ponteiros locais como referência remota válida.
4. Implementar decoder/projetor estrito de plano não secreto, vínculo aos
   objetos originais e confirmação administrativa atribuída; excluir matéria
   secreta de transporte, console, logs e falhas. Fonte ausente/incompleta nega.
5. Compor consumo pré-efeito no controlador, entrega, uso restrito e closes
   independentes. Modelar falha antes/depois de consumo, Stop e perda do peer
   em cada fase. Nenhum ack de canal ou término local confirma a outra fase.
6. Qualificar em modelos/fault injection: par correto, fonte/objeto/SD/token/
   imagem/conta/serviço/alvo/propósito/revisão divergentes, replay, expiração,
   concorrência, substituição, escalada de direitos, forwarding, descarte/
   truncamento, falha de persistência/close e UNKNOWN. Compilar Selected/Legacy
   Debug/Release com warnings tratados como erro; usar regressões afetadas.
7. Manter endpoint real e sessão SCM/CNG fora do alcance desta decisão exata.
   Depois da implementação offline, identificar a menor prova nativa autorizável
   necessária, sem alegar que os modelos a substituem. Nenhum pedido operacional
   com placeholders ou binding histórico será emitido.

## Evidência reaproveitada e primeiro gate posterior

Foram verificados os hashes das **38 fontes** do manifesto ONA: inalteradas.
A evidência anterior de quatro builds/104 CTests permanece válida no escopo
registrado em ONA-QUALIFICATION.json. Nenhum build, teste nativo ou operação
foi repetido nesta revisão documental; não há contagem nova de qualificação.
O arquivo `scq_transport_scope_test.c` testa o predicado real com APIs de token
modeladas, SID helpers reais e zero chamadas de pipe. OHI usa referências
locais; a entrada real continua NULL/DENY antes do KSP.

O primeiro gate atual é a decisão explícita do proprietário sobre B. Depois
dela ainda existe trabalho técnico, não somente autorização operacional.
O primeiro gate operacional posterior será a aprovação específica da prova
nativa mínima que o código offline deixar pendente; se já houver condições
suficientes, poderá ser uma única sessão SCQ com pedido completo, artefatos,
bindings e gates frescos. Nesta revisão não é possível afirmar qual dessas
provas já pode ser agrupada. Criação do parent permanece efeito independente
se necessária. A revisão não emite esse pedido nem permite Start/Stop.

Nada foi instalado/publicado/executado: nenhuma conta/diretório/CA, CURRENT,
SCM/ACL, rede operacional, TPM/NV/Q4, limpeza/refund ou recuperação. Nenhum
código, contrato accepted ou matriz de status foi modificado. ED11/Phase6
permanecem PARTIAL e autoridade DENY. HEAD/WIP/históricos/pins/chave fixture
retida/UNKNOWN/reservas/PST0.6.4 preservados.
