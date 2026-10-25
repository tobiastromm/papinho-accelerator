---
status: proposed
revision: 2
date: 2026-10-21
scope: qualified issuer time observation and status binding in canonical UPDATE owner
---
# Q0-Q7 — origem, validade e rollback da observação UTC issuer

Proposta focal sob a seção3 da tarefa PACC-PHASE6-QUALIFIED-ISSUER-UTC-STATUS-BINDING-1. Não ACCEPTED. ADR-0006 exige wall clock confiável; perfil CA1 §1.4 e U4 exigem UTC qualificada, mas não definem a prova de qualidade/validade/rollback de uma observação. Esta proposta completa somente essa fronteira. Não cria relógio, serviço de tempo, trust store, provider geral, registry, worker, journal ou autoridade paralela; não concede qualificação operacional por texto.

## Q0 Fonte e escopo

Usar somente a porta de tempo do backend administrativo issuer existente, pertencente à composição da mesma root. UTC rege validade civil; papacc_pal_monotonic_time_ns existente rege duração/ordenação local, nunca fornece calendário ou prova de UTC. A presença de backend.utc, teste que retorne hora plausível, leitura OS ou função CNG não qualifica a fonte.

O primeiro adaptador Windows poderá observar a fonte UTC do sistema operacional já administrado na instalação, SOMENTE quando a origem/configuração/qualidade dessa fonte forem factualmente qualificadas para o target e processo. Não selecionar nem criar peer/serviço externo, não sincronizar/ajustar relógio, instalar serviço ou alterar permissões nesta integração. Não presumir que Windows Time está presente, autenticado ou saudável. Se o mecanismo implantado não expuser evidência suficiente, o adaptador permanece indisponível, sem fallback para relógio local não qualificado.

## Q1 Observação e unidades

O owner SHALL preservar explicitamente, em contexto privado transitório: identidade da instância/fonte/backend e de seu owner; processo/lifetime; root/invocação; revisão CURRENT e pin; identidade da candidata/cohort; revisão/aplicabilidade da qualificação; resultado da leitura e de qualidade; amostras monotônicas de início m0 e fim m1; UTC observada nativa; intervalo UTC conservador [L,U] válido em m1; limite de taxa rho_num/rho_den e erro aditivo E do PAL conforme Q2; e término monotônico exclusivo de aplicabilidade D. Campos numéricos não comprovam sua origem. Zero é um valor, não ausência implícita: presença e resultado são separados.

Domínio de cálculo interno proposto: unsigned64, UTC em nanossegundos desde1970-01-01T00:00:00Z e monotônico em nanossegundos do PAL local. Conversão Windows FILETIME: verificar F>=116444736000000000, subtrair essa origem em ticks100ns e multiplicar por100 com overflow verificado; falha/fora da representação nega, sem saturação. Escalar legado backend.utc continua segundos UTC; esta proposta não altera sua assinatura, wire, codec ou a validade accepted dos certificados. Não reduzir o intervalo a esse escalar para autorizar uma fronteira.

[L,U] SHALL conter o UTC real em m1 com limites de erro sustentados pela qualificação da fonte: incluir quantização, incerteza da fonte, atraso de aquisição e variação permitida até m1. m0<=m1<D; falha PAL, erro de leitura, overflow, intervalo invertido ou qualidade desconhecida nega. Leitura de API sem erro não é prova de exatidão. Nenhum preenchimento de L/U/D por valores caller/config ou callback booleano é permitido.

## Q2 Qualificação e origem da qualidade

A composição administrativa SHALL comprovar a procedência e a integridade da instância de backend/fonte, da revisão de sua configuração aprovada, dos resultados nativos de saúde/qualidade, de seu erro limitado e de sua vigência. Essa prova pertence ao procedimento de qualificação de implantação e ao owner existente, não a uma nova lista de confiança ou campo em PDG. Autorização do administrador U1-U3 permite o ato UPDATE, mas não prova que a hora está correta.

A qualificação SHALL demonstrar o erro UTC e o erro relativo do monotônico local, inclusive comportamento de ajuste/slew/step, perda de sincronismo, suspensão/resume, mudança de fonte/configuração e restart. Para Q3, rho=rho_num/rho_den, com0<=rho_num<rho_den e rho_den>0, limita o avanço do UTC real POR unidade de avanço PAL, e NÃO o erro PAL por unidade UTC. Para quaisquer dois cortes ordenados cobertos pela mesma qualificação, com incremento medido d>=0 do PAL e tempo UTC real decorrido delta>=0, demonstrar: max(0,(1-rho)*d-E)<=delta<=(1+rho)*d+E. E é um limite aditivo não negativo em nanossegundos, factual e qualificado, que inclui resolução, quantização, leitura e conversão dos endpoints PAL não cobertas pelo termo de taxa. Essa desigualdade também SHALL cobrir d=0 entre leituras distintas. Não deduzir E=0 da unidade ns da API nem usar limite de orientação inversa sem demonstrar sua conversão conservadora.

