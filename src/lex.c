#include "lex.h"

#include "str.h"
#include "utf8.h"
#include "assert.h"
#include "uprop.h"

static struct bc_tok _none(void) {
    return (struct bc_tok) {
        .loc = { 0 },
        .kind = BC_TOK_NONE,
        .val = { { 0 } },
    };
}

static struct bc_lex_loc _tok_loc(struct bc_lex lex) {
    return bc_lex_loc_new(lex.spos, lex.pos_prev);
}

static struct bc_tok _errval(struct bc_lex lex, struct bc_lex_err err) {
    struct bc_tok res = {
        .loc = _tok_loc(lex),
        .kind = BC_TOK_ERR,
        .val = { .err = err },
    };
    return res;
}

static struct bc_tok _errv(
    struct bc_lex lex, enum bc_lex_err_kind kind, union bc_lex_err_val val) {
    return _errval(lex, (struct bc_lex_err) {
                            .kind = kind,
                            .val = val,
                            .pos = lex.pos_prev,
                        });
}

static struct bc_tok _err(struct bc_lex lex, enum bc_lex_err_kind kind) {
    return _errv(lex, kind, (union bc_lex_err_val) { { 0 } });
}

static struct bc_tok _kind(struct bc_lex lex, enum bc_tok_kind kind) {
    return (struct bc_tok) {
        .loc = _tok_loc(lex),
        .kind = kind,
        .val = { { 0 } },
    };
}

static bool _is_bin_digit(int32_t c) { return c == L'0' || c == L'1'; }

static bool _is_oct_digit(int32_t c) { return L'0' <= c && c <= L'8'; }

static bool _is_hex_digit(int32_t c) {
    return bc_uprop_is_digit(c) || (L'a' <= c && c <= L'f') ||
           (L'A' <= c && c <= L'F');
}

static bool _is_sep(int32_t c) {
    switch (c) {
    case L'(':
    case L')':
    case L'{':
    case L'}':
    case L'[':
    case L']':
    case L'<':
    case L'>':
    case L'!':
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
    case L'^':
    case L'~':
    case L'%':
    case L'?':
        return true;
    default:
        return false;
    }
}

static bool _parse_hex_digits(
    const int32_t* digits, size_t len, int32_t* codepoint) {
    uint32_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        int32_t c = digits[i];
        uint32_t value = 0;

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
    if (result > 0x10FFFF || (result >= 0xD800 && result <= 0xDFFF)) {
        return false;
    }
    *codepoint = (int32_t)result;
    return true;
}

