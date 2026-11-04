# Reconciliação V3 — forma atual do token do runtime

Consulta manual concluída: first0, consumedTrue, completeTrue, observer_close_knownTrue, wrapper0. A autorização está consumida; não repetir. O anexo inclui também a saída V2 anterior; não contar esse trecho como nova execução V2 demonstrada.

Controlador6120/criação134381245905057064 e runtime10080/criação134381246145672895 correlacionados pelos gates do observador. Runtime com SID virtual esperado,10 grupos, TokenElevation1 e TokenElevationTypeDefault1. Classe21 retornou1 byte e valor0; sessão0, nenhuma restricting SID, integridadeHigh(S-1-16-12288),7 privilégios. Administrators S-1-5-32-544 ausente da lista integral observada. SeChangeNotifyPrivilege, SeImpersonatePrivilege e SeCreateGlobalPrivilege habilitados (atributos3); demais quatro observados desabilitados. Isso não é prova completa de acesso efetivo, nem autoridade operacional.

A correção do decoder funcionou nesta consulta; não prova retroativamente o comprimento da V2. A forma atual confirma que TokenIsElevated não zero pode coexistir aqui com Default e ausência do grupo Administrators. Não substituir esses conceitos entre si nem fabricar bit0. O produto continua negando pelo predicado de elevação; o segundo guard SCQ também permanece estrito. Não alterar nenhum deles antes de confrontar a exigência normativa aplicável.

Primeiro gate técnico: localizar a cláusula accepted que exige elevated0 para o runtime virtual PERSONAL_LOCAL e confrontar sua finalidade com a forma observada, incluindo integridadeHigh e SeImpersonatePrivilege. Determinar se é defeito de implementação ou requer especialização normativa focal. Não conceder admissão apenas por SID virtual/ausência de Administrators/Default. Não iniciar nova campanha de observações.

Close conhecido refere-se apenas ao observador. Aplicações, bilateral original, receipt/close, retenção histórica e quiescência OS permanecem UNKNOWN. Não houve disposição, Start/Stop, UPDATE ou mudança de guard nesta reconciliação.
