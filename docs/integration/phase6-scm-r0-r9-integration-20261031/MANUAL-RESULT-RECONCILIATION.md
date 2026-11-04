# Resultado manual — negação antes da apresentação do plano

Origem: saída textual colada pelo proprietário, preservada em OWNER-STOPPED-SCM-RESULT.txt. Não é captura independente, assinatura ou atestação da imagem mapeada. Pedido7eb9e691…/script55102d03… e aprovação2062b2dd… permanecem históricos e imutáveis. Esta reconciliação NÃO executou o script, consulta SCM, coleta, Start/Stop ou outro efeito protegido.

## Conclusão e uso da autorização

A única execução manual foi utilizada: wrapper INVOKED=True / exit2. Não reutilizar a aprovação, inclusive por ausência de consumo nativo. A confirmação local e o registro SCM não foram alcançados no caminho qualificado da implementação. O estado atual do serviço continua UNKNOWN: não foi consultado e não se afirma ausência global a partir dessa falha.

O código em SERVICE=ERROR; WIN32=5 é a resposta genérica da CLI quando o presenter retorna falha. O label DENY_OR_CANCELLED também é genérico: não demonstra cancelamento humano. A mensagem não prova uma API Windows com GetLastError5 nem ACL incorreta. Nenhuma alteração de permissões é justificada.

## Intervalo da primeira falha

Fases de recursos: ENTRY1 → ORIGINALS2 → BEFORE_CLOSE5 → CLOSED6. Não confundir com PHASE=5 do ato: esse campo significa FINISHED após fechamento, não a localização causal da falha.

No código qualificado do presenter, ORIGINALS2 só é amostrada depois da aquisição de parent/EXE/DLLs, verificações iniciais de identidade/proteção/hash e package_captured. Depois dela, antes da primeira saída do plano, existem:

1. Cópia dos bindings de path/comando/digest capturados.
2. BCryptGenRandom e rejeição de tentativa composta apenas de zeros.
3. `scm_register_current(v,1)`: revalidação de escopo/destinatário/originais, layouts/contexto/token, identidades, descritores e hashes de EXE/DLLs.
4. Primeira apresentação por WriteConsole; a sua falha também segue a mesma saída genérica.

O plano, nonce e confirmação não aparecem. Assim, o predicado exato permanece UNKNOWN dentro desse intervalo. Não afirmar RNG=NOT_CALLED ou RNG=PASS: não foi emitida evidência correspondente. A amostra ORIGINALS é um checkpoint anterior, não prova de que a revalidação posterior passou.

O consumo só existe posteriormente em take, depois da confirmação e dos gates de recursos. O manager SCM só é aberto na CLI depois de o presenter e o gate de admissão passarem. Logo, a inferência limitada ao código/pins qualificados é: consumo nativo0 e nenhum manager/Create/configuração/readback nesta tentativa. Esses contadores não foram impressos como fatos nativos independentes. Não há efeito SCM persistente demonstrado por esta saída.

## Recursos e fechamento

Quatro amostras reportadas: handles136/149/152/147, pico privado4.120.576 bytes, working set9.072.640, mínimo físico7.360.573.440 e commit9.921.441.792. São snapshots de fases do controlador; não baseline, pico universal ou prova de atribuição. O intervalo entre o primeiro QPC begin e último end é0,0647987 segundo na frequência reportada, não tempo total do processo ou bound de operação.

Não é o bloqueio antigo de handles124/128: R3 não aplica esse guard ao ato limitado, o erro aqui é5 e houve progressão além da primeira amostra. Não descontar os handles herdados nem tratar152 como teto. Privado/working set não são o orçamento gerido Q. As margens observadas acima de Q não aprovam capacidade: a fase PRE_CREATE4 e a confirmação nem foram alcançadas.

CLOSE=0 e RETAINED=0 reportam fechamento controlado conhecido sem retenção na saída recebida. A diferença entre a contagem inicial/final de handles não prova leak nem descarte indevido; não há atribuição de loader/CRT/provider por essas amostras. OS_QUIESCENCE permanece UNKNOWN.

## Próxima ação técnica precisa

Não há base para alterar guards, DACL, quotas, pacote staged ou política R0–R9. O defeito demonstrado é de observabilidade: múltiplas primeiras causas são condensadas no mesmo retorno genérico. A menor continuação offline é preservar a primeira condição/erro na captura pós-ORIGINALS e decompor a revalidação pré-plano com testes negativos, sem efeitos reais ou callbacks positivos. Ver NEXT-PREPLAN-FIRST-CAUSE-OFFLINE.md.

Nenhum novo pedido operacional/retry é emitido nesta reconciliação. Mesmo depois de diagnóstico offline, qualquer observação real adicional exigirá escopo e aprovação próprios, sem reutilizar o pedido consumido ou presumir que um diagnóstico autoriza CreateService. Start diagnóstico não está pronto: primeiro falta registro parado demonstrado.

Registro SCM: NOT_DEMONSTRATED / DENY para novas tentativas sem autoridade própria. E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4, staged c0f27f94…, históricos e UNKNOWN preservados. Não houve alteração de código ou nova campanha de testes nesta reconciliação.
