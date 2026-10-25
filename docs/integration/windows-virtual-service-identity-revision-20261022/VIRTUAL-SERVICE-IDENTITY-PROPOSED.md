# Virtual service identity V0–V8 — original proposal

Status: PROPOSED. TASK=PACC-PHASE6-WINDOWS-VIRTUAL-SERVICE-IDENTITY-REVISION-1. Owner explicitly selected the architectural direction; this original proposal is preserved independently of its subsequent acceptance. No implementation authorization.

## V0 — Perfil e precedência delimitada

No perfil pessoal Windows SCM, Windows 7/Server 2008 R2 ou superior com capacidades disponíveis é o piso deste backend, não uma qualificação de todos esses sistemas. A conta virtual específica NT SERVICE\PapinhoAccelerator é o padrão escolhido. Não criar conta SAM local nem solicitar senha de usuário para este perfil. Conta local dedicada passa a alternativa avançada futura, não implementada nem fallback. .\Papinho existente não será alterada ou reutilizada implicitamente.

Esta decisão especializa somente a seleção obrigatória de conta local padrão nas cláusulas M1/M5 da DECISION-phase6-personal-mvp-20261022, a seleção de conta candidata na DECISION-personal-windows-optional-scm-20261022, e a seleção/interpretação de identidade da DECISION-personal-ca-preinitialization-20261022 P3 e Identity interpretation. A separação administrativa/operacional de P4 e CA technical profile3.3 permanece integral. Na execução direta, o token efetivo substitui a exigência de criar/selecionar conta dedicada; não substitui as provas de elegibilidade, isolamento ou negação de acesso privado CA. Fora dessas escolhas não há supersession. Originais preservados byte a byte.

## V1 — Serviço opcional, execução direta preservada

Instalar SCM é escolha administrativa explícita. A execução direta pelo CMD/PowerShell continua independente de serviço e usa o token efetivo factual do processo/contexto, sem exigir conta virtual. Mesmo executable/runtime/root/worker e contratos portáveis; não duplicar servidor. Escolher console não instala/inicia serviço. Escolher serviço não concede autoridade administrativa. Token inadequado ou isolamento não demonstrado nega o caminho dependente, sem elevar ou trocar identidade silenciosamente.

## V2 — Conta de logon e SID são fatos distintos

Configurar explicitamente a conta de logon virtual, não NULL/LocalSystem, e senha NULL, pelos parâmetros documentados do SCM. Configurar separadamente SERVICE_CONFIG_SERVICE_SID_INFO com SERVICE_SID_TYPE_UNRESTRICTED através de ChangeServiceConfig2W, verificando resultado e readback. Esse tipo acrescenta o SID ao token; não escolhe conta de logon, não significa token restrito e não prova privilégio mínimo ou ACL segura.

Distinguir nome de logon configurado, SID resolvido, tipo SID configurado, TokenUser e grupos/atributos efetivos do processo iniciado. A identidade virtual e o SID específico podem ter os mesmos bytes no namespace S-1-5-80; não exigir desigualdade artificial nem confundir as diferentes evidências. Lookup/derivação e readback do SCM não provam o token vivo. Validar fatos ligados ao processo/instalação/tentativa antes de admitir runtime. Nenhum SID textual ou nome constitui autorização.

## V3 — Instalação e configuração protegidas

Preservar nome canônico PapinhoAccelerator, plano administrativo específico, configuração exata, executable/hash e identidades nativas protegidas contra substituição até o efeito. %ProgramFiles%\PapinhoAccelerator continua destino selecionado para binários, não armazenamento de autoridade presumido. Serviço preexistente/divergente nega adoção automática; nenhum overwrite, conta padrão alternativa ou criação de outro serviço. Privilégios SCM permitem operação técnica, não autoridade CA ou publicação. Readback de configuração não deve ser apresentado como identidade do processo já em execução.

## V4 — Menor privilégio e custódia

Operador NEW_INSTALL, processo operacional e owner de custódia CA são papéis distintos. A CA permanece no owner administrativo canônico com chaves e contexto protegidos. O serviço não recebe private-key open/use/sign/export, handle CA privado, token administrativo em cache ou API genérica de assinatura. Reusar referências públicas/protegidas com finalidade e lifetime, nunca material privado na CLI.

Qualificar ACL por recurso: leitura/execução de binários e configuração permitida, escrita somente nos dados/logs/objetos operacionais estritamente necessários; sem escrita operacional nos binários ou concessão genérica sobre a root administrativa. Leitura de dados não publica CURRENT. Chaves de outros propósitos somente por seus contratos; nada autoriza acesso privado CA. Provas exigem readback e testes de acesso sob tokens reais administrativo/operacional, incluindo negação KSP. Conta virtual não garante por si só isolamento ou bloqueio de rede. Nenhuma ACL produtiva é alterada por esta aceitação.

## V5 — Lifecycle e efeitos parciais

Reutilizar SCM adapter, ServiceMain, handler, Stop bridge, owners e worker existentes. RUNNING somente com prontidão factual e todos os gates de admissão; STOP_PENDING não é quiescência; STOPPED não decorre de timeout ou inteiro de retorno. Preservar ownership/borrows/retained refs até fechamento factual. Falha após Create/configuração é efeito parcial ou UNKNOWN, não rollback presumido. Não iniciar antes da verificação; não repetir criação/deletar automaticamente em falha incerta. DeleteService solicitado não comprova remoção concluída e não apaga CA/root/config/dados/histórico.

## V6 — Compatibilidade e credenciais

Nenhuma senha de serviço em argv/config/ENV/log/instalador; Windows gerencia a conta virtual. Não exigir AD, nuvem ou provisionamento manual SAM. Não implementar SCM NT4/2000/XP/Vista nesta tarefa; preservar futuro adapter e execução direta/core C89 sem dependência de SID/Win32. API disponível ou build moderno não qualifica Windows 7. Ausência de capacidade nega, sem fallback SYSTEM/LocalService/conta local. Privilégios de rede/host, configuração e quotas não são ampliados pela seleção de identidade.

## V7 — ED11 e qualificação

Medir por target: SCM handles, buffers de configuração/SID/ACL, token, runtime, threads, stacks transitivas, CRT/provider/kernel, retenção e coexistência. Custos UNKNOWN não são zero; margens nominais não são admissão. Preservar limites Legacy2097152 e Selected3145728 somente com seleção explícita, envelopes e host admission; nenhuma quota aumenta. Testes DATA/modelo, LAB nativo e operação real são evidências distintas. Windows 7 e plataformas futuras permanecem não qualificados até provas reais.

## V8 — Gates de implementação e operação

Aceitação normativa não autoriza patch funcional. Após aceitação, exigir autorização funcional específica, hunks/preimagens revisados, builds e regressões. Não habilitar ps_binding_ready por constante, setter ou argumento. Instalação LAB real requer aprovação exata e isolamento; instalação produtiva não é autorizada. Nenhuma conta/chave/certificado/bootstrap/CURRENT/assinatura/recuperação, ACL produtiva, rede operacional ou remoção de guard é autorizada. ED11 PARTIAL; CURRENT_PUBLICATION, SIGNATURE_AUTHORITY e AUTHORITATIVE_RECOVERY permanecem DENY.
