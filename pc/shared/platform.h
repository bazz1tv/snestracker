#pragma once

/* Platform definitions inspired by Love */
#if defined(__APPLE__)
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#   define PLATFORM_IOS 1
# elif TARGET_OS_MAC
#   define PLATFORM_MACOSX 1
# endif
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
# define PLATFORM_LINUX 1
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
// I know it's not linux, but it seems most "linux-only" code is bsd-compatible
# define PLATFORM_LINUX 1
#endif

#if defined(WIN32) || defined(_WIN32)
# define PLATFORM_WINDOWS 1
#endif

// Check we have a sane configuration
#if !defined(PLATFORM_WINDOWS) && !defined(PLATFORM_LINUX) && \
    !defined(PLATFORM_MACOSX)
# error Could not detect target platform
#endif


// SDL 1.2 RULE
#ifdef __APPLE__
#define CMD_CTRL_KEY KMOD_GUI
#else
#define CMD_CTRL_KEY KMOD_CTRL
#endif

#ifndef PATH_MAX
  #define PATH_MAX 2048
#endif

int openUrl(const char *url);
