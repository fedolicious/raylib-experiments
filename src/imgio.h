#pragma once

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb;
typedef struct {
    rgb* pixels;
    unsigned width;
    unsigned height;
} image;

#define rgbAt(_img, _x, _y) _img.pixels[_img.width*_y+_x]

image imgio_readPPM(const char* pathName);
