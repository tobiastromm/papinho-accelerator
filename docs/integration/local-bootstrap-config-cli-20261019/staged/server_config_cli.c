#include "server_config_cli.h"
#include "config_source.h"
#include <string.h>
/* One bounded CLI invocation, before root/console/network construction. No
 * pointer escapes. The workspace cannot coexist with live runtime via main. */
typedef struct CONFIG_CLI_WORKSPACE {
    PC_DOCUMENT document;
    unsigned char input[PC_SOURCE_MAX + 1U];
    PC_DIAGNOSTIC diagnostic;
    PC_TARGET target;
} CONFIG_CLI_WORKSPACE;
size_t papacc_server_config_cli_workspace_bytes(void)
{ return sizeof(CONFIG_CLI_WORKSPACE); }
static int validate_source(const char *path, FILE *out, FILE *err)
{
    CONFIG_CLI_WORKSPACE w;
    FILE *file;
    size_t n;
    int read_failed, close_failed;
    PC_RESULT result;
    unsigned i;
    const PC_LINE *line;
#if defined(_MSC_VER)
    file = NULL;
    if (fopen_s(&file, path, "rb") != 0) file = NULL;
#else
    file = fopen(path, "rb");
#endif
    if (file == NULL) {
        fputs("SOURCE=UNAVAILABLE; CURRENT=NOT_READ\n", err);
        return 1;
    }
    n = fread(w.input, 1U, sizeof(w.input), file);
    read_failed = ferror(file);
    close_failed = fclose(file);
    /* fclose is attempted exactly once. A close error is not success or a
     * retry invitation. No root, lease, credit or worker exists in this path. */
    if (read_failed || close_failed != 0) {
        fputs("SOURCE=IO_INCOMPLETE; CLOSE_NOT_PROVEN; CURRENT=NOT_READ\n", err);
        return 1;
    }
    memset(&w.diagnostic, 0, sizeof(w.diagnostic));
    result = pc_parse(w.input, n, &w.document, &w.diagnostic);
    if (result == PC_OK) {
        w.target.max_listeners = 1U;
        w.target.legacy_supported = 0;
        result = pc_validate(&w.document, &w.target, &w.diagnostic);
    }
    if (result != PC_OK) {
        fprintf(err, "SOURCE=INVALID code=%u line=%u column=%u reason=%s\n",
            (unsigned)result, w.diagnostic.line, w.diagnostic.column,
            w.diagnostic.reason != NULL ? w.diagnostic.reason : "unspecified");
        return 1;
    }
    fputs("SOURCE=VALIDATED; MODEL_BOUND=NO; RESOLVED=NO; CURRENT=NOT_READ\n", out);
    /* Structural validity does not resolve even unused declared objects.
     * No named provider, SELECTED topology or root source is available here. */
    for (i = 0U; i < w.document.line_count; ++i) {
        line = &w.document.lines[i];
        if (strcmp(line->tokens[0].text, "provider_ref") == 0 ||
            strcmp(line->tokens[0].text, "interface_ref") == 0) {
            fprintf(err, "REFERENCES=DENY line=%u column=%u reason=%s\n",
                line->line, line->tokens[0].column,
                strcmp(line->tokens[0].text, "provider_ref") == 0 ?
                "AUTH_PROVIDER_PENDING" : "INTERFACE_TOPOLOGY_NOT_QUALIFIED");
            return 2;
        }
    }
    if (w.document.block_count != 0U) {
        line = &w.document.lines[w.document.blocks[0].opener];
        fprintf(err, "REFERENCES=UNRESOLVED line=%u column=%u reason=FACTUAL_ROOT_SOURCE_UNAVAILABLE\n",
            line->line, line->tokens[0].column);
        return 2;
    }
    fputs("REFERENCES=NOT_APPLICABLE; VALIDATION_SCOPE=OFFLINE_ONLY; APPLY=DENY\n", out);
    return 0;
}
int papacc_server_config_cli(int argc, const char *const *argv,
    FILE *out, FILE *err)
{
    int i, result;
    const char *command;
    if (argc < 2 || argv == NULL || argv[1] == NULL ||
        strcmp(argv[1], "config") != 0) return -1;
    if (out == NULL || err == NULL) return 1;
    for (i = 0; i < argc; ++i) if (argv[i] == NULL) return 1;
    if (argc < 3) {
        fputs("CONFIG=INVALID_COMMAND\n", err); return 1;
    }
    command = argv[2];
    if (strcmp(command, "show-effective") == 0 && argc == 3) {
        fputs("SHOW_EFFECTIVE=UNAVAILABLE; CURRENT_PIN=ABSENT; NO_EFFECTIVE_STATE_EMITTED\n", err);
        return 2;
    }
    if ((strcmp(command, "validate") == 0 || strcmp(command, "diff") == 0 ||
         strcmp(command, "apply") == 0) && argc == 5 &&
         strcmp(argv[3], "--source") == 0 && argv[4][0] != '\0') {
        if (strcmp(command, "apply") == 0) {
            fputs("APPLY=DENY_UNTIL_QUALIFIED; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY; CARRIER_ISSUER=UNSUPPORTED\n", err);
            return 2; /* no file open, root, transaction or worker */
        }
        result = validate_source(argv[4], out, err);
        if (strcmp(command, "diff") == 0) {
            fputs("DIFF=UNAVAILABLE; CURRENT_PIN=ABSENT; NO_COMPARISON_EMITTED\n", err);
            return result == 1 ? 1 : 2;
        }
        return result;
    }
    fputs("CONFIG=INVALID_COMMAND; EXPECTED=config validate|diff|apply --source <file> OR config show-effective\n", err);
    return 1;
}
