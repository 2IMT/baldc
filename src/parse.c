#include "parse.h"

#include <stdbool.h>

#include "ast.h"
#include "lex.h"

#define _ALLOC_NODE(type) BC_MEM_ARENA_ALLOC_TYPE(&parse->node_arena, type)

#define _ERR_EXPECTED_MULTIPLE(...) \
    do { \
        static const char* items[] = { __VA_ARGS__ }; \
        if (parse->err_callback != NULL) { \
            parse->err_callback( \
                (struct bc_parse_err) { \
                    .loc = parse->tok.loc, \
                    .kind = BC_PARSE_ERR_EXPECTED_MULTIPLE, \
                    .val = { .expected_multiple = { .items = items, \
                                 .len = sizeof(items) / sizeof(items[0]), \
                                 .got = parse->tok } }, \
                }, \
                parse->err_user_data); \
        } \
    } while (false)

static bool _prefix_bp(enum bc_tok_kind op, uint8_t* r) {
    switch (op) {
    case BC_TOK_PLUS:
    case BC_TOK_DASH:
    case BC_TOK_EXCLAM:
    case BC_TOK_TILDE:
    case BC_TOK_AMP:
    case BC_TOK_STAR:
        *r = 9;
        return true;
    default:
        return false;
    }
}

static bool _postfix_bp(enum bc_tok_kind op, uint8_t* l) {
    switch (op) {
    case BC_TOK_LPAREN:
    case BC_TOK_LBRACKET:
    case BC_TOK_KW_AS:
    case BC_TOK_KW_TRYAS:
        *l = 11;
        return true;
    default:
        return false;
    }
}

static bool _infix_bp(enum bc_tok_kind op, uint8_t* l, uint8_t* r) {
    switch (op) {
    // Assignment
    case BC_TOK_EQ:
    case BC_TOK_PLUSEQ:
    case BC_TOK_DASHEQ:
    case BC_TOK_STAREQ:
    case BC_TOK_SLASHEQ:
    case BC_TOK_AMPEQ:
    case BC_TOK_PIPEEQ:
    case BC_TOK_CARETEQ:
    case BC_TOK_PERCENTEQ:
    case BC_TOK_LANLANEQ:
    case BC_TOK_RANRANEQ:
        *l = 2;
        *r = 1;
        return true;

    // Logical OR/AND
    case BC_TOK_PIPEPIPE:
        *l = 5;
        *r = 6;
        return true;
    case BC_TOK_AMPAMP:
        *l = 7;
        *r = 8;
        return true;

    // Bitwise
    case BC_TOK_PIPE:
        *l = 9;
        *r = 10;
        return true;
    case BC_TOK_CARET:
        *l = 11;
        *r = 12;
        return true;
    case BC_TOK_AMP:
        *l = 13;
        *r = 14;
        return true;

    // Comparison
    case BC_TOK_EQEQ:
    case BC_TOK_EXCLEQ:
    case BC_TOK_LANGLE:
    case BC_TOK_LANEQ:
    case BC_TOK_RANGLE:
    case BC_TOK_RANEQ:
        *l = 15;
        *r = 16;
        return true;

    // Shifts
    case BC_TOK_LANLAN:
    case BC_TOK_RANRAN:
        *l = 17;
        *r = 18;
        return true;

    // Arithmetic
    case BC_TOK_PLUS:
    case BC_TOK_DASH:
        *l = 19;
        *r = 20;
        return true;
    case BC_TOK_STAR:
    case BC_TOK_SLASH:
    case BC_TOK_PERCENT:
        *l = 21;
        *r = 22;
        return true;

    // Member access and paths
    case BC_TOK_DOT:
    case BC_TOK_COLCOL:
        *l = 23;
        *r = 24;
        return true;

    default:
        return false;
    }
}

static void _nexttok(struct bc_parse* parse) {
    parse->tok = bc_lex_next(&parse->lex);
    if (parse->tok_callback != NULL) {
        parse->tok_callback(parse->tok, parse->tok_user_data);
    }
    if (parse->tok.kind == BC_TOK_ERR) {
        if (parse->err_callback != NULL) {
            parse->err_callback(
                (struct bc_parse_err) { .kind = BC_PARSE_ERR_LEX,
                    .val = { .lex = parse->tok.val.err } },
                parse->err_user_data);
        }
    }
}

static bool _accept_get(
    struct bc_parse* parse, enum bc_tok_kind kind, struct bc_tok* tok) {
    struct bc_tok saved = parse->tok;
    if (parse->tok.kind == kind) {
        if (tok != NULL) {
            *tok = saved;
        }
        _nexttok(parse);
        return true;
    }
    return false;
}

static bool _accept(struct bc_parse* parse, enum bc_tok_kind kind) {
    return _accept_get(parse, kind, NULL);
}

static bool _expect_get(
    struct bc_parse* parse, enum bc_tok_kind kind, struct bc_tok* tok) {
    if (_accept_get(parse, kind, tok)) {
        return true;
    }
    parse->err_callback(
        (struct bc_parse_err) {
            .loc = parse->tok.loc,
            .kind = BC_PARSE_ERR_UNEXPECTED_TOKEN,
            .val = { .unexpected_token = parse->tok },
        },
        parse->err_user_data);
    return false;
}

