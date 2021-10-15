#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "../Types/types.h"

void putpixel(uint64_t x, uint64_t y, uint32_t color);
uint32_t getpixel(int x, int y);
void drawLine(int x, int y, int x1, int y1, uint32_t color);
void drawVLine(int x, int y, int h, int color);
void drawHLine(int x, int y, int w, int color);
void drawRect(int x, int y, int h, int w, int color);
void fillRect(int x, int y, int h, int w, uint32_t color);
void drawTriangle(int x, int y, int x1, int y1, int x2, int y2, int color);
void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t c);
void drawCircle(int x, int y, int r, uint32_t c);
void fillCircle(int x, int y, int r, uint32_t c);

void drawChar(unsigned char c, int x, int y, int fgcolor, int bgcolor);
void drawCharToCursor(unsigned char c, int fgcolor, int bgcolor);
void drawString(const char *str, int x, int y, int fgcolor, int bgcolor);
void drawStringToCursor(const char *str, int fgcolor, int bgcolor);
void backspace();

#endif