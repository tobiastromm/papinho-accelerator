# Integração posterior à aceitação — não implementada

Perfil/admin/original binding, OHI/handoff, receptor, driver, transporte, dispatch e owner/host precisam validar o mesmo propósito tipado. Hoje os checks READ=1 e scqp_key continuam obrigatórios.

Após aceitação: propósito privado 2 e ausência canônica integral de key somente na rota fechada de transporte; READ=1 preservado. Construtores ordinários rejeitam o novo propósito. Adicionar discriminador privado no dispatch se necessário, sem reaproveitar campos de erro ou autoridade. Bloquear qualquer entrada KSP antes do provider. Não reduzir quotas por key vazio.

Revalidar censo e lifetime após mudanças. O binário selecionado não implementa a proposta; outro hash exige nova seleção explícita. Nenhum defeito técnico independente foi demonstrado que justifique alterar código antes da decisão. A continuação nativa, fontes originais e política quantitativa ainda precisam de prova; modelos não as concedem.
