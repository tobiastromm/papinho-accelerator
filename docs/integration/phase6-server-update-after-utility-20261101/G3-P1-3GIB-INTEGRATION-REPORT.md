# Piso de commit P1 — 3 GiB integrados e qualificados offline

2026-11-02, main/f341985924d29870d8ff627ed37c8a0874ad6469. HEAD/WIP preservados.
Proprietário determinou reduzir de8 para3 e confirmou literalmente a unidade
“3 GiB — 3.221.225.472 bytes”. A decisão normativa foi registrada antes do código
em docs/adr/DECISION-personal-scq-p1-commit-floor-3gib-20261102.md, ACCEPTED.
Precedência só sobre o campo commit P1 da decisão8GiB; antigos accepted,
candidatos e históricos preservados sem revisão silenciosa. Risco administrativo
escolhido, não suficiência nativa/consumo/reserva demonstrados. Sem conflito residual
no escopo numérico aprovado, nenhuma autoridade operacional implícita.

Mudança de produção mínima: constante privada PAPACC_SCQ_P1_COMMIT_FLOOR
3221225472ULL. Factories/verificadores/sample/preimage já qualificados a usam
no controlador e receptor. scqc_plan/capture exigem seleção legítima P1 e valor
exato; QUERY receptor usa a mesma seleção compilada antes de comparar policy.
P1 ausente/divergente/antiga8GiB nega; nenhuma ampliação para READ/NEW_INSTALL/
TO sem P1/UPDATE/SCM ou fallback por CLI/peer. Helper de dados não concede origem
administrativa; aquisição/proveniência/identidade/proteção/ancestrais/reparse/
continuidade/atos/consumo permanecem gates originais independentes.

Piso legacy9823326208, físico7239172096, managed3MiB, privado/WS1994944,
entrada128/guard188/cap192, freshness100ms/janela240000ms/global60000ms/menor
prazo intactos. Nenhuma mudança no packet, recursos/handles controlados,
wire público/persistido, serviço, transporte ou owner. Stop/replay/UNKNOWN,
bilateral/empréstimo/receipt-close antes de RUNNING somente diagnóstico intactos.

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings; 68 pares
(17/config) PASS. Fronteiras3221225471/exato/+1; mesmo sample nega legacy;
novo valor sem P1, antigo8GiB/legacy9823326208bytes em P1, inválidos/overflow/NULL/
consulta indisponível, Stop/UNKNOWN e readiness prematura negam. Cinco casos
negativos no corpo original cold-plan negam antes de APIs nativas. Guards de
handles e modelos de source/host/query/bilateral permanecem PASS. Censos
Selected2002152/Legacy1833584 inalterados. APIs protegidas modeladas; nenhuma
consulta nativa/SCM/endpoint/Start/Stop/UPDATE ou outro efeito real executado.

Policy quantitativa antiga8GiB256fe6ab14dcc5554994b589017b1c8e7d7817bcdac89982d44d73048a31cafd;
nova3GiB3961396a607d265f73c730e9599fa8208313a59e65832a24b170bf8206bd15a1.
Preimage continua84bytes; só bytes commit mudam. Binding tardio/PID/token
futuros não são presumidos. Receptor rejeita policy divergente, sem downgrade.
Logs g3-p1-3gib-* e hashes em G3-P1-3GIB-QUALIFICATION.json.

Novo Selected/Release786944bytes/SHA256
bf2f8c67a3f5de3a5a4b88f28b69eb8573aec04db1df8e36c7907f999a6a748a,
congelado papacc_server.g3-p1-3gib-786944.exe. Mesmo tamanho, bytes diferentes.
Instalado ainda é o8GiB historicamente786944/ID417614100:1900544:290164/
pin40b708daadae4342e5cd90090d99189234437ab84f47b2d00f76aeaab05eba76;
nenhuma consulta instalada feita. DLLs idênticas e updater independente mantidos.

Pedido ONE-G3-P1-3GIB-IMAGE-UPDATE-REQUEST.CANDIDATE.md pronto para autorização
específica. A instrução de alterar o parâmetro foi registrada como norma e
implementação offline; não equivale a autorização byte-exata de UPDATE ou E/Q/S.
Não executar automaticamente. Após aprovação/UPDATE manual/readback/ID real,
preparar sessão independente com novos originais, argv commit3221225472 e nova
policy; três atos E/Q/S e autorização operacional própria. Não reutilizar
246173b9... de UPDATE consumido ou f87d9281... de sessão usada. Candidato E/Q/S
90b68869...8GiB fica histórico, não deve ser executado como3GiB.

Históricos/OS/retidos UNKNOWN, produtivo DENY. Preservar todos os objetos/EXEs/
pins/atos/reservas/fixture/PST0.6.4; sem retry/rollback/limpeza/refund/recuperação.
