# ED11 — transporte local sem herança: implementação e qualificação isolada

2026-10-25. TASK=PACC-PHASE6-ED11-NONINHERITED-TRANSPORT-RESOLUTION.
Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservados. Estado anterior/preimagens e hashes em baseline.json. Nenhuma operação LAB foi executada.

## Conclusão

A falha específica de concorrência foi eliminada no mecanismo implementado e testado: o supervisor cria endpoint/evento não herdáveis, inicia o destinatário com bInheritHandles=FALSE e autentica a conexão. Um concorrente com herança global no supervisor e um descendente com herança global no filho não recebem os objetos privados, por comparação de identidade de objetos Windows. Não se depende de lock global ou da cooperação de outros CreateProcess.

Implementação funcional isolada: tests/noninherited_transport_test.c, alvo papacc_noninherited_transport_test no CMake. Nenhum novo serviço, processo-helper de produto, owner de autorização, logger ou Job Object. Os processos concorrentes adicionais são somente adversários sintéticos do teste. Os quatro papéis operacionais permanecem inalterados.

A integração canônica NÃO foi ativada. Existe GOVERNANCE CONFLICT concreto: [N5 accepted](../../../adr/DECISION-first-native-lab-diagnostic-boundary-20261024.md) ainda determina que “new IPC” não está autorizado na topologia. A regra do AGENTS.md exige armazenar a decisão durável aprovada normativamente antes de implementar código dependente. O owner autorizou este protótipo e a seleção técnica, mas reservou a aceitação normativa quando houver conflito. Por isso a exceção focal T0–T9 está preparada para decisão, com código e evidência reais; não foi registrada ACCEPTED por inferência.

## Arquitetura escolhida

1. Endpoint local aleatório criptográfico128 bits via BCrypt; nome não é credencial. CreateNamedPipeW com FILE_FLAG_FIRST_PIPE_INSTANCE, uma instância, PIPE_REJECT_REMOTE_CLIENTS e SECURITY_ATTRIBUTES não herdáveis.
2. DACL protegida explícita desde a criação: SYSTEM e operador exato. O operador recebe máscara0x00120183, sem FILE_CREATE_PIPE_INSTANCE ou permissões explícitas WRITE_DAC/WRITE_OWNER/DELETE. O descritor é lido do handle original e validado, sem normalização. Não se usou a DACL default que permitiria acessos adicionais. Isso cria a proteção do novo objeto kernel isolado; não altera ACL de arquivo/diretório/SCM. RAW1 original permanece intocado; esta DACL NÃO é apresentada como descriptor RAW1 de arquivo.
3. Filho nasce com herança FALSE. Supervisor retém PROCESS_INFORMATION.hProcess original, PID/criação não nulos/liveness; o hThread é fechado após a criação. Identidade do cliente vem de GetNamedPipeClientProcessId e deve corresponder ao original. TokenUser, AuthenticationId, TokenPrimary e sessão devem corresponder; falhas de consulta negam.
4. Filho identifica o servidor por GetNamedPipeServerProcessId e processo do supervisor mantido aberto durante o bootstrap, criação/liveness e contexto de token. Esse handle inicialmente aberto por PID é candidato validado, não substituição da autoridade canônica. Arquivo executável/host/boot/operador elevado/plano/RAW1/aprovação continuam exigindo todos os pins e provas existentes na integração futura.
5. Cliente conecta uma vez com SECURITY_ANONYMOUS; nenhuma impersonation é utilizada. Hello/ack de uma conexão autenticada são transporte, nunca admissão. Cliente errado causa DENY; não há reconexão para dar chance a outro destinatário. Nome/PID/nonce sozinhos não autorizam nada.
6. O endpoint torna-se o canal stdout. Stdin/stderr usam o console compartilhado; nenhum handle de console é duplicado cross-process. No teste, CONIN$/CONOUT$ são próprios do console oculto isolado, sem tocar o console do operador.

O servidor conserva a referência original do processo até término factual. Criação e identidade são revalidadas antes de aceitar o canal. Um PID igual com criação divergente não basta. Endpoint único/handle conectado evitam substituição textual depois da aquisição. Kernel/operador privilegiado confiáveis seguem o threat model existente; não se afirma resistência a administrador equivalente, injeção nos participantes ou Windows comprometido.

