# Handoff exato para codec e resolução

Codec permanece PROPOSED, não implementado. A proposta anterior LST1/header128/record512/SHA256 não foi aceita por esta tarefa. BODY0.5/ROOT06 inalterados.

Interface: entradas propostas valid+uint64 são compatíveis com ADR-0007 e com o token decimal já usado pelo CLI; não gravar Luid como contrato Windows universal: persistir o tipo portátil, com domínio machine-local/root, e disponibilidade de backend explícita. Zero com presença não equivale a ausência. As oito entradas propostas e sua integração ao envelope exigem revisão normativa própria. Não usar campos reservados como autoridade.

Authentication: authref textual sozinho não basta. Layout final deve preservar identidade/revisão/kind/presença do descritor governado, se/quando aceito; não inferir de securityref, Principal ou provider criptográfico. Não congelar offsets novos aqui. Digest prova integridade relativa, não CURRENT/freshness/fencing/autorização.

## Menor slice executável após aceitação

1. Reutilizar o parser decimal overflow-safe do CLI em helper portátil comum, preservando testes CLI, em patch exato por função; não invocar CLI sintética.
2. Entrada explícita de snapshot discovery imutável emprestado na bridge; chamar persistent_bind_selection_resolve e bind_targets_resolve. Não consultar Win32 dentro de cópia assumida pura. Preservar wrapper ALL compatível e coleção sem truncar. Rejeitar capacidade insuficiente antes de publicar saída.
3. Comparar lista tipada/ordem com modelo canônico e source CURRENT; não aceitar um token existente mas não aprovado na configuração. Revalidar root/Stop após cópia. Não adquirir nem retirar pins implicitamente.
4. Provider nomeado continua rejeitado até descritor/root/source aceitos e implementados. Nenhum stub ou callback de fixture produtivo.
5. Recalcular sizeof/stack/snapshot/targets antes de reservar a mudança. Snapshot caller-owned, referências e cópias não contadas duas vezes. Se limite insuficiente, negar.

Regressões novas necessárias: decimal vazio/sinal/overflow/zero/max/zeros iniciais; aliases numéricos duplicados; interface removida/substituída/ambígua; mesmo token com índice/endereço atual diferentes; target sem PersistentID; snapshot inválido; candidato e modelo divergentes; pin CURRENT real durante chamada, Stop/retenção; output capacity. Provider válido real somente após fonte governada, ausente/incompatível/stale sempre rejeitados. Testes positivos sintéticos não qualificam root ou autenticação nativa.

Sem config apply, CURRENT/A/P, serialização nova, rede ou alteração de créditos. Próxima decisão específica: aprovar I1–I3; definir/aceitar descritor A1–A4, sem aceitar automaticamente layout codec.
