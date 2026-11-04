# Correção focal anchor/child — fechamento técnico

Task PACC-PHASE6-ANCHOR-CHILD-GUARD-CORRECTION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Trabalho técnico offline autorizado pelo proprietário; nenhuma aprovação operacional nova. HEAD/WIP, históricos/pins e PST0.6.4 preservados.

## Predicado corrigido

A classificação estrutural estrita continua rejeitando a ACE Authenticated Users ALLOW 0x4. O caso especializado só existe na composição privada do staging para a raiz de volume retida, quando o primeiro filho original foi adquirido relativamente a ela e está sob custódia do mesmo owner.

Para a exceção da âncora, são exigidos simultaneamente:

1. Owner/invocação/thread originais vivos, fase admitida, contexto nativo vivo e root handle original; sem Stop ou referência de filho criado/adotado/alias da raiz.
2. Filho existente adquirido relativamente à raiz pelo fluxo nativo, identidade original verificada, diretório sem reparse, DACL válida e protegida, classificação estrita de ancestral válida.
3. Owner/DACL/ACEs válidos na raiz. A única permissão de escrita não confiável admitida nessa composição é a ACE exata ALLOW para Authenticated Users, máscara somente FILE_ADD_SUBDIRECTORY (`0x4`), flags exatamente zero. Não dispensa máscara combinada com leitura, generic rights ou ACE herdada/propagável nesse caso especializado.
4. Nenhum outro direito aplicável de escrita/substituição para trustees não confiáveis. DELETE, DELETE_CHILD, WRITE_DAC, WRITE_OWNER, ADD_FILE, WRITE_EA, WRITE_ATTRIBUTES, GENERIC_WRITE/ALL e tipos não compreendidos continuam negados. Não se subtrai DENY de ALLOW para obter aceitação.

Não há remoção global de 0x4, alteração de ACL Windows, novo trustee genérico, callback permissivo, setter ou autoridade baseada em hash/fixture. Raízes que já passam na guarda estrita mantêm o comportamento anterior; a nova condição de filho protegido é obrigatória para o caso AU/0x4 especializado. Intermediários, parent criador e objetos novos permanecem estritos. A proteção aplicada aos novos objetos não muda.

## Callers e continuidade

- `server_staging_descriptor_win32.inc`: classificador estrito mantém sua API privada/comportamento; função de política só recebe modo anchor da composição que verifica o filho protegido. `staging_anchor_classify` não é uma API de concessão ou acesso externo.
- `server_launcher_qualification_staging_win32.inc`: `staging_anchor_descriptor` associa o descritor fresco ao primeiro filho original do packet. `staging_acl` aplica essa relação somente no índice 0 (primeiro componente existente após a raiz); os demais índices conservam a política estrita.
- A captura valida o par ao adquirir o primeiro filho. Na localização 5, revalida identidade/bytes da raiz pelo adapter nativo existente e repete identidade/descritor do filho.
- `staging_current` continua revalidando token/raiz e todos os objetos. `staging_limited_refs`, chamado por `staging_limited_refresh`, repete a mesma relação antes dos efeitos selecionados. Portanto criação/write/flush não dependem apenas da aceitação inicial.

O root descriptor original permanece retido; o adapter compara identidade e bytes do mesmo objeto. O filho é reconsultado pelo handle original e comparado por identidade/hash. Não se reabre path para substituir originalidade. A leitura do descritor do filho ocupa o scratch existente e é liberada antes do próximo uso; fechamento incerto preserva cohort/UNKNOWN, sem retry ou refund.

## Fundamentação e evidência

F3 exige parent por identidade nativa retida, proteção contínua, revalidação e criação exclusiva; D5 exige proteção dos ancestrais e ausência de reparse/substituição; B3.1/B4 preservam essas garantias e não autorizam adoção. Nenhuma cláusula foi alterada ou aceita por inferência. A correção implementa a distinção entre criar outro filho na âncora e modificar/substituir o filho protegido; não declara 0x4 universalmente seguro. Não foi identificado conflito normativo dependente desta mudança.

Fonte factual reutilizada: `../phase6-root-chain-procedure-20261031/NATIVE-RESULT-RECONCILIATION.md` e OWNER-CHAIN-RESULT.txt. A coleta manual mostrou raiz AU/0x4, ProgramFiles protegido/strict PASS, Papinho primeiro ausente, close conhecido/retained0/effects0. É saída fornecida pelo proprietário, não nova atestação independente. Não foi repetida. Ela qualifica esses fatos naquela captura; o estado corrente durante um staging futuro exige os gates frescos originais.

A autoridade administrativa legítima do host continua no limite F1. Não se promete proteção contra OS/admin comprometido, ausência universal de handles anteriores, mudanças entre amostras ou cancelamento instantâneo de I/O. Privilégio/elevação e sucesso desta guarda não concedem autorização de staging.

## Recursos

Nenhum campo/objeto estático, pool, handle ou alocação heap adicional foi introduzido pelo predicado. São usados os mesmos root/packet/scratch. A captura acrescenta uma revalidação sequencial dos originais, sem sobrepor novos descritores além dos slots existentes.

O censo de automáticos do staging soma conservadoramente parâmetros/locais do binding, classificador do par, thunk estrito e chamada de política: `14*sizeof(void*) + 5*sizeof(DWORD) + sizeof(SECURITY_DESCRIPTOR_CONTROL) + 3*sizeof(int)`, 146 bytes em x64, sem descontar frames antigos. Não é ceiling de ABI/compiler/stack, CRT, GetSecurityInfo, kernel/cache ou heap opaco. Quotas Selected3MiB/Legacy2MiB e todos os domínios aplicáveis permanecem. Custos opacos continuam UNKNOWN no perfil limitado B0–B8, monitorados pelos gates B3; nenhum pico novo foi medido ou inventado.

