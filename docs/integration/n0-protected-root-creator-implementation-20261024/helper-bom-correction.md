# Correção focal do carregamento do helper

Resultado manual: ROOT_LAUNCHER_FIRST_ERROR com termo FEFF seguido de #. O helper original começa com BOM UTF-8 EF BB BF. GetString preservava U+FEFF ao criar ScriptBlock; o parser interpretava o início como comando. A falha ocorre na linha de carga do helper, antes do recibo e Process.Start. Consulta posterior: raiz e recibo ausentes. Nenhuma criação atribuída a essa execução; não é declaração de cleanup de outras tentativas.

Correção: verificar hash dos bytes originais, decodificar UTF-8 estrito e remover somente U+FEFF inicial antes de ScriptBlock.Create. O helper original e seu hash permanecem intactos. Sem mudança de autorização, descritor, binário ou efeitos. Launcher SHA256 2805150203bfe851543209b7cb87cb6f8718b4926254d11875b372f27bc534a2; entrada SHA256 7ddbaa350218095458554651f6c6b3c45078d952496adc50485f2e4980a07781. Pins anteriores do handoff são históricos; qualified-pins.json contém a correção.

Testes: Review da entrada PASS; carregamento isolado do helper com tratamento do BOM PASS, sem operações de arquivos ou execução do criador. Create não repetido. Sem alteração C, rebuild desnecessário. Reexecução manual requer autorização específica após essa falha, conforme NO_RETRY da operação original. ED11 PARTIAL, autoridades DENY.
