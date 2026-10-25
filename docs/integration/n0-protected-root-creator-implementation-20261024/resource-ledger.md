# Recursos e limites da prova

Estado nativo privado: 616 bytes; descriptor imutável: 104 bytes; cinco slots de diretório, três de referências auxiliares, três de token; cópia de descriptor aceita somente até 65.536 bytes. SID do operador e buffers de APIs possuem lifetime explícito. Mutex, intent e owner operacional existentes não são substituídos ou consumidos pelo criador.

Referências originais dos ancestrais permanecem abertas durante a cadeia. Aquisição de cada componente é fechada após duplicar a referência ao mesmo objeto; todas as referências diretas são fechadas ao final confirmado. Falha de fechamento mantém UNKNOWN e processo vivo, sem retry. Diretórios criados são persistentes e nunca removidos automaticamente; fechar handles não comprova remoção de efeitos parciais.

O launcher mantém streams de fonte/helper/políticas/executável, referências dos ancestrais do executável, processo, pipe de saída e recibo administrativo. Usa um PowerShell já elevado, sem novo PowerShell. Compilação do helper existente pode usar ferramentas inbox transitórias; esta topologia de preparação não é prova de B4 operacional. Console, CRT, stack transitiva, memória gerenciada, kernel/provider, cache e metadados persistentes não possuem teto integral demonstrado aqui. UNKNOWN não equivale a zero.

Sem aumento de quotas ou créditos. HOST_CREDITS=NONE_GRANTED; ED11_TOTAL_STATUS=PARTIAL. Os inputs de 5.358/326 bytes não foram colocados. Pressupõe Windows e administradores confiáveis; não protege contra administrador equivalente ou SO comprometido. SACL, auditoria e mandatory labels não consultados permanecem UNKNOWN.
