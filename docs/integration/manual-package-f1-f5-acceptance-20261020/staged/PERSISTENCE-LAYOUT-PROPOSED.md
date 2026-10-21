---
status: proposed
revision: 1
scope-level: project
scope-target: private same-root package delegation representation
---
# G0–G7 — representação byte-exata proposta para F5

## G0 Escopo e precedência proposta

Esta é a extensão normativa específica que falta à condição5 da tarefa. NÃO ACCEPTED. Não modifica F1–F5 original, M0–M8, D3 de trust, S1–S5, T0–T6, D3 de envelope ou codecs/fontes funcionais. Somente se posteriormente aceita definirá PACROOT8/version3, componente público PDG1 e checkpoint CP3 na MESMA root/arquivo/participante policy/worker/CAS. Não é um store, registry, lista de confiança, journal ou autoridade independente. Nenhuma dependência do Browser em formato da root é criada.

O primeiro perfil é EXISTING_ROOT_UPDATE somente. Predecessor permitido: PACROOT8/version2 ou version3 com checkpoint completo e CURRENT/pin factuais. Version1, ROOT06, bootstrap inicial, recuperação, seleção automática de nova CA e migração automática são UNSUPPORTED neste perfil. Todos os formatos anteriores permanecem byte-exatos. Nenhuma implementação ou reserva ED11 decorre desta proposta.

Apenas delegação explícita F1 é acrescentada à persistência. Evidência emitida pelo Accelerator é observação derivada de root/policy/issuer/TLS/CURRENT factuais e pertence à operação; não se persiste uma segunda autoridade de status ou um recibo reutilizável. A confirmação independente F3 e a autorização local de instalação F4 NÃO entram no arquivo da root, no pacote, em PDG1 ou em novo formato de atestação. A necessidade futura de persistência adicional requer decisão específica; não há campo livre que a autorize.

## G1 Composição, limites e discriminação

Inteiros novos e do envelope externo são unsigned LITTLE-ENDIAN; bytes incorporados preservam os codecs históricos, sem conversão: BODY0.5/ROOT06 BIG-ENDIAN e LST1 LITTLE-ENDIAN. Sem native structs/padding, ponteiros, handles, SID, senha ou material privado. Todos os intervalos têm fim exclusivo e toda soma/multiplicação exige verificação de overflow antes de acesso.

N=quantidade de listeners candidatos1..4; B=quantidade de listeners no predecessor1..4; K=quantidade de listeners Secure candidatos1..N. D=linhas de delegação0..K, no máximo uma por listener Secure no primeiro subconjunto. Isso é limite proposto de representação/suporte, não redução de limite accepted nem aumento de quota. Se precisar de vários signers simultâneos para o mesmo listener, UNSUPPORTED, nunca truncar ou escolher o primeiro. Um signer pode ser delegado explicitamente a vários listeners em linhas distintas.

L=160+512*B; J=16384; O=512+1536*N; S=96+768*K; U=O+S; V=96+384*D; Q=U+V+L+J; T=Q+544. Exatamente20608..30592 bytes. Mínimo com uma delegação:20992. Prefixo anterior ao checkpoint tem Q bytes; imagem sem digest final tem T-32 bytes, máximo30560.

| Região | Offset | Bytes |
|---|---:|---:|
| Header externo |0|512|
| Coleção candidato descriptor/BODY |512|1536*N|
| SNT1 accepted |O|S|
| PDG1 proposta |U|V|
| LST1 do predecessor |U+V|L|
| Carrier issuer ROOT06 accepted |U+V+L|16384|
| CP3 ou ZERO |Q|512|
| Digest W3 |Q+512|32|

Regiões sempre contíguas nessa ordem, sem gaps, sobreposição, alignment ou trailing bytes. D=0 mantém componente header+digest96 e expressa ausência de delegação, não trust aprovado. A coleção não é convertida em listener singular; o target singular ainda nega N>1 antes de efeitos. Carrier é obrigatório e nunca omitido/resetado para caber.

## G2 Header completo e componentes históricos

A matriz offset-matrix-proposed.csv enumera integralmente header512, PDG header64, linha384 e CP3512. Todos os campos do header seguem T0–T6 Revision2 accepted, com estas substituições EXCLUSIVAMENTE sob version3:

