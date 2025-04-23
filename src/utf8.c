#include "utf8.h"

int bc_utf8_codepoint_len(int32_t codepoint) {
    if (codepoint < 0 || codepoint > 0x10FFFF ||
        (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
        return -1;
    }
    if (codepoint <= 0x7F) {
        return 1;
    } else if (codepoint <= 0x7FF) {
        return 2;
    } else if (codepoint <= 0xFFFF) {
        return 3;
    } else {
        return 4;
    }
}

int bc_utf8_encode(int32_t codepoint, char* out) {
    if (codepoint < 0 || codepoint > 0x10FFFF ||
        (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
        return -1;
    }
    if (codepoint <= 0x7F) {
        out[0] = (char)codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        out[0] = (char)(0xC0 | ((codepoint >> 6) & 0x1F));
        out[1] = (char)(0x80 | (codepoint & 0x3F));
        return 2;
    } else if (codepoint <= 0xFFFF) {
        out[0] = (char)(0xE0 | ((codepoint >> 12) & 0x0F));
        out[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[2] = (char)(0x80 | (codepoint & 0x3F));
        return 3;
    } else {
        out[0] = (char)(0xF0 | ((codepoint >> 18) & 0x07));
        out[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[3] = (char)(0x80 | (codepoint & 0x3F));
        return 4;
    }
}

int bc_utf8_decode(const char* data, size_t len, int32_t* out) {
    if (len == 0) {
        return 0;
    }

    unsigned char byte1 = (unsigned char)data[0];
    int32_t codepoint;
    size_t expected_len;

    if (byte1 <= 0x7F) {
        *out = byte1;
        return 1;
    } else if ((byte1 & 0xE0) == 0xC0) {
        expected_len = 2;
        if (len < expected_len)
            return -1;
        unsigned char byte2 = (unsigned char)data[1];
        if ((byte2 & 0xC0) != 0x80)
            return -1;
        codepoint = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
        if (codepoint < 0x80)
            return -1;
    } else if ((byte1 & 0xF0) == 0xE0) {
        expected_len = 3;
        if (len < expected_len)
            return -1;
        unsigned char byte2 = (unsigned char)data[1];
        unsigned char byte3 = (unsigned char)data[2];
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80)
            return -1;
        codepoint =
            ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
        if (codepoint < 0x800)
            return -1;
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
            return -1;
    } else if ((byte1 & 0xF8) == 0xF0) {
        expected_len = 4;
        if (len < expected_len)
            return -1;
        unsigned char byte2 = (unsigned char)data[1];
        unsigned char byte3 = (unsigned char)data[2];
        unsigned char byte4 = (unsigned char)data[3];
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80 ||
            (byte4 & 0xC0) != 0x80)
            return -1;
        codepoint = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) |
                    ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
        if (codepoint < 0x10000 || codepoint > 0x10FFFF)
            return -1;
    } else {
        return -1;
    }

    *out = codepoint;
    return expected_len;
}