static bool _expect(struct bc_parse* parse, enum bc_tok_kind kind) {
    return _expect_get(parse, kind, NULL);
}

static bool _curr(struct bc_parse* parse, enum bc_tok_kind kind) {
    return parse->tok.kind == kind;
}

struct bc_parse bc_parse_new(struct bc_lex lex,
    bc_parse_err_callback err_callback, void* err_user_data,
    bc_parse_tok_callback tok_callback, void* tok_user_data) {
    struct bc_parse parse = {
        .init = true,
        .lex = lex,
        .err_callback = err_callback,
        .err_user_data = err_user_data,
        .tok_callback = tok_callback,
        .tok_user_data = tok_user_data,
        .node_arena = bc_mem_arena_new(8 * 1024),
    };
    _nexttok(&parse);
    return parse;
}

void bc_parse_free(struct bc_parse parse) {
    bc_lex_free(parse.lex);
    bc_mem_arena_free(parse.node_arena);
}

bool bc_parse_import(struct bc_parse* parse, struct bc_ast_import* import) {
    if (!_expect(parse, BC_TOK_KW_IMPORT)) {
        return false;
    }

    import->renamed_to = (struct bc_strv) { 0 };
    import->segments = NULL;
    import->is_glob = false;
    import->is_renamed = false;
    import->super_count = 0;
    import->is_root = false;

    struct bc_ast_ident_list* curr = NULL;
    struct bc_tok tok;

    if (_accept(parse, BC_TOK_KW_ROOT)) {
        if (!_expect(parse, BC_TOK_COLCOL)) {
            return false;
        }
        import->is_root = true;
    }

    while (_accept(parse, BC_TOK_KW_SUPER) && !import->is_root) {
        if (!_expect(parse, BC_TOK_COLCOL)) {
            return false;
        }
        import->super_count++;
    }

    while (true) {
        if (_accept_get(parse, BC_TOK_IDENT, &tok)) {
            struct bc_ast_ident_list* new_segment =
                _ALLOC_NODE(struct bc_ast_ident_list);
            new_segment->segment = tok.val.ident;
            new_segment->next = NULL;
            if (import->segments == NULL) {
                import->segments = new_segment;
                curr = new_segment;
            } else {
                curr->next = new_segment;
                curr = new_segment;
            }

            if (!_accept(parse, BC_TOK_COLCOL)) {
                break;
            }
        } else if (_accept(parse, BC_TOK_STAR)) {
            import->is_glob = true;
            break;
        } else {
            return false;
        }
    }

    if (_accept(parse, BC_TOK_KW_AS)) {
        if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
            return false;
        }
        import->is_renamed = true;
        import->renamed_to = tok.val.ident;
    }

    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }

    return true;
}

