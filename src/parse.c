#include "parse.h"

struct bc_parse bc_parse_new(struct bc_lex lex,
    bc_parse_err_callback err_callback, void* err_user_data,
    bc_parse_tok_callback tok_callback, void* tok_user_data) {
    return (struct bc_parse) {
        .init = true,
        .lex = lex,
        .err_callback = err_callback,
        .err_user_data = err_user_data,
        .tok_callback = tok_callback,
        .tok_user_data = tok_user_data,
    };
}

void bc_parse_free(struct bc_parse parse) { bc_lex_free(parse.lex); }

bool bc_parse(struct bc_parse* parse) {
    enum bc_lex_res res = BC_LEX_OK;
    struct bc_tok tok;
    struct bc_lex_loc loc;
    bool error = false;
    while ((res = bc_lex_next(&parse->lex, &tok, &loc)) != BC_LEX_EMPTY) {
        if (res == BC_LEX_ERR) {
            struct bc_parse_err err = {
                .loc = loc,
                .kind = BC_PARSE_ERR_LEX,
                .val = { .lex = parse->lex.err },
            };
            if (parse->err_callback != NULL)
                parse->err_callback(err, parse->err_user_data);
            error = true;
        }
        parse->tok_callback(tok, loc, parse->tok_user_data);
    }

    return !error;
}