- magic0:8 ASCII PACROOT8;8:u16=3;10:u16=512;12:u32=T;16:u32=3 (UPDATE+ISSUER);20:u16=N.
- 22:u16=512;24:u16=1024;26:u16=1 SHA256;28:u16=512;30:u16=1 LST1.
- 32:u32=512;36:u32=1536*N;40:u32=U+V;44:u32=L;48:u32=U+V+L;52:u32=16384;56:u32=Q;60:u32=512;64:u32=Q+512;68:u32=32.
- 72:32=root domain não zero;104:u64=R;112:u64=predecessor;120:u64=I;128:u32=31;132:u16=5 BODY;134:u16=1 descriptor.
- 136:u64=R_base=predecessor;144:u64=A_base;152:u64=P_base;160:u64=I_base;168:u32=15;172:4 ZERO;176:32=C_base;208:32=W_base.
- 240:u16=prior_kind,2 para predecessor v2 ou3 para predecessor v3, sem fallback.242:u16=1 SNT;244:u32=O;248:u32=S;252:u16=K;254:u16=768;256:u16=1 SHA256.
- 258:u16=1 PDG;260:u32=U;264:u32=V;268:u16=D;270:u16=384;272:u16=1 SHA256;274:238 ZERO.

R/predecessor/base A/P/I são não zero; R=predecessor+1 sem wrap; I é não zero, igual à revisão do carrier e >=I_base. Domain e C_base/W_base referem o MESMO predecessor completo factual. Base epochs não são epochs novos. Campos restantes não existem: toda a área foi enumerada. Bytes274..512 e172..176 são sempre ZERO; usar extensão aqui sob v2 continua proibido.

Descriptor PACCD001/BODY0.5 e SNT1 mantêm seus bytes e predicados T1–T4, inclusive K linhas de nomes, nomes/roles/perfis, estágios e coerência de security_ref compartilhado. LST1 continua apenas fragmento histórico da coleção base. ROOT06 carrier mantém sua identidade/geração/CA-SPKI/serial/high-water/reservas/revogação/event-prefix e arquivo-âncora históricos; não vira policy operacional. A relação externa atualizada com PDG1 é nova somente em v3. Não passar versão3 a um leitor v2 ou reconstruir versão2 retirando PDG1. Readers históricos rejeitam versão desconhecida; um writer que não suporte v3 nega e preserva o arquivo.

## G3 PDG1 — header64, linhas384 e digest32

Header:0:8 ASCII PACPDG01;8:u16=1;10:u16=64;12:u32=V;16:u16=D;18:u16=384;20:u32=0 flags;24:32=domain idêntico ao externo;56:u64=R idêntico ao externo. Seguem D linhas nos offsets64+384*j, j0..D-1; digest SHA256 ocupa[V-32,V). Não há padding entre linhas.

Linha:0:8 ASCII PACPDL01;8:u16=1;10:u16=384;12:u32=1 presença;16:u16=status1 APPROVED ou2 REVOKED;18:u16=1 PACKAGE_SIGNING;20:u16=1 perfil técnico de certificado;22:u16=ordinal do listener, zero-based;24:u64=disposition_approval_root_R;32:32=policy identity;64:16=issuer authority;80:16=issuer generation;96:16=signer record/request ID;112:32=SHA256 DER certificado signer completo;144:32=SHA256 DER SPKI signer completo;176:u64=delegation_not_before UTC segundos;184:u64=delegation_not_after UTC segundos;192:u16=listener_id_length;194:2 ZERO;196:64=listener_id seguido de ZERO;260:32=CA DER SPKI SHA256;292:92 ZERO.

Presença1 significa bit0: tupla completa obrigatória; bits1..31 são ZERO e não concedem autorização. Somente esses status, finalidade, versão, presença e perfil são reconhecidos. Os códigos são discriminação privada desta proposta, não capability/wire/Principal/grant. Validade exige0<not_before<not_after e intervalo contido na validade do signer e da CA, confirmado pelos certificados e UTC confiável; nenhuma duração/TTL/backdating nova é selecionada. REVOKED retém o intervalo e identidade; não significa certificado revogado. APPROVED fora do intervalo não é trust. Status de certificado é consultado separadamente no issuer. Valores desconhecidos são erro, não aprovação por default.

Toda identidade/digest e disposition_approval_root_R é não zero; essa revisão <=R. Ordinal<N e deve apontar a Secure; linhas estritamente ordenadas por ordinal, sem duplicatas. Length1..64 e bytes de listener exatamente iguais ao descriptor/SNT1 correspondentes, incluindo case e ZERO tail. Policy identity igual à policy administrativa efetivamente selecionada para esse listener/BODY/descriptor, não um alias nem preenchimento inventado. Essa identidade não é declaração de grants. O policy ID histórico da emissão issuer NÃO é automaticamente igual à policy de delegação atual; a relação autorizada precisa ser validada contra suas fontes/histórico, nunca inferida de uma igualdade de bytes.

