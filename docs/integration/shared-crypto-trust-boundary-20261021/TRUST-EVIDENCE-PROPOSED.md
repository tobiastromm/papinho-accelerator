# Proposta focal — interfaces de evidência e backend

STATUS=PROPOSED; IMPLEMENTATION_AUTHORIZED_BY_THIS_DOCUMENT=NO. Não cria provider, owner, trust registry ou formato serializado. S3/F5 exigem revisão exata da captura/proveniência antes de API funcional. Documento delimita a menor extração posterior; não altera decisões accepted ou congela uma ABI pública.

## Backend criptográfico

Contrato privado de adapter selecionado explicitamente pelo build, nunca por bytes do pacote: operação RSA pública recebe plano imutável de SP_SIGNED_PARTS e SPKI completo, devolve apenas resultado matemático; KDF recebe senha32 já normalizada/salt16/600000 e output32 disjunto; AEAD recebe key32/nonce12/AAD exato M2/cipher/tag16 e quarentena exclusiva; material recebe views privadas após tag e valida correspondência/perfil aplicável. Retornos distintos INVALID,UNSUPPORTED/UNAVAILABLE,STOPPED,UNCERTAIN e CRYPTO_ONLY devem preservar erros factuais e primeiro erro. Erasure usa mecanismo revisado do backend/owner, somente em memória quiescente. Tabela crypto não pode responder TRUST, CURRENT ou INSTALL. Nada de Win32/CNG/NSS/EVP/PST no header comum. Não há fallback de provider ou ajuste de parâmetros. Não copiar primitivas para o core.

A implementação de tabela/funções exatas deve reutilizar adapters atuais e ser revisada antes de migrar. Nenhuma função crypto ganha responsabilidade de adquirir fontes administrativas. Testes diferenciais futuros devem comparar mesmos bytes públicos, erros e efeitos de saída/Stop respeitando precondições locais; não criar fixture previamente negada.

## Evidência de confiança: três aquisições distintas

1. Accelerator: seu owner administrativo factual adquire policy/PDG, issuer/certificado/perfil/status e binding TLS/listener sob CURRENT aplicável, revisão e pins/leases que cubram todos os backings. Seleção DATA ou hash não é aquisição. Expor leitura emprestada somente após sucesso integral; fonte ausente/obsoleta/revogada/incerta nega sem publicar evidência parcial.
2. Consumidor: seu owner de importação existente captura confirmação administrativa D3.2–D3.6/F3 por canal independente efetivamente reconhecido. Evidência correlaciona pacote completo imutável, SPKI esperado completo/fingerprint, ExpectedServerName, escopo e identidade do signer, observação/revisão da origem e a operação/destino exatos. Accelerator não pode fabricar essa confirmação. Senha, chave inclusa e checkbox não são proveniência. Captura concreta é responsabilidade de integração Browser posterior, não implementada aqui.
3. Consumidor: autorização local de instalação pelo BrowserProfile, distinta das anteriores e posterior à validação completa. Não pertence à root Accelerator nem a uma tabela crypto/trust do core.

## Interface funcional a revisar com os owners

Usar empréstimo opaco originado exclusivamente pela função factual do owner existente; não oferecer construtor público a partir de campos/bools, setter trusted, deserialize-as-trust ou callback genérico selected pelo caller. O empréstimo identifica origem, operação, pacote, revisão e objeto protegido, com método de revalidação e finalização específicos daquele owner. O core valida correspondências e disposição, mas não cria CURRENT, pin ou confirmação. Campos públicos de correlação e predicados não tornam uma implementação fonte confiável. Sem produtor factual demonstrado, o binding compilado de produção continua SOURCE_ABSENT/DENY.

Antes de escrever o header exato, mapear funções reais de aquisição/revalidação/finalização dos dois owners e prova do reconhecimento administrativo. Essas funções ainda não foram identificadas como implementadas para o conjunto completo. O header atual mp_auth_native_port é privado/fixo e nega; NÃO exportá-lo como callback booleano trust. Não inventar um tipo de receipt que o chamador possa fabricar para substituir essa ausência. Não existe nesta proposta token criptográfico, evidência assinada wire, cache, TTL ou armazenamento novo.

## Máquina M1 a extrair, sem reescrita

A única máquina candidata é a existente em manual_package_validation.c. Separar seu agendamento/preflight/quarentena de MP_PDG_INPUT e PC_PDG1_SELECTION privados. Continuar STRUCTURE -> SIGNATURE -> TRUST -> KDF -> AEAD -> CONTENT -> RECHECK_TRUST -> RELEASE. Recheck é adicional e não substitui trust anterior. Somente binding factual autorizado pode avançar TRUST; missing-source sempre nega antes KDF. Hooks sintéticos permanecem binário separado com resultado SYNTHETIC_ONLY, sem fábrica positiva no produto. Não implementar outro executor paralelo ou promovê-lo por teste.

Owner mantém operação, senha, pacote, metadados, evidências, provider, snapshots, pins, q/staging/output e créditos até RESULT factual. Mudança de bytes/identidade/revisão/status, revogação, perda de lease, Stop ou incerteza impede RELEASE. RETAINED preserva todos os objetos; finish consulta quiescência, nunca repete etapa/close nem publica sucesso tardio. Não reutilizar buffer retido/pinned, não apagar segredo borrowed, não inferir DESTROYED por timeout/saída/FINAL. Não transportar dispositions PST para backend crypto sem contrato factual próprio.

## Gate concreto restante

Implementação da API de evidências e extração M1 pendem de desenho exato ligado aos owners, proveniência da captura independente e revisão oficial correspondente. Factual CURRENT permanece PENDING_EXTERNAL_ENVIRONMENT; confirmação positiva não é suprida pela sandbox. Nenhum normativo accepted é reaberto para esconder esse gate. A preparação DATA e regressões negativas independentes estão concluídas. Browser deve consumir snapshot fonte novo somente sob autorização própria, nunca editar vendor pinado ou depender do checkout Accelerator no build.
