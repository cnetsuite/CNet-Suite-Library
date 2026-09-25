#pragma once

#ifndef CNET_API_H
#define CNET_API_H

#include <stddef.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

/** CNET_API marks every public CNet symbol; expands to dllexport when building the library (CNET_EXPORTS defined) and dllimport for consumers on Windows, and to a default-visibility attribute (paired with -fvisibility=hidden as the build default) on Linux/macOS. */

#ifdef _WIN32
#ifdef CNET_EXPORTS
#define CNET_API __declspec(dllexport)
#else
#define CNET_API __declspec(dllimport)
#endif
#else
#define CNET_API __attribute__((visibility("default")))
#endif

#endif
