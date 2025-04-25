#ifndef BALDC_LEX_H
#define BALDC_LEX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "str.h"

enum bc_tok_kind {
    BC_TOK_IDENT,
    BC_TOK_LIT_STRING,
    BC_TOK_LIT_CHARACTER,
    BC_TOK_LIT_INTEGER,
    BC_TOK_LIT_FLOATING,
    BC_TOK_LIT_BOOLEAN,
    BC_TOK_KW_IMPORT,
    BC_TOK_KW_STRUCT,
    BC_TOK_KW_ENUM,
    BC_TOK_KW_FUNC,
    BC_TOK_KW_LET,
    BC_TOK_LPAREN,
    BC_TOK_RPAREN,
    BC_TOK_LBRACE,
    BC_TOK_RBRACE,
    BC_TOK_LBRACKET,
    BC_TOK_RBRACKET,
    BC_TOK_LANGLE,
    BC_TOK_RANGLE,
    BC_TOK_COLON,
    BC_TOK_DOT,
    BC_TOK_COMMA,
    BC_TOK_SEMICOLON,
    BC_TOK_EQ,
    BC_TOK_PLUS,
    BC_TOK_DASH,
    BC_TOK_STAR,
    BC_TOK_SLASH,
    BC_TOK_AMP,
    BC_TOK_PIPE,
    BC_TOK_CARET,
    BC_TOK_COUNT,
};

union bc_tok_val {
    struct bc_strv ident;
    struct bc_strv string;
    struct bc_strv character;
    struct bc_strv integer;
    struct bc_strv floating;
    bool boolean;
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

enum bc_lex_err_kind {
    BC_LEX_ERR_INVALID_UTF8_SEQUENCE,
    BC_LEX_ERR_UNTERMINATED_STRING,
    BC_LEX_ERR_UNTERMINATED_CHARACTER,
    BC_LEX_ERR_UNEXPECTED_CHARACTER_IN_NUMBER,
    BC_LEX_ERR_NO_DIGIT_AFTER_DOT,
    BC_LEX_ERR_UNEXPECTED_CHARACTER,
};

union bc_lex_err_val {
    struct bc_lex_pos unterminated_string;
    struct bc_lex_pos unterminated_character;
    int32_t unexpected_character_in_number;
    int32_t unexpected_character;
};

struct bc_lex_err {
    enum bc_lex_err_kind kind;
    union bc_lex_err_val val;
    struct bc_lex_pos pos;
};

struct bc_lex {
    struct bc_strv src;
    const char* src_ptr_prev;
    struct bc_lex_pos pos;
    struct bc_lex_pos pos_prev;
    int32_t c;
    bool init;
    bool eof;
    struct bc_lex_err err;
};

enum bc_lex_res {
    BC_LEX_ERR = -1,
    BC_LEX_OK,
    BC_LEX_EMPTY,
};

struct bc_lex_loc bc_lex_loc_new(struct bc_lex_pos s, struct bc_lex_pos e);

struct bc_lex bc_lex_new(struct bc_strv src);

enum bc_lex_res bc_lex_next(
    struct bc_lex* lex, struct bc_tok* tok, struct bc_lex_loc* loc);

#endif
