# READ/MACHINE fechado — extensão privada e lifetimes

Revisão técnica K3–K6 accepted, exclusivamente PERSONAL_LOCAL MACHINE_READ tipado. Não é novo contrato de autoridade, protocolo público, registro persistido ou canal. Nenhuma operação real nesta qualificação.

## Seleção e proveniência

Entrada específica `--owner-manual-machine-read-scq-start`, exatamente 17 argv. Os 15 argumentos anteriores continuam DATA de paths/pins/limites; os dois adicionais são o nome canônico da fixture TEST_ONLY e SHA256 público. Não carregam token, HANDLE, aprovação ou nonce. A seleção console explícita é `P1 MACHINE_READ AMD64` mais o tuple original; TRANSPORT_ONLY não satisfaz esse parser e não existe fallback.

O token administrativo original e a seleção nativa precedem a aquisição dos documentos. Os dois originais novos exigem o SD K6 exato, proteção/ancestrais/sem reparse, hash e identidade retidos. O alvo PXR1 tem 572 bytes de preimage: os 428 bytes originais com tag READ, nome canônico112 e publicSHA32. TRANSPORT_ONLY conserva PXT1/428. Hash do alvo liga instalação/imagem/chave pública ao plano original; nunca atribui autoridade à DATA. A política G3KR/84 incorpora a decisão K0–K9; classificação inicial pelo carrier é somente candidata e depende da validação nativa posterior.

## Layouts privados bounded, little endian

| Mensagem | Bytes | Conteúdo e condição |
|---|---:|---|
| Carrier PS_SCQ | 308 | Formato anterior inalterado; candidato, nunca aprovação. |
| Registro SCQ | 1048 | Formato anterior inalterado; READ1/key exige factory e origem tipadas. |
| SQK1 | 1132 | Magic4, registro1048, close-values24, deadline8, QUERY-values16, publicSHA32. Offsets do SQB3 preservados; publicSHA em1100. Recepção exige política/variante READ original, não apenas magic. |
| SKA1 | 100 | Magic4, attempt32, target32, publicSHA32. Só após admissão original do receptor. Watermark de sequenciamento; não é bilateral, grant, receipt ou close. |
| SKP1 | 144 | Magic4, tamanho4, purpose4, versão4, attempt32, request32, decision32, publicSHA32. P original confirmado e consumido uma vez antes do controle administrativo; runtime só usa depois de seu próprio binding/admissão. |
| SKR1 | 128 | Resultado DATA privado já qualificado: versão/tamanho/classificação, statuses/fases/contadores/close, attempt e publicSHA; validação estrita pelo codec existente. Nunca autoridade, SQR ou SQC. |
| SQR1 / SQC3 / retorno | 1052 / 1092 / 1092 | Receipt e fechamento recíproco originais, não reinterpretados como resultado P. |

SKR1 offsets: magic0, bytes4, purpose8, versão12, classificação16, provider_calls20, open_calls24, provider_status28, open_status32, first_stage36, first_origin40, first_error44, close_error48, phase52, close_attempted56, close_calls60, attempt64 e publicSHA96. Decoder rejeita tamanho/versão/escopo/tuple/statuses ou contadores incoerentes; PERMISSION exige NTE_PERM original, fase4/origemNCrypt, provider e OpenKey únicos e close conhecido.

SQB1/SQB2/SQB3 e os perfis anteriores preservam seus layouts e tamanhos. SQF1 continua somente negativo. Nenhum formato ROOT/CURRENT/API público é alterado.

## Ordem e ownership

1. Mesmo owner captura originais READ, plano, Q/E/S atribuídos independentes e P separado confirmado (ainda não consumido). Endpoint/Start continuam seus atos próprios.
2. Entrega QUERY usa as três referências read-only e duas referências QUERY originais existentes. O receptor valida seu próprio token/imagem, ambos os peers, custódia/cadeia/pins, seleção e deadline; somente então admite o empréstimo restrito. Nenhuma chave ou token impersonável é entregue.
3. Receptor envia SKA1 no mesmo pipe. Controller valida SKA1 contra seu plano original e mantém verificações nativas independentes. Só então consome P e reabre a mesma fixture pelo owner CA existente, sem criação, retendo sua referência e fazendo controle positivo público fresco/reabertura/SD/provider/algoritmo/exportpolicy.
4. Controller exige SHA público igual ao pin original, envia SKP1. Runtime valida P, consome o probe uma vez, revalida token efetivo não impersonado/recursos e chama no máximo provider+OpenKey MACHINE|SILENT. Não cria, assina, exporta privado ou promove readiness.
5. Runtime fecha seus objetos CNG uma vez e envia SKR1. NTE_PERM/handleNULL só qualifica com controle administrativo positivo; BAD_KEYSET, provider falho, resultado local, handle inesperado ou close UNKNOWN não qualificam isolamento.
6. Receipt/SQC3/retorno fecham a entrega original; referências de custódia/readers/cadeia continuam até fechamento canônico. Somente resultado factual elegível mais evidências originais permitem RUNNING diagnóstico. Lifecycle finito do serviço termina com exit/SCM STOPPED em modelo; produção sempre DENY.

Uma única buffer de sessão1132 (+32), um único OVERLAPPED e os mesmos pipe/event/process-refs. Nenhum handle novo de transporte. Referências adicionais CNG pertencem exclusivamente ao owner CA/probe existente; contagem de handles kernel é integral e não equiparada a quantidade de objetos abstratos CNG. Ancestrais, SD/backing/hash/provider/controle positivo não são fechados antecipadamente para obter PASS. Cleanup incerto retém UNKNOWN, não tenta novamente nem deleta fixture.

## Recursos e falhas

K7 apenas READ/MACHINE: entrada128, guarda188, cap192, privado1994944, working3145728, físico7239172096, commit3221225472, freshness100ms, janela240000ms/global60000ms sem renovação. O controller atualiza o MESMO observador/baseline antes de cada API CA protegida. O probe runtime exige a amostra original ainda fresca e contagem imediata antes de provider/OpenKey; latência superior à freshness pode negar a próxima chamada. Falha de contagem inclusive erro indisponível nunca vira zero/PASS. Não há rebase, exclusão de handles ou tolerância implícita.

As mensagens reutilizam polling/eventos e deadlines existentes; payloads parciais, magic/tuple/publicSHA divergentes, replay, Stop, peer loss ou I/O inconclusiva negam, preservando backing/primeira causa/consumos. SKA1 não renova janela e não substitui autenticação bilateral. O conjunto é testado offline com APIs mutantes; não demonstra suficiência Windows, runtime futuro, mapped-image attestation ou quiescência OS.

Fixture preparation permanece operação separada, sem herdar K7. Ver MACHINE-FIXTURE-RESOURCE-ANNEX.OFFLINE.md; Selected/Legacy têm censos próprios e Legacy continua DENY na preparação se exceder 2MiB. Sem autorização operacional de criação/READ/UPDATE neste documento.
