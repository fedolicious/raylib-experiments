#pragma once

struct rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    bool operator==(const rgb& other) {
        return
            r == other.r &&
            g == other.g &&
            b == other.b;
    }
    bool operator!=(const rgb& other) {
        return !(*this == other);
    }
};
typedef struct {
    rgb* pixels;
    unsigned width;
    unsigned height;
} image;

#define rgbAt(_img, _x, _y) _img.pixels[_img.width*_y+_x]

image imgio_readPPM(const char* pathName);
