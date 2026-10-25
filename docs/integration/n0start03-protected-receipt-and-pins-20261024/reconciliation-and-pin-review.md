# N0START03 — recibo reconciliado; pins candidatos, leitura atual pendente

HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91. Autoridades aplicáveis e preimagens conferidas por hash; decisões accepted intactas. Nenhum Place repetido, nenhum pin operacional ativado.

## Recibo original

Lido o arquivo real docs/integration/n0start03-protected-input-placement-preparation-20261024/manual-N2P-20261024-03.txt, SHA92574a6f1966358d054daa50484c7bd855d8a2a4fb4d5ed42edca938d231a1e9. Não substituído pelo prompt, não modificado, nenhum novo recibo de colocação criado. Quatro consultas relativas ao pai original retornam STATUS_OBJECT_NAME_NOT_FOUND c0000034; IOSB c0000001 permaneceu o sentinel inicial, não é sucesso nem código do primeiro erro: o gate aceitou o retorno direto inequívoco da chamada síncrona. As consultas não reservavam nomes. A criação final usou CREATE_NEW, explicit SD, com revalidações/readback/flush pelo mesmo mecanismo pinado autorizado.

A evidência criada tem ID417614100:2883584:2416818/SHA15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7; registroV2 ID417614100:1966080:2672522/SHA5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050. Tamanhos7573/397 vêm dos candidatos e do código pinado, não há linha de tamanho separada no recibo. Os quatro RAW base64 foram decodificados e coincidem BYTE A BYTE com o descritor aprovado152bytes/SHA0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814, owner operador1108/groupBA, controle9004, ACEs originais SY→BA→operador, sem normalização. SACL/labels não consultados UNKNOWN.

Recibo registra OWNED_REFERENCES_CLOSED, NATIVE_FIRST_ERROR=NONE, NATIVE_OWNED_CLOSED=True, RETAINED=False. A confirmação ADMIN_RECEIPT_CLOSE=CONFIRMED/EXIT0 está na saída manual fornecida pelo owner; o próprio recibo não pode registrar depois de fechar a si mesmo. Leitura do recibo desta revisão abriu/fechou somente sua própria referência. Isso não prova quiescência global nem liberação de cargas históricas.

## Primeira prova ainda ausente

A tarefa exige nova comparação read-only com os OBJETOS REAIS. Uma abertura read-only da raiz foi tentada uma única vez: Win32=5, SID coletor1003, Administrators não habilitado, opened=false, sem handle adquirido/retido. Não tentamos ACL/UAC/remediação/escalada nem repetir. Não há nova observação do conteúdo/SD/ID dos dois inputs. Assim, colocação histórica é comprovada pelo recibo, mas revalidação atual permanece PENDENTE e CURRENT_TASK_COMPLETED=NO.

## Menor procedimento manual read-only

Dois artefatos de revisão neste pacote adaptam SOMENTE os alvos/IDs/tamanhos/hashes e recibo do leitor anterior qualificado; helper nativo/algoritmo de abertura/identidade/RAW1/content/fechamento são reutilizados. Não criam novo owner/autoridade. Diffs fornecidos. Não executar Place nem leitor histórico para alegar verificação03.

Em um único Windows PowerShell5.1 x64 já elevado, interativo do operador1108, sem classes do leitor carregadas, executar uma vez, somente neste contexto autorizado:

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-protected-receipt-and-pins-20261024\successor-input-readonly.ps1" -Mode Verify -ExpectedScriptSha256 6dfd8a51a9ab70dfcf01b995b2186fe7eba7e39b5984500e02fedec6eb67ea4a
```

Script confere próprioSHA/dependências/recibo, SID/Administrators habilitado/noimpersonation, hostSHA2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff/boot236441/start2026-10-23T18:36:34.5000000Z. Leitor abre os5ancestrais+2arquivos03 shareREAD, OPEN_EXISTING/no-reparse; confere IDs/tipo/path/RAW1 pelo handle original, tamanho/links/contentSHA e revalida; fecha em ordem reversa e retém UNKNOWN. Resultado esperado 7 N0_INPUT_READ_VERIFIED, FIRST_ERROR=NONE, RETAINED=False, EXIT0 e OWN_REFERENCES_CLOSED. Divergência/erro/UNKNOWN: STOP, não repetir/corrigir/fechar owner retido. Enviar saída integral. Esse procedimento não é preflight, não adquire admissão, não modifica SCM/ACL/inputs.

## Matriz, N7 e recursos

prepared-pin-matrix.json e context-pins.NOT-ACTIVATED.txt são documentos candidatos, NÃO headers operacionais. Vinculam tentativa03/operação03/serviço existente/políticas L/H/L1/N/RAW1/B/recorder/runtime/qualificador/launcher/configuração/hostboot/arquivos+recibo. IDs/hashes dos protegidos derivados do recibo, pendingcurrentread; PID/token/métricas/desafio/plano final futuros nulos. Hostboot/serviço/configuração/proveniência estática são expected/históricos, não fatos frescos de admissão. Fonte scm-n0-context-pins.h continua vazia/zero DENY.

N7-REVIEW-NOT-GRANTED.json encadeia as evidências e o recibo, mas não concede aprovação. Identidade/lifetime/token/sessão do auxiliar e aceitação operacional específica03 permanecem ausentes. Não autentica console nem imagem mapeada. H0–H9 cobre somente resíduo opaco histórico no escopo aceito, não recurso controlado incerto. P3 histórico preservado; intent antigo consumido/efeitos/UNKNOWN/reservas reais indisponíveis permanecem. Nenhum refund.

Orçamento independente REQUIRED_NOT_GRANTED; quatro papéis,128handles nativos,1024 administrativos, orçamento administrativo compartilhado, freshness60s1s, headroom5/9GiB e créditosNONE_GRANTED preservados. Amostras antigas/fixture do console não são crédito ou admissão fresca.

## Testes

Compilação do leitor C# na PS5.1 em memória warning4/TreatWarningsAsErrors PASS; hash do script divergente DENY antes de abrir objetos. Modelos Busy115/B+D85/RAW1 13 casos executados nas4configurações =12suites PASS. Cobrem identidade/hash/SD/substituição/replay/boot/token/Stop/retencão/UNKNOWN/close nos modelos existentes, não prova positiva dos inputs reais. Não executado Verify nativo privilegiado nesta sessão; somente tentativa de raiz negada acima. Nenhuma fonte C/C++ alterada, builds não requeridos. Fonte produtiva/launcher/pins e decisões intactos; apenas novos artefatos de revisão e capability append.

CURRENT_TASK_COMPLETED=NO
PROTECTED_INPUTS=PLACED_AND_VERIFIED_HISTORICAL_CURRENT_READ_PENDING
RECEIPT_RECONCILIATION=PASS_ORIGINAL_FILE_AND_MANUAL_CLOSE_OUTPUT
PINS=PREPARED_NOT_ACTIVATED
N7_OPERATIONAL_ACCEPTANCE=NOT_GRANTED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=CURRENT_PROTECTED_OBJECT_READ_DENIED_CONTEXT_WIN32_5
NEXT=N0START03_NATIVE_ADMISSION_PREPARATION_AFTER_AUTHORIZED_READONLY_RESULT
