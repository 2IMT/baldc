#ifndef BALDC_AST_H
#define BALDC_AST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "str.h"
#include "lex.h"
#include "mem.h"

#define BC_AST_DECL_LIST(list_typename, item_type) \
    struct list_typename { \
        item_type item; \
        struct list_typename* next; \
    }

#define BC_AST_DECL_ARR(arr_typename, item_type) \
    struct arr_typename { \
        item_type* items; \
        size_t len; \
    }

#define BC_AST_LIST_FLATTEN(list_type, item_type, list, arr, arena_ptr) \
    do { \
        size_t len = 0; \
        list_type* curr = (list); \
        while (curr != NULL) { \
            len++; \
            curr = curr->next; \
        } \
        curr = (list); \
        item_type* items = NULL; \
        if (len > 0) { \
            items = BC_MEM_ARENA_CALLOC_TYPE((arena_ptr), item_type, len); \
            for (size_t i = 0; i < len; i++) { \
                items[i] = curr->item; \
                curr = curr->next; \
            } \
        } \
        (arr).len = len; \
        (arr).items = items; \
    } while (false)

enum bc_ast_stmt_kind {
    BC_AST_STMT_EXPR,
    BC_AST_STMT_LET,
    BC_AST_STMT_IF,
    BC_AST_STMT_SWITCH,
    BC_AST_STMT_LOOP,
    BC_AST_STMT_FOR,
    BC_AST_STMT_WHILE,
    BC_AST_STMT_CONTINUE,
    BC_AST_STMT_BREAK,
    BC_AST_STMT_RETURN,
    BC_AST_STMT_DEFER,
    BC_AST_STMT_BLOCK,
};

union bc_ast_stmt_val {
    struct bc_ast_expr* expr;
    struct bc_ast_let* let;
    struct bc_ast_if* if_;
    struct bc_ast_switch* switch_;
    struct bc_ast_loop* loop;
    struct bc_ast_for* for_;
    struct bc_ast_while* while_;
    struct bc_ast_return* return_;
    struct bc_ast_defer* defer;
    struct bc_ast_block* block;
};

struct bc_ast_stmt {
    struct bc_lex_loc loc;
    enum bc_ast_stmt_kind kind;
    union bc_ast_stmt_val val;
};

BC_AST_DECL_LIST(bc_ast_stmt_list, struct bc_ast_stmt);

BC_AST_DECL_ARR(bc_ast_stmt_arr, struct bc_ast_stmt);

struct bc_ast_block {
    struct bc_lex_loc loc;
    struct bc_ast_stmt_arr stmts;
};

BC_AST_DECL_LIST(bc_ast_ident_list, struct bc_strv);

BC_AST_DECL_ARR(bc_ast_ident_arr, struct bc_strv);

struct bc_ast_import {
    struct bc_lex_loc loc;
    struct bc_strv renamed_to;
    struct bc_ast_ident_arr segments;
    uint32_t super_count;
    bool is_root;
    bool is_renamed;
    bool is_glob;
};

enum bc_ast_type_kind {
    BC_AST_TYPE_UNIT,
    BC_AST_TYPE_STRING,
    BC_AST_TYPE_CHAR,
    BC_AST_TYPE_INT,
    BC_AST_TYPE_BYTE,
    BC_AST_TYPE_FLOAT,
    BC_AST_TYPE_BOOL,
    BC_AST_TYPE_ARRAY,
    BC_AST_TYPE_TUP,
    BC_AST_TYPE_FUNC,
    BC_AST_TYPE_PATH,
};

BC_AST_DECL_ARR(bc_ast_type_arr, struct bc_ast_type);

union bc_ast_type_val {
    struct bc_ast_type* arr;
    struct bc_ast_type_arr tup;
    struct bc_ast_type_func* func;
    struct bc_ast_type_path* path;
};

struct bc_ast_type_path {
    struct bc_lex_loc loc;
    struct bc_ast_ident_arr segments;
    uint32_t super_count;
    bool is_root;
};

