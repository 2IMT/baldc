#include "tools/test.h"

#include "uprop.h"

#define _CASE_TRUE(codepoint) TEST_ASSERT(_FUNC(codepoint))

#define _CASE_FALSE(codepoint) TEST_ASSERT(!_FUNC(codepoint))

TEST_BEGIN(test_is_space) {

#define _FUNC bc_uprop_is_space

    _CASE_TRUE(0x20);
    _CASE_TRUE(0x9);
    _CASE_TRUE(0xA);
    _CASE_TRUE(0xD);
    _CASE_TRUE(0x2003);
    _CASE_TRUE(0x3000);

    _CASE_FALSE(0x41);
    _CASE_FALSE(0x61);
    _CASE_FALSE(0x30);
    _CASE_FALSE(0x2E);
    _CASE_FALSE(0x21);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_alpha) {

#define _FUNC bc_uprop_is_alpha

    _CASE_TRUE(0x41);
    _CASE_TRUE(0x61);
    _CASE_TRUE(0x3B1);
    _CASE_TRUE(0x391);
    _CASE_TRUE(0x4E2);
    _CASE_TRUE(0x1100);

    _CASE_FALSE(0x30);
    _CASE_FALSE(0x20);
    _CASE_FALSE(0x2D);
    _CASE_FALSE(0x2E);
    _CASE_FALSE(0x2000);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_alnum) {

#define _FUNC bc_uprop_is_alnum

    _CASE_TRUE('A');
    _CASE_TRUE('z');
    _CASE_TRUE(0x00C0);
    _CASE_TRUE(0x0391);
    _CASE_TRUE(0x0410);
    _CASE_TRUE(0x05D0);
    _CASE_TRUE(0x0627);
    _CASE_TRUE(0x3042);
    _CASE_TRUE(0x4E00);
    _CASE_TRUE(0x01C4);
    _CASE_TRUE(0x1E9E);
    _CASE_TRUE(0x212A);
    _CASE_TRUE(0x093E);
    _CASE_TRUE(0x30);
    _CASE_TRUE(0x31);
    _CASE_TRUE(0x32);
    _CASE_TRUE(0x33);
    _CASE_TRUE(0x34);
    _CASE_TRUE(0x35);
    _CASE_TRUE(0x36);
    _CASE_TRUE(0x37);
    _CASE_TRUE(0x38);
    _CASE_TRUE(0x39);

    _CASE_FALSE(0x0000);
    _CASE_FALSE(0x001F);
    _CASE_FALSE(0x007F);
    _CASE_FALSE(0x0660);
    _CASE_FALSE('!');
    _CASE_FALSE(0x3001);
    _CASE_FALSE('+');
    _CASE_FALSE(0x20AC);
    _CASE_FALSE(0xFFFE);
    _CASE_FALSE(0x10FFFF);
    _CASE_FALSE(0xD800);
    _CASE_FALSE(0xDFFF);
    _CASE_FALSE(0x200B);
    _CASE_FALSE(0xFEFF);
    _CASE_FALSE(0xE000);
    _CASE_FALSE(0xF8FF);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_upper) {

#define _FUNC bc_uprop_is_upper

    _CASE_TRUE(0x41);
    _CASE_TRUE(0x391);
    _CASE_TRUE(0x4E2);

    _CASE_FALSE(0x1100);
    _CASE_FALSE(0x61);
    _CASE_FALSE(0x3B1);
    _CASE_FALSE(0x30);
    _CASE_FALSE(0x20);
    _CASE_FALSE(0x2D);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_lower) {

#define _FUNC bc_uprop_is_lower

    _CASE_TRUE(0x61);
    _CASE_TRUE(0x3B1);
    _CASE_TRUE(0x7A);

    _CASE_FALSE(0x4E2);
    _CASE_FALSE(0x41);
    _CASE_FALSE(0x391);
    _CASE_FALSE(0x30);
    _CASE_FALSE(0x20);
    _CASE_FALSE(0x2D);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_digit) {

#define _FUNC bc_uprop_is_digit

    _CASE_TRUE(0x30);
    _CASE_TRUE(0x31);
    _CASE_TRUE(0x32);
    _CASE_TRUE(0x33);
    _CASE_TRUE(0x34);
    _CASE_TRUE(0x35);
    _CASE_TRUE(0x36);
    _CASE_TRUE(0x37);
    _CASE_TRUE(0x38);
    _CASE_TRUE(0x39);

    _CASE_FALSE(0x660);
    _CASE_FALSE(0x6F0);
    _CASE_FALSE(0x9D0);
    _CASE_FALSE(0x41);
    _CASE_FALSE(0x61);
    _CASE_FALSE(0x20);
    _CASE_FALSE(0x2E);
    _CASE_FALSE(0x2000);

    TEST_END;

#undef _FUNC
}

TEST_BEGIN(test_is_print) {

#define _FUNC bc_uprop_is_print

    _CASE_TRUE(0x20);
    _CASE_TRUE(0x7E);
    _CASE_TRUE(0xA0);
    _CASE_TRUE(0x00A9);
    _CASE_TRUE(0x03A9);
    _CASE_TRUE(0x1F600);
    _CASE_TRUE(0x1F4A9);
    _CASE_TRUE(0x3042);
    _CASE_TRUE(0x4E2D);

    _CASE_FALSE(0x00);
    _CASE_FALSE(0x1F);
    _CASE_FALSE(0x7F);
    _CASE_FALSE(0x80);
    _CASE_FALSE(0x9F);
    _CASE_FALSE(0xD800);
    _CASE_FALSE(0xDFFF);
    _CASE_FALSE(0xFDD0);
    _CASE_FALSE(0xFDEF);
    _CASE_FALSE(0xFFFE);
    _CASE_FALSE(0x1FFFE);
    _CASE_FALSE(0x10FFFE);
    _CASE_FALSE(0xFFFF);
    _CASE_FALSE(0x10FFFF);
    _CASE_FALSE(0xE000);
    _CASE_FALSE(0xF8FF);
    _CASE_FALSE(0xF0000);
    _CASE_FALSE(0xFFFFD);
    _CASE_FALSE(0x100000);
    _CASE_FALSE(0x10FFFD);
    _CASE_FALSE(0x200B);
    _CASE_FALSE(0x200C);
    _CASE_FALSE(0x200D);
    _CASE_FALSE(0x202A);
    _CASE_FALSE(0x2060);
    _CASE_FALSE(0xFEFF);
    _CASE_FALSE(0xFFF9);
    _CASE_FALSE(0x034F);
    _CASE_FALSE(0x17B4);
    _CASE_FALSE(0x206A);
    _CASE_FALSE(0x206F);

    TEST_END;

#undef _FUNC
}

TESTS(test_is_space, test_is_alpha, test_is_alnum, test_is_upper, test_is_lower,
    test_is_digit, test_is_print)