bool bc_parse_literal(struct bc_parse* parse, struct bc_ast_literal* lit) {
    struct bc_tok tok;
    if (_accept(parse, BC_TOK_KW_UNIT)) {
        lit->kind = BC_AST_LITERAL_UNIT;
    } else if (_accept(parse, BC_TOK_KW_NULL)) {
        lit->kind = BC_AST_LITERAL_NULL;
    } else if (_accept_get(parse, BC_TOK_LIT_STRING, &tok)) {
        lit->kind = BC_AST_LITERAL_STRING;
        lit->val.primitive = tok.val.literal;
    } else if (_accept_get(parse, BC_TOK_LIT_CHARACTER, &tok)) {
        lit->kind = BC_AST_LITERAL_CHARACTER;
        lit->val.primitive = tok.val.literal;
    } else if (_accept_get(parse, BC_TOK_LIT_INTEGER, &tok)) {
        lit->kind = BC_AST_LITERAL_INTEGER;
        lit->val.primitive = tok.val.literal;
    } else if (_accept_get(parse, BC_TOK_LIT_BYTE, &tok)) {
        lit->kind = BC_AST_LITERAL_BYTE;
        lit->val.primitive = tok.val.literal;
    } else if (_accept_get(parse, BC_TOK_LIT_FLOATING, &tok)) {
        lit->kind = BC_AST_LITERAL_FLOATING;
        lit->val.primitive = tok.val.literal;
    } else if (_accept_get(parse, BC_TOK_LIT_BOOLEAN, &tok)) {
        lit->kind = BC_AST_LITERAL_BOOLEAN;
        lit->val.primitive = tok.val.literal;
    } else if (_accept(parse, BC_TOK_LBRACKET)) {
        lit->kind = BC_AST_LITERAL_ARRAY;
        if (_accept(parse, BC_TOK_KW_DEFAULT)) {
            lit->val.array.kind = BC_AST_LITERAL_ARRAY_DEFAULT;
            if (!_expect(parse, BC_TOK_SEMICOLON)) {
                return false;
            }
            struct bc_ast_expr* expr = _ALLOC_NODE(struct bc_ast_expr);
            if (!bc_parse_expression(parse, expr)) {
                return false;
            }
            lit->val.array.val.default_ = expr;
            if (!_expect(parse, BC_TOK_RBRACKET)) {
                return false;
            }
        } else {
            lit->val.array.kind = BC_AST_LITERAL_ARRAY_REGULAR;
            lit->val.array.val.regular = NULL;
            if (_accept(parse, BC_TOK_RBRACKET)) {
                return true;
            }
            lit->val.array.val.regular = _ALLOC_NODE(struct bc_ast_expr_list);
            if (!bc_parse_expression_list(
                    parse, BC_TOK_RBRACKET, lit->val.array.val.regular)) {
                return false;
            }
            if (!_expect(parse, BC_TOK_RBRACKET)) {
                return false;
            }
        }
    } else if (_accept(parse, BC_TOK_KW_TUP)) {
        lit->kind = BC_AST_LITERAL_TUP;
        lit->val.tuple = _ALLOC_NODE(struct bc_ast_expr_list);
        if (!_expect(parse, BC_TOK_LPAREN)) {
            return false;
        }
        if (!bc_parse_expression_list(parse, BC_TOK_RPAREN, lit->val.tuple)) {
            return false;
        }
        if (!_expect(parse, BC_TOK_RPAREN)) {
            return false;
        }
    } else if (_accept(parse, BC_TOK_KW_FUNC)) {
        lit->kind = BC_AST_LITERAL_FUNC;
        if (!bc_parse_func(parse, &lit->val.func)) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool bc_parse_expression_list(struct bc_parse* parse,
    enum bc_tok_kind terminator, struct bc_ast_expr_list* list) {
    if (!bc_parse_expression(parse, &list->expr)) {
        return false;
    }
    list->next = NULL;
    while (_accept(parse, BC_TOK_COMMA)) {
        struct bc_ast_expr_list* next = _ALLOC_NODE(struct bc_ast_expr_list);
        next->next = NULL;
        if (_curr(parse, terminator)) {
            break;
        }
        if (!bc_parse_expression(parse, &next->expr)) {
            return false;
        }
        list->next = next;
        list = next;
    }

    return true;
}

bool bc_parse_expression_bp(
    struct bc_parse* parse, uint8_t min_bp, struct bc_ast_expr* expr) {
    uint8_t bp_l = 0, bp_r = 0;
    struct bc_ast_expr lhs = { 0 };
    struct bc_tok tok;
    if (_accept(parse, BC_TOK_LPAREN)) {
        if (!bc_parse_expression(parse, &lhs)) {
            return false;
        }
        if (!_expect(parse, BC_TOK_RPAREN)) {
            return false;
        }
    } else if (_accept_get(parse, BC_TOK_IDENT, &tok)) {
        lhs.kind = BC_AST_EXPR_PATH_SEGMENT;
        lhs.val.path_segment.kind = BC_AST_EXPR_PATH_SEGMENT_NAME;
        lhs.val.path_segment.name = tok.val.ident;
    } else if (_accept(parse, BC_TOK_KW_ROOT)) {
        lhs.kind = BC_AST_EXPR_PATH_SEGMENT;
        lhs.val.path_segment.kind = BC_AST_EXPR_PATH_SEGMENT_ROOT;
    } else if (_accept(parse, BC_TOK_KW_SUPER)) {
        lhs.kind = BC_AST_EXPR_PATH_SEGMENT;
        lhs.val.path_segment.kind = BC_AST_EXPR_PATH_SEGMENT_ROOT;
    } else if (_prefix_bp(parse->tok.kind, &bp_r)) {
        lhs.kind = BC_AST_EXPR_UNOP;
        lhs.val.unop.op = parse->tok.kind;
        _nexttok(parse);
        struct bc_ast_expr* operand = _ALLOC_NODE(struct bc_ast_expr);
        if (!bc_parse_expression_bp(parse, bp_r, operand)) {
            return false;
        }
        lhs.val.unop.operand = operand;
    } else if (!bc_parse_literal(parse, &lhs.val.literal)) {
        _ERR_EXPECTED_MULTIPLE("expression");
        return false;
    }

    while (true) {
        if (_accept(parse, BC_TOK_EOF)) {
            break;
        }

        struct bc_ast_expr res;
        struct bc_ast_expr* res_lhs = _ALLOC_NODE(struct bc_ast_expr);
        *res_lhs = lhs;

        if (_postfix_bp(parse->tok.kind, &bp_l)) {
            if (bp_l < min_bp) {
                break;
            }

            if (_accept(parse, BC_TOK_LBRACKET)) {
                struct bc_ast_expr* index = _ALLOC_NODE(struct bc_ast_expr);
                if (!bc_parse_expression(parse, index)) {
                    return false;
                }
                if (!_expect(parse, BC_TOK_RBRACKET)) {
                    return false;
                }
                res.val.index.expr = res_lhs;
                res.kind = BC_AST_EXPR_INDEX;
                res.val.index.index = index;
                lhs = res;
            } else if (_accept(parse, BC_TOK_LPAREN)) {
                res.kind = BC_AST_EXPR_CALL;
                res.val.call.args = NULL;
                res.val.call.expr = res_lhs;
                if (_accept(parse, BC_TOK_RPAREN)) {
                    lhs = res;
                    continue;
                } else {
                    struct bc_ast_expr_list* args =
                        _ALLOC_NODE(struct bc_ast_expr_list);
                    res.val.call.args = args;
                    if (!bc_parse_expression_list(parse, BC_TOK_RPAREN, args)) {
                        return false;
                    }
                    if (!_expect(parse, BC_TOK_RPAREN)) {
                        return false;
                    }
                    lhs = res;
                    continue;
                }
            } else if (_accept(parse, BC_TOK_KW_AS)) {
                res.kind = BC_AST_EXPR_CAST;
                res.val.cast.expr = res_lhs;
                if (!bc_parse_type(parse, &res.val.cast.type)) {
                    return false;
                }
                lhs = res;
            } else if (_accept(parse, BC_TOK_KW_TRYAS)) {
                res.kind = BC_AST_EXPR_TRYCAST;
                res.val.try_cast.expr = res_lhs;
                if (!bc_parse_type(parse, &res.val.try_cast.type)) {
                    return false;
                }
                lhs = res;
            }
            continue;
        }

        if (_infix_bp(parse->tok.kind, &bp_l, &bp_r)) {
            if (bp_l < min_bp) {
                break;
            }
            struct bc_ast_expr* res_rhs = _ALLOC_NODE(struct bc_ast_expr);
            res.kind = BC_AST_EXPR_BINOP;
            res.val.binop.op = parse->tok.kind;
            res.val.binop.l = res_lhs;
            res.val.binop.r = res_rhs;
            lhs = res;
            _nexttok(parse);

            if (!bc_parse_expression_bp(parse, bp_r, res_rhs)) {
                return false;
            }
            continue;
        }

        break;
    }
    *expr = lhs;
    return true;
}

bool bc_parse_expression(struct bc_parse* parse, struct bc_ast_expr* expr) {
    return bc_parse_expression_bp(parse, 0, expr);
}

bool bc_parse_let(struct bc_parse* parse, struct bc_ast_let* let) {
    let->expr = NULL;
    let->type = NULL;
    if (!_expect(parse, BC_TOK_KW_LET)) {
        return false;
    }
    struct bc_tok tok;
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    let->name = tok.val.ident;
    if (_accept(parse, BC_TOK_COLON)) {
        let->type = _ALLOC_NODE(struct bc_ast_type);
        if (!bc_parse_type(parse, let->type)) {
            return false;
        }
    }
    if (_accept(parse, BC_TOK_EQ)) {
        let->expr = _ALLOC_NODE(struct bc_ast_expr);
        if (!bc_parse_expression(parse, let->expr)) {
            return false;
        }
    }
    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }
    return true;
}

bool bc_parse_if(struct bc_parse* parse, struct bc_ast_if* if_) {
    if_->else_ = NULL;
    if_->elifs = NULL;
    if (!_expect(parse, BC_TOK_KW_IF)) {
        return false;
    }
    if (!bc_parse_expression(parse, &if_->expr)) {
        return false;
    }
    if (!bc_parse_block(parse, &if_->main)) {
        return false;
    }
    if (_accept(parse, BC_TOK_KW_ELIF)) {
        if_->elifs = _ALLOC_NODE(struct bc_ast_elif_list);
        struct bc_ast_elif_list* curr = if_->elifs;
        curr->next = NULL;
        if (!bc_parse_expression(parse, &curr->elif.expr)) {
            return false;
        }
        if (!bc_parse_block(parse, &curr->elif.block)) {
            return false;
        }
        while (_accept(parse, BC_TOK_KW_ELIF)) {
            curr->next = _ALLOC_NODE(struct bc_ast_elif_list);
            curr->next->next = NULL;
            curr = curr->next;
            if (!bc_parse_expression(parse, &curr->elif.expr)) {
                return false;
            }
            if (!bc_parse_block(parse, &curr->elif.block)) {
                return false;
            }
        }
    }
    if (_accept(parse, BC_TOK_KW_ELSE)) {
        if_->else_ = _ALLOC_NODE(struct bc_ast_block);
        if (!bc_parse_block(parse, if_->else_)) {
            return false;
        }
    }
    return true;
}

bool bc_parse_switchcase(
    struct bc_parse* parse, struct bc_ast_switchcase* switchcase) {
    if (_accept(parse, BC_TOK_KW_DEFAULT)) {
        switchcase->expr = NULL;
    } else {
        switchcase->expr = _ALLOC_NODE(struct bc_ast_expr);
        if (!bc_parse_expression(parse, switchcase->expr)) {
            return false;
        }
    }
    if (!_expect(parse, BC_TOK_EQRAN)) {
        return false;
    }
    if (!bc_parse_block(parse, &switchcase->block)) {
        return false;
    }
    return true;
}

bool bc_parse_switch(struct bc_parse* parse, struct bc_ast_switch* switch_) {
    switch_->cases = NULL;
    if (!_expect(parse, BC_TOK_KW_SWITCH)) {
        return false;
    }
    if (!bc_parse_expression(parse, &switch_->expr)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_LBRACE)) {
        return false;
    }
    if (!_curr(parse, BC_TOK_RBRACE)) {
        switch_->cases = _ALLOC_NODE(struct bc_ast_switchcase_list);
        struct bc_ast_switchcase_list* curr = switch_->cases;
        curr->next = NULL;
        if (!bc_parse_switchcase(parse, &curr->case_)) {
            return false;
        }
        while (!_curr(parse, BC_TOK_RBRACE)) {
            curr->next = _ALLOC_NODE(struct bc_ast_switchcase_list);
            curr->next->next = NULL;
            curr = curr->next;
            if (!bc_parse_switchcase(parse, &curr->case_)) {
                return false;
            }
        }
    }
    if (!_expect(parse, BC_TOK_RBRACE)) {
        return false;
    }
    return true;
}

