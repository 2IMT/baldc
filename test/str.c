#include "tools/test.h"

#include <string.h>
#include <stddef.h>

#include "str.h"

TEST_BEGIN(test_strv_from_cstr) {
    struct bc_strv strv = bc_strv_from_cstr("Hello World!");
    TEST_ASSERT(strv.len == 12);

    strv = bc_strv_from_cstr("");
    TEST_ASSERT(strv.len == 0);

    TEST_END;
}

TEST_BEGIN(test_strv_from_cstrn) {
    struct bc_strv strv = bc_strv_from_cstrn("Hello World!", 5);
    TEST_ASSERT(strv.len == 5);

    TEST_END;
}

TEST_BEGIN(test_strv_from_range) {
    const char* hello_world = "Hello World! Goodbye World!";
    const char* goodbye_world = hello_world + 12;
    struct bc_strv strv = bc_strv_from_range(hello_world, goodbye_world);
    TEST_ASSERT(strv.len == 12);

    TEST_END;
}

TEST_BEGIN(test_strv_from_str) {
    struct bc_str str = {
        .data = (char*)25,
        .len = 1337,
        .cap = 12,
    };
    struct bc_strv strv = bc_strv_from_str(str);
    TEST_ASSERT(strv.data == (char*)25);
    TEST_ASSERT(strv.len == 1337);

    TEST_END;
}

TEST_BEGIN(test_strv_eq) {
    struct bc_strv strv1 = bc_strv_from_cstr("Hello World!");
    struct bc_strv strv2 = bc_strv_from_cstr("Goodbye World!");
    TEST_ASSERT(!bc_strv_eq(strv1, strv2));
    strv2 = bc_strv_from_cstr("");
    TEST_ASSERT(!bc_strv_eq(strv1, strv2));
    strv1 = bc_strv_from_cstr("");
    TEST_ASSERT(bc_strv_eq(strv1, strv2));
    strv1 = bc_strv_from_cstr("Hello");
    strv2 = bc_strv_from_cstr("Hello");
    TEST_ASSERT(bc_strv_eq(strv1, strv2));

    TEST_END;
}

TEST_BEGIN(test_strv_eq_str) {
    struct bc_strv strv = bc_strv_from_cstr("Hello World!");
    struct bc_str str = bc_str_from_cstr("Hello World");
    TEST_ASSERT(!bc_strv_eq_str(strv, str));
    strv = bc_strv_from_cstr("Hello World");
    TEST_ASSERT(bc_strv_eq_str(strv, str));
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_strv_eq_cstr) {
    struct bc_strv strv = bc_strv_from_cstr("Hello World!");
    TEST_ASSERT(bc_strv_eq_cstr(strv, "Hello World!"));
    TEST_ASSERT(!bc_strv_eq_cstr(strv, ""));
    strv = bc_strv_from_cstr("");
    TEST_ASSERT(bc_strv_eq_cstr(strv, ""));

    TEST_END;
}

TEST_BEGIN(test_strv_eq_cstrn) {
    struct bc_strv strv = bc_strv_from_cstr("Hello World!");
    TEST_ASSERT(bc_strv_eq_cstrn(strv, "Hello World! 123", 12));
    TEST_ASSERT(!bc_strv_eq_cstrn(strv, "Hello World!", 5));
    strv = bc_strv_from_cstr("");
    TEST_ASSERT(bc_strv_eq_cstrn(strv, "", 0));

    TEST_END;
}

TEST_BEGIN(test_str_from_cstr) {
    struct bc_str str = bc_str_from_cstr("Hello World!");
    TEST_ASSERT(str.len == 12);
    TEST_ASSERT(memcmp("Hello World!", str.data, 12) == 0);
    bc_str_free(str);

    str = bc_str_from_cstr("");
    TEST_ASSERT(str.len == 0);
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_from_cstrn) {
    struct bc_str str = bc_str_from_cstrn("Hello World!", 5);
    TEST_ASSERT(str.len == 5);
    TEST_ASSERT(memcmp("Hello", str.data, 5) == 0);
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_from_range) {
    const char* hello_world = "Hello World! Goodbye World!";
    const char* goodbye_world = hello_world + 12;
    struct bc_str str = bc_str_from_range(hello_world, goodbye_world);
    TEST_ASSERT(str.len == 12);
    TEST_ASSERT(memcmp("Hello World!", str.data, 12) == 0);
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_from_strv) {
    struct bc_strv strv = bc_strv_from_cstr("Hello Goodbye!");
    struct bc_str str = bc_str_from_strv(strv);
    TEST_ASSERT(strv.len == str.len);
    TEST_ASSERT(strv.data != str.data);
    TEST_ASSERT(memcmp(str.data, strv.data, strv.len) == 0);
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_reserve) {
    struct bc_str str = bc_str_new();
    bc_str_reserve(&str, 200);
    TEST_ASSERT(str.data != NULL);
    TEST_ASSERT(str.len == 0);
    TEST_ASSERT(str.cap >= 200);
    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_clone) {
    struct bc_str str1 = bc_str_from_cstr("Hello World!");
    struct bc_str str2 = bc_str_clone(str1);
    TEST_ASSERT(str1.data != str2.data);
    TEST_ASSERT(str1.len == str2.len);
    TEST_ASSERT(str1.cap == str2.cap);
    bc_str_free(str1);
    bc_str_free(str2);

    TEST_END;
}

