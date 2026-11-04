# Startup original e readiness diagnóstica fechada — proposta focal

Status: PROPOSED. Nenhuma autorização operacional. Continuação do contrato SCQ
existente; não cria owner, serviço, root, ledger, IPC genérico ou formato persistido.

## Lacuna demonstrada

F3 preserva referências originais no mesmo owner; F4 limita o ato F0 à invocação
viva; F6 proíbe restaurar confirmação/consumo/autoridade após restart por arquivo,
log, novo token ou CLI. OHI exige decisão SCQ independente e atribuída. A regra7
SCQ mantém recursos, lifetime e UNKNOWN. A especialização M5/P3 escolhe identidade
SCM, mas não designa uma fonte original anterior ao callback do ServiceMain.
Não há conflito entre essas normas; falta essa designação e o significado de
readiness de um preparo fechado. LAB_READ_ONLY não fornece autoridade; observar
um host já RUNNING não pode autorizar o próprio startup.

Campos sem fonte legítima: referência original de admissão; associação à
instalação/alvo; escopo independente; lifetime/consumo; contrato de readiness.
Metadados de imagem/SCM/token não completam esses campos.

## Alternativas

A. Somente startup produtivo: adquirir provas originais da instalação já publicada,
configuração, imagem, conta, CURRENT e continuidade, sem restaurar ato F0. SCQ
aguarda readiness ordinária. Não resolve por si a qualificação anterior à primeira
instalação; não se presume que exista CA anterior a NEW_INSTALL.

B. Recomendada: especializar o preparo diagnóstico fechado do MESMO runtime
PERSONAL_LOCAL. Permite qualificação anterior à instalação produtiva, com fonte
administrativa original e lifecycle independentes. Requer implementação posterior
e prova bilateral real separadamente autorizada. A aprovação não produz provas.

## Regras indivisíveis S0–S7 da alternativa B

S0. Escopo exclusivo SCQ PERSONAL_LOCAL, mesmo owner/runtime; não é NORMAL,
RECOVERY, LAB, NEW_INSTALL nem CA_INITIALIZE.

S1. O controlador original fornece decisão SCQ específica e atribuída, destinatário,
alvo congelado, propósito e tentativa. Usar OHI e referências originais existentes;
consumo único antes dos efeitos, sem loader/arquivo de grants ou autoridade
administrativa reutilizável. Não restaurar atos F0 após restart.

S2. Obter e revalidar identidades originais, token completo, PID/criação, imagem,
conta SCM e associação ao alvo. Gates frescos obtêm identidades que só existem
na sessão. Nome, SID resolvido, hash ou presença de registro não autorizam.

S3. START_PENDING pode preparar somente metadados e referências permitidos.
Readiness diagnóstica exige todas as provas originais, exclusividade, limites,
fechamento e Stop governados. RUNNING só pode representar esse perfil fechado
quando factual e explicitamente identificado; nunca para apenas satisfazer peer.
Readiness produtiva permanece distinta e negada sem seus próprios requisitos.

S4. Sem rede produtiva, CA, CURRENT, emissão ou acesso KSP por consequência de
startup. Endpoint, probe CNG e lifecycle/Start/Stop exigem autorizações operacionais
independentes; parent também quando necessário. Não herdar exceções LAB.

S5. Preservar lifetime de originais, consumo/replay/forwarding, exclusividade e
validação bilateral independente. Stop/perda do peer/falha parcial/fechamento
incerto mantêm UNKNOWN e retenção governada. Não realizar retry inferido.

S6. Preservar 124/128/1024, quotas, relógios e freshness existentes; não inventar
prazos, créditos ou ceilings. Não serializar ponteiros nem transportar segredos,
tokens impersonáveis ou autoridade reutilizável. N0 permanece separado.

S7. Aceitação autoriza somente integrar/qualificar offline esse contrato no owner
existente. Antes de operação: qualificar producer original e readiness, concluir
bindings e artefatos, preparar pedido exato e obter aprovações operacionais.
Nenhum endpoint real ou efeito protegido decorre desta decisão normativa.

## Evidência necessária

Testar ausência/substituição de origem, identidade/alvo/escopo divergentes,
replay, restart sem restauração de autoridade, concorrência, Stop e UNKNOWN;
separar readiness diagnóstica/produtiva e impedir efeitos sem aprovação própria.
Modelos não comprovam autenticação bilateral nativa ou isolamento KSP real.
Preservar compatibilidade e formatos accepted. Mudança persistida não está
aprovada por esta proposta.

Decisão solicitável: aprovar exclusivamente B e S0–S7, condicionada aos bytes
exatos e compatibilidade accepted. A permanece alternativa legítima; nenhuma
aceitação é registrada em nome do proprietário.
