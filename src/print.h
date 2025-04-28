#ifndef BALDC_PRINT_H
#define BALDC_PRINT_H

#include <stdint.h>
#include <stddef.h>

void bc_print_int(int64_t n);

void bc_print_uint(uint64_t n);

void bc_print_size(size_t n);

void bc_print_ch(int32_t c);

void bc_print_cch(char c);

void bc_print_float(float v);

void bc_print_double(double v);

void bc_print_cstr(const char* str);

void bc_print_cstrn(const char* str, size_t n);

void bc_print_nl(void);

void bc_printf(const char* fmt, ...);

void bc_eprint_int(int64_t n);

void bc_eprint_uint(uint64_t n);

void bc_eprint_size(size_t n);

void bc_eprint_ch(int32_t c);

void bc_eprint_cch(char c);

void bc_eprint_float(float v);

void bc_eprint_double(double v);

void bc_eprint_cstr(const char* str);

void bc_eprint_cstrn(const char* str, size_t n);

void bc_eprint_nl(void);

void bc_eprintf(const char* fmt, ...);

#endif
