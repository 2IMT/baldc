#include "uprop.h"

bool bc_uprop_is_space(int32_t c) {
    switch (c) {
    case 0x0020:
    case 0x0085:
    case 0x00A0:
    case 0x1680:
    case 0x2028:
    case 0x2029:
    case 0x202F:
    case 0x205F:
    case 0x3000:
        return true;
    }
    if (0x0009 <= c && c <= 0x000D) {
        return true;
    }
    if (0x2000 <= c && c <= 0x200A) {
        return true;
    }
    return false;
}

bool bc_uprop_is_alpha(int32_t c) {
    switch (c) {
    case 0x00AA:
    case 0x00B5:
    case 0x00BA:
    case 0x01BB:
    case 0x0294:
    case 0x02EC:
    case 0x02EE:
    case 0x0345:
    case 0x0374:
    case 0x037A:
    case 0x037F:
    case 0x0386:
    case 0x038C:
    case 0x0559:
    case 0x05BF:
    case 0x05C7:
    case 0x0640:
    case 0x0670:
    case 0x06D5:
    case 0x06ED:
    case 0x06FF:
    case 0x0710:
    case 0x0711:
    case 0x07B1:
    case 0x07FA:
    case 0x081A:
    case 0x0824:
    case 0x0828:
    case 0x0897:
    case 0x08C9:
    case 0x0903:
    case 0x093A:
    case 0x093B:
    case 0x093D:
    case 0x0950:
    case 0x0971:
    case 0x0981:
    case 0x09B2:
    case 0x09BD:
    case 0x09CE:
    case 0x09D7:
    case 0x09FC:
    case 0x0A03:
    case 0x0A51:
    case 0x0A5E:
    case 0x0A75:
    case 0x0A83:
    case 0x0ABD:
    case 0x0AC9:
    case 0x0AD0:
    case 0x0AF9:
    case 0x0B01:
    case 0x0B3D:
    case 0x0B3E:
    case 0x0B3F:
    case 0x0B40:
    case 0x0B56:
    case 0x0B57:
    case 0x0B71:
    case 0x0B82:
    case 0x0B83:
    case 0x0B9C:
    case 0x0BC0:
    case 0x0BD0:
    case 0x0BD7:
    case 0x0C00:
    case 0x0C04:
    case 0x0C3D:
    case 0x0C5D:
    case 0x0C80:
    case 0x0C81:
    case 0x0CBD:
    case 0x0CBE:
    case 0x0CBF:
    case 0x0CC6:
    case 0x0CCC:
    case 0x0CF3:
    case 0x0D3D:
    case 0x0D4E:
    case 0x0D57:
    case 0x0D81:
    case 0x0DBD:
    case 0x0DD6:
    case 0x0E31:
    case 0x0E46:
    case 0x0E4D:
    case 0x0E84:
    case 0x0EA5:
    case 0x0EB1:
    case 0x0EBD:
    case 0x0EC6:
    case 0x0ECD:
    case 0x0F00:
    case 0x0F7F:
    case 0x1031:
    case 0x1038:
    case 0x103F:
    case 0x1061:
    case 0x1082:
    case 0x108D:
    case 0x108E:
    case 0x108F:
    case 0x109D:
    case 0x10C7:
    case 0x10CD:
    case 0x10FC:
    case 0x1258:
    case 0x12C0:
    case 0x17B6:
    case 0x17C6:
    case 0x17D7:
    case 0x17DC:
    case 0x1843:
    case 0x18A9:
    case 0x18AA:
    case 0x1932:
    case 0x1A1B:
    case 0x1A55:
    case 0x1A56:
    case 0x1A57:
    case 0x1A61:
    case 0x1A62:
    case 0x1AA7:
    case 0x1B04:
    case 0x1B35:
    case 0x1B3B:
    case 0x1B3C:
    case 0x1B42:
    case 0x1B43:
    case 0x1B82:
    case 0x1BA1:
    case 0x1BE7:
    case 0x1BED:
    case 0x1BEE:
    case 0x1C36:
    case 0x1CFA:
    case 0x1D78:
    case 0x1F59:
    case 0x1F5B:
    case 0x1F5D:
    case 0x1FBE:
    case 0x2071:
    case 0x207F:
    case 0x2102:
    case 0x2107:
    case 0x2115:
    case 0x2124:
    case 0x2126:
    case 0x2128:
    case 0x2139:
    case 0x214E:
    case 0x2D27:
    case 0x2D2D:
    case 0x2D6F:
    case 0x2E2F:
    case 0x3005:
    case 0x3006:
    case 0x3007:
    case 0x303B:
    case 0x303C:
    case 0x309F:
    case 0x30FF:
    case 0xA015:
    case 0xA60C:
    case 0xA66E:
    case 0xA67F:
    case 0xA770:
    case 0xA788:
    case 0xA78F:
    case 0xA7D3:
    case 0xA7F7:
    case 0xA7FA:
    case 0xA802:
    case 0xA80B:
    case 0xA827:
    case 0xA8C5:
    case 0xA8FB:
    case 0xA8FF:
    case 0xA952:
    case 0xA983:
    case 0xA9CF:
    case 0xA9E5:
    case 0xA9E6:
    case 0xAA43:
    case 0xAA4C:
    case 0xAA4D:
    case 0xAA70:
    case 0xAA7A:
    case 0xAA7B:
    case 0xAA7C:
    case 0xAA7D:
    case 0xAAB0:
    case 0xAAB1:
    case 0xAABE:
    case 0xAAC0:
    case 0xAAC2:
    case 0xAADD:
    case 0xAAEB:
    case 0xAAF2:
    case 0xAAF5:
    case 0xAB69:
    case 0xABE5:
    case 0xABE8:
    case 0xFB1D:
    case 0xFB1E:
    case 0xFB3E:
    case 0xFF70:
    case 0x10341:
    case 0x1034A:
    case 0x10808:
    case 0x1083C:
    case 0x10A00:
    case 0x10D4E:
    case 0x10D4F:
    case 0x10D69:
    case 0x10D6F:
    case 0x10EFC:
    case 0x10F27:
    case 0x11000:
    case 0x11001:
    case 0x11002:
    case 0x11075:
    case 0x11082:
    case 0x110C2:
    case 0x1112C:
    case 0x11144:
    case 0x11147:
    case 0x11176:
    case 0x11182:
    case 0x111BF:
    case 0x111CE:
    case 0x111CF:
    case 0x111DA:
    case 0x111DC:
    case 0x11234:
    case 0x11237:
    case 0x1123E:
    case 0x11241:
    case 0x11288:
    case 0x112DF:
    case 0x1133D:
    case 0x11340:
    case 0x11350:
    case 0x11357:
    case 0x1138B:
    case 0x1138E:
    case 0x113B7:
    case 0x113C2:
    case 0x113C5:
    case 0x113D1:
    case 0x113D3:
    case 0x11445:
    case 0x114B9:
    case 0x114BA:
    case 0x114C1:
    case 0x114C7:
    case 0x115BE:
    case 0x1163D:
    case 0x1163E:
    case 0x11640:
    case 0x11644:
    case 0x116AB:
    case 0x116AC:
    case 0x116AD:
    case 0x116B8:
    case 0x1171D:
    case 0x1171E:
    case 0x1171F:
    case 0x11726:
    case 0x11838:
    case 0x11909:
    case 0x1193F:
    case 0x11940:
    case 0x11941:
    case 0x11942:
    case 0x119E1:
    case 0x119E3:
    case 0x119E4:
    case 0x11A00:
    case 0x11A39:
    case 0x11A3A:
    case 0x11A50:
    case 0x11A97:
    case 0x11A9D:
    case 0x11C2F:
    case 0x11C3E:
    case 0x11C40:
    case 0x11CA9:
    case 0x11CB1:
    case 0x11CB4:
    case 0x11D3A:
    case 0x11D43:
    case 0x11D46:
    case 0x11D47:
    case 0x11D95:
    case 0x11D96:
    case 0x11D98:
    case 0x11F02:
    case 0x11F03:
    case 0x11F40:
    case 0x11FB0:
    case 0x16F4F:
    case 0x16F50:
    case 0x16FE3:
    case 0x1B132:
    case 0x1B155:
    case 0x1BC9E:
    case 0x1D4A2:
    case 0x1D4BB:
    case 0x1D546:
    case 0x1DF0A:
    case 0x1E08F:
    case 0x1E14E:
    case 0x1E4EB:
    case 0x1E5F0:
    case 0x1E947:
    case 0x1E94B:
    case 0x1EE24:
    case 0x1EE27:
    case 0x1EE39:
    case 0x1EE3B:
    case 0x1EE42:
    case 0x1EE47:
    case 0x1EE49:
    case 0x1EE4B:
    case 0x1EE54:
    case 0x1EE57:
    case 0x1EE59:
    case 0x1EE5B:
    case 0x1EE5D:
    case 0x1EE5F:
    case 0x1EE64:
    case 0x1EE7E:
        return true;
    }
    if (0x0041 <= c && c <= 0x005A) {
        return true;
    }
    if (0x0061 <= c && c <= 0x007A) {
        return true;
    }
    if (0x00C0 <= c && c <= 0x00D6) {
        return true;
    }
    if (0x00D8 <= c && c <= 0x00F6) {
        return true;
    }
    if (0x00F8 <= c && c <= 0x01BA) {
        return true;
    }
    if (0x01BC <= c && c <= 0x01BF) {
        return true;
    }
    if (0x01C0 <= c && c <= 0x01C3) {
        return true;
    }
    if (0x01C4 <= c && c <= 0x0293) {
        return true;
    }
    if (0x0295 <= c && c <= 0x02AF) {
        return true;
    }
    if (0x02B0 <= c && c <= 0x02C1) {
        return true;
    }
    if (0x02C6 <= c && c <= 0x02D1) {
        return true;
    }
    if (0x02E0 <= c && c <= 0x02E4) {
        return true;
    }
    if (0x0363 <= c && c <= 0x036F) {
        return true;
    }
    if (0x0370 <= c && c <= 0x0373) {
        return true;
    }
    if (0x0376 <= c && c <= 0x0377) {
        return true;
    }
    if (0x037B <= c && c <= 0x037D) {
        return true;
    }
    if (0x0388 <= c && c <= 0x038A) {
        return true;
    }
    if (0x038E <= c && c <= 0x03A1) {
        return true;
    }
    if (0x03A3 <= c && c <= 0x03F5) {
        return true;
    }
    if (0x03F7 <= c && c <= 0x0481) {
        return true;
    }
    if (0x048A <= c && c <= 0x052F) {
        return true;
    }
    if (0x0531 <= c && c <= 0x0556) {
        return true;
    }
    if (0x0560 <= c && c <= 0x0588) {
        return true;
    }
    if (0x05B0 <= c && c <= 0x05BD) {
        return true;
    }
    if (0x05C1 <= c && c <= 0x05C2) {
        return true;
    }
    if (0x05C4 <= c && c <= 0x05C5) {
        return true;
    }
    if (0x05D0 <= c && c <= 0x05EA) {
        return true;
    }
    if (0x05EF <= c && c <= 0x05F2) {
        return true;
    }
    if (0x0610 <= c && c <= 0x061A) {
        return true;
    }
    if (0x0620 <= c && c <= 0x063F) {
        return true;
    }
    if (0x0641 <= c && c <= 0x064A) {
        return true;
    }
    if (0x064B <= c && c <= 0x0657) {
        return true;
    }
    if (0x0659 <= c && c <= 0x065F) {
        return true;
    }
    if (0x066E <= c && c <= 0x066F) {
        return true;
    }
    if (0x0671 <= c && c <= 0x06D3) {
        return true;
    }
    if (0x06D6 <= c && c <= 0x06DC) {
        return true;
    }
    if (0x06E1 <= c && c <= 0x06E4) {
        return true;
    }
    if (0x06E5 <= c && c <= 0x06E6) {
        return true;
    }
    if (0x06E7 <= c && c <= 0x06E8) {
        return true;
    }
    if (0x06EE <= c && c <= 0x06EF) {
        return true;
    }
    if (0x06FA <= c && c <= 0x06FC) {
        return true;
    }
    if (0x0712 <= c && c <= 0x072F) {
        return true;
    }
    if (0x0730 <= c && c <= 0x073F) {
        return true;
    }
    if (0x074D <= c && c <= 0x07A5) {
        return true;
    }
    if (0x07A6 <= c && c <= 0x07B0) {
        return true;
    }
    if (0x07CA <= c && c <= 0x07EA) {
        return true;
    }
    if (0x07F4 <= c && c <= 0x07F5) {
        return true;
    }
    if (0x0800 <= c && c <= 0x0815) {
        return true;
    }
    if (0x0816 <= c && c <= 0x0817) {
        return true;
    }
    if (0x081B <= c && c <= 0x0823) {
        return true;
    }
    if (0x0825 <= c && c <= 0x0827) {
        return true;
    }
    if (0x0829 <= c && c <= 0x082C) {
        return true;
    }
    if (0x0840 <= c && c <= 0x0858) {
        return true;
    }
    if (0x0860 <= c && c <= 0x086A) {
        return true;
    }
    if (0x0870 <= c && c <= 0x0887) {
        return true;
    }
    if (0x0889 <= c && c <= 0x088E) {
        return true;
    }
    if (0x08A0 <= c && c <= 0x08C8) {
        return true;
    }
    if (0x08D4 <= c && c <= 0x08DF) {
        return true;
    }
    if (0x08E3 <= c && c <= 0x08E9) {
        return true;
    }
    if (0x08F0 <= c && c <= 0x0902) {
        return true;
    }
    if (0x0904 <= c && c <= 0x0939) {
        return true;
    }
    if (0x093E <= c && c <= 0x0940) {
        return true;
    }
    if (0x0941 <= c && c <= 0x0948) {
        return true;
    }
    if (0x0949 <= c && c <= 0x094C) {
        return true;
    }
    if (0x094E <= c && c <= 0x094F) {
        return true;
    }
    if (0x0955 <= c && c <= 0x0957) {
        return true;
    }
    if (0x0958 <= c && c <= 0x0961) {
        return true;
    }
    if (0x0962 <= c && c <= 0x0963) {
        return true;
    }
    if (0x0972 <= c && c <= 0x0980) {
        return true;
    }
    if (0x0982 <= c && c <= 0x0983) {
        return true;
    }
    if (0x0985 <= c && c <= 0x098C) {
        return true;
    }
    if (0x098F <= c && c <= 0x0990) {
        return true;
    }
    if (0x0993 <= c && c <= 0x09A8) {
        return true;
    }
    if (0x09AA <= c && c <= 0x09B0) {
        return true;
    }
    if (0x09B6 <= c && c <= 0x09B9) {
        return true;
    }
    if (0x09BE <= c && c <= 0x09C0) {
        return true;
    }
    if (0x09C1 <= c && c <= 0x09C4) {
        return true;
    }
    if (0x09C7 <= c && c <= 0x09C8) {
        return true;
    }
    if (0x09CB <= c && c <= 0x09CC) {
        return true;
    }
    if (0x09DC <= c && c <= 0x09DD) {
        return true;
    }
    if (0x09DF <= c && c <= 0x09E1) {
        return true;
    }
    if (0x09E2 <= c && c <= 0x09E3) {
        return true;
    }
    if (0x09F0 <= c && c <= 0x09F1) {
        return true;
    }
    if (0x0A01 <= c && c <= 0x0A02) {
        return true;
    }
    if (0x0A05 <= c && c <= 0x0A0A) {
        return true;
    }
    if (0x0A0F <= c && c <= 0x0A10) {
        return true;
    }
    if (0x0A13 <= c && c <= 0x0A28) {
        return true;
    }
    if (0x0A2A <= c && c <= 0x0A30) {
        return true;
    }
    if (0x0A32 <= c && c <= 0x0A33) {
        return true;
    }
    if (0x0A35 <= c && c <= 0x0A36) {
        return true;
    }
    if (0x0A38 <= c && c <= 0x0A39) {
        return true;
    }
    if (0x0A3E <= c && c <= 0x0A40) {
        return true;
    }
    if (0x0A41 <= c && c <= 0x0A42) {
        return true;
    }
    if (0x0A47 <= c && c <= 0x0A48) {
        return true;
    }
    if (0x0A4B <= c && c <= 0x0A4C) {
        return true;
    }
    if (0x0A59 <= c && c <= 0x0A5C) {
        return true;
    }
    if (0x0A70 <= c && c <= 0x0A71) {
        return true;
    }
    if (0x0A72 <= c && c <= 0x0A74) {
        return true;
    }
    if (0x0A81 <= c && c <= 0x0A82) {
        return true;
    }
    if (0x0A85 <= c && c <= 0x0A8D) {
        return true;
    }
    if (0x0A8F <= c && c <= 0x0A91) {
        return true;
    }
    if (0x0A93 <= c && c <= 0x0AA8) {
        return true;
    }
    if (0x0AAA <= c && c <= 0x0AB0) {
        return true;
    }
    if (0x0AB2 <= c && c <= 0x0AB3) {
        return true;
    }
    if (0x0AB5 <= c && c <= 0x0AB9) {
        return true;
    }
    if (0x0ABE <= c && c <= 0x0AC0) {
        return true;
    }
    if (0x0AC1 <= c && c <= 0x0AC5) {
        return true;
    }
    if (0x0AC7 <= c && c <= 0x0AC8) {
        return true;
    }
    if (0x0ACB <= c && c <= 0x0ACC) {
        return true;
    }
    if (0x0AE0 <= c && c <= 0x0AE1) {
        return true;
    }
    if (0x0AE2 <= c && c <= 0x0AE3) {
        return true;
    }
    if (0x0AFA <= c && c <= 0x0AFC) {
        return true;
    }
    if (0x0B02 <= c && c <= 0x0B03) {
        return true;
    }
    if (0x0B05 <= c && c <= 0x0B0C) {
        return true;
    }
    if (0x0B0F <= c && c <= 0x0B10) {
        return true;
    }
    if (0x0B13 <= c && c <= 0x0B28) {
        return true;
    }
    if (0x0B2A <= c && c <= 0x0B30) {
        return true;
    }
    if (0x0B32 <= c && c <= 0x0B33) {
        return true;
    }
    if (0x0B35 <= c && c <= 0x0B39) {
        return true;
    }
    if (0x0B41 <= c && c <= 0x0B44) {
        return true;
    }
    if (0x0B47 <= c && c <= 0x0B48) {
        return true;
    }
    if (0x0B4B <= c && c <= 0x0B4C) {
        return true;
    }
    if (0x0B5C <= c && c <= 0x0B5D) {
        return true;
    }
    if (0x0B5F <= c && c <= 0x0B61) {
        return true;
    }
    if (0x0B62 <= c && c <= 0x0B63) {
        return true;
    }
    if (0x0B85 <= c && c <= 0x0B8A) {
        return true;
    }
    if (0x0B8E <= c && c <= 0x0B90) {
        return true;
    }
    if (0x0B92 <= c && c <= 0x0B95) {
        return true;
    }
    if (0x0B99 <= c && c <= 0x0B9A) {
        return true;
    }
    if (0x0B9E <= c && c <= 0x0B9F) {
        return true;
    }
    if (0x0BA3 <= c && c <= 0x0BA4) {
        return true;
    }
    if (0x0BA8 <= c && c <= 0x0BAA) {
        return true;
    }
    if (0x0BAE <= c && c <= 0x0BB9) {
        return true;
    }
    if (0x0BBE <= c && c <= 0x0BBF) {
        return true;
    }
    if (0x0BC1 <= c && c <= 0x0BC2) {
        return true;
    }
    if (0x0BC6 <= c && c <= 0x0BC8) {
        return true;
    }
    if (0x0BCA <= c && c <= 0x0BCC) {
        return true;
    }
    if (0x0C01 <= c && c <= 0x0C03) {
        return true;
    }
    if (0x0C05 <= c && c <= 0x0C0C) {
        return true;
    }
    if (0x0C0E <= c && c <= 0x0C10) {
        return true;
    }
    if (0x0C12 <= c && c <= 0x0C28) {
        return true;
    }
    if (0x0C2A <= c && c <= 0x0C39) {
        return true;
    }
    if (0x0C3E <= c && c <= 0x0C40) {
        return true;
    }
    if (0x0C41 <= c && c <= 0x0C44) {
        return true;
    }
    if (0x0C46 <= c && c <= 0x0C48) {
        return true;
    }
    if (0x0C4A <= c && c <= 0x0C4C) {
        return true;
    }
    if (0x0C55 <= c && c <= 0x0C56) {
        return true;
    }
    if (0x0C58 <= c && c <= 0x0C5A) {
        return true;
    }
    if (0x0C60 <= c && c <= 0x0C61) {
        return true;
    }
    if (0x0C62 <= c && c <= 0x0C63) {
        return true;
    }
    if (0x0C82 <= c && c <= 0x0C83) {
        return true;
    }
    if (0x0C85 <= c && c <= 0x0C8C) {
        return true;
    }
    if (0x0C8E <= c && c <= 0x0C90) {
        return true;
    }
    if (0x0C92 <= c && c <= 0x0CA8) {
        return true;
    }
    if (0x0CAA <= c && c <= 0x0CB3) {
        return true;
    }
    if (0x0CB5 <= c && c <= 0x0CB9) {
        return true;
    }
    if (0x0CC0 <= c && c <= 0x0CC4) {
        return true;
    }
    if (0x0CC7 <= c && c <= 0x0CC8) {
        return true;
    }
    if (0x0CCA <= c && c <= 0x0CCB) {
        return true;
    }
    if (0x0CD5 <= c && c <= 0x0CD6) {
        return true;
    }
    if (0x0CDD <= c && c <= 0x0CDE) {
        return true;
    }
    if (0x0CE0 <= c && c <= 0x0CE1) {
        return true;
    }
    if (0x0CE2 <= c && c <= 0x0CE3) {
        return true;
    }
    if (0x0CF1 <= c && c <= 0x0CF2) {
        return true;
    }
    if (0x0D00 <= c && c <= 0x0D01) {
        return true;
    }
    if (0x0D02 <= c && c <= 0x0D03) {
        return true;
    }
    if (0x0D04 <= c && c <= 0x0D0C) {
        return true;
    }
    if (0x0D0E <= c && c <= 0x0D10) {
        return true;
    }
    if (0x0D12 <= c && c <= 0x0D3A) {
        return true;
    }
    if (0x0D3E <= c && c <= 0x0D40) {
        return true;
    }
    if (0x0D41 <= c && c <= 0x0D44) {
        return true;
    }
    if (0x0D46 <= c && c <= 0x0D48) {
        return true;
    }
    if (0x0D4A <= c && c <= 0x0D4C) {
        return true;
    }
    if (0x0D54 <= c && c <= 0x0D56) {
        return true;
    }
    if (0x0D5F <= c && c <= 0x0D61) {
        return true;
    }
    if (0x0D62 <= c && c <= 0x0D63) {
        return true;
    }
    if (0x0D7A <= c && c <= 0x0D7F) {
        return true;
    }
    if (0x0D82 <= c && c <= 0x0D83) {
        return true;
    }
    if (0x0D85 <= c && c <= 0x0D96) {
        return true;
    }
    if (0x0D9A <= c && c <= 0x0DB1) {
        return true;
    }
    if (0x0DB3 <= c && c <= 0x0DBB) {
        return true;
    }
    if (0x0DC0 <= c && c <= 0x0DC6) {
        return true;
    }
    if (0x0DCF <= c && c <= 0x0DD1) {
        return true;
    }
    if (0x0DD2 <= c && c <= 0x0DD4) {
        return true;
    }
    if (0x0DD8 <= c && c <= 0x0DDF) {
        return true;
    }
    if (0x0DF2 <= c && c <= 0x0DF3) {
        return true;
    }
    if (0x0E01 <= c && c <= 0x0E30) {
        return true;
    }
    if (0x0E32 <= c && c <= 0x0E33) {
        return true;
    }
    if (0x0E34 <= c && c <= 0x0E3A) {
        return true;
    }
    if (0x0E40 <= c && c <= 0x0E45) {
        return true;
    }
    if (0x0E81 <= c && c <= 0x0E82) {
        return true;
    }
    if (0x0E86 <= c && c <= 0x0E8A) {
        return true;
    }
    if (0x0E8C <= c && c <= 0x0EA3) {
        return true;
    }
    if (0x0EA7 <= c && c <= 0x0EB0) {
        return true;
    }
    if (0x0EB2 <= c && c <= 0x0EB3) {
        return true;
    }
    if (0x0EB4 <= c && c <= 0x0EB9) {
        return true;
    }
    if (0x0EBB <= c && c <= 0x0EBC) {
        return true;
    }
    if (0x0EC0 <= c && c <= 0x0EC4) {
        return true;
    }
    if (0x0EDC <= c && c <= 0x0EDF) {
        return true;
    }
    if (0x0F40 <= c && c <= 0x0F47) {
        return true;
    }
    if (0x0F49 <= c && c <= 0x0F6C) {
        return true;
    }
    if (0x0F71 <= c && c <= 0x0F7E) {
        return true;
    }
    if (0x0F80 <= c && c <= 0x0F83) {
        return true;
    }
    if (0x0F88 <= c && c <= 0x0F8C) {
        return true;
    }
    if (0x0F8D <= c && c <= 0x0F97) {
        return true;
    }
    if (0x0F99 <= c && c <= 0x0FBC) {
        return true;
    }
    if (0x1000 <= c && c <= 0x102A) {
        return true;
    }
    if (0x102B <= c && c <= 0x102C) {
        return true;
    }
    if (0x102D <= c && c <= 0x1030) {
        return true;
    }
    if (0x1032 <= c && c <= 0x1036) {
        return true;
    }
    if (0x103B <= c && c <= 0x103C) {
        return true;
    }
    if (0x103D <= c && c <= 0x103E) {
        return true;
    }
    if (0x1050 <= c && c <= 0x1055) {
        return true;
    }
    if (0x1056 <= c && c <= 0x1057) {
        return true;
    }
    if (0x1058 <= c && c <= 0x1059) {
        return true;
    }
    if (0x105A <= c && c <= 0x105D) {
        return true;
    }
    if (0x105E <= c && c <= 0x1060) {
        return true;
    }
    if (0x1062 <= c && c <= 0x1064) {
        return true;
    }
    if (0x1065 <= c && c <= 0x1066) {
        return true;
    }
    if (0x1067 <= c && c <= 0x106D) {
        return true;
    }
    if (0x106E <= c && c <= 0x1070) {
        return true;
    }
    if (0x1071 <= c && c <= 0x1074) {
        return true;
    }
    if (0x1075 <= c && c <= 0x1081) {
        return true;
    }
    if (0x1083 <= c && c <= 0x1084) {
        return true;
    }
    if (0x1085 <= c && c <= 0x1086) {
        return true;
    }
    if (0x1087 <= c && c <= 0x108C) {
        return true;
    }
    if (0x109A <= c && c <= 0x109C) {
        return true;
    }
    if (0x10A0 <= c && c <= 0x10C5) {
        return true;
    }
    if (0x10D0 <= c && c <= 0x10FA) {
        return true;
    }
    if (0x10FD <= c && c <= 0x10FF) {
        return true;
    }
    if (0x1100 <= c && c <= 0x1248) {
        return true;
    }
    if (0x124A <= c && c <= 0x124D) {
        return true;
    }
    if (0x1250 <= c && c <= 0x1256) {
        return true;
    }
    if (0x125A <= c && c <= 0x125D) {
        return true;
    }
    if (0x1260 <= c && c <= 0x1288) {
        return true;
    }
    if (0x128A <= c && c <= 0x128D) {
        return true;
    }
    if (0x1290 <= c && c <= 0x12B0) {
        return true;
    }
    if (0x12B2 <= c && c <= 0x12B5) {
        return true;
    }
    if (0x12B8 <= c && c <= 0x12BE) {
        return true;
    }
    if (0x12C2 <= c && c <= 0x12C5) {
        return true;
    }
    if (0x12C8 <= c && c <= 0x12D6) {
        return true;
    }
    if (0x12D8 <= c && c <= 0x1310) {
        return true;
    }
    if (0x1312 <= c && c <= 0x1315) {
        return true;
    }
    if (0x1318 <= c && c <= 0x135A) {
        return true;
    }
    if (0x1380 <= c && c <= 0x138F) {
        return true;
    }
    if (0x13A0 <= c && c <= 0x13F5) {
        return true;
    }
    if (0x13F8 <= c && c <= 0x13FD) {
        return true;
    }
    if (0x1401 <= c && c <= 0x166C) {
        return true;
    }
    if (0x166F <= c && c <= 0x167F) {
        return true;
    }
    if (0x1681 <= c && c <= 0x169A) {
        return true;
    }
    if (0x16A0 <= c && c <= 0x16EA) {
        return true;
    }
    if (0x16EE <= c && c <= 0x16F0) {
        return true;
    }
    if (0x16F1 <= c && c <= 0x16F8) {
        return true;
    }
    if (0x1700 <= c && c <= 0x1711) {
        return true;
    }
    if (0x1712 <= c && c <= 0x1713) {
        return true;
    }
    if (0x171F <= c && c <= 0x1731) {
        return true;
    }
    if (0x1732 <= c && c <= 0x1733) {
        return true;
    }
    if (0x1740 <= c && c <= 0x1751) {
        return true;
    }
    if (0x1752 <= c && c <= 0x1753) {
        return true;
    }
    if (0x1760 <= c && c <= 0x176C) {
        return true;
    }
    if (0x176E <= c && c <= 0x1770) {
        return true;
    }
    if (0x1772 <= c && c <= 0x1773) {
        return true;
    }
    if (0x1780 <= c && c <= 0x17B3) {
        return true;
    }
    if (0x17B7 <= c && c <= 0x17BD) {
        return true;
    }
    if (0x17BE <= c && c <= 0x17C5) {
        return true;
    }
    if (0x17C7 <= c && c <= 0x17C8) {
        return true;
    }
    if (0x1820 <= c && c <= 0x1842) {
        return true;
    }
    if (0x1844 <= c && c <= 0x1878) {
        return true;
    }
    if (0x1880 <= c && c <= 0x1884) {
        return true;
    }
    if (0x1885 <= c && c <= 0x1886) {
        return true;
    }
    if (0x1887 <= c && c <= 0x18A8) {
        return true;
    }
    if (0x18B0 <= c && c <= 0x18F5) {
        return true;
    }
    if (0x1900 <= c && c <= 0x191E) {
        return true;
    }
    if (0x1920 <= c && c <= 0x1922) {
        return true;
    }
    if (0x1923 <= c && c <= 0x1926) {
        return true;
    }
    if (0x1927 <= c && c <= 0x1928) {
        return true;
    }
    if (0x1929 <= c && c <= 0x192B) {
        return true;
    }
    if (0x1930 <= c && c <= 0x1931) {
        return true;
    }
    if (0x1933 <= c && c <= 0x1938) {
        return true;
    }
    if (0x1950 <= c && c <= 0x196D) {
        return true;
    }
    if (0x1970 <= c && c <= 0x1974) {
        return true;
    }
    if (0x1980 <= c && c <= 0x19AB) {
        return true;
    }
    if (0x19B0 <= c && c <= 0x19C9) {
        return true;
    }
    if (0x1A00 <= c && c <= 0x1A16) {
        return true;
    }
    if (0x1A17 <= c && c <= 0x1A18) {
        return true;
    }
    if (0x1A19 <= c && c <= 0x1A1A) {
        return true;
    }
    if (0x1A20 <= c && c <= 0x1A54) {
        return true;
    }
    if (0x1A58 <= c && c <= 0x1A5E) {
        return true;
    }
    if (0x1A63 <= c && c <= 0x1A64) {
        return true;
    }
    if (0x1A65 <= c && c <= 0x1A6C) {
        return true;
    }
    if (0x1A6D <= c && c <= 0x1A72) {
        return true;
    }
    if (0x1A73 <= c && c <= 0x1A74) {
        return true;
    }
    if (0x1ABF <= c && c <= 0x1AC0) {
        return true;
    }
    if (0x1ACC <= c && c <= 0x1ACE) {
        return true;
    }
    if (0x1B00 <= c && c <= 0x1B03) {
        return true;
    }
    if (0x1B05 <= c && c <= 0x1B33) {
        return true;
    }
    if (0x1B36 <= c && c <= 0x1B3A) {
        return true;
    }
    if (0x1B3D <= c && c <= 0x1B41) {
        return true;
    }
    if (0x1B45 <= c && c <= 0x1B4C) {
        return true;
    }
    if (0x1B80 <= c && c <= 0x1B81) {
        return true;
    }
    if (0x1B83 <= c && c <= 0x1BA0) {
        return true;
    }
    if (0x1BA2 <= c && c <= 0x1BA5) {
        return true;
    }
    if (0x1BA6 <= c && c <= 0x1BA7) {
        return true;
    }
    if (0x1BA8 <= c && c <= 0x1BA9) {
        return true;
    }
    if (0x1BAC <= c && c <= 0x1BAD) {
        return true;
    }
    if (0x1BAE <= c && c <= 0x1BAF) {
        return true;
    }
    if (0x1BBA <= c && c <= 0x1BE5) {
        return true;
    }
    if (0x1BE8 <= c && c <= 0x1BE9) {
        return true;
    }
    if (0x1BEA <= c && c <= 0x1BEC) {
        return true;
    }
    if (0x1BEF <= c && c <= 0x1BF1) {
        return true;
    }
    if (0x1C00 <= c && c <= 0x1C23) {
        return true;
    }
    if (0x1C24 <= c && c <= 0x1C2B) {
        return true;
    }
    if (0x1C2C <= c && c <= 0x1C33) {
        return true;
    }
    if (0x1C34 <= c && c <= 0x1C35) {
        return true;
    }
    if (0x1C4D <= c && c <= 0x1C4F) {
        return true;
    }
    if (0x1C5A <= c && c <= 0x1C77) {
        return true;
    }
    if (0x1C78 <= c && c <= 0x1C7D) {
        return true;
    }
    if (0x1C80 <= c && c <= 0x1C8A) {
        return true;
    }
    if (0x1C90 <= c && c <= 0x1CBA) {
        return true;
    }
    if (0x1CBD <= c && c <= 0x1CBF) {
        return true;
    }
    if (0x1CE9 <= c && c <= 0x1CEC) {
        return true;
    }
    if (0x1CEE <= c && c <= 0x1CF3) {
        return true;
    }
    if (0x1CF5 <= c && c <= 0x1CF6) {
        return true;
    }
    if (0x1D00 <= c && c <= 0x1D2B) {
        return true;
    }
    if (0x1D2C <= c && c <= 0x1D6A) {
        return true;
    }
    if (0x1D6B <= c && c <= 0x1D77) {
        return true;
    }
    if (0x1D79 <= c && c <= 0x1D9A) {
        return true;
    }
    if (0x1D9B <= c && c <= 0x1DBF) {
        return true;
    }
    if (0x1DD3 <= c && c <= 0x1DF4) {
        return true;
    }
    if (0x1E00 <= c && c <= 0x1F15) {
        return true;
    }
    if (0x1F18 <= c && c <= 0x1F1D) {
        return true;
    }
    if (0x1F20 <= c && c <= 0x1F45) {
        return true;
    }
    if (0x1F48 <= c && c <= 0x1F4D) {
        return true;
    }
    if (0x1F50 <= c && c <= 0x1F57) {
        return true;
    }
    if (0x1F5F <= c && c <= 0x1F7D) {
        return true;
    }
    if (0x1F80 <= c && c <= 0x1FB4) {
        return true;
    }
    if (0x1FB6 <= c && c <= 0x1FBC) {
        return true;
    }
    if (0x1FC2 <= c && c <= 0x1FC4) {
        return true;
    }
    if (0x1FC6 <= c && c <= 0x1FCC) {
        return true;
    }
    if (0x1FD0 <= c && c <= 0x1FD3) {
        return true;
    }
    if (0x1FD6 <= c && c <= 0x1FDB) {
        return true;
    }
    if (0x1FE0 <= c && c <= 0x1FEC) {
        return true;
    }
    if (0x1FF2 <= c && c <= 0x1FF4) {
        return true;
    }
    if (0x1FF6 <= c && c <= 0x1FFC) {
        return true;
    }
    if (0x2090 <= c && c <= 0x209C) {
        return true;
    }
    if (0x210A <= c && c <= 0x2113) {
        return true;
    }
    if (0x2119 <= c && c <= 0x211D) {
        return true;
    }
    if (0x212A <= c && c <= 0x212D) {
        return true;
    }
    if (0x212F <= c && c <= 0x2134) {
        return true;
    }
    if (0x2135 <= c && c <= 0x2138) {
        return true;
    }
    if (0x213C <= c && c <= 0x213F) {
        return true;
    }
    if (0x2145 <= c && c <= 0x2149) {
        return true;
    }
    if (0x2160 <= c && c <= 0x2182) {
        return true;
    }
    if (0x2183 <= c && c <= 0x2184) {
        return true;
    }
    if (0x2185 <= c && c <= 0x2188) {
        return true;
    }
    if (0x24B6 <= c && c <= 0x24E9) {
        return true;
    }
    if (0x2C00 <= c && c <= 0x2C7B) {
        return true;
    }
    if (0x2C7C <= c && c <= 0x2C7D) {
        return true;
    }
    if (0x2C7E <= c && c <= 0x2CE4) {
        return true;
    }
    if (0x2CEB <= c && c <= 0x2CEE) {
        return true;
    }
    if (0x2CF2 <= c && c <= 0x2CF3) {
        return true;
    }
    if (0x2D00 <= c && c <= 0x2D25) {
        return true;
    }
    if (0x2D30 <= c && c <= 0x2D67) {
        return true;
    }
    if (0x2D80 <= c && c <= 0x2D96) {
        return true;
    }
    if (0x2DA0 <= c && c <= 0x2DA6) {
        return true;
    }
    if (0x2DA8 <= c && c <= 0x2DAE) {
        return true;
    }
    if (0x2DB0 <= c && c <= 0x2DB6) {
        return true;
    }
    if (0x2DB8 <= c && c <= 0x2DBE) {
        return true;
    }
    if (0x2DC0 <= c && c <= 0x2DC6) {
        return true;
    }
    if (0x2DC8 <= c && c <= 0x2DCE) {
        return true;
    }
    if (0x2DD0 <= c && c <= 0x2DD6) {
        return true;
    }
    if (0x2DD8 <= c && c <= 0x2DDE) {
        return true;
    }
    if (0x2DE0 <= c && c <= 0x2DFF) {
        return true;
    }
    if (0x3021 <= c && c <= 0x3029) {
        return true;
    }
    if (0x3031 <= c && c <= 0x3035) {
        return true;
    }
    if (0x3038 <= c && c <= 0x303A) {
        return true;
    }
    if (0x3041 <= c && c <= 0x3096) {
        return true;
    }
    if (0x309D <= c && c <= 0x309E) {
        return true;
    }
    if (0x30A1 <= c && c <= 0x30FA) {
        return true;
    }
    if (0x30FC <= c && c <= 0x30FE) {
        return true;
    }
    if (0x3105 <= c && c <= 0x312F) {
        return true;
    }
    if (0x3131 <= c && c <= 0x318E) {
        return true;
    }
    if (0x31A0 <= c && c <= 0x31BF) {
        return true;
    }
    if (0x31F0 <= c && c <= 0x31FF) {
        return true;
    }
    if (0x3400 <= c && c <= 0x4DBF) {
        return true;
    }
    if (0x4E00 <= c && c <= 0xA014) {
        return true;
    }
    if (0xA016 <= c && c <= 0xA48C) {
        return true;
    }
    if (0xA4D0 <= c && c <= 0xA4F7) {
        return true;
    }
    if (0xA4F8 <= c && c <= 0xA4FD) {
        return true;
    }
    if (0xA500 <= c && c <= 0xA60B) {
        return true;
    }
    if (0xA610 <= c && c <= 0xA61F) {
        return true;
    }
    if (0xA62A <= c && c <= 0xA62B) {
        return true;
    }
    if (0xA640 <= c && c <= 0xA66D) {
        return true;
    }
    if (0xA674 <= c && c <= 0xA67B) {
        return true;
    }
    if (0xA680 <= c && c <= 0xA69B) {
        return true;
    }
    if (0xA69C <= c && c <= 0xA69D) {
        return true;
    }
    if (0xA69E <= c && c <= 0xA69F) {
        return true;
    }
    if (0xA6A0 <= c && c <= 0xA6E5) {
        return true;
    }
    if (0xA6E6 <= c && c <= 0xA6EF) {
        return true;
    }
    if (0xA717 <= c && c <= 0xA71F) {
        return true;
    }
    if (0xA722 <= c && c <= 0xA76F) {
        return true;
    }
    if (0xA771 <= c && c <= 0xA787) {
        return true;
    }
    if (0xA78B <= c && c <= 0xA78E) {
        return true;
    }
    if (0xA790 <= c && c <= 0xA7CD) {
        return true;
    }
    if (0xA7D0 <= c && c <= 0xA7D1) {
        return true;
    }
    if (0xA7D5 <= c && c <= 0xA7DC) {
        return true;
    }
    if (0xA7F2 <= c && c <= 0xA7F4) {
        return true;
    }
    if (0xA7F5 <= c && c <= 0xA7F6) {
        return true;
    }
    if (0xA7F8 <= c && c <= 0xA7F9) {
        return true;
    }
    if (0xA7FB <= c && c <= 0xA801) {
        return true;
    }
    if (0xA803 <= c && c <= 0xA805) {
        return true;
    }
    if (0xA807 <= c && c <= 0xA80A) {
        return true;
    }
    if (0xA80C <= c && c <= 0xA822) {
        return true;
    }
    if (0xA823 <= c && c <= 0xA824) {
        return true;
    }
    if (0xA825 <= c && c <= 0xA826) {
        return true;
    }
    if (0xA840 <= c && c <= 0xA873) {
        return true;
    }
    if (0xA880 <= c && c <= 0xA881) {
        return true;
    }
    if (0xA882 <= c && c <= 0xA8B3) {
        return true;
    }
    if (0xA8B4 <= c && c <= 0xA8C3) {
        return true;
    }
    if (0xA8F2 <= c && c <= 0xA8F7) {
        return true;
    }
    if (0xA8FD <= c && c <= 0xA8FE) {
        return true;
    }
    if (0xA90A <= c && c <= 0xA925) {
        return true;
    }
    if (0xA926 <= c && c <= 0xA92A) {
        return true;
    }
    if (0xA930 <= c && c <= 0xA946) {
        return true;
    }
    if (0xA947 <= c && c <= 0xA951) {
        return true;
    }
    if (0xA960 <= c && c <= 0xA97C) {
        return true;
    }
    if (0xA980 <= c && c <= 0xA982) {
        return true;
    }
    if (0xA984 <= c && c <= 0xA9B2) {
        return true;
    }
    if (0xA9B4 <= c && c <= 0xA9B5) {
        return true;
    }
    if (0xA9B6 <= c && c <= 0xA9B9) {
        return true;
    }
    if (0xA9BA <= c && c <= 0xA9BB) {
        return true;
    }
    if (0xA9BC <= c && c <= 0xA9BD) {
        return true;
    }
    if (0xA9BE <= c && c <= 0xA9BF) {
        return true;
    }
    if (0xA9E0 <= c && c <= 0xA9E4) {
        return true;
    }
    if (0xA9E7 <= c && c <= 0xA9EF) {
        return true;
    }
    if (0xA9FA <= c && c <= 0xA9FE) {
        return true;
    }
    if (0xAA00 <= c && c <= 0xAA28) {
        return true;
    }
    if (0xAA29 <= c && c <= 0xAA2E) {
        return true;
    }
    if (0xAA2F <= c && c <= 0xAA30) {
        return true;
    }
    if (0xAA31 <= c && c <= 0xAA32) {
        return true;
    }
    if (0xAA33 <= c && c <= 0xAA34) {
        return true;
    }
    if (0xAA35 <= c && c <= 0xAA36) {
        return true;
    }
    if (0xAA40 <= c && c <= 0xAA42) {
        return true;
    }
    if (0xAA44 <= c && c <= 0xAA4B) {
        return true;
    }
    if (0xAA60 <= c && c <= 0xAA6F) {
        return true;
    }
    if (0xAA71 <= c && c <= 0xAA76) {
        return true;
    }
    if (0xAA7E <= c && c <= 0xAAAF) {
        return true;
    }
    if (0xAAB2 <= c && c <= 0xAAB4) {
        return true;
    }
    if (0xAAB5 <= c && c <= 0xAAB6) {
        return true;
    }
    if (0xAAB7 <= c && c <= 0xAAB8) {
        return true;
    }
    if (0xAAB9 <= c && c <= 0xAABD) {
        return true;
    }
    if (0xAADB <= c && c <= 0xAADC) {
        return true;
    }
    if (0xAAE0 <= c && c <= 0xAAEA) {
        return true;
    }
    if (0xAAEC <= c && c <= 0xAAED) {
        return true;
    }
    if (0xAAEE <= c && c <= 0xAAEF) {
        return true;
    }
    if (0xAAF3 <= c && c <= 0xAAF4) {
        return true;
    }
    if (0xAB01 <= c && c <= 0xAB06) {
        return true;
    }
    if (0xAB09 <= c && c <= 0xAB0E) {
        return true;
    }
    if (0xAB11 <= c && c <= 0xAB16) {
        return true;
    }
    if (0xAB20 <= c && c <= 0xAB26) {
        return true;
    }
    if (0xAB28 <= c && c <= 0xAB2E) {
        return true;
    }
    if (0xAB30 <= c && c <= 0xAB5A) {
        return true;
    }
    if (0xAB5C <= c && c <= 0xAB5F) {
        return true;
    }
    if (0xAB60 <= c && c <= 0xAB68) {
        return true;
    }
    if (0xAB70 <= c && c <= 0xABBF) {
        return true;
    }
    if (0xABC0 <= c && c <= 0xABE2) {
        return true;
    }
    if (0xABE3 <= c && c <= 0xABE4) {
        return true;
    }
    if (0xABE6 <= c && c <= 0xABE7) {
        return true;
    }
    if (0xABE9 <= c && c <= 0xABEA) {
        return true;
    }
    if (0xAC00 <= c && c <= 0xD7A3) {
        return true;
    }
    if (0xD7B0 <= c && c <= 0xD7C6) {
        return true;
    }
    if (0xD7CB <= c && c <= 0xD7FB) {
        return true;
    }
    if (0xF900 <= c && c <= 0xFA6D) {
        return true;
    }
    if (0xFA70 <= c && c <= 0xFAD9) {
        return true;
    }
    if (0xFB00 <= c && c <= 0xFB06) {
        return true;
    }
    if (0xFB13 <= c && c <= 0xFB17) {
        return true;
    }
    if (0xFB1F <= c && c <= 0xFB28) {
        return true;
    }
    if (0xFB2A <= c && c <= 0xFB36) {
        return true;
    }
    if (0xFB38 <= c && c <= 0xFB3C) {
        return true;
    }
    if (0xFB40 <= c && c <= 0xFB41) {
        return true;
    }
    if (0xFB43 <= c && c <= 0xFB44) {
        return true;
    }
    if (0xFB46 <= c && c <= 0xFBB1) {
        return true;
    }
    if (0xFBD3 <= c && c <= 0xFD3D) {
        return true;
    }
    if (0xFD50 <= c && c <= 0xFD8F) {
        return true;
    }
    if (0xFD92 <= c && c <= 0xFDC7) {
        return true;
    }
    if (0xFDF0 <= c && c <= 0xFDFB) {
        return true;
    }
    if (0xFE70 <= c && c <= 0xFE74) {
        return true;
    }
    if (0xFE76 <= c && c <= 0xFEFC) {
        return true;
    }
    if (0xFF21 <= c && c <= 0xFF3A) {
        return true;
    }
    if (0xFF41 <= c && c <= 0xFF5A) {
        return true;
    }
    if (0xFF66 <= c && c <= 0xFF6F) {
        return true;
    }
    if (0xFF71 <= c && c <= 0xFF9D) {
        return true;
    }
    if (0xFF9E <= c && c <= 0xFF9F) {
        return true;
    }
    if (0xFFA0 <= c && c <= 0xFFBE) {
        return true;
    }
    if (0xFFC2 <= c && c <= 0xFFC7) {
        return true;
    }
    if (0xFFCA <= c && c <= 0xFFCF) {
        return true;
    }
    if (0xFFD2 <= c && c <= 0xFFD7) {
        return true;
    }
    if (0xFFDA <= c && c <= 0xFFDC) {
        return true;
    }
    if (0x10000 <= c && c <= 0x1000B) {
        return true;
    }
    if (0x1000D <= c && c <= 0x10026) {
        return true;
    }
    if (0x10028 <= c && c <= 0x1003A) {
        return true;
    }
    if (0x1003C <= c && c <= 0x1003D) {
        return true;
    }
    if (0x1003F <= c && c <= 0x1004D) {
        return true;
    }
    if (0x10050 <= c && c <= 0x1005D) {
        return true;
    }
    if (0x10080 <= c && c <= 0x100FA) {
        return true;
    }
    if (0x10140 <= c && c <= 0x10174) {
        return true;
    }
    if (0x10280 <= c && c <= 0x1029C) {
        return true;
    }
    if (0x102A0 <= c && c <= 0x102D0) {
        return true;
    }
    if (0x10300 <= c && c <= 0x1031F) {
        return true;
    }
    if (0x1032D <= c && c <= 0x10340) {
        return true;
    }
    if (0x10342 <= c && c <= 0x10349) {
        return true;
    }
    if (0x10350 <= c && c <= 0x10375) {
        return true;
    }
    if (0x10376 <= c && c <= 0x1037A) {
        return true;
    }
    if (0x10380 <= c && c <= 0x1039D) {
        return true;
    }
    if (0x103A0 <= c && c <= 0x103C3) {
        return true;
    }
    if (0x103C8 <= c && c <= 0x103CF) {
        return true;
    }
    if (0x103D1 <= c && c <= 0x103D5) {
        return true;
    }
    if (0x10400 <= c && c <= 0x1044F) {
        return true;
    }
    if (0x10450 <= c && c <= 0x1049D) {
        return true;
    }
    if (0x104B0 <= c && c <= 0x104D3) {
        return true;
    }
    if (0x104D8 <= c && c <= 0x104FB) {
        return true;
    }
    if (0x10500 <= c && c <= 0x10527) {
        return true;
    }
    if (0x10530 <= c && c <= 0x10563) {
        return true;
    }
    if (0x10570 <= c && c <= 0x1057A) {
        return true;
    }
    if (0x1057C <= c && c <= 0x1058A) {
        return true;
    }
    if (0x1058C <= c && c <= 0x10592) {
        return true;
    }
    if (0x10594 <= c && c <= 0x10595) {
        return true;
    }
    if (0x10597 <= c && c <= 0x105A1) {
        return true;
    }
    if (0x105A3 <= c && c <= 0x105B1) {
        return true;
    }
    if (0x105B3 <= c && c <= 0x105B9) {
        return true;
    }
    if (0x105BB <= c && c <= 0x105BC) {
        return true;
    }
    if (0x105C0 <= c && c <= 0x105F3) {
        return true;
    }
    if (0x10600 <= c && c <= 0x10736) {
        return true;
    }
    if (0x10740 <= c && c <= 0x10755) {
        return true;
    }
    if (0x10760 <= c && c <= 0x10767) {
        return true;
    }
    if (0x10780 <= c && c <= 0x10785) {
        return true;
    }
    if (0x10787 <= c && c <= 0x107B0) {
        return true;
    }
    if (0x107B2 <= c && c <= 0x107BA) {
        return true;
    }
    if (0x10800 <= c && c <= 0x10805) {
        return true;
    }
    if (0x1080A <= c && c <= 0x10835) {
        return true;
    }
    if (0x10837 <= c && c <= 0x10838) {
        return true;
    }
    if (0x1083F <= c && c <= 0x10855) {
        return true;
    }
    if (0x10860 <= c && c <= 0x10876) {
        return true;
    }
    if (0x10880 <= c && c <= 0x1089E) {
        return true;
    }
    if (0x108E0 <= c && c <= 0x108F2) {
        return true;
    }
    if (0x108F4 <= c && c <= 0x108F5) {
        return true;
    }
    if (0x10900 <= c && c <= 0x10915) {
        return true;
    }
    if (0x10920 <= c && c <= 0x10939) {
        return true;
    }
    if (0x10980 <= c && c <= 0x109B7) {
        return true;
    }
    if (0x109BE <= c && c <= 0x109BF) {
        return true;
    }
    if (0x10A01 <= c && c <= 0x10A03) {
        return true;
    }
    if (0x10A05 <= c && c <= 0x10A06) {
        return true;
    }
    if (0x10A0C <= c && c <= 0x10A0F) {
        return true;
    }
    if (0x10A10 <= c && c <= 0x10A13) {
        return true;
    }
    if (0x10A15 <= c && c <= 0x10A17) {
        return true;
    }
    if (0x10A19 <= c && c <= 0x10A35) {
        return true;
    }
    if (0x10A60 <= c && c <= 0x10A7C) {
        return true;
    }
    if (0x10A80 <= c && c <= 0x10A9C) {
        return true;
    }
    if (0x10AC0 <= c && c <= 0x10AC7) {
        return true;
    }
    if (0x10AC9 <= c && c <= 0x10AE4) {
        return true;
    }
    if (0x10B00 <= c && c <= 0x10B35) {
        return true;
    }
    if (0x10B40 <= c && c <= 0x10B55) {
        return true;
    }
    if (0x10B60 <= c && c <= 0x10B72) {
        return true;
    }
    if (0x10B80 <= c && c <= 0x10B91) {
        return true;
    }
    if (0x10C00 <= c && c <= 0x10C48) {
        return true;
    }
    if (0x10C80 <= c && c <= 0x10CB2) {
        return true;
    }
    if (0x10CC0 <= c && c <= 0x10CF2) {
        return true;
    }
    if (0x10D00 <= c && c <= 0x10D23) {
        return true;
    }
    if (0x10D24 <= c && c <= 0x10D27) {
        return true;
    }
    if (0x10D4A <= c && c <= 0x10D4D) {
        return true;
    }
    if (0x10D50 <= c && c <= 0x10D65) {
        return true;
    }
    if (0x10D70 <= c && c <= 0x10D85) {
        return true;
    }
    if (0x10E80 <= c && c <= 0x10EA9) {
        return true;
    }
    if (0x10EAB <= c && c <= 0x10EAC) {
        return true;
    }
    if (0x10EB0 <= c && c <= 0x10EB1) {
        return true;
    }
    if (0x10EC2 <= c && c <= 0x10EC4) {
        return true;
    }
    if (0x10F00 <= c && c <= 0x10F1C) {
        return true;
    }
    if (0x10F30 <= c && c <= 0x10F45) {
        return true;
    }
    if (0x10F70 <= c && c <= 0x10F81) {
        return true;
    }
    if (0x10FB0 <= c && c <= 0x10FC4) {
        return true;
    }
    if (0x10FE0 <= c && c <= 0x10FF6) {
        return true;
    }
    if (0x11003 <= c && c <= 0x11037) {
        return true;
    }
    if (0x11038 <= c && c <= 0x11045) {
        return true;
    }
    if (0x11071 <= c && c <= 0x11072) {
        return true;
    }
    if (0x11073 <= c && c <= 0x11074) {
        return true;
    }
    if (0x11080 <= c && c <= 0x11081) {
        return true;
    }
    if (0x11083 <= c && c <= 0x110AF) {
        return true;
    }
    if (0x110B0 <= c && c <= 0x110B2) {
        return true;
    }
    if (0x110B3 <= c && c <= 0x110B6) {
        return true;
    }
    if (0x110B7 <= c && c <= 0x110B8) {
        return true;
    }
    if (0x110D0 <= c && c <= 0x110E8) {
        return true;
    }
    if (0x11100 <= c && c <= 0x11102) {
        return true;
    }
    if (0x11103 <= c && c <= 0x11126) {
        return true;
    }
    if (0x11127 <= c && c <= 0x1112B) {
        return true;
    }
    if (0x1112D <= c && c <= 0x11132) {
        return true;
    }
    if (0x11145 <= c && c <= 0x11146) {
        return true;
    }
    if (0x11150 <= c && c <= 0x11172) {
        return true;
    }
    if (0x11180 <= c && c <= 0x11181) {
        return true;
    }
    if (0x11183 <= c && c <= 0x111B2) {
        return true;
    }
    if (0x111B3 <= c && c <= 0x111B5) {
        return true;
    }
    if (0x111B6 <= c && c <= 0x111BE) {
        return true;
    }
    if (0x111C1 <= c && c <= 0x111C4) {
        return true;
    }
    if (0x11200 <= c && c <= 0x11211) {
        return true;
    }
    if (0x11213 <= c && c <= 0x1122B) {
        return true;
    }
    if (0x1122C <= c && c <= 0x1122E) {
        return true;
    }
    if (0x1122F <= c && c <= 0x11231) {
        return true;
    }
    if (0x11232 <= c && c <= 0x11233) {
        return true;
    }
    if (0x1123F <= c && c <= 0x11240) {
        return true;
    }
    if (0x11280 <= c && c <= 0x11286) {
        return true;
    }
    if (0x1128A <= c && c <= 0x1128D) {
        return true;
    }
    if (0x1128F <= c && c <= 0x1129D) {
        return true;
    }
    if (0x1129F <= c && c <= 0x112A8) {
        return true;
    }
    if (0x112B0 <= c && c <= 0x112DE) {
        return true;
    }
    if (0x112E0 <= c && c <= 0x112E2) {
        return true;
    }
    if (0x112E3 <= c && c <= 0x112E8) {
        return true;
    }
    if (0x11300 <= c && c <= 0x11301) {
        return true;
    }
    if (0x11302 <= c && c <= 0x11303) {
        return true;
    }
    if (0x11305 <= c && c <= 0x1130C) {
        return true;
    }
    if (0x1130F <= c && c <= 0x11310) {
        return true;
    }
    if (0x11313 <= c && c <= 0x11328) {
        return true;
    }
    if (0x1132A <= c && c <= 0x11330) {
        return true;
    }
    if (0x11332 <= c && c <= 0x11333) {
        return true;
    }
    if (0x11335 <= c && c <= 0x11339) {
        return true;
    }
    if (0x1133E <= c && c <= 0x1133F) {
        return true;
    }
    if (0x11341 <= c && c <= 0x11344) {
        return true;
    }
    if (0x11347 <= c && c <= 0x11348) {
        return true;
    }
    if (0x1134B <= c && c <= 0x1134C) {
        return true;
    }
    if (0x1135D <= c && c <= 0x11361) {
        return true;
    }
    if (0x11362 <= c && c <= 0x11363) {
        return true;
    }
    if (0x11380 <= c && c <= 0x11389) {
        return true;
    }
    if (0x11390 <= c && c <= 0x113B5) {
        return true;
    }
    if (0x113B8 <= c && c <= 0x113BA) {
        return true;
    }
    if (0x113BB <= c && c <= 0x113C0) {
        return true;
    }
    if (0x113C7 <= c && c <= 0x113CA) {
        return true;
    }
    if (0x113CC <= c && c <= 0x113CD) {
        return true;
    }
    if (0x11400 <= c && c <= 0x11434) {
        return true;
    }
    if (0x11435 <= c && c <= 0x11437) {
        return true;
    }
    if (0x11438 <= c && c <= 0x1143F) {
        return true;
    }
    if (0x11440 <= c && c <= 0x11441) {
        return true;
    }
    if (0x11443 <= c && c <= 0x11444) {
        return true;
    }
    if (0x11447 <= c && c <= 0x1144A) {
        return true;
    }
    if (0x1145F <= c && c <= 0x11461) {
        return true;
    }
    if (0x11480 <= c && c <= 0x114AF) {
        return true;
    }
    if (0x114B0 <= c && c <= 0x114B2) {
        return true;
    }
    if (0x114B3 <= c && c <= 0x114B8) {
        return true;
    }
    if (0x114BB <= c && c <= 0x114BE) {
        return true;
    }
    if (0x114BF <= c && c <= 0x114C0) {
        return true;
    }
    if (0x114C4 <= c && c <= 0x114C5) {
        return true;
    }
    if (0x11580 <= c && c <= 0x115AE) {
        return true;
    }
    if (0x115AF <= c && c <= 0x115B1) {
        return true;
    }
    if (0x115B2 <= c && c <= 0x115B5) {
        return true;
    }
    if (0x115B8 <= c && c <= 0x115BB) {
        return true;
    }
    if (0x115BC <= c && c <= 0x115BD) {
        return true;
    }
    if (0x115D8 <= c && c <= 0x115DB) {
        return true;
    }
    if (0x115DC <= c && c <= 0x115DD) {
        return true;
    }
    if (0x11600 <= c && c <= 0x1162F) {
        return true;
    }
    if (0x11630 <= c && c <= 0x11632) {
        return true;
    }
    if (0x11633 <= c && c <= 0x1163A) {
        return true;
    }
    if (0x1163B <= c && c <= 0x1163C) {
        return true;
    }
    if (0x11680 <= c && c <= 0x116AA) {
        return true;
    }
    if (0x116AE <= c && c <= 0x116AF) {
        return true;
    }
    if (0x116B0 <= c && c <= 0x116B5) {
        return true;
    }
    if (0x11700 <= c && c <= 0x1171A) {
        return true;
    }
    if (0x11720 <= c && c <= 0x11721) {
        return true;
    }
    if (0x11722 <= c && c <= 0x11725) {
        return true;
    }
    if (0x11727 <= c && c <= 0x1172A) {
        return true;
    }
    if (0x11740 <= c && c <= 0x11746) {
        return true;
    }
    if (0x11800 <= c && c <= 0x1182B) {
        return true;
    }
    if (0x1182C <= c && c <= 0x1182E) {
        return true;
    }
    if (0x1182F <= c && c <= 0x11837) {
        return true;
    }
    if (0x118A0 <= c && c <= 0x118DF) {
        return true;
    }
    if (0x118FF <= c && c <= 0x11906) {
        return true;
    }
    if (0x1190C <= c && c <= 0x11913) {
        return true;
    }
    if (0x11915 <= c && c <= 0x11916) {
        return true;
    }
    if (0x11918 <= c && c <= 0x1192F) {
        return true;
    }
    if (0x11930 <= c && c <= 0x11935) {
        return true;
    }
    if (0x11937 <= c && c <= 0x11938) {
        return true;
    }
    if (0x1193B <= c && c <= 0x1193C) {
        return true;
    }
    if (0x119A0 <= c && c <= 0x119A7) {
        return true;
    }
    if (0x119AA <= c && c <= 0x119D0) {
        return true;
    }
    if (0x119D1 <= c && c <= 0x119D3) {
        return true;
    }
    if (0x119D4 <= c && c <= 0x119D7) {
        return true;
    }
    if (0x119DA <= c && c <= 0x119DB) {
        return true;
    }
    if (0x119DC <= c && c <= 0x119DF) {
        return true;
    }
    if (0x11A01 <= c && c <= 0x11A0A) {
        return true;
    }
    if (0x11A0B <= c && c <= 0x11A32) {
        return true;
    }
    if (0x11A35 <= c && c <= 0x11A38) {
        return true;
    }
    if (0x11A3B <= c && c <= 0x11A3E) {
        return true;
    }
    if (0x11A51 <= c && c <= 0x11A56) {
        return true;
    }
    if (0x11A57 <= c && c <= 0x11A58) {
        return true;
    }
    if (0x11A59 <= c && c <= 0x11A5B) {
        return true;
    }
    if (0x11A5C <= c && c <= 0x11A89) {
        return true;
    }
    if (0x11A8A <= c && c <= 0x11A96) {
        return true;
    }
    if (0x11AB0 <= c && c <= 0x11AF8) {
        return true;
    }
    if (0x11BC0 <= c && c <= 0x11BE0) {
        return true;
    }
    if (0x11C00 <= c && c <= 0x11C08) {
        return true;
    }
    if (0x11C0A <= c && c <= 0x11C2E) {
        return true;
    }
    if (0x11C30 <= c && c <= 0x11C36) {
        return true;
    }
    if (0x11C38 <= c && c <= 0x11C3D) {
        return true;
    }
    if (0x11C72 <= c && c <= 0x11C8F) {
        return true;
    }
    if (0x11C92 <= c && c <= 0x11CA7) {
        return true;
    }
    if (0x11CAA <= c && c <= 0x11CB0) {
        return true;
    }
    if (0x11CB2 <= c && c <= 0x11CB3) {
        return true;
    }
    if (0x11CB5 <= c && c <= 0x11CB6) {
        return true;
    }
    if (0x11D00 <= c && c <= 0x11D06) {
        return true;
    }
    if (0x11D08 <= c && c <= 0x11D09) {
        return true;
    }
    if (0x11D0B <= c && c <= 0x11D30) {
        return true;
    }
    if (0x11D31 <= c && c <= 0x11D36) {
        return true;
    }
    if (0x11D3C <= c && c <= 0x11D3D) {
        return true;
    }
    if (0x11D3F <= c && c <= 0x11D41) {
        return true;
    }
    if (0x11D60 <= c && c <= 0x11D65) {
        return true;
    }
    if (0x11D67 <= c && c <= 0x11D68) {
        return true;
    }
    if (0x11D6A <= c && c <= 0x11D89) {
        return true;
    }
    if (0x11D8A <= c && c <= 0x11D8E) {
        return true;
    }
    if (0x11D90 <= c && c <= 0x11D91) {
        return true;
    }
    if (0x11D93 <= c && c <= 0x11D94) {
        return true;
    }
    if (0x11EE0 <= c && c <= 0x11EF2) {
        return true;
    }
    if (0x11EF3 <= c && c <= 0x11EF4) {
        return true;
    }
    if (0x11EF5 <= c && c <= 0x11EF6) {
        return true;
    }
    if (0x11F00 <= c && c <= 0x11F01) {
        return true;
    }
    if (0x11F04 <= c && c <= 0x11F10) {
        return true;
    }
    if (0x11F12 <= c && c <= 0x11F33) {
        return true;
    }
    if (0x11F34 <= c && c <= 0x11F35) {
        return true;
    }
    if (0x11F36 <= c && c <= 0x11F3A) {
        return true;
    }
    if (0x11F3E <= c && c <= 0x11F3F) {
        return true;
    }
    if (0x12000 <= c && c <= 0x12399) {
        return true;
    }
    if (0x12400 <= c && c <= 0x1246E) {
        return true;
    }
    if (0x12480 <= c && c <= 0x12543) {
        return true;
    }
    if (0x12F90 <= c && c <= 0x12FF0) {
        return true;
    }
    if (0x13000 <= c && c <= 0x1342F) {
        return true;
    }
    if (0x13441 <= c && c <= 0x13446) {
        return true;
    }
    if (0x13460 <= c && c <= 0x143FA) {
        return true;
    }
    if (0x14400 <= c && c <= 0x14646) {
        return true;
    }
    if (0x16100 <= c && c <= 0x1611D) {
        return true;
    }
    if (0x1611E <= c && c <= 0x16129) {
        return true;
    }
    if (0x1612A <= c && c <= 0x1612C) {
        return true;
    }
    if (0x1612D <= c && c <= 0x1612E) {
        return true;
    }
    if (0x16800 <= c && c <= 0x16A38) {
        return true;
    }
    if (0x16A40 <= c && c <= 0x16A5E) {
        return true;
    }
    if (0x16A70 <= c && c <= 0x16ABE) {
        return true;
    }
    if (0x16AD0 <= c && c <= 0x16AED) {
        return true;
    }
    if (0x16B00 <= c && c <= 0x16B2F) {
        return true;
    }
    if (0x16B40 <= c && c <= 0x16B43) {
        return true;
    }
    if (0x16B63 <= c && c <= 0x16B77) {
        return true;
    }
    if (0x16B7D <= c && c <= 0x16B8F) {
        return true;
    }
    if (0x16D40 <= c && c <= 0x16D42) {
        return true;
    }
    if (0x16D43 <= c && c <= 0x16D6A) {
        return true;
    }
    if (0x16D6B <= c && c <= 0x16D6C) {
        return true;
    }
    if (0x16E40 <= c && c <= 0x16E7F) {
        return true;
    }
    if (0x16F00 <= c && c <= 0x16F4A) {
        return true;
    }
    if (0x16F51 <= c && c <= 0x16F87) {
        return true;
    }
    if (0x16F8F <= c && c <= 0x16F92) {
        return true;
    }
    if (0x16F93 <= c && c <= 0x16F9F) {
        return true;
    }
    if (0x16FE0 <= c && c <= 0x16FE1) {
        return true;
    }
    if (0x16FF0 <= c && c <= 0x16FF1) {
        return true;
    }
    if (0x17000 <= c && c <= 0x187F7) {
        return true;
    }
    if (0x18800 <= c && c <= 0x18CD5) {
        return true;
    }
    if (0x18CFF <= c && c <= 0x18D08) {
        return true;
    }
    if (0x1AFF0 <= c && c <= 0x1AFF3) {
        return true;
    }
    if (0x1AFF5 <= c && c <= 0x1AFFB) {
        return true;
    }
    if (0x1AFFD <= c && c <= 0x1AFFE) {
        return true;
    }
    if (0x1B000 <= c && c <= 0x1B122) {
        return true;
    }
    if (0x1B150 <= c && c <= 0x1B152) {
        return true;
    }
    if (0x1B164 <= c && c <= 0x1B167) {
        return true;
    }
    if (0x1B170 <= c && c <= 0x1B2FB) {
        return true;
    }
    if (0x1BC00 <= c && c <= 0x1BC6A) {
        return true;
    }
    if (0x1BC70 <= c && c <= 0x1BC7C) {
        return true;
    }
    if (0x1BC80 <= c && c <= 0x1BC88) {
        return true;
    }
    if (0x1BC90 <= c && c <= 0x1BC99) {
        return true;
    }
    if (0x1D400 <= c && c <= 0x1D454) {
        return true;
    }
    if (0x1D456 <= c && c <= 0x1D49C) {
        return true;
    }
    if (0x1D49E <= c && c <= 0x1D49F) {
        return true;
    }
    if (0x1D4A5 <= c && c <= 0x1D4A6) {
        return true;
    }
    if (0x1D4A9 <= c && c <= 0x1D4AC) {
        return true;
    }
    if (0x1D4AE <= c && c <= 0x1D4B9) {
        return true;
    }
    if (0x1D4BD <= c && c <= 0x1D4C3) {
        return true;
    }
    if (0x1D4C5 <= c && c <= 0x1D505) {
        return true;
    }
    if (0x1D507 <= c && c <= 0x1D50A) {
        return true;
    }
    if (0x1D50D <= c && c <= 0x1D514) {
        return true;
    }
    if (0x1D516 <= c && c <= 0x1D51C) {
        return true;
    }
    if (0x1D51E <= c && c <= 0x1D539) {
        return true;
    }
    if (0x1D53B <= c && c <= 0x1D53E) {
        return true;
    }
    if (0x1D540 <= c && c <= 0x1D544) {
        return true;
    }
    if (0x1D54A <= c && c <= 0x1D550) {
        return true;
    }
    if (0x1D552 <= c && c <= 0x1D6A5) {
        return true;
    }
    if (0x1D6A8 <= c && c <= 0x1D6C0) {
        return true;
    }
    if (0x1D6C2 <= c && c <= 0x1D6DA) {
        return true;
    }
    if (0x1D6DC <= c && c <= 0x1D6FA) {
        return true;
    }
    if (0x1D6FC <= c && c <= 0x1D714) {
        return true;
    }
    if (0x1D716 <= c && c <= 0x1D734) {
        return true;
    }
    if (0x1D736 <= c && c <= 0x1D74E) {
        return true;
    }
    if (0x1D750 <= c && c <= 0x1D76E) {
        return true;
    }
    if (0x1D770 <= c && c <= 0x1D788) {
        return true;
    }
    if (0x1D78A <= c && c <= 0x1D7A8) {
        return true;
    }
    if (0x1D7AA <= c && c <= 0x1D7C2) {
        return true;
    }
    if (0x1D7C4 <= c && c <= 0x1D7CB) {
        return true;
    }
    if (0x1DF00 <= c && c <= 0x1DF09) {
        return true;
    }
    if (0x1DF0B <= c && c <= 0x1DF1E) {
        return true;
    }
    if (0x1DF25 <= c && c <= 0x1DF2A) {
        return true;
    }
    if (0x1E000 <= c && c <= 0x1E006) {
        return true;
    }
    if (0x1E008 <= c && c <= 0x1E018) {
        return true;
    }
    if (0x1E01B <= c && c <= 0x1E021) {
        return true;
    }
    if (0x1E023 <= c && c <= 0x1E024) {
        return true;
    }
    if (0x1E026 <= c && c <= 0x1E02A) {
        return true;
    }
    if (0x1E030 <= c && c <= 0x1E06D) {
        return true;
    }
    if (0x1E100 <= c && c <= 0x1E12C) {
        return true;
    }
    if (0x1E137 <= c && c <= 0x1E13D) {
        return true;
    }
    if (0x1E290 <= c && c <= 0x1E2AD) {
        return true;
    }
    if (0x1E2C0 <= c && c <= 0x1E2EB) {
        return true;
    }
    if (0x1E4D0 <= c && c <= 0x1E4EA) {
        return true;
    }
    if (0x1E5D0 <= c && c <= 0x1E5ED) {
        return true;
    }
    if (0x1E7E0 <= c && c <= 0x1E7E6) {
        return true;
    }
    if (0x1E7E8 <= c && c <= 0x1E7EB) {
        return true;
    }
    if (0x1E7ED <= c && c <= 0x1E7EE) {
        return true;
    }
    if (0x1E7F0 <= c && c <= 0x1E7FE) {
        return true;
    }
    if (0x1E800 <= c && c <= 0x1E8C4) {
        return true;
    }
    if (0x1E900 <= c && c <= 0x1E943) {
        return true;
    }
    if (0x1EE00 <= c && c <= 0x1EE03) {
        return true;
    }
    if (0x1EE05 <= c && c <= 0x1EE1F) {
        return true;
    }
    if (0x1EE21 <= c && c <= 0x1EE22) {
        return true;
    }
    if (0x1EE29 <= c && c <= 0x1EE32) {
        return true;
    }
    if (0x1EE34 <= c && c <= 0x1EE37) {
        return true;
    }
    if (0x1EE4D <= c && c <= 0x1EE4F) {
        return true;
    }
    if (0x1EE51 <= c && c <= 0x1EE52) {
        return true;
    }
    if (0x1EE61 <= c && c <= 0x1EE62) {
        return true;
    }
    if (0x1EE67 <= c && c <= 0x1EE6A) {
        return true;
    }
    if (0x1EE6C <= c && c <= 0x1EE72) {
        return true;
    }
    if (0x1EE74 <= c && c <= 0x1EE77) {
        return true;
    }
    if (0x1EE79 <= c && c <= 0x1EE7C) {
        return true;
    }
    if (0x1EE80 <= c && c <= 0x1EE89) {
        return true;
    }
    if (0x1EE8B <= c && c <= 0x1EE9B) {
        return true;
    }
    if (0x1EEA1 <= c && c <= 0x1EEA3) {
        return true;
    }
    if (0x1EEA5 <= c && c <= 0x1EEA9) {
        return true;
    }
    if (0x1EEAB <= c && c <= 0x1EEBB) {
        return true;
    }
    if (0x1F130 <= c && c <= 0x1F149) {
        return true;
    }
    if (0x1F150 <= c && c <= 0x1F169) {
        return true;
    }
    if (0x1F170 <= c && c <= 0x1F189) {
        return true;
    }
    if (0x20000 <= c && c <= 0x2A6DF) {
        return true;
    }
    if (0x2A700 <= c && c <= 0x2B739) {
        return true;
    }
    if (0x2B740 <= c && c <= 0x2B81D) {
        return true;
    }
    if (0x2B820 <= c && c <= 0x2CEA1) {
        return true;
    }
    if (0x2CEB0 <= c && c <= 0x2EBE0) {
        return true;
    }
    if (0x2EBF0 <= c && c <= 0x2EE5D) {
        return true;
    }
    if (0x2F800 <= c && c <= 0x2FA1D) {
        return true;
    }
    if (0x30000 <= c && c <= 0x3134A) {
        return true;
    }
    if (0x31350 <= c && c <= 0x323AF) {
        return true;
    }
    return false;
}

