# Aplicabilidade do descritor da raiz — 2026-10-31

## Resultado factual

A evidência preservada permite identificar uma condição histórica concreta que reproduz a rejeição estrutural: ACE ALLOW de Authenticated Users (`S-1-5-11`), máscara `0x00000004`, aplicada ao próprio `C:\`. No descritor arquivado, é a ACE de índice zero-based 4. Ela intersecta `write_mask=0x500d0156`, e o trustee não pertence à whitelist operador/SYSTEM/Administrators/TrustedInstaller. O resultado correspondente é `STAGE_SD_FOREIGN_WRITE`, motivo 11.

Isso não prova retrospectivamente o submotivo da tentativa que falhou. A tentativa não imprimiu o descritor, seu hash ou a ACE; `first=5/location=5` localiza a negação na guarda da raiz, mas não identifica sozinho a cláusula interna. Não há continuidade demonstrada entre o objeto histórico e o handle daquela execução. Staging permanece DENY.

## Origem e integridade da evidência

Fonte: `../n0-n9-protected-raw1-input-placement2-20261024/root-readonly.json`, SHA256 `319bd8a2bb67190f08e097be4b26367195c911ca30acdf75f7dc845fd3962f45`. Seu uso aqui é exclusivamente factual; não transfere autoridade N0/LAB para staging.

- Identidade nativa histórica: `417614100:327680:5`.
- Descritor bruto: 200 bytes; SHA256 `9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8`.
- Owner: TrustedInstaller; DACL presente e protegida; controle `0x9404`.
- Projeção OWNER+DACL derivada offline: 168 bytes; SHA256 `dd12fd358f9aec2929310ef1c7601e65a84c9960da68a7e46fd1a3a5f1a924c2`.

O comprimento de 168 bytes coincide com o reportado na tentativa, mas igualdade de comprimento não demonstra igualdade de bytes. A projeção não é nova observação Windows nem execução do classificador C. O replay de metadados e sua proveniência estão em [HISTORICAL-DESCRIPTOR-REPLAY.json](HISTORICAL-DESCRIPTOR-REPLAY.json).

| Índice histórico | Trustee | Máscara | Flags relevantes | Guarda atual |
|---|---|---|---|---|
| 0 | Administrators | 0x001f01ff | OI/CI | whitelist |
| 1 | SYSTEM | 0x001f01ff | OI/CI | whitelist |
| 2 | Users | 0x001200a9 | OI/CI | sem interseção de escrita |
| 3 | Authenticated Users | 0xe0010000 | OI/CI/INHERIT_ONLY | não se aplica ao próprio objeto |
| 4 | Authenticated Users | 0x00000004 | nenhuma | FOREIGN_WRITE |

Outra captura histórica ordena ACEs de maneira diferente. Índice 4 é propriedade deste descritor arquivado, não identidade universal da ACE nem previsão de uma coleta futura.

## Semântica e guarda

Em diretórios, o bit 4 significa `FILE_ADD_SUBDIRECTORY`; o nome de arquivo `FILE_APPEND_DATA` compartilha o valor. `FILE_DELETE_CHILD` é outro direito, `0x40`. Criar outro filho não demonstra, por si, capacidade de substituir um filho existente protegido. A análise de substituição deve considerar DELETE/DELETE_CHILD, WRITE_DAC/WRITE_OWNER, proteção do filho, herança, referências originais e reparse. Não se conclui que o bit 4 seja sempre seguro ou dispensável. Ver [constantes Microsoft de acesso a arquivos e diretórios](https://learn.microsoft.com/en-us/windows/win32/fileio/file-access-rights-constants).

`staging_descriptor_classify` em `apps/server/server_launcher_qualification_staging_win32.inc` é uma política estrutural conservadora, não AccessCheck do token: rejeita ALLOW aplicável com escrita fora da whitelist, ignora INHERIT_ONLY e não subtrai DENY de ALLOW. Ordem e grupos têm semântica própria no Windows; uma DENY não resgata uma ALLOW proibida nesta política. Ver [avaliação de DACL](https://learn.microsoft.com/en-us/windows/win32/secauthz/how-dacls-control-access-to-an-object) e [ordem de ACEs](https://learn.microsoft.com/en-us/windows/win32/secauthz/order-of-aces-in-a-dacl).

A raiz é adquirida por `personal_native_open` em `server_configuration_personal_native_win32.inc`; a localização 5 precede a observação seguinte e testa o descritor original da raiz de travessia. O erro 5 é a negação local resultante, não prova de erro da API GetSecurityInfo ou de ACL Windows incorreta. A whitelist de ancestrais admite TrustedInstaller; portanto o owner histórico não é o motivo reproduzido.

## Rastreabilidade normativa e escopos

| Autoridade accepted | Obrigação aplicável | Distinção necessária |
|---|---|---|
| F0–F9, especialmente F3/F5/F6/F8 | referências/parent protegidos, revalidação, exclusividade, admissibilidade e retenção | não estabelece esta máscara/whitelist integral para todo ancestral do OS |
| D0–D7, D5 | known folders, identidade e proteção dos ancestrais, ausência de reparse/substituição, criação exclusiva | não dispensa proteção nem define automaticamente a raiz do volume como diretório privado do aplicativo |
| B0–B8 | especialização limitada de garantia de recursos opacos | não reduz proteção/identidade do filesystem |
| decisão de destino de binários PERSONAL_LOCAL | novo caminho em ProgramFiles; preservação de instalações existentes | não autoriza mudar ACL da raiz ou adotar objetos existentes |

`C:\` é âncora de travessia do volume; ProgramFiles é o parent de `Papinho`; os novos diretórios pertencem a outro momento do fluxo. O descritor novo `O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)` e a exigência de DACL_PROTECTED na validação de objetos criados não devem ser confundidos com a validação de ancestrais existentes. A guarda de ancestral não exige por si esse flag; o histórico já o contém.

Há uma lacuna de fundamentação específica ao aplicar a proibição integral de escrita de diretório de aplicação à âncora existente do OS. Não foi demonstrado conflito entre dois contratos accepted nem autorização para dispensar o bit 4. Antes de eventual correção, é necessário separar criação de filho distinto de capacidade de substituição do caminho protegido. Nenhum contrato accepted foi alterado, nem a ACL foi declarada inadequada.

## Resultado da tentativa e autorização

Preservados `OWNER-PASTED-RESULT.txt` e `AUTHORIZATION-USE-RECONCILIATION.json` do pacote anterior. A autorização do pedido `a9280bc1...` foi utilizada e não pode ser repetida, mesmo com consumo interno 0 e efeitos controlados 0. Fechamento controlado conhecido, sem retenção; quiescência interna do OS UNKNOWN. A falta de READY/volume e os limites impressos em zero são estados não alcançados, não nova seleção B3 nem capacidade zero demonstrada.

## Correção e qualificação

Nenhuma mudança funcional foi feita: não há prova suficiente para relaxar o predicado. Foram lidos artefatos existentes e verificados offline bytes, hashes, projeção e interseção da ACE histórica. Não foi executado coletor, staging, consulta live de ACL, SCM ou operação protegida. Não foram repetidos builds/testes inalterados; os quatro builds e 68 CTests do diagnóstico anterior continuam evidência histórica, sem serem apresentados como nova execução.

## Primeiro gate e caminho mínimo

Falta uma observação atual, especificamente autorizada, do descritor/identidade da raiz e do motivo do mesmo classificador. Ela pode confirmar uma condição atual; não recupera o descritor não salvo da tentativa anterior. A [especificação mínima](ROOT-METADATA-QUERY.SPEC.md) não é autorização nem comando executável. Não usar o preflight completo de staging, que solicita direitos adicionais e já está associado a uma autorização consumida.

Depois: reconciliar os fatos e concluir a análise de proteção da âncora; corrigir somente se o fundamento técnico e accepted permitir, ou apresentar a única decisão focal realmente necessária; qualificar alterações afetadas; preparar novo pedido exato de staging. Registro SCM parado continua etapa separada, sem autoridade concedida por esta investigação. E21 PENDING; ED11/Phase 6 PARTIAL; produção PST 0.6.4 preservada.
