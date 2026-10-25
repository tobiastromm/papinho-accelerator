# Q0-Q7 — revisão normativa e STOP de aceitação

TASK=PACC-PHASE6-QUALIFIED-UTC-STATUS-Q0-Q7-ACCEPTANCE-1. A aceitação condicional exige definição inequívoca e propagação conservadora. Duas lacunas materiais em Q2/Q3 impedem ACCEPTED. Não há conflito entre decisões accepted; a proposta não demonstra completamente o requisito de intervalo conservador de Q1/U4/CA1. Nenhum código ou teste foi executado.

## R1 — direção do erro relativo

Q2 exige “limite factual e qualificado de taxa relativa rho_num/rho_den”, antecedido por “erro relativo do monotônico local”. Q3 usa exatamente:

> Lm=L+floor(d*(rho_den-rho_num)/rho_den); Um=U+ceil(d*(rho_den+rho_num)/rho_den).

Não fica definido se rho limita avanço UTC por incremento PAL ou erro do PAL relativo ao UTC. Essas direções não são intercambiáveis. Exemplo algébrico ilustrativo, NÃO teste de implementação: se o perfil interpretar rho=1/2 como erro relativo de PAL, um incremento PAL100 para UTC real200 satisfaz esse limite, mas o termo superior de Q3 seria150. Logo não se pode aprovar a fórmula sem fixar a orientação e a desigualdade que a qualificação deve provar.

Correção Q2 proposta define explicitamente avanço do UTC real por unidade PAL, seus domínios e a desigualdade conservadora. Não exige um fornecedor de tempo ou muda a origem accepted por inferência.

## R2 — resolução/leitura dos endpoints PAL

pal_time.h declara que ns é unidade, não promessa de resolução1ns. Q3 usa somente termo proporcional d=m-m1. Se duas leituras distintas retornarem o mesmo valor, d=0 e Um=U, embora tempo real possa ter decorrido. Q1 inclui a incerteza da observação em m1, mas não define limite aditivo para a leitura final futura. Não se pode inferir que o intervalo anterior contém esse erro novo ou que rho o absorve para d=0.

Correção proposta acrescenta E factual/qualificado para o erro aditivo da duração PAL e preserva rho para taxa. Para qualquer par de cortes cobertos: max(0,(1-rho)*d-E)<=deltaUTC<=(1+rho)*d+E. Q3 arredonda para fora e usa dl=max(0,floor(d*(1-rho))-E), du=ceil(d*(1+rho))+E; Lm=L+dl, Um=U+du. Q1 preserva rho/E no contexto existente. E não é clock-skew de certificado, TTL ou valor default permissivo. Se não puder ser qualificado, fonte permanece DENY.

O max0 inferior decorre de tempo decorrido não negativo; não encobre overflow. Limites são exigidos para todo o intervalo até o efeito, inclusive suspensão/retomada, ou a observação é invalidada. Não se cria relógio virtual, persistência ou autoridade nova.

## Matriz completa

| Requisito | Fonte/cláusula | Conformidade |
|---|---|---|
| Origem/identidade/autenticidade | Q0/Q1/Q2; U4/F2 | Obrigação definida: backend e instância/qualificação/procedência verificáveis, não scalar/flag/documento isolado; fonte operacional não qualificada |
| Instante/lifetime/idade | Q1/Q3; U0-U7/E0 | Domínios m0/m1/D explícitos, D exclusivo e não prorrogado; idade máxima é D-m1 dentro do perfil factual qualificado, não TTL global ou caller livre |
| Precisão/incerteza | Q1-Q3, PAL e CA1 §1.4 | STOP R1/R2; direção de rho e erro aditivo precisam correção antes de aceitar a propagação |
| Rollback/descontinuidade/troca de fonte | Q2/Q3/Q4 | Fonte/contexto precisam continuar aplicáveis; erro/regressão/inconsistência invalida sem retry favorável; segurança da propagação depende de R1/R2 |
| Suspensão/retomada | Q2 e limites Q3 | Qualificação exige comportamento demonstrado; correção explicita que continuidade não provada invalida observação, não prolonga D |
| Restart | Q4 sob U6/D3.7/G5 | Preservado: não restaura ato/CURRENT/autoridade, nem usa hora como fencing |
| Unknown/Stop/retenção | Q6, E0/Model E/ED08 | Preservado: DENY não é destruição, não libera por timeout, owner/borrows/credits retidos |
| Issuer/validade/revogação/status/revisão | Q5; F1/F2/G3/U4 | Definido como fonte factual CURRENT/pin e status de certificado/delegação separados; DATA e matemática X509 não aprovam |
| Vínculo ao UPDATE e efeito | Q1/Q3/Q5; U2/U3/U6 | Definido, com revalidação antes do efeito; UTC isolado não aprova nem concede TRUST |
| Windows/box/ISA | Q0/Q2/Q7 | Contrato de observação é dados/unidades portáveis; conversão FILETIME pertence ao adaptador Windows. Nenhum target qualificado por texto |
| Fontes locais/externas/PST | Q0/Q2/Q7 | Local OS é somente candidato; sincronização/serviço externo não presumidos ou criados. Nenhuma nova dependência CNG/SID/Win32 no núcleo/PST |
| ED11 | Q7 e decisões ED11 | Nenhuma quota/reserva/economia aprovada; custos transitivos/retidos precisam censo; E futuro também deve ser contado |
| Registro e governança | Seção8 da tarefa | Aceitação suspensa; proposta e ADRs accepted intactos; nenhuma fonte funcional/build/teste/rede |

## Correção focal e próximo gate

[Patch Q1-Q3 PROPOSED](exact-q1-q3-corrections-PROPOSED.patch) e [Revision2 separada](Q0-Q7-REVISION2-PROPOSED.md), com [intervalos/preimagens/hashes](exact-corrections-and-preimages.json). Q0/Q4/Q5/Q6/Q7 ficam byte-exatos. Nenhum patch foi aplicado à proposta original. A Revision2 também permanece PROPOSED e não é aceita por referência nesta revisão.

Depois da decisão humana e revisão da correção exata, futura aceitação poderá incorporar as cláusulas corrigidas delimitadas e manter implementação/qualificação/ativação independentes. Fonte UTC nativa, CURRENT, pin/status e ED11 de ativação permanecem gates factuais, não motivos para aceitar matemática ambígua nem fatos inventados. R1/R2 são os únicos bloqueios materiais identificados nesta revisão.
