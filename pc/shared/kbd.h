#pragma once
#include "SDL.h"

/* Here are some macros to check if only one specific modifier key is
 * held, and to check if any modifier is held, respectively:
 *
 * Examples:
 * check for CTRL + SHIFT:
 *   if (MODONLY(mod, KMOD_CTRL | KMOD_SHIFT))
 *
 *
 *
 *   */
bool MODONLY(const Uint16 &mod, const Uint16 k);
#define MOD_ANY(mod) (mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI))
