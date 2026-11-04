# PapinhoAccelerator — SCQ: adapter nativo de endpoint, sem endpoint real

TASK: PACC-PHASE6-SCQ-NATIVE-ENDPOINT-ADAPTER-OFFLINE

Continuar SDR-CONSOLIDATED-CLOSEOUT.md. Driver DATA e fonte administrativa estão
ligados e qualificados offline; não refazer a composição ou reaprovar B/OHI.
Registro operacional segue vazio e não será preenchido por inferência.

Implementar no mecanismo/owner existentes o adapter nativo de aquisição e entrega
bilateral SCQ usando APIs de efeito integralmente modeladas na qualificação.
Reutilizar primitivas existentes com política SCQ separada; não alterar N0/T0–T14.
Validar nas duas pontas originais de processo/token/SCM/imagem, vínculo da decisão,
referências e direitos mínimos. Não serializar ponteiros/handles locais como
originais remotos, não transferir token impersonável ou autoridade reutilizável.

Integrar custódia de endpoint/eventos/buffers e IO pendente ao driver antes de
permitir close nativo. Cancelamento/receipt DATA não provam conclusão ou ack
autenticado. Testar partial IO, interrupção, perda do peer, substituição, replay,
forwarding, direitos excessivos, concorrência, Stop e retenção UNKNOWN. Não criar
ou conectar endpoint real, inclusive fixture; não executar sessão SCM/CNG.

Identificar a menor prova real com administrador e runtime virtual originais;
same-user não prova o vínculo. Separar bindings prévios de observações frescas.
Pedido de endpoint somente após qualificação/bindings completos; separar sua
aprovação das de sessão SCM/CNG, implantação/Start/Stop e eventual parent.
Não inventar limites/headroom, PID/token futuro, hashes ou comando com placeholders.

Sem instalar serviço/criar diretórios/contas/CA, CURRENT, Start/Stop real, SCM/ACL,
rede operacional, TPM/NV/Q4, limpeza/refund ou recuperação. Sem novo owner, serviço,
root ou IPC genérico. Preservar HEAD/WIP/históricos/chave retida/UNKNOWN/
124/128/1024/PST0.6.4. ED11/Phase6 PARTIAL e autoridades DENY sem prova suficiente.

Entregar adapter/callers, builds/testes afetados, limites da prova, hashes,
primeiro gate real e pedido candidato somente se tecnicamente completo.
