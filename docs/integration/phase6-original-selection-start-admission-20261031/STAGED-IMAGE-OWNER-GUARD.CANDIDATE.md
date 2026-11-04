# Correção técnica focal proposta — owner da imagem original

NÃO APLICADA. Não é ADR accepted, autorização operacional, seleção de imagem ou alteração da política vigente. A revisão automática rejeitou o patch de produção por entender que admitir Administrators no lugar do ator é uma redução persistente de fronteira de autorização não explicitamente aprovada. Esta minuta deixa a mudança concreta revisável para decisão humana; não contorna a rejeição.

## Evidência estática e offline

O stager qualificado em server_launcher_qualification_staging_win32.inc constrói exatamente `O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)`. O resultado histórico QUALIFICATION_COPIED_VERIFIED_CLOSED verificou o conjunto limitado; não se consultou novamente o alvo protegido nesta execução.

personal_scq_inputs_protected exige `EqualSid(owner, actor)` para request, decisão E imagem. Portanto o descritor produzido pelo stager e o predicado do consumidor são incompatíveis quando o ator é o SID pessoal e o owner da imagem é BA. O novo teste scq_staged_owner_boundary executa a guarda INALTERADA sobre esse SD somente em memória e exige sua rejeição. Não usa arquivo real, consulta de token, hash CNG, endpoint ou ACL real. Isso demonstra incompatibilidade entre políticas implementadas; não é observação fresca da ACL atual do alvo.

## Alteração exata solicitada

Somente no consumidor SCQ, distinguir os papéis fixos das três referências: request(1), decisão(2), imagem(3). O índice interno da referência, não um flag de caller, fornece o papel. Rejeitar qualquer outro papel.

Para request e decisão: manter owner igual ao SID do operador original, sem exceção.

Para imagem: admitir owner igual ao SID do operador original OU owner identificado nativamente como WinBuiltinAdministratorsSid, exclusivamente no propósito TRANSPORT_ONLY(2) validado pelo contexto SCQ original do mesmo owner, para consumir a imagem staged protegida deste fluxo PERSONAL_LOCAL. READ e outros perfis conservam a guarda atual de owner igual ao operador. Os chamadores ordinários não podem selecionar a exceção por um bool ou papel isolado; preservar o capturador ordinário estrito e vincular a especialização ao perfil tipado TO existente. Não acrescentar SYSTEM, TrustedInstaller, qualquer grupo ou owner genérico por inferência.

Manter TODOS os outros predicados atuais: descritor válido e DACL protegida/não ausente, tipos/flags/ordem restritos de ACE, proibição de escrita/modificação por principal estrangeiro, metadados/identidade/links/reparse, direitos mínimos, imutabilidade/conteúdo/SD, paths congelados, correlação com imagem/serviço original, token/continuidade, confirmação própria e consumo. Owner BA isolado nunca autoriza uso, Start, endpoint ou readiness. Não editar descritor ou permissões do Windows.

Essa alteração reduz uma exigência atual de owner na imagem e exige avaliação explícita. A justificativa técnica é consumir o descritor já produzido pelo stager governado, em vez de mudar ACLs para satisfazer o consumidor. OHI requer imagem original protegida e owner/SD revalidados; não foi identificada cláusula accepted que exija owner igual ao operador para a imagem. Request e decisão permanecem atribuídos ao operador. Não se propõe alterar OHI/SSO-B/TO ou importar R0–R9/B0–B8.

## Qualificação necessária após aprovação

Aplicar apenas esse predicado. Testar imagem BA aceita no papel3/TO; BA negado em request/decisão e imagem READ; propósito/papel inválidos ou não vinculados; owner estrangeiro; DACL herdada/ausente; escrita estrangeira; ACE não suportada; hash/SD/identidade divergentes; substituição; Stop; retenção UNKNOWN. Executar builds Selected/Legacy Debug/Release e regressões efetivamente afetadas com APIs mutantes simuladas. Não declarar a ACL atual do destino demonstrada por SD em memória.

## Limites da aprovação solicitável

Autorizar somente a correção técnica e sua qualificação offline. Nenhum update, Start/Stop, SCM/ACL real, endpoint, staging, CA/CURRENT, rede, TPM/Q4 ou cleanup/refund. A aprovação não limpa recursos UNKNOWN nem resolve o caller de aquisição administrativa ou o primeiro binding receptor. Nenhuma nova decisão operacional deve ser registrada por consequência.
