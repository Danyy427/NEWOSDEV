#ifndef _FONT_H_
#define _FONT_H_

struct FontAttribute
{
    unsigned int cwidth;
    unsigned int cheight;
};

extern struct FontAttribute defaultAttr;
extern char Default_T_font[];

void initDefaultFont();

#endif