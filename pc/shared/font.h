
#ifndef _font_h__
#define _font_h__

#define FONT_CODE_V_TRI 0x5C
#define FONT_CODE_V_TRI_STR "\x5c"
#define FONT_CODE_H_TRI 0x5D
#define FONT_CODE_H_TRI_STR "\x5d"

#define FONT_CENTERDOT_CHAR '\x5f'
#define FONT_THINDASH_CHAR '\x7d'
#define FONT_ELLIPSIS_CHAR '\x7e'
#define FONT_LONGDASH_CHAR '\x7f'

unsigned char *font_getChar(char c);
unsigned char *font_getChar2c(char c);

#endif // _font_h__

