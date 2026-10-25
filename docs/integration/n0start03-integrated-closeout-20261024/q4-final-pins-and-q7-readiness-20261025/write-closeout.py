from pathlib import Path
import json,hashlib,csv,io,shutil
r=Path.cwd();p=Path(__file__).resolve().parent;a=json.loads((p/'FINAL-ARTIFACTS.json').read_text());arts=a['artifacts'];h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();request=a['request_sha256'];binding=a['binding_sha256']
manual=f'''# Q7 — procedimento candidato, ainda sem aprovação

Operação N0TOKEN-20261025-01, identidade10584f02e11f92c92b43cddf0d9d316f. Q4 NOT_ACTIVATED; orçamento NOT_GRANTED. Nenhum comando operacional foi executado por este pacote.

Pedido exato: Q7-OPERATION-REQUEST.DRAFT.json, SHA256 `{request}`.
Anexo final candidato: Q4-FINAL-BINDING.ANNEX.PROPOSED.json, SHA256 `{binding}`.

A decisão futura deve aprovar expressamente o binding final Q4 e uma única execução manual Q7 nos limites desse pedido. Registrar a decisão normativa final pelo mecanismo existente antes do uso; preservar o anexo candidato. Registrar separadamente a aprovação operacional exata, sem sobrescrever decisões anteriores. Q7-APPROVAL-RECORD.INERT-TEMPLATE.json é apenas esquema negado: não é aprovação, não pode ser renomeado ou usado como autorização. A aprovação real não existe. Seu SHA256 não pode ser antecipado.

Após decisão válida, revisar novamente todos os hashes/identidades e o registro; serializar a aprovação tipada UTF8 sem BOM/LF, começando pelas propriedades status/operation/attempt em ordem e com os valores exigidos pela superfície nativa existente. Somente o registro aprovado terá status EXACT_MANUAL_POSITIVE_OWNER_APPROVED e q7_activation_authorized=true; isso requer decisão humana, não edição automática do template. Incluir as referências exatas do pedido, do binding final e de todas as políticas/artefatos. Calcular o SHA256 integral do novo registro EXACT-Q7-APPROVED.json e fornecer seu literal no comando definitivo. Nenhuma aprovação S2 ou operacional histórica é reutilizável.

Usar um único Windows PowerShell5.1 x64 já elevado, operador autorizado, mesmo host/boot, console interativo original, sem impersonation ou redirecionamento. Não abrir outro launcher/owner. Os gates nativos revalidam os objetos originais, os pins RAW1, SID/identidade, exclusividade, orçamento independente128/1024/quatro papéis, freshness60s/1s, headroom e ausência de Stop/freeze. Divergência cancela; não repinar para produzir PASS.

Forma do comando futuro (não executável enquanto o campo entre sinais de menor/maior não for substituído pelo hash real da aprovação):

```text
& "C:\\Projetos\\PapinhoAccelerator\\tools\\qualification\\scm-n0-protected-single-powershell.ps1" -Mode Cycle -Profile Q4TokenDiagnostic -ExpectedScriptSha256 {arts['launcher']['sha256']} -ExactApprovalSha256 <SHA256_LITERAL_DO_NOVO_REGISTRO_APROVADO>
```

Não executar Prepare separadamente. A admissão fresca ocorre dentro do ciclo exato, antes dos efeitos. A primeira confirmação usa START_N0_SINGLE_LAB N0TOKEN-20261025-01 10584f02e11f92c92b43cddf0d9d316f e o PID/criação factual do auxiliar exibido; BIND_BD_READ_ONLY inclui o qualificador original/criação/challenge factual, não valores históricos. Última confirmação: INSTRUMENT_SCM_LAB 10584f02e11f92c92b43cddf0d9d316f. Mesmo o nome histórico de BIND_BD_READ_ONLY não concede autoridade: vale o plano positivo, sua identidade e a aprovação Q7 específica. Escape solicita Stop entre chamadas síncronas. F10 retorna ao mesmo console somente após fechamento factual; não concede retry.

Máximo condicionado: um consumo canônico novo, imagem/sink exclusivos e arm incorporado, rebind explícito do mesmo serviço, um Start e no máximo um Stop seguro aceito pelo estado real. Não restaurar ImagePath automaticamente. Recusa precoce do serviço é resultado diagnóstico válido; não repetir até RUNNING. Em falha parcial/UNKNOWN, preservar os objetos e referências, primeira falha e dependências; não excluir/rollback/kill/forçar mutex/rearmar. Não fechar novamente handle incerto.

Preservar saída integral original, IG_FINAL, token/SID/atributos/erros, masks esperado/visto/ausente e PID/criação/ordem/wait original, métricas e picos amostrados, recibos de fechamento/retensão e diagnóstico protegido. Retornar a saída ao projeto para reconciliação, sem repetir a operação. A consulta protegida posterior, se necessária, deve ser read-only no contexto autorizado; nenhum segundo recibo de Place é criado.

A nova evidência não recuperará atributos do token histórico03, IDs históricos ausentes, masks não impressas ou quiescência global. ED11/Phase6 seguem PARTIAL até os gates próprios; CURRENT/signature/recovery permanecem DENY.
'''
(p/'MANUAL-Q7-CANDIDATE.md').write_text(manual,encoding='utf-8')
report=f'''# Q4 final pins e preparação Q7 — fechamento consolidado

## Resultado e escopo

Integração técnica concluída e qualificada offline para N0TOKEN-20261025-01 /10584f02e11f92c92b43cddf0d9d316f. Q4 NOT_ACTIVATED, orçamento REQUIRED_NOT_GRANTED e admissão DENY. A próxima decisão humana é aceitar o binding final exato e autorizar a única observação manual Q7; não existe registro operacional aprovado. Não foi executado Place, Prepare/preflight, Start/Stop, consumo de intent, criação de imagem/sink/arm operacional, nem mutação SCM/ACL.

O resultado Q4P original e os registros S1/S2 foram reutilizados como prova no alcance da criação/readback: evidência8432bytes ID417614100:2555904:433780 e registro397bytes ID417614100:327680:2932527. Seus conteúdos e os descritores RAW1 exatos continuam como pins distintos das identidades nativas. Os arquivos protegidos não foram alterados ou reconsultados num contexto inadequado. A revalidação por handles originais é obrigatória no futuro ciclo; igualdade de hash não substitui continuidade de objeto. S1 aceita somente N7 específica e plano de avaliação, nunca orçamento.

## Integração e revisão de fonte

Oito fontes focais/preimagens estão em source-preimages.json e APPLIED-SOURCE.patch. O header de pins seleciona somente a compilação Q4 opt-in e as duas identidades comprovadas/tamanhos/hashes; defaults e tuplas históricas permanecem. IG_N0_N7_ED11_SHA aponta à decisão específica1d6bca53..., sem herdar a N7 de03. O renderer real usa Q0/Q4/Q2/N7 e mantém vínculo do ator, token atual/TokenId/AuthId/ModifiedId, thread, challenge, processos, configuração/imagem, aprovação exata e objetos protegidos. Não carrega claims L1/P3 antigos como autoridade nova.

O launcher canônico seleciona os artefatos Q4 por perfil fixo, mantém EMPTY_DENY, rejeita Prepare em Q4 e exige o registro Q7 no caminho exclusivo ainda ausente. Os gates semânticos do mesmo stream rejeitam aprovação S2/histórica, políticas divergentes, bool/string/array mal tipados e ausência de ativação explicitamente aprovada. A entrada nativa autentica o transporte existente e exige aprovação tipada da nova identidade antes dos efeitos. A política nativa N0/BD/B0–B6/RAW1 mantém seus quatro handles e as revalidações existentes. O alias de guard do intent Q4 foi alinhado ao intent canônico de instalação, preservando o ramo legado; a consulta textual conservadora não substitui CREATE_NEW nem a verificação nativa relativa original de imagem/sink.

Ownership, A1, B1, self-token, Stop, transporte/CRT, predicado token.specific_group.enabled e serializer diagnóstico não foram relaxados. Nenhum novo owner, serviço, mutex, logger, canal ou quota. Cinco pins documentais Q4 no launcher substituem o pin N7 único anterior: quatro referências administrativas adicionais deliberadas, contabilizadas no mesmo orçamento compartilhado fresco1024, não transferidas ao orçamento128 nativo. O custo Windows/assembly/SCM/cache/stack continua parcialmente UNKNOWN; não há garantia de margem operacional.

## Builds e testes

Selected/Legacy Debug/Release: quatro builds produto completos e quatro rebuilds finais de qualificador/DLL PASS, zero warnings, /WX preservado. A imagem Selected Release foi congelada antes de derivar seu pin; qualifiers reconstruídos depois sem relink da imagem congelada. Build dirs novos, artefatos anteriores preservados. As imagens Debug/Legacy são evidência de compilação, não alvo alternativo autorizado para operação Selected Release.

- Fixture canônico estendido de pins/parser/bootstrap:41 casos por configuração,164 resultados PASS. Cobre identidades originais e divergentes, tamanho/links/reparse/diretório, troca de objeto, tupla histórica, parser e freshness.
- Função real ig_n0_plan extraída da fonte atual e compilada em quatro configurações:PASS. Com larguras escalares máximas o formato ocupou1766/2304bytes; Q0/Q4/Q2/N7 presentes e claims L1/P3 ausentes. Backend de hash foi stub de captura nesse teste: prova do renderer/bounds, não uma nova prova criptográfica. O backend produto existente foi preservado. O frame inclui buffer2304+digest32+metadados; não se declara custo total zero ou novo crédito.
-19 regressões relevantes recompiladas sem Q4 em quatro configurações:76 PASS. Incluem default EMPTY_DENY, profiles histórico/read-only, A1/Stop/UNKNOWN/retensão, runtime, replay/política, RAW1/SD e freshness; compatibilidade histórica é testada separadamente do perfil novo.
- Windows PowerShell5.1:31 gates Q7,5 testes dos pins/registro inerte reais e27 gates históricos PASS (63). Apenas AST/funções/guards puros; aprovação hipotética válida existiu somente em memória de fixture, nunca como decisão/arquivo.
- Quatro probes de argv inválido na entrada compilada:negação bootstrap exit2 antes de autenticação/observação. Nenhum --prepare/--cycle foi executado.

Dois problemas exclusivamente de execução dos fixtures foram corrigidos: diretório Legacy/Debug ainda inexistente gerava C1041 ao criar PDB; o runner agora garante o diretório de saída antes do compilador. Get-FileHash não estava disponível no contexto Python/PowerShell de um runner; o fixture passou a usar SHA256 via .NET com Dispose. Sem alteração de produto para contornar os testes, sem warnings suprimidos.

Os modelos de colocação204C/146C#/12workflow/19guards, observação de token96/correlação40 e ensaios nativos anteriores permanecem evidência reutilizada, não novas execuções. Transporte/autenticação/concorrência/console e serializer não mudaram; suas campanhas anteriores permanecem no alcance original, sem repetição equivalente. Não executar o antigo observer_stack_va_isolated_test que exibiu falha de aplicação: foi apenas compilado como dependência histórica, nunca executado aqui. Nenhuma evidência simulada é declarada medição LAB.

## Hashes finais

| Artefato | SHA256 |
|---|---|
| Launcher | `{arts['launcher']['sha256']}` |
| Qualificador Selected Release | `{arts['qualifier']['sha256']}` |
| Imagem candidata Selected Release | `{arts['service_image']['sha256']}` |
| DLL transporte Selected Release | `{arts['native_resource']['sha256']}` |
| Binding framework reutilizado | `{arts['framework_binding']['sha256']}` |
| Anexo final candidato | `{binding}` |
| Pedido Q7 candidato | `{request}` |

FINAL-ARTIFACTS.json, derived-build-artifacts.json e o manifesto final registram artefatos/configurações e fontes; o SHA do registro aprovado não existe. Uma alteração posterior exige rederivar os pins/novo pedido, sem reaproveitar aprovação antiga. O anexo protegido original é snapshot imutável de preparação: seu texto PROPOSED/hashes históricos não foi reescrito para refletir builds atuais; a nova decisão N7 e o binding final são referências separadas.

## Preservação e alcance

HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91 preservado.162 arquivos históricos de pacote/binários foram conferidos byte-exatamente contra os manifestos anteriores; recibo Q4P e pins literais de políticas conferidos. WIP preexistente preservado com preimagens das fontes/live docs alteradas; isso não é prova universal de integridade do disco/objetos kernel. Nenhum ADR accepted foi editado, nenhum commit/push/tag/worktree/clone criado. Direções futuras de recuperação pessoal permanecem diferidas e não autorizam recuperação Q4.

Histórico03 continua consumido/Start1/Stop0, first13 e serviço erro5. Q2 APPLIED_DOCUMENTARY_ONLY não é P3 nem liberação. Intent/imagem/sink/ImagePath históricos, IDs antigos não impressos UNKNOWN, reservas reais indisponíveis e quiescência global UNPROVEN permanecem. N7 mantém associação autenticada completa do console e proveniência mapeada UNPROVEN; opened-file/provenance/identidades/lifetime continuam provas obrigatórias.

## Gate real e próxima ação

Q7-OPERATION-REQUEST.DRAFT.json incorpora o anexo final, inputs reais, artefatos, N7/plano e limites. MANUAL-Q7-CANDIDATE.md explica a execução futura única e o registro ainda ausente. Solicitar decisão exata do owner para esse binding final e uma observação Q7 independente. Após aprovação, registrar a aceitação final no mecanismo designado e o registro operacional exato, calcular seu SHA literal e entregar o comando final; não executar automaticamente.

A admissão fresca poderá negar por orçamento/identidade/freshness/obrigações incompatíveis antes de efeitos. Só se passar, o teto Q7 permite um consumo novo, imagem/sink/arm exclusivos, rebind específico, um Start e no máximo um Stop seguro aceito. Recusa precoce do serviço consome a operação como resultado diagnóstico válido. Não há retry até RUNNING.

Q7 poderá medir o novo SID específico, presença e raw ENABLED0x4/DEFAULT0x2/DENY_ONLY0x10/restrições/query errors; correlacionar PID/criação, máscaras/ordem/lifecycle e fechamento; e medir recursos administrativos/nativos/picos amostrados contemporâneos. Não reconstruirá token/masks históricos nem provará pico não amostrado ou quiescência global. ED11 exige ainda a evidência nativa e gates de ciclo/Stop/lifetimes/recursos aplicáveis; Phase6 também depende dos gates de outros componentes. CURRENT/signature/authoritative recovery continuam DENY.

CURRENT_TASK_COMPLETED=YES
FINAL_PINS=INTEGRATED_OFFLINE_QUALIFIED
N7=ACCEPTED_SPECIFIC_SCOPE
Q4=NOT_ACTIVATED
Q7=PREPARED_NOT_APPROVED_NOT_EXECUTED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_WITH_SCOPED_PREIMAGES_AND_HISTORICAL_HASH_CHECKS
FIRST_UNRESOLVED_DIVERGENCE=EXACT_FINAL_BINDING_AND_Q7_OWNER_DECISION_ABSENT;FRESH_BUDGET_AND_NEW_SERVICE_TOKEN_NOT_OBSERVED
NEXT=OWNER_EXACT_FINAL_Q4_BINDING_AND_MANUAL_Q7_DECISION
'''
(p/'CONSOLIDATED-CLOSEOUT.md').write_text(report,encoding='utf-8')
# Live state updates; historical sections remain untouched.
live=[r/'docs/capabilities/canonical-phase6-consolidation.md',p.parent/'INTEGRATED-CLOSEOUT.md',p.parent/'ED11_GATE_MATRIX.csv']
for f in live:
 dest=p/'live-preimages'/f.name;dest.parent.mkdir(exist_ok=True)
 if not dest.exists():shutil.copyfile(f,dest)
