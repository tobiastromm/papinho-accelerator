# Pedido candidato — um BINARY_UPDATE_CLOSED PERSONAL_LOCAL

Status: READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION / NÃO AUTORIZADO.
Data: 2026-11-01. Preparação técnica somente. A aceitação U1–U7 não autoriza esta
execução. Solicita-se posteriormente a seleção deste artefato e autorização de
UMA execução manual, sem retries; nenhuma operação é executada por este pedido.

## Norma, identidade e alvo

Decisão accepted: docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md,
incorporação da proposta SHA256
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234.
Somente U1–U7, sem importar B3/R3/G3 ou autorização anterior consumida.

Operador candidato para esta autorização:
S-1-5-21-785699550-1440458426-1682014700-1108; host TCORP-TOBIAS.
Esses identificadores são seleção DATA/histórica, não prova de propriedade ou
autoridade. Exigir token/contexto administrativo nativos elegíveis, declaração
atribuída e confirmação local independente para este UPDATE. Não criar conta.
Boot/quiescência OS não são estabelecidos por nome do host/uptime ou este pedido.

Instalação existente, sem adoção/migração: C:\Program Files\Papinho\Accelerator.
Known folder deve resolver ProgramFiles C:\Program Files e ser congelada.
Parent nativo esperado: volume417614100/high8257536/low71209.
EXE anterior: C:\Program Files\Papinho\Accelerator\papacc_server.exe,
volume417614100/high6225920/low71237, 571392 bytes,
SHA256 c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf.
São pins/IDs históricos previamente qualificados, a revalidar por referências
originais nesta operação; nenhuma consulta operacional nova foi feita nesta task.

Serviço exato PapinhoAccelerator, conta NT SERVICE\PapinhoAccelerator,
SERVICE_WIN32_OWN_PROCESS, DEMAND_START(3), ERROR_NORMAL(1), SID UNRESTRICTED(1).
Comando original obrigatório:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Exigir readback completo SERVICE_STOPPED/PID zero, configuração/conta inalteradas
e contexto original revalidado antes de efeitos. Status é snapshot, não fencing
contra outro administrador nem prova universal de quiescência.

## Artefato exato proposto para seleção

Controller E fonte do novo payload:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe.
Selected/Release qualificado, 737280 bytes, SHA256
42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca.
Snapshot de revisão preservado, sem autoridade de recuperação:
build/private-bootstrap-preserved-20261031/papacc_server-update-u1-u7-42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca.exe.
Não substituir a seleção por build posterior. Pin divergente exige STOP.
Hashes on-disk não são atestação da imagem mapeada.

Dependências no diretório do controller e no destino, somente adquirir/validar:

| Arquivo | Bytes | SHA256 |
| --- | ---: | --- |
| libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Não copiar/renomear/alterar DLLs. O EXE staged antigo não executa esta atualização;
o controller é a imagem de workspace acima, com suas dependências conferidas.

## Ato original e nomes tardios legítimos

O caller privado --owner-manual-binary-update apenas inicia a cerimônia; não
concede autoridade. No mesmo owner/invocação, capturar originais, gerar tentativa
CSPRNG de32bytes, apresentar plano protegido completo e receber a declaração/
confirmação nativa OWNER_BINARY_UPDATE para ESSA tentativa. Não fornecer resposta
automaticamente nem aceitar confirmação de NEW_INSTALL, staging, SCM ou Start.

Uma nova identidade de arquivo e o nonce não são inventados neste documento.
Antes da confirmação e de qualquer mutação, o programa mostra os dois nomes
exatos, derivados da tentativa fresca, no parent original acima:

- papacc_server. +64 caracteres hexadecimais minúsculos da tentativa + .candidate;
- papacc_server. +a MESMA tentativa + .previous.

São regras fechadas para geração do plano original, não placeholders de comando
ou autoridade por posse de nome. Mudança de plano/contexto/refs/pins/nomes
invalida a confirmação. Nomes existentes não são adotados nem substituídos.
Capturar a identidade factual do novo objeto só depois da criação exclusiva.

## Efeitos permitidos, nesta ordem

1. Captura/revalidação read-only de arquivos, descritores, token, ancestrais e
   SCM, com direitos necessários; instalar o bridge existente de Stop do
   controller. Consultar contadores próprios/host e volume. Nenhuma mutação SCM.
2. Confirmar e consumir irrevogavelmente o ato antes da primeira criação.
3. Uma NtCreateFile FILE_CREATE exclusiva do candidato no mesmo diretório,
   DACL nova protegida BA/SY full e AU read/execute; nenhuma edição de ACL
   existente. Copiar somente os737280 bytes novos, hash/descritor/size e flush.
4. Um NtSetInformationFile FileRenameInformation(10), ReplaceIfExists FALSE,
   sobre o handle do EXE anterior para o leaf .previous; verificar o nome por
   referência relativa e identidade/hash/descritor originais.
5. Um segundo rename exclusivo sobre o handle do candidato para papacc_server.exe;
   verificar pelo parent original identidade do novo objeto, conteúdo/proteção,
   configuração SCM inalterada e serviço ainda parado.
6. Fechar referências governadas uma vez se factual; remover o bridge do
   controller após fechamento conhecido. Emitir a disposição e o file ID novo
   observado, sem autoridade para Start.

O anterior preserva objeto/bytes, retido em .previous. O nome .candidate deixa
de existir somente pelo rename previsto quando este for confirmado. Não há
overwrite direto, DeleteFile, rollback, limpeza ou remoção do anterior.

