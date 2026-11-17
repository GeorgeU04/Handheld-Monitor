#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/* Packed 1px bitmaps. Bit 7 of each byte is the leftmost pixel.
 * Order in each array: 0-9, A-Z, a-z.
 * small:       5x7,  1 byte per row
 * medium:      8x12, 1 byte per row
 * large:       12x18, 2 bytes per row
 * extra large: 18x27, 3 bytes per row
 */

extern const uint8_t smallGlyphs[];
extern const uint8_t mediumGlyphs[];
extern const uint8_t largeGlyphs[];
extern const uint8_t extraLargeGlyphs[];

struct font {
  const uint8_t *glyph;
  uint8_t height;
  uint8_t width;
  uint8_t bytesPerRow;
};

extern const struct font smallFont;
extern const struct font mediumFont;
extern const struct font largeFont;
extern const struct font extraLageFont;

int8_t getCharIndex(uint8_t c);

#endif
