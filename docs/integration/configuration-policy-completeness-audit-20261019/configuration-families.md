# Matriz de familias de configuracao

Status separados: ACCEPTED e fonte normativa; IMPLEMENTED e fonte de codigo; HISTORICAL e escopo anterior; PROPOSED nao executavel; MISSING e mecanismo nao localizado. 167 requisitos individuais constam em requirements-matrix.json/md.

| Familia | Modelo/autoridade | Implementacao factual | Limite / requisito para source completo |
|---|---|---|---|
| Identidade/revisao do documento | PE4,A44,IR | root domain,R,I,A,P distintos; CAS/snapshots | source nao confirma commit; versoes e refs imutaveis; sem contador paralelo |
| Listeners | A09,A43 | um port/profile/bind selection; varios sockets enderecos | colecao logica prevista nao implementada; default Legacy a reconciliar |
| Interfaces de entrada | A07 | PersistentID com presence, snapshot local, index/scope separado, ALL/SELECTED, IPv4/v6/multiplos enderecos | sem source-context rules por arriving-interface; nome/index nao duraveis; ausencia nega sem fallback |
| Interfaces/rota de saida | A19,A26,FUT | modelo profile/interface/source/default_route; source atual default-route | nao inferir do bind; sem prova de rotas por ID, failover ou hot migration |
| Transport Security | A09,A10,ADR0006 | Secure TLS1.3/mTLS/ALPN papacc/1, Legacy explicitamente separado | resolver/leases/trust/identity antes de RUN; nenhum auto/downgrade |
| Admission Mode geral | A28,A43,A48 | OPEN/CLOSED metadata por listener/contexto | nao e usuario/senha implementado; CLOSED geral pode usar credencial contratada; OPEN nao concede capacidades |
| Authentication e origem | A10,A28 | peer evidence e resolver16/provider; lab um leaf | IP/nome nao autentica; subnet/range/local/remote/arrival-time predicates faltam |
| Capability/AuthZ | A05/A12,A43 | registry keys, intersecao cinco inputs, snapshot, first-terminal bounded | unsupported != enabled != authorized; atualizacao sem silent add; provar todos paths |
| Destination Policy | A19-21,A26 | exact hostname+port e endpoint v4/v6+port, sensitive deny, resolver uma vez | sem CIDR/suffix/ranges/deny rules/PUBLIC_INTERNET; nao usar v1 como linguagem completa |
| Egress Profile | A19,A26 | referencias server-owned, default route, available validation | sem client escolha privilegiada nem fallback silencioso |
| Outbound TLS ModeB | A27 | reference/resolver, SYSTEM/custom explicitamente separado do canal | custom env adapter e source de laboratorio; nao trust herdado |
| Site TLS ModeC | SITE,C2 | um catalogo embedded BODY0.5, SYSTEM, hostname/SNI/TLS13, vetorALPN, UTC validade, bytes/duracao/concorrencia | catalogo nao cria grant, P/revisao/Principal CURRENT; CUSTOM nao suportado nao vira SYSTEM |
| Principal/dispositivo/clientes | PROV,A51,IR | Principal16 exato; issuer identity/CAgen/device record distintos | mapping produtivo e renovacao estavel nao lab truncation; sem user/group registry |
| Usuarios/grupos/RBAC | A28/FUT | MISSING completo | nesting/heranca/conflitos nao aprovados; nao inventar defaults de permissoes |
| Datas/horarios | A28/FUT; SITE/VALID/CAT para tempo especifico | agenda MISSING; monotonic e validadeUTC em dominios distintos | sem calendario/feriados/timezone/DST; invalidez nega efeitos dependentes |
| Regras/condicoes/acoes | A43/A48 | profile/port/admission/key+Principal; terminal ACCEPT/DENY | conjuncao implicita, primeira terminal, default deny; sem AST booleana geral |
| Provisionamento | PROV/CA/CAT | Browser import/store geracoes/leases; issuer core/persist/live parcial | MANUAL/AUTOMATIC obrigatorios; gerar/receber pacote != conceder grants; automatic private key local |
| Custodia/trust/rotacao | CA/CAT,A10,ModelE | contratos e adapters parciais; nenhum CA real autorizada | chaveCA separadaTLS/signer, manualcustody scoped, secrets fora config/log; backup/fence gate |
| Recursos/quotas | C2 ED11,VALID | capacidades/reservas/creditos locais, retencao, observadores | managed2MiB; provider/kernel UNKNOWN; margens/freshness nao inferidas; contas separadas Browser/Accel |
| Lifecycle/Stop | ModelE,ED08 | owners duraveis, quiescencia, pins, factual close, pending TxF worker | FINAL remoto/timeout/process exit nao DESTROYED; credito retido nao devolvido |
| Persistencia/publicacao | A44,A48,IR | BODY05/ROOT06/link1, participants/commit/CURRENT/A/P e 3slots | issuer-only live != arbitrario hot reload; recovery denies sem proof |
| Administracao/auditoria | PE4,PROV,CAT | CLI startup/help/list interfaces/log level; matched-rule index local | sem CLI admin completa/roles/backend produtivo ou effective-rule audit trail uniforme |
| Plataformas | A02,PE4/PE7 | core portable e adapters Win32; box/ISA nao qualificados | indisponibilidade explicita, nao CNG/SID/DACL universal nem mentira de capability |
