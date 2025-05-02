#ifndef BALDC_PARSE_H
#define BALDC_PARSE_H

#include <stdbool.h>

#include "lex.h"

enum bc_parse_err_kind {
    BC_PARSE_ERR_LEX,
};

union bc_parse_err_val {
    struct bc_lex_err lex;
};

struct bc_parse_err {
    struct bc_lex_loc loc;
    enum bc_parse_err_kind kind;
    union bc_parse_err_val val;
};

typedef void (*bc_parse_err_callback)(struct bc_parse_err err, void* user_data);

typedef void (*bc_parse_tok_callback)(struct bc_tok tok, void* user_data);

struct bc_parse {
    bool init;
    struct bc_lex lex;
    struct bc_tok tok;
    bc_parse_err_callback err_callback;
    void* err_user_data;
    bc_parse_tok_callback tok_callback;
    void* tok_user_data;
};

struct bc_parse bc_parse_new(struct bc_lex lex,
    bc_parse_err_callback err_callback, void* err_user_data,
    bc_parse_tok_callback tok_callback, void* tok_user_data);

void bc_parse_free(struct bc_parse parse);

bool bc_parse(struct bc_parse* parse);

#endif
