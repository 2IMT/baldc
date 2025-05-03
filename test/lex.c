#include "tools/test.h"

#include "lex.h"
#include "str.h"

#define _CREATE_LEXER(src) \
    struct bc_lex _lex = bc_lex_new(BC_STRV_FROM_LIT(src)); \
    struct bc_tok _tok;

#define _RESET_LEXER(src) \
    bc_lex_free(_lex); \
    _lex = bc_lex_new(BC_STRV_FROM_LIT(src));

#define _FREE_LEXER() bc_lex_free(_lex);

#define _NEXT() \
    _tok = bc_lex_next(&_lex); \
    TEST_ASSERT(_tok.kind != BC_TOK_ERR); \
    TEST_ASSERT(_tok.kind != BC_TOK_EOF);

#define _NEXT_EMPTY() \
    _tok = bc_lex_next(&_lex); \
    TEST_ASSERT(_tok.kind == BC_TOK_EOF);

#define _NEXT_ERROR() \
    _tok = bc_lex_next(&_lex); \
    TEST_ASSERT(_tok.kind == BC_TOK_ERR);

#define _ASSERT_EMPTY() TEST_ASSERT(_res == BC_LEX_EMPTY);

#define _ASSERT_LOCATION(ls, cs, le, ce) \
    TEST_ASSERT(_tok.loc.s.l == ls); \
    TEST_ASSERT(_tok.loc.s.c == cs); \
    TEST_ASSERT(_tok.loc.e.l == le); \
    TEST_ASSERT(_tok.loc.e.c == ce);

#define _ASSERT_KIND(token_kind) TEST_ASSERT(_tok.kind == token_kind);

#define _ASSERT_IDENT_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_IDENT); \
    TEST_ASSERT(bc_strv_eq_cstr(_tok.val.ident, value));

#define _ASSERT_STRING_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_STRING); \
    TEST_ASSERT(bc_strv_eq(_tok.val.string, BC_STRV_FROM_LIT(value)));

#define _ASSERT_CHARACTER_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_CHARACTER); \
    TEST_ASSERT(bc_strv_eq(_tok.val.character, BC_STRV_FROM_LIT(value)));

#define _ASSERT_INTEGER_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_INTEGER); \
    TEST_ASSERT(bc_strv_eq(_tok.val.integer, BC_STRV_FROM_LIT(value)));

#define _ASSERT_BYTE_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_BYTE); \
    TEST_ASSERT(bc_strv_eq(_tok.val.byte, BC_STRV_FROM_LIT(value)));

#define _ASSERT_FLOATING_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_FLOATING); \
    TEST_ASSERT(bc_strv_eq(_tok.val.floating, BC_STRV_FROM_LIT(value)));

#define _ASSERT_BOOLEAN_VALUE(value) \
    TEST_ASSERT(_tok.kind == BC_TOK_LIT_BOOLEAN); \
    TEST_ASSERT(_tok.val.boolean == value);

#define _NEXT_LOCATION(ls, cs, le, ce) \
    _NEXT(); \
    _ASSERT_LOCATION(ls, cs, le, ce);

#define _NEXT_KIND(token_kind) \
    _NEXT(); \
    _ASSERT_KIND(token_kind);

#define _NEXT_IDENT_VALUE(value) \
    _NEXT(); \
    _ASSERT_IDENT_VALUE(value);

#define _NEXT_STRING_VALUE(value) \
    _NEXT(); \
    _ASSERT_STRING_VALUE(value);

#define _NEXT_CHARACTER_VALUE(value) \
    _NEXT(); \
    _ASSERT_CHARACTER_VALUE(value);

#define _NEXT_INTEGER_VALUE(value) \
    _NEXT(); \
    _ASSERT_INTEGER_VALUE(value);

#define _NEXT_BYTE_VALUE(value) \
    _NEXT(); \
    _ASSERT_BYTE_VALUE(value);

#define _NEXT_FLOATING_VALUE(value) \
    _NEXT(); \
    _ASSERT_FLOATING_VALUE(value);

#define _NEXT_BOOLEAN_VALUE(value) \
    _NEXT(); \
    _ASSERT_BOOLEAN_VALUE(value);

