---
status: proposed
revision: 2
decision-date: 2026-10-24
scope-level: project
scope-target: exact candidate identity binding stage for N0START-20261024-03, not activated
supersedes: []
---
# Candidate — anexo L1 exato, etapa de identidade da sucessora

## X0 — autoridade, origem e alcance desta etapa

O owner selecionou `N0START-20261024-03` e autorizou gerar UM candidato aleatório criptograficamente seguro de128bits/exatamente32hex minúsculos, sem derivação das tentativas anteriores. A única geração utilizou secrets.token_hex(16), alimentado pelo CSPRNG do OS; resultado registrado em [recibo documental](../integration/independent-lab-successor-exact-binding-implementation-20261024/owner-authorized-identity-candidate.json). Nenhuma geração alternativa/retry foi feita. A comparação textual de fontes/docs relevantes não encontrou esse valor anterior; isso NÃO prova ausência factual dos nomes no destino Windows.

**Identidade candidata: `0b1de869e11493b7cb08fb329578f6ce`. Operação administrativa: `N0START-20261024-03`.** A identidade está INATIVA. Esta revisão2 substitui apenas o rascunho incompleto anterior deste ANEXO, preservado byte-exatamente no pacote de revisão; não supersede decisões accepted nem cria política geral de tentativas. Requer aprovação normativa específica desta etapa/hash antes de integração funcional. Não cria intent, imagem, sink, dossiê ou registro de aprovação operacional.

## X1 — alvo exato herdado e exclusividade

Serviço: `PapinhoAcceleratorLab20261023`; conta `NT SERVICE\PapinhoAcceleratorLab20261023`; SID `S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451`. Operador esperado `S-1-5-21-785699550-1440458426-1682014700-1108`, token elevado sem impersonation e continuidade factual obrigatória. Host SHA256 `2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff`; Windows10.0.19045AMD64/NTFS; boot236441/StartTime2026-10-23T18:36:34.5000000Z. Esses são requisitos normativos, não observações atuais.

Mesmo `PS_ADMISSION`, mutex `Global\PapinhoAccelerator.ED11.SingleLabAdmission`, suas verificações/owner/DACL/thread/lifetime e exclusividade canônica. Nenhum owner, serviço, registry, logger, namespace de lock ou reserva paralela. Segunda admissão simultânea, abandono, replay ou lifetime controlado incerto continuam DENY. Quatro papéis: PowerShell único já elevado; qualificador; serviço quando existir; até um auxiliar N2. Sem PID futuro fabricado.

Configuração116bytes SHA256 `a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44`, native417614100:393216:1120240. Pai de instalação `C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release`, native417614100:1966080:79426. Proteção e identidades devem ser revalidadas; o anexo NÃO qualifica nova criação nem remedia ACL nesse pai.

## X2 — nomes candidatos exatos, sem colocação ou reserva

| Objeto | Nome exato candidato |
|---|---|
| Intent/tombstone | `papacc-psr-0b1de869e11493b7cb08fb329578f6ce.intent` |
| Imagem instrumentada | `papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe` |
| Sink/arm | `papacc-psr-0b1de869e11493b7cb08fb329578f6ce.diag` |
| Recibo launch existente, quando requerido pelo owner | `papacc-psr-0b1de869e11493b7cb08fb329578f6ce.launch.log` |
| Evidência protegida | `papacc-n2-0b1de869e11493b7cb08fb329578f6ce.evidence` |
| Registro/dossiê protegido | `papacc-n2-0b1de869e11493b7cb08fb329578f6ce.approved` |

Intent/imagem/sink/recibo launch ficam no pai de instalação acima. Inputs protegidos ficam em `C:\ProgramData\PapinhoAccelerator\Protected\N0`, sob revalidação da cadeia original e autorização separada de colocação. Arm é o frame/estado inicial de1024bytes do MESMO sink, não outro arquivo/logger; final máximo2048bytes, max2refs, um append. Nenhum nome foi consultado/criado/reservado no destino nesta tarefa. Ausência inequívoca canônica ANTES do consumo, FILE_CREATE/CREATE_NEW finais e retenção em colisão posterior permanecem obrigatórios. Sem adoção/overwrite/delete/retry/rollback.

## X3 — integração coordenada prevista, ainda não aplicada

No MESMO owner, vincular tentativa/intent/launch/registro consumido à candidata, rejeitando explicitamente antiga/replay sem aceitar string arbitrária. Coordenar descriptor B+D, IG_ATTEMPT/plano/confirmação, imagem/sink no ciclo, psrd_attempt/psrd_leaf/texto/arm do recorder e contexto protegido/pins. O recorder mantém primeira API/NTSTATUS original e protocolo existente; não segundo sink, novo nonce de ServiceMain ou logger.

