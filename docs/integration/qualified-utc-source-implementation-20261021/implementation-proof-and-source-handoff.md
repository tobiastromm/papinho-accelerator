# UTC Q0â€“Q7 â€” nÃºcleo DATA e gates de fonte

O contrato normativo Ã© DECISION-qualified-utc-q0-q7-corrected-20261021 e seu anexo congelado. Nenhum ADR accepted foi modificado. A tarefa autorizou implementaÃ§Ã£o independente e preservaÃ§Ã£o NOT_QUALIFIED quando nÃ£o existir fonte suficiente. Patches funcionais exatos e correÃ§Ãµes foram submetidos ao executor antes da aplicaÃ§Ã£o; preimagens e pÃ³s-hashes conferidos. Nenhuma aprovaÃ§Ã£o de patch concede autoridade temporal.

## ImplementaÃ§Ã£o e fronteira

src/issuer/qualified_utc.h/.c integra a biblioteca papacc_issuer existente. NÃ£o cria owner, worker, callback/provider, relÃ³gio, serviÃ§o, armazenamento, registry ou journal. A API Ã© privada, nÃ£o serializada, DATA; DATA_OK significa somente cÃ¡lculo vÃ¡lido. NÃ£o Ã© lease E0, aprovaÃ§Ã£o, status factual nem observaÃ§Ã£o qualificada. O produtor UPDATE e seus guards nÃ£o foram alterados. A assinatura escalar backend.utc e os codecs permanecem intactos.

Contexto preserva referÃªncias opacas owner/backend/source/pin, identidades root/candidata, lifetime do processo, invocaÃ§Ã£o, revisÃ£o, qualificaÃ§Ã£o, continuidade e fronteira. SÃ£o correlaÃ§Ã£o DATA, nÃ£o prova. Nenhum ponteiro Ã© dereferenciado para inferir autoridade. VersÃ£o, presenÃ§a, resultado de leitura e qualidade sÃ£o separados; zero Ã© valor vÃ¡lido. InstÃ¢ncia factual, autenticidade, origem e aplicabilidade continuam obrigaÃ§Ãµes do adaptador administrativo, nÃ£o flags fornecidas pelo caller.

A observaÃ§Ã£o preserva UTC nativa em ns, m0/m1, [L,U], rho_num/rho_den, E e D exclusivo. A propagaÃ§Ã£o implementa exatamente Q3: floor inferior, ceil superior, max(0,a-E) sem underflow, somas/produtos/conversÃµes checados. IntermediÃ¡rio nÃ£o representÃ¡vel tambÃ©m nega, mesmo quando uma Ã¡lgebra alternativa poderia representar o resultado; nenhuma saturaÃ§Ã£o favorÃ¡vel. PresenÃ§a incompleta, resultados nÃ£o OK, versÃ£o invÃ¡lida, rho invÃ¡lido, prazo expirado, origem/contexto divergentes e overflow nÃ£o produzem outputs parciais. A conversÃ£o FILETIME Ã© aritmÃ©tica pura, nÃ£o leitura OS ou qualificaÃ§Ã£o Windows. Validade exige inclusÃ£o total em [notBefore,notAfter), sem tolerÃ¢ncia.

HistÃ³rico guarda Ãºltima observaÃ§Ã£o e Ãºltimo corte monotÃ´nico consultado. Detecta regressÃ£o UTC nativa, regressÃ£o monotÃ´nica entre queries, sobreposiÃ§Ã£o inconsistente e mudanÃ§a de contexto/limites. Igualdade nativa nÃ£o Ã© regressÃ£o. Consultas nÃ£o renovam D. Falha invalida aplicabilidade e conserva bytes; sem retry atÃ© uma hora favorÃ¡vel. MudanÃ§a de fonte/continuidade/suspensÃ£o exige invalidaÃ§Ã£o explÃ­cita pelo owner; este nÃºcleo nÃ£o detecta eventos OS sozinho. Restart/new zero-state nÃ£o restaura histÃ³rico, CURRENT ou aprovaÃ§Ã£o. Os testes zeram estados somente para casos DATA independentes.

## PAL, fonte e integraÃ§Ã£o negada

src/platform/pal_time.h promete duraÃ§Ã£o monotÃ´nica em ns, explicitamente sem resoluÃ§Ã£o ns garantida. Win32 QPC/frequency/conversÃ£o nÃ£o provam limites E/rho, saÃºde, autenticidade, continuidade em suspensÃ£o, restart ou qualidade UTC. NÃ£o hÃ¡ porta PAL de erro/resoluÃ§Ã£o/continuidade qualificados que permita adaptador positivo. NÃ£o adicionar essas garantias por inferÃªncia. A implementaÃ§Ã£o PAL existente permanece byte-exata; nenhum dado OS foi promovido.

PAPACC_ISSUER_BACKEND.utc continua um escalar, sem Q1/Q2. O backend CNG delega Ã  administraÃ§Ã£o existente. GetSystemTimeAsFileTime em Site TLS nÃ£o foi reaproveitado. papacc_issuer_qualified_utc_read Ã© a fronteira negativa preparatÃ³ria: retorna NOT_SUPPORTED sempre, sem chamar scalar, PAL, OS, rede ou alterar output. Mesmo backend sintÃ©tico nÃ£o Ã© fonte positiva. NÃ£o hÃ¡ produtor positivo, registro da fonte, novo protocolo de tempo ou qualificaÃ§Ã£o operacional.

