#pragma once

#define member(type, member) (((type *)0)->member)
#define arrlen(arr) sizeof(arr)/sizeof(arr[0])

void perrorExit(const char* msg, int errorValue);