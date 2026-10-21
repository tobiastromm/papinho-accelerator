# Contratos focais — PROPOSED

Não accepted; nenhuma implementação dependente autorizada por este documento.

## I — interface_ref

Proposta I1: interface_ref contém somente a representação decimal unsigned64 do PersistentID machine-local usado pelo CLI --interface-id, entre aspas segundo a gramática accepted. Sequência não vazia de dígitos ASCII, valor <=18446744073709551615, sem sinal, whitespace interno, nome, IP ou índice. Preservar a compatibilidade lexical do CLI, incluindo zeros iniciais; duplicação é por valor tipado, não por grafia. Valor zero é válido somente com presença/is_valid factual. Ausência jamais vira zero.

I2: resolver no mesmo snapshot de descoberta local, por identidade persistente; usar os resolvers canônicos. Ausência, mudança do token, duplicidade, indisponibilidade, target sem suporte ou falta de endereço bindável negam. Mudança de índice/nome/IP não autoriza fallback; o token identifica a interface, endereços são resolvidos separadamente. Não persistir instância runtime como autoridade.

I3: a declaração não autoriza ingress/egress, abertura de socket ou aplicação operacional. Snapshot e pin são empréstimos com lifetime explícito, exclusivos durante chamada síncrona; Stop/alteração de CURRENT invalida a resolução. Não criar registry de aliases.

## A — provider_ref nomeado

A1: futura implementação deve resolver declaração nominal para identidade e revisão de um provider de autenticação de cliente governado pela única root, com kind, suporte do target e escopo explícitos. Usar a source de segurança existente e seu ownership; nenhum segundo registry. A publicação administrativa desses descritores deve ter contrato próprio antes do código dependente.

A2: separar callbacks/credenciais de certificado, usuário/senha, autorização administrativa, enrollment, Principal e grants. O backend OpenSSL/CNG não é provider_ref. NONE não satisfaz exigência CLOSED. Certificado de canal não é autorização de cliente por si só.

A3: descritor deve vincular security source, callbacks existentes e leases factual/imutavelmente à revisão admitida; negação em ausência, ambiguidade, revisão obsoleta, source incompatível, pin inválido, Stop, retenção ou target sem suporte. Não serializar ponteiros/callbacks, segredos ou credenciais no papacc.conf. Não chamar callbacks de autenticação como teste de existência de referência.

A4: aceitar um provider não concede grants nem aprova revogação de Sessions ativas. Novos efeitos exigem autoridade CURRENT aplicável; snapshot antigo preserva memória/ownership, não nova autorização.

Gate: owner deve aceitar o contrato exato e selecionar o conjunto de providers efetivamente disponíveis na source canônica. Esta proposta NÃO declara provider válido existente, NÃO implementa catálogo e NÃO decide RBAC geral, calendários ou novas exigências da Phase 6.