## Recursos e risco U4 selecionáveis neste pedido

Selected: orçamento gerido compartilhado3145728 bytes, sem aumento de quotas.
Censo estrutural atual KNOWN=2000800; K do caminho completo compilado=2025386
bytes, incluindo custo de serviço/bridge e frames nomeados/caller conhecidos.
O programa usa esse mesmo K para reserva e piso físico/commit antes de efeitos.
São somas conhecidas, não pico nativo, reserva física ou garantia de conclusão.
Legacy foi qualificado separadamente e não é selecionado por este pedido.

Física disponível e commit disponível >=2025386 em amostra nova antes de cada
submissão mutante. Contar handles totais, inclusive herdados; inventariar todos
os originais, candidato e readbacks, além dos dois contextos SCM. Não há novo
ceiling total de handles. 124/128/1024 ficam em seus domínios, não são dispensados
para SCQ/Start nem transplantados para este UPDATE.

R=ceil(737280/U)*U, com U da geometria real do volume, aritmética verificada e
geometria congelada/revalidada. Caller-free e actual-free >=R antes da criação;
antes de cada escrita, conferir payload restante arredondado. Se U=4096, R=737280;
esse exemplo não substitui a consulta fresca. Anterior já alocado não é nova
cópia. Metadados/flush/rename não são considerados cobertos por R.

Freshness por evento/contexto: revalidar e amostrar imediatamente antes de cada
efeito, depois da confirmação humana e chamadas anteriores. Sem TTL/duração
numéricos adicionais selecionados por inferência. Não se garante latência máxima,
cancelamento imediato, ausência de pressão entre amostras ou capacidade total.
Continuam UNKNOWN custos de loader/CRT/PowerShell, alocador/hash/descritores,
RPC/SCM, filesystem/cache/metadados/flush/rename/kernel e retenções opacas.
O risco residual U1–U7 exige reconhecimento explícito na autorização operacional.

## STOP, falhas e reconciliação

Esc cancela a confirmação; Ctrl+C/Ctrl+Break sinaliza somente Stop do controller
pelo bridge existente, nunca ControlService. Stop observado, recurso/consulta
desconhecido ou insuficiente, divergência de identidade/proteção/serviço/plano,
conflito de nome ou falha nativa impede nova mutação. Não fechar console, matar
processo ou ampliar permissões como procedimento de correção.

Sem atomicidade/durabilidade presumidas entre renames. Falha após criação pode
deixar candidato parcial e antigo canônico; falha entre renames pode deixar o
canônico ausente; após o segundo pode haver novo canônico ainda não confirmado.
Em pending/erro/close incerto, conservar cohort/objetos/créditos UNKNOWN, sem
rollback, delete, novo leaf, timeout cleanup ou retry. Não interpretar caminhos
possíveis como fatos sem readback. Processo encerrado não prova quiescência OS.

Após a única execução, recolher a saída completa, primeira causa, consumo,
submissões, identidades/pins, K/R/gates e fechamento. Somente o marcador nativo
LIMITED_UPDATE_VERIFIED_CLOSED com primeira causa zero, dois renames confirmados,
fechamento conhecido e canonical_verified_closed=1 sustenta sucesso limitado.
Exit0 isolado ou saída incompleta não bastam. Retenção/OS_QUIESCENCE UNKNOWN
permanecem explícitos. Mesmo sucesso não autoriza endpoint, Start ou CA/CURRENT.

## Procedimento manual candidato e fronteira de autorização

Arquivo: update-binary.MANUAL.CANDIDATE.ps1 nesta pasta.
SHA256 85531965578ba126200bfab6d2168226dfdaadd9c4e1b96135411d01d1e6ac55.
Conferido sintaticamente por AST, NÃO EXECUTADO. Windows PowerShell5.1 Desktop
x64, sem perfil, no contexto administrativo legítimo já obtido pelo proprietário.
Não há autoelevação. Se necessária, a invocação posterior poderá usar
ExecutionPolicy Bypass somente no processo filho para esse script; sem alterar
CurrentUser/LocalMachine/GPO ou contornar recusa de política administrada.

O procedimento recebe os hashes explícitos do script e deste pedido aprovados
pelo proprietário, confere-os ANTES da chamada, confere tamanhos/pins de todos
os seis arquivos e identificadores do operador/host. São gates de integridade,
não substitutos dos originais e da confirmação nativa. A única chamada ao
controller contém caminho, seis componentes numéricos de IDs e quatro pins
literais acima; nenhum PID/token futuro, resposta automática ou placeholder.

A tentativa manual é utilizada ao invocar o procedimento; negação, consumo
interno0, erro de shell ou wrapper INVOKED=False não concedem retry automático.
Após autorização explícita futura deste pedido/hash, preparar o comando manual
literal com seus dois hashes e aguardar a execução única pelo proprietário.
Nenhum comando de efeito é emitido ou executado por esta task offline.

Proibidos: SCM config/ACL/CreateService/Start/Stop/DeleteService, instalação,
criação de diretórios/contas, mudanças de DLL, CA/CURRENT, endpoint/rede operacional,
TPM/NV/Q4, rollback/restauração, limpeza/refund ou recuperação autoritativa.
Preservar histórico/pins/atos consumidos/fixture/reservas/HEAD/WIP e PST0.6.4.
E21 PENDING; ED11/Phase6 PARTIAL; autoridade operacional atual DENY.
