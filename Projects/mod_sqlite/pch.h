// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
#ifdef _MSC_VER
#include "targetver.h"
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <GalacticInterfaces.h>
#include <GalacticStrongPtr.h>

#include <SDKHelper.h>