TEST_BEGIN(test_str_eq) {
    struct bc_str str1 = bc_str_from_cstr("Hello World!");
    struct bc_str str2 = bc_str_from_cstr("Hello world");
    struct bc_str str3 = bc_str_from_cstr("");
    struct bc_str str4 = bc_str_from_cstr("");
    struct bc_str str5 = bc_str_from_cstr("Hello World!");
    TEST_ASSERT(!bc_str_eq(str1, str2));
    TEST_ASSERT(!bc_str_eq(str2, str1));
    TEST_ASSERT(!bc_str_eq(str1, str3));
    TEST_ASSERT(!bc_str_eq(str3, str1));
    TEST_ASSERT(bc_str_eq(str3, str4));
    TEST_ASSERT(bc_str_eq(str4, str3));
    TEST_ASSERT(bc_str_eq(str1, str5));
    TEST_ASSERT(bc_str_eq(str5, str1));
    bc_str_free(str1);
    bc_str_free(str2);
    bc_str_free(str3);
    bc_str_free(str4);
    bc_str_free(str5);

    TEST_END;
}

TEST_BEGIN(test_str_eq_strv) {
    struct bc_str str1 = bc_str_from_cstr("Hello World!");
    struct bc_str str2 = bc_str_from_cstr("Hello world");
    struct bc_str str3 = bc_str_from_cstr("");
    struct bc_str str4 = bc_str_from_cstr("");
    struct bc_str str5 = bc_str_from_cstr("Hello World!");
    struct bc_strv strv1 = bc_strv_from_cstr("Hello World!");
    struct bc_strv strv2 = bc_strv_from_cstr("Hello world");
    struct bc_strv strv3 = bc_strv_from_cstr("");
    struct bc_strv strv4 = bc_strv_from_cstr("");
    struct bc_strv strv5 = bc_strv_from_cstr("Hello World!");
    TEST_ASSERT(!bc_str_eq_strv(str1, strv2));
    TEST_ASSERT(!bc_str_eq_strv(str2, strv1));
    TEST_ASSERT(!bc_str_eq_strv(str1, strv3));
    TEST_ASSERT(!bc_str_eq_strv(str3, strv1));
    TEST_ASSERT(bc_str_eq_strv(str3, strv4));
    TEST_ASSERT(bc_str_eq_strv(str4, strv3));
    TEST_ASSERT(bc_str_eq_strv(str1, strv5));
    TEST_ASSERT(bc_str_eq_strv(str5, strv1));
    bc_str_free(str1);
    bc_str_free(str2);
    bc_str_free(str3);
    bc_str_free(str4);
    bc_str_free(str5);

    TEST_END;
}

TEST_BEGIN(test_str_eq_cstr) {
    struct bc_str str1 = bc_str_from_cstr("Hello World!");
    struct bc_str str2 = bc_str_from_cstr("Hello world");
    struct bc_str str3 = bc_str_from_cstr("");
    struct bc_str str4 = bc_str_from_cstr("");
    struct bc_str str5 = bc_str_from_cstr("Hello World!");
    const char* cstr1 = "Hello World!";
    const char* cstr2 = "Hello world";
    const char* cstr3 = "";
    const char* cstr4 = "";
    const char* cstr5 = "Hello World!";
    TEST_ASSERT(!bc_str_eq_cstr(str1, cstr2));
    TEST_ASSERT(!bc_str_eq_cstr(str2, cstr1));
    TEST_ASSERT(!bc_str_eq_cstr(str1, cstr3));
    TEST_ASSERT(!bc_str_eq_cstr(str3, cstr1));
    TEST_ASSERT(bc_str_eq_cstr(str3, cstr4));
    TEST_ASSERT(bc_str_eq_cstr(str4, cstr3));
    TEST_ASSERT(bc_str_eq_cstr(str1, cstr5));
    TEST_ASSERT(bc_str_eq_cstr(str5, cstr1));
    bc_str_free(str1);
    bc_str_free(str2);
    bc_str_free(str3);
    bc_str_free(str4);
    bc_str_free(str5);

    TEST_END;
}

