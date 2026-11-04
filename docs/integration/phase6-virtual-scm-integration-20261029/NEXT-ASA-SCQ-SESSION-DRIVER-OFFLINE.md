# PapinhoAccelerator — SCQ: driver da sessão e entrega bilateral offline

TASK: PACC-PHASE6-SCQ-SESSION-DRIVER-OFFLINE-READINESS

Continuar ASA-CONSOLIDATED-CLOSEOUT.md. Fonte administrativa ligada ao receptor,
B/OHI/F1–F6 accepted; não reabrir nem repetir essa integração sem regressão.
O registro operacional nativo permanece vazio: não preenchê-lo por inferência.

Concluir no owner/mecanismo existentes a composição técnica offline da sessão e
da entrega bilateral. Reutilizar primitivas de transporte, separando a política
SCQ de N0 e mantendo T0–T14 intactos. Não serializar ponteiros/handles locais como
originais remotos; adquirir e revalidar identidades e direitos mínimos nas duas
pontas. Não promover modelos/recibos/registro vazio a autoridade real.

Preparar os caminhos nativos de aquisição/entrega/fechamento usando APIs modeladas
para efeitos de endpoint. Nenhum endpoint real pode ser criado, conectado ou
testado, inclusive em fixture. Não criar outro owner, serviço ou IPC genérico.
Qualificar perda de peer, substituição, token/grupos/restrições, direitos excessivos,
forwarding, consumo antes do primeiro efeito, concorrência, Stop e UNKNOWN.
Não declarar autenticação bilateral nativa ou quiescência por testes de modelo.

Congelar artefatos qualificados/pins reais e separar bindings prévios dos gates
frescos do controlador/runtime. Planejar endpoint, sessão SCM/CNG, eventual
implantação/Start/Stop, chave MACHINE e eventual parent como efeitos separados.
Não copiar headroom/freshness do LAB nem inventar valores/provider/kernel.
Pedido operacional específico somente quando bindings, driver e requisitos
estiverem completos. Não criar aprovação ou comando com placeholders.

Sem instalação, diretórios/contas/CA, CURRENT, Start/Stop real, SCM/ACL, rede,
TPM/NV/Q4, limpeza/refund ou recuperação. Preservar HEAD/WIP/históricos,
chave fixture retida, UNKNOWN, 124/128/1024 e PST0.6.4.

Entregar implementação/testes afetados, limites, primeiro gate factual e pedido
candidato somente se completo. Manter DENY e ED11/Phase6 PARTIAL sem prova factual.
