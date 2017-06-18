#pragma once

#ifndef WINPP_COMMON_HEADERS_H
#define WINPP_COMMON_HEADERS_H

#define NOMINMAX

#include <windows.h>
#include <windowsx.h>

#include <Uxtheme.h>
#include <Vsstyle.h>
#include <Vssym32.h>

#include <wingdi.h>
#include <commctrl.h>

#include <mmreg.h>
#include <wmsdk.h>

#pragma comment(lib, "UxTheme.lib")

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comctl32.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wmvcore.lib")

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min

#endif /* !WINPP_COMMON_HEADERS_H */
