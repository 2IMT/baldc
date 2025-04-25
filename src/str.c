#include "str.h"

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "mem.h"
#include "utf8.h"

static void _ensure_cap(struct bc_str* v, size_t atleast) {
    bc_str_reserve(v, atleast);
    while (v->cap < atleast) {
        bc_str_reserve(v, v->cap * 2);
    }
}

struct bc_strv bc_strv_from_cstr(const char* str) {
    size_t n = strlen(str);
    return bc_strv_from_cstrn(str, n);
}

struct bc_strv bc_strv_from_cstrn(const char* str, size_t n) {
    return (struct bc_strv) {
        .data = str,
        .len = n,
    };
}

struct bc_strv bc_strv_from_range(const char* begin, const char* end) {
    return bc_strv_from_cstrn(begin, end - begin);
}

struct bc_strv bc_strv_from_str(struct bc_str v) {
    return (struct bc_strv) {
        .data = v.data,
        .len = v.len,
    };
}

bool bc_strv_eq(struct bc_strv l, struct bc_strv r) {
    if (l.len != r.len) {
        return false;
    }
    return memcmp(l.data, r.data, l.len) == 0;
}

bool bc_strv_eq_str(struct bc_strv l, struct bc_str r) {
    if (l.len != r.len) {
        return false;
    }
    return memcmp(l.data, r.data, l.len) == 0;
}

bool bc_strv_eq_cstr(struct bc_strv l, const char* r) {
    size_t n = strlen(r);
    return bc_strv_eq_cstrn(l, r, n);
}

bool bc_strv_eq_cstrn(struct bc_strv l, const char* r, size_t n) {
    if (l.len != n) {
        return false;
    }
    return memcmp(l.data, r, n) == 0;
}

struct bc_str bc_str_new(void) {
    return (struct bc_str) {
        .data = NULL,
        .cap = 0,
        .len = 0,
    };
}

struct bc_str bc_str_from_cstr(const char* str) {
    size_t n = strlen(str);
    return bc_str_from_cstrn(str, n);
}

struct bc_str bc_str_from_cstrn(const char* str, size_t n) {
    struct bc_str res = {
        .data = BC_MALLOC(n),
        .cap = n,
        .len = n,
    };
    bc_mem_check(res.data);
    memcpy(res.data, str, n);
    return res;
}

struct bc_str bc_str_from_range(const char* begin, const char* end) {
    return bc_str_from_cstrn(begin, end - begin);
}

struct bc_str bc_str_from_strv(struct bc_strv v) {
    return bc_str_from_cstrn(v.data, v.len);
}

void bc_str_reserve(struct bc_str* v, size_t cap) {
    if (v->cap >= cap) {
        return;
    }
    v->data = BC_REALLOC(v->data, cap);
    bc_mem_check(v->data);
    v->cap = cap;
}

struct bc_str bc_str_clone(struct bc_str v) {
    struct bc_str res = {
        .len = v.len,
        .cap = v.cap,
        .data = BC_MALLOC(v.cap),
    };
    bc_mem_check(res.data);
    memcpy(res.data, v.data, v.cap);
    return res;
}

void bc_str_free(struct bc_str* v) {
    if (v->data != NULL) {
        BC_FREE(v->data);
    }
    *v = bc_str_new();
}

bool bc_str_eq(struct bc_str l, struct bc_str r) {
    if (l.len != r.len) {
        return false;
    }
    return memcmp(l.data, r.data, l.len) == 0;
}

bool bc_str_eq_strv(struct bc_str l, struct bc_strv r) {
    if (l.len != r.len) {
        return false;
    }
    return memcmp(l.data, r.data, l.len) == 0;
}

bool bc_str_eq_cstr(struct bc_str l, const char* r) {
    size_t n = strlen(r);
    return bc_str_eq_cstrn(l, r, n);
}

bool bc_str_eq_cstrn(struct bc_str l, const char* r, size_t n);

void bc_str_push_cstr(struct bc_str* v, const char* str) {
    size_t n = strlen(str);
    bc_str_push_cstrn(v, str, n);
}

void bc_str_push_cstrn(struct bc_str* v, const char* str, size_t n) {
    _ensure_cap(v, v->len + n);
    memcpy(v->data + v->len, str, n);
    v->len += n;
}

bool bc_str_push_ch(struct bc_str* v, int32_t c) {
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

void bc_str_push_strv(struct bc_str* v, struct bc_strv view) {
    bc_str_push_cstrn(v, view.data, view.len);
}

struct bc_strv bc_str_to_strv(const struct bc_str* v) {
    return (struct bc_strv) {
        .data = v->data,
        .len = v->len,
    };
}

struct bc_strv_iter bc_strv_iter_new(struct bc_strv v) {
    return (struct bc_strv_iter) {
        .str = v,
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