TEST_BEGIN(test_empty) {

    _CREATE_LEXER("");
    _NEXT_EMPTY();
    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_identifiers) {

    _CREATE_LEXER("Hello world1 _world _____world");
    _NEXT_IDENT_VALUE("Hello");
    _ASSERT_LOCATION(1, 1, 1, 6);
    _NEXT_IDENT_VALUE("world1");
    _ASSERT_LOCATION(1, 7, 1, 13);
    _NEXT_IDENT_VALUE("_world");
    _ASSERT_LOCATION(1, 14, 1, 20);
    _NEXT_IDENT_VALUE("_____world");
    _ASSERT_LOCATION(1, 21, 1, 31);
    _NEXT_EMPTY();

    _RESET_LEXER("\nhello\n\nhola");
    _NEXT_IDENT_VALUE("hello");
    _ASSERT_LOCATION(2, 1, 2, 6);
    _NEXT_IDENT_VALUE("hola");
    _ASSERT_LOCATION(4, 1, 4, 5);
    _NEXT_EMPTY();

    _RESET_LEXER("hello truee falsee");
    _NEXT_IDENT_VALUE("hello");
    _ASSERT_LOCATION(1, 1, 1, 6);
    _NEXT_IDENT_VALUE("truee");
    _ASSERT_LOCATION(1, 7, 1, 12);
    _NEXT_IDENT_VALUE("falsee");
    _ASSERT_LOCATION(1, 13, 1, 19);
    _NEXT_EMPTY();

    _RESET_LEXER("GПривет world1 _woПривет прив_ _____world")
    _NEXT_IDENT_VALUE("GПривет");
    _ASSERT_LOCATION(1, 1, 1, 8);
    _NEXT_IDENT_VALUE("world1");
    _ASSERT_LOCATION(1, 9, 1, 15);
    _NEXT_IDENT_VALUE("_woПривет");
    _ASSERT_LOCATION(1, 16, 1, 25);
    _NEXT_IDENT_VALUE("прив_");
    _ASSERT_LOCATION(1, 26, 1, 31);
    _NEXT_IDENT_VALUE("_____world");
    _ASSERT_LOCATION(1, 32, 1, 42);
    _NEXT_EMPTY();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_strings) {

    _CREATE_LEXER("\"\" \"Hello World\\n\"");
    _NEXT_STRING_VALUE("");
    _ASSERT_LOCATION(1, 1, 1, 3);
    _NEXT_STRING_VALUE("Hello World\n");
    _ASSERT_LOCATION(1, 4, 1, 19);
    _NEXT_EMPTY();

    _RESET_LEXER("\n\"\\xAB\"");
    _NEXT_STRING_VALUE("\u00AB");
    _ASSERT_LOCATION(2, 1, 2, 7);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Tab:\\t Backslash:\\\\ Quote:\\\"\"");
    _NEXT_STRING_VALUE("Tab:\t Backslash:\\ Quote:\"");
    _ASSERT_LOCATION(1, 1, 1, 31);
    _NEXT_EMPTY();

    _RESET_LEXER("\"First line\\nSecond line\"");
    _NEXT_STRING_VALUE("First line\nSecond line");
    _ASSERT_LOCATION(1, 1, 1, 26);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Single quote: \\'\"");
    _NEXT_STRING_VALUE("Single quote: '");
    _ASSERT_LOCATION(1, 1, 1, 19);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Carriage return\\rEnd\"");
    _NEXT_STRING_VALUE("Carriage return\rEnd");
    _ASSERT_LOCATION(1, 1, 1, 23);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Tab\\t End\"");
    _NEXT_STRING_VALUE("Tab\t End");
    _ASSERT_LOCATION(1, 1, 1, 12);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Hex escape\\xAB\"");
    _NEXT_STRING_VALUE("Hex escape\u00AB");
    _ASSERT_LOCATION(1, 1, 1, 17);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Unicode\\u1234\"");
    _NEXT_STRING_VALUE("Unicode\u1234");
    _ASSERT_LOCATION(1, 1, 1, 16);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Null char\\0End\"");
    _NEXT_STRING_VALUE("Null char\0End");
    _ASSERT_LOCATION(1, 1, 1, 17);
    _NEXT_EMPTY();

    _RESET_LEXER("\"Invalid hex\\xGZ\"");
    _NEXT_ERROR();

    _RESET_LEXER("\"Invalid unicode\\uXYZ\"");
    _NEXT_ERROR();

    _RESET_LEXER("\"Invalid long unicode\\UXY123\"");
    _NEXT_ERROR();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_characters) {

    _CREATE_LEXER("''");
    _NEXT_ERROR();

    _RESET_LEXER("'A'");
    _NEXT_CHARACTER_VALUE("A");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_EMPTY();

    _RESET_LEXER("'\\n' '\\r' '\\t' '\\0' '\\\\' '\\'' '\"' '\\\"'");
    _NEXT_CHARACTER_VALUE("\n");
    _ASSERT_LOCATION(1, 1, 1, 5);
    _NEXT_CHARACTER_VALUE("\r");
    _ASSERT_LOCATION(1, 6, 1, 10);
    _NEXT_CHARACTER_VALUE("\t");
    _ASSERT_LOCATION(1, 11, 1, 15);
    _NEXT_CHARACTER_VALUE("\0");
    _ASSERT_LOCATION(1, 16, 1, 20);
    _NEXT_CHARACTER_VALUE("\\");
    _ASSERT_LOCATION(1, 21, 1, 25);
    _NEXT_CHARACTER_VALUE("'");
    _ASSERT_LOCATION(1, 26, 1, 30);
    _NEXT_CHARACTER_VALUE("\"");
    _ASSERT_LOCATION(1, 31, 1, 34);
    _NEXT_CHARACTER_VALUE("\"");
    _ASSERT_LOCATION(1, 35, 1, 39);
    _NEXT_EMPTY();

    _RESET_LEXER("'\\u03A9'");
    _NEXT_CHARACTER_VALUE("\u03A9");
    _ASSERT_LOCATION(1, 1, 1, 9);
    _NEXT_EMPTY();

    _RESET_LEXER("'\\u01f6'");
    _NEXT_CHARACTER_VALUE("\u01f6");
    _ASSERT_LOCATION(1, 1, 1, 9);
    _NEXT_EMPTY();

    _RESET_LEXER("'B' '\\n' '\"' 'Z'");
    _NEXT_CHARACTER_VALUE("B");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_CHARACTER_VALUE("\n");
    _ASSERT_LOCATION(1, 5, 1, 9);
    _NEXT_CHARACTER_VALUE("\"");
    _ASSERT_LOCATION(1, 10, 1, 13);
    _NEXT_CHARACTER_VALUE("Z");
    _ASSERT_LOCATION(1, 14, 1, 17);
    _NEXT_EMPTY();

    _RESET_LEXER("'X'\n'\\t'\n'\\u4F60'");
    _NEXT_CHARACTER_VALUE("X");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_CHARACTER_VALUE("\t");
    _ASSERT_LOCATION(2, 1, 2, 5);
    _NEXT_CHARACTER_VALUE("\u4F60");
    _ASSERT_LOCATION(3, 1, 3, 9);
    _NEXT_EMPTY();

    _RESET_LEXER("'\\xGZ'");
    _NEXT_ERROR();

    _RESET_LEXER("'\\u12'");
    _NEXT_ERROR();

    _RESET_LEXER("'\\uXYZW'");
    _NEXT_ERROR();

    _RESET_LEXER("'\\x1'");
    _NEXT_ERROR();

    _RESET_LEXER("'A");
    _NEXT_ERROR();

    _RESET_LEXER("'\\n");
    _NEXT_ERROR();

    _RESET_LEXER("'A' 'B' \"String\" '\\n'");
    _NEXT_CHARACTER_VALUE("A");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_CHARACTER_VALUE("B");
    _ASSERT_LOCATION(1, 5, 1, 8);
    _NEXT_STRING_VALUE("String");
    _ASSERT_LOCATION(1, 9, 1, 17);
    _NEXT_CHARACTER_VALUE("\n");
    _ASSERT_LOCATION(1, 18, 1, 22);
    _NEXT_EMPTY();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_integers) {

    _CREATE_LEXER("0 123 0xFF 0Xff 0b1010 0B0101 0o777 0O123");
    _NEXT_INTEGER_VALUE("0");
    _ASSERT_LOCATION(1, 1, 1, 2);
    _NEXT_INTEGER_VALUE("123");
    _ASSERT_LOCATION(1, 3, 1, 6);
    _NEXT_INTEGER_VALUE("0xFF");
    _ASSERT_LOCATION(1, 7, 1, 11);
    _NEXT_INTEGER_VALUE("0Xff");
    _ASSERT_LOCATION(1, 12, 1, 16);
    _NEXT_INTEGER_VALUE("0b1010");
    _ASSERT_LOCATION(1, 17, 1, 23);
    _NEXT_INTEGER_VALUE("0B0101");
    _ASSERT_LOCATION(1, 24, 1, 30);
    _NEXT_INTEGER_VALUE("0o777");
    _ASSERT_LOCATION(1, 31, 1, 36);
    _NEXT_INTEGER_VALUE("0O123");
    _ASSERT_LOCATION(1, 37, 1, 42);
    _NEXT_EMPTY();

    _RESET_LEXER("123abc");
    _NEXT_ERROR();

    _RESET_LEXER("0z123");
    _NEXT_ERROR();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_bytes) {

    _CREATE_LEXER("42y 0xFFy 0b101y 0o77y");
    _NEXT_BYTE_VALUE("42y");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_BYTE_VALUE("0xFFy");
    _ASSERT_LOCATION(1, 5, 1, 10);
    _NEXT_BYTE_VALUE("0b101y");
    _ASSERT_LOCATION(1, 11, 1, 17);
    _NEXT_BYTE_VALUE("0o77y");
    _ASSERT_LOCATION(1, 18, 1, 23);
    _NEXT_EMPTY();

    _RESET_LEXER("0xABcdy");
    _NEXT_BYTE_VALUE("0xABcdy");
    _ASSERT_LOCATION(1, 1, 1, 8);
    _NEXT_EMPTY();

    _RESET_LEXER("3.14y");
    _NEXT_ERROR();

    _RESET_LEXER("-42y");
    _NEXT_ERROR();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_floats) {
    _CREATE_LEXER("0.0 1.1 123.456");
    _NEXT_FLOATING_VALUE("0.0");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_FLOATING_VALUE("1.1");
    _ASSERT_LOCATION(1, 5, 1, 8);
    _NEXT_FLOATING_VALUE("123.456");
    _ASSERT_LOCATION(1, 9, 1, 16);
    _NEXT_EMPTY();

    _RESET_LEXER(".1");
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 1, 1, 2);
    _NEXT_INTEGER_VALUE("1");
    _ASSERT_LOCATION(1, 2, 1, 3);
    _NEXT_EMPTY();

    _RESET_LEXER("1.");
    _NEXT_INTEGER_VALUE("1");
    _ASSERT_LOCATION(1, 1, 1, 2);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 2, 1, 3);
    _NEXT_EMPTY();

    _RESET_LEXER(".");
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 1, 1, 2);
    _NEXT_EMPTY();

    _RESET_LEXER("1.1.1");
    _NEXT_FLOATING_VALUE("1.1");
    _ASSERT_LOCATION(1, 1, 1, 4);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 4, 1, 5);
    _NEXT_INTEGER_VALUE("1");
    _ASSERT_LOCATION(1, 5, 1, 6);
    _NEXT_EMPTY();

    _RESET_LEXER("3.14pi");
    _NEXT_ERROR();

    _RESET_LEXER("1.23f");
    _NEXT_ERROR();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_numbers) {

    _CREATE_LEXER("x.1 .y 2..3 4.5z");
    _NEXT_IDENT_VALUE("x");
    _ASSERT_LOCATION(1, 1, 1, 2);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 2, 1, 3);
    _NEXT_INTEGER_VALUE("1");
    _ASSERT_LOCATION(1, 3, 1, 4);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 5, 1, 6);
    _NEXT_IDENT_VALUE("y");
    _ASSERT_LOCATION(1, 6, 1, 7);
    _NEXT_INTEGER_VALUE("2");
    _ASSERT_LOCATION(1, 8, 1, 9);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 9, 1, 10);
    _NEXT_KIND(BC_TOK_DOT);
    _ASSERT_LOCATION(1, 10, 1, 11);
    _NEXT_INTEGER_VALUE("3");
    _ASSERT_LOCATION(1, 11, 1, 12);
    _NEXT_ERROR();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_booleans) {

    _CREATE_LEXER(" true  false\n");
    _NEXT_BOOLEAN_VALUE(true);
    _ASSERT_LOCATION(1, 2, 1, 6);
    _NEXT_BOOLEAN_VALUE(false);
    _ASSERT_LOCATION(1, 8, 1, 13);

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_keywords) {

    _CREATE_LEXER("string char int byte float\n"
                  "bool tup unit import export\n"
                  "root super as try_as struct\n"
                  "enum func type const defer let if\n"
                  "elif else loop for while\n"
                  "switch default break\n"
                  "continue return\n");

    _NEXT_KIND(BC_TOK_KW_STRING);
    _ASSERT_LOCATION(1, 1, 1, 7);
    _NEXT_KIND(BC_TOK_KW_CHAR);
    _ASSERT_LOCATION(1, 8, 1, 12);
    _NEXT_KIND(BC_TOK_KW_INT);
    _ASSERT_LOCATION(1, 13, 1, 16);
    _NEXT_KIND(BC_TOK_KW_BYTE);
    _ASSERT_LOCATION(1, 17, 1, 21);
    _NEXT_KIND(BC_TOK_KW_FLOAT);
    _ASSERT_LOCATION(1, 22, 1, 27);

    _NEXT_KIND(BC_TOK_KW_BOOL);
    _ASSERT_LOCATION(2, 1, 2, 5);
    _NEXT_KIND(BC_TOK_KW_TUP);
    _ASSERT_LOCATION(2, 6, 2, 9);
    _NEXT_KIND(BC_TOK_KW_UNIT);
    _ASSERT_LOCATION(2, 10, 2, 14);
    _NEXT_KIND(BC_TOK_KW_IMPORT);
    _ASSERT_LOCATION(2, 15, 2, 21);
    _NEXT_KIND(BC_TOK_KW_EXPORT);
    _ASSERT_LOCATION(2, 22, 2, 28);

    _NEXT_KIND(BC_TOK_KW_ROOT);
    _ASSERT_LOCATION(3, 1, 3, 5);
    _NEXT_KIND(BC_TOK_KW_SUPER);
    _ASSERT_LOCATION(3, 6, 3, 11);
    _NEXT_KIND(BC_TOK_KW_AS);
    _ASSERT_LOCATION(3, 12, 3, 14);
    _NEXT_KIND(BC_TOK_KW_TRYAS);
    _ASSERT_LOCATION(3, 15, 3, 21);
    _NEXT_KIND(BC_TOK_KW_STRUCT);
    _ASSERT_LOCATION(3, 22, 3, 28);

    _NEXT_KIND(BC_TOK_KW_ENUM);
    _ASSERT_LOCATION(4, 1, 4, 5);
    _NEXT_KIND(BC_TOK_KW_FUNC);
    _ASSERT_LOCATION(4, 6, 4, 10);
    _NEXT_KIND(BC_TOK_KW_TYPE);
    _ASSERT_LOCATION(4, 11, 4, 15);
    _NEXT_KIND(BC_TOK_KW_CONST);
    _ASSERT_LOCATION(4, 16, 4, 21);
    _NEXT_KIND(BC_TOK_KW_DEFER);
    _ASSERT_LOCATION(4, 22, 4, 27);
    _NEXT_KIND(BC_TOK_KW_LET);
    _ASSERT_LOCATION(4, 28, 4, 31);
    _NEXT_KIND(BC_TOK_KW_IF);
    _ASSERT_LOCATION(4, 32, 4, 34);

    _NEXT_KIND(BC_TOK_KW_ELIF);
    _ASSERT_LOCATION(5, 1, 5, 5);
    _NEXT_KIND(BC_TOK_KW_ELSE);
    _ASSERT_LOCATION(5, 6, 5, 10);
    _NEXT_KIND(BC_TOK_KW_LOOP);
    _ASSERT_LOCATION(5, 11, 5, 15);
    _NEXT_KIND(BC_TOK_KW_FOR);
    _ASSERT_LOCATION(5, 16, 5, 19);
    _NEXT_KIND(BC_TOK_KW_WHILE);
    _ASSERT_LOCATION(5, 20, 5, 25);

    _NEXT_KIND(BC_TOK_KW_SWITCH);
    _ASSERT_LOCATION(6, 1, 6, 7);
    _NEXT_KIND(BC_TOK_KW_DEFAULT);
    _ASSERT_LOCATION(6, 8, 6, 15);
    _NEXT_KIND(BC_TOK_KW_BREAK);
    _ASSERT_LOCATION(6, 16, 6, 21);

    _NEXT_KIND(BC_TOK_KW_CONTINUE);
    _ASSERT_LOCATION(7, 1, 7, 9);
    _NEXT_KIND(BC_TOK_KW_RETURN);
    _ASSERT_LOCATION(7, 10, 7, 16);

    _NEXT_EMPTY();

    _FREE_LEXER()

    TEST_END;
}

