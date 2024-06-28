#include "imgio.h"

#include "mystdlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

const char PPM_SIGNATURE[2] = "P6";
#define PPM_VALUE_MAX 255

//returns the first character that would be read from the stream
int skipWhitespaceAndComments(FILE* stream) {
    int nextChar;
    while(true) {
        nextChar = getc(stream);
        if(nextChar == '#') {
            do {
                nextChar = getc(stream);
            } while(nextChar != '\n');
        } else if(!isspace(nextChar)) {
            break;
        }
    }
    ungetc(nextChar, stream);
    return nextChar;
}

image imgio_readPPM(const char* pathName) {
    image img;
    FILE* imgStream = fopen(pathName,"rb");
    fseek(imgStream, 0, SEEK_END);
    long fileSize = ftell(imgStream);
    fseek(imgStream, 0, SEEK_SET);
    
    char signature[sizeof(PPM_SIGNATURE)];
    int amountRead = fread(signature, sizeof(signature), 1, imgStream);
    if(amountRead < 1 || memcmp(signature, PPM_SIGNATURE, sizeof(signature)) != 0) {
        perrorExit("malformed .ppm file - invalid signature", -1);
    }
    skipWhitespaceAndComments(imgStream);
    
    if(fscanf(imgStream, "%d", &img.width) != 1 || img.width <= 0) {
        perrorExit("malformed .ppm file - invalid width", -1);
    }
    skipWhitespaceAndComments(imgStream);
    
    if(fscanf(imgStream, "%d", &img.height) != 1 || img.height <= 0) {
        perrorExit("malformed .ppm file - invalid height", -1);
    }
    skipWhitespaceAndComments(imgStream);
    
    int maxValue;
    if(fscanf(imgStream, "%d", &maxValue) != 1 || maxValue != PPM_VALUE_MAX) {
        perrorExit("malformed .ppm file - invalid max colour value", -1);
    }
    
    if(getc(imgStream) != '\n') {
        perrorExit("malformed .ppm file - no newline between header and data", -1);   
    }
    
    long filePos = ftell(imgStream);
    long imgDataSize = (long)img.width*(long)img.height*3L;
    if(imgDataSize != fileSize-filePos) {
        perrorExit("malformed .ppm file - invalid data size", -1);   
    }
    
    img.pixels = malloc(imgDataSize);
    amountRead = fread(img.pixels, imgDataSize, 1, imgStream);
    //sanity check
    if(amountRead < 1 || ftell(imgStream) != fileSize || getc(imgStream) != EOF) {
        perrorExit("error occured while reading data from .ppm file", -1);
    }
    assert(fclose(imgStream) == 0);
    printf("\\n: %d, \\r: %d", '\n', '\r');
    return img;
}