Issuer status e UPDATE U4 nÃ£o consomem resultados DATA como autorizaÃ§Ã£o. MantÃªm SOURCE_ABSENT/DENY, CURRENT factual ausente e demais guards. A nova funÃ§Ã£o nÃ£o estÃ¡ conectada a um caminho positivo do cohort. Sem fonte qualificada, nÃ£o Ã© possÃ­vel implementar consumo positivo seguro; isso Ã© implementaÃ§Ã£o/qualificaÃ§Ã£o de origem pendente, alÃ©m do ambiente CURRENT conhecido e capacidade de ativaÃ§Ã£o, nÃ£o apenas problema de sandbox.

## Lifetime e retenÃ§Ã£o

Todos os structs sÃ£o caller-owned no owner administrativo existente; nenhum foi instanciado no grafo runtime. Chamadas requerem serializaÃ§Ã£o pelo owner e outputs disjuntos. Um histÃ³rico pertence a uma Ãºnica operaÃ§Ã£o/fronteira. O caller deve proteger todas as referÃªncias externas por seus owners e os pins canÃ´nicos atÃ© quiescÃªncia factual; o pin root nÃ£o protege fonte externa por inferÃªncia. NÃ£o hÃ¡ aquisiÃ§Ã£o ou release de borrow/handle/lease em nenhuma funÃ§Ã£o.

Stop sÃ³ marca stopped/invalid, sem I/O/espera/cleanup; preserva histÃ³rico/contexto. Invalidar UTC nÃ£o libera handles, crÃ©dito, backings ou retidos. O nÃºcleo nÃ£o fornece close/reset/abort destrutivo e nÃ£o altera Model E, E0 acquire/release ou exactly-once. RELEASE_INVALID e retenÃ§Ã£o do cohort anterior seguem intactos. CÃ³digo DATA sÃ³ serÃ¡ embutido no owner apÃ³s origem/proteÃ§Ã£o/reserva reais e revisÃ£o de lifecycle.

## Handoff mÃ­nimo

1. Qualificar a fonte realmente administrada na instalaÃ§Ã£o e a instÃ¢ncia PAL: origem/autenticidade/erro UTC, rho na direÃ§Ã£o UTC/PAL, E aditivo incluindo d=0, D, saÃºde, suspensÃ£o/retomada, rollback e restart. NÃ£o selecionar serviÃ§o externo implicitamente. Se a instalaÃ§Ã£o nÃ£o expuser esses fatos, manter NOT_QUALIFIED e propor a alternativa governada necessÃ¡ria.
2. Adquirir observaÃ§Ã£o no backend administrativo issuer antes de E0; ligar ao mesmo owner/invocaÃ§Ã£o/CURRENT/pin/candidata e proteger fontes externas. NÃ£o expor um callback booleano ou API pÃºblica de autoqualificaÃ§Ã£o.
3. Reservar coexistÃªncia real, incluindo ambas observaÃ§Ãµes/histÃ³rico, DER/status, snapshots/pins, buffers, stack e retidos. O cenÃ¡rio completo atual jÃ¡ excede 3 MiB; parar integraÃ§Ã£o dependente atÃ© otimizaÃ§Ã£o/decisÃ£o prÃ³pria. NÃ£o criar quotas.
4. Validar issuer/certificado/delegaÃ§Ã£o/policy/histÃ³rico sob CURRENT factual e revalidar tempo/status/ato imediatamente antes do efeito. CURRENT ausente e autorizaÃ§Ã£o administrativa bloqueada nÃ£o sÃ£o supridos por fixture. U0â€“U7, predecessor, CAS/UNKNOWN e fencing continuam independentes.
5. Integrar consumidor issuer/UPDATE por hunks exatos aprovados e qualificar nativamente em ambiente autorizado. Browser, PST, wire e confianÃ§a M1 continuam fora do escopo; nÃ£o criar lease ou confirmar importaÃ§Ã£o local.

## ValidaÃ§Ã£o e limites

MSVC x64 Selected/Legacy Debug/Release /W4 /WX: 11+11+10+10=42 suÃ­tes CTest. VC6 x86 /Za /W4 /WX Debug/Release: duas execuÃ§Ãµes diretas. 11583 verificaÃ§Ãµes UTC por configuraÃ§Ã£o, seis configuraÃ§Ãµes; sÃ£o casos matemÃ¡ticos/negativos, nÃ£o provas factuais. Intervalos/rho/E, zero, arredondamento, intermediÃ¡rios overflow, presenÃ§a/resultados, FILETIME, validade exclusiva, expiraÃ§Ã£o, fonte/contexto/revisÃ£o/pin/boundary divergentes, rollback UTC/monotÃ´nico, inconsistÃªncia, Stop, bytes retidos e restart cobertos. A fonte negativa nÃ£o chama o scalar do backend. SuÃ­tes anteriores issuer/checkpoint/UPDATE/ED11 preservadas; SHA e executÃ¡veis locais nÃ£o qualificam UTC. NT4, box e ISA nÃ£o qualificados; compilaÃ§Ã£o VC6 nÃ£o Ã© execuÃ§Ã£o em NT4.

Falhas de desenvolvimento preservadas: VC6 C1004 no issuer_core.h preexistente sem newline; corrigido somente acrescentando um LF aprovado. Macro teste do/while(0) gerou C4127 VC6; substituÃ­do por funÃ§Ã£o sem suprimir warning. EdiÃ§Ã£o de resultados criou CR duplicado e C4335 MSVC; trÃªs arquivos novos normalizados CRLF por operaÃ§Ã£o aprovada. Logs finais sÃ£o livres de warnings; anteriores nÃ£o sÃ£o apresentados como PASS. Nenhuma fixture criptogrÃ¡fica, chave, certificado ou rede foi gerada.
