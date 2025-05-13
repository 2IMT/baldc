#include "ast.h"

#include "print.h"
#include "str.h"
#include <complex.h>

#define _I 2

static void _indent(uint32_t indent) {
    for (uint32_t i = 0; i < indent; i++) {
        bc_printf(" ");
    }
}

void bc_ast_print_stmt(struct bc_ast_stmt v, uint32_t indent) {
    _indent(indent);
    bc_printf("stmt(");
    switch (v.kind) {
    case BC_AST_STMT_EXPR:
        bc_printf("expr) {$n");
        bc_ast_print_expr(*v.val.expr, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_LET:
        bc_printf("let) {$n");
        bc_ast_print_let(*v.val.let, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_IF:
        bc_printf("if) {$n");
        bc_ast_print_if(*v.val.if_, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_SWITCH:
        bc_printf("switch) {$n");
        bc_ast_print_switch(*v.val.switch_, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_LOOP:
        bc_printf("loop) {$n");
        bc_ast_print_loop(*v.val.loop, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_FOR:
        bc_printf("for) {$n");
        bc_ast_print_for(*v.val.for_, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_WHILE:
        bc_printf("while) {$n");
        bc_ast_print_while(*v.val.while_, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_CONTINUE:
        bc_printf("continue)");
        break;
    case BC_AST_STMT_BREAK:
        bc_printf("break)");
        break;
    case BC_AST_STMT_RETURN:
        bc_printf("return) {$n");
        bc_ast_print_return(*v.val.return_, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_DEFER:
        bc_printf("defer) {$n");
        bc_ast_print_defer(*v.val.defer, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_STMT_BLOCK:
        bc_printf("block) {$n");
        bc_ast_print_block(*v.val.block, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    }
}

void bc_ast_print_stmt_list(const struct bc_ast_stmt_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("stmt_list {$n");
    while (v != NULL) {
        bc_ast_print_stmt(v->stmt, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_block(struct bc_ast_block v, uint32_t indent) {
    _indent(indent);
    bc_printf("block {$n");
    bc_ast_print_stmt_list(v.stmts, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_ident_list(
    const struct bc_ast_ident_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("ident_list {$n");
    while (v != NULL) {
        bc_printf(BC_STRV_PRNT, BC_STRV_PRNTV(v->segment));
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_import(struct bc_ast_import v, uint32_t indent) {
    _indent(indent);
    bc_printf("import(");
    bc_printf("is_glob:$s,", v.is_glob ? "true" : "false");
    bc_printf("is_root:$s,", v.is_root ? "true" : "false");
    bc_printf("is_renamed:$s", v.is_renamed ? "true" : "false");
    if (v.is_renamed) {
        bc_printf(",renamed_to:" BC_STRV_PRNT, BC_STRV_PRNTV(v.renamed_to));
    }
    bc_printf(") {$n");
    bc_ast_print_ident_list(v.segments, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_type(struct bc_ast_type v, uint32_t indent) {
    _indent(indent);
    bc_printf("type(");
    switch (v.kind) {
    case BC_AST_TYPE_UNIT:
        bc_printf("unit)");
        break;
    case BC_AST_TYPE_STRING:
        bc_printf("string)");
        break;
    case BC_AST_TYPE_CHAR:
        bc_printf("char)");
        break;
    case BC_AST_TYPE_INT:
        bc_printf("int)");
        break;
    case BC_AST_TYPE_BYTE:
        bc_printf("byte)");
        break;
    case BC_AST_TYPE_FLOAT:
        bc_printf("float)");
        break;
    case BC_AST_TYPE_BOOL:
        bc_printf("bool)");
        break;
    case BC_AST_TYPE_ARRAY:
        bc_printf("array) {$n");
        bc_ast_print_type(*v.val.arr, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_TYPE_TUP:
        bc_printf("tup) {$n");
        bc_ast_print_type_list(v.val.tup, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_TYPE_FUNC:
        bc_printf("func) {$n");
        _indent(indent + _I);
        bc_printf("params {$n");
        bc_ast_print_type_list(v.val.func->params, indent + _I * 2);
        bc_printf("$n");
        _indent(indent + _I);
        bc_printf("}$n");
        _indent(indent + _I);
        bc_printf("ret {$n");
        bc_ast_print_type(v.val.func->ret, indent + _I * 2);
        bc_printf("$n");
        _indent(indent + _I);
        bc_printf("}$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_TYPE_PATH:
        bc_printf("path(is_root:$s,super_count:$i)) {$n",
            v.val.path->is_root ? "true" : "false", v.val.path->super_count);
        bc_ast_print_ident_list(v.val.path->segments, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    }
}

void bc_ast_print_type_list(const struct bc_ast_type_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("types {$n");
    while (v != NULL) {
        bc_ast_print_type(v->type, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_func_param(struct bc_ast_func_param v, uint32_t indent) {
    _indent(indent);
    bc_printf("param(" BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    bc_ast_print_type(v.type, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_func_param_list(
    const struct bc_ast_func_param_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("params {$n");
    while (v != NULL) {
        bc_ast_print_func_param(v->param, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_func(struct bc_ast_func v, uint32_t indent) {
    _indent(indent);
    bc_printf("func {$n");
    bc_ast_print_func_param_list(v.params, indent + _I);
    bc_printf("$n");
    bc_ast_print_type(v.ret, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_literal(struct bc_ast_literal v, uint32_t indent) {
    _indent(indent);
    bc_printf("literal(");
    switch (v.kind) {
    case BC_AST_LITERAL_UNIT:
        bc_printf("unit)");
        break;
    case BC_AST_LITERAL_NULL:
        bc_printf("null)");
        break;
    case BC_AST_LITERAL_STRING:
        bc_printf(
            "string(\"" BC_STRV_PRNT "\"))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_CHARACTER:
        bc_printf(
            "character('" BC_STRV_PRNT "'))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_INTEGER:
        bc_printf("integer(" BC_STRV_PRNT "))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_BYTE:
        bc_printf("byte(" BC_STRV_PRNT "))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_FLOATING:
        bc_printf(
            "floating(" BC_STRV_PRNT "))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_BOOLEAN:
        bc_printf("boolean(" BC_STRV_PRNT "))", BC_STRV_PRNTV(v.val.primitive));
        break;
    case BC_AST_LITERAL_ARRAY:
        switch (v.val.array.kind) {
        case BC_AST_LITERAL_ARRAY_REGULAR:
            bc_printf("array(regular)) {$n");
            bc_ast_print_expr_list(v.val.array.val.regular, indent + _I);
            break;
        case BC_AST_LITERAL_ARRAY_DEFAULT:
            bc_printf("array(default)) {$n");
            bc_ast_print_expr(*v.val.array.val.default_, indent + _I);
            break;
        }
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_LITERAL_TUP:
        break;
    case BC_AST_LITERAL_FUNC:
        bc_printf("func) {$n");
        bc_ast_print_func(v.val.func, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    }
}

void bc_ast_print_expr(struct bc_ast_expr v, uint32_t indent) {
    _indent(indent);
    bc_printf("expr(");
    switch (v.kind) {
    case BC_AST_EXPR_LITERAL:
        bc_printf("literal) {$n");
        bc_ast_print_literal(v.val.literal, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_UNOP:
        bc_printf(
            "unop(" BC_STRV_PRNT ")) {$n", bc_tok_kind_to_strv(v.val.unop.op));
        bc_ast_print_expr(*v.val.unop.operand, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_BINOP:
        bc_printf("binop(" BC_STRV_PRNT ")) {$n",
            bc_tok_kind_to_strv(v.val.binop.op));
        bc_ast_print_expr(*v.val.binop.l, indent + _I);
        bc_printf("$n");
        bc_ast_print_expr(*v.val.binop.r, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_INDEX:
        bc_printf("index) {$n");
        bc_ast_print_expr(*v.val.index.expr, indent + _I);
        bc_printf("$n");
        bc_ast_print_expr(*v.val.index.index, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_CALL:
        bc_printf("call) {$n");
        bc_ast_print_expr(*v.val.call.expr, indent + _I);
        bc_printf("$n");
        bc_ast_print_expr_list(v.val.call.args, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_CAST:
        bc_printf("cast) $n");
        bc_ast_print_expr(*v.val.cast.expr, indent + _I);
        bc_printf("$n");
        bc_ast_print_type(v.val.cast.type, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_TRYCAST:
        bc_printf("try_cast) $n");
        bc_ast_print_expr(*v.val.try_cast.expr, indent + _I);
        bc_printf("$n");
        bc_ast_print_type(v.val.try_cast.type, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
        break;
    case BC_AST_EXPR_PATH_SEGMENT:
        switch (v.val.path_segment.kind) {
        case BC_AST_EXPR_PATH_SEGMENT_ROOT:
            bc_printf("path_segment(root))");
            break;
        case BC_AST_EXPR_PATH_SEGMENT_SUPER:
            bc_printf("path_segment(super))");
            break;
        case BC_AST_EXPR_PATH_SEGMENT_NAME:
            bc_printf("path_segment(" BC_STRV_PRNT "))",
                BC_STRV_PRNTV(v.val.path_segment.name));
            break;
        }
        break;
    }
}

void bc_ast_print_expr_list(const struct bc_ast_expr_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("expr_list {$n");
    while (v != NULL) {
        bc_ast_print_expr(v->expr, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_let(struct bc_ast_let v, uint32_t indent) {
    _indent(indent);
    bc_printf("let(" BC_STRV_PRNT ")", BC_STRV_PRNTV(v.name));
    if (v.expr != NULL || v.type != NULL) {
        bc_printf(" {$n");
        if (v.type != NULL) {
            bc_ast_print_type(*v.type, indent + _I);
            bc_printf("$n");
        }
        if (v.expr != NULL) {
            bc_ast_print_expr(*v.expr, indent + _I);
            bc_printf("$n");
        }
        _indent(indent);
        bc_printf("}");
    }
}

void bc_ast_print_elif(struct bc_ast_elif v, uint32_t indent) {
    _indent(indent);
    bc_printf("elif {$n");
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_elif_list(const struct bc_ast_elif_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("elif_list {$n");
    while (v != NULL) {
        bc_ast_print_elif(v->elif, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("{");
}

void bc_ast_print_if(struct bc_ast_if v, uint32_t indent) {
    _indent(indent);
    bc_printf("if {$n");
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.main, indent + _I);
    bc_printf("$n");
    bc_ast_print_elif_list(v.elifs, indent + _I);
    bc_printf("$n");
    if (v.else_ != NULL) {
        bc_ast_print_block(*v.else_, indent + _I);
        bc_printf("$n");
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_switchcase(struct bc_ast_switchcase v, uint32_t indent) {
    _indent(indent);
    bc_printf("switchcase {$n");
    bc_ast_print_expr(*v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_switchcase_list(
    const struct bc_ast_switchcase_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("switchcase_list {$n");
    while (v != NULL) {
        bc_ast_print_switchcase(v->case_, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_switch(struct bc_ast_switch v, uint32_t indent) {
    _indent(indent);
    bc_printf("switch {$n");
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_switchcase_list(v.cases, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_loop(struct bc_ast_loop v, uint32_t indent) {
    _indent(indent);
    bc_printf("loop {$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_for(struct bc_ast_for v, uint32_t indent) {
    _indent(indent);
    bc_printf("for(" BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_while(struct bc_ast_while v, uint32_t indent) {
    _indent(indent);
    bc_printf("while {$n");
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    bc_ast_print_block(v.block, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_return(struct bc_ast_return v, uint32_t indent) {
    _indent(indent);
    bc_printf("return");
    if (v.expr != NULL) {
        bc_printf(" {$n");
        bc_ast_print_expr(*v.expr, indent + _I);
        bc_printf("$n");
        _indent(indent);
        bc_printf("}");
    }
}

void bc_ast_print_defer(struct bc_ast_defer v, uint32_t indent) {
    _indent(indent);
    bc_printf("defer {$n");
    bc_ast_print_expr(v.callee, indent + _I);
    bc_printf("$n");
    bc_ast_print_expr_list(v.args, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_struct_item(struct bc_ast_struct_item v, uint32_t indent) {
    _indent(indent);
    bc_printf("struct_item");
    if (v.is_export) {
        bc_printf("(export " BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    } else {
        bc_printf("(" BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    }
    bc_ast_print_type(v.type, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_struct_item_list(
    const struct bc_ast_struct_item_list* v, uint32_t indent) {
    bc_printf("struct_item_list {$n");
    while (v != NULL) {
        bc_ast_print_struct_item(v->item, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_struct(struct bc_ast_struct v, uint32_t indent) {
    _indent(indent);
    bc_printf("struct {$n");
    bc_ast_print_struct_item_list(v.items, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_enum(struct bc_ast_enum v, uint32_t indent) {
    _indent(indent);
    bc_printf("enum {$n");
    bc_ast_print_ident_list(v.items, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_const(struct bc_ast_const v, uint32_t indent) {
    _indent(indent);
    bc_printf("const {$n");
    bc_ast_print_type(v.type, indent + _I);
    bc_printf("$n");
    bc_ast_print_expr(v.expr, indent + _I);
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_decl(struct bc_ast_decl v, uint32_t indent) {
    _indent(indent);
    bc_printf("decl");
    if (v.is_export) {
        bc_printf("(export " BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    } else {
        bc_printf("(" BC_STRV_PRNT ") {$n", BC_STRV_PRNTV(v.name));
    }
    switch (v.kind) {
    case BC_AST_DECL_FUNC:
        bc_ast_print_func(v.val.func, indent + _I);
        break;
    case BC_AST_DECL_STRUCT:
        bc_ast_print_struct(v.val.struct_, indent + _I);
        break;
    case BC_AST_DECL_ENUM:
        bc_ast_print_enum(v.val.enum_, indent + _I);
        break;
    case BC_AST_DECL_TYPE_ALIAS:
        bc_ast_print_type(v.val.type_alias, indent + _I);
        break;
    case BC_AST_DECL_CONST:
        bc_ast_print_const(v.val.const_, indent + _I);
        break;
    }
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_top_level(struct bc_ast_top_level v, uint32_t indent) {
    _indent(indent);
    bc_printf("top_level {$n");
    switch (v.kind) {
    case BC_AST_TOP_LEVEL_IMPORT:
        bc_ast_print_import(v.val.import, indent + _I);
    case BC_AST_TOP_LEVEL_DECL:
        bc_ast_print_decl(v.val.decl, indent + _I);
        break;
    }
    bc_printf("$n");
    _indent(indent);
    bc_printf("}");
}

void bc_ast_print_top_level_list(
    const struct bc_ast_top_level_list* v, uint32_t indent) {
    _indent(indent);
    bc_printf("top_level_list {$n");
    while (v != NULL) {
        bc_ast_print_top_level(v->top_level, indent + _I);
        if (v->next != NULL) {
            bc_printf(",");
        }
        bc_printf("$n");
        v = v->next;
    }
    _indent(indent);
    bc_printf("}");
}
