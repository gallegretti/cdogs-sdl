/*
    C-Dogs SDL
    A port of the legendary (and fun) action/arcade cdogs.

    Copyright (c) 2013, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __COLOR
#define __COLOR

#include "sys_specifics.h"

#include <stdint.h>

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;
extern color_t colorRed;
extern color_t colorGreen;
extern color_t colorPoison;
extern color_t colorBlack;
extern color_t colorDarker;
extern color_t colorPurple;

typedef struct
{
	double h, s, v;
} HSV;
extern HSV tintRed;
extern HSV tintGreen;
extern HSV tintPoison;
extern HSV tintGray;
extern HSV tintPurple;
extern HSV tintDarker;
// Multiply by HSV components; used for tinting
// h: hue, if >= 0 then the hue is forced to be this value
// s: saturation, where all RGB components are shifted towards the average value
// v: scale factor on the final components
color_t ColorTint(color_t c, HSV hsv);

#endif
