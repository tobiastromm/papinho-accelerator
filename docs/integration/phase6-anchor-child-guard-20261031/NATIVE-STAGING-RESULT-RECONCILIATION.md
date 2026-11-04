# Staging corrigido — reconciliação da execução manual

## Resultado e alcance

PASS no alcance do ensaio PERSONAL_LOCAL LIMITED_QUALIFICATION informado: QUALIFICATION_COPIED_VERIFIED_CLOSED, consumed=1, effects=5, first=0, retained=0, native exit=0. Fechamento controlado conhecido, zero referências controladas restantes e wrapper sem close incerto. A autorização específica 72fb886f.../0703ffe9... está UTILIZADA e não pode ser repetida; a exceção process-only foi delimitada a essa invocação. Registros de aprovação anteriores permanecem consumidos.

Fonte: saída integral fornecida pelo proprietário, preservada em OWNER-STAGING-RESULT.txt; não é captura independente, assinatura do OS ou atestação criptográfica da imagem mapeada. O hash da transcrição local apenas identifica o arquivo arquivado. Não houve execução ou consulta adicional pelo agente.

## Identidade, confirmação e efeitos

SID do operador S-1-5-21-785699550-1440458426-1682014700-1108, host TCORP-TOBIAS; evento de boot 237900/2026-10-26T21:35:17.6469098Z é metadata, não horário da operação ou autoridade. A confirmação exibida e a entrada fornecida coincidem no identificador público de tentativa `152772afab13de6658a1dd7c7e79d9dabfeb65b7824b81f8d805e30e9c1b00f4`. Ele está consumido, não é segredo/grant ou identificador reutilizável.

O presenter exibe barras como `\x5c`; isso é escaping do token de console, não prova de um caminho contendo literalmente x5c. A fonte apresentada corresponde ao pacote Selected/Release aprovado; destino lógico C:\Program Files\Papinho\Accelerator. O CWD final do console é o diretório anterior do proprietário, sem substituir o source/target absoluto do procedimento.

| Arquivo apresentado | Bytes | SHA256 aprovado |
|---|---:|---|
| papacc_server.exe | 571392 | c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

No caminho qualificado, effects=5 e sucesso significam duas criações exclusivas de diretórios (Papinho/Accelerator) e três arquivos copiados, com flush, tamanho/hash/readback e descritores verificados antes de fechar. Os hashes impressos na tabela são do plano; a saída não imprime novos digests pós-cópia ou FileIDs de cada objeto. A inferência de verificação final depende do caminho qualificado e de seu resultado, não de uma segunda leitura independente do agente. Esses arquivos/diretórios são efeitos persistentes de qualificação, não instalação/CA/root operacional.

Os FileIDs correntes do staged e continuidade após fechamento/restart não estão publicados nesta saída. A futura operação SCM deve adquirir/revalidar originais frescos; não inventar IDs nem repetir staging para obtê-los. Guard root/child não rejeitou no caminho concluído; reason0 isolado também pode significar não alcançado, mas aqui há fase de confirmação, cinco efeitos e conclusão do fluxo. Não declarar nova norma ou ACL universalmente segura por esse campo.

## B3 e recursos

| Critério | Valor observado | Avaliação no escopo amostrado |
|---|---|---|
| READY handles | 166 | limite congelado 171 = READY+5 |
| Pico handles | 171 | atingiu exatamente o limite; sem folga adicional no pico |
| READY PrivateUsage | 4.079.616 | limite 13.173.760 = READY+9.094.144 |
| Pico privado | 4.612.096 | crescimento sobre READY 532.480; abaixo do limite |
| READY working set | 8.962.048 | limite 18.056.192 = READY+9.094.144 |
| Working set pré-efeitos impresso | 10.502.144 | abaixo do limite; pico global não impresso |
| Mínimo físico disponível | 6.715.600.896 | acima do piso 6.296.461.312 por 419.139.584 |
| Mínimo commit disponível | 9.507.323.904 | acima do piso 8.479.985.664 por 1.027.338.240 |
| Payload lógico | 9.110.528 | corresponde ao pacote aprovado |
| Cluster/R | 4096 / 9.117.696 | arredondamento por arquivo confere; pad total 7168 |
| Espaço caller/global reportado | 2.130.935.808 cada | snapshot reportado acima de R; valor inicial separado não impresso |
| Amostras | 2242 | observações por gates, não teto físico/OS |