bool bc_parse_loop(struct bc_parse* parse, struct bc_ast_loop* loop) {
    if (!_expect(parse, BC_TOK_KW_LOOP)) {
        return false;
    }
    if (!bc_parse_block(parse, &loop->block)) {
        return false;
    }
    return true;
}

bool bc_parse_for(struct bc_parse* parse, struct bc_ast_for* for_) {
    if (!_expect(parse, BC_TOK_KW_FOR)) {
        return false;
    }
    struct bc_tok tok;
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    for_->name = tok.val.ident;
    if (!_expect(parse, BC_TOK_COLON)) {
        return false;
    }
    if (!bc_parse_expression(parse, &for_->expr)) {
        return false;
    }
    if (!bc_parse_block(parse, &for_->block)) {
        return false;
    }
    return true;
}

bool bc_parse_while(struct bc_parse* parse, struct bc_ast_while* while_) {
    if (!_expect(parse, BC_TOK_KW_WHILE)) {
        return false;
    }
    if (!bc_parse_expression(parse, &while_->expr)) {
        return false;
    }
    if (!bc_parse_block(parse, &while_->block)) {
        return false;
    }
    return true;
}

bool bc_parse_return(struct bc_parse* parse, struct bc_ast_return* return_) {
    return_->expr = NULL;
    if (!_expect(parse, BC_TOK_KW_RETURN)) {
        return false;
    }
    if (_accept(parse, BC_TOK_SEMICOLON)) {
        return true;
    }
    return_->expr = _ALLOC_NODE(struct bc_ast_expr);
    if (!bc_parse_expression(parse, return_->expr)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }
    return true;
}