## Qualificação final

| Perfil/configuração | Build MSVC /W4 /WX | CTests afetados |
|---|---|---|
| Selected Debug | PASS, zero warnings | 5 PASS |
| Selected Release | PASS, zero warnings | 5 PASS |
| Legacy Debug | PASS, zero warnings | 5 PASS |
| Legacy Release | PASS, zero warnings | 5 PASS |

Total final: 20 execuções CTest PASS. Builds serializados `--parallel 1`, mesmos targets/backends. A revisão do censo de automáticos foi seguida de rebuild/regressões afetadas; logs anteriores foram preservados com sufixo before-census-refinement. Não foi repetida campanha geral histórica sem relação com a mudança.

Novo teste `papacc.personal.anchor_child_guard`: 38 cenários, 145 verificações/configuração, no harness existente. Inclui root AU/0x4 com owner TrustedInstaller e filho protegido; permissões herdáveis INHERIT_ONLY na raiz sem aplicação ao filho protegido; filho não protegido; AU/0x4 no filho; DELETE/WRITE_DAC/WRITE_OWNER/DELETE_CHILD na raiz e no filho; generic rights; outro trustee; flags divergentes; DACL nula/owner divergente/tipo não suportado; reparse; substituição de root/filho; mudança de descritor; ausência de componente; Stop; concorrência/owner incorreto; falha de free/close e retenção. Também exercita `staging_limited_refs` com originais alterados.

Regressões: staging 57 cenários/185 verificações; recursos 53 casos/184 verificações; coleta modelada 26 cenários/127 verificações, preservando classificação estrita AU/0x4; close guard seis estados. Os dois caminhos positivos novos exercitam a composição de cinco criações em modelo e confirmam que `staging_admit` ainda nega sem plano/capacidade/autoridade próprios.

APIs de filesystem/token/recursos/RNG são simuladas nos testes afetados; codecs SD/SID e SHA256 em memória são reais. Nenhum diretório operacional foi consultado/criado, nenhuma ACL alterada, nenhum staging ou serviço executado. Logs/BUILD-VERIFICATION.json registram evidência, sem apresentar contagens como aceitação total da Phase6.

## Pacote e próximo pedido

Selected/Release final: papacc_server.exe 571392 bytes, SHA256 `c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf`. DLLs preservam seus pins. Pacote proposto de três arquivos: 9.110.528 bytes; FINAL-PACKAGE.json e CANDIDATE-PINS.json vinculam bytes/source/destino. Não é uma imagem instalada nem pacote selecionado por inferência.

Os bytes c58d1eb2... da consulta já consumida e suas DLLs foram preservados antes do rebuild em `build/staging-observation-preserved-20261031/approved-df0e9ee7-chain`. A cópia preserva bytes, não FileID ou autoridade operacional. O pacote anterior 59e8808e... e seus pedidos/autorizações continuam históricos; não atualizar esses pins para executar novamente.

[Novo pedido operacional candidato](ONE-SELECTED-STAGING-REQUEST.CANDIDATE.md), SHA256 `72fb886f6ba8e6eafd77dcd59d39d46b0e9f7c07248a6bf13fd380cbfd7f58b8`, está pronto para decisão específica. Aprovar esse pedido seleciona explicitamente o NOVO pacote e autoriza somente uma nova tentativa manual delimitada, sem rearmar atos anteriores. Nenhuma aprovação foi registrada nesta preparação.

Os valores B3 permanecem exatamente os já selecionados: H_READY+5; crescimento privado/working 9.094.144 cada; físico 6.296.461.312 e commit 8.479.985.664; disco 2R/R com volume real; freshness500ms/duração120s. O aumento de bytes do pacote não aumenta esses orçamentos. Nenhuma nova seleção dos mesmos números é necessária. A receita candidata reutiliza o script anterior com somente o diretório-fonte atualizado; parsing/delta/chamada única foram conferidos, sem executá-la.

Não há lacuna técnica conhecida para a apresentação desse pedido. Permanecem factuais/frescos no preflight autorizado: contexto/rights/fontes/known folder/ausência de colisão, objetos originais, unidade/espaço do volume, amostra READY, recursos/tempo, nonce e confirmação atribuída. Não se exige nova coleta independente para antecipar fatos que o gate já adquire antes de efeitos. Não há comando com hash de aprovação futuro: o registro só pode ser criado após decisão explícita.

## Efeitos futuros separados

Se especificamente autorizado e admitido: criar exclusivamente Papinho e Accelerator sob os originais retidos, com DACL protegida existente no código, e três arquivos com os bytes/pins do pacote; cópia, flush, hash/readback e close governados. Colisão implica DENY sem adoção/overwrite/limpeza. Até cinco criações, sem promessa de atomicidade ou refund. Falha parcial/UNKNOWN conserva evidência/retenção.

Registro SCM PARADO exige outro pedido e readback original do staged; Start/Stop e sessão SCQ/CNG continuam separados. Nenhuma criação de CA/CURRENT, endpoint/rede, TPM/NV/Q4, cleanup/refund, recuperação ou promoção PST0.6.5. E21 PENDING; ED11/Phase6 PARTIAL; staging real permanece DENY até autorização específica e gates satisfeitos.
