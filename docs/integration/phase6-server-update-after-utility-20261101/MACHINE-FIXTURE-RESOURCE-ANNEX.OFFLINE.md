# Anexo técnico offline — recursos da preparação MACHINE TEST_ONLY

2026-11-03. Evidência técnica, não decisão normativa, capacidade nativa, aprovação de fixture, UPDATE ou sessão. K0–K9/K7 exige qualificação própria antes da operação. Não se importa o envelope READ nem se altera o piso de 3 GiB aprovado para READ.

## Custo conhecido e lifetime

O mesmo root canônico contém PAPACC_MACHINE_FIXTURE_CONTROL; o mesmo PAPACC_CA_KEY mantém provider/key e uma reabertura positiva, sem novo owner. O processo usa o console original e nenhum worker, pipe, evento ou SCM nesta preparação.

| Recurso | Ownership / retenção |
|---|---|
| Token administrativo original | Mesmo pending owner; adquirido e revalidado pelas primitivas existentes; até fechamento canônico. Tokens transitórios da validação CA fecham pelo mesmo owner. |
| Âncora, ancestrais e imagem | image_chain existente: até oito diretórios e um arquivo; referências originais retidas, OBJ_DONT_REPARSE, compartilhamento somente leitura, imagem ligada ao processo por observação nativa. Não há atestação das páginas mapeadas. |
| Pedido/decisão e duas DLLs | Quatro referências novas, somente leitura, abertas relativamente ao parent retido. Sem rename, escrita, substituição ou alteração de DACL. Imagem reference[2] é empréstimo da cadeia, nunca um segundo close. |
| SDs / scratch | Payloads limitados a 65.536 bytes. Reserva conservadora de quinze slots inclui backing retido e temporários pequenos da criação; nenhum scratch pendente é liberado por conveniência. Overhead de alocador/Windows permanece opaco. |
| SHA256 de DATA | Cápsula portátil já existente para arquivo/SD/plano/blob público. Sem novo provider de hash nesta integração do produto; a implementação matemática não se torna autoridade. O backend da chave continua Microsoft Software KSP. |
| Provider / chave / reabertura | Handles abstratos CNG do mesmo owner, não equiparados a uma quantidade provada de handles de kernel. Provider e chave original permanecem durante o controle positivo; reabertura fecha uma vez. Não há delete. |
| Stacks nomeadas | Reserva adicional conservadora de 65.536 bytes para os frames síncronos controlados. Não é limite nativo de pilha/provider/kernel nem prova de working set. |
| Console / módulos | GetStdHandle e GetModuleHandle são empréstimos; não são fechados pela preparação. Nenhuma nova janela é lançada pelo código. |

PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA = 15 × 65.536 + 65.536 = 1.048.576 bytes. Getter do launcher soma o grafo existente, storage do serviço, LONG de estado e esse extra; o valor não vem da CLI.

Selected: ROOT 380.136; grafo conhecido 2.007.184; preparação conhecida 3.070.732; quota 3.145.728; margem aritmética 74.996 bytes. Isso não cobre por presunção custos opacos e não prova suficiência Windows.

Legacy: ROOT 211.568; grafo conhecido 1.838.616; preparação conhecida 2.902.164 excede quota 2.097.152. Entrada real da preparação permanece DENY antes de efeitos. Os modelos de componentes não são uma licença para substituir o censo real do launcher. Nenhuma quota Legacy é ampliada.

## Checks conservadores atualmente implementados

A preparação, separada de READ, tem checks necessários conservadores: guarda124/cap128, private/working growth1.994.944, físico7.239.172.096, commit9.823.326.208, freshness100ms e deadline60.000ms não renovável. Antes de cada API CNG protegida, o mesmo observador de recursos existente é consultado; indisponibilidade, Stop, excesso, overflow ou prazo expirado negam. Cleanup controlado continua uma única vez após erro; close incerto retém.

Esses checks não constituem envelope operacional novo aceito, orçamento nativo suficiente ou aprovação antecipada. Não importar automaticamente 188/192 ou 3 GiB de K7. Se a preparação operacional depender de outra tolerância, localizar a obrigação aplicável e submeter somente a decisão focal necessária antes de implementar mudança dependente. Não se presume o estado atual de memória/handles deste host a partir de observações antigas.

Não há teto nativo provado de custo/latência de NCrypt, RNG, alocador ou kernel. Uma API síncrona pode exceder o prazo antes de devolver controle. O prazo observado entre chamadas não promete cancelamento forçado. Ausência de suficiência/assurance exigida mantém DENY; risco opaco não vira zero.

## Gate operacional ainda aberto

Antes de qualquer execução: imagem final completa/qualificada, executor protegido original e contexto; dois originais novos e decisão de criação real independente; anexo próprio e assurance aplicável; integridade/proveniência/identidade/SD/ancestrais/Stop/deadline frescos. Consumo antes da criação; colisão não é adotada. Nome CSPRNG e identidade pública futuros somente pelo produtor real. Falha parcial mantém objeto persistente e UNKNOWN; nenhum retry/rollback/delete/refund.

Nenhuma coleta, fixture, atualização, Start/Stop ou outro efeito operacional foi executado. Ver MACHINE-FIXTURE-AND-READ-SOURCE-OFFLINE-QUALIFICATION.json para hashes e limites dos modelos.


## Snapshot posterior — fechamento READ/MACHINE 2026-11-03

Os números anteriores são o snapshot intermediário preservado. Censo final desta integração: Selected ROOT380408/KNOWN2007456/preparação3071004/quota3145728/margem74724; Legacy ROOT211840/KNOWN1838888/preparação2902436/quota2097152. Legacy continua DENY para preparação; nenhum limite foi ampliado. Ver READ-MACHINE-END-TO-END-OFFLINE-QUALIFICATION.json e READ-MACHINE-END-TO-END-CLOSURE-REPORT.md. Os checks conservadores e a necessidade de anexo/assurance/decisão operacional próprios permanecem; K7 READ não se aplica à criação. Nenhuma suficiência nativa ou execução da fixture demonstrada.
