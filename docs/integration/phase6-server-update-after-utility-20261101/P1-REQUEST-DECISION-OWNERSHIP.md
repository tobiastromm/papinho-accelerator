# P1 — ownership entre pedido e guarda da decisão

2026-11-02 (America/Sao_Paulo). main / HEAD
f341985924d29870d8ff627ed37c8a0874ad6469. Revisão exclusivamente offline.
Fonte factual P1-CHAIN-SESSION-RESULT.json/reconciliação; ato608b6...4441 consumido.

**Conclusão:** não há redução segura demonstrada dos125handles por fechamento
adicional de referências controladas pelo PapinhoAccelerator nesse intervalo.
Não foi demonstrado vazamento. Nenhum código/guarda/quota foi alterado.
O original do pedido é necessário e permanece aberto; os temporários próprios
de hash/descritor já foram liberados antes da query que negou a decisão.

## Contrato de simultaneidade, antes de qualquer sugestão de fechamento

OHI accepted incorpora OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md: seção Fonte exige
objetos originais protegidos, identidade/conteúdo/SD/proveniência revalidados;
seção Handoff mantém originais da confirmação ao resultado terminal. Digest ou
reabertura pelo mesmo nome não substituem referência original contínua.
P0–P7 accepted incorpora EXISTING-PROGRAMFILES-ORIGINAL-BINDING.CANDIDATE.md:
P2 exige aquisição/retenção de ancestrais/imagem; P4 continuidade; P5 permite
transferência/encerramento só com ownership/lifetime/continuidade provados,
guarda124 antes de cada referência e contagem de todas as sobreposições.

Isso NÃO significa que todo buffer/objeto BCrypt ou alias físico deva permanecer
aberto até o fim. Temporários podem terminar quando sua função termina. Também
não congela toda duplicação da implementação como mínimo normativo universal.
Uma transferência comprovada para outra referência do MESMO objeto pode
encerrar a predecessora sem romper custódia. No código isso ocorre depois de
session_acquire_originals/capture/contents, seguido de scqc_close_raw.

No ponto da falha há APENAS o original do pedido como referência própria desse
arquivo, sem sucessora original qualificada. Fechá-lo e substituir por ID/hash/
nome ou reabri-lo depois não é uma transferência demonstrada. Tampouco há
duplicata, reader ou parent recém-adquirido para escolher qual dispensar.
Não se provou uma reorganização que reduzisse o total quando a admissão completa
exige novamente os originais e seus custos, antes dos efeitos.

Não mover hashing/QueryDosDevice para depois da guarda só para esconder seu
custo; não antecipar guard e ignorar recursos ainda obrigatórios. P5 veta isso.
G0–G9/QUERY-G3 mantém quotas/UNKNOWN apesar do risco residual de custos opacos.

## Intervalo exato, sem misturar execuções

Q0=consulta antes de NtCreateFile(request0): passou (<124), valor exato não
preservado. Depois: GetModuleHandleW/GetProcAddress, abertura request0,
GetFileType/GetFileInformationByHandle, GetSecurityInfo, validação/hash do SD,
destruição do hash/close provider, LocalFree, próxima iteração/Stop,
GetDriveTypeW e QueryDosDeviceW(nome da decisão), Q1=count125/guard124 nega.
A decisão NÃO é aberta. Imagem/cadeia/SCM e confirmadores não foram alcançados.
Não subtrair113 de outra sessão. O crescimento líquido entre Q0 e Q1 é>=2
porque Q0<=123, mas esse limite não é inventário de handles individuais.

## Inventário por ownership e lifetime

