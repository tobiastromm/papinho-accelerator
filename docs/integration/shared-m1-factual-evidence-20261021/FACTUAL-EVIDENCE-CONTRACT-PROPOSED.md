# API factual de evidências — contrato exato E0–E8, Revision 1

STATUS=PROPOSED. IMPLEMENTATION_OF_THIS_API=DENY_PENDING_NORMATIVE_ACCEPTANCE_AND_FACTUAL_OWNER_BINDING. Revisa TRUST-EVIDENCE-PROPOSED.md sem alterá-lo ou aceitar nova autoridade por referência. S0–S8/M0–M8/D3/F1–F5/G0–G7 permanecem accepted. Nenhum header funcional desta API foi criado.

## E0 Alcance e resultados distintos

A API proposta correlaciona e revalida evidências adquiridas pelos owners existentes. Não é prova criptográfica nova, wire, persistência, provider geral, trust store ou registry. DATA, assinatura matemática, evidência administrativa, confirmação independente, autorização local e instalação são resultados distintos. O controle M1 compartilhado é somente agendamento e não pode produzir uma evidência. SOURCE_ABSENT permanece DENY antes KDF.

## E1 Tipos e assinaturas propostos

Tipos são opacos, pertencentes à operação existente, nunca construíveis/deserializáveis pelo caller. Nomes privados propostos, sem versão pública/ABI congelada. Nenhum alocador ou owner novo é implícito:

```c
typedef struct SP_TRUST_PACKAGE SP_TRUST_PACKAGE;
typedef struct SP_ADMIN_OBSERVATION SP_ADMIN_OBSERVATION;
typedef struct SP_INDEPENDENT_CONFIRMATION SP_INDEPENDENT_CONFIRMATION;
typedef struct SP_TRUST_LEASE SP_TRUST_LEASE;
typedef enum SP_TRUST_PHASE {
 SP_TRUST_BEFORE_KDF=1, SP_TRUST_BEFORE_RELEASE=2
} SP_TRUST_PHASE;
typedef enum SP_TRUST_RESULT {
 SP_TRUST_APPLICABLE=1, SP_TRUST_DENIED=-1,
 SP_TRUST_RETAINED=-2, SP_TRUST_INVALID=-3
} SP_TRUST_RESULT;
SP_TRUST_RESULT sp_trust_acquire(
 const SP_TRUST_PACKAGE *,const SP_ADMIN_OBSERVATION *,
 const SP_INDEPENDENT_CONFIRMATION *,const SP_TRUST_LEASE **);
SP_TRUST_RESULT sp_trust_revalidate(const SP_TRUST_LEASE *,SP_TRUST_PHASE);
typedef enum SP_TRUST_RELEASE_RESULT {
 SP_TRUST_RELEASED=1, SP_TRUST_RELEASE_RETAINED=-1,
 SP_TRUST_RELEASE_INVALID=-2
} SP_TRUST_RELEASE_RESULT;
SP_TRUST_RELEASE_RESULT sp_trust_release(const SP_TRUST_LEASE *);
SP_TRUST_RELEASE_RESULT sp_trust_release_query(const SP_TRUST_LEASE *);
```

Estas declarações são texto PROPOSED, não símbolos implementados. acquire/revalidate são ligados somente ao adapter factual revisado, fixado pelo build; não recebem tabela de callbacks nem boolean de confiança. O produtor é responsável por provar a origem por aquisição existente, e não por um campo source_id arbitrário. Tipos completos/fábricas ficam privados ao owner concreto e precisam patch próprio. Sem funções factuais identificadas/revisadas, não implementar um shim que aceite dados do caller. Não criar mapa global de handles ou mecanismo paralelo de pins.

## E2 Pacote e operação

SP_TRUST_PACKAGE é borrow do mesmo objeto imutável capturado pelo owner existente. Liga identidade da operação/destino, bytes completos/limites e SHA256 de transferência, SPKI esperado completo/fingerprint e escopo ExpectedServerName/root aplicável. SHA256, SPKI e strings são correlação, não autoridade. Entrada de path não é reaberta para recuperar o pacote. Material trocado/destino/operação divergente invalida o contexto; não reaproveitar para outro pacote/reimportação. Dados cifrados não estabelecem fatos anteriores ao KDF. Identidades mantêm domínios accepted; sem novo contador/epoch.

## E3 Observação administrativa e proveniência

SP_ADMIN_OBSERVATION somente nasce de aquisição factual pela root/policy/issuer canônicos, ou da observação efetivamente atestada pelo administrador conforme D3 no consumidor. Distinguir explicitamente verificação local de certificado daquilo que foi somente atestado; não afirmar validação local de certificado não recebido. Browser não precisa ler root/PDG1 ou adquirir pin remoto por um protocolo inventado.

