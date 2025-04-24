#include "lex.h"

struct bc_lex bc_lex_new(struct bc_strv src) {
    return (struct bc_lex) { .src = src,
        .pos = {
            .l = 1,
            .c = 1,
        } };
}

enum bc_lex_res bc_lex_next(
    struct bc_lex* lex, struct bc_tok* tok, struct bc_lex_loc* loc) {
    return BC_LEX_EMPTY;
}
