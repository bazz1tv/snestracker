#pragma once

// SDL 1.2 RULE
#ifdef __APPLE__
#define CMD_CTRL_KEY KMOD_GUI
#else
#define CMD_CTRL_KEY KMOD_CTRL
#endif

#ifndef PATH_MAX
  #define PATH_MAX 2048
#endif