A origem inclui owner, instalação/escopo e operação, revisão administrativa factual e contexto de observação; associa CURRENT e R/A/P/I sem fabricar valores. A fonte Accelerator mantém snapshot imutável, pin/lease válido de todos os backings e autoridade aplicável; pin antigo preserva memória, não status. Digest, carrier decodificado ou referência policy[32] não satisfaz aquisição. Nenhum contexto pode ser obtido pela API de dados, pelo pacote ou senha. Recuperação/fencing não qualificados permanecem DENY.

## E4 Signer, propósito e status

Observação SHALL comprovar signer/certificado/SPKI exatos, identidade/geração do issuer e CA aplicáveis, delegação explícita da policy para PACKAGE_SIGNING e escopo servidor. Validar perfil dedicado accepted por rotinas qualificadas: RSA2048/e65537, SHA256WithRSAEncryption, BC crítico CA=FALSE, KU crítico digitalSignature, EKU não crítico codeSigning-only, identidades/AKI/SKI e associação ao issuer. Chain, codeSigning ou PACKAGE isolados não substituem delegação.

Validade do certificado e da delegação são verificações separadas sob UTC confiável; revogação/status de ambos precisam ser conhecidos e aplicáveis. Ausência, revogação, expiração, propósito diferente, issuer divergente, revisão obsoleta ou CURRENT/pin não comprovados nega. Não inserir TTL, tolerância, freshness global, cache autorizado, retry destrutivo ou fallback histórico.

## E5 Confirmação independente e autorização local

SP_INDEPENDENT_CONFIRMATION pertence ao owner do destinatário, após cerimônia D3.2–D3.6/F3 efetivamente realizada com administrador independentemente reconhecido e autorizado. Cerimônia mínima presencial; canal alternativo somente sob procedimento já aprovado que prove independência, identidade, papel e integridade. Liga observação E3/E4, fingerprint/SPKI esperado, pacote exato, servidor e operação/destino. Senha, chave embutida, checkbox, token elevado ou assinatura RSA não constituem essa confirmação.

Accelerator produz fatos administrativos, não a confirmação do destinatário. Root não concede autorização local de instalação. Esta é decisão posterior do BrowserProfile existente, fora desta API/M1, seguida do writer/transação/leases existentes. Sem canal ou confirmação factual, DENY antes KDF. Não criar PapinhoTrustStore nem persistir confirmação transitória como autoridade reutilizável.

## E6 Aquisição, lifetime e revalidação

Output de acquire permanece intacto em erro; não publicar empréstimo parcial. Sucesso exige E2–E5 integrais com referências pertencentes ao mesmo lifetime/owner. O lease é armazenamento público transitório do owner existente, não token bearer copiável ou capacidade de outra operação. Core recebe borrow somente após aquisição factual; dados candidatos permanecem DATA.

Revalidar antes TRUST/KDF e antes RELEASE, incluindo validade, revisão/approval/status aplicáveis e confirmação independente contemporânea. Se não se puder provar aplicabilidade da observação anterior, exigir nova observação administrativa conforme D3; não inventar consulta remota ou evidência offline automática. Alteração de revisão, revogação, Stop, perda de pin/custódia, mudança de pacote ou incerteza nega liberação. Um lease de memória não prova CURRENT contínuo/global.

## E7 Stop, liberação e retenção

Stop solicita encerramento, não cancela KDF/I/O nem encerra borrow por inferência. Owner mantém pacote, senha/key, quarentena, metadados, snapshots/pins, contexto provider, evidências e créditos até fim factual de todos filhos/borrows. Sem limpeza/reuso de retidos ou segredo emprestado.

release autoriza uma única tentativa governada pelo owner; SP_TRUST_RELEASED significa somente referências da operação efetivamente liberadas; é tipo distinto do resultado de trust, nunca trust ou instalação. Depois de RELEASED, invalidar o alias exatamente uma vez e não consultar novamente. Resultado SP_TRUST_RELEASE_RETAINED mantém o mesmo handle e dependências somente para release_query; query nunca reemite close, revalida para continuar M1 ou publica sucesso tardio. Repetição de release/destrutivo negada. Distinguir resultado de operação, destruição factual e finalização do owner; nenhum alias dos dispositions PST é criado. Esta semântica proposta exige registro exato antes de implementação.

## E8 Gates de aceitação e implantação

APPLICABLE não pode ser fornecido por caller, modelo, getter DATA ou tabela selecionável pelo usuário. Código de teste separado nunca é fonte produtiva. Não criar produtor sintético para fazer a API parecer completa. API de evidências continua dependente de decisão normativa/revisão dos owners e aquisição factual indisponível; implementação positiva negada.

Definições dos tipos opacos/captura, funções reais de aquisição root/policy/issuer/TLS e confirmação do consumidor, seus lifetimes, revalidação e release precisam ser demonstrados antes dos patches dependentes. M1 extraído pode ser qualificado negativamente com port nativa fixa SOURCE_ABSENT. Não gerar pacotes/chaves/certificados, rede, CURRENT ou assinatura. Contabilizar backing, estado, referências, pins, stack, coexistência e retenção; sem reserva ou quota aprovada. Provider/kernel UNKNOWN, ED11 PARTIAL.
