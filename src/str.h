#ifndef BALDC_STR_H
#define BALDC_STR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct bc_strv {
    const char* data;
    size_t len;
};

#define BC_STRV_INIT ((struct bc_strv) { .data = NULL, .len = 0 })

#define BC_STRV_FORMAT "%.*s"
#define BC_STRV_FORMATV(v) (int)(v).len, (v).data

struct bc_str {
    char* data;
    size_t len;
    size_t cap;
};

#define BC_STR_INIT ((struct bc_str) { .data = NULL, .len = 0, .cap = 0 })

#define BC_STR_FORMAT "%.*s"
#define BC_STR_FORMATV(v) (int)(v).len, (v).data

struct bc_strv_iter {
    struct bc_strv str;
    size_t pos;
};

#define BC_STRV_ITER_INIT ((struct bc_strv_iter) { .str = BC_STRV_INIT, .pos = 0 })

enum bc_strv_iter_res {
    BC_STRV_ITER_ERR = -1,
    BC_STRV_ITER_OK,
    BC_STRV_ITER_EMPTY,
};

struct bc_strv bc_strv_from(const char* str);

struct bc_strv bc_strv_fromn(const char* str, size_t n);

void bc_str_reserve(struct bc_str* v, size_t cap);

struct bc_str bc_str_clone(const struct bc_str* v);

void bc_str_free(struct bc_str* v);

void bc_str_push(struct bc_str* v, const char* str);

void bc_str_pushn(struct bc_str* v, const char* str, size_t n);

bool bc_str_pushc(struct bc_str* v, int32_t c);

void bc_str_pushv(struct bc_str* v, struct bc_strv view);

struct bc_strv bc_str_to_strv(const struct bc_str* v);

struct bc_strv_iter bc_strv_iter_from(const struct bc_strv* v);

enum bc_strv_iter_res bc_strv_iter_next(struct bc_strv_iter* i, int32_t* c);

#endif
