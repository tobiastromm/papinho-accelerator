# Plano quantitativo SCQ — fundamento disponível e fronteira

Não é política operacional aprovada nem pedido de execução. Ver JSON homônimo.

| Elemento | Fundamento verificável | Estado restante |
|---|---|---|
| Propósito/key | TO0–TO9; purpose2,112 bytes/WCHAR zero, READ preservado | Qualificado offline |
| Bytes | record1048; SQR11052; SQC11076; buffer1076; retorno reutiliza SQC1 | Wire4252; soma de leituras/escritas8504, sem duplicar reserva |
| Progresso | scqt_io_result exige progresso positivo e limitado; no máximo uma submissão/poll por chamada | Até4252 conclusões com bytes positivos por ator; polls pendentes não têm teto demonstrado |
| Validade | scqp_window; mesma invocação/operador/plano protegido, eventos invalidantes e consumo monotônico | valid_from/until precisam de origem e janela justificada, sem default |
| Freshness | Revalidação de originais/token/SCM/processo antes das transições; coleta não é instantânea | Idade máxima e duração de coleta não demonstradas para SCQ |
| Duração global | Expiração nega novos efeitos e pede Stop; não representa conclusão nativa | Não há limite global/encerramento demonstrado. Retenção não recebe refund por deadline |
| Handles | Accepted124/128/1024; refs no owner e proteção de lifetime | Contagem nativa de todos os participantes aplicáveis, alias/shared/retained ainda necessária |
| Managed | Censo compilado Selected1979784 e Legacy1811216; transporte728,driver1888,buffer1076 | Não é físico/commit, stack transitivo ou envelope Windows |
| Headroom | Disponível observado deve cobrir reserva ativa/transitória/retida/compartilhada e margem justificada, físico e commit separados | Termos opacos e margem ainda UNKNOWN; não preencher com números LAB/Browser |
| Close | SQC1 comparado ao DuplicateHandle original, retornado no mesmo peer e confrontado com IDs efetivamente fechados | Evidência nativa das duas pontas ainda não obtida |

Não medir duração de fixtures como teto nativo. O1024 do loop de teste é somente
um limite do harness e não uma política ou orçamento de produção. Custos de RNG/
hash públicos, token/SCM/pipe, stacks e kernel continuam mesmo sem KSP. Os campos
não fundamentados permanecem null; a especialização não contém exceção para eles.

Primeiro insumo factual faltante: caracterização ou garantia aplicável do envelope
nativo SCQ e dos relógios/coleta, independente de uma execução protegida presumida.
Uma futura coleta protegida exige escopo próprio; esta tarefa não a executa nem
a aprova. Não há fundamento para converter testes offline ou baixa ocupação do
host em aceitação quantitativa. Enquanto faltarem os termos: DENY, sem pedido
operacional numérico fabricado. Não foi emitido comando com placeholders.
