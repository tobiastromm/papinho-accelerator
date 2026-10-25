# UTC/status issuer — revisão de fontes e gate normativo

Não foi identificado conflito entre decisões accepted. O primeiro bloqueio é insuficiência do contrato concreto de qualificação da observação UTC: ADR-0006 e CA1 §1.4 exigem wall clock/UTC confiável; U4 não considera callback/relógio OS prova suficiente. Faltam origem operacional qualificada, erro/vigência verificáveis e comportamento de rollback ligados ao owner.

| Fonte real | Evidência | Limite / ação |
|---|---|---|
| issuer_backend.utc | issuer_core.h devolve escalar U64 UTC; core consulta em prepare/sign/publicação | Unidade de domínio e validade são usadas, mas não há origem/qualidade/lifetime/rollback na porta; não elevar scalar a fonte qualificada |
| CNG utc | server_issuer_cng_backend_win32.c chama administrative->utc após ready | É delegação, não fonte de qualidade; ponteiro não NULL não comprova confiabilidade |
| Monotônico PAL | pal_time_win32.c usa QPC/frequency e conversão checked para ns | Reutilizável para duração/ordenação; não tem calendário nem prova UTC, não substitui wall clock |
| Relógio Site TLS | launcher_site_profile lê GetSystemTimeAsFileTime sob guard próprio | Contexto distinto; não qualifica issuer e não foi adaptado por inferência |
| Issuer root read | issuer_root_port_read lê/copía imagem e encontra registro, deixando epoch/proofs ausentes explicitamente | Estado/DER/histórico existem como DATA; não preencher CURRENT/fencing/key/prefix proofs para satisfazer o core |
| Fixtures UTC | atribuições administrative.utc/clock_value em testes test_only | Testam domínio, não precisão ou origem nativa; não foram promovidas |
| Ato UPDATE | prova nativa224 e cohort304 já implementados, status SOURCE_ABSENT | Autorização do ato não atesta tempo, status ou delegação; captura anterior permanece intacta |

## Extensão focal Q0-Q7 PROPOSED

[Contrato proposto](UTC-STATUS-SOURCE-CONTRACT-PROPOSED.md) define observação de UTC pelo backend administrativo existente e duração pelo PAL, origem/qualidade/proteção da instância, intervalo conservador de erro, vigência monotônica, revalidação de fonte e cálculo de validade sem tolerância permissiva. Erro/vigência/taxa precisam evidência do perfil de fonte/target qualificado; não são valores livres ou booleans de config. Nenhum fornecedor externo, relógio virtual ou serviço novo é selecionado. Fonte Windows do OS é apenas candidata, dependente de prova operacional de qualidade; API disponível não aprova a fonte.

Rollback, monotônico regressivo, troca de fonte, descontinuidade/inconsistência/estado desconhecido invalidam o avanço, sem congelar hora favorável ou retry até sucesso. Restart não restaura observação, ato, CURRENT, high-water ou fencing. Intervalos não ampliam notBefore/notAfter; se erro/rate/expiry não puderem ser comprovados, DENY. Proposta não altera assinatura backend.utc nem formatos persistentes.

Q5 preserva consulta de status pelo owner de root com CURRENT/pin/lifetime reais. Certificado e delegação têm estados/validade/revogação separados; dado PDG ou certificado válido não aprova a candidata. Snapshot/pin protege memória, não prova que status permanece vigente. Diante da ausência da fonte UTC e de provas factuais da root, não há integração positiva segura para aplicar nesta tarefa. Seção3 autoriza a proposta focal; seção9 impede UTC/status presumidos.

## Sequência mínima após eventual aceitação

1. Identificar e qualificar operacionalmente a instância de tempo já implantada, com mecanismo verificável de origem/qualidade/erro/vigência, sem nova rede ou alteração de relógio. Se não existir, fonte permanece indisponível; não preencher dados fictícios para implementar a interface.
2. Fixar representação privada transitória no mesmo owner, censo e lifecycle da observação/qualificação. Qualquer prova externa precisa proteção própria; E0 acquire não deve iniciar consultas/aquisições. Não aumentar quotas nem usar previous/current como scratch.
3. Conectar backend administrativo issuer existente, leitura de status/certificados/histórico sob CURRENT/pin e a candidata U0-U7, revalidando fonte/ato/identidade/revisão/tuplas/intervalos antes do efeito. Não substituir provas ausentes da porta root por flags.
4. Implementar regressões numéricas/negativas isoladas, depois qualificar fonte nativa e contexto administrativo em ambiente autorizado. Resultados sintéticos não encerram qualificação de UTC/status. Commit v3, publicação e TRUST permanecem fora do escopo.

## ED11 e evidências desta tarefa

Sem patch funcional ou alocação runtime: delta0. Censo fixo anterior Windows1948880 e Legacy1790872 mantido, sem medir pico novo. Proof224/cohort304, backings116616 e imagens91776 já contabilizados. Futuras observações anterior/nova, erros/qualificação/taxa, buffers, DER/workspaces, caller/stack/handles e retidos exigem censo compilado e prova de simultaneidade; não há economia presumida. Cenário completo conservador3747825 excede3MiB em602097, não pico/reserva; provider/kernel/stack transitivo UNKNOWN e totalPARTIAL.

Builds/testes/rede NOT_RUN nesta revisão documental sem fonte qualificada ou código novo. Os4 builds/74 suítes/24 casos do closeout anterior são história preservada, não reexecução nem prova de UTC qualificada. Q7 lista os casos futuros, incluindo rollback, status desconhecido/revogado, fonte ausente, pin/revisão divergentes e retenção. WIP de ambos os projetos, .gitignore Browser e quatro LAB preservados; nenhum relógio/token/ACL foi alterado.