paragraph=f'\n\n## Q4 final pins integrated; Q7 prepared — 2026-10-25\n\nSee docs/integration/n0start03-integrated-closeout-20261024/q4-final-pins-and-q7-readiness-20261025/CONSOLIDATED-CLOSEOUT.md. Actual protected IDs8432bytes417614100:2555904:433780 /397bytes417614100:327680:2932527 and specific S1 N7 are integrated in the opt-in canonical Q4 profile. Four product/final qualifier builds zero warnings;164 focused C outcomes,4 actual-renderer fixtures,76 historical regressions,63 PowerShell gates and4 invalid-entry probes PASS. Old packages/binaries and placement receipt hash-checked unchanged. Exact Q7 request SHA256 {request}, final annex {binding}; neither is an owner approval. Default EMPTY_DENY; Q4 NOT_ACTIVATED; independent budget NOT_GRANTED; no new native observation, Place, intent, Start/Stop or SCM/ACL mutation. Q2 documentary only; real reservations/UNKNOWN retained, global quiescence UNPROVEN. ED11/Phase6 PARTIAL and productive authorities DENY. Next: exact final binding/Q7 owner decision, then one manual operation subject to fresh gates.\n'
for f in live[:2]:
 with f.open('a',encoding='utf-8') as out:out.write(paragraph)
row=['Q4_FINAL_PINS_Q7_READINESS','Final independent protected-object and policy binding','QUALIFIED_OFFLINE_Q7_PREPARED_NOT_APPROVED','q4-final-pins-and-q7-readiness-20261025/CONSOLIDATED-CLOSEOUT.md; actual protected IDs and N7 integrated; four builds zero warnings;164 C/4 renderer/76 regression/63 PS/4 invalid probes PASS','No Q4 activation or fresh independent budget/new token observation; sampled and historical UNKNOWN retained','Owner exact final Q4 annex and manual Q7 decision; then one fresh gated independent observation','YES exact new Q7 owner approval and fresh native gates','Any identity RAW1 hash freshness resource or controlled ownership uncertainty => DENY retain no retry','PENDING_Q7_NATIVE']
with live[2].open('a',encoding='utf-8',newline='') as out:csv.writer(out).writerow(row)
print('CONSOLIDATED_REPORT_MANUAL_PROCEDURE_AND_LIVE_MATRIX_WRITTEN; NO_AUTHORITY_ACTIVATED')
