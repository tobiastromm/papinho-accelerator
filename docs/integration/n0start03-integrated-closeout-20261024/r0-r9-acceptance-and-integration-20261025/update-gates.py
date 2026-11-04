from pathlib import Path
import csv,json,collections
r=Path.cwd();base=r/'docs/integration/n0start03-integrated-closeout-20261024';p=base/'r0-r9-acceptance-and-integration-20261025';f=base/'ED11_GATE_MATRIX.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);ref='r0-r9-acceptance-and-integration-20261025/CONSOLIDATED-CLOSEOUT.md'
for x in rows:
 g=x['gate']
 if g=='E01':x['factual_gap']='R0–R9 ACCEPTED apenas para regra prospectiva privada e avaliação documental; nenhuma autoridade operacional';x['closure_evidence_or_action']='Preservar limites e escopos accepted; sem waiver geral'
 if g=='E02':x['current_evidence']+='; R integrado: +72bytes no owner compilado, frame próprio/custos distintos medidos; '+ref;x['factual_gap']='Custos novos ledgerados offline, sem crédito; composição completa do serviço e alvo ainda não observada'
 if g=='E07':x['closure_evidence_or_action']='R0–R9 resolvido normativamente e implementado offline; comprovar runtime em futuro escopo independente, sem mudar SCM'
 if g=='E17':
  x.update(status='PENDENTE_OBSERVACAO_OPERACIONAL',current_evidence='R0–R9 ACCEPTED byte-exato; regra prospectiva integrada/4builds/negativos/recorder/acesso qualificados offline; '+ref,factual_gap='Nenhum serviço real executou o novo artefato; token/prontidão/recursos completos seguem não demonstrados',closure_evidence_or_action='Somente após nova identidade/escopo/cobertura/pins/aprovação próprios, qualificar ciclo independente; nunca repetir Q4',operational_dependency='FUTURE_INDEPENDENT_SCOPE_NOT_AUTHORIZED',closure_class='PENDENTE_OBSERVACAO_OPERACIONAL')
 if g=='E19':
  x.update(status='PENDENTE_DECISAO_NORMATIVA_E_INTEGRACAO_ALVO',current_evidence='80checks novos das funções reais extraídas; Accelerator admite somente RESOURCE_UNKNOWN mesmo com raw válido; '+ref,factual_gap='Sem receita provider/kernel ou política empírica instalada; parâmetros ModeC de métrica/freshness/headroom dependem decisão separada',closure_evidence_or_action='Definir prova/garantia e política específica por alvo/backend; integrar receita vinculada antes de qualificação real',operational_dependency='TARGET_POLICY_THEN_PRODUCER_AND_REAL_COMPOSITION',closure_class='PENDENTE_DECISAO_NORMATIVA_E_INTEGRACAO_ALVO')
 if g=='E20':
  x.update(status='PENDENTE_INTEGRACAO_E_QUALIFICACAO_ALVO',current_evidence='Topologia real C2/C3/Browser mapeada; c2_target Browser estático zero sem writer; SDK91entries preservado; '+ref,factual_gap='Ausente produtor owner-bound Browser; OpenSSLx64 versus NSS/VC6x86 sem receita equivalente nem composição ativa comprovada',closure_evidence_or_action='Implementar produtor/recipe sob política aprovada, qualificar par real e lifecycle/control/data/retidos; não usar LAB como teto',operational_dependency='TARGET_POLICY_AND_ARTIFACT_PAIR_THEN_AUTHORIZED_TARGET_RUN',closure_class='PENDENTE_INTEGRACAO_E_QUALIFICACAO_ALVO')
 if g=='E21':
  x.update(status='PENDENTE_AUTORIZACAO_DOCUMENTAL_E_READBACK',current_evidence='R6–R8 ACCEPTED para avaliação documental; Q4 consumido/reservas/UNKNOWN preservados; nenhum readback/aplicação; '+ref,factual_gap='Leitura protegida não autorizada; disposição exata depende resultado e autorização própria; orçamento novo não concedido',closure_evidence_or_action='Pedido readback2aa9... separado; após resultado preparar aplicação documental exata; sem refund/liberação física',operational_dependency='SEPARATE_READBACK_AND_LATER_DOCUMENTARY_APPROVALS',closure_class='PENDENTE_AUTORIZACAO_DOCUMENTAL_E_READBACK')
 if g=='E22':x.update(status='PENDENTE_DEPENDENCIAS',current_evidence='22gates reconciliados; 4demonstrados/12limitados/6pendentes; '+ref,factual_gap='E17/E18/E19/E20/E21 e escopos reais dos gates limitados ainda abertos',closure_class='PENDENTE_DEPENDENCIAS')
for dest in [f,p/'CURRENT-ED11-22-GATE-MATRIX.csv']:
 with dest.open('w',encoding='utf-8',newline='') as out:w=csv.DictWriter(out,fields);w.writeheader();w.writerows(rows)
(p/'GATE-COUNTS.json').write_text(json.dumps(dict(total=len(rows),demonstrated=4,limited=12,pending=6,statuses=dict(collections.Counter(x['status'] for x in rows))),indent=2)+'\n',encoding='utf-8')
f=base/'PHASE6_GATE_SEPARATION.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);
for x in rows:
 if x['gate']=='P02':x['authority_evidence']='ED11_GATE_MATRIX.csv; '+ref;x['remaining_scope']='R0-R9 ACCEPTED e integração offline; Q4 sem readback/disposição; E19/E20 requerem política/producer/recipe/par real; ED11 4demonstrados/12limitados/6pendentes'
 if x['gate']=='P04':x['remaining_scope']='Fonte real investigada: Browser c2_target sem writer, Accelerator recipe ausente; política ModeC quantitativa pendente; nenhum par ativo qualificado'
with f.open('w',encoding='utf-8',newline='') as out:w=csv.DictWriter(out,fields);w.writeheader();w.writerows(rows)
