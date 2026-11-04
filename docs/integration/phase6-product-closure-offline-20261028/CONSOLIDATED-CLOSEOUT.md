# Phase 6 — execução do pacote técnico offline

TASK PACC-PHASE6-PRODUCT-CLOSURE-OFFLINE-EXECUTION, 2026-10-28, conforme autorização de executar o prompt preparado. A etapa offline foi executada; a Phase6 não foi declarada concluída.

## Resultado verificável

- Baseline documental anterior SHA2567588bfcd2f0d395fc841c6a71ff0838b839fa631c6910a84db2d221a0f5197c3 reconferida antes de alterar derivados. Cópia BASELINE.json e preimages da matriz/gerador preservadas.
- 36 fronteiras de código localizadas em ambos projetos; mapa inclui campos/guardas, linhas, estado e fonte factual ausente. Localização lexical não é trace de execução nem call graph completo. Revisão manual dos trechos críticos fundamenta os achados abaixo.
- Sete registros accepted reconferidos e pinados; 30 ingredientes quantitativos reconciliados sem alterar os valores históricos ou preencher parâmetros sem fundamento.
- 21 etapas de jornadas concretas em JOURNEYS.csv: manual, automático, revogação, restart normal, Browser/certificados/GUI/Legacy/NT4, com funções e condições de STOP.
- EXECUTION-VERIFICATION.json registra hashes/fontes/refs e nenhuma operação. Nenhum teste de produto/build repetido: não houve patch funcional ou defeito cuja correção justificasse campanha. O verificador do novo pacote é auditoria documental/de fontes, não regressão de segurança ou prova positiva nativa.

## Divergências documentais resolvidas

**Não há nova decisão de formato manual a pedir.** DECISION-manual-package-m0-m8-20261020 aceita M0–M8; F1–F5/G0–G7 também estão aceitos em DECISION-package-signer-delegation-persistence-20261020. Os closeouts antigos generator/factual-trust que diziam formato/extensão PROPOSED são históricos. A fonte positiva não existe por aceitar representação.

**Não há nova decisão de código/ALPN/framing de enrollment a pedir.** P0–P9/T0–T7 revision2 e a especialização de listener foram aceitos. A exceção autoriza endpoint ENROLLMENT separado na MESMA coleção/root/owner, com papacc-enroll/1 e servidor autenticado; não é serviço/autoridade genérica adicional e não altera mTLS operacional. Falta integração canônica de propósito/coleção/runtime e producers factuais, não nova escolha do mesmo protocolo.

**M3 e M7 já estão resolvidos normativamente.** A implementação não acompanhou toda a norma. Não pedir outra aceitação do MVP nem tentar resolver suas lacunas com TPM por default.

## Bloqueios técnicos demonstrados por caller

1. **Issuer/root:** issuer_root_port_read deixa epoch/proofs deliberadamente ausentes; issuer_root_port_cas recusa publish e devolve UNCERTAIN após commit dormente. O hook separado issuer_publish_live existe, mas não comprova integração ponta a ponta com o core. view_ok exige ROOT_PROOFS31 incluindo COMPLETE_CHAIN/FENCED_HIGH_WATER. Logo M3 NORMAL precisa de representação e producer corretos por propósito, preservando a rota RECOVERY; root03/04 witness não é implementação issuer M3. Não preencher bits31 ou retorno OK para fazer emissão passar.
2. **Pacote/trust:** mp_auth_pdg_candidate termina SOURCE_ABSENT mesmo com DATA/digest/SPKI correspondentes; sp_trust_acquire é binding fixo negativo. Browser pb_m1_step para em TRUST, antes de KDF. Falta preparação factual prévia dos owners e binding borrow-only E8. Não é problema corrigível trocando callback por um PASS nem abrindo ownerAuth.
3. **Enrollment:** ep_frame_decode/ep_trace_observe/ep_pair_claim_data/ep_limiter_admit e ep_tls_profile_prepare existem, mas são DATA/perfil offline. Não há ligação produtiva demonstrada de listener a esses helpers no escopo apps/server revisado. EP_TRACE não autentica AUTO/MANUAL nem valida password CLOSED. Propósito persistente/runtime/CSR-to-issuer/client key flow ainda precisam composição.
4. **M7:** papacc_issuer_transition representa estado canônico de registro; revogação de operação/Session C2 existe. Não foi demonstrada ligação factual credencial→todas Sessions/operações→negação de novos efeitos/Stop/quiescência. Nenhuma flag ou snapshot mutável substitui esse fanout.
5. **Alvo:** C8 view conserva policy ABSENT/RESOURCE_UNKNOWN, Browser c2_capacity depende de fonte provider/kernel/host/freshness não suprida. Isso é negação intencional conforme norma, não defeito para remover. O anexo separa custos rígidos/históricos/ativos UNKNOWN, clocks e lifetimes, sem instalar setter de capacidade.

Essas lacunas são de implementação/fonte/qualificação, não evidência de exploit ou regressão em caminho já admitido. Corrigir docs e precisão da matriz foi autorizado; fabricar producers/semântica de autoridade/política ainda incompleta não foi. Não houve patch de segurança para enfraquecer DENY.

## Sequência de implementação que o mapa permite

**Em paralelo, sem efeitos:** (a) fechar desenho técnico purpose-specific de provas NORMAL M3 e consumidores issuer/root para preservar RECOVERY; (b) preparar owner→observação administrativa→confirmação independente→borrow E8 antes de KDF; (c) confrontar modelo/persistência de listener e fluxo enrollment aos contratos já aceitos, sem aceitar representação nova por inferência; (d) D4/D5 e composição Browser/rotas/GUI/NT4 por alvo. Reusar algoritmos, owners e formatos existentes. Cada proposta funcional deve declarar exatamente se depende de representação ainda não aceita; só essa diferença exige decisão normativa.

**Depois, operações próprias:** raiz/configuração/issuer reais e suas autorizações onde necessários; emissão/provisão manual/automatic; serviço/readiness/acesso/Stop; par Mode C com política completa e evidência por fase. Nenhum pacote de execução positiva está completo agora. Não pedir autorização operacional genérica nem outro Start para reconfirmar a falha conhecida.

Q4/TPM permanece trilha histórica independente, sem aplicação, leitura, crédito ou cleanup. O produto não passa a exigir NV/ownerAuth por causa do ledger Q4. Isso não fecha E21 ou autoriza reutilizar suas reservas; compatibilidade de futura admissão ainda precisa fatos próprios.

## Estado final e artefatos

E17/E18 dependem de prova operacional própria; E19/E20 de política e composição reais; E21 histórico continua PENDING; E22 derivado. ED11 e Phase6 PARTIAL, autoridades DENY. Provisão/certificados/GUI/Legacy/NT4 não ficam implicitamente fechados por ED11.

CALLER-MAP.csv, CALLER-EVIDENCE.json, DECISION-STATE.json, D4-D5-ANNEX.json, D4-D5-DECISION-AND-BINDINGS.md, JOURNEYS.csv e EXECUTION-VERIFICATION.json constituem o pacote executado. O roadmap/matriz anterior recebe correções derivadas destes achados. Não há outro comando operacional ou prompt de nova auditoria genérica como entrega.

HEAD/WIP/pins/ADRs accepted preservados; Browser somente lido. Sem secret, rede, TPM/NV, Q4, intents, bootstrap, Start/Stop, SCM/ACL, cleanup/refund, CURRENT, assinatura, recuperação ou publicação PST.