bool bc_parse_defer(struct bc_parse* parse, struct bc_ast_defer* defer) {
    if (!_expect(parse, BC_TOK_KW_DEFER)) {
        return false;
    }
    if (!bc_parse_expression(parse, &defer->callee)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_LPAREN)) {
        return false;
    }
    if (!_curr(parse, BC_TOK_RPAREN)) {
        defer->args = _ALLOC_NODE(struct bc_ast_expr_list);
        if (!bc_parse_expression_list(parse, BC_TOK_RPAREN, defer->args)) {
            return false;
        }
    }
    if (!_expect(parse, BC_TOK_LPAREN)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }
    return true;
}

bool bc_parse_stmt(struct bc_parse* parse, struct bc_ast_stmt* stmt) {
    while (_accept(parse, BC_TOK_SEMICOLON))
        ;
    if (_curr(parse, BC_TOK_KW_LET)) {
        stmt->kind = BC_AST_STMT_LET;
        stmt->val.let = _ALLOC_NODE(struct bc_ast_let);
        if (!bc_parse_let(parse, stmt->val.let)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_IF)) {
        stmt->kind = BC_AST_STMT_IF;
        stmt->val.if_ = _ALLOC_NODE(struct bc_ast_if);
        if (!bc_parse_if(parse, stmt->val.if_)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_SWITCH)) {
        stmt->kind = BC_AST_STMT_SWITCH;
        stmt->val.switch_ = _ALLOC_NODE(struct bc_ast_switch);
        if (!bc_parse_switch(parse, stmt->val.switch_)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_LOOP)) {
        stmt->kind = BC_AST_STMT_LOOP;
        stmt->val.loop = _ALLOC_NODE(struct bc_ast_loop);
        if (!bc_parse_loop(parse, stmt->val.loop)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_FOR)) {
        stmt->kind = BC_AST_STMT_FOR;
        stmt->val.for_ = _ALLOC_NODE(struct bc_ast_for);
        if (!bc_parse_for(parse, stmt->val.for_)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_WHILE)) {
        stmt->kind = BC_AST_STMT_WHILE;
        stmt->val.while_ = _ALLOC_NODE(struct bc_ast_while);
        if (!bc_parse_while(parse, stmt->val.while_)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_RETURN)) {
        stmt->kind = BC_AST_STMT_RETURN;
        stmt->val.return_ = _ALLOC_NODE(struct bc_ast_return);
        if (!bc_parse_return(parse, stmt->val.return_)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_KW_DEFER)) {
        stmt->kind = BC_AST_STMT_DEFER;
        stmt->val.defer = _ALLOC_NODE(struct bc_ast_defer);
        if (!bc_parse_defer(parse, stmt->val.defer)) {
            return false;
        }
    } else if (_curr(parse, BC_TOK_LBRACE)) {
        stmt->kind = BC_AST_STMT_BLOCK;
        stmt->val.block = _ALLOC_NODE(struct bc_ast_block);
        if (!bc_parse_block(parse, stmt->val.block)) {
            return false;
        }
    } else if (_accept(parse, BC_TOK_KW_CONTINUE)) {
        stmt->kind = BC_AST_STMT_CONTINUE;
        if (!_expect(parse, BC_TOK_SEMICOLON)) {
            return false;
        }
    } else if (_accept(parse, BC_TOK_KW_BREAK)) {
        stmt->kind = BC_AST_STMT_BREAK;
        if (!_expect(parse, BC_TOK_SEMICOLON)) {
            return false;
        }
    } else {
        stmt->kind = BC_AST_STMT_EXPR;
        stmt->val.expr = _ALLOC_NODE(struct bc_ast_expr);
        if (!bc_parse_expression(parse, stmt->val.expr)) {
            return false;
        }
        if (!_expect(parse, BC_TOK_SEMICOLON)) {
            return false;
        }
    }
    return true;
}