TEST_BEGIN(test_symbols) {

    _CREATE_LEXER("( ) { } [ ] < <= <<\n"
                  "<<= > >= >> >>= ! !=\n"
                  ": :: . , ; = == => +\n"
                  "+= - -= -> * *= / /=\n"
                  "& &= && | |= || ^ ^=\n"
                  "~ % %= ?\n");

    _NEXT_KIND(BC_TOK_LPAREN);
    _NEXT_KIND(BC_TOK_RPAREN);
    _NEXT_KIND(BC_TOK_LBRACE);
    _NEXT_KIND(BC_TOK_RBRACE);
    _NEXT_KIND(BC_TOK_LBRACKET);
    _NEXT_KIND(BC_TOK_RBRACKET);
    _NEXT_KIND(BC_TOK_LANGLE);
    _NEXT_KIND(BC_TOK_LANEQ);
    _NEXT_KIND(BC_TOK_LANLAN);

    _NEXT_KIND(BC_TOK_LANLANEQ);
    _NEXT_KIND(BC_TOK_RANGLE);
    _NEXT_KIND(BC_TOK_RANEQ);
    _NEXT_KIND(BC_TOK_RANRAN);
    _NEXT_KIND(BC_TOK_RANRANEQ);
    _NEXT_KIND(BC_TOK_EXCLAM);
    _NEXT_KIND(BC_TOK_EXCLEQ);

    _NEXT_KIND(BC_TOK_COLON);
    _NEXT_KIND(BC_TOK_COLCOL);
    _NEXT_KIND(BC_TOK_DOT);
    _NEXT_KIND(BC_TOK_COMMA);
    _NEXT_KIND(BC_TOK_SEMICOLON);
    _NEXT_KIND(BC_TOK_EQ);
    _NEXT_KIND(BC_TOK_EQEQ);
    _NEXT_KIND(BC_TOK_EQRAN);
    _NEXT_KIND(BC_TOK_PLUS);

    _NEXT_KIND(BC_TOK_PLUSEQ);
    _NEXT_KIND(BC_TOK_DASH);
    _NEXT_KIND(BC_TOK_DASHEQ);
    _NEXT_KIND(BC_TOK_DASHRAN);
    _NEXT_KIND(BC_TOK_STAR);
    _NEXT_KIND(BC_TOK_STAREQ);
    _NEXT_KIND(BC_TOK_SLASH);
    _NEXT_KIND(BC_TOK_SLASHEQ);

    _NEXT_KIND(BC_TOK_AMP);
    _NEXT_KIND(BC_TOK_AMPEQ);
    _NEXT_KIND(BC_TOK_AMPAMP);
    _NEXT_KIND(BC_TOK_PIPE);
    _NEXT_KIND(BC_TOK_PIPEEQ);
    _NEXT_KIND(BC_TOK_PIPEPIPE);
    _NEXT_KIND(BC_TOK_CARET);
    _NEXT_KIND(BC_TOK_CARETEQ);

    _NEXT_KIND(BC_TOK_TILDE);
    _NEXT_KIND(BC_TOK_PERCENT);
    _NEXT_KIND(BC_TOK_PERCENTEQ);
    _NEXT_KIND(BC_TOK_QUESTION);

    _NEXT_EMPTY();

    _RESET_LEXER("(){}[]<<=<<\n"
                 "<<=>>=>>>>=!!=\n"
                 ":::.,;====>+\n"
                 "+=--=->**=//=\n"
                 "&&=&&||=||^^=\n"
                 "~%%=?\n");

    _NEXT_KIND(BC_TOK_LPAREN);
    _NEXT_KIND(BC_TOK_RPAREN);
    _NEXT_KIND(BC_TOK_LBRACE);
    _NEXT_KIND(BC_TOK_RBRACE);
    _NEXT_KIND(BC_TOK_LBRACKET);
    _NEXT_KIND(BC_TOK_RBRACKET);
    _NEXT_KIND(BC_TOK_LANLANEQ);
    _NEXT_KIND(BC_TOK_LANLAN);

    _NEXT_KIND(BC_TOK_LANLANEQ);
    _NEXT_KIND(BC_TOK_RANRANEQ);
    _NEXT_KIND(BC_TOK_RANRAN);
    _NEXT_KIND(BC_TOK_RANRANEQ);
    _NEXT_KIND(BC_TOK_EXCLAM);
    _NEXT_KIND(BC_TOK_EXCLEQ);

    _NEXT_KIND(BC_TOK_COLCOL);
    _NEXT_KIND(BC_TOK_COLON);
    _NEXT_KIND(BC_TOK_DOT);
    _NEXT_KIND(BC_TOK_COMMA);
    _NEXT_KIND(BC_TOK_SEMICOLON);
    _NEXT_KIND(BC_TOK_EQEQ);
    _NEXT_KIND(BC_TOK_EQEQ);
    _NEXT_KIND(BC_TOK_RANGLE);
    _NEXT_KIND(BC_TOK_PLUS);

    _NEXT_KIND(BC_TOK_PLUSEQ);
    _NEXT_KIND(BC_TOK_DASH);
    _NEXT_KIND(BC_TOK_DASHEQ);
    _NEXT_KIND(BC_TOK_DASHRAN);
    _NEXT_KIND(BC_TOK_STAR);
    _NEXT_KIND(BC_TOK_STAREQ);

    _NEXT_KIND(BC_TOK_AMPAMP);
    _NEXT_KIND(BC_TOK_EQ);
    _NEXT_KIND(BC_TOK_AMPAMP);
    _NEXT_KIND(BC_TOK_PIPEPIPE);
    _NEXT_KIND(BC_TOK_EQ);
    _NEXT_KIND(BC_TOK_PIPEPIPE);
    _NEXT_KIND(BC_TOK_CARET);
    _NEXT_KIND(BC_TOK_CARETEQ);

    _NEXT_KIND(BC_TOK_TILDE);
    _NEXT_KIND(BC_TOK_PERCENT);
    _NEXT_KIND(BC_TOK_PERCENTEQ);
    _NEXT_KIND(BC_TOK_QUESTION);

    _NEXT_EMPTY();

    _FREE_LEXER();

    TEST_END;
}

