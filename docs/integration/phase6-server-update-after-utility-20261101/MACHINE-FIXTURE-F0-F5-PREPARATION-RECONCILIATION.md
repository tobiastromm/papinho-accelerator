# Reconciliação — decisão fechada, fixture negada por crescimento

2026-11-03. Fonte integral byte-exata OWNER-MACHINE-FIXTURE-F0-F5-PREPARATION-OUTPUT.txt, copiada do attachment210dfafa-e5c9-43de-8040-b93da4ae8fb1 sem edição, SHA256140bffdd6e61a25759713cc2351215b22320047a9f7cc48b49945e78e3ade2e3. Resultado MACHINE-FIXTURE-F0-F5-PREPARATION-RESULT.json/SHA256adc2791779810dc6e42de47763decb625de7ad831968d7e72109a9240227f157. Nenhuma repetição ou consulta real adicional pelo agente.

## D1 concluído

Nova decisão protegida criada exclusivamente em MACHINE-TEST-ONLY-FIXTURE-F0-F5-DECISION.ORIGINAL.txt: ID417614100:5177344:169433,4497bytes, SHA256e2e47dc1062f26b5952bddc68e720426210d66bb9148419b06562bb1f90118f3. FirstNONE/consumedTrue/created_mask1/verified_mask1/close_knownTrue/exit0. Não recriou pedido histórico nem decisão antiga. Preservar essa nova cópia também, sem write/delete/rename/ACLedit.

## F negado antes dos efeitos da chave

Adquiriu originais, cadeia e RNG; exibiu plano f3810cff7ca60c8aff41ad7cbd8bb58a4137936ec8a829be3ac6aeb973bc1ef8 e recebeu a confirmação F. Revalidação posterior negou first1816/source_stage11, ANTES do InterlockedCompareExchange de source_consumed e ANTES de papacc_ca_key_fixture_create. Consumed0/created0/positive_control0/identity_known0, close_known1/persistent_key_created0; document_calls4/chain_calls5/rng_calls3. Não houve criação/controle positivo de chave nem READ/E/Q/S/P/Start.

O nome PapinhoAccelerator.CA.e7bcdeef492b2d74085b0f85ff9b71e0.5395902b8cc02f2e6b307fa62e0f43a0 foi apenas candidato gerado e mostrado, NÃO identidade persistente criada. Não o adotar/reusar como fixture; publicSHA é desconhecido e não foi fabricado.

## Primeira causa delimitada com evidência nova

Budget: attempts6/samples6/query_error0/query_stage0; inicial116handles, último143; físico8441376768, commit10500542464. A aquisição inicial e a consulta posterior tiveram sucesso.143<188/192, físico>7239172096 e commit>3221225472. O budget_begin já tinha passado; o primeiro budget1816 é sticky, sem nova amostra após falha. Timeout/Stop têm outros códigos.

No código instalado server_ca_key_win32.c, o retorno1816 dessa última verificação só pode resultar, portanto, de private_growth>1994944 OU working_growth>1994944, ou ambos. Isso demonstra a classe de predicado da tentativa atual; não determina magnitudes, qual métrica ou atribuição a API/alocador. Os quatro valores private/working baseline/last não foram exportados e permanecem UNKNOWN. Não é falha de consulta, limite de handles ou piso de físico/commit nessa amostra. Não presumir vazamento. O1816 mais antigo não recebe esta conclusão retroativamente.

Revisão offline do trecho relevante: referências/SDs originais normativamente retidos, scratch temporário liberado, BCrypt hash/provider fechados uma vez nos caminhos conhecidos; nenhum fechamento adicional seguro ou vazamento funcional demonstrado. Não adiar guarda/rebase/fechar original ou cache interno para obter PASS.

## Não conformidade de apresentação e correção offline

A integração anterior de F0–F5 deixou o texto de confirmação desatualizado: cap128/precall124/commit9823326208 e referência genérica a pedido não consumido, apesar dos predicados e plano compilados F0–F5. Esse texto diverge do pedido aprovado e deveria ter sido corrigido antes da operação; não é prova de execução dos limites antigos. Não reescrever a saída, não tratar a confirmação divergente como validação de política antiga e não promover essa tentativa a qualificada. Nenhum efeito F ocorreu; D1 teve seu escopo separado e fechou.

