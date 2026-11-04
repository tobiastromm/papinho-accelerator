from pathlib import Path
import csv,json,re
r=Path.cwd();base=r/'docs/integration/n0start03-integrated-closeout-20261024';p=base/'q4-target-evidence-closure-20261025';f=base/'ED11_GATE_MATRIX.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);ref='q4-target-evidence-closure-20261025/CONSOLIDATED-CLOSEOUT.md'
for x in rows:
 if x['gate']=='E02':x['current_evidence']+='; typedviews: Accelerator1624bytes caller scratch/zero persistent owner growth; Browser216bytes scratch, frames measured; '+ref
 if x['gate']=='E17':x['closure_evidence_or_action']='Desenho futuro independente pronto: nova identidade/escopo próprios ainda não criados; acesso efetivo, prontidão e recursos reais obrigatórios; não repetir Q4'
 if x['gate']=='E18':x['current_evidence']+='; desenho de Stop solicitado/aceito/concluído e retenção independente preparado; '+ref
 if x['gate']=='E19':
  x['current_evidence']='Projeção tipada do produtor native já existente integrada ao root; 528checks novos, quatro builds normais e APIs/fontes oficiais conferidas; '+ref
  x['factual_gap']='Receita observacional com escopos/unidades disponível; envelope hard ou cobertura empírica e parâmetros quantitativos ModeC ainda ausentes; provider/kernel UNKNOWN'
  x['closure_evidence_or_action']='Owner decide garantia/suporte; produzir anexo quantitativo com evidência e receita exata antes de instalação/admissão positiva; nenhuma constante LAB herdada'
 if x['gate']=='E20':
  x['current_evidence']='Browser owner receipt usa projeções tipadas de slots nativos existentes; 4builds VC6/x86,8142checks,16regressões; Accelerator4builds/12runs normais; '+ref
  x['factual_gap']='Mecanismos locais implementados; ledger de capacidade continua sem política/recipe/expiry aplicáveis e DENY; par NSSx86/OpenSSLx64 ativo não qualificado'
  x['closure_evidence_or_action']='Após decisão quantitativa completa, integrar ligação da política/receita aos owners e qualificar um par real com escopo e execução próprios; sem setters fictícios'
 if x['gate']=='E21':
  x['current_evidence']='Pedido Q4 e22 dependências revalidados por hash; registro candidato inerte/command condicionado preparados; sem Verify/disposição; '+ref
  x['factual_gap']='Readback Q4 não autorizado; disposição requer resultado factual e aprovação posterior; reservas/UNKNOWN preservados. Ensaio inicial de perfil errado negado, ponto interno/mutex não instrumentado registrado separado'
 if x['gate']=='E22':x['current_evidence']='4demonstrados/12limitados/6pendentes mantidos; novas implementações reais e testes não equivalem a admissão/fechamento; '+ref
for dest in [f,p/'CURRENT-ED11-22-GATE-MATRIX.csv']:
 with dest.open('w',encoding='utf-8',newline='') as o:w=csv.DictWriter(o,fields);w.writeheader();w.writerows(rows if dest==f else [x for x in rows if re.fullmatch(r'E(?:0[1-9]|1[0-9]|2[0-2])',x['gate'])])
f=base/'PHASE6_GATE_SEPARATION.csv';rows=list(csv.DictReader(f.open(encoding='utf-8-sig',newline='')));fields=list(rows[0]);
for x in rows:
 if x['gate']=='P02':x['authority_evidence']='ED11_GATE_MATRIX.csv; '+ref;x['remaining_scope']='Mecanismos tipados reais qualificados nos dois projetos; política/receita quantitativa/par ativo, Q4 readback/disposição, E17/E18 ainda pendentes; 4/12/6 preservados'
 if x['gate']=='P04':x['remaining_scope']='Mecanismos de evidência integrados aos owners Browser e Accelerator, mas sem política/provider/kernel/real pair. Não é capacidade positiva nem execução operacional'
with f.open('w',encoding='utf-8',newline='') as o:w=csv.DictWriter(o,fields);w.writeheader();w.writerows(rows)
