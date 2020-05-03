#pragma once
#include "SDL.h"

/* Here are some macros to check if only one specific modifier key is
 * held, and to check if any modifier is held, respectively:
 *
 * Examples:
 * check if only the CTRL modifier (left or right) is held:
 *   if (MODONLY(mod, KMOD_CTRL))
 *
 *
 *
 *   */
#define MODONLY(mod, k) ( (mod) & (k) && !( (mod) & ~(k) ) )
#define MOD_ANY(mod) (mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI))