Correção técnica já integrada offline: texto agora mostra entry128/cap192/precall188/private-working1994944/physical7239172096/commit3221225472 e distingue pedido histórico DATA de NOVA decisão efetiva. O teste do source intercepta a projeção real e exige essa política/escopo ANTES de aceitar a confirmação. A falha anterior de cobertura do texto fica registrada aqui.

Também exporta private/working baseline/last já capturados, com sample_known explícito e sem nova consulta, buffer ou autoridade. Esses campos futuros NÃO recuperam dados desta tentativa. Guards/quota/ordem/baseline/deadlines não mudaram. Nenhuma operação para testar a nova saída foi executada.

MACHINE-FIXTURE-PRESENTATION-OFFLINE-QUALIFICATION.json/SHA256efa36b6be45d0d3d0d4dae6b1f3061cab4fdfbf622bccbf3cc096fb5001b3348: quatro builds Selected/Legacy Debug/Release, zero warnings, quatro regressões afetadas por configuração/16pares PASS (sourcefixture, callerCLI, lifecycle READ, censo). Campanhas anteriores não repetidas. ROOT380408/KNOWN2007456/K3071004 preservados. Hash do arquivo de budget comparado ao manifesto F0–F5 confirma predicados inalterados.

## Estado e primeiro gate

Instalado continua última imagem871936/ID417614100:5373952:164063/SHAe4a36490737eb7e1896997f873e5e69511a7a6a65291b0f44399b4ed18f3b676; não houve UPDATE novo. Candidato offline de apresentação tem MESMO tamanho871936 mas SHA DIFERENTE f6f1513e2316a0d0a80744122880355fdbf43d836c69c0f0a22ebbc3ec8b8982; não confundir ou instalar com pins antigos. Snapshot papacc_server.fixture-presentation-871936.exe. Não há novo pedido UPDATE executável para intermediário de instrumentação.

Sessão D1+F utilizada e não reutilizável, ainda que Fconsumed0. Snapshot pendente preservado, estado posterior em MACHINE-FIXTURE-F0-F5-PREPARATION-AUTHORIZATION-USED.json. Nenhuma repetição/cópia/key/retry/cleanup autorizada. Originais e decisões anteriores permanecem preservados. Close conhecido desta operação não resolve application-close/retenção/reservas/quiescênciaOS históricos UNKNOWN.

Primeiro gate: decidir focalmente se deve manter o crescimento atual ou aceitar outra tolerância explícita. Proposta MACHINE-FIXTURE-GROWTH-ENVELOPE.CANDIDATE.md/SHA256fa443a1cc70011ccd60ed40a602df45c13acb59d4ae1c0fe48cd2667436f8000 oferece A/manter ou B/R0–R4, privado/working16MiB cada, estritamente fixture Selected, sem afirmar suficiência. Ainda NÃO aprovada/implementada. A cláusula F1 accepted preserva1994944 e exige decisão antes de mudança. Não subir limites por inferência da amostra.

Se B aprovada: registrar norma separada, integrar/qualificar só delta quantitativo, consolidar apresentação e números em UM candidato final, só então preparar UPDATE governado para autorização própria. Depois obter nova decisão operacional fixture por mecanismo existente, uma cópia auxiliar exclusiva em leaf novo explicitamente delimitado, sem sobrescrever o atual; só após fixture/controle/close reais preparar READ E/Q/S/P independente. Se A, conservar DENY e não repetir ensaios para tentar sorte.

Próximo prompt NEXT-MACHINE-FIXTURE-GROWTH-DECISION.md. Preservar HEAD/WIP, EXEs/DLLs/SCM/ProgramData/CA-CURRENT/históricos/pins/consumos/PST0.6.4; sem coleta, UPDATE, KSP/fixture/cópia real, endpoint/SCQ/P/StartStop, ACL-SCM, rede, TPM/NV/Q4, limpeza/refund/kill ou produção nesta continuação.
