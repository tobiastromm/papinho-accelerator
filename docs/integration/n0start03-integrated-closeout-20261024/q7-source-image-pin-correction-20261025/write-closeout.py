from pathlib import Path
import json,hashlib,csv,shutil,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q7-source-image-pin-correction-20261025';h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();art=json.loads((p/'CORRECTED-ARTIFACTS.json').read_text());reqsha=art['request_sha256'];bindsha=art['binding_sha256'];a=art['artifacts']
report=f'''# Q7 SOURCE_IMAGE — reconciliação e correção consolidada

## Resultado factual

A única execução manual aprovada de N0TOKEN-20261025-01 /10584f02e11f92c92b43cddf0d9d316f foi negada em SOURCE_IMAGE. Saída original informada pelo owner preservada em OWNER-MANUAL-Q7-RESULT-AS-REPORTED.txt; os valores reconciliados estão em FACTUAL-RECONCILIATION.json. Aprovação c07384b26a57a438c10a382948ab2be5ffe8ab80fab56138881e605c8c6f4c46 foi USADA e não pode ser reutilizada. O registro original permanece byte-idêntico, inclusive seu estado NOT_STARTED que era verdadeiro na emissão; a atualização de execução é este relatório/live matrix, não reescrita da autorização histórica.

first=13, close=0, starts=0, stops=0, quiescent=0, intent_consumed=0, effect=0, diagnostic_created=0, exit2. Transporte local confirmou fechamento, first0/retained0. Qualificador PID9804, criação134374372593546741; auxiliar PID11552/criação134374372404186311. O valor13 é ERROR_INVALID_DATA reportado pelo qualificador, não uma falha NTSTATUS original do serviço. SOURCE_IMAGE/13 desta execução não é o first13 histórico de03 em outro estágio.

Não houve consumo de intent, criação de imagem/sink, rebind ou Start/Stop por esta execução, conforme o recibo. Isso não prova ausência global de arquivos com nomes previstos, extinção de custos kernel ou quiescência histórica. As ausências futuras precisam de verificação nativa própria. Os inputs protegidos colocados e efeitos históricos continuam preservados; nenhum novo readback protegido/SCM foi executado pelo agente. Não houve nova operação automática.

Não há métricas ED11 contemporâneas, SID/atributos do token de serviço ou máscaras de censo na saída: a falha ocorreu antes desse percurso. PID0/role4 no diagnóstico genérico não é prova de um quinto processo; não há censo nesta saída para essa conclusão. Nenhum ENABLED/DEFAULT/DENY_ONLY ou restrição pode ser inferido. Native protected N0 binding/RAW1 freshness também não foi alcançado no qualificador; a validação histórica dos inputs continua no seu alcance de colocação, sem promoção a prova operacional nova.

## Causa demonstrada e limite da prova

A imagem candidata aprovada e ainda preservada tem SHA256128834681905d861eb3252c1e0079e1d06a02b46c2f6d34ddb3612bcc85b3dc8. O header derivado tinha esse mesmo valor, mas o qualificador aprovado57bb39d3... contém o valor anterior2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821 duas vezes em ASCII e uma em UTF16; não contém o esperado. A chamada lab_hash(cycle_source_file,cycle_sha) devolve13 quando o hash difere.

O log final anterior mostra apenas compilação/link da DLL, sem recompilar o qualificador depois da troca do header derivado. O grafo não tinha dependência explícita de scm-q4-image-pin.h e a descoberta automática /showIncludes não protegeu essa alteração. Há divergência observável do prefixo localizado/encoding no rules.ninja; não se presume causa universal do toolchain nem se altera sua linguagem/backend global para contornar a falha. O defeito específico comprovado é o pin gerado não invalidar o objeto do qualificador. Conferências anteriores de hash dos arquivos/header falharam em verificar o valor incorporado no executável; esse controle insuficiente foi corrigido.

O recibo não imprime o digest calculado pelo handle SOURCE_IMAGE que falhou. A identidade/conteúdo exatos naquele instante não são reconstruídos retrospectivamente pelo hash atual. Ainda assim, o defeito de compilação é factual e suficiente para rejeitar a imagem aprovada, foi reproduzido com o executável realmente usado e foi removido nos novos binários. Não se solicita repetir a operação para redescobrir a causa.

## Correção e qualificação técnica

CMakeLists.txt agora declara OBJECT_DEPENDS explícito dos pins Q4/successor/tuple/context para scm-registration-lab.c. A imagem não depende do próprio digest e não foi relinkada. O novo build não sobrescreve os diretórios aprovados. O header continua vinculado à mesma imagem congelada. Um verificador read-only novo compara imagem/header e constantes ASCII/UTF16 da seção .rdata do PE AMD64; o pipeline de build corrigido chama essa atestação após cada qualificador. Esse controle de constantes não é execução/medição operacional nem prova de comportamento arbitrário do PE.

A seleção Q4 no launcher e a entrada nativa apontam para um registro futuro exclusivo EXACT-Q7-CORRECTION-APPROVED.json, que NÃO existe. Isso preserva a aprovação usada e bloqueia sua reutilização; nenhuma aprovação foi criada. O novo qualificador e os pins derivados são candidatos técnicos, sem admissão. Os ramos históricos/default EMPTY_DENY e os demais contratos não foram relaxados. Nenhum novo owner/logger/serviço/IPC ou quota.

Resultados finais:

- Quatro builds de qualificador Selected/Legacy Debug/Release e quatro rebuilds focais após separar o caminho da futura aprovação:PASS, zero warnings com /WX. Bibliotecas/modelos relacionados recompilados; a imagem candidata congelada permaneceu12883468..., sem novo build/imagem operacional.
- Quatro atestações dos binários finais:pin esperado presente em ASCII e UTF16, pin antigo ausente, dependência explícita no Ninja. Quatro probes de argumento inválido:exit2 antes do transporte/preflight, sem --prepare/--cycle.
- Sete negativos de atestação:PASS, incluindo rejeição do executável aprovado que realmente falhou, hash errado, remoção de ASCII ou UTF16, PE truncado/inválido.
- Fixture mínimo de build incremental:duas variantes PASS. Com descoberta automática deliberadamente indisponível somente no fixture, alteração A→B não atualiza o executável sem dependência explícita; com OBJECT_DEPENDS recompila e incorpora B. Nenhum header de produto foi alterado para o teste. É regressão nova do grafo de build, não outro harness de handles/ciclo.
- Windows PowerShell5.1:31 gates de Q7 atualizados apenas para o novo nome de aprovação,27 históricos e3 dos artefatos/registro inerte atuais:61 PASS. Aprovação antiga é rejeitada; registro novo ausente; aprovação hipotética válida somente em memória de fixture. Nenhum launcher operacional foi chamado.

As campanhas anteriores164C/76regressões/token/censo/ownership/console/transporte permanecem evidência no alcance original, não novas execuções. Não foram repetidas sem alteração pertinente. O backend de hash, predicados token.specific_group.enabled, Stop, UNKNOWN, A1/B1/self-token, RAW1 e transporte não foram alterados. O antigo fixture de stack-VA que exibiu crash não foi executado.

## Hashes e preservação

| Artefato | SHA256 |
|---|---|
| Launcher candidato corrigido | `{a['launcher']['sha256']}` |
| Qualificador Selected Release corrigido | `{a['qualifier']['sha256']}` |
| Imagem congelada preservada | `{a['service_image']['sha256']}` |
| Pedido corrigido candidato | `{reqsha}` |
| Binding corrigido candidato | `{bindsha}` |

CORRECTED-ARTIFACTS.json, compiled-pin-and-entry-results.json e FINAL-HASHES.json registram os arquivos reais. Preimagens de CMake/launcher/entrada nativa e APPLIED-SOURCE.patch preservam o WIP tocado. Os antigos binários, request e aprovação foram conferidos byte-exatamente; o launcher histórico também permanece em preimages do pacote anterior. Nenhum ADR accepted foi editado. HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91 preservado, sem commit/push/tag/clone/worktree. Sem SCM/ACL, limpeza, refund, aumento de quota, CURRENT/assinatura/recuperação.

## Próximo gate real

O bloqueio técnico SOURCE_IMAGE está corrigido e qualificado offline. Não há autorização para nova invocação. O task Q7 proíbe repetir após falha/UNKNOWN e a decisão aceita autoriza uma única execução manual, já realizada. A falha antes de efeitos não cria exceção automática. Q7-CORRECTED-OPERATION-REQUEST.DRAFT.json apresenta o novo binding e pede decisão EXPRESSA do proprietário sobre uma única invocação corretiva após esta falha, considerando a restrição de retry; não herda aprovação/consumo nem gera identidade/objetos. Se for necessária outra identidade/escopo normativo, isso deve ser decidido explicitamente antes de novos vínculos, nunca inventado pelo agente.

NEXT-OWNER-DECISION-AND-MANUAL-CANDIDATE.md contém os valores conhecidos e as dependências. Nenhum comando positivo executável é entregue enquanto essa decisão não existir. Qualquer aprovação futura precisa de novo registro/hashes e gates frescos, incluindo128/1024, quatro papéis,60s/1s, headroom, original-handle RAW1/identidade e compatibilidade das obrigações. A proposta não garante margem operacional, orçamento ou obtenção de RUNNING.

ED11 ainda depende de evidência do novo token e seus atributos brutos, correlação PID/criação/máscaras, lifecycle/Stop/quiescência no alcance correto, fechamento/retensão e recursos contemporâneos. A Phase6 mantém também seus gates de outros componentes; esta falha não concede crédito a eles. Q2 continua documental, global quiescence UNPROVEN, reservas reais indisponíveis e UNKNOWN históricos preservados. N7 mantém suas limitações explícitas; a operação não chegou à observação que poderia esclarecer o token.

CURRENT_TASK_COMPLETED=YES_FOR_RECONCILIATION_AND_AUTHORIZED_TECHNICAL_CORRECTION
Q7_RESULT=DENIED_SOURCE_IMAGE_BEFORE_EFFECTS
PRIOR_APPROVAL=USED_ONCE_NONREPLAYABLE
FIRST_ERROR=13
CLOSE=0_REPORTED
INTENT_CONSUMED=NO
STARTS=0
STOPS=0
EFFECTS=0
SERVICE_TOKEN=NOT_OBSERVED
CENSUS_MASKS=NOT_OBSERVED
RESOURCE_BUDGET=NOT_MEASURED_NOT_GRANTED
SOURCE_IMAGE_PIN_CORRECTION=QUALIFIED_OFFLINE
FURTHER_EXECUTION=DENY_PENDING_EXPLICIT_POSTFAILURE_OWNER_DECISION
GLOBAL_QUIESCENCE=UNPROVEN
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_POSTFAILURE_SCOPE_AND_EXACT_CORRECTED_BINDING_DECISION
'''
(p/'CONSOLIDATED-RECONCILIATION-AND-CORRECTION.md').write_text(report,encoding='utf-8')
paragraph=f'\n\n## Q7 denied at SOURCE_IMAGE; compiled pin corrected — 2026-10-25\n\nSee docs/integration/n0start03-integrated-closeout-20261024/q7-source-image-pin-correction-20261025/CONSOLIDATED-RECONCILIATION-AND-CORRECTION.md. Original owner run first13/SOURCE_IMAGE, close0, starts0/stops0, intent_consumed0/effect0/diagnostic_created0; transport close confirmed. Approval c07384... USED once, not reusable. Actual approved qualifier embedded pre-freeze2386d63a... instead of frozen image12883468...; explicit generated-header dependency and compiled PE pin attestation correct this defect. Four configurations zero warnings, four compiled-pin checks,7 negative cases including actual failed artifact,2 incremental dependency variants,61 PowerShell gates and4 invalid-entry probes PASS. Historical image/binaries/approval/request preserved; original protected inputs unchanged. No new operational execution. Corrected request candidate {reqsha}; no new approval exists and no retry is authorized. A fresh explicit post-failure owner scope/binding decision is required before any further invocation. Token/census/ED11 budget not observed; controlled close is not global quiescence. Historical UNKNOWN/reservations retained, ED11/Phase6 PARTIAL and productive authorities DENY.\n'
for f in [r/'docs/capabilities/canonical-phase6-consolidation.md',p.parent/'INTEGRATED-CLOSEOUT.md',p.parent/'ED11_GATE_MATRIX.csv']:
 dest=p/'live-preimages'/f.name;dest.parent.mkdir(exist_ok=True);shutil.copyfile(f,dest)
 if f.suffix!='.csv':
  with f.open('a',encoding='utf-8') as out:out.write(paragraph)
 else:
  with f.open('a',encoding='utf-8',newline='') as out:csv.writer(out).writerow(['Q7_NATIVE_SOURCE_IMAGE_RESULT','Independent token observation and actual compiled pin','DENIED_BEFORE_EFFECTS_PIN_CORRECTED_OFFLINE','q7-source-image-pin-correction-20261025/CONSOLIDATED-RECONCILIATION-AND-CORRECTION.md; first13 close0 starts0 consume0 effects0; compiled old pin reproduced; dependency/PE attestation corrected','No service token or census masks/budget observed; prior one-run approval used; global quiescence UNPROVEN','Explicit owner post-failure scope decision and exact corrected binding request then only separately authorized manual effects','YES new explicit decision required; no retry authority inferred','Any controlled UNKNOWN identity RAW1 freshness budget or unresolved authority => DENY retain no retry','PENDING_OWNER_POSTFAILURE_DECISION'])
assert not (p/'EXACT-Q7-CORRECTION-APPROVED.json').exists();assert subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()=='913c90cf2328f7939d8f5b927640601903ac6e91'
manifest={str(f.relative_to(r)):h(f) for f in p.rglob('*') if f.is_file() and f.name!='FINAL-HASHES.json'}
for f in [r/'CMakeLists.txt',r/'tools/qualification/scm-instrumented-cycle-lab.inc',r/'tools/qualification/scm-n0-protected-single-powershell.ps1',r/'docs/capabilities/canonical-phase6-consolidation.md',p.parent/'INTEGRATED-CLOSEOUT.md',p.parent/'ED11_GATE_MATRIX.csv'] :manifest[str(f.relative_to(r))]=h(f)
(p/'FINAL-HASHES.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps(dict(status='PASS_RECONCILED_AND_TECHNICALLY_CORRECTED',request_sha256=reqsha,launcher=a['launcher']['sha256'],qualifier=a['qualifier']['sha256'],execution='NO_NEW_EXECUTION',next='EXPLICIT_OWNER_POSTFAILURE_SCOPE_DECISION'),indent=2))