TEST_BEGIN(test_str_eq_cstrn) {
    struct bc_str str1 = bc_str_from_cstr("Hello World!");
    struct bc_str str2 = bc_str_from_cstr("Hello world");
    struct bc_str str3 = bc_str_from_cstr("");
    struct bc_str str4 = bc_str_from_cstr("");
    struct bc_str str5 = bc_str_from_cstr("Hello World!");
    const char* cstr1 = "Hello World!";
    const char* cstr2 = "Hello world";
    const char* cstr3 = "";
    const char* cstr4 = "";
    const char* cstr5 = "Hello World!";
    TEST_ASSERT(!bc_str_eq_cstrn(str1, cstr2, 11));
    TEST_ASSERT(!bc_str_eq_cstrn(str2, cstr1, 11));
    TEST_ASSERT(!bc_str_eq_cstrn(str1, cstr3, 0));
    TEST_ASSERT(bc_str_eq_cstrn(str3, cstr1, 0));
    TEST_ASSERT(bc_str_eq_cstrn(str3, cstr4, 0));
    TEST_ASSERT(bc_str_eq_cstrn(str4, cstr3, 0));
    TEST_ASSERT(bc_str_eq_cstrn(str1, cstr5, 12));
    TEST_ASSERT(bc_str_eq_cstrn(str5, cstr1, 12));
    TEST_ASSERT(!bc_str_eq_cstrn(str1, cstr5, 10));
    TEST_ASSERT(!bc_str_eq_cstrn(str5, cstr1, 10));
    bc_str_free(str1);
    bc_str_free(str2);
    bc_str_free(str3);
    bc_str_free(str4);
    bc_str_free(str5);

    TEST_END;
}

TEST_BEGIN(test_str_push_cstr) {
    struct bc_str str = bc_str_new();

    bc_str_push_cstr(&str, "");
    TEST_ASSERT(str.len == 0);

    const char* res = "Hello World!";
    size_t res_len = 12;
    bc_str_push_cstr(&str, res);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    res = "Hello World! Goodbye World!";
    res_len = 27;
    bc_str_push_cstr(&str, " Goodbye World!");
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_push_cstr(&str, "");
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_push_cstrn) {
    struct bc_str str = bc_str_new();

    bc_str_push_cstrn(&str, "", 0);
    TEST_ASSERT(str.len == 0);

    bc_str_push_cstrn(&str, "test!", 0);
    TEST_ASSERT(str.len == 0);

    const char* res = "Hello World!";
    size_t res_len = 12;
    bc_str_push_cstr(&str, res);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    res = "Hello World! Goodbye World";
    res_len = 26;
    bc_str_push_cstrn(&str, " Goodbye World!", 14);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_push_cstrn(&str, " Hello!", 0);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_push_cstrn(&str, "", 0);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_push_ch) {
    struct bc_str str = bc_str_new();

    const char* res = "A";
    size_t res_len = 1;
    bc_str_push_ch(&str, L'A');
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    res = "AB";
    res_len = 2;
    bc_str_push_ch(&str, L'B');
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    res = "ABП";
    res_len = 4;
    bc_str_push_ch(&str, L'П');
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_push_cch) {
    struct bc_str str = bc_str_new();

    const char* res = "A";
    size_t res_len = 1;
    bc_str_push_cch(&str, 'A');
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    res = "AB";
    res_len = 2;
    bc_str_push_cch(&str, 'B');
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res, str.data, res_len) == 0);

    bc_str_free(str);

    TEST_END;
}

TEST_BEGIN(test_str_push_strv) {
    struct bc_str str = bc_str_new();

    bc_str_push_strv(&str, bc_strv_from_cstr(""));
    TEST_ASSERT(str.len == 0);

    struct bc_strv res = bc_strv_from_cstr("Hello World!");
    size_t res_len = 12;
    bc_str_push_strv(&str, res);
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res.data, str.data, res_len) == 0);

    res = bc_strv_from_cstr("Hello World! Goodbye World!");
    res_len = 27;
    bc_str_push_strv(&str, bc_strv_from_cstr(" Goodbye World!"));
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res.data, str.data, res_len) == 0);

    bc_str_push_strv(&str, bc_strv_from_cstr(""));
    TEST_ASSERT(str.len == res_len);
    TEST_ASSERT(memcmp(res.data, str.data, res_len) == 0);

    bc_str_free(str);

    TEST_END;
}

TESTS(test_strv_from_cstr, test_strv_from_cstrn, test_strv_from_range,
    test_strv_from_str, test_strv_eq, test_strv_eq_str, test_strv_eq_cstr,
    test_strv_eq_cstrn, test_str_from_cstr, test_str_from_cstrn,
    test_str_from_range, test_str_reserve, test_str_clone, test_str_from_strv,
    test_str_eq, test_str_eq_strv, test_str_eq_cstr, test_str_eq_cstrn,
    test_str_push_cstr, test_str_push_cstrn, test_str_push_ch,
    test_str_push_cch, test_str_push_strv)
