#pragma once

#define member(type, member) (((type *)0)->member)

void perrorExit(const char* msg, int errorValue);