bool bc_parse_block(struct bc_parse* parse, struct bc_ast_block* block) {
    if (!_expect(parse, BC_TOK_LBRACE)) {
        return false;
    }
    if (!_curr(parse, BC_TOK_RBRACE)) {
        block->stmts = _ALLOC_NODE(struct bc_ast_stmt_list);
        struct bc_ast_stmt_list* curr = block->stmts;
        curr->next = NULL;
        if (!bc_parse_stmt(parse, &curr->stmt)) {
            return false;
        }
        while (!_curr(parse, BC_TOK_RBRACE)) {
            curr->next = _ALLOC_NODE(struct bc_ast_stmt_list);
            curr->next->next = NULL;
            if (!bc_parse_stmt(parse, &curr->next->stmt)) {
                return false;
            }
            curr = curr->next;
        }
    }
    if (!_expect(parse, BC_TOK_RBRACE)) {
        return false;
    }
    return true;
}

bool bc_parse_type_path(struct bc_parse* parse, struct bc_ast_type_path* path) {
    path->is_root = false;
    path->super_count = 0;
    path->segments = NULL;

    struct bc_ast_ident_list* curr = NULL;
    struct bc_tok tok;

    if (_accept(parse, BC_TOK_KW_ROOT)) {
        if (!_expect(parse, BC_TOK_COLCOL)) {
            return false;
        }
        path->is_root = true;
    }

    while (_accept(parse, BC_TOK_KW_SUPER) && !path->is_root) {
        if (!_expect(parse, BC_TOK_COLCOL)) {
            return false;
        }
        path->super_count++;
    }

    while (true) {
        if (_accept_get(parse, BC_TOK_IDENT, &tok)) {
            struct bc_ast_ident_list* new_segment =
                _ALLOC_NODE(struct bc_ast_ident_list);
            new_segment->segment = tok.val.ident;
            new_segment->next = NULL;
            if (path->segments == NULL) {
                path->segments = new_segment;
                curr = new_segment;
            } else {
                curr->next = new_segment;
                curr = new_segment;
            }

            if (!_accept(parse, BC_TOK_COLCOL)) {
                break;
            }
        }
    }

    return true;
}

bool bc_parse_type_func(struct bc_parse* parse, struct bc_ast_type_func* func) {
    func->params = NULL;
    func->ret.kind = BC_AST_TYPE_UNIT;
    if (!_expect(parse, BC_TOK_KW_FUNC)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_LPAREN)) {
        return false;
    }
    if (!_curr(parse, BC_TOK_RPAREN)) {
        struct bc_ast_type_list* param = _ALLOC_NODE(struct bc_ast_type_list);
        param->next = NULL;
        func->params = param;
        if (!bc_parse_type(parse, &param->type)) {
            return false;
        }
        while (_accept(parse, BC_TOK_COMMA)) {
            if (_curr(parse, BC_TOK_RPAREN)) {
                break;
            }
            param->next = _ALLOC_NODE(struct bc_ast_type_list);
            param->next->next = NULL;
            if (!bc_parse_type(parse, &param->next->type)) {
                return false;
            }
            param = param->next;
        }
    }
    if (!_expect(parse, BC_TOK_RPAREN)) {
        return false;
    }
    if (_accept(parse, BC_TOK_DASHRAN)) {
        if (!bc_parse_type(parse, &func->ret)) {
            return false;
        }
    }
    return true;
}

