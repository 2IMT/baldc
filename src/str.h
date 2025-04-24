#ifndef BALDC_STR_H
#define BALDC_STR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct bc_strv {
    const char* data;
    size_t len;
};

#define BC_STRV_FORMAT     "%.*s"
#define BC_STRV_FORMATV(v) (int)((v).len), (v).data

struct bc_str {
    char* data;
    size_t len;
    size_t cap;
};

#define BC_STR_FORMAT     "%.*s"
#define BC_STR_FORMATV(v) (int)((v).len), (v).data

struct bc_strv_iter {
    struct bc_strv str;
    size_t pos;
};

enum bc_strv_iter_res {
    BC_STRV_ITER_ERR = -1,
    BC_STRV_ITER_OK,
    BC_STRV_ITER_EMPTY,
};

struct bc_strv bc_strv_from_cstr(const char* str);

struct bc_strv bc_strv_from_cstrn(const char* str, size_t n);

#define BC_STRV_FROM_LIT(literal) \
    bc_strv_from_cstrn(literal, sizeof(literal) - 1)

struct bc_str bc_str_new();

struct bc_str bc_str_from_cstr(const char* str);

struct bc_str bc_str_from_cstrn(const char* str, size_t n);

#define BC_STR_FROM_LIT(literal) bc_str_from_cstrn(literal, sizeof(literal) - 1)

void bc_str_reserve(struct bc_str* v, size_t cap);

struct bc_str bc_str_clone(struct bc_str v);

void bc_str_free(struct bc_str* v);

void bc_str_push_cstr(struct bc_str* v, const char* str);

void bc_str_push_cstrn(struct bc_str* v, const char* str, size_t n);

bool bc_str_push_ch(struct bc_str* v, int32_t c);

void bc_str_push_strv(struct bc_str* v, struct bc_strv view);

struct bc_strv_iter bc_strv_iter_new(struct bc_strv v);

enum bc_strv_iter_res bc_strv_iter_next(struct bc_strv_iter* i, int32_t* c);

#endif