static bool _unescape(struct bc_strv input, struct bc_strv* output,
    struct bc_lex_pos begin_pos, struct bc_mem_arena* arena,
    struct bc_lex_err* err) {
#define _ITERTRYNEXT() \
    { \
        enum bc_strv_iter_res res = bc_strv_iter_next(&input_iter, &c); \
        BC_ASSERT(res != BC_STRV_ITER_ERR); \
        if (res != BC_STRV_ITER_OK) { \
            return false; \
        } \
        err->kind = BC_LEX_ERR_INVALID_ESCAPE_SEQUENCE; \
        err->val.invalid_escape_sequence = begin_pos; \
        begin_pos.c++; \
        if (c == '\n') { \
            begin_pos.l++; \
            begin_pos.c = 0; \
        } \
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
        if (c == '\n') {
            begin_pos.l++;
            begin_pos.c = 0;
        }
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
                    for (size_t i = 0; i < len; i++) {
                        _ITERTRYNEXT();
                        digits[i] = c;
                    }
                    break;
                case L'u':
                    len = 4;
                    for (size_t i = 0; i < len; i++) {
                        _ITERTRYNEXT();
                        digits[i] = c;
                    }
                    break;
                case L'U':
                    len = 6;
                    for (size_t i = 0; i < len; i++) {
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
        .escaped_strings_arena = bc_mem_arena_new(1024 * 4),
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
    };
}

void bc_lex_free(struct bc_lex lex) {
    bc_mem_arena_free(lex.escaped_strings_arena);
}

static void _nextc(struct bc_lex* lex) {
    lex->pos_prev = lex->pos;
    lex->src_ptr_prev = lex->src.data;
    if (lex->src.len == 0) {
        lex->eof = true;
        return;
    }
    int32_t codepoint = 0;
    int len = bc_utf8_decode(lex->src.data, lex->src.len, &codepoint);
    BC_ASSERT(len > 0);
    lex->pos.c++;
    if (codepoint == L'\n') {
        lex->pos.c = 1;
        lex->pos.l++;
    }
    lex->c = codepoint;
    lex->src.data += len;
    lex->src.len -= len;
    return;
}

static void _recover(struct bc_lex* lex) {
    while (!bc_uprop_is_space(lex->c) && !_is_sep(lex->c) && !lex->eof) {
        _nextc(lex);
    }
}

static void _recover_to(struct bc_lex* lex, int32_t c) {
    bool escaped = false;
    while (!lex->eof) {
        if (escaped) {
            escaped = false;
        } else if (lex->c == L'\\') {
            escaped = true;
        } else if (lex->c == c) {
            _nextc(lex);
            break;
        }
        _nextc(lex);
    }
}

static struct bc_tok _lex_string(struct bc_lex* lex) {
    if (lex->c == L'"' || lex->c == L'\'') {
        bool is_char = false;
        if (lex->c == L'\'') {
            is_char = true;
        }

        _nextc(lex);
        bool escaped = false;
        bool closed = false;
        while (!lex->eof) {
            if (!bc_uprop_is_print(lex->c)) {
                struct bc_tok err =
                    _err(*lex, BC_LEX_ERR_NON_PRINTABLE_CHARACTER);
                if (is_char) {
                    _recover_to(lex, L'\'');
                } else {
                    _recover_to(lex, L'"');
                }
                return err;
            }
            if (escaped) {
                escaped = false;
            } else {
                if ((lex->c == L'\'' && is_char) ||
                    (lex->c == L'"' && !is_char)) {
                    closed = true;
                    break;
                } else if (lex->c == L'\\') {
                    escaped = true;
                }
            }

            _nextc(lex);
        }
        if (!closed) {
            union bc_lex_err_val val = { .unterminated_character = lex->spos };
            if (is_char) {
                return _errv(*lex, BC_LEX_ERR_UNTERMINATED_CHARACTER, val);
            } else {
                return _errv(*lex, BC_LEX_ERR_UNTERMINATED_CHARACTER, val);
            }
        }
        _nextc(lex);
        struct bc_strv data =
            bc_strv_from_range(lex->tok_begin + 1, lex->src_ptr_prev - 1);
        if (is_char) {
            size_t len = 0;
            bc_strv_len(data, &len);
            if (len == 0) {
                return _err(*lex, BC_LEX_ERR_EMPTY_CHARACTER);
            }
        }
        struct bc_strv escaped_data = { 0 };
        struct bc_lex_err unescape_err;
        if (!_unescape(data, &escaped_data, lex->spos,
                &lex->escaped_strings_arena, &unescape_err)) {
            return _errval(*lex, unescape_err);
        }
        if (is_char) {
            size_t len = 0;
            bc_strv_len(escaped_data, &len);
            if (len > 1) {
                return _err(*lex, BC_LEX_ERR_MULTICHARACTER);
            }
        }
        enum bc_tok_kind kind;
        union bc_tok_val val;
        if (is_char) {
            kind = BC_TOK_LIT_CHARACTER;
            val.literal = escaped_data;
        } else {
            kind = BC_TOK_LIT_STRING;
            val.literal = escaped_data;
        }

        return (struct bc_tok) {
            .loc = _tok_loc(*lex),
            .kind = kind,
            .val = val,
        };
    }
    return _none();
}

static struct bc_tok _lex_num(struct bc_lex* lex, bool negative) {
    if (bc_uprop_is_digit(lex->c)) {
        bool has_dot = false;
        bool has_digit_after_prefix = false;
        bool has_byte_postfix = false;
        int base = 10;

        if (lex->c == L'0') {
            _nextc(lex);
            switch (lex->c) {
            case L'b':
            case L'B':
                base = 2;
                break;
            case L'o':
            case L'O':
                base = 8;
                break;
            case L'x':
            case L'X':
                base = 16;
                break;
            case L'.':
                has_dot = true;
                break;
            default:
                if (bc_uprop_is_space(lex->c) || _is_sep(lex->c)) {
                    struct bc_strv data =
                        bc_strv_from_range(lex->tok_begin, lex->src_ptr_prev);
                    return (struct bc_tok) {
                        .loc = _tok_loc(*lex),
                        .kind = BC_TOK_LIT_INTEGER,
                        .val = { .literal = data },
                    };
                }
                struct bc_tok err =
                    _errv(*lex, BC_LEX_ERR_INVALID_INTEGER_PREFIX,
                        (union bc_lex_err_val) {
                            .invalid_integer_prefix = lex->c });
                _recover(lex);
                return err;
            }
        }

        _nextc(lex);
        while (!lex->eof) {
            if (lex->c == L'.' && base == 10) {
                if (has_dot) {
                    break;
                }
                has_dot = true;
                struct bc_lex saved_lex = *lex;
                _nextc(lex);
                if (!bc_uprop_is_digit(lex->c)) {
                    *lex = saved_lex;
                    has_dot = false;
                    break;
                }
            } else if (base == 2 && _is_bin_digit(lex->c)) {
                has_digit_after_prefix = true;
            } else if (base == 8 && _is_oct_digit(lex->c)) {
                has_digit_after_prefix = true;
            } else if (base == 10 && bc_uprop_is_digit(lex->c)) {
                // OK
            } else if (base == 16 && _is_hex_digit(lex->c)) {
                has_digit_after_prefix = true;
            } else if ((_is_sep(lex->c) || bc_uprop_is_space(lex->c))) {
                if (base != 10 && !has_digit_after_prefix) {
                    struct bc_tok err =
                        _err(*lex, BC_LEX_ERR_NO_DIGIT_AFTER_PREFIX);
                    _recover(lex);
                    return err;
                }
                break;
            } else if (lex->c == L'y' || lex->c == L'Y') {
                if (has_dot) {
                    struct bc_tok err =
                        _err(*lex, BC_LEX_ERR_BYTE_POSTFIX_IN_FLOATING);
                    _recover(lex);
                    return err;
                } else {
                    if (base != 10 && !has_digit_after_prefix) {
                        struct bc_tok err =
                            _err(*lex, BC_LEX_ERR_NO_DIGIT_AFTER_PREFIX);
                        _recover(lex);
                        return err;
                    }
                }
                if (negative) {
                    struct bc_tok err =
                        _err(*lex, BC_LEX_ERR_NEGATIVE_BYTE_LITERAL);
                    _recover(lex);
                    return err;
                }
                has_byte_postfix = true;
                _nextc(lex);
                break;
            } else if (lex->c == L'_') {
                // OK
            } else {
                struct bc_tok err =
                    _errv(*lex, BC_LEX_ERR_UNEXPECTED_CHARACTER_IN_NUMBER,
                        (union bc_lex_err_val) {
                            .unexpected_character_in_number = lex->c });
                _recover(lex);
                return err;
            }
            _nextc(lex);
        }

        struct bc_strv data =
            bc_strv_from_range(lex->tok_begin, lex->src_ptr_prev);

        enum bc_tok_kind kind;
        union bc_tok_val val;
        if (has_dot) {
            kind = BC_TOK_LIT_FLOATING;
            val.literal = data;
        } else if (has_byte_postfix) {
            kind = BC_TOK_LIT_BYTE;
            val.literal = data;
        } else {
            kind = BC_TOK_LIT_INTEGER;
            val.literal = data;
        }
        return (struct bc_tok) {
            .loc = _tok_loc(*lex),
            .kind = kind,
            .val = val,
        };
    }
    return _none();
}

struct bc_tok bc_lex_next(struct bc_lex* lex) {
    if (lex->init) {
        _nextc(lex);
        lex->init = false;
    }
    while (true) {
        if (!lex->eof) {
            if (bc_uprop_is_space(lex->c)) {
                _nextc(lex);
                continue;
            }

            lex->tok_begin = lex->src_ptr_prev;
            lex->spos = lex->pos_prev;

            // String and character
            struct bc_tok string_tok = _lex_string(lex);
            if (string_tok.kind != BC_TOK_NONE) {
                return string_tok;
            }

            // Floating, integer, and byte
            struct bc_tok num_tok = _lex_num(lex, false);
            if (num_tok.kind != BC_TOK_NONE) {
                return num_tok;
            }

            // Ident, keywords, and boolean
            if (bc_uprop_is_alpha(lex->c) || lex->c == L'_') {
                _nextc(lex);
                while (!lex->eof) {
                    if (bc_uprop_is_alnum(lex->c) || lex->c == L'_') {
                        _nextc(lex);
                    } else {
                        break;
                    }
                }

                struct bc_strv data =
                    bc_strv_from_range(lex->tok_begin, lex->src_ptr_prev);

                if (BC_STRV_EQ_LIT(data, "string")) {
                    return _kind(*lex, BC_TOK_KW_STRING);
                } else if (BC_STRV_EQ_LIT(data, "char")) {
                    return _kind(*lex, BC_TOK_KW_CHAR);
                } else if (BC_STRV_EQ_LIT(data, "int")) {
                    return _kind(*lex, BC_TOK_KW_INT);
                } else if (BC_STRV_EQ_LIT(data, "byte")) {
                    return _kind(*lex, BC_TOK_KW_BYTE);
                } else if (BC_STRV_EQ_LIT(data, "float")) {
                    return _kind(*lex, BC_TOK_KW_FLOAT);
                } else if (BC_STRV_EQ_LIT(data, "bool")) {
                    return _kind(*lex, BC_TOK_KW_BOOL);
                } else if (BC_STRV_EQ_LIT(data, "tup")) {
                    return _kind(*lex, BC_TOK_KW_TUP);
                } else if (BC_STRV_EQ_LIT(data, "unit")) {
                    return _kind(*lex, BC_TOK_KW_UNIT);
                } else if (BC_STRV_EQ_LIT(data, "import")) {
                    return _kind(*lex, BC_TOK_KW_IMPORT);
                } else if (BC_STRV_EQ_LIT(data, "export")) {
                    return _kind(*lex, BC_TOK_KW_EXPORT);
                } else if (BC_STRV_EQ_LIT(data, "root")) {
                    return _kind(*lex, BC_TOK_KW_ROOT);
                } else if (BC_STRV_EQ_LIT(data, "super")) {
                    return _kind(*lex, BC_TOK_KW_SUPER);
                } else if (BC_STRV_EQ_LIT(data, "as")) {
                    return _kind(*lex, BC_TOK_KW_AS);
                } else if (BC_STRV_EQ_LIT(data, "try_as")) {
                    return _kind(*lex, BC_TOK_KW_TRYAS);
                } else if (BC_STRV_EQ_LIT(data, "struct")) {
                    return _kind(*lex, BC_TOK_KW_STRUCT);
                } else if (BC_STRV_EQ_LIT(data, "enum")) {
                    return _kind(*lex, BC_TOK_KW_ENUM);
                } else if (BC_STRV_EQ_LIT(data, "func")) {
                    return _kind(*lex, BC_TOK_KW_FUNC);
                } else if (BC_STRV_EQ_LIT(data, "type")) {
                    return _kind(*lex, BC_TOK_KW_TYPE);
                } else if (BC_STRV_EQ_LIT(data, "const")) {
                    return _kind(*lex, BC_TOK_KW_CONST);
                } else if (BC_STRV_EQ_LIT(data, "defer")) {
                    return _kind(*lex, BC_TOK_KW_DEFER);
                } else if (BC_STRV_EQ_LIT(data, "let")) {
                    return _kind(*lex, BC_TOK_KW_LET);
                } else if (BC_STRV_EQ_LIT(data, "if")) {
                    return _kind(*lex, BC_TOK_KW_IF);
                } else if (BC_STRV_EQ_LIT(data, "elif")) {
                    return _kind(*lex, BC_TOK_KW_ELIF);
                } else if (BC_STRV_EQ_LIT(data, "else")) {
                    return _kind(*lex, BC_TOK_KW_ELSE);
                } else if (BC_STRV_EQ_LIT(data, "loop")) {
                    return _kind(*lex, BC_TOK_KW_LOOP);
                } else if (BC_STRV_EQ_LIT(data, "for")) {
                    return _kind(*lex, BC_TOK_KW_FOR);
                } else if (BC_STRV_EQ_LIT(data, "while")) {
                    return _kind(*lex, BC_TOK_KW_WHILE);
                } else if (BC_STRV_EQ_LIT(data, "switch")) {
                    return _kind(*lex, BC_TOK_KW_SWITCH);
                } else if (BC_STRV_EQ_LIT(data, "default")) {
                    return _kind(*lex, BC_TOK_KW_DEFAULT);
                } else if (BC_STRV_EQ_LIT(data, "break")) {
                    return _kind(*lex, BC_TOK_KW_BREAK);
                } else if (BC_STRV_EQ_LIT(data, "continue")) {
                    return _kind(*lex, BC_TOK_KW_CONTINUE);
                } else if (BC_STRV_EQ_LIT(data, "return")) {
                    return _kind(*lex, BC_TOK_KW_RETURN);
                } else if (BC_STRV_EQ_LIT(data, "true")) {
                    return (struct bc_tok) {
                        .loc = _tok_loc(*lex),
                        .kind = BC_TOK_LIT_BOOLEAN,
                        .val = { .literal = data },
                    };
                } else if (BC_STRV_EQ_LIT(data, "false")) {
                    return (struct bc_tok) {
                        .loc = _tok_loc(*lex),
                        .kind = BC_TOK_LIT_BOOLEAN,
                        .val = { .literal = data },
                    };
                } else {
                    return (struct bc_tok) {
                        .loc = _tok_loc(*lex),
                        .kind = BC_TOK_IDENT,
                        .val = { .ident = data },
                    };
                }
            }

            // Special symbol tokens, comments, negative numbers
            enum bc_tok_kind sym_kind = 0;
            if (_is_sep(lex->c)) {
                switch (lex->c) {
                case L'(':
                    sym_kind = BC_TOK_LPAREN;
                    _nextc(lex);
                    break;
                case L')':
                    sym_kind = BC_TOK_RPAREN;
                    _nextc(lex);
                    break;
                case L'{':
                    sym_kind = BC_TOK_LBRACE;
                    _nextc(lex);
                    break;
                case L'}':
                    sym_kind = BC_TOK_RBRACE;
                    _nextc(lex);
                    break;
                case L'[':
                    sym_kind = BC_TOK_LBRACKET;
                    _nextc(lex);
                    break;
                case L']':
                    sym_kind = BC_TOK_RBRACKET;
                    _nextc(lex);
                    break;
                case L'<':
                    sym_kind = BC_TOK_LANGLE;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_LANEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'<' && !lex->eof) {
                        sym_kind = BC_TOK_LANLAN;
                        _nextc(lex);
                        if (lex->c == L'=' && !lex->eof) {
                            sym_kind = BC_TOK_LANLANEQ;
                            _nextc(lex);
                            break;
                        }
                        break;
                    }
                    break;
                case L'>':
                    sym_kind = BC_TOK_RANGLE;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_RANEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'>' && !lex->eof) {
                        sym_kind = BC_TOK_RANRAN;
                        _nextc(lex);
                        if (lex->c == L'=' && !lex->eof) {
                            sym_kind = BC_TOK_RANRANEQ;
                            _nextc(lex);
                            break;
                        }
                        break;
                    }
                    break;
                case L'!':
                    sym_kind = BC_TOK_EXCLAM;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_EXCLEQ;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L':':
                    sym_kind = BC_TOK_COLON;
                    _nextc(lex);
                    if (lex->c == L':' && !lex->eof) {
                        sym_kind = BC_TOK_COLCOL;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'.':
                    sym_kind = BC_TOK_DOT;
                    _nextc(lex);
                    break;
                case L',':
                    sym_kind = BC_TOK_COMMA;
                    _nextc(lex);
                    break;
                case L';':
                    sym_kind = BC_TOK_SEMICOLON;
                    _nextc(lex);
                    break;
                case L'=':
                    sym_kind = BC_TOK_EQ;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_EQEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'>' && !lex->eof) {
                        sym_kind = BC_TOK_EQRAN;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'+':
                    sym_kind = BC_TOK_PLUS;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_PLUSEQ;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'-':
                    sym_kind = BC_TOK_DASH;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_DASHEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'>' && !lex->eof) {
                        sym_kind = BC_TOK_DASHRAN;
                        _nextc(lex);
                        break;
                    }
                    // Negative floating, integer, and byte
                    struct bc_tok num_tok = _lex_num(lex, true);
                    if (num_tok.kind != BC_TOK_NONE) {
                        return num_tok;
                    }
                    break;
                case L'*':
                    sym_kind = BC_TOK_STAR;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_STAREQ;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'/':
                    sym_kind = BC_TOK_SLASH;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_SLASHEQ;
                        _nextc(lex);
                        break;
                    }
                    // Skip comments
                    if (lex->c == L'/') {
                        while (!lex->eof && lex->c != L'\n') {
                            _nextc(lex);
                        }
                        continue;
                    }
                    break;
                case L'&':
                    sym_kind = BC_TOK_AMP;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_AMPEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'&' && !lex->eof) {
                        sym_kind = BC_TOK_AMPAMP;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'|':
                    sym_kind = BC_TOK_PIPE;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_PIPEEQ;
                        _nextc(lex);
                        break;
                    }
                    if (lex->c == L'|' && !lex->eof) {
                        sym_kind = BC_TOK_PIPEPIPE;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'^':
                    sym_kind = BC_TOK_CARET;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_CARETEQ;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'~':
                    sym_kind = BC_TOK_TILDE;
                    _nextc(lex);
                    break;
                case L'%':
                    sym_kind = BC_TOK_PERCENT;
                    _nextc(lex);
                    if (lex->c == L'=' && !lex->eof) {
                        sym_kind = BC_TOK_PERCENTEQ;
                        _nextc(lex);
                        break;
                    }
                    break;
                case L'?':
                    sym_kind = BC_TOK_QUESTION;
                    _nextc(lex);
                    break;
                default:
                    BC_ASSERT_UNREACHABLE();
                }
                return _kind(*lex, sym_kind);
            }

            if (bc_uprop_is_print(lex->c)) {
                return _errv(*lex, BC_LEX_ERR_UNEXPECTED_CHARACTER,
                    (union bc_lex_err_val) { .unexpected_character = lex->c });
            } else {
                return _err(*lex, BC_LEX_ERR_NON_PRINTABLE_CHARACTER);
            }
        } else {
            return _kind(*lex, BC_TOK_EOF);
        }
    }
    return _kind(*lex, BC_TOK_EOF);
}