bool bc_parse_type(struct bc_parse* parse, struct bc_ast_type* type) {
    if (_accept(parse, BC_TOK_KW_STRING)) {
        type->kind = BC_AST_TYPE_STRING;
        return true;
    } else if (_accept(parse, BC_TOK_KW_CHAR)) {
        type->kind = BC_AST_TYPE_CHAR;
        return true;
    } else if (_accept(parse, BC_TOK_KW_INT)) {
        type->kind = BC_AST_TYPE_INT;
        return true;
    } else if (_accept(parse, BC_TOK_KW_BYTE)) {
        type->kind = BC_AST_TYPE_BYTE;
        return true;
    } else if (_accept(parse, BC_TOK_KW_FLOAT)) {
        type->kind = BC_AST_TYPE_FLOAT;
        return true;
    } else if (_accept(parse, BC_TOK_KW_BOOL)) {
        type->kind = BC_AST_TYPE_BOOL;
        return true;
    } else if (_accept(parse, BC_TOK_LBRACKET)) {
        type->kind = BC_AST_TYPE_ARRAY;
        type->val.arr = _ALLOC_NODE(struct bc_ast_type);
        if (!bc_parse_type(parse, type->val.arr)) {
            return false;
        }
        if (!_expect(parse, BC_TOK_RBRACKET)) {
            return false;
        }
        return true;
    } else if (_accept(parse, BC_TOK_KW_TUP)) {
        type->kind = BC_AST_TYPE_TUP;
        type->val.tup = _ALLOC_NODE(struct bc_ast_type_list);
        if (_expect(parse, BC_TOK_LPAREN)) {
            return false;
        }
        struct bc_ast_type_list* curr = type->val.tup;
        curr->next = NULL;
        if (!bc_parse_type(parse, &curr->type)) {
            return false;
        }
        while (_accept(parse, BC_TOK_COMMA)) {
            if (_curr(parse, BC_TOK_RPAREN)) {
                break;
            }
            curr->next = _ALLOC_NODE(struct bc_ast_type_list);
            curr->next->next = NULL;
            if (!bc_parse_type(parse, &curr->next->type)) {
                return false;
            }
            curr = curr->next;
        }
        if (!_expect(parse, BC_TOK_RPAREN)) {
            return false;
        }
        return true;
    } else if (_curr(parse, BC_TOK_KW_FUNC)) {
        type->kind = BC_AST_TYPE_FUNC;
        if (!bc_parse_type_func(parse, type->val.func)) {
            return false;
        }
        return true;
    } else if (_curr(parse, BC_TOK_IDENT) || _curr(parse, BC_TOK_KW_ROOT) ||
               _curr(parse, BC_TOK_KW_SUPER)) {
        type->kind = BC_AST_TYPE_PATH;
        if (!bc_parse_type_path(parse, type->val.path)) {
            return false;
        }
    }

    _ERR_EXPECTED_MULTIPLE("type");

    return false;
}

bool bc_parse_func_param(
    struct bc_parse* parse, struct bc_ast_func_param* param) {
    struct bc_tok tok = { 0 };
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    param->name = tok.val.ident;

    if (!_expect(parse, BC_TOK_COLON)) {
        return false;
    }

    return bc_parse_type(parse, &param->type);
}

bool bc_parse_func(struct bc_parse* parse, struct bc_ast_func* func) {
    func->params = NULL;
    func->ret.kind = BC_AST_TYPE_UNIT;
    if (!_expect(parse, BC_TOK_KW_FUNC)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_LPAREN)) {
        return false;
    }
    if (_curr(parse, BC_TOK_IDENT)) {
        struct bc_ast_func_param_list* param =
            _ALLOC_NODE(struct bc_ast_func_param_list);
        param->next = NULL;
        func->params = param;
        if (!bc_parse_func_param(parse, &param->param)) {
            return false;
        }
        while (_accept(parse, BC_TOK_COMMA)) {
            if (_curr(parse, BC_TOK_RPAREN)) {
                break;
            }
            param->next = _ALLOC_NODE(struct bc_ast_func_param_list);
            param->next->next = NULL;
            if (!bc_parse_func_param(parse, &param->next->param)) {
                return false;
            }
            param = param->next;
        }
    }
    if (!_expect(parse, BC_TOK_RPAREN)) {
        return false;
    }
    if (_accept(parse, BC_TOK_DASHRAN)) {
        if (!bc_parse_type(parse, &func->ret)) {
            return false;
        }
    }
    if (!bc_parse_block(parse, &func->block)) {
        return false;
    }
    return true;
}

bool bc_parse_struct_item(
    struct bc_parse* parse, struct bc_ast_struct_item* item) {
    struct bc_tok tok;
    item->is_export = false;
    if (_accept(parse, BC_TOK_KW_EXPORT)) {
        item->is_export = true;
    }
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    item->name = tok.val.ident;
    if (!_expect(parse, BC_TOK_COLON)) {
        return false;
    }
    return bc_parse_type(parse, &item->type);
}

bool bc_parse_struct_decl(
    struct bc_parse* parse, struct bc_ast_struct* struct_) {
    if (!_expect(parse, BC_TOK_KW_STRUCT)) {
        return false;
    }

    if (!_expect(parse, BC_TOK_LBRACE)) {
        return false;
    }

    struct_->items = NULL;
    if (_curr(parse, BC_TOK_KW_EXPORT) || _curr(parse, BC_TOK_IDENT)) {
        struct_->items = _ALLOC_NODE(struct bc_ast_struct_item_list);
        struct bc_ast_struct_item_list* curr = struct_->items;
        curr->next = NULL;
        if (!bc_parse_struct_item(parse, &curr->item)) {
            return false;
        }
        while (_accept(parse, BC_TOK_COMMA)) {
            if (_curr(parse, BC_TOK_RBRACE)) {
                break;
            }
            curr->next = _ALLOC_NODE(struct bc_ast_struct_item_list);
            curr->next->next = NULL;
            if (!bc_parse_struct_item(parse, &curr->next->item)) {
                return false;
            }
            curr = curr->next;
        }
    }

    if (!_expect(parse, BC_TOK_RBRACE)) {
        return false;
    }

    return true;
}