Authority/generation/CA-SPKI coincidem com SNT1 do listener e carrier issuer aplicável. Signer ID é registro existente de purpose PACKAGE; certificado exato/hash/SPKI e perfil dedicado são verificados nesse registro/histórico e com rotina qualificada. Signer é distinto de CA/servidor/cliente. Antes de TRUST, comparar o SPKI DER completo derivado do certificado autorizado com o SPKI completo do pacote e verificar ambos os fingerprints SHA256; o campo do pacote não autentica sua própria chave. Nenhum DER ou chave é duplicado em PDG1. Ausência de material/fonte, issuer inválido ou referência divergente nega consumo. Decodificação pode verificar apenas forma/correlação; nunca retornar trust/RESOLVED/CURRENT.

Disposition revision referencia a transação explicitamente autorizada para a tupla completa root/policy/listener/security_ref/configuração TLS/nome/issuer/geração/CA-SPKI/signer/certificado/SPKI/finalidade/validade/status. Mudança dessa tupla exige nova disposição aprovada; revisão<=R ou digest não provam que ocorreu. Mover ordinal mantendo identidade/tupla íntegros é reposicionamento, não nova delegação. Reintrodução após revogação exige ato explícito novo; não se reaproveita antiga aprovação.

Remover linha ou reduzir D impede novos efeitos para a identidade removida, mas não apaga histórico. Mudanças exigem predecessor integral, histórico de disposições e retenção existentes. Substituir signer/rotacionar issuer exige transação explícita, preservando o predecessor e trabalho antigo retido; nunca reclassificar serial/certificado antigo como novo. D0 ou linha ausente é DENY, sem fallback para ancestral ainda APPROVED. Não criar log lateral para caber no componente; sem capacidade/histórico comprovado, negar mudança.

## G4 Integridade, CP3 e transação

SHA256 é apenas integridade/correlação por adapter já aprovado, nunca prova de aprovação/freshness/fencing. Ausência/falha do hash nega. Domínios exatos:

1. Digest PDG1 em[V-32,V): bytes do componente[0,V-32).
2. Digest SNT1 permanece SHA256 do componente[0,S-32), armazenado em[S-32,S).
3. C3=SHA256(outer[0,Q)), inclui header, coleção, SNT1 completo, PDG1 completo, LST1 e carrier.
4. Hcp3=SHA256(CP3[0,480)), armazenado CP3[480,512).
5. W3=SHA256(outer[0,Q+512)), armazenado outer[Q+512,Q+544).

C_base é o digest de conteúdo do predecessor definido pela versão efetivamente selecionada e confirmado por seu checkpoint; W_base é seu digest W armazenado, não o SHA256 de transferência do arquivo completo. São sempre do mesmo predecessor integral. Não há digest dentro do próprio domínio. CP é512 ZERO para ausência; caso presente:0:8 ASCII PACR8CP3;8:u16=3;10:u16=512;12:u32=1 issuer;16:32=domain;48:u64=R;56:u64=predecessor;64:u64=A publicado factual;72:u64=P factual;80:u64=I;88:32=C3;120:32=C_base;152:32=W_base;184:296 ZERO;480:32=Hcp3. A/P não zero e demais campos exatos do header/história. CP3 somente com v3, CP2 com v2, CP1 com v1, nunca PACICP01 reinterpretado.

PREPARE fixa candidato, fontes, autorização, orçamento, pin e predecessor completo imutável. VALIDATE exige contexto factual e resolve todas as referências; MODEL_BOUND/codec OK não é RESOLVED. COMMIT usa o MESMO worker/CAS contra bytes completos e identidade nativa e grava v3 com CP ZERO. CAS_CONFLICT/UNKNOWN nega avanço, mantém ownership/retidos e nunca escolhe versão antiga como fallback ou retry destrutivo.

Somente a autoridade canônica pode publicar CURRENT após commit durável, depois A/P factuais; I permanece o conteúdo real issuer, sem incremento sintético para delegação. Alteração de delegação não presume projeção C2 idêntica: P é preservado apenas com a prova accepted da projeção operacional. Não alterar grants/snapshots de Sessions retroativamente.

Após publicação confirmada, CP3 é persistido no MESMO worker por checkpoint-only CAS contra imagem exata CP ZERO; prefixo[0,Q) permanece byte-exato. Só CP e W3 mudam. Sem publicação factual não escrever A/P nem CP. Fonte/certificado/policy/confirmation aplicáveis são revalidados antes de TRUST e RELEASE; CP válido não substitui essas fontes. Pin retém memória/proveniência, não nova autoridade.

## G5 Compatibilidade e recuperação conservadora

Primeira v2→v3 é adoção explícita com CURRENT, predecessor integral e CP2 correspondentes; v3→v3 exige CP3 correspondente. C_base/W_base/R/A/P/I selecionam o predecessor completo, não apenas LST1 ou PDG. Aprovações/header/componentes e event-prefix/high-water/revogações issuer não podem regredir. Hash válido não prova que não há outro writer/instalação mais recente. Nenhuma migração de v3 para v2 que descarte delegação é autorizada.

