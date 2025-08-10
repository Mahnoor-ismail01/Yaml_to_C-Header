#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "output.h"

char* single_line_description(const char* input) {
    if (!input) return NULL;
    char* output = strdup(input);
    if (!output) {
        perror("Error allocating memory for description");
        return NULL;
    }
    for (char* p = output; *p; p++) {
        if (*p == '\n' || *p == '\r') *p = ' ';
    }
    return output;
}

char* format_multi_line_content(const char* input) {
    if (!input) return NULL;
    char* output = strdup(input);
    if (!output) {
        perror("Error allocating memory for multi-line content");
        return NULL;
    }

    // Allocate enough space for output (account for indentation and newlines)
    size_t len = strlen(input) * 3 + 1024;
    char* temp = malloc(len);
    if (!temp) {
        perror("Error allocating memory for multi-line processing");
        free(output);
        return NULL;
    }
    temp[0] = '\0'; // Initialize output buffer

    // Process input character by character
    char* p = output;
    char* q = temp;
    const int indent_level = 2; // YAML standard 2-space indent
    int is_newline = 1;

    while (*p) {
        if (is_newline) {
            // Add 2-space indentation at the start of each line
            for (int i = 0; i < indent_level; i++) *q++ = ' ';
            is_newline = 0;
        }

        if (p[0] == '\\' && p[1] == 'n') {
            *q++ = '\n';
            p += 2;
            is_newline = 1; // Next character starts a new line
        } else if (p[0] == '\\' && p[1] == '$') {
            *q++ = '$';
            p += 2;
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';

    // Trim trailing whitespace and ensure a single trailing newline
    size_t temp_len = strlen(temp);
    while (temp_len > 0 && (temp[temp_len - 1] == ' ' || temp[temp_len - 1] == '\n')) {
        temp[--temp_len] = '\0';
    }
    if (temp_len == 0) {
        // Minimal valid literal block
        strcpy(temp, "  "); // Two spaces for indentation
    }
    strcat(temp, "\n"); // Ensure single trailing newline

    free(output);
    return temp;
}

int main() {
    FILE *yaml_file = fopen("outputnew.yaml", "w");
    if (!yaml_file) {
        perror("Error opening outputnew.yaml");
        return 1;
    }

    if (fprintf(yaml_file, "---\n") < 0) {
        perror("Error writing YAML header");
        fclose(yaml_file);
        return 1;
    }

    int data_written = 0;

#ifdef INST_KIND
    printf("Debug: INST_KIND is defined: %s\n", INST_KIND);
    if (fprintf(yaml_file, "kind: %s\n", INST_KIND) < 0) {
        perror("Error writing kind");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#else
    printf("Debug: INST_KIND is NOT defined\n");
#endif

#ifdef INST_NAME
    printf("Debug: INST_NAME is defined: %s\n", INST_NAME);
    if (fprintf(yaml_file, "name: %s\n", INST_NAME) < 0) {
        perror("Error writing name");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#else
    printf("Debug: INST_NAME is NOT defined\n");
#endif

#ifdef INST_LONG_NAME
    printf("Debug: INST_LONG_NAME is defined: %s\n", INST_LONG_NAME);
    if (fprintf(yaml_file, "long_name: %s\n", INST_LONG_NAME) < 0) {
        perror("Error writing long_name");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#else
    printf("Debug: INST_LONG_NAME is NOT defined\n");
#endif

#ifdef INST_DESCRIPTION
    printf("Debug: INST_DESCRIPTION is defined: %s\n", INST_DESCRIPTION);
    char* desc = single_line_description(INST_DESCRIPTION);
    if (!desc) {
        fclose(yaml_file);
        return 1;
    }
    if (fprintf(yaml_file, "description: %s\n", desc) < 0) {
        perror("Error writing description");
        free(desc);
        fclose(yaml_file);
        return 1;
    }
    free(desc);
    data_written++;
#else
    printf("Debug: INST_DESCRIPTION is NOT defined\n");
#endif

#ifdef INST_ASSEMBLY
    printf("Debug: INST_ASSEMBLY is defined: %s\n", INST_ASSEMBLY);
    if (fprintf(yaml_file, "assembly: %s\n", INST_ASSEMBLY) < 0) {
        perror("Error writing assembly");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#else
    printf("Debug: INST_ASSEMBLY is NOT defined\n");
#endif

#ifdef INST_ACCESS_S
    printf("Debug: INST_ACCESS_S is defined: %s\n", INST_ACCESS_S);
    if (fprintf(yaml_file, "access:\n") < 0) {
        perror("Error writing access");
        fclose(yaml_file);
        return 1;
    }
    if (fprintf(yaml_file, "  s: %s\n", INST_ACCESS_S) < 0) {
        perror("Error writing access.s");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#ifdef INST_ACCESS_U
    printf("Debug: INST_ACCESS_U is defined: %s\n", INST_ACCESS_U);
    if (fprintf(yaml_file, "  u: %s\n", INST_ACCESS_U) < 0) {
        perror("Error writing access.u");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#endif
#ifdef INST_ACCESS_VS
    printf("Debug: INST_ACCESS_VS is defined: %s\n", INST_ACCESS_VS);
    if (fprintf(yaml_file, "  vs: %s\n", INST_ACCESS_VS) < 0) {
        perror("Error writing access.vs");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#endif
#ifdef INST_ACCESS_VU
    printf("Debug: INST_ACCESS_VU is defined: %s\n", INST_ACCESS_VU);
    if (fprintf(yaml_file, "  vu: %s\n", INST_ACCESS_VU) < 0) {
        perror("Error writing access.vu");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#endif
#else
    printf("Debug: INST_ACCESS_S is NOT defined\n");
#endif

#ifdef INST_DATA_INDEPENDENT_TIMING
    printf("Debug: INST_DATA_INDEPENDENT_TIMING is defined: %d\n", INST_DATA_INDEPENDENT_TIMING);
    if (fprintf(yaml_file, "data_independent_timing: %s\n", INST_DATA_INDEPENDENT_TIMING ? "true" : "false") < 0) {
        perror("Error writing data_independent_timing");
        fclose(yaml_file);
        return 1;
    }
    data_written++;
#else
    printf("Debug: INST_DATA_INDEPENDENT_TIMING is NOT defined\n");
#endif

#ifdef INST_OPERATION__
    printf("Debug: INST_OPERATION__ is defined: %s\n", INST_OPERATION__);
    char* operation_content = format_multi_line_content(INST_OPERATION__);
    if (!operation_content) {
        fclose(yaml_file);
        return 1;
    }
    if (fprintf(yaml_file, "operation(): |\n%s", operation_content) < 0) {
        perror("Error writing operation()");
        free(operation_content);
        fclose(yaml_file);
        return 1;
    }
    free(operation_content);
    data_written++;
#else
    printf("Debug: INST_OPERATION__ is NOT defined\n");
#endif

    if (fprintf(yaml_file, "...\n") < 0) {
        perror("Error writing YAML footer");
        fclose(yaml_file);
        return 1;
    }

    fclose(yaml_file);

    if (data_written == 0) {
        fprintf(stderr, "Warning: No data written to YAML file. Check if output.h defines expected macros.\n");
    } else {
        printf("Generated YAML file: outputnew.yaml with %d fields written\n", data_written);
    }

    return 0;
}