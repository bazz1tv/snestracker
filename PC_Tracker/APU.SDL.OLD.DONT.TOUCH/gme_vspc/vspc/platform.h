#pragma once

// SDL 1.2 RULE
#ifdef __APPLE__
#define CMD_CTRL_KEY KMOD_GUI
#else
#define CMD_CTRL_KEY KMOD_CTRL
#endif