Referências oficiais: [proteção de named pipes](https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipe-security-and-access-rights), [CreateNamedPipe](https://learn.microsoft.com/en-us/windows/win32/api/namedpipeapi/nf-namedpipeapi-createnamedpipew), [PID do servidor](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getnamedpipeserverprocessid). O teste não presume que uma ACL dispense autenticar o peer.

## Alternativas rejeitadas

- HANDLE_LIST: contraprova anterior permanece válida; references privadas herdáveis vazam para outra criação global.
- Serialização local: não coordena criadores que não participem do lock.
- Helper-processo: acrescentaria papel/lifetimes e violaria a topologia atual.
- DuplicateHandle para destinatário: pode usar o processo original, mas ainda requer autenticação e bootstrap do valor remoto. Acrescenta referências sem vantagem aqui. A API não autentica um PID e [limita duplicação cross-process de console](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle). Foi escolhido que o filho abra o endpoint autenticado.
- _dup2 para retarget stdout: rejeitado. A fonte UCRT instalada dup2.cpp usa DuplicateHandle com TRUE e limpa FNOINHERIT. SHA256 da fonte inspecionada acef16d8e462a3730afbfbfc5e69188f82e1759c87978f820b5e0e0e22ff7dde. Não se corrige isso alterando flags depois de uma janela herdável.

## CRT, console e erros

O protótipo usa _open_osfhandle(_O_NOINHERIT|_O_BINARY|_O_WRONLY) seguido de _fdopen: ownership move de HANDLE para descriptor e depois FILE, sem duplicação. fprintf/fflush escrevem o protocolo de42 bytes com linha vazia e UTF16_HEX; GetHandleInformation confirma que o mesmo objeto continua não herdável. O teste do descendente ocorre DEPOIS desse binding CRT. SetStdHandle aponta para o canal como alias e é removido antes de fclose. Não existe CloseHandle adicional depois que FILE recebe ownership.

SetStdHandle sozinho não altera o FILE stdout do CRT. O protótipo usa stream explícito; não afirma que todos os printf/wprintf/fputs do qualificador real já foram redirecionados. O adapter futuro deverá cobrir esses emissores, truncamento4096/131072, fflush, primeira falha e recibos sem mudar o wire. Não é um novo logger.

Console real isolado: GetConsoleProcessList contém pai original e filho; confirmação por ReadConsoleW, stderr por WriteConsoleW, Escape e F10 por eventos sintéticos. Não declara associação autenticada de conhost nem altera N7. Não prova Read-Host/Console.ReadKey/transcript/console loss do PowerShell canônico. O console é criado oculto somente no fixture, não como bypass do censo operacional.

Fechar servidor antes da escrita produz erro CRT/Win32 original109 no teste, com saída do filho75 (código do fixture). Encerramento precoce produz bytes0/erro de pipe, não recibo de sucesso. O pai registra independentemente erros próprios e exit-code do filho; first=0 do pai não apaga erro do participante.

## Ownership, pending I/O e retenção

Endpoint e evento overlapped pertencem ao contexto persistente antes das aquisições. Evento/buffer não são reutilizados com pending. Em timeout, CancelIoEx é seguido de conclusão factual por GetOverlappedResult; cancelamento solicitado não equivale a conclusão. Wait/close incerto retém o processo/thread/contexto, imprime UNKNOWN e bloqueia o avanço; o runner registra PID e não mata em timeout.

Criação falha real de executável inexistente retorna Win322, nenhum filho; o connect pendente é cancelado e sua conclusão confirmada antes do fechamento. Colisão do endpoint foi negada com231 nas primeiras variantes e5 após restringir direitos de instância; não houve adoção ou retry.

Falha simulada de close no filho preserva HANDLE e FILE no contexto, primeira falha5 e freeze. O fixture encerra a SI PRÓPRIO com ExitProcess2 para conter o experimento sem executar fclose/CloseHandle de retry; pai vê exit2 e preserva o resultado como UNKNOWN. Isso não é política de encerramento do produto, prova de quiescência universal, refund ou autorização de kill. A integração real deverá usar a retenção canônica, não terminar o operador.

Nos caminhos com fechamento comprovado, todos os slots próprios do supervisor ficam0 e pending0. Não se converte esse fato em zero para kernel/cache/stack/transitivos ou referências desconhecidas.

## Qualificação executada

Selected Debug/Release PASS, /W4 /WX, zero warnings.24 cenários de processo (12 por configuração), cada um com5 iterações =120 execuções internas, resultados esperados PASS:

- clean: conexão correta, autenticação, CRT e fechamento;
- wrong: processo errado conecta primeiro, nenhum ack/canal aceito;
- bad-creation: criação esperada divergente, filho rejeita e conexão pendente termina sem efeitos;
- token: fault injection de comparação de token divergente, não uma execução real com credenciais de outro usuário;
- bad-server: identidade esperada do servidor divergente;
- early: término antes do payload, erro/bytes0 preservados;
- close-unknown: FILE/HANDLE retidos no filho, exit2 factual;
- descendant: criação global no filho após binding CRT não recebe o objeto privado;
- collision: segunda criação do endpoint negada, sem filho;
- create-fail: CreateProcess real falha2, cancelamento confirmado;
- server-close: escrita/fflush falha, erro original preservado;
- console: confirmação, stderr, Escape/F10 e contexto de console corretos.

Em todos os casos aplicáveis, um concorrente global no supervisor foi observado por PSS e CompareObjectHandles; zero correspondências com endpoint/evento privados. Não foi usada a coincidência de valores numéricos como prova. Não é prova de que nenhum processo com privilégio equivalente possa deliberadamente obter acesso por outro mecanismo.

62 regressões existentes PASS (32 cenários observador/tabela +30 do owner/recursos). As36 provas anteriores do mecanismo de herança não foram repetidas nesta etapa porque suas fontes/semânticas não mudaram; permanecem evidência histórica, não prova do novo mecanismo. Logs intermediários, inclusive a expectativa inicial incorreta de erro5 na colisão, foram preservados. O tratamento foi corrigido para registrar código real e negar colisão inequivocamente; a DACL posterior remove o direito de criar instâncias.

## Contabilidade e capacidade

Ledger factual por papel/fase em phase-resource-ledger.csv. Os processos e amostras NÃO são somados como um único budget ou pico simultâneo. A ordem física das linhas pode refletir buffering de stdout do fixture; não é timeline global de aquisições nem admissão fresca.

Filho clean:52 handles na entrada,54 após autenticação (pipe+processo do supervisor),53 depois do fechamento da referência transitória ao supervisor. Os tokens temporários já foram fechados nessa amostra; pico interno entre amostras permanece UNKNOWN. Abrir os dois canais de console no teste tem custo próprio separado. Descriptor/FILE não duplicam o handle do pipe.

Supervisor clean:52→80 na primeira criação de endpoint e79 ao fechar; segunda iteração79→82→80; iterações posteriores80→82→80. Inicialização Windows/segurança/BCrypt/PSS deixa custos não atribuídos, que não são declarados leak ou zero. Memória Private clean Release observada cerca de0,8–0,9MiB, sem tratar isso como orçamento da composição real. Estado NT_OWNER compilado144 bytes, além dos buffers/frames, SID/descritor, FILE/heap, bibliotecas e custos transitivos.144 NÃO é custo total nem incremento de quota aprovado.

Referências de transporte em steady state no supervisor: endpoint+evento+processo original (3), frente a pipe-reader+processo (2) na hipótese anterior. Autenticação tem tokens temporários; o adversário/PSS do teste tem custo próprio de observação, não custo de produto. No filho, endpoint1 estável e autenticação transitória antecedem N0. Não deduzir esses valores dos126 históricos nem creditar o operador para ultrapassar128 no qualificador.

Resultado: elimina-se a HERANÇA indevida demonstrada; NÃO foi demonstrada economia líquida no launcher integrado, nem capacidade positiva. As diferenças68→56/52→40 anteriores pertencem a outros fixtures e não se transferem automaticamente. As observações reais71/105/126 e pico amostrado128 continuam históricas, assim como administração984/1024. Preservados128/1024, quatro papéis,60s/1s, headroom, NONE_GRANTED, H/P3 e reservas UNKNOWN.

## Integração, pins e próximo gate

Não foi aplicada mudança ao launcher, qualificador canônico, recorder ou pins operacionais. Não há nova aprovação executável, Prepare, Start/Stop, intent, imagem/sink/arm ou alteração de SCM/ACL existente. O novo alvo é exclusivamente de teste Windows moderno, sem mudança nos componentes NT4/legados.

A proposta T0–T9 isola exatamente a decisão necessária: permitir o transporte local sem herança como exceção focal a N5, mantendo owner e todas as provas. SHA256 integral: 045b3edd03cb5841036eb047a62f54adee9763055429cfac6fc062d131257395. A ADR anterior de herança foi atualizada para revisão4 PROPOSED, apontando para essa alternativa; nenhuma norma accepted foi editada.

Após aprovação normativa específica e registro autorizado, a autorização técnica desta tarefa permite continuar a implementação do adapter no launcher existente e a qualificação PowerShell/dispatch completo stdout, sem microautorizações para rebuild/testes. Só então produzir novos pins e pedido exato de observação read-only, com custos do endpoint explícitos. Não há comando operacional seguro e aprovado para executar agora. Uma nova observação do launcher antigo não resolveria essa fronteira e não foi preparada.

## Estado final

CURRENT_TASK_COMPLETED=PARTIAL_PROTOTYPE_QUALIFIED_NORMATIVE_INTEGRATION_GATE
ARCHITECTURE=LOCAL_PRIVATE_PIPE_NO_INHERIT_MUTUAL_PROCESS_BINDING
PROTOTYPE=IMPLEMENTED_NATIVE_AND_CRT_QUALIFIED
CONCURRENT_INHERITANCE=ZERO_PRIVATE_OBJECT_MATCHES_IN_TESTED_PARENT_AND_CHILD_WINDOWS
DEBUG_RELEASE=PASS_ZERO_WARNINGS
NATIVE_CASES=24_EXPECTED_OUTCOMES_120_ITERATIONS
REGRESSIONS=62_PASS
CONTROLLED_STATE_BYTES=144_NOT_TOTAL_COST
INTEGRATED_HANDLE_REDUCTION=NOT_MEASURED
POSITIVE_CAPACITY=NOT_DEMONSTRATED
CANONICAL_LAUNCHER=UNCHANGED
OPERATIONAL_PINS=UNCHANGED
ADR=PROPOSED_T0_T9
NORMATIVE_CONFLICT=N5_NEW_IPC_REQUIRES_EXACT_DECISION
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE_EXISTING
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_DECIDES_T0_T9_THEN_CANONICAL_ADAPTER_WITH_EXISTING_TECHNICAL_AUTHORIZATION
