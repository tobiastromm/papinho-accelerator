---
status: proposed
revision: 1
scope-level: project
scope-target: same-root manual package delegation and operation-local confirmation
---
# Extensão focal das fontes D3.3/D3.4

Proposta preparada sob a decisão do owner nesta tarefa. Não ACCEPTED; não implementa campos, APIs, codec, autoridade ou fonte factual. M0–M8 e D3.1–D3.11 accepted permanecem integralmente vigentes. PACKAGE_SIGNING abaixo é finalidade semântica, não novo enum de wire/capability ou grant operacional.

## F1 Delegação explícita na policy da única root

A aprovação de um assinante para PACKAGE_SIGNING SHALL ser dado administrativo público da policy na única root, publicado pela transação existente. SHALL associar inequivocamente a identidade da policy ao registro dedicado do assinante, à autoridade e geração issuer aplicáveis, ao certificado exato e ao fingerprint SHA256 do DER SPKI completo. Identidades de root, policy, issuer, geração e signer reutilizam os domínios existentes; digest de certificado e SPKI são correlação, não nova autoridade. Não reutilizar bytes de Principal, conditions, policy[32], PACKAGE, codeSigning, campos reservados ou nomes para representar aprovação implícita.

A aprovação SHALL conter finalidade explícita PACKAGE_SIGNING, intervalo autorizado e disposição administrativa inequívoca. Ausência, ambiguidade, revogação ou status desconhecido nega. Validade e revogação da delegação SHALL ser avaliadas separadamente das do certificado; emissão/PUBLISHED não é delegação. Revogar delegação não altera nem apaga histórico issuer, não reutiliza serial e não altera grants/Sessions. Mudanças usam PREPARE → VALIDATE → COMMIT → CURRENT → EPOCH com base factual, pin e CAS; não existe edição lateral/allowlist paralela. R/A/P/I são obtidos do contexto da root, sem contador ou epoch próprio. A preservação de P depende da comparação integral da projeção operacional accepted, não apenas do rótulo administrativo da mudança.

Um único signer exato pode ter aprovação aplicável na policy para o escopo root/servidor determinado. Aprovações conflitantes não são resolvidas por ordem textual ou fallback. Não se seleciona nesta proposta cardinalidade ou quota nova. Aprovação não autoriza por si geração de chave, uso de chave privada, exportação, admissão ou emissão.

## F2 Evidência adquirida, distinta do registro

O adapter SHALL adquirir somente pelas funções e pelo owner administrativos canônicos: snapshot imutável aplicável da policy, registros/histórico issuer e certificado correspondente, vínculo de ExpectedServerName/listener/TLS já accepted e CURRENT factual. Decodificação de carrier ou consulta que devolva apenas dados sem autoridade não satisfaz aquisição. A prova inclui revisão aplicável, identidade e pin/lease factual cobrindo TODOS os objetos; pin da root não protege automaticamente buffers externos.

Certificado e chain SHALL ser verificados por rotinas qualificadas segundo perfil1: signer dedicado, RSA2048/e65537, sha256WithRSAEncryption, CN aprovado, BC crítico CA=FALSE, KU crítico digitalSignature, EKU não crítico codeSigning somente, AKI/SKI, validade e issuer/generation/certificado exatos. UTC confiável e status da delegação e do certificado são obrigatórios. Registro issuer ou cadeia válida isolados não concedem trust. Recuperação/fencing não qualificados não podem ser preenchidos por hash válido ou CURRENT inventado.

A observação contemporânea pertence à operação exata; SHALL ser revalidada antes de TRUST e antes de RELEASE. Pin antigo preserva memória, não autoridade atual. Se a revisão mudar, não usar a revisão histórica como fallback; negar esta operação e exigir nova validação explícita. Revogação, expiração, perda de lifetime, Stop ou incerteza impedem liberação mesmo após KDF/AEAD. Não introduzir TTL, tolerância de relógio, consulta de rede ou cache autorizado por esta proposta.

## F3 Confirmação independente pertencente à operação

