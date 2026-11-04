# Consulta focal proposta — somente metadados da raiz

Estado: SPECIFICATION / NOT_EXECUTED / NOT_AUTHORIZED. Não é pedido pronto de execução nem permite reutilizar autorização histórica.

Finalidade única: observar a condição atual que o classificador estrutural rejeita na raiz da travessia. Não repetir preflight completo ou staging. Não coletar conteúdo de arquivos, credenciais, SACL ou segredos.

## Aquisição mínima

Resolver ProgramFiles pelo mecanismo de known folder existente e derivar sua raiz, sem tratar caminho como autoridade. O caminho esperado de planejamento é `C:\`; divergência deve ser reportada, não substituída silenciosamente. Adquirir somente essa raiz no adapter/owner existente, com READ_CONTROL e FILE_READ_ATTRIBUTES; SYNCHRONIZE apenas se tecnicamente exigido pelo modo nativo escolhido. Não pedir FILE_ADD_SUBDIRECTORY, escrita, DELETE, WRITE_DAC, WRITE_OWNER ou listagem recursiva. Não ajustar privilégios.

O `personal_native_open` usado pelo staging solicita também direitos de criação: não o reutilizar integralmente como se fosse esta aquisição mínima. Qualquer ajuste deve ser tipado e restrito a metadados, não um setter de direitos/autoridade genérico.

Reter o handle original, registrar volume/file ID, atributos de diretório e ausência de reparse, revalidando o mesmo objeto antes do fechamento. A consulta TokenUser, se necessária ao classificador, é TOKEN_QUERY no processo controlador; não exportar token nem inferir autoridade operacional de elevação/SID.

## Saída permitida

- Identidade original do objeto e contexto observados, separados de identidade histórica.
- Owner SID; DACL presente/nula/válida; flags de controle e proteção.
- ACEs em ordem original: tipo, flags, máscara e trustee SID, sem resolução de nomes de rede.
- Motivo, índice e máscara produzidos pelo mesmo classificador atual; não implementar um AccessCheck substituto.
- Comprimento e SHA256 dos bytes exatos OWNER+DACL, sem normalizar/reordenar; método de aquisição/serialização declarado.
- Revalidação de identidade/descritor no mesmo handle, resultado de fechamento e retenção UNKNOWN quando pertinente.

Não incluir SACL, blobs condicionais desconhecidos, conteúdo bruto de arquivos ou segredos. Tipos desconhecidos devem produzir diagnóstico/STOP, não interpretação permissiva. A alocação interna de GetSecurityInfo antes da validação de comprimento continua custo opaco; não afirmar limite físico provado por um teste de comprimento posterior.

## Gates e encerramento

Uma aquisição e revalidação delimitadas, sem retries automáticos. STOP em acesso negado, erro de consulta, reparse, identidade/bytes alterados, tipo não suportado, falha de recursos ou fechamento incerto. Fechamento conhecido somente com evidência; backings/retidos preservados em UNKNOWN. Não confundir referências emprestadas com handles próprios.

Sem RNG, confirmação de staging, criação/escrita/cópia/flush, ACL, SCM, endpoint, CNG, serviço ou mudança de execution policy. Saída no console existente; persistência de resultado somente pelo mecanismo autorizado que vier a ser definido. Não importar envelope LAB/TPM/SCQ nem afirmar que B3 autoriza esta coleta.

## Antes de entregar comando

Auditar o caminho mínimo existente; implementar apenas se necessário e dentro dos contratos; qualificar offline com APIs mutantes proibidas; congelar os artefatos/hashes do procedimento e preparar pedido byte-exato de uma consulta manual. A execução exige aprovação específica do proprietário. Não emitir placeholders, comandar tentativa ou declarar a consulta já autorizada.

Uma captura atual, mesmo com hash igual ao arquivo histórico, não prova retroativamente qual descritor foi lido na tentativa consumida.
