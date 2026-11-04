# Cinco cortes — resultado factual reconciliado

Estado READONLY_BOUNDARY_OBSERVATION_VERIFIED_CLOSED. Autorização de coleta
e7b641...f4a42/0d945...cdd6 UTILIZADA, sem retry. Registro anterior de pendência
preservado como histórico e sucedido por este resultado. Não repetir comando.

Fonte: mensagem do proprietário. Linhas emitidas preservadas literalmente em
OWNER-COLD-BOUNDARY-EMITTED-OUTPUT.txt (excerto, não transcript completo de console),
SHA256 4f59daae5757c167653fd487cb5f1668dcc70f8d54c8e29d711e0bccb50a31df.
Resultado estruturado COLD-BOUNDARY-OBSERVATION-RESULT.json.

| Corte | Handles conhecidos | Delta líquido desde corte anterior |
| --- | ---: | ---: |
|0: primeiro despacho pós-CRT/pré-owner|112|não aplicável|
|1: após owner|112|0|
|2: após bridge console|112|0|
|3: após token/grupos|113|+1|
|4: após knownfolders/drive|136|+23|

Todos known1/error0. Resultado first0/close_known1/sample_bytes60;
wrapper exit0/INVOKEDTrue/WRAPPER_CLOSE_KNOWNTrue. A coleta encerrou segundo
os gates publicados; não significa capacidade ou quiescência OS.

O excesso não estava presente no primeiro corte DESTA observação:112 é menor
que124. O aumento líquido de24 até136 se divide em+1 na fase de token e+23 no
grupo seguinte. O+1 é consistente com o TOKEN_QUERY próprio explicitamente
adquirido/retido, sem provar ausência de variações internas compensadas.
Console/owner delta0 não prova ausência de recursos transitórios nessas fases.

O grupo do+23 contém SHGetKnownFolderPath ProgramFiles e ProgramData,
construção/cópia dos caminhos, CoTaskMemFree de ambos os PWSTR, GetDriveTypeW e
a consulta final. Não há corte interno que separe essas APIs. Não atribuir23
a ProgramData, shell/cache/registro, COM, drive ou vazamento individualmente.
CoTaskMemFree libera buffers, não autoriza fechar handles internos opacos.
112 anterior a main continua sem decomposição CRT/loader/herdados/ambiente.

Isso LOCALIZA o aumento de fase numa NOVA invocação diagnóstica. Não prova
retroativamente a decomposição dos136 da sessão anterior, nem a causa da
primeira sessão1816 sem instrumentação. A diferença de artefato/local/cenário,
custo de cinco queries/60bytes e mudanças assíncronas permanecem limitações.
Nenhum handle observado ganha ownership dispensável por simples delta.

Guard124/cap128 preservados.136 não seria admissível na fronteira SCQ:
é8 acima de128 e13 acima do maior valor aceito pelo predicado pré-open (<124).
Isso é aritmética, não13 handles demonstrados como removíveis. Nenhuma redução
segura está estabelecida. A guarda não foi usada como permissão para abrir
originais; o modo observado não executa SCQ/endpoint/Start/SCM.

Primeiro gate técnico: examinar offline o uso do resolver full-layout pelo
controlador que usa o destino binário, sua necessidade de ProgramData e a
compatibilidade dos consumidores/revalidações com o resolver binary-only já
existente. Não afirmar economia por trocar helper antes de demonstrar delta.
Também confrontar proteção/knownfolder original: caminho fixo/ambiente não
substitui fonte legítima só para evitar inicialização nativa.

Nenhum fechamento de cache/console/herdados ou API interna sem contrato de
ownership; nenhuma mudança dequota. Se o código não bastar para justificar
correção, a lacuna factual é o interior DESSE grupo, não nova contagem genérica
ou tracing global. Coleta adicional requer pedido/autoridade próprios, sem
reuso desta autorização. Não executar nada adicional nesta reconciliação.

Retenção histórica, OS_QUIESCENCE e causa1816 anterior continuam UNKNOWN;
instalado/previous/decisões/pins/HEAD/WIP/PST0.6.4 preservados. Admissão Start
DENY, bilateral/receipt/close/RUNNING diagnóstico não demonstrados. Sem novo
build/teste, porque só evidência documental foi reconciliada nesta tarefa.
