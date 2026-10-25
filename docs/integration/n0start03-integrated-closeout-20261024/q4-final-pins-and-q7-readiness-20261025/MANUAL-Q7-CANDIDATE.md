# Q7 — procedimento candidato, ainda sem aprovação

Operação N0TOKEN-20261025-01, identidade10584f02e11f92c92b43cddf0d9d316f. Q4 NOT_ACTIVATED; orçamento NOT_GRANTED. Nenhum comando operacional foi executado por este pacote.

Pedido exato: Q7-OPERATION-REQUEST.DRAFT.json, SHA256 `cf88f42ad86700ca1eb7891c834d8c9521f1c30dfc5a55317cb6e4665ba3db58`.
Anexo final candidato: Q4-FINAL-BINDING.ANNEX.PROPOSED.json, SHA256 `0a841f4ec3d261ccdf24e71d849602c88cb9d477ec03ea0c89d1391bb9361ca9`.

A decisão futura deve aprovar expressamente o binding final Q4 e uma única execução manual Q7 nos limites desse pedido. Registrar a decisão normativa final pelo mecanismo existente antes do uso; preservar o anexo candidato. Registrar separadamente a aprovação operacional exata, sem sobrescrever decisões anteriores. Q7-APPROVAL-RECORD.INERT-TEMPLATE.json é apenas esquema negado: não é aprovação, não pode ser renomeado ou usado como autorização. A aprovação real não existe. Seu SHA256 não pode ser antecipado.

Após decisão válida, revisar novamente todos os hashes/identidades e o registro; serializar a aprovação tipada UTF8 sem BOM/LF, começando pelas propriedades status/operation/attempt em ordem e com os valores exigidos pela superfície nativa existente. Somente o registro aprovado terá status EXACT_MANUAL_POSITIVE_OWNER_APPROVED e q7_activation_authorized=true; isso requer decisão humana, não edição automática do template. Incluir as referências exatas do pedido, do binding final e de todas as políticas/artefatos. Calcular o SHA256 integral do novo registro EXACT-Q7-APPROVED.json e fornecer seu literal no comando definitivo. Nenhuma aprovação S2 ou operacional histórica é reutilizável.

Usar um único Windows PowerShell5.1 x64 já elevado, operador autorizado, mesmo host/boot, console interativo original, sem impersonation ou redirecionamento. Não abrir outro launcher/owner. Os gates nativos revalidam os objetos originais, os pins RAW1, SID/identidade, exclusividade, orçamento independente128/1024/quatro papéis, freshness60s/1s, headroom e ausência de Stop/freeze. Divergência cancela; não repinar para produzir PASS.

Forma do comando futuro (não executável enquanto o campo entre sinais de menor/maior não for substituído pelo hash real da aprovação):

```text
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -Profile Q4TokenDiagnostic -ExpectedScriptSha256 a78f6c573e65322a34cafc32dd67b08ec6d1fc964552a18ee2c65ab00fed505d -ExactApprovalSha256 <SHA256_LITERAL_DO_NOVO_REGISTRO_APROVADO>
```

Não executar Prepare separadamente. A admissão fresca ocorre dentro do ciclo exato, antes dos efeitos. A primeira confirmação usa START_N0_SINGLE_LAB N0TOKEN-20261025-01 10584f02e11f92c92b43cddf0d9d316f e o PID/criação factual do auxiliar exibido; BIND_BD_READ_ONLY inclui o qualificador original/criação/challenge factual, não valores históricos. Última confirmação: INSTRUMENT_SCM_LAB 10584f02e11f92c92b43cddf0d9d316f. Mesmo o nome histórico de BIND_BD_READ_ONLY não concede autoridade: vale o plano positivo, sua identidade e a aprovação Q7 específica. Escape solicita Stop entre chamadas síncronas. F10 retorna ao mesmo console somente após fechamento factual; não concede retry.

Máximo condicionado: um consumo canônico novo, imagem/sink exclusivos e arm incorporado, rebind explícito do mesmo serviço, um Start e no máximo um Stop seguro aceito pelo estado real. Não restaurar ImagePath automaticamente. Recusa precoce do serviço é resultado diagnóstico válido; não repetir até RUNNING. Em falha parcial/UNKNOWN, preservar os objetos e referências, primeira falha e dependências; não excluir/rollback/kill/forçar mutex/rearmar. Não fechar novamente handle incerto.

Preservar saída integral original, IG_FINAL, token/SID/atributos/erros, masks esperado/visto/ausente e PID/criação/ordem/wait original, métricas e picos amostrados, recibos de fechamento/retensão e diagnóstico protegido. Retornar a saída ao projeto para reconciliação, sem repetir a operação. A consulta protegida posterior, se necessária, deve ser read-only no contexto autorizado; nenhum segundo recibo de Place é criado.

A nova evidência não recuperará atributos do token histórico03, IDs históricos ausentes, masks não impressas ou quiescência global. ED11/Phase6 seguem PARTIAL até os gates próprios; CURRENT/signature/recovery permanecem DENY.
