# Plano executável de integração técnica — condicionado ao aceite T0–T9 revisão2

Este é plano de implementação, não aprovação executável ou comando de operação. Uma única decisão normativa exata é pendente. Após registro autorizado, continuar no escopo técnico já concedido, sem nova autorização para cada edição/build/teste. Não executar LAB automaticamente.

## Sequência fechada e arquivos/funções

| Componente | Alteração focal planejada | Prova exigida |
|---|---|---|
| tools/qualification/scm-n0-protected-single-powershell.ps1 | Preservar Pin, Hash-Stream, Lock-QualifierPath, Select-AuxiliaryCandidate, perfil EMPTY_DENY, operador/no-impersonation e console. Substituir somente bloco ProcessStartInfo/Process.Start/StandardOutput/WaitForExit por recurso nativo do MESMO supervisor. | PID/criação e Wait/exit-code via handle original; um PowerShell, nenhum GetProcessById para reanexar; confirmação/console unchanged. |
| tools/qualification/scm-n0-private-stdout-native.c/.h (novos, após aceite) | Extrair as primitivas de endpoint/identidade/overlapped/reader do protótipo para API fail-closed, contexto persistente sem autoridade. Compilar papacc_n0_private_stdout.dll e carregar no MESMO processo, sob pin de bytes e lock de arquivo. Nenhum processo/helper externo. | Aquisição pré-registrada, thread/generation/borrow controlados, recursos desconhecidos retidos; custo DLL/CLR/evento explícito no administrativo. |
| tools/qualification/scm-registration-lab.c e composição de includes | Introduzir adapter opt-in de writer explícito antes dos includes consumidores, somente no alvo de qualificação moderno e perfil03. Não renomear APIs stdio globalmente sem classificação de stream/target. | Default sem macro preserva todas as rotas, registro/Legacy/read-only prévios. Rejeitar client/bootstrap genérico; somente entrada03/read-only autorizável. |
| tools/qualification/scm-instrumented-cycle-lab.inc::main | Validar argumentos fechados/identidade do supervisor/endpoint antes de emitir protocolo nesse canal; manter ig_n0_readonly_command/ig_n0_inputs_ready/ig_approval, lab_operator, ig_boot, PROTECTED_BINDING, N0 e censo. Converter escritores alcançáveis para adapter preservando texto e erro. | Bootstrap não pula guardas nem altera precedência da autoridade; uma conexão; nenhuma autorização pelo ack. |
| Mesmo include::done/final receipt e bibliotecas compartilhadas de saída | Emitir IG_FINAL pelo mesmo stream; fflush e fechamento comprovados; erro incompleto mantém UNKNOWN; manter primeira falha canônica, não substituir pelo erro de transporte secundário. | Não concluir quiescência por EOF/exit/timeout. Sem fclose+CloseHandle sobre mesmo owner. |
| CMakeLists.txt | Nova opção OFF por default e link/helper somente no target LAB moderno; exigir read-only pins ON e positive OFF. Incluir arquivos no OBJECT_DEPENDS de targets de composição textual. | Sem feature em NT4/VC6/produto; sem fallback global; quatro configurações aplicáveis e guardas default testados. |
| Tests e pacote integrado | Reutilizar protótipo atual e extrair testes para os callers REAIS/adapter, não criar outro harness equivalente. | Casos finais rodam contra mesmas funções compiladas para integração; fixtures não contam como prova LAB. |

O inventário stdout-callsite-inventory.csv identifica104 ocorrências em arquivos scm*.c/inc, incluindo ramos mortos/outros targets. NÃO é prova de104 calls alcançáveis. Classificar por defines/include graph primeiro; printf/puts/fputs/fprintf/wprintf/fflush/fwrite precisam respeitar seu stream real. Preservar stderr e stdout enquanto roteia o stream explícito. Não redirecionar Read-Host/Write-Host administrativo. _dup2 é proibido; _fdopen não duplica; SetStdHandle é alias e não substitui FILE.

