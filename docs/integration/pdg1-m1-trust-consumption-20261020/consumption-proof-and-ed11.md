# Consumo PDG1, lifetime e ED11

## Fronteira implementada

`pc_root8_v3_pdg_select_data` valida a imagem completa pelo codec canônico v3, com todos os validadores DATA obrigatórios, antes de selecionar por ordinal de listener. Copia a linha PDG1, identidade root, digest integral, R/I e estado do certificado para armazenamento próprio. A/P de CP3, quando presentes, são históricos; não são CURRENT. Não há parser, root ou persistência novos.

`mp_auth_pdg_candidate`, chamado pelo adaptador nativo em TRUST e RECHECK_TRUST, correlaciona SHA-256 dos bytes integrais do pacote e SPKI DER, rejeita dados inválidos, propósito divergente e revogação. Mesmo DATA inteiramente correspondente retorna SOURCE_ABSENT. Não há ramo nativo positivo, callback de trust fornecido pelo caller, concessão por boolean, política textual ou digest. O callback existente de componentes root serve exclusivamente a validação pública DATA, síncrona, sem I/O, KDF, operações privadas, efeitos ou retenção. Perfil completo de certificado exige fonte/adapter governado ainda pendente.

A sequência existente STRUCTURE → SIGNATURE → TRUST → KDF → AEAD → CONTENT → RECHECK_TRUST → RELEASE foi preservada. O novo filtro executa depois de RSA público e antes do KDF. Sem CURRENT, policy, pin e confirmação independente factuais, DENY. Dados decodificados não produzem RESOLVED, autenticidade de origem, autorização ou créditos. CONTENT completo segue sem fonte operacional ligada.

## Prova por função e ownership

| Função/objeto | Início e fim factual | Ownership e retenção |
|---|---|---|
| PC_PDG1_SELECTION | após validação integral, até descarte da operação | cópia própria 488 bytes, dentro de MP_AUTH_OPERATION; não contém ponteiro ao produtor |
| MP_PDG_INPUT e root candidata | antes de run, até retorno final ou finish factual | caller existente mantém imagem/contexto imutáveis, disjuntos e serializados; não é pin CURRENT |
| arguments/budget | antes de efeitos criptográficos | inclui metadata PDG e bytes root; rejeita alias, excesso e provider desconhecido |
| mp_auth_pdg_candidate | chamada síncrona TRUST | não adquire pin nem autoridade; Stop antes/depois do codec; nenhuma consulta bloqueante ou novo worker |
| abort_operation/finish | até quiescência factual | incerteza retém operação, buffers, input, senha, contexto e créditos; finish não repete etapas nem publica resultado tardio |
| wipe/release | somente após quiescência | chave, quarentena e staging limpos; output único após todas as verificações, inalcançável no native atual |

Não emprestar dados de buffers temporários; owner do callback/contexto continua responsável pelo armazenamento transitivo e por impedir mutação concorrente. O helper é síncrono e não deve ser chamado pelo caminho de Stop. Nenhuma liberação por timeout/saída de processo foi introduzida. Os testes de retenção são modelos; não provam pins ou CURRENT reais.

## Censo

| Parcela x64 | Bytes |
|---|---:|
| Base selected conhecida anterior | 2103847 |
| Base legacy conhecida anterior | 2067655 |
| Operação anterior → atual | 256 → 776 |
| Input anterior → atual | 120 → 128 |
| Metadata PDG candidata | 72 |
| Root emprestada máxima contabilizada | 30592 |
| Delta nomeado por operação com root máxima | 31192 |
| Parcela nomeada anterior → atual, sem provider/stack | 200532 → 231724 |
| Pico CRT do teste criptográfico existente | 1290354 |
| Cenário conservador não compartilhado selected | 3625925 |
| Limite Windows explícito | 3145728 |
| Excesso conservador | 480197 |

A seleção 488 e o digest32 já estão dentro da operação776: não somá-los novamente. Envelope, payload e key32 também estão contidos. Quarentena/staging/output máximos são 49654 cada, pacote51042, senha128, port readonly24. A raiz máxima e metadata são simultâneas ao input/seleção, não substituem committed/pending, snapshots ou imagens retidas. O caller deve contabilizar backing excedente, contexto, demais owners e objetos retidos em other_reserved; só descontar sobreposição contábil com prova de identidade e lifetime. Não mover heap/stack para fora do censo.

O pico CRT inclui contexto global de assinatura exclusivo da fixture pública; não é medição operacional do signer. Não excluí-lo por inferência nem declarar que toda composição necessariamente consome esse valor. Stack transitiva, páginas de código, fontes de confirmação, histórico/leases, provider/kernel e ativação real continuam sem censo qualificado. Estimativas numéricas não reservam créditos. Não há novo buffer permanente no runtime, alocador, worker ou configuração de quotas. Ativação dependente DENY pelo cenário acima e custos desconhecidos; ED11 total PARTIAL. Legacy permanece2097152; Windows3145728 somente explícito.

## Evidência e limites

Selected/Legacy Debug/Release: 146 asserções SYNTHETIC_CONTROL_ONLY e1189 de criptografia real offline por configuração, incluindo RSA com chave TEST ONLY pública upstream; não foram criadas chaves/certificados/pacotes operacionais. PDG/issuer/BODY/certificado/approval nas novas fixtures são sintéticos. Correspondência DATA positiva ainda falha em TRUST com zero KDF/AEAD/output. Casos de ausência, revogação, propósito/status/issuer divergentes, pacote divergente, callback ausente, Stop durante DATA, alias, limite e retenção foram exercitados. Pin expirado/CURRENT/trust positivo não têm fonte nativa: não qualificados. Não alegar sucesso positivo por modelos.

14/14 suítes root/issuer/ED11 por configuração preservam ROOT06/ROOT08, LST1, v2/v3, checkpoint e quotas. Nenhum commit v3 ou publicação CURRENT foi executado. Native/model são bibliotecas separadas; hooks do modelo não são API de trust nativa. Primeira compilação falhou em C4456 no teste; correção delimitada aprovada antes do rebuild. Logs iniciais preservados; logs verified/root são os resultados finais.
