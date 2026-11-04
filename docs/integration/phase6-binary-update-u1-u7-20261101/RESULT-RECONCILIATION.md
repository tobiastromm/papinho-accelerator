# Primeira execução UPDATE — negação na origem, autorização utilizada

Proveniência: saída colada pelo proprietário, preservada em
ONE-BINARY-UPDATE-RESULT.received.txt com formatação Markdown removida. Não é
captura assinada nem atestação da imagem mapeada. Request baa501…4726; imagem
selecionada42d468…29ca ainda confere no workspace. Nenhum retry foi executado.

## Disposição factual reportada

prepared0, first5, location108, consumed0, submitted0, created0, renames0;
close_known1, wrapper INVOKED=True, exit2, WRAPPER_CLOSE_UNCERTAIN=False.
Nenhuma criação/cópia/rename foi submetida pelo fluxo qualificado. O EXE antigo
não foi sobrescrito nem movido por essa tentativa. Não há rollback a executar.
Fechamento controlado foi reportado conhecido; não foi exportado contador
retained. OS_QUIESCENCE continua UNKNOWN. Não inferir o estado SCM atual.

A autorização operacional está UTILIZADA e não pode ser reutilizada. O consumo
interno0 não a rearma. O registro anterior à execução foi preservado; o registro
de autorização corrente aponta este resultado. Não executar novamente o comando
manual anterior, nem novo leaf, limpeza, ACL, staging, SCM ou Start/Stop.

## Localização comprovada pelo código selecionado

`staged_update_object(index,first)` fixa location=100+index. O slot8 é o NOVO
EXE de origem em C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
não o antigo staged (slot13), nem DLL, parent ou raiz de volume.

Na primeira captura desse slot, first5 pode vir da consulta de metadados ou
rejeição de tipo/reparse/volume; também da conjunção de tamanho/controle do SD,
owner/DACL efetivos e digest. Não há metadado de subpredicado nesse resultado.
Falha de GetSecurityInfo é mapeada separadamente para1338; não declarar erro
5 do acesso ao descritor apenas por essa numeração. native_predicate0 registra
que o diagnóstico comum não apontou uma falha, não uma autorização adicional.

A ordem do entry é: originais do destino/DLLs -> fonte8 -> abrir/query SCM ->
prepared1 -> tentativa/apresentação/confirmação -> consumo/criação. Portanto
essa saída não é cancelamento humano do plano, falha de RNG, falta de recursos
observada nem falha de rename. K2025386 é composição; R0 indica gate de volume
não alcançado, não disco vazio. Não repetir coletor para obter o mesmo erro.

## Snapshot read-only adicional e alcance

Foi lida apenas a ACL do arquivo-fonte no workspace acessível ao agente, sem
alteração. SOURCE-ACL-SNAPSHOT.json preserva os campos relevantes. Esse snapshot
não é o descritor do handle no instante da tentativa nem prova nova do host/
token operacional. Não se assume equivalência entre contextos por mesmo path.

Owner observado: S-1-5-21-896877160-3262526087-2538948522-1003.
Não é o operador selecionado S-1-5-21-785699550-1440458426-1682014700-1108,
SYSTEM ou Administrators. Há também ALLOW Modify/Synchronize aplicáveis,
incluindo Authenticated Users S-1-5-11, mask0x001301bf, herdados sem
INHERIT_ONLY. A interseção com os direitos de escrita rejeitados é não vazia
(inclui WRITE_DATA/APPEND/WRITE_EA/WRITE_ATTRIBUTES/DELETE).

O `scm_descriptor` chamado pelo source retorna falso com esse snapshot: sob o
operador esperado, a primeira rejeição de descritor é owner não admitido; mesmo
que esse owner fosse admitido em outro contexto, a ACE AU Modify ainda rejeita.
Isso demonstra incompatibilidade concreta da fonte observada com a política
atual, consistente com location108. Não prova qual teste falhou no handle passado.

Não é prova de ACL Windows incorreta. Workspace de build e origem de controller
administrativo são domínios distintos. A falta de SE_DACL_PROTECTED por si só
NÃO explica a falha: slot8 já aceita herança quando o owner/DACL efetivos passam.
Não adicionar SIDs de build/sandbox à allowlist, não converter hash/lock em
autoridade e não ajustar ACL para obter PASS por conveniência.

## Primeiro gate e caminho mínimo

Antes de novo pedido, demonstrar uma fonte legítima do controller compatível
com U2. O pedido utilizou o mesmo EXE como controller E payload; hash aprovado
não resolve automaticamente a proteção do arquivo executado como administrador.
Reutilizar mecanismos e caminhos governados existentes para essa distinção;
não criar serviço/root/launcher genérico, grant ou mecanismo de recuperação.
Se a guarda estiver sendo aplicada ao papel errado, demonstrar a cláusula
accepted exata antes de corrigir. Se a proteção for insuficiente, preservar
DENY e delimitar o efeito legítimo necessário, sem executá-lo aqui.

Não há correção funcional comprovada que autorize enfraquecer a guarda nesta
reconciliação. Código/pins mantidos, nenhum build/campanha inalterado repetido.
U1–U7 permanece accepted; a questão agora é a fonte concreta, não nova aceitação
da mesma política de risco. Qualquer nova operação exigirá pedido/autoridade
próprios; a anterior fica consumida independentemente de consumed0 interno.

[Próximo prompt](NEXT-TRUSTED-UPDATE-SOURCE.md). E21 PENDING; ED11/Phase6 PARTIAL;
autoridade produtiva DENY; quiescência OS UNKNOWN. HEAD/WIP, históricos/pins,
EXE antigo, fixture/reservas e PST0.6.4 preservados.
