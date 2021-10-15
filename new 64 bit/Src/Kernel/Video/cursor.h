#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "font.h"
#include "video.h"

typedef struct
{
    uint64_t x;
    uint64_t y;
} __attribute__((packed)) Cursor_t;

extern Cursor_t defaultCursor;

void initCursor();

void setCursor(int x, int y);

Cursor_t getCursor();

int getCursorX();

int getCursorY();

void incrementCursor();

void nextLine();

#endif