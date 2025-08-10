#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void restore_key(const char *macro, char *out) {
    int out_i = 0;
    for (int i = 0; macro[i]; i++) {
        if (macro[i] == '_') out[out_i++] = '.';
        else out[out_i++] = tolower((unsigned char)macro[i]);
    }
    out[out_i] = '\0';
}

void yaml_escape_and_print(FILE *out, const char *key, const char *value) {
    int needs_block = strchr(value, '\n') != NULL;
    fprintf(out, "%s: ", key);

    if (needs_block) {
        fprintf(out, "|\n");
        const char *p = value;
        while (*p) {
            if (*p == '\n') {
                fputc('\n', out);
                if (*(p+1)) fputs("  ", out); // indent continuation lines
            } else {
                fputc(*p, out);
            }
            p++;
        }
        fputc('\n', out);
    } else {
        fprintf(out, "\"");
        for (const char *p = value; *p; p++) {
            if (*p == '"') fputs("\\\"", out);
            else fputc(*p, out);
        }
        fprintf(out, "\"\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.h output.yaml\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("input"); return 1; }
    FILE *out = fopen(argv[2], "w");
    if (!out) { perror("output"); fclose(in); return 1; }

    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        if (strncmp(line, "#define", 7) == 0) {
            char macro[256], value[768];
            if (sscanf(line, "#define %255s %767[^\n]", macro, value) == 2) {
                char key[256];
                restore_key(macro, key);

                // Remove wrapping quotes from value if present
                size_t len = strlen(value);
                if (len >= 2 && value[0] == '"' && value[len-1] == '"') {
                    value[len-1] = '\0';
                    memmove(value, value+1, len-1);
                }
                yaml_escape_and_print(out, key, value);
            }
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
