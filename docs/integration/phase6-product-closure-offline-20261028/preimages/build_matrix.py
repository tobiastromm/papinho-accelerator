"""Rebuild derived documentation only; never execute product/operational code."""
import csv
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
rows = []
def add(id, requirement, origin, profile, implementation, evidence, missing, dependencies, applicability, action):
    rows.append(dict(id=id, requirement=requirement, normative_origin=origin, profile=profile,
                     implementation=implementation, existing_evidence=evidence, missing_evidence=missing,
                     dependencies=dependencies, applicability=applicability, next_action=action))

old = 'docs/capabilities/phase6-c0-c12-recovery-20261011.md'
joint = 'docs/adr/ADR-0029-contrato-mode-c-c2-e-autorizacao-laboratorial.md'
wire = 'docs/adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md'
browser = 'B:docs/integration/c2-s6-canonical-implementation-20261018/CLOSEOUT.txt'
pair = 'B:docs/integration/c2-s6-canonical-implementation-20261018/HANDOFF-MODE-C-HTTPS.txt'
c8 = 'docs/integration/canonical-root-c2-c3-wiring-20261018/closeout.md'
mvp = 'docs/adr/DECISION-phase6-personal-mvp-20261022.md'
prov = 'docs/adr/DECISION-production-client-provisioning-20261018.md'
ed = 'docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv'
d2 = 'docs/adr/DECISION-mode-c-d2-one-operation-preparation-exception-20261026.md'
d45 = 'docs/integration/n0start03-integrated-closeout-20261024/d2-d4-d5-closure-20261026/CONSOLIDATED-CLOSEOUT.md'
for n, req, impl, gap, deps, act in [
 (0,'Endpoint snapshot / exact connect','QUALIFIED_FOR_SCOPE; consumidor posterior preservado','revisão de equivalência atual e NT4','nenhuma reabertura automática','Conservar casos históricos; testar só alteração concreta'),
 (1,'Site socket pump não bloqueante/cancelável','INTEGRADO_OFFLINE no consumidor S6 posterior','target e cancelamento DNS ainda não demonstrados integralmente','C0','Confrontar pump atual/receipts com alvo antes de execução'),
 (2,'Operation + APPLICATION/RECORD PACC','CONTRATO_ENCODING_ACCEPTED; codec/consumer integrados offline','interoperabilidade real autorizada e identidade/grants','C0;C1','Reusar encoding congelado e consumer; qualificar par real após admissão'),
 (3,'Bridge C2 privada / C3','INTEGRADA_CANONICA_OFFLINE; sem PASS conjunto','PST normal, EOF/prefixos e ownership no par real','C1;C2','Mapear owner/attach/teardown atuais e conservar ED08'),
 (4,'Secure authorization / role binding','FOUNDATION_E_NEGATIVOS_OFFLINE','Principal/grants/epoch e roles efetivamente autenticados','C2;C3;E19;E20','Preparar bindings sem fabricar Principal ou CURRENT'),
 (5,'Budgets/backpressure/cancel/deadline/terminal','IMPLEMENTACAO_OFFLINE_LIMITADA','coexistência/retidos/provider/kernel e medições alvo','C1;C2;C3;E19;E20','Fechar anexo quantitativo sem ceiling inventado'),
 (6,'Fixtures determinísticas E1–E12','COBERTURA_PARCIAL; não equivalência com Mode B','rastreio de cada experimento no owner atual','C0;C1;C2;C3;C4;C5','Matriz caso/caller/oráculo; não repetir qualificações válidas'),
 (7,'Experimentos Secure Mode C E1–E12','EXECUCAO_CONJUNTA_NAO_DEMONSTRADA','revisão humana de pré-requisitos e autoridade exata','C6;E19;E20','Preparar pedido só após política/bindings; sem executar'),
 (8,'Integração produtiva Secure Mode C','PARCIAL; hooks/catálogo/consumer offline','produção conjunta e journeys autorizadas','C7;PROV-MANUAL;PROV-AUTO','Integrar evidências conjuntas, sem promover LAB'),
 (9,'Aprovação de segurança Legacy produtivo','DECISAO_D9B_NAO_DEMONSTRADA','decisão vigente aplicável além de D9A LAB','nenhuma aprovação inferida','Conferir estado D9B antes de pedir escolha focal'),
 (10,'Implementação Legacy Mode C','MECANISMOS_PARCIAIS; produto não demonstrado','enforcement perfil inferior e sem downgrade','C9;C8','Reusar runtime qualificado sob perfil aprovado'),
 (11,'Validação Legacy / profile matrix','NAO_QUALIFICADA_INTEGRALMENTE','isolamento, admissão, quotas/audit/revogação','C9;C10','Preparar casos por perfil sem chamar plaintext de Secure'),
 (12,'NT4 Mode C real','VC6_CURRENT_HOST_NAO_E_NT4','execução identificada NT4, primitivas/budgets/lifecycle','C7;NT4','Preparar matriz alvo sem trocar toolchain ou runtime'),
]:
    add(f'C{n}',req,joint+';'+wire,'Mode C conjunto Secure/Legacy conforme marco',impl,
        old+';'+browser+';'+c8,gap,deps,'OBRIGATORIO_ROADMAP_AMPLIADO_POR_PERFIL',act)

