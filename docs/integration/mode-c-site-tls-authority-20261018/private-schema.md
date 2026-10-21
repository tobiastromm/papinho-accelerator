# Catálogo privado Site TLS Mode C — implementação inicial

Autoridade: DECISION-mode-c-site-tls-profile-authority-20261018 accepted, sob C2§7, ADR-0044/0048 e a decisão de consolidação. Entrada programática no Configuration Model existente; nenhuma nova source, root, login ou Authorization Provider.

## Modelo
Uma entrada por configuração CURRENT, armazenamento por valor de200 bytes no targetx64. Capacidade fixa; não há registry dinâmico. Identidade de catálogo, identidade de perfil e origem administrativa:16 bytes públicos cada, todos explícitos/não zero. Não são Principal, credential ou prova de autenticação. Versão1, revisão de catálogo/perfil U64 não zero, referência U32. Referência0 só é default se explicitamente cadastrada.

Trust inicial SYSTEM explícito; CUSTOM e UNKNOWN não suportados são rejeitados. Hostname/SNI obrigatórios e TLS1.3 only. ALPN é vetor exato de até64 bytes [U8 tamanho, protocolo opaco], sem entradas vazias/duplicadas/trailing bytes. Não amplia a gramática PACC. Revogação explícita; janela temporal [not_before,expires) em milissegundos FILETIME UTC. max_bytes/max_duration não zero; max_concurrent1..2. Ausência é estrutura inteiramente zero e nunca default permissivo.

Esses limites descrevem intenção administrativa. A aplicação operacional de ALPN, byte budgets, concorrência e deadlines será conectada ao dispatch antes de qualquer habilitação. O presente gate C3 retorna a negação da boundary de admissão existente: armazenar limites não é comprovar sua execução.

## Persistência BODY0.5
Journal existente8192 bytes, até quatro commits do componente, sem aumento de slots, buffers ou quotas. O entry point root permanece first-commit-only. Discriminantes PACL0501, CONF0501 e ADMP0501 são exclusivos; não há autodetect/migração.

BODY tem1024 bytes. Campos0..743 preservam a gramática explícita Principal16 do BODY0.4; a versão do envelope é nova. Extensão:
| Offset | Conteúdo |
|---:|---|
|744|SITE0101,8 bytes|
|752|catalog identity,16|
|768|profile identity,16|
|784|administrative origin,16|
|800|catalog revision,U64BE|
|808|profile revision,U64BE|
|816|not_before UTC ms,U64BE|
|824|expires UTC ms,U64BE|
|832|max bytes,U64BE|
|840|max duration ms,U64BE|
|848|tls_profile_ref,U32BE|
|852|max concurrent,U32BE|
|856|revoked,0/1|
|857|perfil fixo1: SYSTEM/hostname/SNI/TLS1.3 obrigatórios|
|858|ALPN length,1..64|
|859..863|zero|
|864..927|ALPN vector e padding zero|
|928..991|zero|
|992..1023|SHA256 bytes0..991|

O journal mantém hash, cadeia, identidade de arquivo, predecessor e commit nativo existentes. Integridade não é prova universal contra restore malicioso. A reutilização interna do codec comum04 usa cópia local explícita, sem escrever/migrar registros históricos. Leitor04 rejeita05; leitor05 rejeita04. Encoder04 rejeita catálogo presente. Recovery/normal restart05 não foi habilitado; não adotar arquivo05 por interface histórica.

## Binding e segurança
A consulta administrativa exige owner/invocation, pin/ticket CURRENT, commit factual, epoch atual e conteúdo de catálogo da mesma root. Retorna cópia por valor; valida revisão/ref, janela e revogação. Ausência de CURRENT ou epoch falha fechado. Saída sobreposta à root, ao grafo C2 ou ao pin/entrada é rejeitada antes de qualquer limpeza.

A fronteira C3 exige Session factual current e grants/snapshot pela lógica existente, perfil CURRENT, trust SYSTEM identificado no próprio owner e template CLIENT/TLS1.3/verificação de certificado. Não aceita CA inbound, trust arbitrário ou Browser como autoridade. Mantém a boundary operacional existente em DENY; não há caminho positivo de admissão neste slice. Não aloca, não faz file/network I/O, não chama callback arbitrário e não libera quota/recursos.

A leitura de horário é local; não é relógio entre processos nem evidência empírica de provider. Mudanças de configuração não revalidam snapshots antigos automaticamente. Reload operacional e recuperação05 exigem gates próprios.