TEST_BEGIN(test_recovery) {

    _CREATE_LEXER("import std::io;\nio::println(-10y); return 1;");
    _NEXT_KIND(BC_TOK_KW_IMPORT);
    _NEXT_IDENT_VALUE("std")
    _NEXT_KIND(BC_TOK_COLCOL);
    _NEXT_IDENT_VALUE("io");
    _NEXT_KIND(BC_TOK_SEMICOLON);
    _NEXT_IDENT_VALUE("io");
    _NEXT_KIND(BC_TOK_COLCOL);
    _NEXT_IDENT_VALUE("println");
    _NEXT_KIND(BC_TOK_LPAREN);
    _NEXT_ERROR();
    _NEXT_KIND(BC_TOK_RPAREN);
    _NEXT_KIND(BC_TOK_SEMICOLON);
    _NEXT_KIND(BC_TOK_KW_RETURN);
    _NEXT_INTEGER_VALUE("1");
    _NEXT_KIND(BC_TOK_SEMICOLON);
    _NEXT_EMPTY();

    _RESET_LEXER("hello world \"hello \0 world\" 25.25");
    _NEXT_IDENT_VALUE("hello")
    _NEXT_IDENT_VALUE("world")
    _NEXT_ERROR();
    _NEXT_FLOATING_VALUE("25.25");
    _NEXT_EMPTY();

    _RESET_LEXER("\"HE\0LLO\"");
    _NEXT_ERROR();
    _NEXT_EMPTY();

    _RESET_LEXER("break continue 0b2 break");
    _NEXT_KIND(BC_TOK_KW_BREAK);
    _NEXT_KIND(BC_TOK_KW_CONTINUE);
    _NEXT_ERROR();
    _NEXT_KIND(BC_TOK_KW_BREAK);
    _NEXT_EMPTY();

    _RESET_LEXER("break continue 0of break");
    _NEXT_KIND(BC_TOK_KW_BREAK);
    _NEXT_KIND(BC_TOK_KW_CONTINUE);
    _NEXT_ERROR();
    _NEXT_KIND(BC_TOK_KW_BREAK);
    _NEXT_EMPTY();

    _FREE_LEXER();

    TEST_END;
}

TESTS(test_empty, test_identifiers, test_strings, test_characters,
    test_integers, test_bytes, test_floats, test_numbers, test_booleans,
    test_keywords, test_symbols, test_recovery)