R: ceil(571392/4096)*4096 + ceil(1321472/4096)*4096 + ceil(7217664/4096)*4096 = 9.117.696. 2R = 18.235.392. Código qualificado exige 2R antes do primeiro efeito e R depois, com revalidação; espaço livre não é reserva nem bound de metadata.

Importante para o tempo: CONTROLLER_RESOURCE phase=6/sample=6 é a observação em `staging_apply_composition` ANTES do consumo/criações, não a amostra de fechamento. Do primeiro qpc_begin a esse qpc_end decorrem 12,3310553 segundos. Não declarar esse valor como duração total. O código executa verificação QPC após close contra 120 segundos e só então emite sucesso, mas o QPC final não está impresso. Do mesmo modo, freshness500ms é verificada nos gates; não há máximo de freshness medido publicado nem todas as 2242 amostras impressas (phase10 tem sink suprimido). Sucesso/first0 sustentam aprovação desses predicados no fluxo qualificado, sem inventar métricas ausentes.

missing=19 = envelope nativo + margem host + custos ativos filesystem continuam UNKNOWN quanto à garantia integral, conforme B0–B8/B3. FS_METADATA=UNKNOWN e overhead do alocador UNKNOWN são esperados nesse perfil limitado, não zeros nem concessão produtiva. Pico privado não é memória gerida e não deve ser comparado como se fosse a quota gerida Selected3MiB. Nada aumenta quotas ou transfere 124/128 de outro domínio.

## Fechamento e pendências preservadas

Close controlado conhecido, retained0, controlled_stage_refs0 e WRAPPER_CLOSE_UNCERTAIN=False. Não há evidência de limpeza/refund, nem necessidade/autorização para apagar os objetos. Quiescência interna OS, bounds universais, proteção contra admin/OS comprometido, continuidade após reboot e autenticidade de imagem mapeada permanecem UNKNOWN/não demonstrados. Não transformar esse PASS limitado em ED11 total ou readiness produtiva.

Sem defeito técnico demonstrado nesta saída: nenhuma alteração funcional ou repetição de builds/testes foi necessária para reconciliá-la.

## Próximo gate real

Preparar a qualificação dos originais staged e do ato SCM_REGISTER_CLOSED, no owner existente, sem executar o registro. `server_launcher_scm_registration_win32.inc` já adquire/revalida o diretório de binários e imagem; esse caminho precisa receber as identidades frescas e hashes, não os dados de plano como autoridade.

O registro permanece sob admissão própria: `server_scm_registration_resources_win32.inc` ainda reporta classes materiais pendentes, e `server_service_win32.c` nega antes de OpenSCManager quando `ps_registration_binding_ready()` falha. B0–B8 é exclusivo de staging; não aplicar automaticamente sua dispensa opaca, orçamento B3 ou limite171 ao registro SCM. Rastrear o requisito específico e o primeiro predicado real; não exigir serviço futuro/M1 ou CA para adquirir o alvo existente e não abrir nova campanha de coleta por rotina.

Até resolver os gates técnicos/normativos próprios e houver novo pedido aprovado, não emitir/comandar `service install`, não instalar/iniciar serviço, alterar ACL/SCM, criar endpoint/CA/CURRENT, acessar TPM/NV/Q4 ou executar cleanup/refund/recuperação. O próximo prompt concentra a aquisição/readback já existente e a admissão específica de registro parado, sem nova root/owner/ledger/ADR por inferência.

Estado: staging limitado PASS no alcance observado e autorização consumida; registro SCM NÃO AUTORIZADO; E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 preservado.
