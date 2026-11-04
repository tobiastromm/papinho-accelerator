---
status: candidate-not-authorized
date: 2026-10-31
scope: one manual PERSONAL_LOCAL LIMITED_R0_R9 stopped SCM registration
---
# Pedido operacional candidato — UM registro SCM parado

NÃO AUTORIZADO / NÃO EXECUTADO. Aceitação R0–R9 permite integração offline, não este efeito. Pedido completo para decisão específica do proprietário; todos os gates frescos abaixo permanecem obrigatórios. Nenhuma execução foi feita para preparar o pedido.

## Identidade e plano congelado

Host selecionável: TCORP-TOBIAS. Operador esperado: S-1-5-21-785699550-1440458426-1682014700-1108, proprietário/administrador legítimo sob declaração atribuída e token original elegível. Os identificadores históricos não provam contexto atual; divergência nega. Uma tentativa manual, uma invocação viva; sem retry mesmo se o gate inicial falhar. Nenhuma aprovação histórica é reutilizada.

Perfil exclusivo: PERSONAL_LOCAL SCM_REGISTER_CLOSED / LIMITED_R0_R9. Serviço local exato PapinhoAccelerator. Conta virtual NT SERVICE\PapinhoAccelerator, sem senha ou criação de usuário SAM. SERVICE_WIN32_OWN_PROCESS; SERVICE_DEMAND_START=3; SERVICE_ERROR_NORMAL=1; SERVICE_SID_TYPE_UNRESTRICTED=1. Comando registrado exato:

`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`

Não é o comando de iniciar serviço. A primeira execução deste pedido é o controlador administrativo abaixo, não o executável staged como runtime.

## Artefatos separados e imutáveis para este pedido

Controlador candidato Selected/Release: C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe. Gerado/qualificado no workspace, NÃO staged/instalado e NÃO atestado como imagem mapeada. Aprovar este pedido selecionará estes bytes para esta execução manual somente.

| Local | Arquivo | Bytes | SHA256 |
|---|---|---:|---|
| Controlador Selected/Release | papacc_server.exe | 579072 | ca4e1fa707230510da19eb7aebd4ab0e39504661504c27ebecfc494c95ccec55 |
| Mesma pasta do controlador | libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| Mesma pasta do controlador | libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |
| Alvo staged existente | papacc_server.exe | 571392 | c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf |
| Mesmo alvo staged | libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| Mesmo alvo staged | libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Alvo staged: C:\Program Files\Papinho\Accelerator. Três arquivos existentes, 9.110.528 bytes; não copiar, substituir ou recriar nenhum. Evidência anterior LIMITED_QUALIFICATION consumida, close controlado conhecido, retained0; não atesta FileIDs/ACLs atuais nem quiescência OS. Nesta tarefa só foram reconferidos os bytes de referência preservados no workspace; o alvo protegido atual será adquirido/revalidado nos gates desta tentativa, não presumido por existência/hash histórico.

## Quantitativos e garantia aceitos, aplicados a este plano

Norma: DECISION-personal-stopped-scm-registration-r0-r9-20261031 incorpora proposta SHA256 da633b69d2f1214bc7f0a35b2d327f0f45fa7a850d02e3a0551729a03c2c999a.

Q Selected = 3.145.728 bytes geridos. O censo estrutural atual KNOWN=1.989.272 bytes é soma conhecida, NÃO pico nativo, reserva física ou medição da futura execução. A reserva real do presenter, incluindo storage/frames conhecidos, deve caber no Q original; nenhuma quota individual aumenta. Legacy foi qualificado com seu Q=2.097.152 mas NÃO é o artefato selecionável deste pedido.

Pisos físico e commit disponíveis >= 3.145.728 bytes, em amostra válida e fresca no gate antes de CADA submissão mutante. Esse piso é a tolerância fraca aceita em R3: não cobre necessariamente RPC/kernel, não constitui reserva e não garante o próximo passo. GetProcessHandleCount, privado, working set e margens são observados/registrados, sem ceilings opacos fictícios. No máximo dois contextos SCM controlados simultâneos (manager e serviço), distintos do inventário completo de handles/referências originais; não equivalem a H_READY+2. Nenhum 124/128/171 transplantado; demais domínios conservam quotas accepted.

Uma CreateService e no máximo uma ChangeServiceConfig2 para SID type. Queries/readback e fechamento finitos; zero retries. Recursos internos de persistência SCM/Windows, CRT/loader, hashing, kernel/RPC e retenção opaca permanecem UNKNOWN quanto a máximo/atribuição completa. Zero novo payload, diretório ou arquivo de produto pelo controlador. Não importar disco2R/R do staging.

Freshness por evento/contexto F4/R3: nova amostra depois da interação e revalidação antes de cada efeito, QPC/frequência/continuidade registrados; não reutilizar amostra pré-confirmação. Não há TTL, bound de duração, cancelamento imediato de RPC ou promessa entre amostras. Exaustão/degradação/bloqueio/registro parcial são riscos explicitamente reconhecidos para este ensaio, sem capacidade produtiva universal.

## Gates frescos e sequência manual única

