# Ledger focal — collector diagnóstico

- Estado estático IG_CONSOLE_PROOF:8816 bytes nas quatro variantes, medidos pelos testes. Inclui fatos, buffer SHA4096, dois buffers de caminho1024 WCHAR, membros32 DWORD, métricas e estruturas de confiança. Contar uma vez; não somar novamente seus buffers embutidos.
- Novos refs diretos: um handle de processo e um de imagem, simultâneos; uma referência opaca de estado WinTrust pode coexistir com esses dois. Estado Trust fecha antes de iniciar medição PSS.
- PSS snapshot/marker/thread usam o owner e storage existentes; não são um segundo observer. Até3 referências temporárias existentes podem coexistir com os dois handles do collector. Assim, a fase nova sobrepõe2+max(1,3) refs, não a soma de Trust e PSS como fases simultâneas. Refs canônicos existentes continuam contabilizados à parte. Sem novo processo/thread/worker controlado.
- O wrapper ig_observe_process passa a chamar o núcleo comum; Debug pode adicionar frame. Stack transitiva CRT/WinTrust/PSS/Windows e custos kernel/provider/cache continuam UNKNOWN, não0. Sem soma de reserva de VA como consumo físico.
- Limites de armazenamento/log são limites controlados de diagnóstico, não novas quotas: máximo32 IDs de console, path buffers1024 WCHAR, linha limitada768 unidades, hash em blocos4096 até16MiB de arquivo, uma captura por processo qualificador, zero retry. Erros/tamanho excedido não promovem autoridade.
- Medições do host/processo não são teto universal. O collector observa pico de commit/WS por processo quando a API fornece; threads/stack/handles são amostras. Simultaneidade real não é atomicidade. Private Bytes/commit/stack não devem ser somados como três consumos independentes.
- Captura e refs pertencem ao qualificador, não ao storage/runtime do serviço futuro. O candidato de serviço eda84c0c... é preservado. Não há modificação de quota Selected3145728/Legacy2097152 ou dos guards B4. Atual alocação do provedor ainda não foi medida pela operação real.
- Na falha WinTrust close ou PSS close, manter dependências e canonical owner retidos/query-only. Nenhum segundo close, refund, kill, timeout de cleanup ou promoção de ausência de processo a release. NONE_GRANTED.

ED11_TOTAL_STATUS=PARTIAL. Este ledger não admite a operação positiva e não preenche custos opacos com reserva arbitrária.
