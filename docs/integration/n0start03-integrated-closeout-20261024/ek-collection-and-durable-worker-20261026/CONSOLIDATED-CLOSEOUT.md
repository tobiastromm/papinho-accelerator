# ED11 — coleta pública EK e worker durável isolado

TASK=PACC-PHASE6-ED11-TPM-EK-COLLECTION-AND-DURABLE-WORKER

Coletor manual pronto; worker durável implementado e qualificado em ambiente isolado. Nenhuma coleta TPM ou operação protegida foi executada. A integração publicada na root canônica continua não ativada: o fixture não modifica seu formato/worker nem substitui os requisitos L1. E21=PENDING; ED11/Phase6=PARTIAL; autoridades produtivas=DENY.

## Coletor e procedimento manual

Revisado o candidato anterior e preparado um artefato novo, preservando o histórico. APIs públicas TrustedPlatformModule Get-Tpm/Get-TpmEndorsementKeyInfo, PowerShell5.1 Desktop x64 já elevado, sem UAC automático. Guards de SHA256 próprio/helper/assembly on-disk, operador observado e ausência de impersonation. RAW ASN.1 e o certificado são limitados aos fingerprints previamente observados; algoritmo/parâmetros e bits da chave do certificado são impressos como dados separados, não como correspondência já demonstrada. OID do ASN.1 pode estar UNSPECIFIED; não presumimos SPKI, RSA ou ECC a partir do digest.