D, rho_num/rho_den e E são resultados restritos do perfil de fonte/target qualificado, não parâmetros livres de aplicação nem tolerâncias para contornar certificados. Exigir evidência de validade dos limites no intervalo inteiro até o efeito. Se continuidade dos limites não for comprovada em suspensão/retomada ou alteração de condições, invalidar a observação e negar propagação através desse evento. Sem evidência que sustente valores finitos e sua vigência, não há observação positiva.

Não se escolhe aqui mecanismo externo de aquisição/autenticação de hora, novo formato assinado ou protocolo de atestação. A prova operacional da fonte efetivamente implantada continua obrigatória. Documento de qualificação ou status textual isolado não cria um produtor factual: o adaptador deve verificar a instância e sua saúde/aplicabilidade atuais conforme o procedimento aprovado. Enquanto esse mecanismo não existir ou não estiver qualificado, QUALIFIED_UTC_SOURCE=DENY. Outros targets exigem adaptador equivalente e qualificação própria.

## Q3 Uso, validade e ausência de tolerância permissiva

Cada fronteira que autorize avanço SHALL obter nova observação diretamente do mesmo backend sob o owner, sem renovar timestamps na exportação ou reutilizar uma leitura antiga para outra operação. O intervalo preservado pode ser consultado como história; seu uso positivo fica restrito à fronteira/operação em que foi adquirido e ao mesmo contexto protegido. Antes do efeito, ler novamente o monotônico m: exigir m1<=m<D e fonte/qualificação/CURRENT/cohort ainda aplicáveis.

Propagar conservadoramente para m, usando d=m-m1 e os limites qualificados Q2: a=floor(d*(rho_den-rho_num)/rho_den); b=ceil(d*(rho_den+rho_num)/rho_den); dl=max(0,a-E); du=b+E; Lm=L+dl; Um=U+du. Computar dl=0 quando a<=E, sem subtração unsigned negativa; esse max é a definição do limite inferior físico de tempo decorrido, não tratamento permissivo de overflow. Cálculos unsigned e arredondamentos para fora, com toda soma/multiplicação/divisão e conversão verificadas; overflow/indeterminação nega, sem saturar ou suprimir erro desconhecido. Não assumir rho=0 ou E=0 sem prova; manter separadas a incerteza UTC [L,U] em m1 e a incerteza de duração PAL durante a propagação, sem deduzir componentes por sobreposição não demonstrada. Essa propagação local não cria outro relógio nem UTC de origem monotônica: apenas conserva o intervalo cuja origem Q1/Q2 já foi comprovada. Fora da vigência, negar e exigir nova leitura factual; não estender D por atividade, query ou tempo de usuário.

Para cada intervalo civil accepted [notBefore,notAfter), exigir notBefore*1000000000<=Lm e Um<notAfter*1000000000, com conversão verificada, tanto CA como certificado e delegação. Isso reduz o conjunto aceito diante de incerteza; não amplia validade, adiciona clock-skew/backdating, TTL de pacote ou tolerância de certificado. Fronteira sem capacidade de revalidar esses fatos imediatamente antes de seu efeito permanece DENY.

## Q4 Rollback e descontinuidade

O mesmo owner SHALL preservar a última observação nativa aplicável e seu contexto durante o lifetime corrente, sem tabela global ou persistência nova. Para uma fonte/instância inalterada, UTC nativa regressiva, monotônico regressivo, qualidade revogada/desconhecida, perda de origem ou inconsistência entre os intervalos UTC sucessivos e a propagação Q3 implica observação inválida e bloqueio do efeito. Igualdade de valores UTC por resolução limitada não é regressão, mas continua sujeita aos limites de erro/validade. Intervalos propagados e recém-observados que não se sobreponham são inconsistentes: não escolher o mais favorável.

Avanço abrupto também é verificado por essa consistência; mesmo um avanço coerente pode expirar certificado/delegação e negar. Nenhum relógio é ajustado, congelado no máximo anterior ou substituído silenciosamente por monotônico. Não há retry até conseguir um UTC favorável. Descontinuidade/falha de qualidade invalida a observação da operação e o avanço U4; requalificação explícita da fonte e nova operação governada serão necessárias, preservando qualquer efeito/retido anterior.

Restart não restaura observações, saldo temporal ou ato UPDATE; baseline em memória perdida não prova ausência de rollback do store. D3.7/G5/U6 e checkpoint/fencing continuam autoridades distintas: UTC jamais prova última revisão/high-water/uniquidade de issuer. AUTHORITATIVE_RECOVERY e efeitos operacionais pós-restart continuam DENY sem seus contratos/provas próprios.