| Recurso/API | Owner e natureza | Durante intervalo / em Q1 | Encerramento correto e possibilidade de redução |
|---|---|---|---|
| n->scq_cold_raw[0], NtCreateFile | Owner canônico; HANDLE original do pedido, shareREAD, direitos00120089 | Um arquivo explicitamente adquirido e ainda retido em Q1 | scqc_close_raw só após transferência validada ou falha síncrona conhecida. Não fechar antes da decisão por conveniência |
| scq_cold_raw[1]/[2] | Decisão/imagem originais futuras | NULL/não adquiridos nesse caminho | Nenhuma economia possível nesse ponto |
| Imagem_chain.directory[]/image | Cadeia independente P1, owner/parent originais | Ainda não adquiridos: scqi_chain_capture é posterior às três aberturas e demais provas | Não são a origem demonstrada deste crescimento; cadeia futura continua obrigatória |
| scq_inputs.file[] | Duplicatas próprias por DuplicateHandle após aquisição completa | Ainda não criadas; session_acquire_originals não alcançado | Fechar duplicatas futuras não reduz125 atual |
| scq_inputs.reader[] | Readers próprios por ReOpenFile/cursors independentes | Ainda não abertos | Otimização de sobreposição futura não resolve esta guarda pré-decisão |
| checking, GetSecurityInfo | Memória alocada do descritor; PSECURITY_DESCRIPTOR, não HANDLE de arquivo | Capturada temporariamente; LocalFree passou e checkingNULL antes de Q1 | Já liberada na ordem correta; não CloseHandle desse ponteiro nem contar como um kernel handle dispensável |
| content_hash.algorithm | Referência opaca BCRYPT_ALG_HANDLE pertencente ao PAPACC_STORE_HASH do packet | Abre no hash do SD; BCryptCloseAlgorithmProvider obrigatório no sucesso; NULL em Q1 | Já fechada; não fechar novamente nem alegar que isso descarrega todos os internos do provider |
| content_hash.hash | Referência opaca BCRYPT_HASH_HANDLE pertencente ao mesmo packet | BCryptCreateHash; DestroyHash antes de fechar provider; NULL em Q1 | Já destruída. Falha de cleanup mantém retained e impede alcançar Q1 por esse caminho |
| BYTE digest[32], info[0], UNICODE_STRING/OBJECT_ATTRIBUTES/IO_STATUS_BLOCK e buffers de nome | Memória/dados no frame/packet do owner | Sem novo HANDLE por sua representação | Não geram fechamento de kernel handle; não usar sizeof como economia de handles |
| HMODULE de GetModuleHandleW(ntdll), endereço de GetProcAddress | Referência emprestada ao módulo já carregado; ponteiro de função | Lookup antes da primeira abertura | GetModuleHandle não incrementa refcount. Não CloseHandle/FreeLibrary; não é um arquivo original adicional |
| GetCurrentProcess/GetCurrentThreadId/GetDriveType/QueryDosDevice | Pseudohandle/valores/API com buffer de saída; nenhum novo HANDLE retornado ao caller neste trecho | Internals, caches ou inicialização Windows podem usar recursos, mas não estão inventariados individualmente | Não fechar handles internos. Retorno string/valor não transfere ownership desses recursos |
| Token original, console/CRT/owner preexistentes | Token próprio e referências herdadas/emprestadas têm owners/lifetimes distintos | Já existiam antes de Q0; contados no total completo | Não atribuir sua origem a este trecho nem fechar token/stdio para liberar margem |
| SCM/peer/eventos/pipe/carrier | Owners das fases posteriores | Não adquiridos nesse intervalo | Não são handles auxiliares que possam ser liberados aqui |

Os dois objetos CNG gerenciados não equivalem necessariamente a dois executive
handles contados por GetProcessHandleCount. Não calcular125-2 como economia
possível, especialmente porque ambos já foram liberados em Q1.

## Prova do cleanup e limitações

papacc_store_hash só publica digest com sucesso depois de BCryptDestroyHash e
BCryptCloseAlgorithmProvider bem-sucedidos; zera os slots e active. Falha marca
retained/retorno0. scqc_plan então retorna erro de SD antes da próxima iteração.
LocalFree nãoNULL também marca unresolved/erro antes de Q1. A query125 alcançada
é incompatível com temporários próprios ainda ativos por falha nesses cleanups,
sob o código qualificado. Isso NÃO prova que DLL/provider/kernel não mantenham
cache, internals ou outros recursos próprios após liberar o objeto público.

