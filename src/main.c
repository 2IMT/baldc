#include <stdio.h>
#include <locale.h>

#include "lex.h"
#include "str.h"

int main(void) {
    setlocale(LC_ALL, "");

    struct bc_lex lex = bc_lex_new(BC_STRV_FROM_LIT("hello world"));

    struct bc_tok tok;
    struct bc_lex_loc loc;
    while (true) {
        enum bc_lex_res res = bc_lex_next(&lex, &tok, &loc);
        if (res == BC_LEX_EMPTY) {
            break;
        }
        if (res == BC_LEX_ERR) {
            fprintf(stderr, "error\n");
            return 1;
        }
    }

    return 0;
}
