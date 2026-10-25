# Revisão do criador N0 — 2026-10-24

Escopo: autorização funcional desta tarefa, sem autorização da operação real. HEAD main 913c90cf2328f7939d8f5b927640601903ac6e91. Preimagens em baseline.json e CMakeLists.preimage.txt; decisões accepted não alteradas.

Autoridades: DECISION-n0-protected-root-descriptor-text-errata-20261024, DECISION-n0-n9-security-descriptor-digest-profile-20261024, DECISION-first-native-lab-diagnostic-boundary-20261024; Model E, ED08/ED11 e B+D restringem a continuação. Nenhuma nova decisão normativa ou autoridade de admissão.

O executável de preparação embute os 104 bytes originais, SHA256 688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579. Não há conversão SDDL. Valida owner/group BA, controle 0x9004 e ACEs SYSTEM/Administrators FA OI/CI na ordem original.

A sequência valida e retém os pais originais, exige ausência, chama CreateDirectoryW exclusivamente uma vez por componente com SECURITY_ATTRIBUTES explícito, abre e verifica identidade, tipo, ausência de reparse e política do descriptor. Duplica referência ao mesmo objeto para continuar a cadeia e confirma fechamento da aquisição antes de avançar. Não adota objetos existentes. Não usa setters de ACL ou exclusão. Falhas preservam primeiro erro e efeitos parciais. Referência de fechamento incerto permanece retida; não há liberação forçada ou retry.

Limite conservador: o descriptor observado também deve satisfazer controle 0x9004. Se o Windows devolver outro controle, inclusive 0x9404, haverá STOP e possível árvore parcial. Não há qualificação factual de criação nativa nesta tarefa nem garantia antecipada de completar a árvore. Hash observado do objeto é registrado e revalidado separadamente do hash dos bytes de criação; não se afirma identidade binária entre layouts diferentes.

Selected/Legacy Debug/Release: quatro builds /W4 /WX, zero warnings; 78 verificações de modelo por variante e --describe em memória. Casos: digest, owner, DACL, ACE/order, colisão, reparse, identidade, falha parcial, replay, Stop, retenção e fechamento incerto. Modelos positivos não provam proteção nativa. Review do launcher PASS, hash errado DENY; Create não executado. Logs e manifests anexos. Revisão de fonte realizada nesta execução; não implica aprovação humana da operação futura.
