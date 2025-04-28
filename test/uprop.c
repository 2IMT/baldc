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

TESTS(test_is_space, test_is_alpha, test_is_upper, test_is_lower, test_is_digit)
