# Custos materiais — aplicação ao primeiro Start fechado

Fundamentos conferidos: F8; W2/W4/W5 do perfil Windows3MiB;
DECISION-ed11-preserve-incremental-resource-envelopes-20261012; OHI (limites);
B7; SSO-B S3/S5/S6; TO7. B0–B8 e R0–R9 são domínios distintos e não se aplicam
por analogia. Nenhum accepted ou quota foi alterado.

| Custo | Materialidade e evidência | Antes / durante / depois |
|---|---|---|
| Owner/processo gerenciado | Censo compilado Selected1993760 e Legacy1825192; quotas3145728 e2097152 | Verificação controlada prévia; não físico/commit/working-set |
| Ato endpoint novo | Campos privados somam16 bytes no layout alinhado do owner; zero novo handle/heap no ato | Censo prévio; incluído uma vez no root, não adicionar novamente |
| Plano/callbacks | Plano local1048; apresentações e chamadas de validação transitivas | sizeof de um objeto não é teto do stack; custo total transitivo não demonstrado |
| Originais | Três file e três reader; cadeia até oito diretórios mais imagem, SDs, buffers hash | Identidade/proteção/lifetime obrigatórios antes da confirmação; guard124 por referência, cap128/1024 nos seus domínios; custos internos de query/hash permanecem opacos |
| SCM e novo processo | Estado SCM compilado14928; PE/DLLs disponíveis em disco | Tamanho do arquivo/sizeof não prova loader/CRT/stacks, memória residente ou RPC; suficiência/política anterior ao efeito obrigatória |
| Endpoint privado | Buffers/OVERLAPPED já limitados; record1048, receipt1052, close1076 | Custódia controlada demonstrada; pipe/event/kernel/IO e alocador não têm máximos completos demonstrados |
| Host | Histórico não é margem fresca | Observar/fundamentar físico/commit/contagem, idade e janela próprias antes do efeito; não importar pisos B3/R0–R9 |
| Runtime real | PID/criação, TokenUser/grupos/restrições, SCM/imagem e contagem | Só após Start. Não é possível nem exigível adquirir PID/token antes; gates tardios antes de promoção/readiness |
| Receipt/close/Stop | Máquinas e negativos qualificados em modelos | Só após entrega. Deadline/Stop solicitado não prova cancelamento, fechamento kernel ou quiescência OS |
| Retenção | Originais/buffers/IO retidos no mesmo owner | Conta enquanto não fechar factualmente. Não usar crédito futuro presumido, refund ou término por timeout |

Mantêm-se resource_pre_missing63 (seis termos), resource_effect_obligations7
(três obrigações) e o bit de recurso obrigatório. A nova confirmação endpoint
NÃO altera esses termos. O primeiro gate de criação consulta essa admissão antes
de consumir o ato endpoint e antes de abrir provider/pipe/event.

Custos mensuráveis prévios: sizeof/slots, aritmética controlada e observações
frescas do próprio controlador/host. Custos sem máximo disponível: processo novo,
RPC/loader/provider/kernel/CRT e sobreposição nativa/retida. Provas posteriores:
PID/token/processo real, autenticação bilateral, uso observado, receipt/close.
As duas últimas classes não devem ser confundidas; medir depois não provisiona
antes e desconhecimento anterior não dispensa identidade posterior.

TO7 torna indispensável uma escolha de garantia caso se pretenda executar sem
máximos opacos completos. Uma nova coleta pontual não resolveria essa escolha.
A proposta única FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md compara A integral
com B limitada e delimita a precedência necessária. Continua PROPOSED, não
implementada como política de capacidade. B não é herdada de staging/registro,
não aumenta quotas e não seleciona números operacionais por inferência.

Mesmo aceitando B futuramente, faltariam os producers frios, plano quantitativo
concreto, atualização de imagem e autorizações próprias. Não se promete que
aprovar risco torne uma futura contagem/margem insuficiente admissível.
