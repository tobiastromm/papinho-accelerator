# Autorização operacional — UPDATE da imagem P1

TASK: PACC-PHASE6-P1-IMAGE-UPDATE-AUTHORIZATION.
Registro: 2026-11-01. AUTHORIZED_ONE_MANUAL_EXECUTION_PENDING / NÃO EXECUTADO.
Fonte: autorização explícita do proprietário nesta sessão, escopo registrado
documentalmente; não fabrica confirmação nativa ou grant persistido.

O proprietário selecionou o candidato P1 Selected/Release772608bytes,
SHA25654d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50,
e autorizou UMA execução MANUAL BINARY_UPDATE_CLOSED PERSONAL_LOCAL conforme
ONE-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md,
SHA2569a2d514469efb1c1ccc02847ac02e60e54de740ac72aae5c9c84c39f93306e24.
Pedido aprovado e procedimento89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9
conferidos e preservados sem alteração. U1–U7/U2 lidos e fontes normativas
byte-exatas, hashes9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234
e58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.

Operador esperado S-1-5-21-785699550-1440458426-1682014700-1108, host TCORP-TOBIAS.
Metadados não estabelecem autoridade; exigir token/contexto administrativo
legítimo, declaração e confirmação atribuída OWNER_BINARY_UPDATE pelo owner.
Nenhuma resposta automática, autoelevação ou permissão ampliada.

## Conferência preparatória e gates futuros

Identidades/hashes/tamanhos reconferidos somente leitura por referências retidas:

| Papel | Bytes | Identidade | SHA256 |
| --- | ---: | --- | --- |
| Executor protegido | 104448 | 417614100:30670848:57859 | 084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a |
| EXE canônico anterior | 763904 | 417614100:1835008:134613 | 7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d |
| Candidato P1 DATA | 772608 | 417614100:196608:2588986 | 54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50 |
| libssl-3-x64.dll | 1321472 | 417614100:15990784:71249 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 417614100:22216704:9543 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Parent417614100:8257536:71209. Executor/canônico/DLLs/parent e Papinho têm
owner Administrators, DACL protegida com BA/SY full e AU RX; SD digest
b328dcebc8928d808abef1479210a40ff98611cec20f6fb91f9f93a629d8a89f.
Ancestrais adquiridos sem reparse; continuidade verificada dentro do snapshot,
fechamento conhecido. Fonte DATA tem owner/DACL do workspace inspecionados,
não promovidos a autoridade nem exigidos como executor; U2 aplica-se somente
com todas as outras provas cumulativas. Não presumir continuidade histórica.

Evidência P1-UPDATE-AUTHORIZATION-READONLY-SID.json. Conferência auxiliar inicial
falhou ao traduzir nome de ACE pelo domínio, com fechamento conhecido e zero
UPDATE; P1-UPDATE-AUTHORIZATION-READONLY.json preserva a falha. A leitura foi
corrigida para SIDs nativos, sem tradução/consulta de nomes ao domínio. Isso
não foi falha de guarda ACL, tentativa UPDATE, nova autorização ou efeito mutante.

Consultas SCM somente leitura nesta preparação: STOPPED/PID0/exit1077,
WIN32_OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, NT SERVICE\PapinhoAccelerator,
SID UNRESTRICTED; comando original:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Nenhum Start/Stop ou configuração submetidos. Snapshot não é lease, fencing,
prova de páginas mapeadas ou quiescência OS. Readquirir/revalidar tudo no owner
da futura invocação, incluindo associação executor/processo/token e proteção.

## Escopo autorizado e tentativa única

Permite somente efeitos do pedido: um candidato protegido exclusivo, cópia/
hash/flush772608bytes, rename do canônico anterior para .previous exclusivo,
rename do candidato verificado para canônico, readback e fechamento factual.
Nomes da tentativa CSPRNG original apresentados antes de confirmar/consumir.
Consumo irrevogável antes da primeira criação. Autorizações anteriores consumidas
não são reutilizadas. Execução manual ainda pendente; este registro não consome
o ato nativo nem afirma sucesso. Invocação manual única, sem retries, mesmo
se consumed interno0 ou falha de shell. Não trocar leaf para superar conflito.

Antes de cada efeito: Stop, plano, contexto/originais, SCM parado/configuração,
K2027314 do executor, geometria/R e capacidade frescos. U4096 dá R774144 apenas
como exemplo; usar geometria real. Custos opacos/latência/cancelamento continuam
UNKNOWN sob U4, não máximos demonstrados. G3/guard124/cap128 preservados nos
seus domínios; nenhuma quota alterada ou transplantada.

Riscos residuais reconhecidos pelo proprietário: custos opacos, substituição
parcial, intervalo sem EXE canônico, retenção persistente e ausência de rollback
automático. Retenção histórica/coorte/créditos e OS_QUIESCENCE UNKNOWN. Se isso
impedir predicado material, STOP; não presumir recuperação/crédito liberado.
Stop não cancela I/O submetido; falha impede próximo efeito, retenção preservada.

Proibidos execução automática, retries, update de utilitário/DLLs, limpeza,
restauração, rollback, exclusão, endpoint/SCQ, Start/Stop, SCM/ACL, CA/CURRENT,
rede operacional, TPM/NV/Q4, refund e autoridades produtivas. Preservar
HEAD/WIP, históricos/pins/EXEs anteriores, reservas/retidos e PST0.6.4.

Depois da saída MANUAL integral: conferir primeira falha/consumo/submissões,
renames3 como bitmask dos DOIS renames, anterior retido, canônico P1, identidades/
hashes/SD/readback, SCM parado, canonical_verified_closed1/close_known1 e
LIMITED_UPDATE_VERIFIED_CLOSED. Exit0/wrapperclose isolados não bastam.
Não repetir. Só após sucesso qualificado preparar NOVOS originais P1 e sessão
E/Q/S para autorização própria, sem presumir economia de handles ou readiness.

Estado final deste registro: autorizado apenas o procedimento MANUAL único,
aguardando proprietário. Evidência de execução futura pendente. E21 PENDING,
ED11/Phase6 PARTIAL, Start e readiness produtiva DENY, UNKNOWN preservado.
