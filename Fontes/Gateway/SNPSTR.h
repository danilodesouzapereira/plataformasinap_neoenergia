#ifndef SNPSTR_H
#define SNPSTR_H

#include <wchar.h>

#ifdef _UNICODE
   #define SNPSTR UnicodeString
#else
   #define SNPSTR AnsiString
#endif

#endif