Formato/magic/versão desconhecidos, flags/presença/enum inválidos, contagem/length fora do limite, campos ZERO inválidos, referência ambígua, digest errado, overlap/gap/truncamento/trailing bytes rejeitam imagem completa. Sem output utilizável parcial. Erro não autoriza reparo por rehash, truncamento, escolha de ancestor ou exclusão do issuer.

Crash antes/depois de commit sem CURRENT: nenhuma nova autoridade. CURRENT sem CP3 durável: restart nega recuperação. CP3 completo e hashes válidos: apenas história coerente, sem garantia global. Rollback relativo de R/I/high-water/status/event-prefix, perda de CP ou histórico, alteração no mesmo R e predecessor incorreto negam. Status/delegação removidos não voltam por restauração de ancestral. AUTHORITATIVE_RECOVERY=DENY até backup/checkpoint/fencing independentes aprovados; não recriar CA ou serial.

## G6 Evidência transitória e três autoridades

O Accelerator apenas produz observação pública F2 sob pin/lease e fontes reais: tupla resolvida da delegação/certificados, CURRENT/R/A/P/I, status/UTC e escopo servidor. O owner existente associa essa observação ao pacote completo imutável/digest de transferência, SPKI esperado e à operação nomeada para apresentação pela cerimônia D3. Não há recibo de status autossuficiente, assinatura de atestação ou novo transporte. Nenhum status obtido exclusivamente da root é considerado confirmação do Browser.

O Browser recebe a confirmação pelo canal independente accepted, comprova o reconhecimento do administrador/papel e associação ao seu pacote/operação. Depois da validação autenticada, exige a autorização local BrowserProfile para instalar. Essas duas decisões não são flags no registro de delegação. Senha, texto/callback ou chave fornecida no próprio pacote não suprem origem independente. Sem canal/contexto de origem factual, manter DENY antes do KDF. A root do Accelerator não concede direito local de instalação.

Confirmação e observação são transitórias pertencentes aos owners existentes; dados internos do pacote permanecem em quarentena até validar tudo. Revalidação do status vigente antes de release/instalação continua obrigatória. Auditoria/idempotência pública já exigida permanece no lifecycle existente; não é aprovação reutilizável. Persistência adicional de recibos exigirá contrato próprio. Este texto não modifica Browser, não define ABI de evidência nem congela detalhes do frontend.

## G7 Lifetime, ED11 e implementação bloqueada

Owner canônico mantém committed/predecessor/candidato, snapshots/pins/leases, certificados/fontes externas, buffers de input/encode/decode/hash/CAS, contextos e observação até finalização factual. Pin de root não protege objeto externo sem cópia própria ou lease do seu owner. Stop não espera I/O/KDF nem prova cancelamento/destruição; falha/incerteza retém buffers/créditos e não dispara retry destrutivo. Nunca sobrescrever slot pinned/retido. Model E/ED08/exactly-once e política pendente de Sessions ativas permanecem.

V=96..1632 serializados; com uma linha V480. O aumento de máximo de imagem é1632,28960→30592. Hipóteses de buffers completos independentes:3E91776 (+4896),4E122368 (+6528),6E183552 (+9792) em relação aos máximos v2. Esses subtotais não são deltas de composição real nem reservas. PDG bytes já estão em E; não somar de novo. Copiar PDG separado adiciona só cada cópia real comprovada. É preciso medir histórico/snapshots/retidos, evidence/refs, DER/crypto/provider, KDF/AEAD/RSA, input/quarentena/staging/output, callbacks/stack e coexistência, sem mover memória para escapar do censo.

Runtime adicional desta proposta=0. Sem censo compilado de v3, orçamento de ativação/reserva não aprovado. O cenário conservador anterior3594733 excedia Windows3MiB antes desta extensão e incluía fixture/provider; não é pico operacional nem margem para novas alocações. Default2097152 e Windows3145728 explícito por processo inalterados; quotas individuais inalteradas. Provider/kernel UNKNOWN e ED11 total PARTIAL. Não presumir bytes livres, compartilhamento ou viabilidade. Sem reserva qualificada, DENY.

F1–F5 e G0–G7 dependem de revisão/aceitação exata antes de código. Sem aceitação, manter todos os readers/formatos atuais intactos. Depois ainda serão necessários patches exatos, prova de fontes/pins/lifetime, regressões separadas e qualificação operacional. Nada autoriza CURRENT, assinatura, recuperação, bootstrap, chaves, certificados, pacotes, admissão, rede ou release.