bool bc_uprop_is_upper(int32_t c) {
    switch (c) {
    case 0x0100:
    case 0x0102:
    case 0x0104:
    case 0x0106:
    case 0x0108:
    case 0x010A:
    case 0x010C:
    case 0x010E:
    case 0x0110:
    case 0x0112:
    case 0x0114:
    case 0x0116:
    case 0x0118:
    case 0x011A:
    case 0x011C:
    case 0x011E:
    case 0x0120:
    case 0x0122:
    case 0x0124:
    case 0x0126:
    case 0x0128:
    case 0x012A:
    case 0x012C:
    case 0x012E:
    case 0x0130:
    case 0x0132:
    case 0x0134:
    case 0x0136:
    case 0x0139:
    case 0x013B:
    case 0x013D:
    case 0x013F:
    case 0x0141:
    case 0x0143:
    case 0x0145:
    case 0x0147:
    case 0x014A:
    case 0x014C:
    case 0x014E:
    case 0x0150:
    case 0x0152:
    case 0x0154:
    case 0x0156:
    case 0x0158:
    case 0x015A:
    case 0x015C:
    case 0x015E:
    case 0x0160:
    case 0x0162:
    case 0x0164:
    case 0x0166:
    case 0x0168:
    case 0x016A:
    case 0x016C:
    case 0x016E:
    case 0x0170:
    case 0x0172:
    case 0x0174:
    case 0x0176:
    case 0x017B:
    case 0x017D:
    case 0x0184:
    case 0x01A2:
    case 0x01A4:
    case 0x01A9:
    case 0x01AC:
    case 0x01B5:
    case 0x01BC:
    case 0x01C4:
    case 0x01C7:
    case 0x01CA:
    case 0x01CD:
    case 0x01CF:
    case 0x01D1:
    case 0x01D3:
    case 0x01D5:
    case 0x01D7:
    case 0x01D9:
    case 0x01DB:
    case 0x01DE:
    case 0x01E0:
    case 0x01E2:
    case 0x01E4:
    case 0x01E6:
    case 0x01E8:
    case 0x01EA:
    case 0x01EC:
    case 0x01EE:
    case 0x01F1:
    case 0x01F4:
    case 0x01FA:
    case 0x01FC:
    case 0x01FE:
    case 0x0200:
    case 0x0202:
    case 0x0204:
    case 0x0206:
    case 0x0208:
    case 0x020A:
    case 0x020C:
    case 0x020E:
    case 0x0210:
    case 0x0212:
    case 0x0214:
    case 0x0216:
    case 0x0218:
    case 0x021A:
    case 0x021C:
    case 0x021E:
    case 0x0220:
    case 0x0222:
    case 0x0224:
    case 0x0226:
    case 0x0228:
    case 0x022A:
    case 0x022C:
    case 0x022E:
    case 0x0230:
    case 0x0232:
    case 0x0241:
    case 0x0248:
    case 0x024A:
    case 0x024C:
    case 0x024E:
    case 0x0370:
    case 0x0372:
    case 0x0376:
    case 0x037F:
    case 0x0386:
    case 0x038C:
    case 0x03CF:
    case 0x03D8:
    case 0x03DA:
    case 0x03DC:
    case 0x03DE:
    case 0x03E0:
    case 0x03E2:
    case 0x03E4:
    case 0x03E6:
    case 0x03E8:
    case 0x03EA:
    case 0x03EC:
    case 0x03EE:
    case 0x03F4:
    case 0x03F7:
    case 0x0460:
    case 0x0462:
    case 0x0464:
    case 0x0466:
    case 0x0468:
    case 0x046A:
    case 0x046C:
    case 0x046E:
    case 0x0470:
    case 0x0472:
    case 0x0474:
    case 0x0476:
    case 0x0478:
    case 0x047A:
    case 0x047C:
    case 0x047E:
    case 0x0480:
    case 0x048A:
    case 0x048C:
    case 0x048E:
    case 0x0490:
    case 0x0492:
    case 0x0494:
    case 0x0496:
    case 0x0498:
    case 0x049A:
    case 0x049C:
    case 0x049E:
    case 0x04A0:
    case 0x04A2:
    case 0x04A4:
    case 0x04A6:
    case 0x04A8:
    case 0x04AA:
    case 0x04AC:
    case 0x04AE:
    case 0x04B0:
    case 0x04B2:
    case 0x04B4:
    case 0x04B6:
    case 0x04B8:
    case 0x04BA:
    case 0x04BC:
    case 0x04BE:
    case 0x04C3:
    case 0x04C5:
    case 0x04C7:
    case 0x04C9:
    case 0x04CB:
    case 0x04CD:
    case 0x04D0:
    case 0x04D2:
    case 0x04D4:
    case 0x04D6:
    case 0x04D8:
    case 0x04DA:
    case 0x04DC:
    case 0x04DE:
    case 0x04E0:
    case 0x04E2:
    case 0x04E4:
    case 0x04E6:
    case 0x04E8:
    case 0x04EA:
    case 0x04EC:
    case 0x04EE:
    case 0x04F0:
    case 0x04F2:
    case 0x04F4:
    case 0x04F6:
    case 0x04F8:
    case 0x04FA:
    case 0x04FC:
    case 0x04FE:
    case 0x0500:
    case 0x0502:
    case 0x0504:
    case 0x0506:
    case 0x0508:
    case 0x050A:
    case 0x050C:
    case 0x050E:
    case 0x0510:
    case 0x0512:
    case 0x0514:
    case 0x0516:
    case 0x0518:
    case 0x051A:
    case 0x051C:
    case 0x051E:
    case 0x0520:
    case 0x0522:
    case 0x0524:
    case 0x0526:
    case 0x0528:
    case 0x052A:
    case 0x052C:
    case 0x052E:
    case 0x10C7:
    case 0x10CD:
    case 0x1C89:
    case 0x1E00:
    case 0x1E02:
    case 0x1E04:
    case 0x1E06:
    case 0x1E08:
    case 0x1E0A:
    case 0x1E0C:
    case 0x1E0E:
    case 0x1E10:
    case 0x1E12:
    case 0x1E14:
    case 0x1E16:
    case 0x1E18:
    case 0x1E1A:
    case 0x1E1C:
    case 0x1E1E:
    case 0x1E20:
    case 0x1E22:
    case 0x1E24:
    case 0x1E26:
    case 0x1E28:
    case 0x1E2A:
    case 0x1E2C:
    case 0x1E2E:
    case 0x1E30:
    case 0x1E32:
    case 0x1E34:
    case 0x1E36:
    case 0x1E38:
    case 0x1E3A:
    case 0x1E3C:
    case 0x1E3E:
    case 0x1E40:
    case 0x1E42:
    case 0x1E44:
    case 0x1E46:
    case 0x1E48:
    case 0x1E4A:
    case 0x1E4C:
    case 0x1E4E:
    case 0x1E50:
    case 0x1E52:
    case 0x1E54:
    case 0x1E56:
    case 0x1E58:
    case 0x1E5A:
    case 0x1E5C:
    case 0x1E5E:
    case 0x1E60:
    case 0x1E62:
    case 0x1E64:
    case 0x1E66:
    case 0x1E68:
    case 0x1E6A:
    case 0x1E6C:
    case 0x1E6E:
    case 0x1E70:
    case 0x1E72:
    case 0x1E74:
    case 0x1E76:
    case 0x1E78:
    case 0x1E7A:
    case 0x1E7C:
    case 0x1E7E:
    case 0x1E80:
    case 0x1E82:
    case 0x1E84:
    case 0x1E86:
    case 0x1E88:
    case 0x1E8A:
    case 0x1E8C:
    case 0x1E8E:
    case 0x1E90:
    case 0x1E92:
    case 0x1E94:
    case 0x1E9E:
    case 0x1EA0:
    case 0x1EA2:
    case 0x1EA4:
    case 0x1EA6:
    case 0x1EA8:
    case 0x1EAA:
    case 0x1EAC:
    case 0x1EAE:
    case 0x1EB0:
    case 0x1EB2:
    case 0x1EB4:
    case 0x1EB6:
    case 0x1EB8:
    case 0x1EBA:
    case 0x1EBC:
    case 0x1EBE:
    case 0x1EC0:
    case 0x1EC2:
    case 0x1EC4:
    case 0x1EC6:
    case 0x1EC8:
    case 0x1ECA:
    case 0x1ECC:
    case 0x1ECE:
    case 0x1ED0:
    case 0x1ED2:
    case 0x1ED4:
    case 0x1ED6:
    case 0x1ED8:
    case 0x1EDA:
    case 0x1EDC:
    case 0x1EDE:
    case 0x1EE0:
    case 0x1EE2:
    case 0x1EE4:
    case 0x1EE6:
    case 0x1EE8:
    case 0x1EEA:
    case 0x1EEC:
    case 0x1EEE:
    case 0x1EF0:
    case 0x1EF2:
    case 0x1EF4:
    case 0x1EF6:
    case 0x1EF8:
    case 0x1EFA:
    case 0x1EFC:
    case 0x1EFE:
    case 0x1F59:
    case 0x1F5B:
    case 0x1F5D:
    case 0x1F5F:
    case 0x2102:
    case 0x2107:
    case 0x2115:
    case 0x2124:
    case 0x2126:
    case 0x2128:
    case 0x2145:
    case 0x2183:
    case 0x2C60:
    case 0x2C67:
    case 0x2C69:
    case 0x2C6B:
    case 0x2C72:
    case 0x2C75:
    case 0x2C82:
    case 0x2C84:
    case 0x2C86:
    case 0x2C88:
    case 0x2C8A:
    case 0x2C8C:
    case 0x2C8E:
    case 0x2C90:
    case 0x2C92:
    case 0x2C94:
    case 0x2C96:
    case 0x2C98:
    case 0x2C9A:
    case 0x2C9C:
    case 0x2C9E:
    case 0x2CA0:
    case 0x2CA2:
    case 0x2CA4:
    case 0x2CA6:
    case 0x2CA8:
    case 0x2CAA:
    case 0x2CAC:
    case 0x2CAE:
    case 0x2CB0:
    case 0x2CB2:
    case 0x2CB4:
    case 0x2CB6:
    case 0x2CB8:
    case 0x2CBA:
    case 0x2CBC:
    case 0x2CBE:
    case 0x2CC0:
    case 0x2CC2:
    case 0x2CC4:
    case 0x2CC6:
    case 0x2CC8:
    case 0x2CCA:
    case 0x2CCC:
    case 0x2CCE:
    case 0x2CD0:
    case 0x2CD2:
    case 0x2CD4:
    case 0x2CD6:
    case 0x2CD8:
    case 0x2CDA:
    case 0x2CDC:
    case 0x2CDE:
    case 0x2CE0:
    case 0x2CE2:
    case 0x2CEB:
    case 0x2CED:
    case 0x2CF2:
    case 0xA640:
    case 0xA642:
    case 0xA644:
    case 0xA646:
    case 0xA648:
    case 0xA64A:
    case 0xA64C:
    case 0xA64E:
    case 0xA650:
    case 0xA652:
    case 0xA654:
    case 0xA656:
    case 0xA658:
    case 0xA65A:
    case 0xA65C:
    case 0xA65E:
    case 0xA660:
    case 0xA662:
    case 0xA664:
    case 0xA666:
    case 0xA668:
    case 0xA66A:
    case 0xA66C:
    case 0xA680:
    case 0xA682:
    case 0xA684:
    case 0xA686:
    case 0xA688:
    case 0xA68A:
    case 0xA68C:
    case 0xA68E:
    case 0xA690:
    case 0xA692:
    case 0xA694:
    case 0xA696:
    case 0xA698:
    case 0xA69A:
    case 0xA722:
    case 0xA724:
    case 0xA726:
    case 0xA728:
    case 0xA72A:
    case 0xA72C:
    case 0xA72E:
    case 0xA732:
    case 0xA734:
    case 0xA736:
    case 0xA738:
    case 0xA73A:
    case 0xA73C:
    case 0xA73E:
    case 0xA740:
    case 0xA742:
    case 0xA744:
    case 0xA746:
    case 0xA748:
    case 0xA74A:
    case 0xA74C:
    case 0xA74E:
    case 0xA750:
    case 0xA752:
    case 0xA754:
    case 0xA756:
    case 0xA758:
    case 0xA75A:
    case 0xA75C:
    case 0xA75E:
    case 0xA760:
    case 0xA762:
    case 0xA764:
    case 0xA766:
    case 0xA768:
    case 0xA76A:
    case 0xA76C:
    case 0xA76E:
    case 0xA779:
    case 0xA77B:
    case 0xA780:
    case 0xA782:
    case 0xA784:
    case 0xA786:
    case 0xA78B:
    case 0xA78D:
    case 0xA790:
    case 0xA792:
    case 0xA796:
    case 0xA798:
    case 0xA79A:
    case 0xA79C:
    case 0xA79E:
    case 0xA7A0:
    case 0xA7A2:
    case 0xA7A4:
    case 0xA7A6:
    case 0xA7A8:
    case 0xA7B6:
    case 0xA7B8:
    case 0xA7BA:
    case 0xA7BC:
    case 0xA7BE:
    case 0xA7C0:
    case 0xA7C2:
    case 0xA7C9:
    case 0xA7D0:
    case 0xA7D6:
    case 0xA7D8:
    case 0xA7DA:
    case 0xA7DC:
    case 0xA7F5:
    case 0x1D49C:
    case 0x1D4A2:
    case 0x1D546:
    case 0x1D7CA:
        return true;
    }
    if (0x0041 <= c && c <= 0x005A) {
        return true;
    }
    if (0x00C0 <= c && c <= 0x00D6) {
        return true;
    }
    if (0x00D8 <= c && c <= 0x00DE) {
        return true;
    }
    if (0x0178 <= c && c <= 0x0179) {
        return true;
    }
    if (0x0181 <= c && c <= 0x0182) {
        return true;
    }
    if (0x0186 <= c && c <= 0x0187) {
        return true;
    }
    if (0x0189 <= c && c <= 0x018B) {
        return true;
    }
    if (0x018E <= c && c <= 0x0191) {
        return true;
    }
    if (0x0193 <= c && c <= 0x0194) {
        return true;
    }
    if (0x0196 <= c && c <= 0x0198) {
        return true;
    }
    if (0x019C <= c && c <= 0x019D) {
        return true;
    }
    if (0x019F <= c && c <= 0x01A0) {
        return true;
    }
    if (0x01A6 <= c && c <= 0x01A7) {
        return true;
    }
    if (0x01AE <= c && c <= 0x01AF) {
        return true;
    }
    if (0x01B1 <= c && c <= 0x01B3) {
        return true;
    }
    if (0x01B7 <= c && c <= 0x01B8) {
        return true;
    }
    if (0x01F6 <= c && c <= 0x01F8) {
        return true;
    }
    if (0x023A <= c && c <= 0x023B) {
        return true;
    }
    if (0x023D <= c && c <= 0x023E) {
        return true;
    }
    if (0x0243 <= c && c <= 0x0246) {
        return true;
    }
    if (0x0388 <= c && c <= 0x038A) {
        return true;
    }
    if (0x038E <= c && c <= 0x038F) {
        return true;
    }
    if (0x0391 <= c && c <= 0x03A1) {
        return true;
    }
    if (0x03A3 <= c && c <= 0x03AB) {
        return true;
    }
    if (0x03D2 <= c && c <= 0x03D4) {
        return true;
    }
    if (0x03F9 <= c && c <= 0x03FA) {
        return true;
    }
    if (0x03FD <= c && c <= 0x042F) {
        return true;
    }
    if (0x04C0 <= c && c <= 0x04C1) {
        return true;
    }
    if (0x0531 <= c && c <= 0x0556) {
        return true;
    }
    if (0x10A0 <= c && c <= 0x10C5) {
        return true;
    }
    if (0x13A0 <= c && c <= 0x13F5) {
        return true;
    }
    if (0x1C90 <= c && c <= 0x1CBA) {
        return true;
    }
    if (0x1CBD <= c && c <= 0x1CBF) {
        return true;
    }
    if (0x1F08 <= c && c <= 0x1F0F) {
        return true;
    }
    if (0x1F18 <= c && c <= 0x1F1D) {
        return true;
    }
    if (0x1F28 <= c && c <= 0x1F2F) {
        return true;
    }
    if (0x1F38 <= c && c <= 0x1F3F) {
        return true;
    }
    if (0x1F48 <= c && c <= 0x1F4D) {
        return true;
    }
    if (0x1F68 <= c && c <= 0x1F6F) {
        return true;
    }
    if (0x1FB8 <= c && c <= 0x1FBB) {
        return true;
    }
    if (0x1FC8 <= c && c <= 0x1FCB) {
        return true;
    }
    if (0x1FD8 <= c && c <= 0x1FDB) {
        return true;
    }
    if (0x1FE8 <= c && c <= 0x1FEC) {
        return true;
    }
    if (0x1FF8 <= c && c <= 0x1FFB) {
        return true;
    }
    if (0x210B <= c && c <= 0x210D) {
        return true;
    }
    if (0x2110 <= c && c <= 0x2112) {
        return true;
    }
    if (0x2119 <= c && c <= 0x211D) {
        return true;
    }
    if (0x212A <= c && c <= 0x212D) {
        return true;
    }
    if (0x2130 <= c && c <= 0x2133) {
        return true;
    }
    if (0x213E <= c && c <= 0x213F) {
        return true;
    }
    if (0x2160 <= c && c <= 0x216F) {
        return true;
    }
    if (0x24B6 <= c && c <= 0x24CF) {
        return true;
    }
    if (0x2C00 <= c && c <= 0x2C2F) {
        return true;
    }
    if (0x2C62 <= c && c <= 0x2C64) {
        return true;
    }
    if (0x2C6D <= c && c <= 0x2C70) {
        return true;
    }
    if (0x2C7E <= c && c <= 0x2C80) {
        return true;
    }
    if (0xA77D <= c && c <= 0xA77E) {
        return true;
    }
    if (0xA7AA <= c && c <= 0xA7AE) {
        return true;
    }
    if (0xA7B0 <= c && c <= 0xA7B4) {
        return true;
    }
    if (0xA7C4 <= c && c <= 0xA7C7) {
        return true;
    }
    if (0xA7CB <= c && c <= 0xA7CC) {
        return true;
    }
    if (0xFF21 <= c && c <= 0xFF3A) {
        return true;
    }
    if (0x10400 <= c && c <= 0x10427) {
        return true;
    }
    if (0x104B0 <= c && c <= 0x104D3) {
        return true;
    }
    if (0x10570 <= c && c <= 0x1057A) {
        return true;
    }
    if (0x1057C <= c && c <= 0x1058A) {
        return true;
    }
    if (0x1058C <= c && c <= 0x10592) {
        return true;
    }
    if (0x10594 <= c && c <= 0x10595) {
        return true;
    }
    if (0x10C80 <= c && c <= 0x10CB2) {
        return true;
    }
    if (0x10D50 <= c && c <= 0x10D65) {
        return true;
    }
    if (0x118A0 <= c && c <= 0x118BF) {
        return true;
    }
    if (0x16E40 <= c && c <= 0x16E5F) {
        return true;
    }
    if (0x1D400 <= c && c <= 0x1D419) {
        return true;
    }
    if (0x1D434 <= c && c <= 0x1D44D) {
        return true;
    }
    if (0x1D468 <= c && c <= 0x1D481) {
        return true;
    }
    if (0x1D49E <= c && c <= 0x1D49F) {
        return true;
    }
    if (0x1D4A5 <= c && c <= 0x1D4A6) {
        return true;
    }
    if (0x1D4A9 <= c && c <= 0x1D4AC) {
        return true;
    }
    if (0x1D4AE <= c && c <= 0x1D4B5) {
        return true;
    }
    if (0x1D4D0 <= c && c <= 0x1D4E9) {
        return true;
    }
    if (0x1D504 <= c && c <= 0x1D505) {
        return true;
    }
    if (0x1D507 <= c && c <= 0x1D50A) {
        return true;
    }
    if (0x1D50D <= c && c <= 0x1D514) {
        return true;
    }
    if (0x1D516 <= c && c <= 0x1D51C) {
        return true;
    }
    if (0x1D538 <= c && c <= 0x1D539) {
        return true;
    }
    if (0x1D53B <= c && c <= 0x1D53E) {
        return true;
    }
    if (0x1D540 <= c && c <= 0x1D544) {
        return true;
    }
    if (0x1D54A <= c && c <= 0x1D550) {
        return true;
    }
    if (0x1D56C <= c && c <= 0x1D585) {
        return true;
    }
    if (0x1D5A0 <= c && c <= 0x1D5B9) {
        return true;
    }
    if (0x1D5D4 <= c && c <= 0x1D5ED) {
        return true;
    }
    if (0x1D608 <= c && c <= 0x1D621) {
        return true;
    }
    if (0x1D63C <= c && c <= 0x1D655) {
        return true;
    }
    if (0x1D670 <= c && c <= 0x1D689) {
        return true;
    }
    if (0x1D6A8 <= c && c <= 0x1D6C0) {
        return true;
    }
    if (0x1D6E2 <= c && c <= 0x1D6FA) {
        return true;
    }
    if (0x1D71C <= c && c <= 0x1D734) {
        return true;
    }
    if (0x1D756 <= c && c <= 0x1D76E) {
        return true;
    }
    if (0x1D790 <= c && c <= 0x1D7A8) {
        return true;
    }
    if (0x1E900 <= c && c <= 0x1E921) {
        return true;
    }
    if (0x1F130 <= c && c <= 0x1F149) {
        return true;
    }
    if (0x1F150 <= c && c <= 0x1F169) {
        return true;
    }
    if (0x1F170 <= c && c <= 0x1F189) {
        return true;
    }
    return false;
}

