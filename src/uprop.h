#ifndef BC_UPROP_H
#define BC_UPROP_H

#include <stdint.h>
#include <stdbool.h>

bool bc_uprop_is_space(int32_t c);

bool bc_uprop_is_alpha(int32_t c);

bool bc_uprop_is_upper(int32_t c);

bool bc_uprop_is_lower(int32_t c);

bool bc_uprop_is_digit(int32_t c);

#endif