struct bc_ast_type {
    struct bc_lex_loc loc;
    enum bc_ast_type_kind kind;
    union bc_ast_type_val val;
};

struct bc_ast_type_func {
    struct bc_lex_loc loc;
    struct bc_ast_type_arr params;
    struct bc_ast_type ret;
};

BC_AST_DECL_LIST(bc_ast_type_list, struct bc_ast_type);

struct bc_ast_func_param {
    struct bc_lex_loc loc;
    struct bc_strv name;
    struct bc_ast_type type;
};

BC_AST_DECL_LIST(bc_ast_func_param_list, struct bc_ast_func_param);

BC_AST_DECL_ARR(bc_ast_func_param_arr, struct bc_ast_func_param);

struct bc_ast_func {
    struct bc_lex_loc loc;
    struct bc_ast_func_param_arr params;
    struct bc_ast_type ret;
    struct bc_ast_block block;
};

enum bc_ast_literal_array_kind {
    BC_AST_LITERAL_ARRAY_REGULAR,
    BC_AST_LITERAL_ARRAY_DEFAULT,
};

BC_AST_DECL_ARR(bc_ast_expr_arr, struct bc_ast_expr);

union bc_ast_literal_array_val {
    struct bc_ast_expr_arr regular;
    struct bc_ast_expr* default_;
};

struct bc_ast_literal_array {
    struct bc_lex_loc loc;
    enum bc_ast_literal_array_kind kind;
    union bc_ast_literal_array_val val;
};

enum bc_ast_literal_kind {
    // Keyword literals
    BC_AST_LITERAL_UNIT,
    BC_AST_LITERAL_NULL,
    // Primitive literals
    BC_AST_LITERAL_STRING,
    BC_AST_LITERAL_CHARACTER,
    BC_AST_LITERAL_INTEGER,
    BC_AST_LITERAL_BYTE,
    BC_AST_LITERAL_FLOATING,
    BC_AST_LITERAL_BOOLEAN,
    // Non-primitive literals
    BC_AST_LITERAL_ARRAY,
    BC_AST_LITERAL_TUP,
    BC_AST_LITERAL_FUNC,
};

union bc_ast_literal_val {
    struct bc_strv primitive;
    struct bc_ast_literal_array array;
    struct bc_ast_expr_arr tuple;
    struct bc_ast_func func;
};

struct bc_ast_literal {
    struct bc_lex_loc loc;
    enum bc_ast_literal_kind kind;
    union bc_ast_literal_val val;
};

enum bc_ast_expr_kind {
    BC_AST_EXPR_LITERAL,
    BC_AST_EXPR_UNOP,
    BC_AST_EXPR_BINOP,
    BC_AST_EXPR_INDEX,
    BC_AST_EXPR_CALL,
    BC_AST_EXPR_CAST,
    BC_AST_EXPR_TRYCAST,
    BC_AST_EXPR_PATH_SEGMENT,
};

struct bc_ast_expr_unop {
    struct bc_lex_loc loc;
    enum bc_tok_kind op;
    struct bc_ast_expr* operand;
};

struct bc_ast_expr_binop {
    struct bc_lex_loc loc;
    enum bc_tok_kind op;
    struct bc_ast_expr* l;
    struct bc_ast_expr* r;
};

struct bc_ast_expr_index {
    struct bc_lex_loc loc;
    struct bc_ast_expr* expr;
    struct bc_ast_expr* index;
};

struct bc_ast_expr_call {
    struct bc_lex_loc loc;
    struct bc_ast_expr* expr;
    struct bc_ast_expr_arr args;
};

enum bc_ast_expr_access_kind {
    BC_AST_EXPR_ACCESS_ROOT,
    BC_AST_EXPR_ACCESS_SUPER,
    BC_AST_EXPR_ACCESS_NAME,
};

