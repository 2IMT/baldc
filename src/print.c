#include "print.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>

#include "utf8.h"

static void _print_int(FILE* f, int64_t n) { fprintf(f, "%" PRId64, n); }

static void _print_uint(FILE* f, uint64_t n) { fprintf(f, "%" PRIu64, n); }

static void _print_size(FILE* f, size_t n) { fprintf(f, "%zu", n); }

static void _print_ch(FILE* f, int32_t c) {
    char buf[4] = { 0 };
    if (bc_utf8_encode(c, buf) != -1) {
        fprintf(f, "%s", buf);
    }
}

static void _print_cch(FILE* f, char c) { fprintf(f, "%c", c); }

static void _print_float(FILE* f, float v) { fprintf(f, "%f", v); }

static void _print_double(FILE* f, double v) { fprintf(f, "%lf", v); }

static void _print_cstr(FILE* f, const char* str) { fprintf(f, "%s", str); }

static void _print_cstrn(FILE* f, const char* str, size_t n) {
    fprintf(f, "%.*s", (int)n, str);
}

static void _print_nl(FILE* f) {
    fprintf(f, "\n");
    fflush(f);
}

static void _printf(FILE* f, const char* fmt, va_list args) {
    char curr = '\0';
    const char* last_arg_ptr = fmt;
    size_t last_arg_adv = 1;
    size_t pos = 0;

    while ((curr = fmt[pos]) != '\0') {
        if (curr == '$') {
            pos++;
            if ((curr = fmt[pos]) == '\0') {
                return;
            }
            _print_cstrn(f, last_arg_ptr, pos - last_arg_adv);

            switch (curr) {
            case 'i': {
                int64_t n = va_arg(args, int64_t);
                _print_int(f, n);
            } break;
            case 'u': {
                uint64_t n = va_arg(args, uint64_t);
                _print_uint(f, n);
            } break;
            case 'z': {
                size_t n = va_arg(args, size_t);
                _print_size(f, n);
            } break;
            case 'C': {
                int32_t c = va_arg(args, int32_t);
                _print_ch(f, c);
            } break;
            case 'c': {
                char c = (char)va_arg(args, int);
                _print_cch(f, c);
            } break;
            case 'f': {
                float v = (float)va_arg(args, double);
                _print_float(f, v);
            } break;
            case 'd': {
                double v = va_arg(args, double);
                _print_double(f, v);
            } break;
            case 's': {
                const char* str = va_arg(args, const char*);
                _print_cstr(f, str);
            } break;
            case 'S': {
                const char* str = va_arg(args, const char*);
                size_t n = va_arg(args, size_t);
                _print_cstrn(f, str, n);
            } break;
            case 'n': {
                _print_nl(f);
            } break;
            case '$': {
                _print_cch(f, '$');
            } break;
            }

            last_arg_ptr = fmt + pos + 1;
            pos++;
            if (fmt[pos] == '\0') {
                break;
            }
            last_arg_adv = pos + 1;

            continue;
        }
        pos++;
    }
    _print_cstr(f, last_arg_ptr);
}

void bc_print_int(int64_t n) { _print_int(stdout, n); }

void bc_print_uint(uint64_t n) { _print_uint(stdout, n); }

void bc_print_size(size_t n) { _print_size(stdout, n); }

void bc_print_ch(int32_t c) { _print_ch(stdout, c); }

void bc_print_cch(char c) { _print_cch(stdout, c); }

void bc_print_float(float v) { _print_float(stdout, v); }

void bc_print_double(double v) { _print_double(stdout, v); }

void bc_print_cstr(const char* str) { _print_cstr(stdout, str); }

void bc_print_cstrn(const char* str, size_t n) { _print_cstrn(stdout, str, n); }

void bc_print_nl(void) { _print_nl(stdout); }

void bc_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(stdout, fmt, args);
    va_end(args);
}

void bc_eprint_int(int64_t n) { _print_int(stderr, n); }

void bc_eprint_uint(uint64_t n) { _print_uint(stderr, n); }

void bc_eprint_size(size_t n) { _print_size(stderr, n); }

void bc_eprint_ch(int32_t c) { _print_ch(stderr, c); }

void bc_eprint_cch(char c) { _print_cch(stderr, c); }

void bc_eprint_float(float v) { _print_float(stderr, v); }

void bc_eprint_double(double v) { _print_double(stderr, v); }

void bc_eprint_cstr(const char* str) { _print_cstr(stderr, str); }

void bc_eprint_cstrn(const char* str, size_t n) {
    _print_cstrn(stderr, str, n);
}

void bc_eprint_nl(void) { _print_nl(stderr); }

void bc_eprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(stderr, fmt, args);
    va_end(args);
}
