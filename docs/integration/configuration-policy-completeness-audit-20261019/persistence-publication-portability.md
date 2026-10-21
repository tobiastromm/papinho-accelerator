# Persistencia, publicacao e plataformas

## Matriz de persistencia/publicacao

| Camada | Conteudo e codigo | Fato permitido | O que NAO comprova |
|---|---|---|---|
| Source CLI | server_cli.c parse/request; modelo source-agnostic | port/bind/perfil/refs/log parseados | commit, parser de arquivo geral, admin autorizado |
| Model | server_config.h.api3.inc e validate | config normalizada bounded com policy/catalogo opcionais | colecao completa, grupos/tempo, resolver pronto |
| BODY0.2 | private versioned historical records | antigas refs/campos na propria versao | conversao em policy/Principal16 atual |
| BODY0.3 | server_configuration_store03_win32.inc | policy historica, Principal campo32 | identidade16 autenticada por padding/hash/truncation |
| BODY0.4 | server_configuration_body04_win32.inc | Principal type1/length16 e policy/contexto | schema geral de admin ou reinterpretacao de BODY03 |
| BODY0.5 | body05_encode/decode | policy04 e um catalogo Site TLS embedded; checksums/presence/zero reserved | outro catalogo/trust implicito, multiplosperfis completos, grupo/calendario |
| ROOT06 | issuer_codec + configuration_issuer06 | mesma root8192+issuer8192; full serial/status/request history | PUBLISHED/REVOKED produtivo, signer habilitado ou freshnessglobal |
| Link1 opcional | issuer_link_encode/decode, pending_win32.inc checkpoint APIs | 512bytes no MESMO arquivo, total16896: root/R/predecessor/A/P/I/highwater/CAgen/digests/correlacao | autoridade de restore ou antirollback global |
| PREPARE/VALIDATE | root_transaction/participants/security-source/leases | participant ready, recursosreservados, expected predecessor/projecao conhecida | readiness sintetica ou mera copia de config |
| COMMIT | configuration_commit/pending worker | TxF/CAS byte-exato/file identity/write/flush/commit confirmado | CURRENT automatico; rollback depois de commit |
| CURRENT/A | issuer_publish_live apos confirmacao | publicacao atomica do snapshot e A factual | P copiado de A/revision ou emissao/grants de cliente |
| P C2 | autoridade de policy existente | mantido so para projecao operacional identica; vigente em novos efeitos | arquivo recuperar Sessions/grants ou caller escolherepoch |
| Snapshots/pins | 3slots, Session/listener/attachment refs | antigas geracoes imutaveis e leases mantidas ate quiescencia | nova autoridade de pin antigo; expulsar retidos por capacidade |
| Stop | pendente nao bloqueia owner esperando worker; poll zero wait | retorno factual, contencao e credito retido | timeout=cancelado, exit=DESTROYED, retry destrutivo |
| Recovery | recovery06 e checkpoint_data07 | dados historicos validados com retorno NOT_SUPPORTED | CURRENT/A/P live, latest checkpoint/fencing ou assinatura; DENY continua |

Atualizacao live comprovada no ultimo closeout e issuer-only com projecao identica. Nao a anunciar como reload geral de listener/trust/egress/grupo/time. Mudanca de transport profile exige recriacao explicita do listener (A09), e revisoes antigas nao podem ter grants alterados retroativamente. refs resolvidas exigem snapshot/versao/ownership estaveis; environment nao e identidade duravel.

## Matriz de portabilidade

| Classe | Contrato/codigo localizado | Estado | Consequencia para configuracao |
|---|---|---|---|
| CORE_PORTABLE | PAPACC_SERVER_CONFIG, auth/Principal ports, capability snapshot, exact dest, policy/profile models, issuer core/codec | IMPLEMENTED nos subsets; nao expos Win32/CNG como tipo comum | mesmas regras normalizadas, largura/endianness/presenca e limites explicitos |
| WINDOWS_ADAPTER | interfacesWin32, WinSock bind/listener, security source, TxF, BCrypt hash, CNGbackend parcial, Registry/filesystem de credenciaisBrowser | IMPLEMENTED/PARTIAL conforme subsistema; perfil candidato065 | local mechanism nao e arquitetura nem qualificacaoCNG/ED11 |
| BOX_ADAPTER | nenhum firmware/backend completo correspondente localizado | UNQUALIFIED / futuro | nao forcar paths/Registry/CNG; incapacidade de storage/auth/time nega efeitos |
| ISA_ADAPTER | possibilidade arquitetural A02, sem driver/protocolo/hardware implementado | UNQUALIFIED / futuro | nao inventar DMA/OS/time/crypto/identity; nenhum suporte operacional alegado |
| UNQUALIFIED | NT4 exato novo perfilcert, box/ISA, restore/fencing, totalModeC | gates separados | nao consumir configuracao parcialmente suportada como se campos tivessem sido aplicados |

CNG/SID/DACL/NTFS/Registry sao adaptadores Windows, nao nomes obrigatorios no formato comum. Interface persistente indisponivel deve permanecer ausente, nunca index/nome inventado. Um target sem UTC confiavel, durabilidade ou algoritmo requerido nega efeito dependente; nao altera profile nem faz downgrade.

## ED11 disponivel (sem recompilacao nesta auditoria)

Ultimo closeout issuer-same-root-checkpoint-linkage reporta known1801168, combinado1972291 comattachment, cap2097152, owner176680, configuracao105632. Valores sao evidencia compilada anterior do perfil declarado, nao medicao/requalificacao desta tarefa nem orcamento para engine completo. Novo formato/colecoes/strings/rules/groups/snapshots/parser/scratch/reload precisa novo censo/delta/retencao antes de efeitos, sem aumentar quota. Provider/kernel UNKNOWN, freshness/headroom pendentes; nenhuma margem512MiB ou1000ms importada.
