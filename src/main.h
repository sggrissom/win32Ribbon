#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define global static
#define internal static
#define persist static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#ifdef debug
#define assert(expression) if(!(expression)) {*(int *)0 = 0;}
#else
#define assert(expression)
#endif

#define arrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define addMenuOption(menuHandle, name) HMENU hSubMenu = CreatePopupMenu(); AppendMenu(menuHandle, MF_STRING | MF_POPUP, (UINT)hSubMenu, name);
#define addToMenuOption(menuHandle, id, name) AppendMenu(hSubMenu, MF_STRING, id, name);

#define fileExtention(text, extension) ""text" ("extension")\0"extension"\0"

char *menu[] = {"open",
                "exit"};

#endif