O launcher continua `tools/qualification/scm-n0-protected-single-powershell.ps1`, PowerShell5.1x64 único já elevado, mantendo `[Console]::Out.WriteLine($line)` no encaminhamento e os Write-Host administrativos. Novo binding da operação deverá usar confirmação exata vinculada a `N0START-20261024-03`/`0b1de869e11493b7cb08fb329578f6ce`, com desafio nativo contemporâneo existente. Desafio/PID/token futuros não são preenchidos agora. Nenhum launcher foi alterado ou executado.

Dossiê novo deve vincular identidade/operação, host/boot, operador/token, plano/confirmação, config/binário/política, RAW1, evidência de arquivo Windows e auxiliar. Evidência estática antiga só por referência explícita após revisão de aplicabilidade/revalidação; não copiar/relabelar aprovação/claim anterior. Não se presume que novo nome corresponde ao conteúdo antigo5358/326bytes: conteúdo/tamanho/hashes resultantes serão qualificados na etapa factual antes de qualquer colocação. Sem criação protegida nesta tarefa.

## X4 — histórico e política de recursos preservados

P3 continua registrado para N0START-20261024-02/75330d11cc7e4123ae62eadc9a59c2f5. Intent consumido, imagem/sink/arm/ImagePath históricos preservados; starts0/stops0/close0/quiescent0/first1816 não mudam. Não reutilizar aprovação/objetos/tombstone antigos; nenhum rollback ou quiescência inferida.

H0–H9 autoriza normativamente avaliar a incerteza opaca delimitada sem teto retrospectivo universal, não recurso zero/refund/reserva reutilizável/budget aprovado. UNKNOWN e reservas reais indisponíveis preservados. Orçamento independente REQUIRED_NOT_GRANTED. Todos os recursos controlados e observáveis precisam de prova completa e compatibilidade; lifetime controlado incerto ou obrigação incompatível causa DENY.

Caps128handles por nativo/1024admin compartilhados/quatro papéis, demais limites N6/B+D, freshness60s/1s, headroom5/9GiB/pisos4/8, HOST_CREDITS=NONE_GRANTED, Stop/retenção/exactly-once/anti-replay permanecem. N7 operacional específico para `0b1de869e11493b7cb08fb329578f6ce`/evidências atuais NÃO concedido. Sem associação autenticada de console/imagem mapeada, whitelist ou autoridade auxiliar. Patch de console em fixture não promete folga real.

## X5 — duas etapas e gates restantes

A aprovação solicitada é SOMENTE da etapa normativa de identidade/nomes/vínculos previstos deste anexo. Se aceita e registrada pelo processo autorizado, retomar o slice de implementação já autorizado pelo owner; nenhuma operação real decorre disso. Hashes reais dependentes de compilação não podem ser aprovados em branco.

Etapa factual posterior deve congelar/revisar hashes de fontes/binários/qualificador/launcher, conteúdo do dossiê/políticas/plano e eventuais novos inputs; quatro builds Selected/Legacy Debug/Release zero warnings e negativos de identidade antiga/intent/replay/arquivo ou sink históricos/dossiê/token/boot/quinto processo/caps/freshness/Stop/UNKNOWN/close incerto. Não promover testes sintéticos a prova nativa.

Colocação protegida, qualificação factual read-only, N7 operacional, orçamento independente, efeitos SCM/rebind/Start e aprovação manual final permanecem gates separados. Sem comando operacional, preflight, Start/Stop, SCM/ACL, rede/listener/CA/certificados/bootstrap/CURRENT ou produção0.6.4. Sem terceiro retry derivado dessa identidade.

ANNEX_STATUS=PROPOSED_IDENTITY_STAGE_ONLY
CANDIDATE_ATTEMPT=0b1de869e11493b7cb08fb329578f6ce
ADMINISTRATIVE_OPERATION=N0START-20261024-03
CANDIDATE_ACTIVATED=NO
FUNCTIONAL_INTEGRATION=NOT_APPLIED
N7_OPERATIONAL_ACCEPTANCE=NOT_GRANTED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
SUCCESSOR_ADMISSION=DENY
FILES_CREATED_OPERATIONAL=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY

## Autoridades

- [L0–L9 ACCEPTED](DECISION-first-independent-lab-successor-scope-20261024.md), especialmente L1/L9 e etapas de hashes.
- [H0–H9 ACCEPTED](DECISION-l5-p5-target-specific-historical-resource-coverage-20261024.md).
- [P0–P9](DECISION-prestart-consumed-attempt-administrative-disposition-20261024.md) e [P3 registrado](../integration/prestart-administrative-disposition-acceptance-20261024/P3-EXACT-DISPOSITION-RECORDED.md).
- [Matriz candidata não executável](../integration/independent-lab-successor-exact-binding-implementation-20261024/binding-matrix-CANDIDATE-NONEXECUTABLE.json).