struct bc_ast_expr_cast {
    struct bc_lex_loc loc;
    struct bc_ast_expr* expr;
    struct bc_ast_type type;
};

enum bc_ast_expr_path_segment_kind {
    BC_AST_EXPR_PATH_SEGMENT_ROOT,
    BC_AST_EXPR_PATH_SEGMENT_SUPER,
    BC_AST_EXPR_PATH_SEGMENT_NAME,
};

struct bc_ast_expr_path_segment {
    struct bc_lex_loc loc;
    enum bc_ast_expr_path_segment_kind kind;
    struct bc_strv name;
};

union bc_ast_expr_val {
    struct bc_ast_literal literal;
    struct bc_ast_expr_unop unop;
    struct bc_ast_expr_binop binop;
    struct bc_ast_expr_index index;
    struct bc_ast_expr_call call;
    struct bc_ast_expr_cast cast;
    struct bc_ast_expr_cast try_cast;
    struct bc_ast_expr_path_segment path_segment;
};

struct bc_ast_expr {
    struct bc_lex_loc loc;
    enum bc_ast_expr_kind kind;
    union bc_ast_expr_val val;
};

BC_AST_DECL_LIST(bc_ast_expr_list, struct bc_ast_expr);

struct bc_ast_let {
    struct bc_lex_loc loc;
    struct bc_strv name;
    struct bc_ast_type type;
    struct bc_ast_expr expr;
    bool has_type;
    bool has_expr;
};

