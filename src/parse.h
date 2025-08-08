#ifndef BALDC_PARSE_H
#define BALDC_PARSE_H

#include <stdbool.h>
#include <stddef.h>

#include "lex.h"
#include "ast.h"
#include "mem.h"

enum bc_parse_err_kind {
    BC_PARSE_ERR_LEX,
    BC_PARSE_ERR_UNEXPECTED_TOKEN,
    BC_PARSE_ERR_EXPECTED_MULTIPLE,
};

struct bc_parse_err_expected_multiple {
    const char** items;
    struct bc_tok got;
    size_t len;
};

union bc_parse_err_val {
    struct bc_lex_err lex;
    struct bc_tok unexpected_token;
    struct bc_parse_err_expected_multiple expected_multiple;
};

struct bc_parse_err {
    struct bc_lex_loc loc;
    enum bc_parse_err_kind kind;
    union bc_parse_err_val val;
};

typedef void (*bc_parse_err_callback)(struct bc_parse_err err, void* user_data);

typedef void (*bc_parse_tok_callback)(struct bc_tok tok, void* user_data);

struct bc_parse {
    struct bc_lex lex;
    struct bc_tok tok;
    struct bc_lex_loc last_loc;
    bc_parse_err_callback err_callback;
    void* err_user_data;
    bc_parse_tok_callback tok_callback;
    void* tok_user_data;
    struct bc_mem_arena* mem_arena;
    struct bc_mem_arena* temp_mem_arena;
};

struct bc_parse bc_parse_new(struct bc_lex lex,
    bc_parse_err_callback err_callback, void* err_user_data,
    bc_parse_tok_callback tok_callback, void* tok_user_data,
    struct bc_mem_arena* mem_arena, struct bc_mem_arena* temp_mem_arena);

bool bc_parse_import(struct bc_parse* parse, struct bc_ast_import* import);

bool bc_parse_literal(struct bc_parse* parse, struct bc_ast_literal* lit);

bool bc_parse_expression_list(struct bc_parse* parse,
    enum bc_tok_kind terminator, struct bc_ast_expr_list* list);

bool bc_parse_expression_bp(
    struct bc_parse* parse, uint8_t min_bp, struct bc_ast_expr* expr);

bool bc_parse_expression(struct bc_parse* parse, struct bc_ast_expr* expr);

bool bc_parse_let(struct bc_parse* parse, struct bc_ast_let* let);

bool bc_parse_if(struct bc_parse* parse, struct bc_ast_if* if_);

bool bc_parse_switchcase(
    struct bc_parse* parse, struct bc_ast_switchcase* switchcase);

bool bc_parse_switch(struct bc_parse* parse, struct bc_ast_switch* switch_);

bool bc_parse_loop(struct bc_parse* parse, struct bc_ast_loop* loop);

bool bc_parse_for(struct bc_parse* parse, struct bc_ast_for* for_);

bool bc_parse_while(struct bc_parse* parse, struct bc_ast_while* while_);

bool bc_parse_return(struct bc_parse* parse, struct bc_ast_return* return_);

bool bc_parse_defer(struct bc_parse* parse, struct bc_ast_defer* defer);

bool bc_parse_stmt(struct bc_parse* parse, struct bc_ast_stmt* stmt);

bool bc_parse_block(struct bc_parse* parse, struct bc_ast_block* block);

bool bc_parse_type_path(struct bc_parse* parse, struct bc_ast_type_path* path);

bool bc_parse_type_func(struct bc_parse* parse, struct bc_ast_type_func* func);

bool bc_parse_type(struct bc_parse* parse, struct bc_ast_type* type);

bool bc_parse_func_param(
    struct bc_parse* parse, struct bc_ast_func_param* param);

bool bc_parse_func(struct bc_parse* parse, struct bc_ast_func* func);

bool bc_parse_struct_item(
    struct bc_parse* parse, struct bc_ast_struct_item* item);

bool bc_parse_struct_decl(
    struct bc_parse* parse, struct bc_ast_struct* struct_);

bool bc_parse_enum_decl(struct bc_parse* parse, struct bc_ast_enum* enum_);

bool bc_parse_type_alias(struct bc_parse* parse, struct bc_ast_type* type);

bool bc_parse_const_decl(struct bc_parse* parse, struct bc_ast_const* const_);

bool bc_parse_decl(struct bc_parse* parse, struct bc_ast_decl* top_level);

bool bc_parse_top_level_item(
    struct bc_parse* parse, struct bc_ast_top_level* top_level);

bool bc_parse_top_level_list(
    struct bc_parse* parse, struct bc_ast_top_level_list* list);

bool bc_parse_module(struct bc_parse* parse, struct bc_ast_module* module);

#endif
