# .conf e retenção — especificação PROPOSED

Sintaxe atual: UTF8, format_version1 primeiro, diretivas/linha e tipos/id com end; nãoINI. Parser src/config/config_source.c/.h: fonte8192/linhas96/tokens8/texto64/blocos16; implementa binding/listener/channel_tls/access_auth/capability_policy e quatro globals negativos. Conhecido sem implementação falha. Não adotar [Pairing]/RevokedHistoryRetentionDays.

Extensão GLOBAL única proposta:

```text
format_version 1
revoked_history_retention_days 0
```

É fragmento, NÃO configuração operacional completa; parser atual rejeita a nova diretiva como unknown. Depois de aceito/implementado: ausente ou0=nunca ocultar automaticamente; positivoN=ocultar view de revogados após N dias de86400000ms desde revogação confirmada. Faixa0..4294967295 unsigned32, conversão checked; rejeitar sinal,+/-,decimal,expoente,string,overflow/argumentoextra/duplicata/aninhamento. Zeros à esquerda seguem number existente. Ordem livre no global após version, comentários/UTF8/quotes seguem norma. Sem clamp/default em erro. Target que não implementa rejeita candidato inteiro, sem aumentar buffers/limites.

Default0 não reinterpreta bytes históricos. Aplicação/projeção/comparação completa/hashing/CAS e campo persistido exigem layout/versionamento revisados; sem padding ou novo root/store. Formato antigo pode projetar0 só mediante migração/regra explícita validada, não reinterpretando binário. Nenhum codec novo é aceito por esta proposta.

Settings-only restore: parse completo -> allowlist por campo suportado -> validar target/referências locais NOVAS -> plano/confirm NEW_INSTALL -> prepare/validate/commit canônico. Copiar arquivo é backup manual de opções, não apply autorizado. Não restaurar certificados/CA/trust refs, rootidentity/CURRENT/serial/high-water/status/revogações/grants/pins/autorizações/credenciais. Referências públicas antigas também não são confiança transferível. Campos desconhecidos/autoridade não são silenciosamente descartados. Revalidar portas/binds/interfaces no novo equipamento; identidade antiga de interface não prova alvo atual. Senhas/contaSCM não estão no .conf. Sem include/env/comandos/parser paralelo.

Viewativa=atuais não revogados; histórico=revogados não ocultos. Campo sem fonte factual mostra indisponível. Excluir do histórico manual só após REVOKED publicado, ator/scope/confirmação e decisão durável canônica; não chama delete de credencial/record nem libera backingpinado. Tombstone/revogação/serial/DER/história necessários continuam, mesmo sem campos na UI. Layout/representação de ocultação exige revisão específica antes de código, não segundo banco.

Autocleanup: revogação factual no intervalo[rmin,rmax], now qualificado[nmin,nmax], UTCms. Checked delta=N*86400000, expiry=rmax+delta; ocultar só se nmin>=expiry e policyCURRENT/status/pin/autoridade válidos, sem Stop/descontinuidade/rollback. N0 não agenda. Timestamp ausente, UTC não qualificado/UNKNOWN ou overflow mantém visível. Nunca usar uptime/mtime/UTCescalar/Browser como prova de dias. Ocultar não altera revogação ou cria crédito; manual independe de idade mas não de autorização/revogação.

Turnos bounded do owner/scheduler existente, sem novo worker/autoridade. Revalidar antes da transação; CASconflict/UNKNOWN não repete efeito automaticamente. Coleção cheia nega novos registros, não purga tombstone nem aumenta quota. Nenhuma chave/timer/ação implementada aqui.

