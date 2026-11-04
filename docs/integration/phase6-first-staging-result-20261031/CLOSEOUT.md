# Resultado do primeiro staging limitado — reconciliação e diagnóstico offline

Fonte: saída integral de console fornecida pelo proprietário nesta conversa, transcrita em OWNER-PASTED-RESULT.txt; não captura assinada/atestação de imagem mapeada. Task continuada de reconciliação autorizada. Branchmain/HEADf341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Resultado factual

| Propriedade | Evidência e alcance |
|---|---|
| Invocação manual | Wrapper INVOKED=True, exit2; não repetida pelo agente |
| Aprovação | Pedidoa9280bc14623dfdf23e4c67caf65b4baa58d0109c34afd348ae7223e5c173d16 / registrofa7c3c1da45159a562057d354ada43df10746b9a9e376ad02d9923b56ce5b11f UTILIZADOS, não reutilizáveis |
| Primeira falha |5, localização5, antes de sample2 e de fontes/volume/READY/nonce/presenter |
| READY | ready_frozen=0; critérios0 significam NÃO CONGELADOS, não seleção B3 perdida ou limites aprovados zero |
| Consumo/efeitos | consumed=0, effects=0: nenhuma submissão criadora registrada; aprovação administrativa usada é distinta do take nativo |
| Close | known=1, retained=0, controlled_stage_refs=0 após fechar; WRAPPER_CLOSE_UNCERTAIN=False |
| OS | Quiescência/charges internos UNKNOWN; effects0 não significa ausência de cache/metadata OS |
| Contexto observado | SID1108/TCORP-TOBIAS/boot237900 informados pelo wrapper, metadata não autoridade; contexto preparatório nativo alcançou o fluxo posterior ao open/consulta, não é instalação pronta |
| Recursos | Uma amostra fase1:109handles/private2813952/WS6574080/físico7329312768/commit10820751360; pisos iniciais satisfeitos no sample, sem prova de pico posterior/capacidade completa |
| Volume | volume_known=0/missing83; unidades, espaço, R e gates2R/R não alcançados; zeros não são espaço zero observado |
| Descritores | parent168/nova proteção124/scratch_peak392bytes; comprimentos não são conteúdo ou custo máximo do alocador |

O first_predicate0/native_error0 refere-se ao diagnóstico do contexto nativo, não à aprovação da política de proteção do volume. Fontes foram pinadas pelo wrapper; aquisição/hash/readback pelo stager não alcançados. Não existem arquivos copiados/qualificados ou autorização de registro SCM decorrentes deste resultado. Código nunca chegou a pedir confirmação do staging.

## Localização e causa que se pode afirmar

No artefato aprovado, localização4 abre os componentes ProgramFiles e valida seus objetos/descritores. Localização5 verifica ancestor_count e chama staging_descriptor(n->descriptor,1), depois tentaria sample2. O n->descriptor vem de GetSecurityInfo do parent passado a personal_native_open; neste caminho parent é o diretório raiz do volume derivado de ProgramFiles (drive C:\ no layout planejado), não o futuro diretório Papinho/Accelerator. Scratch392 mostra materialização de descritor de ancestral antes desse ponto; contagem de referências0 é posterior ao close.

A saída completa sem sample2/query-error, em conjunto com o fluxo, localiza a negação na guarda do descritor da raiz da travessia. O5 pode ser gerado pelo fallback local ERROR_ACCESS_DENIED após predicado falso; NÃO identifica por si falha de API/Windows nem ACE culpada. Guard de owner/DACL/SID/tipoACE/rights ainda não era discriminado. Não há bytes do SD, owner/ACE/mask na evidência recebida. Motivo interno exato permanece UNKNOWN. Não assumir que é Authenticated Users/create-directory, owner errado ou ACL realmente insegura só por parecer configuração comum do Windows.

Não é repetição demonstrada do bloqueio132: o processo avançou além do antigo corte de layout, mas só109 foi amostrado. Não atribuir109 às fases posteriores ou declarar pico total109. H_READY+5 e growth9094144 não chegaram a ser congelados; nenhuma nova seleção quantitativa é necessária para explicar isso.

## Correção offline permitida concluída

A deficiência demonstrada é o diagnóstico genérico do predicado. Refatorado somente o MESMO teste em staging_descriptor_classify + wrapper booleano equivalente. Mantidas a mesma whitelist de owner, write_mask, regras de ACE/DACL, INHERIT_ONLY, aplicação e negação de unsupported. Nenhuma permissão nova ou relaxamento para produzir PASS.

