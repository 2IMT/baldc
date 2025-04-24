#ifndef BALDC_LEX_H
#define BALDC_LEX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "str.h"

enum bc_tok_kind {
    BC_TOK_STRING,
    BC_TOK_CHARACTER,
    BC_TOK_INTEGER,
    BC_TOK_FLOATING,
    BC_TOK_BOOLEAN,
    BC_TOK_IDENT,
    BC_TOK_IMPORT,
    BC_TOK_STRUCT,
    BC_TOK_ENUM,
    BC_TOK_FUNC,
    BC_TOK_LPAREN,
    BC_TOK_RPAREN,
    BC_TOK_LBRACE,
    BC_TOK_RBRACE,
    BC_TOK_LBRACKET,
    BC_TOK_RBRACKET,
    BC_TOK_COLON,
    BC_TOK_DOT,
    BC_TOK_COMMA,
    BC_TOK_SEMICOLON,
    BC_TOK_PLUS,
    BC_TOK_MINUS,
    BC_TOK_STAR,
    BC_TOK_SLASH,
    BC_TOK_COUNT,
};

union bc_tok_val {
    struct bc_strv string;
    int32_t character;
    int64_t integer;
    double floating;
    bool boolean;
    struct bc_strv ident;
};

struct bc_tok {
    enum bc_tok_kind kind;
    union bc_tok_val val;
};

struct bc_lex_pos {
    size_t l;
    size_t c;
};

struct bc_lex_loc {
    struct bc_lex_pos s;
    struct bc_lex_pos e;
};

struct bc_lex {
    struct bc_strv src;
    struct bc_lex_pos pos;
};

enum bc_lex_res {
    BC_LEX_ERR = -1,
    BC_LEX_OK,
    BC_LEX_EMPTY,
};

struct bc_lex bc_lex_new(struct bc_strv src);

enum bc_lex_res bc_lex_next(
    struct bc_lex* lex, struct bc_tok* tok, struct bc_lex_loc* loc);

#endif
