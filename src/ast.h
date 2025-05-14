#ifndef BALDC_AST_H
#define BALDC_AST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "str.h"
#include "lex.h"

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
    enum bc_ast_stmt_kind kind;
    union bc_ast_stmt_val val;
};

struct bc_ast_stmt_list {
    struct bc_ast_stmt stmt;
    struct bc_ast_stmt_list* next;
};

struct bc_ast_block {
    struct bc_ast_stmt_list* stmts;
};

struct bc_ast_ident_list {
    struct bc_strv segment;
    struct bc_ast_ident_list* next;
};

struct bc_ast_import {
    struct bc_strv renamed_to;
    struct bc_ast_ident_list* segments;
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

union bc_ast_type_val {
    struct bc_ast_type* arr;
    struct bc_ast_type_list* tup;
    struct bc_ast_type_func* func;
    struct bc_ast_type_path* path;
};

struct bc_ast_type_path {
    struct bc_ast_ident_list* segments;
    uint32_t super_count;
    bool is_root;
};

struct bc_ast_type {
    enum bc_ast_type_kind kind;
    union bc_ast_type_val val;
};

struct bc_ast_type_func {
    struct bc_ast_type_list* params;
    struct bc_ast_type ret;
};

struct bc_ast_type_list {
    struct bc_ast_type type;
    struct bc_ast_type_list* next;
};

struct bc_ast_func_param {
    struct bc_strv name;
    struct bc_ast_type type;
};

struct bc_ast_func_param_list {
    struct bc_ast_func_param param;
    struct bc_ast_func_param_list* next;
};

struct bc_ast_func {
    struct bc_ast_func_param_list* params;
    struct bc_ast_type ret;
    struct bc_ast_block block;
};

enum bc_ast_literal_array_kind {
    BC_AST_LITERAL_ARRAY_REGULAR,
    BC_AST_LITERAL_ARRAY_DEFAULT,
};

union bc_ast_literal_array_val {
    struct bc_ast_expr_list* regular;
    struct bc_ast_expr* default_;
};

struct bc_ast_literal_array {
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
    struct bc_ast_expr_list* tuple;
    struct bc_ast_func func;
};

struct bc_ast_literal {
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
    enum bc_tok_kind op;
    struct bc_ast_expr* operand;
};

struct bc_ast_expr_binop {
    enum bc_tok_kind op;
    struct bc_ast_expr* l;
    struct bc_ast_expr* r;
};

struct bc_ast_expr_index {
    struct bc_ast_expr* expr;
    struct bc_ast_expr* index;
};

struct bc_ast_expr_call {
    struct bc_ast_expr* expr;
    struct bc_ast_expr_list* args;
    size_t args_len;
};

enum bc_ast_expr_access_kind {
    BC_AST_EXPR_ACCESS_ROOT,
    BC_AST_EXPR_ACCESS_SUPER,
    BC_AST_EXPR_ACCESS_NAME,
};

struct bc_ast_expr_cast {
    struct bc_ast_expr* expr;
    struct bc_ast_type type;
};

enum bc_ast_expr_path_segment_kind {
    BC_AST_EXPR_PATH_SEGMENT_ROOT,
    BC_AST_EXPR_PATH_SEGMENT_SUPER,
    BC_AST_EXPR_PATH_SEGMENT_NAME,
};

struct bc_ast_expr_path_segment {
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
    enum bc_ast_expr_kind kind;
    union bc_ast_expr_val val;
};

struct bc_ast_expr_list {
    struct bc_ast_expr expr;
    struct bc_ast_expr_list* next;
};

struct bc_ast_let {
    struct bc_strv name;
    struct bc_ast_type type;
    struct bc_ast_expr expr;
    bool has_type;
    bool has_expr;
};

struct bc_ast_elif {
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

struct bc_ast_elif_list {
    struct bc_ast_elif elif;
    struct bc_ast_elif_list* next;
};

struct bc_ast_if {
    struct bc_ast_expr expr;
    struct bc_ast_block main;
    struct bc_ast_elif_list* elifs;
    struct bc_ast_block else_;
    bool has_else;
};

struct bc_ast_switchcase {
    struct bc_ast_expr expr;
    struct bc_ast_block block;
    bool is_default;
};

struct bc_ast_switchcase_list {
    struct bc_ast_switchcase case_;
    struct bc_ast_switchcase_list* next;
};

struct bc_ast_switch {
    struct bc_ast_expr expr;
    struct bc_ast_switchcase_list* cases;
};

struct bc_ast_loop {
    struct bc_ast_block block;
};

struct bc_ast_for {
    struct bc_strv name;
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

struct bc_ast_while {
    struct bc_ast_expr expr;
    struct bc_ast_block block;
};

struct bc_ast_return {
    struct bc_ast_expr expr;
    bool is_empty;
};

struct bc_ast_defer {
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
    struct bc_strv name;
    struct bc_ast_type type;
    bool is_export;
};

struct bc_ast_struct_item_list {
    struct bc_ast_struct_item item;
    struct bc_ast_struct_item_list* next;
};

struct bc_ast_struct {
    struct bc_ast_struct_item_list* items;
};

struct bc_ast_enum {
    struct bc_ast_ident_list* items;
};

struct bc_ast_const {
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
    enum bc_ast_top_level_kind kind;
    union bc_ast_top_level_val val;
};

struct bc_ast_top_level_list {
    struct bc_ast_top_level top_level;
    struct bc_ast_top_level_list* next;
};

struct bc_ast_module {
    struct bc_ast_top_level_list* top_level_items;
};

void bc_ast_print_stmt(struct bc_ast_stmt v, uint32_t indent);

void bc_ast_print_stmt_list(const struct bc_ast_stmt_list* v, uint32_t indent);

void bc_ast_print_block(struct bc_ast_block v, uint32_t indent);

void bc_ast_print_ident_list(
    const struct bc_ast_ident_list* v, uint32_t indent);

void bc_ast_print_import(struct bc_ast_import v, uint32_t indent);

void bc_ast_print_type(struct bc_ast_type v, uint32_t indent);

void bc_ast_print_type_list(const struct bc_ast_type_list* v, uint32_t indent);

void bc_ast_print_func_param(struct bc_ast_func_param v, uint32_t indent);

void bc_ast_print_func_param_list(
    const struct bc_ast_func_param_list* v, uint32_t indent);

void bc_ast_print_func(struct bc_ast_func v, uint32_t indent);

void bc_ast_print_literal(struct bc_ast_literal v, uint32_t indent);

void bc_ast_print_expr(struct bc_ast_expr v, uint32_t indent);

void bc_ast_print_expr_list(const struct bc_ast_expr_list* v, uint32_t indent);

void bc_ast_print_let(struct bc_ast_let v, uint32_t indent);

void bc_ast_print_elif(struct bc_ast_elif v, uint32_t indent);

void bc_ast_print_elif_list(const struct bc_ast_elif_list* v, uint32_t indent);

void bc_ast_print_if(struct bc_ast_if v, uint32_t indent);

void bc_ast_print_switchcase(struct bc_ast_switchcase v, uint32_t indent);

void bc_ast_print_switchcase_list(
    const struct bc_ast_switchcase_list* v, uint32_t indent);

void bc_ast_print_switch(struct bc_ast_switch v, uint32_t indent);

void bc_ast_print_loop(struct bc_ast_loop v, uint32_t indent);

void bc_ast_print_for(struct bc_ast_for v, uint32_t indent);

void bc_ast_print_while(struct bc_ast_while v, uint32_t indent);

void bc_ast_print_return(struct bc_ast_return v, uint32_t indent);

void bc_ast_print_defer(struct bc_ast_defer v, uint32_t indent);

void bc_ast_print_struct_item(struct bc_ast_struct_item v, uint32_t indent);

void bc_ast_print_struct_item_list(
    const struct bc_ast_struct_item_list* v, uint32_t indent);

void bc_ast_print_struct(struct bc_ast_struct v, uint32_t indent);

void bc_ast_print_enum(struct bc_ast_enum v, uint32_t indent);

void bc_ast_print_const(struct bc_ast_const v, uint32_t indent);

void bc_ast_print_decl(struct bc_ast_decl v, uint32_t indent);

void bc_ast_print_top_level(struct bc_ast_top_level v, uint32_t indent);

void bc_ast_print_top_level_list(
    const struct bc_ast_top_level_list* v, uint32_t indent);

void bc_ast_print_module(struct bc_ast_module v, uint32_t indent);

#endif
