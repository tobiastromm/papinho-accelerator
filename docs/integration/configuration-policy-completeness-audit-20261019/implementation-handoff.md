# Handoff minimo de implementacao canonica (nao novo roadmap)

Esta tarefa autoriza auditoria/documentacao somente. Nenhuma implementacao funcional, build, teste, rede, CA ou mudanca de admissao foi executada. Os tres gates de admissao permanecem negados; PST0.6.5 candidato ate fechamento integralPhase6, producao0.6.4 preservada.

1. Owner revisa authority-inventory,167requirements e achadosF01-F09; confirma o escopo exato da primeira source de configuracao. FIRST_TERMINAL_MATCH_WINS/defaultdeny e ortogonalidade ja accepted nao precisam nova escolha. Manter conflitos de modelo visiveis, sem aplicar patch nesta auditoria.
2. Preparar patch futuro por funcao para reconciliar colecao/perfil explicito/normalizacao com A09 e a fonte de autoridade atual, preservando comportamentos/pins/grants e producao isolada. Source deve convergir na mesma root, resolvers e transactions; nao criar engine/provider/catalogo paralelo. Novo arquivo nao edita storagecommitted. Operacoes perigosas exigem exactofficialreview conforme autorizacao da futura tarefa.
3. Somente depois especificar o formato privado/source profile com matriz campo->clausula->modelo->resolver->enforcement->durabilidade->lifetime->audit. Requisitos fora do perfil sao REJECTED/UNSUPPORTED, nao ignorados. Nome/encodingpublico, defaults, IDs, policy language, groups/time semantics nao inferidos. Reusar BODYs e codigos existentes por adaptacao valida, nao tomar bytes de reserva como API.

## Verificacao requerida em futura tarefa, nao executada aqui

- Parser: ausente/invalido/unknown/duplicado/overflow/refdesconhecida/defaultprofile/secretredaction; nenhuma falha seleciona Legacy.
- Listeners: colecao, conflitosendpoint, isolamento de perfil, PersistentID vsindex/nome, IPv4/v6/addresses e indisponibilidade; nao criar listener extra por conveniencia.
- Policy: primeira terminal ACCEPT/DENY, no-match, todascondicoes, unsupportedreject, per-Principal source/policy current, snapshotupperbound, ModeB e C2 paths explicitamente mapeados.
- Egress: refsserver-owned, exacthostname+port e endpoint, single-resolution/no-rebinding, sensitive/management/selftarget conforme suporte, profileavailable/noimplicitfallback; interface de entrada nunca escolhe link de saida.
- TLS: trust de canal distintoSite, hostname/SNI/ALPN/TLS13, expired/revoked/UNKNOWN, SYSTEM vsCUSTOMrefuse, Principal/grants reais; sem CAfixture como configuracao.
- Apply: participants+commit+CURRENT+R/I/A/P, compare-and-commit, stale/conflict/unknown, oldSessions/pins/leases, Stop/pendingworker/retainedslot; nao revertercommitconfirmado nem resetarserial.
- ED11: novo censo real com todas colecoes/parser/copies/scratch/generacoes/retencao, quota2MiB inalterada, target/providerevidence; nenhuma margemhistorica ou thresholds inventados.
- Plataforma: core compila sem Win32/CNG obrigatorio, adapters explicitamente suportados; box/ISA/NT4 nao qualificados por buildWindows.

## Browser handoff de referencia

Manter importer/store/CredentialGeneration/Reference/leases e VC6/C89, commit atomico/safe retirement; pacoteunico e automaticenrollment ainda requerem contrato/implementacao proprios. Browser nao aprova trustSite/policy/grants servidor. Se futura implementacao precisar alteracao Browser, enviar hunk-handoff para Codex Browser; nao escrever nessa arvore por esta autorizacao.

## Gates que este trabalho nao fecha

Backend CNG/keydisposition, backup/restore/fencing, activeSessioncertrevocation, parametrosModeCfreshness/headroom/provider/kernel, mTLS/HTTPSjointqualification, E10/C8integratedqualification e producao. Eles nao autorizam simplificar config/modelo nem impor novas featuresfuturas aPhase6.

NEXT=OWNER_REVIEW_OF_COMPLETE_CONFIGURATION_POLICY_MODEL
