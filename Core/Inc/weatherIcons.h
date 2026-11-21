#ifndef WEATHERICONS_H
#define WEATHERICONS_H

#include <stdint.h>

#define CLOUD_WIDTH 140U
#define CLOUD_HEIGHT 140U

/* RGB565 pixels, row by row. High byte is sent first. */
extern const uint16_t cloudBitmap[CLOUD_WIDTH * CLOUD_HEIGHT];
extern const uint16_t cloudyBitmap[CLOUD_WIDTH * CLOUD_HEIGHT];
extern const uint16_t sunnyBitmap[CLOUD_WIDTH * CLOUD_HEIGHT];

#endif
