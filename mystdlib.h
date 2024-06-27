#pragma once

#define member_size(type, member) (sizeof( ((type *)0)->member ))

void perrorExit(const char* msg, int errorValue);