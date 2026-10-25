# Q0-Q7 corrigido — revisão final R1/R2

Nenhuma lacuna material remanescente identificada no contrato corrigido. R1/R2 encerrados no escopo normativo, sem prova de fonte/target ou implementação.

| Obrigação | Evidência corrigida | Resultado |
|---|---|---|
| Significado/direção/sinal de rho | Q2: avanço UTC real por unidade PAL,0<=num<den,den>0 | Inequívoco; limite inverso não pode ser inserido diretamente |
| Limites inferior/superior | Q2: max(0,(1-rho)d-E)<=delta<=(1+rho)d+E | Inclui taxa e erro aditivo, d=0 e tempo decorrido não negativo |
| Propagação/arredondamento | Q3 usa floor inferior e ceil superior, dl=max0 e du=b+E | Conservador sob a desigualdade qualificada; não arredonda para dentro |
| Resolução/leitura/conversão PAL | Q2 inclui esses endpoints em E; Q1 preserva rho/E no contexto | Nenhuma suposição de resolução1ns ou E=0; incerteza inicial UTC continua separada |
| Unidades/limites | Q1 UTC/PAL ns unsigned64, domínio/FILETIME checked, E ns | Sem troca implícita entre UTC seconds legado e intervalo ns |
| Underflow/overflow/saturação | Q3 dl0 se a<=E; demais cálculos/conversões checked | Max0 é definição matemática; overflow/indeterminação nega, não satura |
| UNKNOWN/qualificação/vigência | Q1-Q3 exigem fonte/qualidade/rho/E/D factuais e finitos, m<D | Sem números caller/flags; desconhecido/expirado implica DENY |
| Suspensão/troca/restart/rollback | Q2 exige limites válidos até efeito; Q4 preservado | Sem continuidade factual não propaga; restart não restaura ato/CURRENT; relógio não é fencing |
| Status/UPDATE | Q5 preservado sob issuer/F/G/U0-U7 | Status de certificado e delegação separados, fonte CURRENT/pin real; UTC não aprova candidata |
| Ownership/Stop/retidos | Q6 e E0/Model E/ED08 preservados | Não adquire em E0, não libera por timeout nem transforma erro UTC em destruição |
| Portabilidade | Q0/Q7 preservados; Q1 dados/unidades e conversão Windows específica | Nenhum Win32/CNG/SID obrigatório no núcleo; targets não qualificados por texto |
| ED11 | Q7 preservado; rho/E/proveniência ampliam grafo futuro contabilizável | Delta runtime0 nesta decisão; sem aumento de quota/reserva/economia ou ativação |
| Incorporação | Três hunks Q1-Q3 reconstruídos e comparados byte-exatos ao patch e Revision2 | Original preservado; Q0/Q4/Q5/Q6/Q7 exatos; nenhum hunk funcional aceito |

## Justificativa algébrica documental

Do limite Q2, floor((1-rho)d)<= (1-rho)d e ceil((1+rho)d)>= (1+rho)d. Subtrair/somar E e tomar max0 inferior conserva um intervalo que contém delta. Somar os endpoints ao intervalo inicial [L,U] conserva inclusão. Essa derivação é leitura/revisão normativa, não execução de testes ou prova da fonte física. O guard civil exige o intervalo inteiro dentro da validade, sem tolerância adicional.

D e o erro/taxa não precisam receber constante global inventada para aceitar o contrato: são limites que o perfil/fonte factual qualificado deve demonstrar; sem parâmetros/proveniência válidos a implementação deverá negar. A ausência atual de fonte qualificada não é evidência positiva nem bloqueia a aceitação normativa já determinada.

## Escopo e implementação posterior

A futura implementação deve preservar o mesmo owner/backend/PAL e dimensões em censo, sem novo serviço ou fonte permissiva. Definir tipos privados e verificações de origem sem transformar POD/boolean/callback em autoridade. Qualificar fonte real e contexto CURRENT/pins/status antes de qualquer avanço; fontes em testes sintéticos apenas exercitam cálculo. Todos os gates operacionais permanecem negados. Sem builds/testes/rede nesta tarefa.