### Interface e binding fechados para a implementação

API nativa planejada, não existente ainda: ContextBytes/Initialize, OpenEndpoint, CreateOriginalQualifier, AuthenticateConnection, ReadChunk, QueryOriginalProcess, WaitOriginal, CloseChecked e QueryStatus. Contexto deve conter slots/gerações/primeira falha/freeze/pending/retention e buffer de leitura bounded persistente; ReadChunk só retorna buffer ao managed depois de conclusão factual. Não deixar GC mover um buffer emprestado com I/O pendente.

Binding managed planejado: tools/qualification/scm-n0-private-stdout-binding.cs → Pacc.N0PrivateStdout.Binding.dll, pré-compilado OFFLINE para o .NET Framework do PowerShell5.1 existente e pinado como dependência. Carregar assembly de bindings no mesmo processo; não chamar compilador C#/Add-Type source durante a operação, nem criar wrapper PowerShell, processo supervisor ou segundo owner. A toolchain existente do produto continua MSVC; esta é somente camada de interop moderna, sem alterar serviço/targets legados. Revisar a assembly para ausência de inicializador/side effect/auto-close oculto. Tipos não são carregados/ativados pela rota Review se isso violar seu contrato static-pins-only.

Conservar source/pins do helper PaccFileAcl histórico; novos artefatos recebem pins próprios no pacote sucessor. Nada disso está instalado ou operacional agora. O escopo normativo não exige adotar outra arquitetura depois do aceite: essas escolhas técnicas estão fechadas para execução, sujeitas somente aos testes de segurança/capacidade.

## Ownership e lifetimes

| Recurso | Owner | Lifetime/fechamento |
|---|---|---|
| Endpoint servidor | Contexto limitado no supervisor original | Criado não herdável, DACL explícita, FIRST_INSTANCE, local-only; não reconectar. Retido com erro/pending; close só depois de conclusão de I/O. |
| Evento OVERLAPPED e buffer/estrutura | Mesmo contexto | Preexistem a cada I/O; não reutilizar até GetOverlappedResult factual. CancelIoEx não é close. |
| Processo qualificador/hThread | Contexto retornado por CreateProcess | CriaçãoFALSE; hThread fecha uma vez no ponto qualificado; hProcess original preservado até término. Nunca reabrir PID para supervisão. |
| Processo supervisor aberto no filho | Contexto de bootstrap candidato | PID+criação/token/session/liveness contra plano; depois vínculo canônico original. Fechar após último uso demonstrado, sem permitir lacuna de autoridade. |
| Token temporário | Owner de aquisição, não borrower | Comparação completa planejada com contexto canônico; close comprovado antes da amostra final quando último uso permitir. |
| Handle cliente → fd → FILE | Exatamente um owner por estado | Transferências explícitas, sem duplicação. FILE retido após fclose incerto; nunca CloseHandle de retry. |
| Pins, path locks, dossiê, mutex, intent | Owners existentes, não transporte | Nenhuma transferência ao reader; não fechar/liberar por retorno do pipe. |
| Buffer managed e decoder | Supervisor existente | ASCII/hex atual; limite de linha4096 e total131072, drenar mesmo truncado, preservar recibos especiais. Sem ReadToEnd ilimitado, logger ou buffer paralelo. |

Read no handle OVERLAPPED não será entregue a FileStream síncrono por suposição: usar API nativa bounded e converter chunks/linhas no supervisor, ou provar uma alternativa de I/O com ownership equivalente antes de adotá-la. O evento persistente é custo adicional, não omitido para igualar a contagem antiga.

## Aceitação offline antes de atualizar pins

