#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

#include "lex.h"
#include "str.h"

static void _print_tok(struct bc_tok tok) {
    const char* n = NULL;
    struct bc_strv v;
    switch (tok.kind) {
    case BC_TOK_IDENT:
        n = "ident";
        v = tok.val.ident;
        break;
    case BC_TOK_LIT_STRING:
        n = "literal";
        v = tok.val.string;
        v.data--;
        v.len += 2;
        break;
    case BC_TOK_LIT_CHARACTER:
        n = "literal";
        v = tok.val.character;
        v.data--;
        v.len += 2;
        break;
    case BC_TOK_LIT_INTEGER:
        n = "literal";
        v = tok.val.integer;
        break;
    case BC_TOK_LIT_FLOATING:
        n = "literal";
        v = tok.val.floating;
        break;
    case BC_TOK_LIT_BOOLEAN:
        n = "literal";
        if (tok.val.boolean) {
            v = BC_STRV_FROM_LIT("true");
        } else {
            v = BC_STRV_FROM_LIT("false");
        }
        break;
    case BC_TOK_KW_IMPORT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("import");
        break;
    case BC_TOK_KW_STRUCT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("struct");
        break;
    case BC_TOK_KW_ENUM:
        n = "keyword";
        v = BC_STRV_FROM_LIT("enum");
        break;
    case BC_TOK_KW_FUNC:
        n = "keyword";
        v = BC_STRV_FROM_LIT("func");
        break;
    case BC_TOK_KW_LET:
        n = "keyword";
        v = BC_STRV_FROM_LIT("let");
        break;
    case BC_TOK_LPAREN:
        n = "sym";
        v = BC_STRV_FROM_LIT("(");
        break;
    case BC_TOK_RPAREN:
        n = "sym";
        v = BC_STRV_FROM_LIT(")");
        break;
    case BC_TOK_LBRACE:
        n = "sym";
        v = BC_STRV_FROM_LIT("{");
        break;
    case BC_TOK_RBRACE:
        n = "sym";
        v = BC_STRV_FROM_LIT("}");
        break;
    case BC_TOK_LBRACKET:
        n = "sym";
        v = BC_STRV_FROM_LIT("[");
        break;
    case BC_TOK_RBRACKET:
        n = "sym";
        v = BC_STRV_FROM_LIT("]");
        break;
    case BC_TOK_LANGLE:
        n = "sym";
        v = BC_STRV_FROM_LIT("<");
        break;
    case BC_TOK_RANGLE:
        n = "sym";
        v = BC_STRV_FROM_LIT(">");
        break;
    case BC_TOK_COLON:
        n = "sym";
        v = BC_STRV_FROM_LIT(";");
        break;
    case BC_TOK_DOT:
        n = "sym";
        v = BC_STRV_FROM_LIT(".");
        break;
    case BC_TOK_COMMA:
        n = "sym";
        v = BC_STRV_FROM_LIT(",");
        break;
    case BC_TOK_SEMICOLON:
        n = "sym";
        v = BC_STRV_FROM_LIT(";");
        break;
    case BC_TOK_EQ:
        n = "sym";
        v = BC_STRV_FROM_LIT("=");
        break;
    case BC_TOK_PLUS:
        n = "sym";
        v = BC_STRV_FROM_LIT("+");
        break;
    case BC_TOK_DASH:
        n = "sym";
        v = BC_STRV_FROM_LIT("-");
        break;
    case BC_TOK_STAR:
        n = "sym";
        v = BC_STRV_FROM_LIT("*");
        break;
    case BC_TOK_SLASH:
        n = "sym";
        v = BC_STRV_FROM_LIT("/");
        break;
    case BC_TOK_AMP:
        n = "sym";
        v = BC_STRV_FROM_LIT("&");
        break;
    case BC_TOK_PIPE:
        n = "sym";
        v = BC_STRV_FROM_LIT("|");
        break;
    case BC_TOK_CARET:
        n = "sym";
        v = BC_STRV_FROM_LIT("^");
        break;
    default:
        n = "[invalid]";
        v = BC_STRV_FROM_LIT("[invalid]");
    }

    printf("%s\t" BC_STRV_FORMAT "\n", n, BC_STRV_FORMATV(v));
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        fprintf(stderr, "error: no source file provided\n");
        return 1;
    }
    const char* src = argv[1];

    struct bc_str file_data = bc_str_new();
    FILE* file = fopen(src, "r");
    if (file == NULL) {
        fprintf(stderr, "error: failed to open file `%s`: %s\n", src,
            strerror(errno));
        return 1;
    }
#define BUFFER_SIZE (1024 * 8)
    char buffer[BUFFER_SIZE] = { 0 };
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        if (ferror(file)) {
            fprintf(stderr, "error: failed to read file `%s`: %s\n", src,
                strerror(errno));
            return 1;
        }
        bc_str_push_cstrn(&file_data, buffer, bytes_read);
    }

    fclose(file);

    struct bc_lex lex = bc_lex_new(bc_strv_from_str(file_data));

    struct bc_tok tok;
    struct bc_lex_loc loc;
    while (true) {
        enum bc_lex_res res = bc_lex_next(&lex, &tok, &loc);
        if (res == BC_LEX_EMPTY) {
            break;
        }
        if (res == BC_LEX_ERR) {
            fprintf(stderr, "error: invalid syntax\n");
            return 1;
        }
        printf(
            "%s:%zu:%zu-%zu:%zu:\t", src, loc.s.l, loc.s.c, loc.e.l, loc.e.c);
        _print_tok(tok);
    }

    return 0;
}