with (ROOT / ed).open(encoding='utf-8-sig', newline='') as f:
    for item in csv.DictReader(f):
        if item['gate'] not in {f'E{i:02}' for i in range(1,23)}:
            continue
        name=item['gate']; number=int(name[1:])
        profile=('Alvo Browser/C2/C3 e provider' if number in (19,20) else
                 'Fechamento agregado ED11' if number==22 else
                 'LAB N0/Q4; compatibilidade de budgets e obrigações históricas' if number==21 else
                 'Matriz LAB N0/Q4; não norma universal de todo processo produto')
        scope_origin = ('docs/adr/DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md')
        if number in (4,15):
            scope_origin += ';docs/adr/DECISION-n0-n9-security-descriptor-digest-profile-20261024.md'
        if number == 5:
            scope_origin += ';docs/adr/DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md'
        if number in (17,21):
            scope_origin += ';docs/adr/DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025.md'
        if number in (19,20):
            scope_origin = 'docs/adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md;'+d2
        add(name,item['requirement'], wire+';'+scope_origin+';'+ed,profile,item['status'],
            item['current_evidence'],item['factual_gap'],item['operational_dependency'],
            'APLICAVEL_AO_GATE_E_PERFIL; NAO_GENERALIZAR',item['closure_evidence_or_action'])

for id,req,impl,gap,act in [
 ('MODE-A','Rede e TLS locais','QUALIFICADO_HISTORICO_LIMITADO','trust Site local e runtime alvo','Conservar rotas históricas e separar trust local de mTLS'),
 ('MODE-B','Rede e TLS Accelerator','NUCLEO_HISTORICO_QUALIFICADO; integração posterior limitada','jornada Browser/certificados/Model E atual conforme alvo','Não reabrir núcleo sem divergência; qualificar consumidores atuais'),
 ('MODE-C','Rede Browser e TLS Accelerator','INTEGRADO_OFFLINE_PARCIAL','par real, ED11 e E1–E12','Seguir C0–C12 e pacote correlacionado'),
 ('MODE-D','Rede Accelerator e TLS Browser','TCP_PLAIN_FOUNDATION; matriz Browser aberta','rota mista suportada e fallback/permission negativos','Preparar validação por placement, sem renomear B/C'),
]:
    add(id,req,joint,'Browser placement',impl,old+';'+browser,gap,'capabilities/policy/trust/resources independentes',
        'ROTEAMENTO_APLICAVEL; NAO_INFERIR_QUALIFICACAO',act)

