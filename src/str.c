#include "str.h"

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "mem.h"
#include "utf8.h"
#include "assert.h"

static void _ensure_cap(struct bc_str* v, size_t atleast) {
    bc_str_reserve(v, atleast);
    while (v->cap < atleast) {
        bc_str_reserve(v, v->cap * 2);
    }
}

struct bc_strv bc_strv_from(const char* str) {
    size_t n = strlen(str);
    return bc_strv_fromn(str, n);
}

struct bc_strv bc_strv_fromn(const char* str, size_t n) {
    return (struct bc_strv) {
        .data = str,
        .len = n,
    };
}

void bc_str_reserve(struct bc_str* v, size_t cap) {
    if (v->cap >= cap) {
        return;
    }
    v->data = BC_REALLOC(v->data, cap);
    BC_ASSERT_NOT_NULL(v->data);
    v->cap = cap;
}

struct bc_str bc_str_clone(const struct bc_str* v) {
    struct bc_str res = {
        .len = v->len,
        .cap = v->cap,
        .data = BC_MALLOC(v->cap),
    };
    BC_ASSERT_NOT_NULL(res.data);
    memcpy(res.data, v->data, v->cap);
    return res;
}

void bc_str_free(struct bc_str* v) {
    BC_FREE(v->data);
    *v = BC_STR_INIT;
}

void bc_str_push(struct bc_str* v, const char* str) {
    size_t n = strlen(str);
    bc_str_pushn(v, str, n);
}

void bc_str_pushn(struct bc_str* v, const char* str, size_t n) {
    _ensure_cap(v, v->len + n);
    memcpy(v->data + v->len, str, n);
    v->len += n;
}

bool bc_str_pushc(struct bc_str* v, int32_t c) {
    _ensure_cap(v, v->len + 4);
    char chbytes[4] = { 0 };
    int n = bc_utf8_encode(c, chbytes);
    if (n == -1) {
        return false;
    }
    memcpy(v->data + v->len, chbytes, n);
    v->len += n;
    return true;
}

void bc_str_pushv(struct bc_str* v, struct bc_strv view) {
    bc_str_pushn(v, view.data, view.len);
}

struct bc_strv bc_str_to_strv(const struct bc_str* v) {
    return (struct bc_strv) {
        .data = v->data,
        .len = v->len,
    };
}

struct bc_strv_iter bc_strv_iter_from(const struct bc_strv* v) {
    return (struct bc_strv_iter) {
        .str = *v,
        .pos = 0,
    };
}

enum bc_strv_iter_res bc_strv_iter_next(struct bc_strv_iter* i, int32_t* c) {
    size_t remaining = i->str.len - i->pos;
    int32_t codepoint = 0;
    if (remaining == 0) {
        return BC_STRV_ITER_EMPTY;
    }
    int adv = bc_utf8_decode(i->str.data + i->pos, remaining, &codepoint);
    if (adv == -1) {
        return BC_STRV_ITER_ERR;
    }
    i->pos += adv;
    *c = codepoint;
    return BC_STRV_ITER_OK;
}
