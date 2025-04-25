#include "lex.h"

#include <wctype.h>

#include "str.h"
#include "utf8.h"
#include "assert.h"

struct bc_lex_loc bc_lex_loc_new(struct bc_lex_pos s, struct bc_lex_pos e) {
    return (struct bc_lex_loc) {
        .s = s,
        .e = e,
    };
}

struct bc_lex bc_lex_new(struct bc_strv src) {
    return (struct bc_lex) { .src = src,
        .src_ptr_prev = NULL,
        .pos = {
            .l = 1,
            .c = 1,
        }, .c = 0,
        .pos_prev = {
            .l = 1,
            .c = 1,
        },
        .init = true,
        .eof = false,
        .err = {
            .kind = 0,
            .val = { 0 },
            .pos = {
                .l = 0,
                .c = 0
            },
        }
    };
}

static enum bc_lex_res _nextc(struct bc_lex* lex) {
    if (lex->src.len == 0) {
        lex->eof = true;
        return BC_LEX_EMPTY;
    }
    int32_t codepoint = 0;
    int len = bc_utf8_decode(lex->src.data, lex->src.len, &codepoint);
    if (len == -1) {
        return BC_LEX_ERR;
    }
    lex->pos_prev = lex->pos;
    lex->pos.c++;
    if (codepoint == L'\n') {
        lex->pos.c = 1;
        lex->pos.l++;
    }
    lex->c = codepoint;
    lex->src_ptr_prev = lex->src.data;
    lex->src.data += len;
    lex->src.len -= len;
    return BC_LEX_OK;
}

