# SCQ — aquisição e entrega dos originais sob contrato já accepted

TASK: PACC-PHASE6-SCQ-NATIVE-ORIGINAL-ACQUISITION-AND-DELIVERY.

Continuar a implementação autorizada por PACC-PHASE6-OSR-SCQ-HANDOFF-ACCEPTANCE-AND-INTEGRATION.
Ler OHI-CONSOLIDATED-INTEGRATION.md, OHI-QUALIFICATION.json e
docs/adr/DECISION-personal-scq-original-handoff-20261030.md. A proposta original
34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561
está ACCEPTED pelo registro; não pedir outra aprovação da mesma norma.

O receptor de empréstimo está modelado/qualificado, mas a entrada do owner
ainda recebe NULL por ausência do produtor original. Não renomear esse
estado como operação pronta. Não remover o gate de origem nem substituir NULL
por uma estrutura preenchida pelo caller, association/hash, self-pointer,
receipt ou valor aprovado sintético.

Concluir a aquisição nativa de pedido/decisão byte-exatos como objetos
originais protegidos, identidade de operador/processo/token administrativo,
conteúdo e descritores, confirmação local atribuída do plano, consumo único
no controlador antes do primeiro efeito, e entrega somente de empréstimo
ao runtime original por PID/criação/token/imagem/chave e propósito SCQ.
Reutilizar primitivas e owners existentes; não introduzir serviço, authority
provider, ROOT/CURRENT ou IPC genérico por inferência. Se as primitivas existentes
não puderem comprovar a fronteira exigida, demonstrar a incompatibilidade
concreta e completar componentes independentes sem fabricar proveniência.

Qualificar sender/receiver juntos em fixtures explicitamente isolados:
conteúdo/SD/referência/proveniência divergentes, operador/destinatário,
expiração, consumo antes de Start no modelo, replay, crash/UNKNOWN, perda do
controlador, concorrência e fechamento. Não reutilizar F0/CA_INITIALIZE nem
aprovações TPM/LAB. Source view e verificações do receptor não bastam como
prova de origem, proteção ou consumo durável.

Somente depois de completar esse código e os bindings, preparar UM pedido
operacional SCQ, sem aprovação inventada nem comando prematuro. Parent não
adquirido: criação se indispensável exige autorização própria. Não criar
diretórios, serviço, conta, chave/CA produtiva ou CURRENT. Não executar SCM/CNG
real, Start/Stop, SCM/ACL, rede, TPM/Q4, cleanup/refund ou recuperação.
Preservar WIP/HEAD/históricos/pins/chave retida/UNKNOWN/reservas/PST 0.6.4.
Foco exclusivo primeira instalação; sem mudar para E8/Browser/Legacy.