extras = [
 ('CORE','Núcleo Network/TLS histórico','docs/phase6-network-tls-offload.md','Original Mode B','READY_HISTORICO_LIMITADO','docs/phase6-network-tls-offload.md','equivalência apenas se fonte/pin mudaram','nenhum novo TPM','OBRIGATORIO_ORIGINAL','Preservar escopo original, não declarar Phase6 integral'),
 ('PST','SDK API3/SPI4 normal / API2 produção / R2','docs/adr/DECISION-phase6-canonical-consolidation-20261018.md','Ambos consumidores','INTEGRACAO_OFFLINE_COM_EVIDENCIA','B:docs/integration/api3-real-composition-20261018/CLOSEOUT.txt','qualificação conjunta normal final','C7;E19;E20','OBRIGATORIO_AMPLIADO','Verificar fonte e artefato exatos, não publicar065'),
 ('LIFECYCLE-E5-E11','Qualificações históricas E5/E11 e ED08/Model E',wire,'Escopos originais separados dos E05/E11 LAB','PRESERVAR_SEM_REGRESSAO_DEMONSTRADA',old,'apenas diferenças de composição/callers atuais','nenhuma reabertura por checkpoint antigo','VALIDO_NO_ESCOPO; NAO_QUALIFICA_TODO_ALVO','Rastrear reutilização e diferenças, não refazer campanha'),
 ('PROV-MANUAL','Pacote manual + importação granular permanente',prov,'Secure pessoal/produto','COMPONENTES_SHARED_E_IMPORTACAO_R1','B:docs/integration/shared-provisioning-core-consumption-20261020/CLOSEOUT.md','geração/admin/importação real e bootstrap verificável','PROV-ROOT;PROV-CA;ED11','OBRIGATORIO_AMPLIACAO_ACCEPTED','Mapear gerador/decoder/commit e completar jornada real'),
 ('PROV-AUTO','Enrollment com chave cliente/CSR-PoP/código',prov,'Secure pessoal/produto','COMPONENTES_OFFLINE; JORNADA_NAO_DEMONSTRADA',prov,'bootstrap servidor, confirmação e emissão reais','PROV-ROOT;PROV-CA;ED11','OBRIGATORIO_AMPLIACAO_ACCEPTED','Preparar fluxo mesmo owner/cadastro, sem CA LAB'),
 ('PROV-ACCESS','OPEN/CLOSED e AUTO/MANUAL_APPROVAL',prov,'Acesso enrollment, não redefinição geral AuthN','CONTRATOS_ACCEPTED; cobertura produto incompleta',prov,'validação password TLS e binding decisão/request','PROV-AUTO','OBRIGATORIO_AMPLIACAO_ACCEPTED','Negativos replay/pending/grants no caller real'),
 ('PROV-HOT','Add/approve/renew/revoke sem restart',prov,'Mesmo root/issuer/cadastro','TRANSACOES_OFFLINE; fluxo produtivo não demonstrado','docs/integration/canonical-v3-transaction-completion-20261021/closeout.md','callers hot e coexistência/retirement','PROV-ROOT;PROV-CA','OBRIGATORIO_AMPLIACAO_ACCEPTED','Confrontar PREPARE/COMMIT/CURRENT/epoch e leases'),
 ('PROV-REVOKE','Revogação ativa e novas admissões; histórico',mvp,'Pessoal M7','NORMA_RESOLVIDA; integração completa não demonstrada',mvp,'Stop/fences/negação por credencial no runtime real','PROV-HOT;E18','OBRIGATORIO_AMPLIACAO_ACCEPTED','Não reabrir escolha M7; verificar propagação e retenção'),
 ('PROV-ROOT','Autoridade configuração/root/CURRENT/epoch',mvp,'Pessoal NORMAL e configuração produto','DORMANT_NATIVE_OFFLINE; operacional NOT_DEMONSTRATED',c8,'ato autorizado/publicação e referência original viva','autoridade instalação/configuração; sem TPM global','OBRIGATORIO_PRODUTO; INSTANCIA_A_DESCOBRIR','Mapear entrada/consumo atuais antes de propor bootstrap'),
 ('PROV-CA','CA protegida/serial/UTC/issuer NORMAL',mvp,'M1/M3 pessoal não recuperável','COMPONENTES_QUALIFICADOS; emissão produtiva não demonstrada','docs/integration/internal-ca-canonical-implementation-20261019/closeout.md','custódia/tempo/serial/assinatura factuais no perfil','PROV-ROOT;ED11','OBRIGATORIO; RECOVERY_NAO_HERDADA','Separar provas NORMAL/RECOVERY sem bits fictícios'),
 ('SCM','Instalação/lifecycle/conta operacional pessoal',mvp,'Windows pessoal M5','HOST_INTEGRADO; LAB_START_FAIL_NAO_E_READINESS',ed,'readiness/acesso/Stop/restart normal/uninstall','E17;E18;PROV-ROOT','OBRIGATORIO_PESSOAL','Mapear CLI ao mesmo owner; não criar serviço agora'),
 ('CONFIG','Config settings-only/listeners/live apply',mvp+';docs/adr/DECISION-personal-listener-selection-direction-20261023.md','Pessoal M6 e direção posterior','SUBSET_PARCIAL; detalhes defaultLan não aceitos', 'docs/future/personal-listener-selection-and-live-configuration.md','reconciliação CURRENT/reload e fontes reais','PROV-ROOT;PROV-HOT','OBRIGATORIO_NOS_PONTOS_ACCEPTED; DETALHES_PENDENTES','Não congelar gramática/default novos por inferência'),
 ('CERTIFICATES','Certificados Browser / CredentialGeneration/Reference/store',prov,'Secure manual e automatic; trust Site separado','R1_CURRENT_HOST_E_SHARED_CORE_LIMITADOS','B:docs/architecture-roadmap.md','pacote/enrollment/renovação/retirement e runtime final','PROV-MANUAL;PROV-AUTO;PST','OBRIGATORIO_CONSUMIDOR','Provar jornada e erro sem exportar chaves'),
 ('GUI-TLS','GUI TLS, estado/certificados/permissions independentes','B:docs/adr/ADR-0007-integracao-policy-e-ux-do-papinhoaccelerator-no-papinhobrowser.md','Browser UI vinculada a fatos','EVIDENCIAS_HISTORICAS_LIMITADAS','B:docs/architecture-roadmap.md','revisão UX humana pendente; metadata remota/certificados','CERTIFICATES;MODE-A;MODE-B;MODE-C;MODE-D','OBRIGATORIO_CONFORME_DEBITOS; NAO_CSS_GERAL','Mapear controles/campos ao provider e qualificar UX real'),
 ('LEGACY','Perfil inferior e sem downgrade; C9–C11',joint,'Legacy explicitamente admitido','D9A_LAB_NAO_E_D9B_PRODUCAO',old,'decisão/qualificação produtiva aplicável','C9;C10;C11','OBRIGATORIO_ROADMAP; HABILITACAO_NEGADA','Não inventar paridade criptográfica ou excluir requisito'),
 ('NT4','Execução real NT4/VC6/NSS/x86','B:docs/nt4-toolchain.md','Browser alvo NT4','VC6_CURRENT_HOST_PASS_NAO_E_TARGET','B:docs/nt4-toolchain.md','runtime identificado, imports, pump/recursos/lifetime','C12;CERTIFICATES','OBRIGATORIO_TARGET; NAO_TESTADO_REAL','Preparar ambiente/receita e evidências, sem executar agora'),
 ('D4-D5','Política quantitativa diagnóstico uma operação',d2,'Mode C diagnóstico limitado','D2_ACCEPTED_PREPARATION_ONLY',d45,'valores fundamentados age/duration/headroom e bindings','E19;E20','OBRIGATORIO_ANTES_DESTE_DIAGNOSTICO','Completar anexo sem copiar LAB ou inventar ceiling'),
 ('Q4-HISTORY','Disposição de efeitos10584/UNKNOWN','docs/adr/DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025.md','Histórico LAB pós-Start','PRESERVADO_NAO_DISPOSTO','docs/integration/n0start03-integrated-closeout-20261024/q7-audited-native-result-20261025/CONSOLIDATED-RECONCILIATION-AND-NEXT-GATE.md','readback/disposição própria; compatibilidade reservas','E21','OBRIGATORIO_HISTORICO; NAO_ADMISSAO_UNIVERSAL','Conservar sem refund; não estender M3'),
 ('Q4-TPM','Crossboot/ledger/testemunha TPM/NV/ownerAuth','docs/adr/DECISION-q4-w0-w7-20261026.md;docs/adr/DECISION-q4-l0-l7-20261026.md','Rota histórica específica','MODELOS_E_POSSE_TRANSITORIA_LIMITADOS','docs/adr/DECISION-q4-cross-boot-historical-readback-20261026.md','continuidade real, custódia/geração/NV não demonstradas','Q4-HISTORY','CONDICIONAL_A_ROTA_ACCEPTED; NAO_PRE_REQ_GLOBAL_NEW_INSTALL','Congelar expansão nesta tarefa; nenhuma operação'),
 ('DEFERRED','Backup antigo/migração/MDM/sync/CSS/richer policies',mvp,'Futuro fora do MVP conforme limites','NAO_PROMOVER_A_BLOQUEADOR','docs/future/personal-backup-and-identity-recovery.md','nenhuma evidência necessária para escopo excluído; segurança vigente permanece','sem autoridade de recuperação','DEFERIDO_NO_PERFIL; NAO_WAIVER_Q4','Não iniciar nova infraestrutura/ADR para fechar produto'),
 ('RELEASE','Fechamento integral e publicação PST065','docs/adr/DECISION-phase6-canonical-consolidation-20261018.md','Release posterior','NAO_AUTORIZADO',prov,'todos gates aplicáveis e decisão release própria','C0-C12;E01-E22;provisão/GUI/NT4','OBRIGATORIO_FINAL','Não promover065 ou alterar pin064 antecipadamente'),
]
for r in extras:
    add(*r)

with (HERE / 'REQUIREMENTS.csv').open('w', encoding='utf-8-sig', newline='') as f:
    writer = csv.DictWriter(f, fieldnames=list(rows[0]))
    writer.writeheader(); writer.writerows(rows)
print(f'{len(rows)} documentary requirements written; no operational effects')
