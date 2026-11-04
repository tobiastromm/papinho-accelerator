# Alvo staged e admissão do registro SCM parado

Estado em 2026-10-31: integração e qualificação offline concluídas no alcance abaixo. Registro SCM NOT_READY / DENY. E21 PENDING; ED11/Phase 6 PARTIAL. HEAD f341985924d29870d8ff627ed37c8a0874ad6469, branch main, WIP preservado. Nenhuma operação protegida executada.

## Evidência reutilizada

O resultado manual anterior em `../phase6-anchor-child-guard-20261031/OWNER-STAGING-RESULT.txt` é QUALIFICATION_COPIED_VERIFIED_CLOSED, consumed=1/effects=5/first=0/retained=0. Aprovação 72fb886f… utilizada; não rearmada. A evidência prova o caminho limitado de cópia/verificação/fechamento controlado, não atestação da imagem mapeada, FileIDs atuais ou quiescência OS. UNKNOWN preservado.

Destino existente esperado: `C:\Program Files\Papinho\Accelerator`. Não é uma nova criação a repetir nem uma root CA. Pacote esperado:

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 571392 | c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Esses pins não substituem aquisição original fresca. Uma cópia preservada em build/staging-observation-preserved-20261031/approved-72fb886f-corrected conserva bytes de referência, sem transferir FileID ou autoridade. O novo controlador compilado NÃO substitui o EXE staged e NÃO foi selecionado operacionalmente.

## Integração realizada

No mesmo owner/presenter SCM_REGISTER_CLOSED, a aquisição relativa passa a reter EXE e ambas as DLLs sob o parent original. Nomes fixos, READ_DATA/READ_ATTRIBUTES/READ_CONTROL/SYNCHRONIZE, SHARE_READ, FILE_OPEN e abertura não-reparse. Cada dependência exige identidade de volume/arquivo distinta, arquivo não vazio, descritor estrito, hash e continuidade; revalidação antes da confirmação/consumo e efeitos dependentes. Não se aceita path/hash isolado como autoridade. Os hashes observados são apresentados para confirmação atribuída; o futuro pedido/manual deve ainda fixar e conferir os pins exatos do pacote acima.

Handles adicionais e IOSB pertencem ao packet existente. Falhas/pending/fechamento incerto preservam owner/cohort e UNKNOWN; não há retry de close ou reabertura de referência consumida. LAB e seu opener não foram alterados. Nenhum endpoint, arquivo protegido ou serviço foi aberto pelos testes.

O alvo SCM fechado é resolvido por known folder e adquirido independentemente da imagem do controlador. O perfil produtivo conserva o comportamento anterior. Isso permite qualificar um controlador atualizado sem exigir novo staging do pacote já aprovado; um pedido futuro precisa distinguir os pins do controlador dos pins do alvo.

O presenter usa o layout binário existente: não exige ProgramData, root CA, CURRENT, runtime futuro ou M1 para registrar parado. Continuam obrigatórios token administrativo original, parent/ancestrais protegidos, identidade da imagem, conta virtual exata, propósito, confirmação específica, lifetime, consumo e exclusividade. Confirmações staging/SCQ/NEW_INSTALL não autorizam SCM.

O readback adiciona prova explícita de SERVICE_STOPPED, além de configuração/SID type. Query falha, tamanho incompleto ou estado RUNNING negam conclusão e preservam falha parcial; nenhuma chamada Start/Stop/DeleteService é acrescentada.

## Qualificação

Selected/Legacy × Debug/Release: quatro builds seriais PASS, sem warnings nos logs finais. Dez CTests afetados por configuração, 40 execuções PASS: original_registration, registration_resource_observations, registration_files, scm_virtual_model, qualification_staging, staging_resources, anchor_child_guard, root_chain_metadata, staging_close_guard e memory_census. Logs neste diretório.

O teste novo do opener possui 29 verificações, com NtCreateFile/CloseHandle simulados: direitos mínimos, nomes, diretório relativo original, ausência de dupla abertura/fechamento, falha/pending/retention. Ensaios do presenter cobrem DLL ausente, reparse, volume divergente, alias EXE/DLL, identidade/hash/ACL substituídos, perda de referência/captura e fechamento. Modelos SCM cobrem controlador em caminho diferente e readback não parado/falho/truncado. APIs mutantes, RNG e objetos originais nesses ensaios são modelados; não constituem prova nativa atual do destino ou SCM.

Houve uma invocação prematura de CTest Legacy/Debug enquanto seu build ainda estava em andamento: três executáveis ainda não estavam disponíveis (Not Run). Após terminar o build, os dez testes passaram. Não foi uma coleta operacional nem repetição de operação consumida.

Censo estrutural Selected/Debug: ROOT=362168, COMMIT=283872, PENDING=91560, KNOWN=1989216, PROCESS_LIMIT=3145728. É sizeof/censo, não medição de pico nativo ou reserva física. A reserva do presenter inclui as novas estruturas e frames automáticos nomeados; ABI/CRT/provider/kernel não ganham teto por essa soma. Legacy conserva seu limite próprio.

## Primeira lacuna restante: suficiência própria do registro

`papacc_server_service_registration_source_status` conserva ERROR_ACCESS_DENIED; após ato original confirmado reporta RESOURCES_UNQUALIFIED. Um relatório observacional válido deixa missing=63. Separação nova: pre_missing=31, effect_missing=32. Sem observação válida: 95/32. Essa separação é diagnóstico, não concessão: resultado de CreateService/readback só pode existir durante/depois do efeito, e não exige processo futuro antes do registro. As demais classes permanecem evidência/política incompleta.

Fontes normativas: DECISION-ed11-preserve-incremental-resource-envelopes-20261012 exige incremento e capacidade factual do host e nega evidência requerida UNKNOWN; W2/W4 de DECISION-ed11-windows-3mib-process-profile-20261020 conservam quotas e contabilização nativa/transitiva/retida; F8 conserva ED11. B0/B2/B6 da especialização LIMITED-STAGING-ASSURANCE excluem expressamente serviço/SCM. D0–D7 não criam política de registro. Portanto o staging PASS não resolve suficiência de loader/CRT, RPC/SCM, transientes, margem/retention do ato SCM.

Não foi encontrada extensão accepted específica de 124/128 ao registro SCM. O sampler compartilhado ainda mantém o guard conservador existente; não o removemos nem o declaramos norma universal. Substituí-lo depende de política própria fundamentada, não do pico171 observado no staging. Não importamos B3, seus pisos, 500ms ou 120s.

## Caminho mínimo

1. Resolver somente a garantia de recursos do registro parado: provisão integral fundamentada ou decisão explícita de especialização limitada própria, conforme alternativas no documento seguinte. Não pedir nova coleta genérica: uma amostra não demonstra teto opaco RPC/kernel.
2. Integrar/qualificar a política escolhida no owner existente. Gates frescos usam os originais do alvo existente e do controlador; não criar diretórios nem repetir staging. PID/token do futuro serviço não são pré-condição do registro parado.
3. Congelar controlador e alvo separadamente e preparar pedido operacional byte-exato de UMA criação SCM, configuração de SID type e readback STOPPED, sem Start/Stop, endpoint, CA/CURRENT ou efeitos de filesystem adicionais. Somente então solicitar autorização específica.

Não existe comando operacional pronto nesta entrega: a política material de admissão continua ausente. Ver [próxima decisão concreta](NEXT-SCM-REGISTRATION-ASSURANCE-CHOICE.md). Nenhum contrato accepted foi modificado.