bool bc_parse_enum_decl(struct bc_parse* parse, struct bc_ast_enum* enum_) {
    struct bc_tok tok;
    _accept(parse, BC_TOK_KW_EXPORT);
    if (!_expect(parse, BC_TOK_KW_ENUM)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_LBRACE)) {
        return false;
    }
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    enum_->items = _ALLOC_NODE(struct bc_ast_ident_list);
    struct bc_ast_ident_list* curr = enum_->items;
    curr->next = NULL;
    curr->segment = tok.val.ident;
    while (_accept(parse, BC_TOK_COMMA)) {
        if (_curr(parse, BC_TOK_RBRACE)) {
            break;
        }
        if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
            break;
        }
        curr->next = _ALLOC_NODE(struct bc_ast_ident_list);
        curr->next->next = NULL;
        curr->next->segment = tok.val.ident;
        curr = curr->next;
    }
    if (!_expect(parse, BC_TOK_RBRACE)) {
        return false;
    }

    return true;
}

bool bc_parse_type_alias(struct bc_parse* parse, struct bc_ast_type* type) {
    _accept(parse, BC_TOK_KW_EXPORT);
    if (!_expect(parse, BC_TOK_KW_TYPE)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_EQ)) {
        return false;
    }
    if (!bc_parse_type(parse, type)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }
    return true;
}

bool bc_parse_const_decl(struct bc_parse* parse, struct bc_ast_const* const_) {
    if (!_expect(parse, BC_TOK_KW_CONST)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_COLON)) {
        return false;
    }
    if (!bc_parse_type(parse, &const_->type)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_EQ)) {
        return false;
    }
    if (!bc_parse_expression(parse, &const_->expr)) {
        return false;
    }
    if (!_expect(parse, BC_TOK_SEMICOLON)) {
        return false;
    }
    return true;
}

bool bc_parse_decl(struct bc_parse* parse, struct bc_ast_decl* decl) {
    decl->is_export = false;
    if (_accept(parse, BC_TOK_KW_EXPORT)) {
        decl->is_export = true;
    }

    struct bc_tok tok;
    if (!_expect_get(parse, BC_TOK_IDENT, &tok)) {
        return false;
    }
    decl->name = tok.val.ident;

    if (!_expect(parse, BC_TOK_COLCOL)) {
        return false;
    }

    if (_curr(parse, BC_TOK_KW_FUNC)) {
        decl->kind = BC_AST_DECL_FUNC;
        return bc_parse_func(parse, &decl->val.func);
    } else if (_curr(parse, BC_TOK_KW_STRUCT)) {
        decl->kind = BC_AST_DECL_STRUCT;
        return bc_parse_struct_decl(parse, &decl->val.struct_);
    } else if (_curr(parse, BC_TOK_KW_ENUM)) {
        decl->kind = BC_AST_DECL_ENUM;
        return bc_parse_enum_decl(parse, &decl->val.enum_);
    } else if (_curr(parse, BC_TOK_KW_TYPE)) {
        decl->kind = BC_AST_DECL_TYPE_ALIAS;
        return bc_parse_type_alias(parse, &decl->val.type_alias);
    } else if (_curr(parse, BC_TOK_KW_CONST)) {
        decl->kind = BC_AST_DECL_CONST;
        return bc_parse_const_decl(parse, &decl->val.const_);
    }

    _ERR_EXPECTED_MULTIPLE("`func`", "`struct`", "`enum`", "`type`", "`const`");

    return false;
}

bool bc_parse_top_level_item(
    struct bc_parse* parse, struct bc_ast_top_level* top_level) {
    if (_curr(parse, BC_TOK_KW_IMPORT)) {
        top_level->kind = BC_AST_TOP_LEVEL_IMPORT;
        return bc_parse_import(parse, &top_level->val.import);
    }
    if (_curr(parse, BC_TOK_IDENT) || _curr(parse, BC_TOK_KW_EXPORT)) {
        top_level->kind = BC_AST_TOP_LEVEL_DECL;
        return bc_parse_decl(parse, &top_level->val.decl);
    }

    _ERR_EXPECTED_MULTIPLE("import statement", "declaration");

    return false;
}

bool bc_parse_top_level_list(
    struct bc_parse* parse, struct bc_ast_top_level_list* list) {
    if (!bc_parse_top_level_item(parse, &list->top_level)) {
        return false;
    }
    list->next = NULL;
    while (_curr(parse, BC_TOK_KW_IMPORT) || _curr(parse, BC_TOK_IDENT) ||
           _curr(parse, BC_TOK_KW_EXPORT)) {
        struct bc_ast_top_level_list* next =
            _ALLOC_NODE(struct bc_ast_top_level_list);
        bc_parse_top_level_item(parse, &next->top_level);
        list->next = next;
        list = next;
    }
    return true;
}

bool bc_parse_module(struct bc_parse* parse, struct bc_ast_module* module) {
    module->top_level_items = NULL;
    if (!_curr(parse, BC_TOK_EOF)) {
        module->top_level_items = _ALLOC_NODE(struct bc_ast_top_level_list);
        bc_parse_top_level_list(parse, module->top_level_items);
    }
    return true;
}
