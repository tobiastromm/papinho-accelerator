# Continuação delimitada do issuer/root

Revisar o CONSOLIDATED-CLOSEOUT e QUALIFICATION deste pacote e preservar o WIP. O core já suporta CAS pendente; não refazer essa implementação. A ligação nativa ao root não foi aplicada: a revisão automática rejeitou a rota por potencial publicação CURRENT fora do escopo autorizado. Não contornar essa rejeição.

Após autorização específica para a integração de código nativo e revisão dos limites, qualificar somente em fixtures exclusivos a ligação prepare/begin/poll/publicação, sem executar CURRENT produtivo, serviço, rede, assinatura autoritativa ou recuperação. Definir corretamente epochs A/P e confirmação por releitura antes de declarar sucesso. Implementar o produtor NORMAL e fanout somente com fontes factuais e owners reais; ausência mantém DENY/SOURCE_ABSENT. Preservar gates L2, E8 e D4/D5; não retomar TPM por default.
