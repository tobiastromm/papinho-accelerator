# Delta de imagem — snapshot offline, não seleção operacional

Selected/Release: 617984 bytes, SHA256
`944535ef159686b4d274fdb6af1c18d9d9dcf2ba045c49c56cc1a7b52ea3f497`.

Anterior de workspace: 598528 bytes,
`7b2ef0a2bdb8f60e783ba44667c0d50b6c2219615446f055cab5b072d5f352ee`;
preservado antes do novo build em build/private-bootstrap-preserved-20261031.
Delta: +19456 bytes.

Staged HISTÓRICO: 571392 bytes,
`c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf`.
Delta de conteúdo/tamanho candidato: +46592 bytes. Não houve abertura,
hash ou revalidação atual dos arquivos protegidos, cópia ou alteração SCM.
Os IDs históricos permanecem evidência datada; não atestam objetos atuais.

DLLs de workspace Selected/Release sem delta frente aos pins históricos:
libssl-3-x64.dll 1321472 bytes / `3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed`;
libcrypto-3-x64.dll 7217664 bytes / `09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044`.

O delta funcional EXE inclui a composição controladora, bridges CLI/owner,
fechamento pré-efeito/handler e censo explícito do objeto de plano. Não completa
os producers frios nem a admissão. Tamanho em disco não prova custo de execução.
Será necessária uma atualização governada para usar esse código no serviço,
mas NÃO agora: ainda há integração funcional pendente. Nenhum artefato final
foi congelado/selecionado, nenhum procedimento de overwrite foi executado e
nenhuma autorização consumida foi reutilizada. Uma futura atualização, endpoint
e Start diagnóstico exigem escopos e pedidos separados.
