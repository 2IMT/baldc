#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

#include "lex.h"
#include "str.h"
#include "print.h"

static void _print_err(struct bc_lex_err err, const char* src) {
    switch (err.kind) {
    case BC_LEX_ERR_INVALID_UTF8_SEQUENCE: {
        bc_eprintf("invalid UTF-8 sequence$n");

    } break;
    case BC_LEX_ERR_UNTERMINATED_STRING: {
        struct bc_lex_pos pos = err.val.unterminated_string;
        bc_eprintf("unterminated string literal (starts at $s:$z:$z)$n", src,
            pos.l, pos.c);
    } break;
    case BC_LEX_ERR_UNTERMINATED_CHARACTER: {
        struct bc_lex_pos pos = err.val.unterminated_character;
        bc_eprintf("unterminated character literal (starts at $s:$z:$z)$n", src,
            pos.l, pos.c);
    } break;
    case BC_LEX_ERR_UNEXPECTED_CHARACTER_IN_NUMBER: {
        bc_eprintf("unexpected character `$C` in number literal$n",
            err.val.unexpected_character_in_number);
    } break;
    case BC_LEX_ERR_UNEXPECTED_CHARACTER: {
        bc_eprintf("unexpected character `$C`$n", err.val.unexpected_character);
    } break;
    case BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE: {
        struct bc_lex_pos pos = err.val.invalid_escape_sequence;
        bc_eprintf("invalid escape sequence at $z:$z$n", pos.l, pos.c);
    } break;
    case BC_LEX_ERR_MULTICHARACTER: {
        bc_eprintf("character literal contains more than one character$n");
    } break;
    case BC_LEX_ERR_EMPTY_CHARACTER: {
        bc_eprintf("character literal is empty$n");
    } break;
    case BC_LEX_ERR_NON_PRINTABLE_CHARACTER: {
        bc_eprintf("non-printable character encountered$n");
    } break;
    case BC_LEX_ERR_INVALID_INTEGER_PREFIX: {
        bc_eprintf(
            "invalid integer prefix `$C`$n", err.val.invalid_integer_prefix);
    } break;
    case BC_LEX_ERR_NO_DIGIT_AFTER_PREFIX: {
        bc_eprintf("no digit after prefix in integer literal$n");
    } break;
    case BC_LEX_ERR_BYTE_POSTFIX_IN_FLOATING: {
        bc_eprintf("byte postfix in floating point literal$n");
    } break;
    case BC_LEX_ERR_NEGATIVE_BYTE_LITERAL: {
        bc_eprintf("negative byte literal$n");
    } break;
    }
}

static void _print_tok(struct bc_tok tok) {
    const char* n = NULL;
    struct bc_strv v;
    char delim = '\0';
    switch (tok.kind) {
    case BC_TOK_IDENT:
        n = "ident";
        v = tok.val.ident;
        break;
    case BC_TOK_LIT_STRING:
        n = "lit s";
        v = tok.val.string;
        delim = '"';
        break;
    case BC_TOK_LIT_CHARACTER:
        n = "lit c";
        v = tok.val.character;
        delim = '\'';
        break;
    case BC_TOK_LIT_INTEGER:
        n = "lit i";
        v = tok.val.integer;
        break;
    case BC_TOK_LIT_BYTE:
        n = "lit bt";
        v = tok.val.byte;
        break;
    case BC_TOK_LIT_FLOATING:
        n = "lit f";
        v = tok.val.floating;
        break;
    case BC_TOK_LIT_BOOLEAN:
        n = "lit b";
        if (tok.val.boolean) {
            v = BC_STRV_FROM_LIT("true");
        } else {
            v = BC_STRV_FROM_LIT("false");
        }
        break;
    case BC_TOK_KW_STRING:
        n = "keyword";
        v = BC_STRV_FROM_LIT("string");
        break;
    case BC_TOK_KW_CHAR:
        n = "keyword";
        v = BC_STRV_FROM_LIT("char");
        break;
    case BC_TOK_KW_INT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("int");
        break;
    case BC_TOK_KW_BYTE:
        n = "keyword";
        v = BC_STRV_FROM_LIT("byte");
        break;
    case BC_TOK_KW_FLOAT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("float");
        break;
    case BC_TOK_KW_BOOL:
        n = "keyword";
        v = BC_STRV_FROM_LIT("bool");
        break;
    case BC_TOK_KW_IMPORT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("import");
        break;
    case BC_TOK_KW_EXPORT:
        n = "keyword";
        v = BC_STRV_FROM_LIT("export");
        break;
    case BC_TOK_KW_AS:
        n = "keyword";
        v = BC_STRV_FROM_LIT("as");
        break;
    case BC_TOK_KW_TRY_AS:
        n = "keyword";
        v = BC_STRV_FROM_LIT("try_as");
        break;
    case BC_TOK_KW_TRY:
        n = "keyword";
        v = BC_STRV_FROM_LIT("try");
        break;
    case BC_TOK_KW_CATCH:
        n = "keyword";
        v = BC_STRV_FROM_LIT("catch");
        break;
    case BC_TOK_KW_DEFER:
        n = "keyword";
        v = BC_STRV_FROM_LIT("defer");
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
    case BC_TOK_COUNT:
        n = "[invalid]";
        v = BC_STRV_FROM_LIT("[invalid]");
    }
    char delim_str[] = { delim, '\0' };
    bc_printf("$s\t$s" BC_STRV_PRNT "$s$n", n, delim_str, BC_STRV_PRNTV(v),
        delim_str);
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        bc_eprintf("error: no source file provided$n");
        return 1;
    }
    const char* src = argv[1];

    struct bc_str file_data = bc_str_new();
    FILE* file = fopen(src, "r");
    if (file == NULL) {
        bc_eprintf(
            "error: failed to open file `$s`: $s$n", src, strerror(errno));
        bc_str_free(file_data);
        return 1;
    }
#define BUFFER_SIZE (1024 * 8)
    char buffer[BUFFER_SIZE] = { 0 };
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        if (ferror(file)) {
            bc_eprintf(
                "error: failed to read file `$s`: $s$n", src, strerror(errno));
            bc_str_free(file_data);
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
            struct bc_lex_pos pos = lex.err.pos;
            bc_eprintf("$s:$z:$z: error: ", src, pos.l, pos.c);
            _print_err(lex.err, src);
        } else {
            bc_printf(
                "$s:$z:$z-$z:$z:\t", src, loc.s.l, loc.s.c, loc.e.l, loc.e.c);
            _print_tok(tok);
        }
    }

    bc_lex_free(lex);
    bc_str_free(file_data);

    return 0;
}