struct bc_ast_elif {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

BC_AST_DECL_LIST(bc_ast_elif_list, struct bc_ast_elif);

BC_AST_DECL_ARR(bc_ast_elif_arr, struct bc_ast_elif);

struct bc_ast_if {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    struct bc_ast_block main;
    struct bc_ast_elif_arr elifs;
    struct bc_ast_block else_;
    bool has_else;
};

struct bc_ast_switchcase {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    struct bc_ast_block block;
    bool is_default;
};

BC_AST_DECL_LIST(bc_ast_switchcase_list, struct bc_ast_switchcase);

BC_AST_DECL_ARR(bc_ast_switchcase_arr, struct bc_ast_switchcase);

struct bc_ast_switch {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    struct bc_ast_switchcase_arr cases;
};

struct bc_ast_loop {
    struct bc_lex_loc loc;
    struct bc_ast_block block;
};

struct bc_ast_for {
    struct bc_lex_loc loc;
    struct bc_strv name;
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

struct bc_ast_while {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

struct bc_ast_return {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
    bool is_empty;
};

struct bc_ast_defer {
    struct bc_lex_loc loc;
    struct bc_ast_expr expr;
};

enum bc_ast_decl_kind {
    BC_AST_DECL_FUNC,
    BC_AST_DECL_STRUCT,
    BC_AST_DECL_ENUM,
    BC_AST_DECL_TYPE_ALIAS,
    BC_AST_DECL_CONST,
};

struct bc_ast_struct_item {
    struct bc_lex_loc loc;
    struct bc_strv name;
    struct bc_ast_type type;
    bool is_export;
};

BC_AST_DECL_LIST(bc_ast_struct_item_list, struct bc_ast_struct_item);

BC_AST_DECL_ARR(bc_ast_struct_item_arr, struct bc_ast_struct_item);

struct bc_ast_struct {
    struct bc_lex_loc loc;
    struct bc_ast_struct_item_arr items;
};

struct bc_ast_enum {
    struct bc_lex_loc loc;
    struct bc_ast_ident_arr items;
};

struct bc_ast_const {
    struct bc_lex_loc loc;
    struct bc_ast_type type;
    struct bc_ast_expr expr;
};

union bc_ast_decl_val {
    struct bc_ast_func func;
    struct bc_ast_struct struct_;
    struct bc_ast_enum enum_;
    struct bc_ast_type type_alias;
    struct bc_ast_const const_;
};

struct bc_ast_decl {
    struct bc_lex_loc loc;
    union bc_ast_decl_val val;
    struct bc_strv name;
    enum bc_ast_decl_kind kind;
    bool is_export;
};

enum bc_ast_top_level_kind {
    BC_AST_TOP_LEVEL_IMPORT,
    BC_AST_TOP_LEVEL_DECL,
};

union bc_ast_top_level_val {
    struct bc_ast_import import;
    struct bc_ast_decl decl;
};

struct bc_ast_top_level {
    struct bc_lex_loc loc;
    enum bc_ast_top_level_kind kind;
    union bc_ast_top_level_val val;
};

BC_AST_DECL_LIST(bc_ast_top_level_list, struct bc_ast_top_level);

BC_AST_DECL_ARR(bc_ast_top_level_arr, struct bc_ast_top_level);

struct bc_ast_module {
    struct bc_lex_loc loc;
    struct bc_ast_top_level_arr top_level_items;
};

void bc_ast_print_stmt(struct bc_ast_stmt v, uint32_t indent);

void bc_ast_print_stmt_arr(const struct bc_ast_stmt_arr v, uint32_t indent);

void bc_ast_print_block(struct bc_ast_block v, uint32_t indent);

void bc_ast_print_ident_arr(const struct bc_ast_ident_arr v, uint32_t indent);

void bc_ast_print_import(struct bc_ast_import v, uint32_t indent);

void bc_ast_print_type(struct bc_ast_type v, uint32_t indent);

void bc_ast_print_type_arr(const struct bc_ast_type_arr v, uint32_t indent);

void bc_ast_print_func_param(struct bc_ast_func_param v, uint32_t indent);

void bc_ast_print_func_param_arr(
    const struct bc_ast_func_param_arr v, uint32_t indent);

void bc_ast_print_func(struct bc_ast_func v, uint32_t indent);

void bc_ast_print_literal(struct bc_ast_literal v, uint32_t indent);

void bc_ast_print_expr(struct bc_ast_expr v, uint32_t indent);

void bc_ast_print_expr_arr(const struct bc_ast_expr_arr v, uint32_t indent);

void bc_ast_print_let(struct bc_ast_let v, uint32_t indent);

void bc_ast_print_elif(struct bc_ast_elif v, uint32_t indent);

void bc_ast_print_elif_arr(const struct bc_ast_elif_arr v, uint32_t indent);

void bc_ast_print_if(struct bc_ast_if v, uint32_t indent);

void bc_ast_print_switchcase(struct bc_ast_switchcase v, uint32_t indent);

void bc_ast_print_switchcase_arr(
    const struct bc_ast_switchcase_arr v, uint32_t indent);

void bc_ast_print_switch(struct bc_ast_switch v, uint32_t indent);

void bc_ast_print_loop(struct bc_ast_loop v, uint32_t indent);

void bc_ast_print_for(struct bc_ast_for v, uint32_t indent);

void bc_ast_print_while(struct bc_ast_while v, uint32_t indent);

void bc_ast_print_return(struct bc_ast_return v, uint32_t indent);

void bc_ast_print_defer(struct bc_ast_defer v, uint32_t indent);

void bc_ast_print_struct_item(struct bc_ast_struct_item v, uint32_t indent);

void bc_ast_print_struct_item_arr(
    const struct bc_ast_struct_item_arr v, uint32_t indent);

void bc_ast_print_struct(struct bc_ast_struct v, uint32_t indent);

void bc_ast_print_enum(struct bc_ast_enum v, uint32_t indent);

void bc_ast_print_const(struct bc_ast_const v, uint32_t indent);

void bc_ast_print_decl(struct bc_ast_decl v, uint32_t indent);

void bc_ast_print_top_level(struct bc_ast_top_level v, uint32_t indent);

void bc_ast_print_top_level_arr(
    const struct bc_ast_top_level_arr v, uint32_t indent);

void bc_ast_print_module(struct bc_ast_module v, uint32_t indent);

#endif
