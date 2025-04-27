#include "tools/test.h"

#include "utf8.h"

TEST_BEGIN(test_codepoint_len) {

#define _CASE(codepoint, len) \
    TEST_ASSERT(bc_utf8_codepoint_len(codepoint) == len)

#define _CASE_INVALID(codepoint) \
    TEST_ASSERT(bc_utf8_codepoint_len(codepoint) == -1)

    _CASE(0x00, 1);
    _CASE(0x41, 1);
    _CASE(0x7F, 1);

    _CASE(0x80, 2);
    _CASE(0x7FF, 2);

    _CASE(0x800, 3);
    _CASE(0xFFFF, 3);

    _CASE(0x10000, 4);
    _CASE(0x10FFFF, 4);

    _CASE(0x2A, 1);
    _CASE(0x400, 2);
    _CASE(0x1234, 3);

    _CASE(0x9, 1);
    _CASE(0x1F, 1);

    _CASE_INVALID(0x110000);
    _CASE_INVALID(-1);
    _CASE_INVALID(0xD800);
    _CASE_INVALID(0xDFFF);
    _CASE_INVALID(-2147483648);

    TEST_END;

#undef _CASE
#undef _CASE_INVALID
}

TEST_BEGIN(test_encode) {

#define _CASE(codepoint, str, len) \
    TEST_ASSERT( \
        bc_utf8_encode(codepoint, out) == len && memcmp(out, str, len) == 0)

#define _CASE_INVALID(codepoint) \
    TEST_ASSERT(bc_utf8_encode(codepoint, out) == -1)

    char out[4];

    _CASE(0x00, "\x00", 1);
    _CASE(0x41, "\x41", 1);
    _CASE(0x7F, "\x7F", 1);

    _CASE(0x80, "\xC2\x80", 2);
    _CASE(0x7FF, "\xDF\xBF", 2);

    _CASE(0x800, "\xE0\xA0\x80", 3);
    _CASE(0xFFFF, "\xEF\xBF\xBF", 3);

    _CASE(0x10000, "\xF0\x90\x80\x80", 4);
    _CASE(0x10FFFF, "\xF4\x8F\xBF\xBF", 4);

    _CASE(0x7F, "\x7F", 1);
    _CASE(0x80, "\xC2\x80", 2);
    _CASE(0x7FF, "\xDF\xBF", 2);
    _CASE(0x800, "\xE0\xA0\x80", 3);
    _CASE(0xFFFF, "\xEF\xBF\xBF", 3);
    _CASE(0x10000, "\xF0\x90\x80\x80", 4);
    _CASE(0x10FFFF, "\xF4\x8F\xBF\xBF", 4);

    _CASE(0x9, "\x09", 1);
    _CASE(0x1F, "\x1F", 1);

    _CASE_INVALID(0x110000);
    _CASE_INVALID(-1);
    _CASE_INVALID(0xD800);
    _CASE_INVALID(0xDFFF);

    TEST_END;

#undef _CASE
#undef _CASE_INVALID
}

TEST_BEGIN(test_decode) {

#define _CASE(str, len, codepoint) \
    TEST_ASSERT(bc_utf8_decode(str, len, &out) == len && out == codepoint)

#define _CASE_INVALID(str, len) \
    TEST_ASSERT(bc_utf8_decode(str, len, &out) == -1);

    int32_t out;

    _CASE("\x00", 1, 0x00);
    _CASE("\x41", 1, 0x41);
    _CASE("\x7F", 1, 0x7F);

    _CASE("\xC2\x80", 2, 0x80);
    _CASE("\xDF\xBF", 2, 0x7FF);

    _CASE("\xE0\xA0\x80", 3, 0x800);
    _CASE("\xEF\xBF\xBF", 3, 0xFFFF);

    _CASE("\xF0\x90\x80\x80", 4, 0x10000);
    _CASE("\xF4\x8F\xBF\xBF", 4, 0x10FFFF);

    _CASE("\x7F", 1, 0x7F);
    _CASE("\xC2\x80", 2, 0x80);
    _CASE("\xDF\xBF", 2, 0x7FF);
    _CASE("\xE0\xA0\x80", 3, 0x800);
    _CASE("\xEF\xBF\xBF", 3, 0xFFFF);
    _CASE("\xF0\x90\x80\x80", 4, 0x10000);
    _CASE("\xF4\x8F\xBF\xBF", 4, 0x10FFFF);

    _CASE("\x09", 1, 0x9);
    _CASE("\x1F", 1, 0x1F);

    _CASE_INVALID("\x80", 1);
    _CASE_INVALID("\xC0\x80", 2);
    _CASE_INVALID("\xF8\x80\x80\x80\x80", 5);
    _CASE_INVALID("\xF0\x90\x80", 3);
    _CASE_INVALID("\x80\x80", 2);
    _CASE_INVALID("\xED\xA0\x80", 3);
    _CASE_INVALID("\xED\xBF\xBF", 3);

    TEST_END;

#undef _CASE
#undef _CASE_INVALID
}

TESTS(test_codepoint_len, test_encode, test_decode)