1. Conferir hash deste pedido e do script ANTES da chamada. Windows PowerShell 5.1 Desktop x64, console já elevado; não autoelevar nem alterar políticas persistentes. A execução futura poderá usar Bypass somente no processo filho dessa chamada, sem relaxar GPO ou permissões. Se política superior negar, STOP sem retry.
2. Wrapper somente leitura: host/SID/contexto administrativo; known folder ProgramFiles = C:\Program Files; seis arquivos/pins/tamanhos acima e ausência de reparse nos arquivos. On-disk pin não é atestação de módulo mapeado nem autorização. O wrapper não registra confirmação ou aprovação em nome do proprietário.
3. Invocar UMA vez o controlador fixado, argumentos `service install --scq-closed`. O owner nativo adquire/revalida token/contexto administrativo, known folder/alvo congelado, ancestrais e parent originais protegidos, EXE e duas DLLs por referências originais, conteúdo e descritores estritos, exclusividade/lifetime/Stop. Ausência/substituição/proteção inadequada nega. Não adotar o alvo como NEW_INSTALL nem exigir CA/C8/ProgramData.
4. Presenter mostra operador, alvo/FileIDs/pins, serviço/conta/comando/início/SID type e risco R0–R9. O proprietário deve conferir o plano e os três pins staged contra este pedido antes de confirmar a frase específica gerada no console. Em qualquer divergência, cancelar. Não há confirmação automática ou por stdin redirecionado. O nonce é futuro e não é placeholder: é adquirido nativamente pelo CSPRNG governado da tentativa.
5. Gate de admissão limitada, manager local com direitos mínimos e consulta do nome original: existente, acesso negado, marcado para exclusão ou ausência não demonstrada nega sem adoção. Ausência observada não é reserva; Create exclusivo rejeita corrida. Nenhum PID/token de serviço futuro é exigido antes da sua criação.
6. Revalidar plano/originais/Stop e recursos, consumir uma vez antes de CreateService; registrar intervalo QPC/retorno. Revalidar e obter NOVA amostra antes de configurar SID type. Se Stop/recurso falhar entre efeitos, permanecer parcial, sem completar por conveniência.
7. Readback original de conta/comando/tipo/início/error-control/SID type e SERVICE_STOPPED completos e concordantes. Fechar referências controladas uma vez em ordem governada; falha parcial ou close incerto conserva cohort/charges/UNKNOWN. Não inferir quiescência OS, readiness, token real do serviço ou isolamento CA.

## Efeitos autorizáveis e proibidos

Aprovação específica futura autorizaria somente as consultas/metadados/pins/contexto, CSPRNG da tentativa, confirmação atribuída, criação exclusiva do registro do serviço local PARADO, configuração de SID type, readback e fechamento governado acima. Persistem o registro SCM e sua configuração de conta/comando/SID; o próprio Windows poderá persistir metadados internos. Isso não é uma instalação produtiva concluída.

Não autoriza StartService/ControlService/Stop/DeleteService, automação de startup, mudança adicional de conta/SCM/security descriptor, ACL de arquivos/serviço, staging/migração, criação de diretórios/usuários/CA, KSP/CNG de chave, CURRENT, endpoint/SCQ, rede operacional/remota, TPM/NV/Q4, cleanup/refund ou recuperação. O controlador já seleciona manager LOCAL, sem host remoto.

## STOP, disposição e ausência de retries

Qualquer falha de pin/identidade/token/contexto/known folder/descritor/reparse, plano divergente, Stop, consulta incompleta, perda de continuidade, insuficiência controlada/piso R3, overflow, replay/concorrência ou erro nativo impede novas submissões. Readback/close necessários podem registrar disposição sem mutação adicional. Não ampliar permissões, trocar pacote/operador/path, apagar serviço parcial ou repetir comando.

Falha após Create ou durante configuração pode deixar registro persistente. Conservar primeira causa, consumo e referências/charges quando necessário. Falha/resultado incerto não prova rollback nem ausência do serviço. Nova reconciliação/remoção exige autoridade própria. Wrapper exit0 sozinho não demonstra readback/close; exigir resultado nativo correlacionado STOPPED_REGISTRATION_QUALIFIED e fechamento conhecido. OS_QUIESCENCE=UNKNOWN sempre preservado no alcance desta prova.

## Procedimento fixado e evidência permitida

Script candidato: docs/integration/phase6-scm-r0-r9-integration-20261031/register-stopped.MANUAL.ps1.
SHA256: `55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571`.
Preparado e validado por parsing; NÃO EXECUTADO. A sintaxe nativa, confirmação, recursos, falhas, readback/close e separação de perfis foram qualificados em modelos, sem APIs SCM/RPC mutantes reais. Não é uma prova nativa Windows da futura operação.

Evidência futura permitida: saída textual não secreta completa do controlador/wrapper, tentativa pública, identidades observadas de operador/alvo/arquivos, hashes/tamanhos, fases e recursos/QPC, erros/disposições, consumo, readback/close/retention. Não solicitar segredos, tokens exportados, bytes de arquivos ou dados de chaves. Nenhum logger/sidecar/ledger operacional novo por conveniência.

## Decisão solicitável

Autorizar UMA execução MANUAL deste pedido byte-exato, selecionando o controlador e os pins acima e reconhecendo os riscos residuais R0–R9, sem execução automática e sem retry. Aprovação normativa R0–R9 não vale como essa autorização operacional. Após o resultado, reconciliar antes de qualquer marco posterior. Registro/início SCM não foram executados nesta preparação.

E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 preservada. Start diagnóstico, endpoint/transporte bilateral e demais provas terão pedidos próprios se tecnicamente completos; não são aprovados por este documento.
