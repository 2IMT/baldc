#ifndef BALDC_UTF8_H
#define BALDC_UTF8_H

#include <stdint.h>
#include <stddef.h>

int bc_utf8_codepoint_len(int32_t codepoint);

/**
Converts unicode character to UTF-8 sequence.

Arguments:
`codepoint` - unicode character.
`out` - buffer to write UTF-8 sequence to.

Returns:
Length of the resulting UTF-8 sequence or `-1` on invalid codepoints.
*/
int bc_utf8_encode(int32_t codepoint, char* out);

/**
Converts the first UTF-8 character in sequence to unicode character.

Arguments:
`data` - UTF-8 sequence.
`len` - length of the sequence.
`out` - buffer to write to.

Returns:
Length of converted UTF-8 character or `-1` on invalid sequences.
*/
int bc_utf8_decode(const char* data, size_t len, int32_t* out);

#endif