GetFileType/info operam sobre o mesmo raw0, sem API própria de diretório ou
duplicação no trecho. Não há CloseHandle faltante identificado por inspeção.
No fail-path atual, scqc_close_raw tenta os originais síncronos uma vez, ordem
imagem/pedido/decisão; falha preserva unresolved. Não tentar essa limpeza agora
na coorte consumida. Ausência de marcador positivo de close do controlador
na saída continua não resolvida por inferência; o close documental é outra coorte.

Fontes primárias consultadas:
- [GetSecurityInfo](https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo): retorna cópia de SD, liberada com LocalFree.
- [GetModuleHandleW](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew): referência emprestada, sem incremento; FreeLibrary não é fechamento legítimo dela.
- [BCryptOpenAlgorithmProvider](https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptopenalgorithmprovider): inicializa provider e requer BCryptCloseAlgorithmProvider; seleção default não demonstra internos/contagem.
- [GetProcessHandleCount](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesshandlecount): total de executive handles do processo, não inventário de todos os tipos abstratos de recurso.

Não selecionar outro provider, pseudo-handle/one-shot, hash alternativo ou
warm-up antecipado como correção presumida. Não há prova de que tais mudanças
reduzam esse125 e elas podem mudar suporte/seleção de provider/lifetime.

## Testes afetados e primeira lacuna

Nenhum defeito funcional demonstrado; nenhum código alterado. Não executados
builds/CTests inalterados nem real CNG/token/arquivo/coleta nesta revisão.
Foram auditados os testes existentes store_hash_lifetime_win32_test.c (sucesso
somente após ambos cleanups; fail/Stop/reentrância/retained), store_hash_owner
(UNKNOWN não liberado por owner), scq_cold_open (124/128/query/firstcause),
scq_cold_controller (handoff/close) e personal_scq_inputs (duplicatas/readers/
continuidade/close). Não declarar novo PASS de campanha que não foi executada.

Primeira lacuna factual: valor Q0 e distribuição das variações pelos grupos
efetivamente executados. Não é mais necessário localizar o erro1816: foi
guard124 com125 confirmado. Tampouco falta uma política nova de simultaneidade.
Falta saber onde aparece o saldo não explicado pelo único HANDLE próprio novo.

A menor prova que evita novo total inconclusivo está delimitada em
P1-REQUEST-DECISION-MINIMAL-OBSERVATION.SPEC.md: uma observação read-only focal,
em percurso diagnóstico existente, com nove cortes e flags dos slots próprios,
sem decisão/imagem/SCM/endpoint/Start. Nenhuma coleta ou implementação dessa
extensão foi executada nesta tarefa. Um observador standalone de68handles não
é evidência equivalente ao controlador; a disponibilização deve preservar
startup/P1/APIs relevantes e explicitar custo do observador. Não instalar uma
imagem só para instrumentação ou repetir a sessão consumida.

## Estado final

Redução segura demonstrada: **nenhuma adicional**. Temporários já encerrados;
pedido original continua necessário. Não há decisão normativa nova/ADR a pedir.
Primeiro gate: qualificar offline a disponibilização dessa prova no observador
existente e, só com artefato/procedimento exatos, solicitar autorização específica.
Se o comportamento necessário não puder ser reproduzido, reportar limitação,
não obter outra coleta global que não discrimine o intervalo.

Próximo prompt NEXT-P1-REQUEST-DECISION-FOCAL-OBSERVATION-PREPARATION.md.
Sem coleta/UPDATE/E/Q/S/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/cleanup/
refund/recuperação. Guard124/cap128/G3/DENY/UNKNOWN histórico/OS, originais,
EXEs/históricos/pins/atos consumidos/HEAD/WIP/reservas/fixture/PST0.6.4 intactos.
