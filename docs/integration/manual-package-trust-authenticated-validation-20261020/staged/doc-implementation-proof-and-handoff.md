# Prova de implementação e handoff

Estado: implementação offline parcial; nenhuma qualificação operacional positiva.

## Fronteira implementada

O núcleo portável novo executa STRUCTURE → SIGNATURE → TRUST → KDF → AEAD → CONTENT → RECHECK_TRUST → RELEASE. A revalidação final não substitui TRUST anterior ao KDF. O adaptador nativo verifica matematicamente RSA pelo domínio M2, mas retorna SOURCE_ABSENT em TRUST: não existe neste slice ligação factual da policy/issuer e da confirmação administrativa independente exigidas por D3. Nenhuma chave embarcada, senha, enum PACKAGE ou assinatura válida supre essa fonte. O caminho CONTENT também preserva DENY sem a fonte administrativa completa dos materiais/perfis/identidades. Não se declara validação autenticada positiva do produto.

O registro issuer policy[32] identifica policy; não é prova de delegação. O adapter C8 de policy de Session não foi reinterpretado como delegação de pacote. O próximo patch precisa ligar leitura factual da policy e issuer, propósito, validade, revogação, revisão/CURRENT, escopo exato do pacote e atestação independente com owner/lifetime existentes. D3 está accepted e não precisa ser reaberto. Se forem necessários campos persistentes novos, seu layout exige decisão específica antes de gravar. Nenhum trust store, registry ou owner paralelo foi criado.

## Ownership por função

mp_package_validate_authenticated recebe operação e input pertencentes ao caller. Estado inicial unused, ranges disjuntos e orçamento conhecido são obrigatórios. Reentrada e reutilização da operação são rejeitadas. Input, password, contexto e buffers devem permanecer imutáveis/protegidos pelo owner até resultado factual. A API não adquire pin CURRENT fictício; a ausência do adapter factual mantém DENY. A port de produção é fixa, sem callback de trust fornecido pelo chamador.

O executor comum mantém chave, envelope e metadados de payload dentro da operação (256 bytes x64), evitando referências para metadados locais após retorno RETAINED. Quarentena, staging pós-tag e output são disjuntos. Inspeção ZIP/TLV e verificações finais precedem a única cópia ao output. Falha mantém output intocado e tamanho zero. Stop é observado nas fronteiras; o primeiro resultado factual de parser/Stop é preservado, sem reinterpretá-lo por uma segunda consulta.

mp_package_validation_finish consulta quiescência, nunca repete assinatura, KDF, AEAD, conteúdo ou liberação. Resultado incerto ou owner não quiescente retém input, referências, buffers, chave e crédito. Somente quiescência factual permite limpeza; finish descarta resultado retido e não publica sucesso tardio. Stop não cancela implicitamente I/O, não fabrica destruição e não concede restituição de crédito. Neste adapter as chamadas criptográficas são síncronas; retenção assíncrona é coberta apenas por modelo explícito. Não há importação em filesystem ou arquivos parcialmente instalados.

mp_crypto_verify_signature fornece somente verificação matemática; o wrapper anterior de identidade SPKI aprovada permanece. Limpeza sensível usa OPENSSL_cleanse. Parâmetros e formatos accepted permanecem intactos. O validador estrutural anterior continua negado. Nenhum desses componentes foi conectado ao runtime produtivo.

## Evidência

Selected e Legacy, Debug e Release: /W4 /WX, zero warnings, 2/2 suítes por configuração. Cada configuração passou 139 asserções SYNTHETIC_CONTROL_ONLY e 1163 asserções REAL OFFLINE CRYPTO. Modelo cobre confiança ausente/divergente, propósito, status desconhecido/revogado/expirado, pin/revisão/identidade, ordem, zero KDF antes de TRUST, Stop, resultados incertos, retenção, corrupção, limites e aliasing. Sucesso do modelo retorna SYNTHETIC_ONLY, nunca autoridade produtiva.

Testes criptográficos usam backend existente e fixtures públicas TEST ONLY, incluindo comparação KDF ao BCrypt e vetor AEAD. Nova integração nativa comprova assinatura matemática válida seguida de DENY em TRUST e zero KDF/AEAD/output; assinatura inválida é rejeitada antes. Não foram criados chaves/certificados/pacotes operacionais. Não há prova positiva nativa de delegação, CURRENT, perfil completo ou importação Browser. O binário nativo não contém a fábrica do modelo; modelo não importa libcrypto/BCrypt/CNG.

Uma execução intermediária encontrou objetos de teste obsoletos após mudança do header; dois testes pendentes foram interrompidos. Foram declaradas dependências explícitas dos headers no CMake de testes, sem trocar backend ou enfraquecer verificações. As quatro execuções finais passaram. Logs intermediários foram preservados separadamente; não são resultados finais.

## ED11

Censo detalhado: ed11-census.json. Runtime novo ligado: 0 bytes. Operação256 + input120 + port24 + três buffers49654 + input51042 + senha128 = 200532 bytes sem provider/stack. Chave32 e views64/80 já estão na operação, sem dupla contagem. Capacidades reais fornecidas são cobradas; backing excedente e outros owners retidos devem constar em other_reserved. Estimativa numérica não é crédito de admissão.

Base conhecida Selected2103847; Legacy2067655. Cenário conservador não compartilhado Selected + componentes + pico CRT de teste1290354 =3594733, excesso449005 sobre3145728, antes de custos desconhecidos. O pico inclui contexto global exclusivo da fixture: não é pico operacional medido nem fundamento para apagar custos. Sem prova de compartilhamento e reserva qualificada, ativação permanece DENY. Provider/kernel, stack transitiva e metadados do futuro owner de trust permanecem desconhecidos/não reservados. Limites e quotas não mudaram; Windows3MiB exige seleção explícita, default2MiB permanece.

## Próximo gate implementável

Ligar o adapter factual read-only à policy/issuer e à atestação independente, com correlação ao pacote imutável, status/revisão/CURRENT e lifetime protegidos; completar validação de materiais/perfis e reservar composição ED11 medida. Executar qualificação positiva somente com fontes autorizadas disponíveis. Até então manter DENY. Browser/importação/CLI são handoff posterior, sem mudanças nesta tarefa.