Primeira classificação rejeitada guarda reason/ace_index/access_mask privados no packet e em saída não secreta, sem SD bruto/SID/secrets. Primeiro motivo não é sobrescrito por falha posterior. Reason0 significa não rejeitado ou não alcançado, não prova de segurança. Enum:1SDinválido;2queryowner;3ownerinválido;4SIDplataforma;5queryDACL;6DACLinválida/nula/ausente;7ownernãoconfiável;8ACEinválida;9tipoACEunsupported;10SIDdeACEinválido;11escritadeprincipalforadawhitelist;12políticadeobjetoaplicação. Falha de owner/DACL antes de iterar não tem ACE culpada; MAXDWORD é sentinela sem índice, mask0 não prova ausência de direitos.

Metadata nova NÃO é resultado nativo desta tentativa. Só poderá explicar uma observação futura especificamente autorizada; não preencher retroativamente a causa desconhecida. Admitir/rejeitar continua pelo wrapper equivalente; diagnóstico não concede capacidade. Nenhuma ACL/SD do equipamento foi consultada ou alterada pelo agente.

## Qualificação e pins

Quatro builds Selected/Legacy Debug/Release serializados/parallel1 PASS sem warnings (/W4/WX);17CTests/configuração,68execuções PASS. Staging57cenários/185checks e recursos53casos/184checks/configuração, mais regressores existentes. Novos casos sintéticos: SDválido, escritaWorld negada, ownerWorld negado, DACLnula negada, equivalência booleano/classificação e preservação da primeira causa. SD/SID parsing/SHA em memória reais; consultas de alvo/IO/token/RNG/recursos mutantes modeladas. Não houve nova coleta, staging, SCM ou efeito protegido.

CensoSelected1.988.936 geridos (antes1.988.928), sem quota aumentada; automáticos adicionais nomeados. Fonte/pins/logs/preimages em HASHES.json/BUILD-VERIFICATION.json. Artefato diagnóstico novo SHA256f296cd0003a278035a3eee8309aaaa8fb55402df91b1c305e9defa3e93615475, NÃO aprovado para execução. Pacote aprovado59e8808e... e DLLs preservados byte-exatamente antes do rebuild em build/staging-observation-preserved-20261031/approved-a9280bc1. Cópia preserva bytes, não fileID/originalidade operacional.

Pedido, manifesto e script aprovados ficaram imutáveis. Registrofa7... também imutável; AUTHORIZATION-USE-RECONCILIATION.json registra uso separadamente. O comando antigo NÃO deve ser repetido: além de autorização utilizada, o novo binário não corresponde ao pin do pacote aprovado. Não substituir pins do pedido consumido para rodar novamente.

## Primeiro gate restante

Antes de outra operação: determinar o submotivo real da rejeição do descritor da raiz e confrontá-lo com a obrigação de proteção aplicável à raiz de travessia versus ao parent/diretórios do alvo. Não alterar ACL de C:\, ProgramFiles, pasta de dados ou arquivos para superar o guard. Não presumir que toda criação de outro filho num ancestral permite substituir o filho protegido; nem dispensar DELETE_CHILD/WRITE_DAC/WRITE_OWNER/reparse sem prova.

Primeiro usar evidência existente autorizada, se houver, com identidade/proveniência e conteúdo suficiente. Se inexistente, a menor observação seria metadata/classificação do SD original da raiz/ancestrais usando o predicado já qualificado, read-only, sem fonte de CA ou tentativa de criar. Ela exige pedido/artefato e aprovação próprios; não é retry sob a autorização consumida e não precisa repetir staging mutante para aprender o motivo. Nenhum pedido/execução novo é aprovado ou preparado prematuramente nesta reconciliação.

Somente após esclarecer a causa, uma correção técnica fundamentada ou decisão focal realmente necessária pode preceder novo pedido. Não criar ADR/owner/root, não dispensar proteção porflag, não repetir números/TPM/Q4. Nenhum próximo marco SCM está pronto: staging não ocorreu. E21 PENDING; ED11/Phase6 PARTIAL; staging DENY. UNKNOWN, HEAD/WIP/históricos/pins/reservas/chave fixture e PST0.6.4 preservados.