## Q5 Status issuer e vínculo U4

Somente o adapter da root, não o codec, pode prover a observação de status. Usar registros/certificado/histórico issuer e policy factualmente protegidos pelo CURRENT/pin exato do predecessor e fontes necessárias. Conferir root/R/A/P/I, authority/generation, propósito, request/signer/certificado/SPKI, policy e tupla SNT/PDG/candidata. Epoch/proofs ausentes na porta issuer permanecem ausentes: não preencher os bits para inicializar o core ou satisfazer Q0.

Certificado PUBLISHED exato, perfil e intervalo comprovados são necessários para status positivo, não suficientes para delegação. REVOKED e estados de assinatura/reserva/pendência/abandono/resultado incerto são distintos; desconhecido/ausente/erro/derivação incompleta nega. Aprovação da delegação exige ato candidato U1-U3 ou herança factual permitida em G3, validade/revogação separadas e observação Q1-Q4 atual. X509/RSA válido, PDG APPROVED ou hash não são autoridade. Candidata de revogação pode ser preparada sem gerar evidência positiva de PACKAGE_SIGNING; não negar a administração segura da revogação por exigir que o alvo já seja confiável.

Antes de expor o resultado protegido e antes do efeito, revalidar ambas dimensões tempo/status e CURRENT/invocação/slot/pins/fontes/ato. Mudança de revisão ou perda de fonte invalida a observação; pin histórico retém memória, não oferece fallback. Mesmo com Q5 positivo, esta integração não concede TRUST/M1, lease E0, assinatura ou commit v3 positivo.

## Q6 Lifetime, Stop e retenção

Aquisição nativa de tempo/qualidade/status pertence ao owner administrativo existente antes de exposição. Não executá-la em sp_trust_acquire: E0 permanece ligação síncrona borrow-only de referências já protegidas. A observação, qualificação e fontes externas exigem cópia própria contabilizada ou proteção de seus owners durante uso; root pin não cobre ponteiro externo. Resultado de preparação anterior permanece consultável na mesma operação, mesmo sem lease SDK.

Stop impede novas observações/efeitos, sem bloquear esperando clock/provider, cancelar I/O presumidamente ou limpar dados ainda emprestados. Unknown/close não confirmado conserva owner, referências e créditos até quiescência/fechamento factuais. Falha de UTC nega autoridade mas não implica que handles/filhos foram destruídos; cleanup segue Model E. Não usar timeout, exit, erro de tempo ou reset de estado para liberar um resultado incerto. RELEASE_INVALID mantém sua semântica sem tentativa/efeito.

## Q7 Implementação e ED11

Nenhuma implementação ou reserva é concedida por esta proposta. Reutilizar backend administrativo issuer, PAL, root/cohort/pins e status existentes. Não substituir algoritmos/formatos, mexer no relógio do host, buscar rede, alterar ACL ou criar serviço. Se a fonte de implantação não puder atender Q2, manter fonte indisponível e preparar somente a qualificação/alternativa governada necessária, sem um provider permissivo.

Antes de hunks: fixar tipos C89 privados, origens/propriedade dos campos e primitivas de aquisição; medir estruturas de qualidade/observação, amostras anterior/nova, snapshots, pins, DER/workspaces, estado status, caller/stack/handles/provider e coexistência/retidos. Não duplicar bytes comprovadamente compartilhados nem omitir memória por colocá-la em stack/heap/caller. Windows3145728 somente opt-in, Legacy2097152 e quotas individuais intactos. Fixo anterior1948880/1790872 não é pico/crédito; cenário completo3747825 excede3MiB em602097 e não autoriza ativação. Provider/kernel/stack transitivo UNKNOWN e ED11 totalPARTIAL.

Regressões futuras: intervalo UTC válido sintético; origem/qualidade ausente; D expirado; overflow/conversão; rollback wall/monotônico; mudança de fonte/suspensão; inconsistência/avanço; fronteiras notBefore/notAfter; status desconhecido/revogado/issuer divergente; CURRENT/revisão/pin/candidata divergentes; Stop/retidos. Separar testes de cálculo e fixtures da qualificação nativa da fonte. Não produzir autoridade operacional por testes sintéticos.

NORMATIVE_STATUS=PROPOSED
IMPLEMENTATION_AUTHORIZED=NO_BY_THIS_PROPOSAL
QUALIFIED_UTC_SOURCE=DENY_UNTIL_SOURCE_PROFILE_AND_NATIVE_QUALIFICATION
ISSUER_STATUS_BINDING=DENY_UNTIL_FACTUAL_SOURCES
V3_COMMIT=DENY
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
