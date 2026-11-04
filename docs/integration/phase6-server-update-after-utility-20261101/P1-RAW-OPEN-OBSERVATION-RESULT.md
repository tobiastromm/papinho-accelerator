# Reconciliação da observação manual raw0

Fonte: saída integral enviada pelo proprietário nesta conversa. Sem repetição
da consulta, sem nova inspeção dos objetos instalados.
Pedido ce5b3f33ee311eae9f5771e0117a112e4a81d6e0197e388c049617da13bee166.
Autorização read-only: USED_NO_RETRY, não mais pendente. Registro original de
aprovação preservado; esta reconciliação registra o estado posterior.

```
RAW_OPEN_RESULT attempted=1 handles_before=68 ntstatus=0xc000050b iosb_status_snapshot=0x00000000 iosb_information_snapshot=0 returned_handle=0; INITIALIZED_IOSB_NOT_PROVEN_WRITTEN_ON_FAILURE
RAW_OPEN_NO_HANDLE_RETURNED; HISTORICAL_CAUSE=UNKNOWN; AUTHORITY=DENY
P1_RAW_OPEN_MANUAL_EXIT=0; INVOKED=True; EXIT_NOT_AUTHORITY; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY
```

Uma chamada efetivamente alcançada, guard124 não rejeitou: count68 no observador
independente, não redução demonstrada do controlador histórico113. Nenhum handle
de arquivo retornado; hash/identidade/leitura do alvo não alcançados. Portanto
esta saída NÃO revalida ID/proteção/hash atuais do documento. Nenhuma nova
confirmação E/Q/S ou Start; autorização dessas operações continua consumida.
IOSB zeros são snapshots inicializados, não comprovação de resultado escrito.
Exit0 indica observação encerrada, NÃO abertura bem-sucedida/readiness.

0xC000050B = STATUS_REPARSE_POINT_ENCOUNTERED, confirmado no SDK Microsoft
10.0.26100.0/shared/ntstatus.h:10793. A documentação primária de
[OBJECT_ATTRIBUTES](https://learn.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes)
define que OBJ_DONT_REPARSE rejeita reparses encontrados na resolução do nome.
Logo a falha atual é uma rejeição de resolução por reparse, não prova de ACL
inadequada, excesso de handles ou falta de capacidade física/commit.

A saída NÃO localiza o componente responsável: pode envolver namespace de
objetos/DOS ou componente do filesystem. Não afirmar que o arquivo final é um
symlink/junction, nem que C:\ ou um ancestral é inadequado sem prova específica.
O nome original é `\??\C:\...`; existe hipótese focal de resolução do namespace
DOS, mas o status isolado não identifica o ponto. Processo/token/contexto
históricos não equivalentes por prova integral; causa histórica continua UNKNOWN.

Não remover OBJ_DONT_REPARSE ou FILE_OPEN_REPARSE_POINT, nem ampliar sharing,
direitos/quotas ou alterar ACL para obter PASS. Não foi feita correção funcional.
Sem alteração de código, não repetir builds/campanhas inalteradas.

Próximo trabalho exclusivamente offline: avaliar a reutilização das primitivas
já existentes de resolução nativa e validação original/ancestrais, distinguindo
namespace DOS de reparse filesystem, com continuidade e proteção preservadas.
QueryDosDevice/string isolados não dão identidade/proveniência ou autoridade.
Comparar eventual nome de dispositivo absoluto com referências originais e
contratos antes de implementar. Nenhuma nova prova ou chamada alternativa está
autorizada por esta observação consumida. Se faltar fato, delimitá-lo antes de
propor outra consulta; não preparar retry da sessão ou UPDATE agora.

Guard124/cap128/G3, DENY, históricos/pins/EXEs/originais/HEAD/WIP/PST0.6.4
preservados; retenção histórica e OS_QUIESCENCE UNKNOWN. Não inferir fechamento
global de coortes anteriores da ausência de handle retornado nesta abertura.