A confirmação de primeiro contato SHALL ser evidência pública transitória sob o owner da operação existente, não um catálogo de confiança. SHALL associar pacote completo imutável/digest SHA256 de transferência, SPKI esperado completo, escopo root/ExpectedServerName, identidade do signer e observação F2 à operação de importação e ao destino que recebe a confirmação. Sua origem inclui o administrador independentemente reconhecido, papel autorizado e cerimônia D3.2 efetivamente realizada. A cerimônia mínima é presencial; outro canal exige procedimento administrativo já aprovado para esse uso. Ausência dessa origem/prova nega antes do KDF.

O servidor pode fornecer os fatos de policy/issuer para confirmação; não pode declarar que o destinatário os reconheceu ou autorizou instalação. Token administrativo elevado comprova somente sua própria autorização/contexto quando aplicável; não comprova canal independente, reconhecimento pelo destinatário ou aprovação local. Um boolean de CLI, senha, texto digitado, assinatura ou chave do pacote não constitui evidência suficiente.

A confirmação autoriza continuar validação de uma única operação/pacote, nunca confiança permanente na chave, outro pacote ou reimportação automática. Material ou destino alterados, cancelamento ou interrupção invalidam sua aplicabilidade; não serializar evidência para permitir bypass posterior. A correlação de operação usa a identidade existente do owner; não cria registro global, contador ou nonce criptográfico novo. Auditoria/proveniência pública eventualmente retida converge ao lifecycle existente, sem senha, chave ou plaintext.

## F4 Três decisões e lifetime

Autorização administrativa do servidor para assinar, confiança independente do destinatário no assinante e autorização local BrowserProfile para instalar SHALL ser decisões distintas. Esta proposta não implementa nem modifica BrowserProfile, importador, CredentialGeneration/Reference/store/gate/leases. A autorização local de instalação continua posterior à validação completa e nunca decorre de F1 ou F3.

Owner existente SHALL adquirir evidências antes da exposição, manter input/metadata/snapshots/pins/contexto imutáveis até finalização factual e liberar dependências exatamente uma vez após quiescência. Falha de aquisição não publica evidência parcial. Resultado incerto ou falha de liberação mantém retenção, buffers e créditos; nenhum retry destrutivo. Stop solicita encerramento, não prova cancelamento nativo nem destruição. Sem fonte quiescente ou revisão aplicável, DENY/RETAINED, nunca liberar conteúdo.

M1 SHALL permanecer STRUCTURE → SIGNATURE → TRUST → KDF → AEAD → CONTENT → RELEASE; revalidação final adicional é obrigatória sob D3.6. Sem F1–F3 factuais, zero KDF e nenhum resultado confidencial. Confiança válida não substitui validação completa de ZIP/TLV, certificados/chave, perfil/identidade/issuer/serial/validade e autorização de release.

## F5 Limite de implementação, persistência e ED11

O modelo atual não representa F1 e não produz F3. Aceitação desta proposta semântica futura NÃO aprovará offsets, tamanho, cardinalidade, magic, enumeração, campos ou APIs por inferência. Persistência da delegação exige contrato versionado byte-exato no participante policy da mesma root, com limites, presença, hash, histórico, predecessor/CAS e coexistência explícitos antes de qualquer escrita. ROOT06/BODY0.5/ROOT08/v2/D3/LST1/SNT1/CP2 e o formato .papacc permanecem intactos. Não embutir certificado signer como novo membro .papacc nem criar signed-attestation format.

A confirmação F3 permanece pertencente à operação; nenhuma persistência nova é aprovada. Handoff Browser separado será necessário para aquisição independente e autorização local, sem mudanças Browser nesta tarefa. Mecanismo de captura, origem e invalidade da confirmação deve ser revisado com as fontes de autoridade existentes antes de qualquer API funcional. Caller-supplied facts não são automaticamente fontes confiáveis.

Contabilizar uma vez com prova de backing/lifetime: delegação, histórico, contexto público da confirmação, policy/issuer/TLS snapshots, pins, certificados/DER/hash, pacote imutável, password/key, quarentena/staging/output, KDF/AEAD/RSA/provider, callbacks/stack e todos os retidos. Não reutilizar armazenamento pinned/retido. Nenhuma reserva ou quota é aprovada. Windows3MiB apenas explícito; default2MiB e quotas individuais permanecem. Custos não medidos e provider/kernel UNKNOWN mantêm ED11 total PARTIAL e ativação DENY.
