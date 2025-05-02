#include "parse.h"
#include "lex.h"

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
    parse->tok = bc_lex_next(&parse->lex);
    parse->tok_callback(parse->tok, parse->tok_user_data);
    while (parse->tok.kind != BC_TOK_EOF) {
        parse->tok = bc_lex_next(&parse->lex);
        parse->tok_callback(parse->tok, parse->tok_user_data);
    }
    return true;
}
