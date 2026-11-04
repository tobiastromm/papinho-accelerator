# Reconciliação da consulta manual da cadeia

## Resultado observado

Saída fornecida pelo proprietário, preservada em OWNER-CHAIN-RESULT.txt. Origem: transcript colado no chat, não captura independente ou atestação criptográfica do processo pelo agente. O hash do arquivo arquivado protege esta transcrição local, não autentica externamente sua execução.

Coleta concluída no alcance declarado: collected=1, first=0, exit=0, SUBMISSION_ATTEMPTED=True. Dois objetos existentes, primeiro ausente índice 2/Papinho. Fechamento controlado conhecido, retained=0, EFFECTS=0; quiescência interna OS e identidade de boot permanecem UNKNOWN. Nenhum erro operacional informado. A razão estrutural 11 é diagnóstico de política, não falha da coleta.

Operador SID esperado, host TCORP-TOBIAS, PID 21072, token 0:749039782/ModifiedId 0:699859805. São observações desta sessão, não bindings futuros reutilizáveis. O fluxo qualificado executa revalidação de token primário/identidades/descritores antes do sucesso; a saída não publica todos os valores intermediários ou uma prova independente de cada chamada. O marcador inicial indica que os gates de pins do wrapper foram ultrapassados; não publica novo digest nem atesta imagem mapeada.

## Objetos e descritores

| Objeto | Identidade volume/file | Descritor OWNER+DACL | Resultado |
|---|---|---|---|
| C:\ | 417614100 / 327680:5 | 168 bytes; dd12fd358f9aec2929310ef1c7601e65a84c9960da68a7e46fd1a3a5f1a924c2 | reason 11, ACE 4, máscara 0x4 |
| C:\Program Files | 417614100 / 131072:226316 | 392 bytes; 3e56550c9b305c98d4ca07f98ba18e2be96c86546539cd09b024ad506c838f40 | reason 0; índice MAXDWORD significa nenhuma ACE rejeitada |
| Papinho | nenhuma identidade atribuída | não adquirido | primeiro componente ausente |
| Accelerator | não consultado | não adquirido | consulta encerrou antes deste componente |

Ambos os objetos adquiridos são diretórios sem flag REPARSE_POINT. Owner TrustedInstaller em ambos; controle 0x9404 inclui DACL protegida. A identidade/hash da raiz coincide com a evidência histórica e sua projeção OWNER+DACL. Isso confirma a condição na coleta atual, não recupera retroativamente o descritor não salvo da tentativa de staging anterior nem prova continuidade ininterrupta entre as sessões.

## Distinção de direitos demonstrada

Na raiz, a ACE AU de índice 4 é ALLOW 0x4, flags 0, aplicada ao próprio objeto: criação de subdiretório. A outra ACE AU (índice 3, 0xe0010000, flags 0x0b) é INHERIT_ONLY, não concessão efetiva sobre a própria raiz. Users tem leitura/travessia; não aparece DELETE_CHILD, WRITE_DAC/WRITE_OWNER ou outro bit de escrita aplicável à própria raiz para esses trustees não confiáveis no conjunto observado. Privilégios e handles previamente concedidos de todos os terceiros não foram enumerados; não declarar proteção universal.

Program Files tem DACL protegida e passou na guarda estrutural: direitos de modificação efetivos observados pertencem a TrustedInstaller/SYSTEM/Administrators, não aos trustees de leitura Users/App Packages. As ACEs INHERIT_ONLY, inclusive Creator Owner, não são permissões sobre o próprio Program Files. A concessão AU/0x4 na raiz não demonstra capacidade de substituir este filho existente protegido; a avaliação não é um AccessCheck geral de qualquer token.

Papinho está ausente nesta captura: não há identidade factual para adotar. Um terceiro que pudesse ocupar um nome futuro não receberia autoridade de instalação: criação exclusiva deve negar colisão sem fallback, adoção, rename ou limpeza. A consulta não autoriza criar esse componente.

## Consumo e preservação

Autorização específica do pedido df0e9ee7... UTILIZADA, sem retries; exceção de política limitada ao processo filho também utilizada para esta submissão relatada. Não existe novo crédito de execução porque first=0/effects=0. Os registros originais/pins com estados anteriores permanecem históricos e imutáveis; RESULT-CONSUMPTION.json registra o resultado posterior. Nenhuma garantia nova de prevenção técnica de reexecução entre invocações é alegada.

## Próximo trabalho técnico concreto

Há evidência suficiente para a revisão offline focal da guarda, sem repetir esta consulta para descobrir a mesma ACE. A correção deve distinguir raiz-âncora, próximo filho existente protegido, parent de criação e objetos novos. Não retirar 0x4 globalmente da máscara e não ampliar whitelist de trustees. A guarda atual permanece inalterada e staging DENY até patch fundamentado/qualificado e novo pedido operacional.

Hipótese mínima a qualificar: permitir na âncora apenas o direito de criar outro subdiretório fora da cadeia, condicionado à custódia original/revalidação do filho existente, DACL protegida desse filho e ausência de direitos perigosos que permitam substituição. Qualquer DELETE_CHILD na âncora, DELETE/WRITE_DAC/WRITE_OWNER ou escrita não confiável no filho, direito genérico não resolvido, tipo não compreendido, reparse/substituição ou incerteza continua DENY. O acesso de administradores/OS confiáveis segue o limite F1, sem prometer resistência a comprometimento administrativo.

Fundamentos existentes: F3 requer parent original, proteção contínua e criação exclusiva; D5 mantém proteção de ancestrais/ausência de substituição; B3.1/B4 não dispensa essas garantias. Não há cláusula aqui aceita para uma dispensa global ou guarda positiva por hash de fixture. A revisão deve comprovar aplicação no capture e nas revalidações, não somente contornar a primeira localização 5.

Sem código funcional alterado nesta reconciliação, não se repetiram builds/testes. Não houve nova consulta, retry, staging ou modificação ACL/SCM. E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4/UNKNOWN/históricos/reservas preservados. Próximo marco é a correção offline qualificada da guarda por papel, seguida de novo pedido exato de staging; não outra campanha genérica de coleta.
