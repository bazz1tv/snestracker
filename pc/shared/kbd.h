#pragma once
#include "SDL.h"

// modifier keys that toggle on and off (NUMLOCK, CAPS LOCK)
#define TOGGLED_MODIFIERS (KMOD_NUM | KMOD_CAPS)
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
bool MOD_ANY(const Uint16 &mod);