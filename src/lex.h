#ifndef BALDC_LEX_H
#define BALDC_LEX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "str.h"
#include "mem.h"

enum bc_tok_kind {
    // Special
    BC_TOK_ERR = -3,
    BC_TOK_EOF = -2,
    BC_TOK_NONE = -1,
    // Identifier
    BC_TOK_IDENT,
    // Literals
    BC_TOK_LIT_STRING,
    BC_TOK_LIT_CHARACTER,
    BC_TOK_LIT_INTEGER,
    BC_TOK_LIT_BYTE,
    BC_TOK_LIT_FLOATING,
    BC_TOK_LIT_BOOLEAN,
    // Types
    BC_TOK_KW_STRING,
    BC_TOK_KW_CHAR,
    BC_TOK_KW_INT,
    BC_TOK_KW_BYTE,
    BC_TOK_KW_FLOAT,
    BC_TOK_KW_BOOL,
    BC_TOK_KW_TUP,
    BC_TOK_KW_UNIT,
    // Visibility and importing
    BC_TOK_KW_IMPORT,
    BC_TOK_KW_EXPORT,
    BC_TOK_KW_ROOT,
    BC_TOK_KW_SUPER,
    // Casting
    BC_TOK_KW_AS,
    BC_TOK_KW_TRYAS,
    // Declarations
    BC_TOK_KW_STRUCT,
    BC_TOK_KW_ENUM,
    BC_TOK_KW_FUNC,
    BC_TOK_KW_TYPE,
    BC_TOK_KW_CONST,
    // Statements
    BC_TOK_KW_DEFER,
    BC_TOK_KW_LET,
    BC_TOK_KW_IF,
    BC_TOK_KW_ELIF,
    BC_TOK_KW_ELSE,
    BC_TOK_KW_LOOP,
    BC_TOK_KW_FOR,
    BC_TOK_KW_WHILE,
    BC_TOK_KW_SWITCH,
    BC_TOK_KW_DEFAULT,
    BC_TOK_KW_BREAK,
    BC_TOK_KW_CONTINUE,
    BC_TOK_KW_RETURN,
    // Symbols
    BC_TOK_LPAREN, // (
    BC_TOK_RPAREN, // )
    BC_TOK_LBRACE, // {
    BC_TOK_RBRACE, // }
    BC_TOK_LBRACKET, // [
    BC_TOK_RBRACKET, // ]
    BC_TOK_LANGLE, // <
    BC_TOK_LANEQ, // <=
    BC_TOK_LANLAN, // <<
    BC_TOK_LANLANEQ, // <<=
    BC_TOK_RANGLE, // >
    BC_TOK_RANEQ, // >=
    BC_TOK_RANRAN, // >>
    BC_TOK_RANRANEQ, // >>=
    BC_TOK_EXCLAM, // !
    BC_TOK_EXCLEQ, // !=
    BC_TOK_COLON, // :
    BC_TOK_COLCOL, // ::
    BC_TOK_DOT, // .
    BC_TOK_COMMA, // ,
    BC_TOK_SEMICOLON, // ;
    BC_TOK_EQ, // =
    BC_TOK_EQEQ, // ==
    BC_TOK_EQRAN, // =>
    BC_TOK_PLUS, // +
    BC_TOK_PLUSEQ, // +=
    BC_TOK_DASH, // -
    BC_TOK_DASHEQ, // -=
    BC_TOK_DASHRAN, // ->
    BC_TOK_STAR, // *
    BC_TOK_STAREQ, // *=
    BC_TOK_SLASH, // /
    BC_TOK_SLASHEQ, // /=
    BC_TOK_AMP, // &
    BC_TOK_AMPEQ, // &=
    BC_TOK_AMPAMP, // &&
    BC_TOK_PIPE, // |
    BC_TOK_PIPEEQ, // |=
    BC_TOK_PIPEPIPE, // ||
    BC_TOK_CARET, // ^
    BC_TOK_CARETEQ, // ^=
    BC_TOK_TILDE, // ~
    BC_TOK_PERCENT, // %
    BC_TOK_PERCENTEQ, // %=
    BC_TOK_QUESTION, // ?
    // Token count
    BC_TOK_COUNT,
};

enum bc_lex_err_kind {
    BC_LEX_ERR_INVALID_UTF8_SEQUENCE,
    BC_LEX_ERR_UNTERMINATED_STRING,
    BC_LEX_ERR_UNTERMINATED_CHARACTER,
    BC_LEX_ERR_UNEXPECTED_CHARACTER_IN_NUMBER,
    BC_LEX_ERR_UNEXPECTED_CHARACTER,
    BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE,
    BC_LEX_ERR_MULTICHARACTER,
    BC_LEX_ERR_EMPTY_CHARACTER,
    BC_LEX_ERR_NON_PRINTABLE_CHARACTER,
    BC_LEX_ERR_INVALID_INTEGER_PREFIX,
    BC_LEX_ERR_NO_DIGIT_AFTER_PREFIX,
    BC_LEX_ERR_BYTE_POSTFIX_IN_FLOATING,
    BC_LEX_ERR_NEGATIVE_BYTE_LITERAL,
};

struct bc_lex_pos {
    size_t l;
    size_t c;
};

struct bc_lex_loc {
    struct bc_lex_pos s;
    struct bc_lex_pos e;
};

union bc_lex_err_val {
    struct bc_lex_pos unterminated_string;
    struct bc_lex_pos unterminated_character;
    int32_t unexpected_character_in_number;
    int32_t unexpected_character;
    struct bc_lex_pos invalid_escape_sequence;
    int32_t invalid_integer_prefix;
};

struct bc_lex_err {
    enum bc_lex_err_kind kind;
    union bc_lex_err_val val;
    struct bc_lex_pos pos;
};

union bc_tok_val {
    struct bc_lex_err err;
    struct bc_strv ident;
    struct bc_strv literal;
};

struct bc_tok {
    struct bc_lex_loc loc;
    enum bc_tok_kind kind;
    union bc_tok_val val;
};

struct bc_lex {
    struct bc_mem_arena escaped_strings_arena;
    struct bc_strv src;
    const char* src_ptr_prev;
    struct bc_lex_pos pos;
    struct bc_lex_pos pos_prev;
    const char* tok_begin;
    struct bc_lex_pos spos;
    int32_t c;
    bool init;
    bool eof;
};

struct bc_lex_loc bc_lex_loc_new(struct bc_lex_pos s, struct bc_lex_pos e);

struct bc_lex bc_lex_new(struct bc_strv src);

void bc_lex_free(struct bc_lex lex);

struct bc_tok bc_lex_next(struct bc_lex* lex);

#endif
