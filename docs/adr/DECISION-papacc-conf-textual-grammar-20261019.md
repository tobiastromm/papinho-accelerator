---
status: accepted
revision: 1
decision-date: 2026-10-19
scope-level: project
scope-target: PapinhoAccelerator offline configuration parser
supersedes: []
decision-makers: [Tobias Tromm]
---
# Aceitação focal da gramática textual papacc.conf

Autoridade: decisões explícitas do owner no slice PACC-PHASE6-PAPACC-CONF-PARSER-LISTENERS-1: aprovar gramática proposta, codec pendente; preservar end explícito e indentação exclusivamente visual. Corrige a descrição anterior do chat de blocos semânticos por indentação, sem modificar ADR accepted existente. Registro sujeito à revisão oficial da operação exata e verificação de hashes antes da implementação dependente.

## Cláusulas aceitas

F1. UTF-8 válido, NUL proibido, BOM apenas no início, LF/CRLF. Uma diretiva por linha; espaços/tab separam tokens. Indentação não possui efeito semântico. Bloco começa por tipo e id e termina por end sozinho, sem nesting. Não introduzir parser alternativo baseado em dedent.

F2. Strings usam aspas duplas. Os únicos escapes são barra seguida de barra (produz barra) e barra seguida de aspas duplas (produz aspas). Escape desconhecido, aspas não fechadas e controle/newline em string falham. # fora de aspas comenta integral/inline; ; primeiro não branco desativa a linha inteira; dentro de string é literal; outro uso lexical fora de comentário é erro. Linhas desativadas não produzem nós/ref/regras/listeners. Preservar localização/ordem física. Bloco ativo incompleto falha sem defaults permissivos.

F3. Aceitar a estrutura textual, identificadores ASCII, inteiros unsigned decimais com overflow rejeitado, versão1 explícita, catálogo fechado de blocos/campos e aridades/valores/obrigatoriedade descritos na fonte identificada abaixo e directive-matrix.md da mesma pasta. Estas definições são sintaxe e validação de candidate, não criação de operadores/ACLs nem suporte operacional. Campo conhecido sem implementação no target rejeita o candidate inteiro. Nenhum bloco não utilizado é silenciosamente ignorado. Não incluir includes, expansão de ambiente, interpolação ou comandos.

F4. O primeiro slice pode implementar apenas subset explicitamente anunciado e testar candidates offline. Limites de implementação bounded e armazenamento/scratch são sujeitos à revisão exata do patch/censo, sem elevar quotas ou congelar arquitetura. Coleção listeners[] com identidade, binding, porta, perfil explícito, referências de segurança e contexto administrativo continua exigida; target inicial um listener rejeita excedente, sem truncar. Ausência não seleciona Legacy. Default deny/gates independentes e separação Principal/usuário permanecem.

F5. Esta aceitação não aceita layout binário/integridade de codec novo, nem autoriza usar bytes reservados, alterar BODY0.5, publicar CURRENT/A/P ou escolher epoch. Codec permanece PROPOSED até revisão específica. Não exige implementar codec não autorizado para concluir o subset independente. Não implementar ativação/reload/config apply produtivo. Uma única root e I/R/A/P/CAS/snapshots/pins/Stop/Model E/ED08/ED11 permanecem integrais.

F6. Submeter patch funcional exato antes de aplicar. Parser portável sem CNG/Win32, nenhuma credencial/CA/certificado real/rede, Browser/PST/PACC wire/produção0.6.4 inalterados. CLIENT_ADMISSION=DISABLED, SITE_TLS_ADMISSION=DENY, ED11_ADMISSION=DENY. Provider/kernel UNKNOWN; nenhum PASS operacional por parse/build. PST0.6.5 candidato até encerramento integral da Phase6.

## Fonte e precedência delimitada

Fonte lexical/estrutura: docs/integration/squid-inspired-configuration-contract-20261019/grammar-proposed.md SHA256 904bc187378dd8101b2cbca113a855d57d22e578e5e9300b34a8f55ebe8316d5; catálogo: directive-matrix.md SHA256 39ed3a0e15628717ed1f7c3791cf0f602667fb43dc01b739399a77973f08d293. Aceitação restrita a léxico/estrutura/presença/validação: não incorpora histórico que diz permanecer PROPOSED, codecs, parâmetros não aprovados ou handoffs funcionais. O texto F1–F6 resolve a ambiguidade tipográfica de escapes e a menção incorreta a indentação semântica. N1–N8 da decisão papacc-conf-comments-acls-listeners, ADR0009/0010/0012/0028, root/C8, issuer/C2 e decisões SiteTLS continuam preservados, sem substituição/revogação. ACLs futuras não se tornam novas obrigações Phase6 nem diretivas executáveis.