- Aprovação normativa byte-exata registrada antes da mudança funcional dependente.
- Source/default route gates: EMPTY_DENY; perfil read-only opt-in; rota Cycle/positive DENY; antigo nome de endpoint/attempt/pins/approval replay negados.
- Identidade: handle original/PID+criação não zero/token primário/sessão/TokenId/AuthId/ModifiedId conforme contexto canônico, nenhum override por booleano/argv. Endpoint local fechado/namespace estrito antes de Open; cliente errado/replay/troca de processo/encerrado/erro negam antes de payload.
- Console em um PowerShell5.1 x64 de teste já no console: Read-Host + desafio nativo, confirmação divergente, stdin/stderr, Escape, F10, saída redirecionada, console loss/host loss e erros de escrita. Nenhum LAB; teclado sintético só em console próprio do teste. Prova C anterior não substitui teste PowerShell.
- Output completo: registrar bytes/linhas/payloads/recibos do fluxo real; ASCII e UTF16 hexadecimal, vazios, stdout extenso, truncamento, flush/fclose, erro e receipt ausente. Não prometer Unicode arbitrário/transcript/custom host.
- Overlapped: connect/read/write imediatos e pendentes, cancel race, timeout, falha parcial, buffer lifetime, controle de geração, freeze, zero aquisição após UNKNOWN. Retenção do processo/pins/locks sem kill/rollback.
- Modelo positivo existente, A1/B1/self-token, N0/RAW1, freshness60s/final1s, quinto processo,128/1024 e limites de memória preservados. Testar sem elevar quotas.
- Quatro builds Selected/Legacy Debug/Release zero warnings conforme B+D R7, usando os scripts/perfis aplicáveis já documentados (a1-functional-ownership-reservation-20261024/build-*.cmd como referência de toolchain/perfil). Não confundir Legacy profile moderno com portar a API para NT4.

## Pins derivados e pacote seguinte

Após os testes acima, calcular SHA256 reais de launcher/helper/qualificador/fonts/políticas e dependências. Preservar hashes/approvals históricos byte-exatos. Atualizar apenas pins derivados do pacote NOVO, nunca substituir aprovação consumida. Perfil compilado não concede admissão.

Novo pedido deverá autorizar explicitamente o endpoint transitório local, aquisições/fechamentos e consulta read-only do owner, sem intent/SCM mutation/Start. Incluir T0–T9 accepted e toda composição N/L/H/P3/RAW1/N7, host/boot/operador atuais e evidência de substituição protegida; dados runtime futuros permanecem desafios, não valores fabricados. Aprovação executável e comando literal só depois da aprovação operacional específica desse novo pacote.

## Reversão técnica antes de ativação

Se build/test/revisão falhar, manter feature OFF e perfil vazio/positivo DENY. Preservar artefatos de falha e primeira divergência. Para desfazer um patch técnico ainda não ativado, verificar hashes atuais contra o patch/preimagens e reverter só os hunks próprios; nunca reset/checkout genérico sobre WIP. Não apagar históricos, binários aprovados, arquivos protegidos ou recibos. Se os bytes atuais divergirem, STOP de reversão para não sobrescrever WIP. Não há rollback automático de objetos Windows ou efeitos LAB nesta política.

## Limites da observação e orçamento

Registrar por processo, fase e sobreposição: supervisor+endpoint+evento+DLL/CLR e qualificador+stream+token queries; medir PSS/stack separadamente. Não subtrair economia12 do fixture dos126 históricos. Estado144+scalar4 bytes não são memória total. Parent/runtime warm-up80 e child52/54/53 não são capacidade da composição real. Custos UNKNOWN, reservas históricas e H/P3 preservados; nenhum grant/credit/refund. Se novos recursos não couberem, DENY antes de consumo. Uma observação read-only futura demonstra somente a observação, não o ciclo positivo.

## Ponto único de decisão

Aceitar normativamente T0–T9 revisão2 e sua emenda exata a N5 é o próximo gate. O escopo técnico já autorizado continua após esse registro, sem novos ciclos artificiais de autorização técnica. Não há pedido de Start/Prepare embutido nesta decisão.
