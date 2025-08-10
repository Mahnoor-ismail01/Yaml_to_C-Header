
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void restore_key(const char *macro, char *out) {
    size_t j = 0;
    for (size_t i = 0; macro[i] != '\0'; ++i) {
        if (macro[i] == '_') out[j++] = '.';
        else out[j++] = tolower((unsigned char)macro[i]);
    }
    out[j] = '\0';
}
void decode_c_escapes(char *s) {
    char *src = s, *dst = s;
    while (*src) {
        if (*src == '\\' && src[1]) {
            src++;
            char c = *src;
            if (c == 'n') *dst++ = '\n';
            else if (c == 'r') *dst++ = '\r';
            else if (c == 't') *dst++ = '\t';
            else if (c == '\\') *dst++ = '\\';
            else if (c == '"') *dst++ = '"';
            else if (c == 'x') {
               
                src++;
                int hi = 0, lo = 0, hlen = 0;
                if (isxdigit((unsigned char)*src)) {
                    char ch = *src++;
                    hi = (ch <= '9') ? ch - '0' : (tolower(ch) - 'a' + 10);
                    hlen++;
                }
                if (isxdigit((unsigned char)*src)) {
                    char ch = *src;
                    lo = (ch <= '9') ? ch - '0' : (tolower(ch) - 'a' + 10);
                    src++;
                    hlen++;
                }
                if (hlen >= 1) *dst++ = (char)((hi << 4) | lo);
                else *dst++ = 'x'; // fallback
                continue;
            } else {
                
                *dst++ = c;
            }
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}


int contains_newline(const char *s) {
    for (const char *p = s; *p; ++p) if (*p == '\n') return 1;
    return 0;
}


void print_yaml_value(FILE *out, const char *key, char *value) {
    
    if (contains_newline(value)) {
        
        fprintf(out, "%s: |\n", key);
        char *line = strtok(value, "\n");
        while (line) {
            fprintf(out, "  %s\n", line);
            line = strtok(NULL, "\n");
        }
    } else {
        
        fprintf(out, "%s: \"", key);
        for (const char *p = value; *p; ++p) {
            if (*p == '"') fputs("\\\"", out);
            else if (*p == '\\') fputs("\\\\", out);
            else fputc(*p, out);
        }
        fprintf(out, "\"\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.h output.yaml\n", argv[0]);
        return 2;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("fopen input"); return 3; }
    FILE *out = fopen(argv[2], "w");
    if (!out) { perror("fopen output"); fclose(in); return 4; }

    char line[8192];
    while (fgets(line, sizeof(line), in)) {
       
        char *p = line;
        while (*p && isspace((unsigned char)*p)) ++p;
        if (strncmp(p, "#define", 7) != 0) continue;
        p += 7;
        while (*p && isspace((unsigned char)*p)) ++p;
        char macro[1024];
        int i = 0;
        while (*p && !isspace((unsigned char)*p) && i < (int)sizeof(macro)-1) {
            macro[i++] = *p++;
        }
        macro[i] = '\0';

        
        while (*p && isspace((unsigned char)*p)) ++p;
        
        char value_buf[4096];
        size_t vi = 0;
        while (*p && *p != '\n' && vi < sizeof(value_buf)-1) {
            value_buf[vi++] = *p++;
        }
        value_buf[vi] = '\0';

        
        char *valptr = value_buf;
        if (valptr[0] == '"' && valptr[strlen(valptr)-1] == '"' && strlen(valptr) >= 2) {
            
            memmove(valptr, valptr+1, strlen(valptr)); 
           
            size_t L = strlen(valptr);
            if (L > 0 && valptr[L-1] == '"') valptr[L-1] = '\0';
        }

        
        decode_c_escapes(valptr);

        
        char key[1024];
        restore_key(macro, key);

        
        print_yaml_value(out, key, valptr);
    }

    fclose(in);
    fclose(out);
    return 0;
}
