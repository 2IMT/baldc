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

struct bc_strv bc_strv_from_range(const char* begin, const char* end);

#define BC_STRV_FROM_LIT(literal) \
    bc_strv_from_cstrn(literal, sizeof(literal) - 1)

struct bc_strv bc_strv_from_str(struct bc_str v);

bool bc_strv_eq(struct bc_strv l, struct bc_strv r);

bool bc_strv_eq_str(struct bc_strv l, struct bc_str r);

bool bc_strv_eq_cstr(struct bc_strv l, const char* r);

bool bc_strv_eq_cstrn(struct bc_strv l, const char* r, size_t n);

#define BC_STRV_EQ_LIT(l, r_literal) \
    bc_strv_eq_cstrn(l, r_literal, sizeof(r_literal) - 1)

struct bc_str bc_str_new(void);

struct bc_str bc_str_from_cstr(const char* str);

struct bc_str bc_str_from_cstrn(const char* str, size_t n);

struct bc_str bc_str_from_range(const char* begin, const char* end);

#define BC_STR_FROM_LIT(literal) bc_str_from_cstrn(literal, sizeof(literal) - 1)

struct bc_str bc_str_from_strv(struct bc_strv v);

void bc_str_reserve(struct bc_str* v, size_t cap);

struct bc_str bc_str_clone(struct bc_str v);

void bc_str_free(struct bc_str v);

bool bc_str_eq(struct bc_str l, struct bc_str r);

bool bc_str_eq_strv(struct bc_str l, struct bc_strv r);

bool bc_str_eq_cstr(struct bc_str l, const char* r);

bool bc_str_eq_cstrn(struct bc_str l, const char* r, size_t n);

#define BC_STR_EQ_LIT(l, r_literal) \
    bc_str_eq_cstrn(l, r_literal, sizeof(r_literal) - 1)

void bc_str_push_cstr(struct bc_str* v, const char* str);

void bc_str_push_cstrn(struct bc_str* v, const char* str, size_t n);

bool bc_str_push_ch(struct bc_str* v, int32_t c);

void bc_str_push_cch(struct bc_str* v, char c);

void bc_str_push_strv(struct bc_str* v, struct bc_strv view);

void bc_str_push_cstr_unchecked(struct bc_str* v, const char* str);

void bc_str_push_cstrn_unchecked(struct bc_str* v, const char* str, size_t n);

bool bc_str_push_ch_unchecked(struct bc_str* v, int32_t c);

void bc_str_push_cch_unchecked(struct bc_str* v, char c);

void bc_str_push_strv_unchecked(struct bc_str* v, struct bc_strv view);

struct bc_strv_iter bc_strv_iter_new(struct bc_strv v);

enum bc_strv_iter_res bc_strv_iter_next(struct bc_strv_iter* i, int32_t* c);

#endif
