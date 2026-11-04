# Destino binário para novas instalações PERSONAL_LOCAL — alteração focal

Status: PROPOSED. Sem aceitação normativa ou autorização operacional.

Fonte vigente integralmente confrontada: DECISION-personal-windows-optional-scm-20261022.md. Sua cláusula de destino seleciona `%ProgramFiles%\PapinhoAccelerator`; demais cláusulas mantêm hosting opcional, runtime/owners compartilhados e ausência de efeitos pela decisão. A proposta anterior TARGET-BINARY-DESTINATION.CANDIDATE.md é preservada; esta revisão restringe explicitamente o alcance a novas instalações.

## Única especialização proposta

Para **novas instalações PERSONAL_LOCAL**, substituir exclusivamente a seleção do destino dos binários por `%ProgramFiles%\Papinho\Accelerator`, em lugar de `%ProgramFiles%\PapinhoAccelerator`.

Instalações existentes preservam caminho, identidade e configuração originais. Não mover, adotar, rebindar, migrar ou reinterpretar uma instalação existente como NEW_INSTALL por esta decisão. Ausência de arquivos não prova instalação nova.

Resolver ProgramFiles pela known folder do target; congelar path absoluto e adquirir a identidade original/proteção no procedimento autorizado. Não criar diretórios nem publicar binários por aceitação normativa.

Nenhuma outra cláusula da decisão de 2026-10-22 é alterada. Preservar SCM opcional/console, mesmo runtime/owner, alternativa SAM console e especialização virtual já accepted, dados/root independentes, P3/P4/F0–F9/M0–M11, CNG, quotas, Stop, UNKNOWN e históricos. Não alterar formatos persistidos nem autorizar instalação, CA, CURRENT, Start/Stop, SCM/ACL ou qualquer operação protegida.

Alternativa sem esta especialização: conservar o destino accepted anterior. Esta decisão pendente não bloqueia implementação offline independente de SCM/CNG, MANUAL/E8 ou enrollment.
