# Proveniência, API, wire e distribuição

Fonte de desenvolvimento: PapinhoAccelerator. Browser consome third_party/PapinhoPACC-1.1.0.zip e third_party/papinhopacc sob BrowserADR0013. package-comparison.json compara todas as20 entradas com snapshot e origem: nenhuma diferença. Archive SHA256:1e3637506a8746ec19b75f41ae54022f037e3d6305d003459967bb20586328a4.

| Identificador | Valor | Semântica |
|---|---|---|
| Pacote |1.1.0|distribuição pinada|
| C API |1.1.0|chamadas, independente do wire|
| Wire generation |2|metadata cumulativa, não campo wire|
| Envelope |1.0|header16|
| CONTROL |1.0|versão própria|

Generation2 preserva IDs1..14 e acrescenta NETWORK_ABORT_REASON0x000F para NETWORK_EGRESS major2. TLS_TRUNCATED=1; zero inválido, nonzero desconhecido terminal genérico não limpo. Nenhuma alteração nesta auditoria. PAE1/enrollment e PACCMP02/package são contratos separados, não extensão implícita de papacc/1.

API pública: frame/frame_parser, control_protocol, data_association_protocol, capability_wire, capability_negotiation_protocol, network_offload_protocol, message_types, version e types. Sete fontes papacc_protocol, sem sockets/PST/Win32/runtime/policy. Parser retorna views apenas do input da chamada, não retém input. Capacidades/EOF explícitos. Browser client usa papacc_frame_header_encode/parser_feed desse pacote, não redefine o framing operacional.

ABI: source package; não há promessa de .lib intercambiável entre VC6/x64/CRT/compiladores. size_t, enum e structs públicos são locais, não serialização. types.h tem ramo VC6 sem stdint e checks de largura. CMake C90/3.16 é caminho de build host, não promessa de CMake em NT4. Browser usa Makefile.nt4.mak. C++ ABI/box/ISA/NT4 runtime não são qualificados por headers ou builds modernos.

Atualizações: fixes upstream, nova versão governada, archive+snapshot+provenance atômicos; sem editar vendored. Sem fetch/sibling checkout no build consumidor. Não regenerar ZIP nesta auditoria. Release remoto não consultado por rede; live doc local registra publicação pendente, não se declara estado atual do GitHub por inferência.

Não há LICENSE/NOTICE no ZIP nem termos próprios selecionados. É gate de distribuição, não licença concedida pela auditoria. Preservar MPL2 do PBKDF2 derivado NSS, BearSSL LICENSE.txt e termos dos backends selecionados. Dependências opcionais não viram obrigatórias no pacote wire-only. Nenhum release/pin de produção foi alterado.
