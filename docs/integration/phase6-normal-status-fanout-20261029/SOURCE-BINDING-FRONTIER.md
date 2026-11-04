# Fontes NORMAL — primeira lacuna factual e limites de integração

Não foi localizada uma instância administrativa operacional root/CA legitimamente publicada para esta composição. Os fixtures canônicos demonstram transações reais de arquivos locais, não uma CA produtiva nem autoridade de emissão. Não os promovemos.

| Fonte | Implementação existente | Por que não fornece o fato NORMAL completo |
|---|---|---|
| Chave/CA | server_issuer_cng_backend_win32.c, acquire/public_match; server_ca_key_win32.c | Valida uma key já adquirida e o SPKI. O launcher/issuer live não recebe uma fonte original CA/cert/geração com custódia e lifetime estabelecidos. Um ponteiro, nome ou digest proposto não supre isso. |
| Custódia da cerimônia | personal_ca_bind/current no mesmo commit owner | bind rejeita o estado published. A única borrow está no pacote personal da cerimônia e a validação exige aquele contexto original. Não existe transferência qualificada para o issuer live; não se apagou/reutilizou o pacote. |
| História/serial | PACIS601, checkpoints, worker nativo e snapshots | Conteúdo/ordem/high-water/CAS local são verificáveis. Não provam por si a origem original da autoridade, unicidade do prefixo nessa geração e a fonte de exclusividade entre emissores exigida por M3. |
| Writer | writer thread + CAS/TxF de cada transação | O thread owner e o handle exclusivo durante um commit não equivalem a uma prova completa de exclusividade de emissão durante todo o lifetime. |
| CURRENT/P | preparação/publicação canônica e pin/snapshot existentes | Há fatos reais no escopo do fixture. Sem as fontes anteriores, não autoriza preencher KEY_BINDING/ORIGINAL_HISTORY/EXCLUSIVE_WRITER ou ativar assinatura. |

**Primeira aquisição faltante:** binding original CA/key/cert/generation e sua custódia ao owner/root que será usado pelo issuer live. O equipamento tem evidências N0 históricas; elas não são essa autoridade C8.

Para a prova positiva, é necessário fornecer uma fonte original existente verificável ou autorizar separadamente uma cerimônia isolada que materialize os objetos protegidos necessários ao teste. Esta tarefa não executa criação de key persistente MACHINE, bootstrap/root produtiva, ACL/SCM ou assinatura real para preencher essa falta. Não se pede segredo, não se reabre M3 e não se presume uma autorização operacional a partir da permissão de alterar código.

O código novo de status/fanout continua independente dessa aquisição: não concede provas NORMAL e só pode atuar a partir de uma credencial exata encontrada no CURRENT original. Fontes ausentes negam. A confirmação PUBLISHED ainda exige a resolução descrita em PUBLISHED-ACK-FRONTIER.md; por isso não se declara composição NORMAL completa nem fanout nativo positivo demonstrado.