bool bc_uprop_is_lower(int32_t c) {
    switch (c) {
    case 0x00AA:
    case 0x00B5:
    case 0x00BA:
    case 0x0101:
    case 0x0103:
    case 0x0105:
    case 0x0107:
    case 0x0109:
    case 0x010B:
    case 0x010D:
    case 0x010F:
    case 0x0111:
    case 0x0113:
    case 0x0115:
    case 0x0117:
    case 0x0119:
    case 0x011B:
    case 0x011D:
    case 0x011F:
    case 0x0121:
    case 0x0123:
    case 0x0125:
    case 0x0127:
    case 0x0129:
    case 0x012B:
    case 0x012D:
    case 0x012F:
    case 0x0131:
    case 0x0133:
    case 0x0135:
    case 0x013A:
    case 0x013C:
    case 0x013E:
    case 0x0140:
    case 0x0142:
    case 0x0144:
    case 0x0146:
    case 0x014B:
    case 0x014D:
    case 0x014F:
    case 0x0151:
    case 0x0153:
    case 0x0155:
    case 0x0157:
    case 0x0159:
    case 0x015B:
    case 0x015D:
    case 0x015F:
    case 0x0161:
    case 0x0163:
    case 0x0165:
    case 0x0167:
    case 0x0169:
    case 0x016B:
    case 0x016D:
    case 0x016F:
    case 0x0171:
    case 0x0173:
    case 0x0175:
    case 0x0177:
    case 0x017A:
    case 0x017C:
    case 0x0183:
    case 0x0185:
    case 0x0188:
    case 0x0192:
    case 0x0195:
    case 0x019E:
    case 0x01A1:
    case 0x01A3:
    case 0x01A5:
    case 0x01A8:
    case 0x01AD:
    case 0x01B0:
    case 0x01B4:
    case 0x01B6:
    case 0x01C6:
    case 0x01C9:
    case 0x01CC:
    case 0x01CE:
    case 0x01D0:
    case 0x01D2:
    case 0x01D4:
    case 0x01D6:
    case 0x01D8:
    case 0x01DA:
    case 0x01DF:
    case 0x01E1:
    case 0x01E3:
    case 0x01E5:
    case 0x01E7:
    case 0x01E9:
    case 0x01EB:
    case 0x01ED:
    case 0x01F3:
    case 0x01F5:
    case 0x01F9:
    case 0x01FB:
    case 0x01FD:
    case 0x01FF:
    case 0x0201:
    case 0x0203:
    case 0x0205:
    case 0x0207:
    case 0x0209:
    case 0x020B:
    case 0x020D:
    case 0x020F:
    case 0x0211:
    case 0x0213:
    case 0x0215:
    case 0x0217:
    case 0x0219:
    case 0x021B:
    case 0x021D:
    case 0x021F:
    case 0x0221:
    case 0x0223:
    case 0x0225:
    case 0x0227:
    case 0x0229:
    case 0x022B:
    case 0x022D:
    case 0x022F:
    case 0x0231:
    case 0x023C:
    case 0x0242:
    case 0x0247:
    case 0x0249:
    case 0x024B:
    case 0x024D:
    case 0x0345:
    case 0x0371:
    case 0x0373:
    case 0x0377:
    case 0x037A:
    case 0x0390:
    case 0x03D9:
    case 0x03DB:
    case 0x03DD:
    case 0x03DF:
    case 0x03E1:
    case 0x03E3:
    case 0x03E5:
    case 0x03E7:
    case 0x03E9:
    case 0x03EB:
    case 0x03ED:
    case 0x03F5:
    case 0x03F8:
    case 0x0461:
    case 0x0463:
    case 0x0465:
    case 0x0467:
    case 0x0469:
    case 0x046B:
    case 0x046D:
    case 0x046F:
    case 0x0471:
    case 0x0473:
    case 0x0475:
    case 0x0477:
    case 0x0479:
    case 0x047B:
    case 0x047D:
    case 0x047F:
    case 0x0481:
    case 0x048B:
    case 0x048D:
    case 0x048F:
    case 0x0491:
    case 0x0493:
    case 0x0495:
    case 0x0497:
    case 0x0499:
    case 0x049B:
    case 0x049D:
    case 0x049F:
    case 0x04A1:
    case 0x04A3:
    case 0x04A5:
    case 0x04A7:
    case 0x04A9:
    case 0x04AB:
    case 0x04AD:
    case 0x04AF:
    case 0x04B1:
    case 0x04B3:
    case 0x04B5:
    case 0x04B7:
    case 0x04B9:
    case 0x04BB:
    case 0x04BD:
    case 0x04BF:
    case 0x04C2:
    case 0x04C4:
    case 0x04C6:
    case 0x04C8:
    case 0x04CA:
    case 0x04CC:
    case 0x04D1:
    case 0x04D3:
    case 0x04D5:
    case 0x04D7:
    case 0x04D9:
    case 0x04DB:
    case 0x04DD:
    case 0x04DF:
    case 0x04E1:
    case 0x04E3:
    case 0x04E5:
    case 0x04E7:
    case 0x04E9:
    case 0x04EB:
    case 0x04ED:
    case 0x04EF:
    case 0x04F1:
    case 0x04F3:
    case 0x04F5:
    case 0x04F7:
    case 0x04F9:
    case 0x04FB:
    case 0x04FD:
    case 0x04FF:
    case 0x0501:
    case 0x0503:
    case 0x0505:
    case 0x0507:
    case 0x0509:
    case 0x050B:
    case 0x050D:
    case 0x050F:
    case 0x0511:
    case 0x0513:
    case 0x0515:
    case 0x0517:
    case 0x0519:
    case 0x051B:
    case 0x051D:
    case 0x051F:
    case 0x0521:
    case 0x0523:
    case 0x0525:
    case 0x0527:
    case 0x0529:
    case 0x052B:
    case 0x052D:
    case 0x052F:
    case 0x10FC:
    case 0x1C8A:
    case 0x1D78:
    case 0x1E01:
    case 0x1E03:
    case 0x1E05:
    case 0x1E07:
    case 0x1E09:
    case 0x1E0B:
    case 0x1E0D:
    case 0x1E0F:
    case 0x1E11:
    case 0x1E13:
    case 0x1E15:
    case 0x1E17:
    case 0x1E19:
    case 0x1E1B:
    case 0x1E1D:
    case 0x1E1F:
    case 0x1E21:
    case 0x1E23:
    case 0x1E25:
    case 0x1E27:
    case 0x1E29:
    case 0x1E2B:
    case 0x1E2D:
    case 0x1E2F:
    case 0x1E31:
    case 0x1E33:
    case 0x1E35:
    case 0x1E37:
    case 0x1E39:
    case 0x1E3B:
    case 0x1E3D:
    case 0x1E3F:
    case 0x1E41:
    case 0x1E43:
    case 0x1E45:
    case 0x1E47:
    case 0x1E49:
    case 0x1E4B:
    case 0x1E4D:
    case 0x1E4F:
    case 0x1E51:
    case 0x1E53:
    case 0x1E55:
    case 0x1E57:
    case 0x1E59:
    case 0x1E5B:
    case 0x1E5D:
    case 0x1E5F:
    case 0x1E61:
    case 0x1E63:
    case 0x1E65:
    case 0x1E67:
    case 0x1E69:
    case 0x1E6B:
    case 0x1E6D:
    case 0x1E6F:
    case 0x1E71:
    case 0x1E73:
    case 0x1E75:
    case 0x1E77:
    case 0x1E79:
    case 0x1E7B:
    case 0x1E7D:
    case 0x1E7F:
    case 0x1E81:
    case 0x1E83:
    case 0x1E85:
    case 0x1E87:
    case 0x1E89:
    case 0x1E8B:
    case 0x1E8D:
    case 0x1E8F:
    case 0x1E91:
    case 0x1E93:
    case 0x1E9F:
    case 0x1EA1:
    case 0x1EA3:
    case 0x1EA5:
    case 0x1EA7:
    case 0x1EA9:
    case 0x1EAB:
    case 0x1EAD:
    case 0x1EAF:
    case 0x1EB1:
    case 0x1EB3:
    case 0x1EB5:
    case 0x1EB7:
    case 0x1EB9:
    case 0x1EBB:
    case 0x1EBD:
    case 0x1EBF:
    case 0x1EC1:
    case 0x1EC3:
    case 0x1EC5:
    case 0x1EC7:
    case 0x1EC9:
    case 0x1ECB:
    case 0x1ECD:
    case 0x1ECF:
    case 0x1ED1:
    case 0x1ED3:
    case 0x1ED5:
    case 0x1ED7:
    case 0x1ED9:
    case 0x1EDB:
    case 0x1EDD:
    case 0x1EDF:
    case 0x1EE1:
    case 0x1EE3:
    case 0x1EE5:
    case 0x1EE7:
    case 0x1EE9:
    case 0x1EEB:
    case 0x1EED:
    case 0x1EEF:
    case 0x1EF1:
    case 0x1EF3:
    case 0x1EF5:
    case 0x1EF7:
    case 0x1EF9:
    case 0x1EFB:
    case 0x1EFD:
    case 0x1FBE:
    case 0x2071:
    case 0x207F:
    case 0x210A:
    case 0x2113:
    case 0x212F:
    case 0x2134:
    case 0x2139:
    case 0x214E:
    case 0x2184:
    case 0x2C61:
    case 0x2C68:
    case 0x2C6A:
    case 0x2C6C:
    case 0x2C71:
    case 0x2C81:
    case 0x2C83:
    case 0x2C85:
    case 0x2C87:
    case 0x2C89:
    case 0x2C8B:
    case 0x2C8D:
    case 0x2C8F:
    case 0x2C91:
    case 0x2C93:
    case 0x2C95:
    case 0x2C97:
    case 0x2C99:
    case 0x2C9B:
    case 0x2C9D:
    case 0x2C9F:
    case 0x2CA1:
    case 0x2CA3:
    case 0x2CA5:
    case 0x2CA7:
    case 0x2CA9:
    case 0x2CAB:
    case 0x2CAD:
    case 0x2CAF:
    case 0x2CB1:
    case 0x2CB3:
    case 0x2CB5:
    case 0x2CB7:
    case 0x2CB9:
    case 0x2CBB:
    case 0x2CBD:
    case 0x2CBF:
    case 0x2CC1:
    case 0x2CC3:
    case 0x2CC5:
    case 0x2CC7:
    case 0x2CC9:
    case 0x2CCB:
    case 0x2CCD:
    case 0x2CCF:
    case 0x2CD1:
    case 0x2CD3:
    case 0x2CD5:
    case 0x2CD7:
    case 0x2CD9:
    case 0x2CDB:
    case 0x2CDD:
    case 0x2CDF:
    case 0x2CE1:
    case 0x2CEC:
    case 0x2CEE:
    case 0x2CF3:
    case 0x2D27:
    case 0x2D2D:
    case 0xA641:
    case 0xA643:
    case 0xA645:
    case 0xA647:
    case 0xA649:
    case 0xA64B:
    case 0xA64D:
    case 0xA64F:
    case 0xA651:
    case 0xA653:
    case 0xA655:
    case 0xA657:
    case 0xA659:
    case 0xA65B:
    case 0xA65D:
    case 0xA65F:
    case 0xA661:
    case 0xA663:
    case 0xA665:
    case 0xA667:
    case 0xA669:
    case 0xA66B:
    case 0xA66D:
    case 0xA681:
    case 0xA683:
    case 0xA685:
    case 0xA687:
    case 0xA689:
    case 0xA68B:
    case 0xA68D:
    case 0xA68F:
    case 0xA691:
    case 0xA693:
    case 0xA695:
    case 0xA697:
    case 0xA699:
    case 0xA69B:
    case 0xA723:
    case 0xA725:
    case 0xA727:
    case 0xA729:
    case 0xA72B:
    case 0xA72D:
    case 0xA733:
    case 0xA735:
    case 0xA737:
    case 0xA739:
    case 0xA73B:
    case 0xA73D:
    case 0xA73F:
    case 0xA741:
    case 0xA743:
    case 0xA745:
    case 0xA747:
    case 0xA749:
    case 0xA74B:
    case 0xA74D:
    case 0xA74F:
    case 0xA751:
    case 0xA753:
    case 0xA755:
    case 0xA757:
    case 0xA759:
    case 0xA75B:
    case 0xA75D:
    case 0xA75F:
    case 0xA761:
    case 0xA763:
    case 0xA765:
    case 0xA767:
    case 0xA769:
    case 0xA76B:
    case 0xA76D:
    case 0xA76F:
    case 0xA770:
    case 0xA77A:
    case 0xA77C:
    case 0xA77F:
    case 0xA781:
    case 0xA783:
    case 0xA785:
    case 0xA787:
    case 0xA78C:
    case 0xA78E:
    case 0xA791:
    case 0xA797:
    case 0xA799:
    case 0xA79B:
    case 0xA79D:
    case 0xA79F:
    case 0xA7A1:
    case 0xA7A3:
    case 0xA7A5:
    case 0xA7A7:
    case 0xA7A9:
    case 0xA7AF:
    case 0xA7B5:
    case 0xA7B7:
    case 0xA7B9:
    case 0xA7BB:
    case 0xA7BD:
    case 0xA7BF:
    case 0xA7C1:
    case 0xA7C3:
    case 0xA7C8:
    case 0xA7CA:
    case 0xA7CD:
    case 0xA7D1:
    case 0xA7D3:
    case 0xA7D5:
    case 0xA7D7:
    case 0xA7D9:
    case 0xA7DB:
    case 0xA7F6:
    case 0xA7FA:
    case 0xAB69:
    case 0x10780:
    case 0x1D4BB:
    case 0x1D7CB:
        return true;
    }
    if (0x0061 <= c && c <= 0x007A) {
        return true;
    }
    if (0x00DF <= c && c <= 0x00F6) {
        return true;
    }
    if (0x00F8 <= c && c <= 0x00FF) {
        return true;
    }
    if (0x0137 <= c && c <= 0x0138) {
        return true;
    }
    if (0x0148 <= c && c <= 0x0149) {
        return true;
    }
    if (0x017E <= c && c <= 0x0180) {
        return true;
    }
    if (0x018C <= c && c <= 0x018D) {
        return true;
    }
    if (0x0199 <= c && c <= 0x019B) {
        return true;
    }
    if (0x01AA <= c && c <= 0x01AB) {
        return true;
    }
    if (0x01B9 <= c && c <= 0x01BA) {
        return true;
    }
    if (0x01BD <= c && c <= 0x01BF) {
        return true;
    }
    if (0x01DC <= c && c <= 0x01DD) {
        return true;
    }
    if (0x01EF <= c && c <= 0x01F0) {
        return true;
    }
    if (0x0233 <= c && c <= 0x0239) {
        return true;
    }
    if (0x023F <= c && c <= 0x0240) {
        return true;
    }
    if (0x024F <= c && c <= 0x0293) {
        return true;
    }
    if (0x0295 <= c && c <= 0x02AF) {
        return true;
    }
    if (0x02B0 <= c && c <= 0x02B8) {
        return true;
    }
    if (0x02C0 <= c && c <= 0x02C1) {
        return true;
    }
    if (0x02E0 <= c && c <= 0x02E4) {
        return true;
    }
    if (0x037B <= c && c <= 0x037D) {
        return true;
    }
    if (0x03AC <= c && c <= 0x03CE) {
        return true;
    }
    if (0x03D0 <= c && c <= 0x03D1) {
        return true;
    }
    if (0x03D5 <= c && c <= 0x03D7) {
        return true;
    }
    if (0x03EF <= c && c <= 0x03F3) {
        return true;
    }
    if (0x03FB <= c && c <= 0x03FC) {
        return true;
    }
    if (0x0430 <= c && c <= 0x045F) {
        return true;
    }
    if (0x04CE <= c && c <= 0x04CF) {
        return true;
    }
    if (0x0560 <= c && c <= 0x0588) {
        return true;
    }
    if (0x10D0 <= c && c <= 0x10FA) {
        return true;
    }
    if (0x10FD <= c && c <= 0x10FF) {
        return true;
    }
    if (0x13F8 <= c && c <= 0x13FD) {
        return true;
    }
    if (0x1C80 <= c && c <= 0x1C88) {
        return true;
    }
    if (0x1D00 <= c && c <= 0x1D2B) {
        return true;
    }
    if (0x1D2C <= c && c <= 0x1D6A) {
        return true;
    }
    if (0x1D6B <= c && c <= 0x1D77) {
        return true;
    }
    if (0x1D79 <= c && c <= 0x1D9A) {
        return true;
    }
    if (0x1D9B <= c && c <= 0x1DBF) {
        return true;
    }
    if (0x1E95 <= c && c <= 0x1E9D) {
        return true;
    }
    if (0x1EFF <= c && c <= 0x1F07) {
        return true;
    }
    if (0x1F10 <= c && c <= 0x1F15) {
        return true;
    }
    if (0x1F20 <= c && c <= 0x1F27) {
        return true;
    }
    if (0x1F30 <= c && c <= 0x1F37) {
        return true;
    }
    if (0x1F40 <= c && c <= 0x1F45) {
        return true;
    }
    if (0x1F50 <= c && c <= 0x1F57) {
        return true;
    }
    if (0x1F60 <= c && c <= 0x1F67) {
        return true;
    }
    if (0x1F70 <= c && c <= 0x1F7D) {
        return true;
    }
    if (0x1F80 <= c && c <= 0x1F87) {
        return true;
    }
    if (0x1F90 <= c && c <= 0x1F97) {
        return true;
    }
    if (0x1FA0 <= c && c <= 0x1FA7) {
        return true;
    }
    if (0x1FB0 <= c && c <= 0x1FB4) {
        return true;
    }
    if (0x1FB6 <= c && c <= 0x1FB7) {
        return true;
    }
    if (0x1FC2 <= c && c <= 0x1FC4) {
        return true;
    }
    if (0x1FC6 <= c && c <= 0x1FC7) {
        return true;
    }
    if (0x1FD0 <= c && c <= 0x1FD3) {
        return true;
    }
    if (0x1FD6 <= c && c <= 0x1FD7) {
        return true;
    }
    if (0x1FE0 <= c && c <= 0x1FE7) {
        return true;
    }
    if (0x1FF2 <= c && c <= 0x1FF4) {
        return true;
    }
    if (0x1FF6 <= c && c <= 0x1FF7) {
        return true;
    }
    if (0x2090 <= c && c <= 0x209C) {
        return true;
    }
    if (0x210E <= c && c <= 0x210F) {
        return true;
    }
    if (0x213C <= c && c <= 0x213D) {
        return true;
    }
    if (0x2146 <= c && c <= 0x2149) {
        return true;
    }
    if (0x2170 <= c && c <= 0x217F) {
        return true;
    }
    if (0x24D0 <= c && c <= 0x24E9) {
        return true;
    }
    if (0x2C30 <= c && c <= 0x2C5F) {
        return true;
    }
    if (0x2C65 <= c && c <= 0x2C66) {
        return true;
    }
    if (0x2C73 <= c && c <= 0x2C74) {
        return true;
    }
    if (0x2C76 <= c && c <= 0x2C7B) {
        return true;
    }
    if (0x2C7C <= c && c <= 0x2C7D) {
        return true;
    }
    if (0x2CE3 <= c && c <= 0x2CE4) {
        return true;
    }
    if (0x2D00 <= c && c <= 0x2D25) {
        return true;
    }
    if (0xA69C <= c && c <= 0xA69D) {
        return true;
    }
    if (0xA72F <= c && c <= 0xA731) {
        return true;
    }
    if (0xA771 <= c && c <= 0xA778) {
        return true;
    }
    if (0xA793 <= c && c <= 0xA795) {
        return true;
    }
    if (0xA7F2 <= c && c <= 0xA7F4) {
        return true;
    }
    if (0xA7F8 <= c && c <= 0xA7F9) {
        return true;
    }
    if (0xAB30 <= c && c <= 0xAB5A) {
        return true;
    }
    if (0xAB5C <= c && c <= 0xAB5F) {
        return true;
    }
    if (0xAB60 <= c && c <= 0xAB68) {
        return true;
    }
    if (0xAB70 <= c && c <= 0xABBF) {
        return true;
    }
    if (0xFB00 <= c && c <= 0xFB06) {
        return true;
    }
    if (0xFB13 <= c && c <= 0xFB17) {
        return true;
    }
    if (0xFF41 <= c && c <= 0xFF5A) {
        return true;
    }
    if (0x10428 <= c && c <= 0x1044F) {
        return true;
    }
    if (0x104D8 <= c && c <= 0x104FB) {
        return true;
    }
    if (0x10597 <= c && c <= 0x105A1) {
        return true;
    }
    if (0x105A3 <= c && c <= 0x105B1) {
        return true;
    }
    if (0x105B3 <= c && c <= 0x105B9) {
        return true;
    }
    if (0x105BB <= c && c <= 0x105BC) {
        return true;
    }
    if (0x10783 <= c && c <= 0x10785) {
        return true;
    }
    if (0x10787 <= c && c <= 0x107B0) {
        return true;
    }
    if (0x107B2 <= c && c <= 0x107BA) {
        return true;
    }
    if (0x10CC0 <= c && c <= 0x10CF2) {
        return true;
    }
    if (0x10D70 <= c && c <= 0x10D85) {
        return true;
    }
    if (0x118C0 <= c && c <= 0x118DF) {
        return true;
    }
    if (0x16E60 <= c && c <= 0x16E7F) {
        return true;
    }
    if (0x1D41A <= c && c <= 0x1D433) {
        return true;
    }
    if (0x1D44E <= c && c <= 0x1D454) {
        return true;
    }
    if (0x1D456 <= c && c <= 0x1D467) {
        return true;
    }
    if (0x1D482 <= c && c <= 0x1D49B) {
        return true;
    }
    if (0x1D4B6 <= c && c <= 0x1D4B9) {
        return true;
    }
    if (0x1D4BD <= c && c <= 0x1D4C3) {
        return true;
    }
    if (0x1D4C5 <= c && c <= 0x1D4CF) {
        return true;
    }
    if (0x1D4EA <= c && c <= 0x1D503) {
        return true;
    }
    if (0x1D51E <= c && c <= 0x1D537) {
        return true;
    }
    if (0x1D552 <= c && c <= 0x1D56B) {
        return true;
    }
    if (0x1D586 <= c && c <= 0x1D59F) {
        return true;
    }
    if (0x1D5BA <= c && c <= 0x1D5D3) {
        return true;
    }
    if (0x1D5EE <= c && c <= 0x1D607) {
        return true;
    }
    if (0x1D622 <= c && c <= 0x1D63B) {
        return true;
    }
    if (0x1D656 <= c && c <= 0x1D66F) {
        return true;
    }
    if (0x1D68A <= c && c <= 0x1D6A5) {
        return true;
    }
    if (0x1D6C2 <= c && c <= 0x1D6DA) {
        return true;
    }
    if (0x1D6DC <= c && c <= 0x1D6E1) {
        return true;
    }
    if (0x1D6FC <= c && c <= 0x1D714) {
        return true;
    }
    if (0x1D716 <= c && c <= 0x1D71B) {
        return true;
    }
    if (0x1D736 <= c && c <= 0x1D74E) {
        return true;
    }
    if (0x1D750 <= c && c <= 0x1D755) {
        return true;
    }
    if (0x1D770 <= c && c <= 0x1D788) {
        return true;
    }
    if (0x1D78A <= c && c <= 0x1D78F) {
        return true;
    }
    if (0x1D7AA <= c && c <= 0x1D7C2) {
        return true;
    }
    if (0x1D7C4 <= c && c <= 0x1D7C9) {
        return true;
    }
    if (0x1DF00 <= c && c <= 0x1DF09) {
        return true;
    }
    if (0x1DF0B <= c && c <= 0x1DF1E) {
        return true;
    }
    if (0x1DF25 <= c && c <= 0x1DF2A) {
        return true;
    }
    if (0x1E030 <= c && c <= 0x1E06D) {
        return true;
    }
    if (0x1E922 <= c && c <= 0x1E943) {
        return true;
    }
    return false;
}

bool bc_uprop_is_digit(int32_t c) { return 0x30 <= c && c <= 0x39; }
