#include "lex.h"

#include <wctype.h>

#include "str.h"
#include "utf8.h"
#include "assert.h"

static bool _parse_hex_digits(const int32_t* digits, size_t len, int32_t* codepoint) {
    int32_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        int32_t c = digits[i];
        int value = 0;

        if (c >= L'0' && c <= L'9') {
            value = c - L'0';
        } else if (c >= L'a' && c <= L'f') {
            value = 10 + (c - L'a');
        } else if (c >= L'A' && c <= L'F') {
            value = 10 + (c - L'A');
        } else {
            return false;
        }

        result = (result << 4) | value;
    }
    *codepoint = result;
    return true;
}

static bool _unescape(struct bc_strv input, struct bc_strv* output,
    struct bc_lex_pos begin_pos, struct bc_mem_arena* arena,
    struct bc_lex_err* err) {
#define _ITERTRYNEXT() \
    { \
        enum bc_strv_iter_res res = bc_strv_iter_next(&input_iter, &c); \
        if (res != BC_STRV_ITER_OK) { \
            return false; \
        } \
        err->kind = BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE; \
        err->val.invalid_escape_sequence = begin_pos; \
        begin_pos.c++; \
    }

    struct bc_str unescaped = {
        .data = bc_mem_arena_alloc(arena, input.len),
        .len = 0,
        .cap = input.len,
    };

    struct bc_strv_iter input_iter = bc_strv_iter_new(input);
    enum bc_strv_iter_res res = BC_STRV_ITER_OK;
    int32_t c = 0;
    while ((res = bc_strv_iter_next(&input_iter, &c)) == BC_STRV_ITER_OK) {
        begin_pos.c++;
        if (c == L'\\') {
            _ITERTRYNEXT();

            switch (c) {
            case L'\\': {
                bc_str_push_cch_unchecked(&unescaped, '\\');
            } break;
            case L'"': {
                bc_str_push_cch_unchecked(&unescaped, '"');
            } break;
            case L'\'': {
                bc_str_push_cch_unchecked(&unescaped, '\'');
            } break;
            case L'r': {
                bc_str_push_cch_unchecked(&unescaped, '\r');
            } break;
            case L'n': {
                bc_str_push_cch_unchecked(&unescaped, '\n');
            } break;
            case L't': {
                bc_str_push_cch_unchecked(&unescaped, '\t');
            } break;
            case L'b': {
                bc_str_push_cch_unchecked(&unescaped, '\b');
            } break;
            case L'v': {
                bc_str_push_cch_unchecked(&unescaped, '\v');
            } break;
            case L'f': {
                bc_str_push_cch_unchecked(&unescaped, '\f');
            } break;
            case L'0': {
                bc_str_push_cch_unchecked(&unescaped, '\0');
            } break;
            case L'x':
            case L'u':
            case L'U': {
                int32_t digits[8] = { 0 };
                size_t len = 0;
                switch (c) {
                case L'x':
                    len = 2;
                    for (size_t i = 0; i < 2; i++) {
                        _ITERTRYNEXT();
                        digits[i] = c;
                    }
                    break;
                case L'u':
                    len = 4;
                    for (size_t i = 0; i < 4; i++) {
                        _ITERTRYNEXT();
                        digits[i] = c;
                    }
                    break;
                case L'U':
                    len = 8;
                    for (size_t i = 0; i < 8; i++) {
                        _ITERTRYNEXT();
                        digits[i] = c;
                    }
                    break;
                default:
                    BC_ASSERT_UNREACHABLE();
                }

                int32_t codepoint;
                if (!_parse_hex_digits(digits, len, &codepoint)) {
                    err->kind = BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE;
                    err->val.invalid_escape_sequence = begin_pos;
                    return false;
                }

                bc_str_push_ch_unchecked(&unescaped, codepoint);
            } break;
            default: {
                err->kind = BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE;
                err->val.invalid_escape_sequence = begin_pos;
                return false;
            }
            }
        } else {
            bc_str_push_ch_unchecked(&unescaped, c);
        }
    }

    BC_ASSERT(res != BC_STRV_ITER_ERR);

    *output = bc_strv_from_str(unescaped);
    return true;
}

struct bc_lex_loc bc_lex_loc_new(struct bc_lex_pos s, struct bc_lex_pos e) {
    return (struct bc_lex_loc) {
        .s = s,
        .e = e,
    };
}

struct bc_lex bc_lex_new(struct bc_strv src) {
    return (struct bc_lex) {
        .escaped_strings_arena = bc_mem_arena_new(1024 * 8),
        .src = src,
        .src_ptr_prev = NULL,
        .pos = {
            .l = 1,
            .c = 1,
        },
        .c = 0,
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

void bc_lex_free(struct bc_lex lex) {
    bc_mem_arena_free(lex.escaped_strings_arena);
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
                    if (!iswprint(lex->c)) {
                        lex->err.kind = BC_LEX_ERR_NON_PRINTABLE_CHARACTER;
                        lex->err.pos = lex->pos_prev;
                        return BC_LEX_ERR;
                    }
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
                struct bc_strv escaped_data = { 0 };
                if (!_unescape(data, &escaped_data, spos,
                        &lex->escaped_strings_arena, &lex->err)) {
                    lex->err.pos = spos;
                    return BC_LEX_ERR;
                }

                tok->kind = BC_TOK_LIT_STRING;
                tok->val.string = escaped_data;
                *loc = bc_lex_loc_new(spos, lex->pos_prev);

                return BC_LEX_OK;
            }

            // Character
            if (lex->c == L'\'') {
                _NEXTC();
                bool escaped = false;
                bool closed = false;
                while (true) {
                    if (!iswprint(lex->c)) {
                        lex->err.kind = BC_LEX_ERR_NON_PRINTABLE_CHARACTER;
                        lex->err.pos = lex->pos_prev;
                        return BC_LEX_ERR;
                    }
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
                if (data.len == 0) {
                    lex->err.kind = BC_LEX_ERR_EMPTY_CHARACTER;
                    lex->err.pos = spos;
                    return BC_LEX_ERR;
                }
                struct bc_strv escaped_data = { 0 };
                if (!_unescape(data, &escaped_data, spos,
                        &lex->escaped_strings_arena, &lex->err)) {
                    lex->err.pos = spos;
                    return BC_LEX_ERR;
                }
                if (escaped_data.len > 1) {
                    lex->err.kind = BC_LEX_ERR_MULTICHARACTER;
                    lex->err.pos = spos;
                    return BC_LEX_ERR;
                }

                tok->kind = BC_TOK_LIT_CHARACTER;
                tok->val.character = escaped_data;
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

            if (iswprint(lex->c)) {
                lex->err.kind = BC_LEX_ERR_UNEXPECTED_CHARACTER;
                lex->err.val.unexpected_character = lex->c;
            } else {
                lex->err.kind = BC_LEX_ERR_NON_PRINTABLE_CHARACTER;
            }
            lex->err.pos = lex->pos_prev;
            return BC_LEX_ERR;
        } else {
            return BC_LEX_EMPTY;
        }
    }
    return BC_LEX_EMPTY;
}