Uma execução manual do comando abaixo, em console já elevado. Não executado pelo agente; nenhum registro de aprovação foi criado. O processo deve conseguir executar o script segundo a política vigente; não há alteração automática de execution policy.

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\ek-collection-and-durable-worker-20261026\collect-ek-public-bytes.NOT-EXECUTED.ps1" -ExpectedScriptSha256 e64cd0d6b4660fcb4594aceeb4cde12aa7808263d93e914ab40cc4d335394279
```

Saída JSON deve conter first=NONE/exit=0 e public_bundle. É material público de identidade: preservar localmente o resultado completo, sem publicar base64 de certificado/chave desnecessariamente. Retornar o estado factual e disponibilizar o arquivo local para análise. Não repetir se houver erro, mismatch ou fechamento UNKNOWN. Não mudar pins automaticamente. O script não escreve arquivo de evidência, não coleta segredos e não solicita material privado/ownerAuth.

A leitura é pública: não NV_DefineSpace/Increment/UndefineSpace/Clear, alteração de hierarquia, Q4 ou admissão. Dispose controlado retornado será relatado; contextos internos do cmdlet continuam UNOBSERVED. Nenhum fingerprint é transformado em prova de posse/hardware ou geração do índice. Revisão detalhada em [COLLECTOR-REVIEW.json](COLLECTOR-REVIEW.json).

## Identidade EK

Bytes ainda não recebidos nesta tarefa: correspondência, algoritmo efetivo, cadeia/âncoras e posse nativa continuam pendentes. Não repetimos a consulta anterior nem inventamos resultados. Ao receber o material, normalizar o ASN.1 com formato explícito, comparar os parâmetros públicos com o certificado e validar finalidade/constraints/cadeia sob âncoras aplicáveis explicitamente confiáveis. AdditionalCertificates não é automaticamente fabricante ou trust anchor. Revogação offline, posse nativa e continuidade de geração permanecem separados. Nenhuma âncora instalada ou nova confiança concedida.

## Implementação do worker

[durable_worker_fixture.py](durable_worker_fixture.py) liga codec/protocolo anteriores, owner exclusivo, armazenamento real em arquivos sintéticos com flush/readback e testemunha simulada separada. Grava PREPARED antes do incremento simulado, CONSUMED antes do único slot de leitura fictícia e RESULT depois. Preserva primeira falha, grava UNKNOWN quando seguro e nunca reenvia incremento ambíguo. Reinício não adota registros anteriores. Dados/recibos de teste são preservados em runs.

O fixture usa uma root sintética única, cadeia de frames com predecessor e registros372 bytes; não usa root/CURRENT/issuer operacional, autoridade de aprovação, novo serviço ou journal de produto. O counter é explicitamente simulado. O API DATA canônico anterior e as decisões accepted permanecem inalterados. L1 ainda exige revisão exata de schema/participante/CAS nativo para integração produtiva. Nenhum código operacional mudou.

[Revisão completa de integração, custódia e limites](WORKER-INTEGRATION-REVIEW.md) documenta identidade controlada dos arquivos, lock entre processos, ordenação, crash gaps, UNKNOWN, perda de testemunha e limitações. Restaurar ambos os arquivos sintéticos pode passar: teste registra expressamente a ausência de independência física. Process restart não é reboot de Windows/TPM; fsync e os._exit não provam durabilidade a queda de energia. Close UNKNOWN é injetado e o processo de teste depois libera referências sintéticas; isso não prova retenção física do owner operacional.

## Adapter, builds e regressões

- Dois builds C# x64 .NET Framework, Debug/Release, /warn:4 /warnaserror+: PASS, zero warnings.
-67 verificações por build,134 total: serialização existente, cinco TPM_RC exatos, inclusive classes RETRY/RATE sem retry; resposta truncada, autenticação, primeira falha, uso único e close UNKNOWN. Endpoint apenas simulado, sem instanciar/enviar NativeTransport.
-14 testes do worker: PASS, incluindo9 fronteiras de crash/torn write,12 processos concorrentes com um vencedor, replay, restart, rollback do ledger, perda/corrupção de testemunha, identidade/autorização divergentes, Stop e UNKNOWN.
-Python bytecode compile PASS. Coletor AST PowerShell PASS, sem execução nativa.
-As728 verificações criptográficas e38 testes/160 combinações anteriores foram reutilizados sem repetição inalterada. Produto não mudou; não reconstruímos os quatro targets sem motivo.

Logs: [BUILD-RESULTS.json](BUILD-RESULTS.json), [WORKER-TESTS.log](WORKER-TESTS.log). Arquivos sintéticos e evidências não foram apagados. Tamanhos serializados/contagens de teste não são orçamento ou créditos ED11; memória/stack/kernel e witness físico continuam sem envelope factual novo.

## Custódia e provisionamento

Mantida a proposta concreta anterior: cerimônia autorizada de OWNER para definir, owner canônico/lease/sessão para incrementar, AUTHREAD para contador e leitura pública independente do public area, DPAPI CurrentUser/RAW1 no mesmo participante da root para custódia. Observação do SID não nomeia automaticamente um custódio. Mesmo usuário/admin, perfil perdido, substituição/Clear/redefinição e geração incerta continuam no modelo de ameaça. Nenhum segredo operacional foi criado ou extraído.

Provisionamento=NOT_READY. O perfil candidato0x01804143/0x00040014/8bytes/uma definição/até dois incrementos permanece sem reserva e sem comando de escrita. Faltam: bytes e base de confiança EK, posse/sessão nativa, custódia efetiva e binding nativo do participante persistente. Não produzimos pedido executável com esses campos inventados. O candidato anterior não foi promovido nem aprovado.

## Hashes finais

- collect-ek-public-bytes.NOT-EXECUTED.ps1: `e64cd0d6b4660fcb4594aceeb4cde12aa7808263d93e914ab40cc4d335394279`
- ek-public-normalize.ps1: `5623d7fbd248772c4b2d8c9fbb6bd492dbd78c96deafb45299421e41834b7ef4`
- durable_worker_fixture.py: `a254d9ae35519fbd3ceaf5a9cbbdb531f2f6d3ae14916726c7994d7b8efcb66e`
- test_durable_worker.py: `46af7888c6226ae814e39b4aab1ff90b3b14dbc44ce62363bdb26eac01278ed0`
- AdapterFaultTests.cs: `43355c4cf0406801e48c36d61fdefd7d3158c39c7d6c436b97c8414ce8b89410`
- AdapterDebug.exe: `62add1d690cf169e12977a614054ec6742e41fc2f8515214d9903dd4e286a886`
- AdapterRelease.exe: `9618ca41103c193c71dd3ef2e1612fb34b386fbc53b07d718fd4b53afdd666e3`

## Próximo gate

O próximo passo concreto é a única coleta pública manual acima. Depois dos bytes, a verificação de identidade pode prosseguir sem nova campanha de implementação. Prova de posse ou provisionamento reais exigem sua autorização específica, com perfil/bindings conhecidos. Q4 e disposição histórica permanecem independentes; sem limpeza, refund ou reaproveitamento de aprovações consumidas. E17/E18 e E19/E20 não são concluídos por este fixture; E22 continua derivado e pendente.

HEAD/WIP/históricos/inputs/pins preservados. Nenhum Start/Stop, SCM/ACL, quota, CURRENT, assinatura produtiva ou recuperação autoritativa. Estado: preparação/coletor=READY; worker=QUALIFIED_ISOLATED; identidade real=PENDING; E21=PENDING; ED11/Phase6=PARTIAL.