enum bc_lex_res bc_lex_next(
    struct bc_lex* lex, struct bc_tok* tok, struct bc_lex_loc* loc) {
#define _NEXTC() \
    { \
        if (_nextc(lex) == BC_LEX_ERR) { \
            lex->err.kind = BC_LEX_ERR_INVALID_UTF8_SEQUENCE; \
            lex->err.pos = lex->pos_prev; \
            return BC_LEX_ERR; \
        } \
    };

    if (lex->init) {
        _nextc(lex);
        lex->init = false;
    }
    while (true) {
        if (!lex->eof) {
            if (iswspace(lex->c)) {
                _NEXTC();
                continue;
            }

            const char* tok_begin = lex->src_ptr_prev;
            struct bc_lex_pos spos = lex->pos_prev;

            // String
            if (lex->c == L'"') {
                _NEXTC();
                bool escaped = false;
                bool closed = false;
                while (true) {
                    if (lex->eof) {
                        break;
                    }
                    if (escaped) {
                        escaped = false;
                    } else {
                        if (lex->c == L'"') {
                            closed = true;
                            break;
                        } else if (lex->c == L'\\') {
                            escaped = true;
                        }
                    }
                    _NEXTC();
                }
                if (!closed) {
                    lex->err.kind = BC_LEX_ERR_UNTERMINATED_STRING;
                    lex->err.val.unterminated_string = spos;
                    lex->err.pos = lex->pos_prev;
                    return BC_LEX_ERR;
                }
                _NEXTC();
                struct bc_strv data =
                    bc_strv_from_range(tok_begin + 1, lex->src_ptr_prev - 1);

                tok->kind = BC_TOK_LIT_STRING;
                tok->val.string = data;
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }

            // Character
            if (lex->c == L'\'') {
                _NEXTC();
                bool escaped = false;
                bool closed = false;
                while (true) {
                    if (lex->eof) {
                        break;
                    }
                    if (escaped) {
                        escaped = false;
                    } else {
                        if (lex->c == L'\'') {
                            closed = true;
                            break;
                        } else if (lex->c == L'\\') {
                            escaped = true;
                        }
                    }
                    _NEXTC();
                }
                if (!closed) {
                    lex->err.kind = BC_LEX_ERR_UNTERMINATED_CHARACTER;
                    lex->err.val.unterminated_character = spos;
                    lex->err.pos = lex->pos_prev;
                    return BC_LEX_ERR;
                }
                _NEXTC();
                struct bc_strv data =
                    bc_strv_from_range(tok_begin + 1, lex->src_ptr_prev - 1);

                tok->kind = BC_TOK_LIT_CHARACTER;
                tok->val.character = data;
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }

            // Floating and integer
            if (iswdigit(lex->c)) {
                _NEXTC();
                bool has_dot = false;
                bool has_digit_after_dot = false;
                while (!lex->eof) {
                    if (lex->c == L'.') {
                        if (has_dot) {
                            break;
                        }
                        has_dot = true;
                    } else if (iswdigit(lex->c)) {
                        if (has_dot) {
                            has_digit_after_dot = true;
                        }
                    } else if (iswalpha(lex->c)) {
                        lex->err.kind =
                            BC_LEX_ERR_UNEXPECTED_CHARACTER_IN_NUMBER;
                        lex->err.val.unexpected_character_in_number = lex->c;
                        lex->err.pos = lex->pos_prev;
                        return BC_LEX_ERR;
                    } else if (lex->c != L'_') {
                        if (has_dot && !has_digit_after_dot) {
                            lex->err.kind = BC_LEX_ERR_NO_DIGIT_AFTER_DOT;
                            lex->err.pos = lex->pos_prev;
                            return BC_LEX_ERR;
                        }
                        break;
                    }
                    _NEXTC();
                }

                struct bc_strv data =
                    bc_strv_from_range(tok_begin, lex->src_ptr_prev);

                if (has_dot) {
                    tok->kind = BC_TOK_LIT_FLOATING;
                    tok->val.floating = data;
                } else {
                    tok->kind = BC_TOK_LIT_INTEGER;
                    tok->val.integer = data;
                }
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }

            // Ident, keywords, and boolean
            if (iswalpha(lex->c) || lex->c == L'_') {
                _NEXTC();
                while (!lex->eof) {
                    if (iswalnum(lex->c) || lex->c == L'_') {
                        _NEXTC();
                    } else {
                        break;
                    }
                }

                struct bc_strv data =
                    bc_strv_from_range(tok_begin, lex->src_ptr_prev);

                if (BC_STRV_EQ_LIT(data, "import")) {
                    tok->kind = BC_TOK_KW_IMPORT;
                } else if (BC_STRV_EQ_LIT(data, "struct")) {
                    tok->kind = BC_TOK_KW_STRUCT;
                } else if (BC_STRV_EQ_LIT(data, "enum")) {
                    tok->kind = BC_TOK_KW_ENUM;
                } else if (BC_STRV_EQ_LIT(data, "func")) {
                    tok->kind = BC_TOK_KW_FUNC;
                } else if (BC_STRV_EQ_LIT(data, "let")) {
                    tok->kind = BC_TOK_KW_LET;
                } else if (BC_STRV_EQ_LIT(data, "true")) {
                    tok->kind = BC_TOK_LIT_BOOLEAN;
                    tok->val.boolean = true;
                } else if (BC_STRV_EQ_LIT(data, "false")) {
                    tok->kind = BC_TOK_LIT_BOOLEAN;
                    tok->val.boolean = false;
                } else {
                    tok->kind = BC_TOK_IDENT;
                    tok->val.ident = data;
                }
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }

            // Single character tokens
            switch (lex->c) {
            case L'(':
            case L')':
            case L'{':
            case L'}':
            case L'[':
            case L']':
            case L'<':
            case L'>':
            case L':':
            case L'.':
            case L',':
            case L';':
            case L'=':
            case L'+':
            case L'-':
            case L'*':
            case L'/':
            case L'&':
            case L'|':
            case L'^': {
                enum bc_tok_kind kind = 0;
                switch (lex->c) {
                case L'(':
                    kind = BC_TOK_LPAREN;
                    break;
                case L')':
                    kind = BC_TOK_RPAREN;
                    break;
                case L'{':
                    kind = BC_TOK_LBRACE;
                    break;
                case L'}':
                    kind = BC_TOK_RBRACE;
                    break;
                case L'[':
                    kind = BC_TOK_LBRACKET;
                    break;
                case L']':
                    kind = BC_TOK_RBRACKET;
                    break;
                case L'<':
                    kind = BC_TOK_LANGLE;
                    break;
                case L'>':
                    kind = BC_TOK_RANGLE;
                    break;
                case L':':
                    kind = BC_TOK_COLON;
                    break;
                case L'.':
                    kind = BC_TOK_DOT;
                    break;
                case L',':
                    kind = BC_TOK_COMMA;
                    break;
                case L';':
                    kind = BC_TOK_SEMICOLON;
                    break;
                case L'=':
                    kind = BC_TOK_EQ;
                    break;
                case L'+':
                    kind = BC_TOK_PLUS;
                    break;
                case L'-':
                    kind = BC_TOK_DASH;
                    break;
                case L'*':
                    kind = BC_TOK_STAR;
                    break;
                case L'/':
                    kind = BC_TOK_SLASH;
                    break;
                case L'&':
                    kind = BC_TOK_AMP;
                    break;
                case L'|':
                    kind = BC_TOK_PIPE;
                    break;
                case L'^':
                    kind = BC_TOK_CARET;
                    break;
                default:
                    BC_ASSERT_UNREACHABLE();
                }
                _NEXTC();

                tok->kind = kind;
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }
            }

            lex->err.kind = BC_LEX_ERR_UNEXPECTED_CHARACTER;
            lex->err.val.unexpected_character = lex->c;
            lex->err.pos = lex->pos_prev;
            return BC_LEX_ERR;
        } else {
            return BC_LEX_EMPTY;
        }
    }
    return BC_LEX_EMPTY;
}
