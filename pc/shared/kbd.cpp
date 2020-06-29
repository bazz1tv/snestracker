#include "kbd.h"

bool MODONLY(const Uint16 &mod, const Uint16 k)
{
  enum {
    CTRL=0,
    SHIFT,
    ALT,
    GUI,
    MOD_END
  };
  // iteration of each left/right modifier key group
  Uint16 modLR[4] = {
    KMOD_CTRL,
    KMOD_SHIFT,
    KMOD_ALT,
    KMOD_GUI
  };

  bool match = true;

  // for each modifier key group L/R pair
  for (int i=0; i < MOD_END; i++)
  {
    if (k & modLR[i]) // if we're supposed to be checking for ANY of these pair
    {
      // if the keys pressed do not reflect the keys checked for
      // (applies to any specified L / R / LR mod key combo)
      if ( ! ( (mod & modLR[i]) & (k & modLR[i]) ) )
      {
        match = false;
        break;
      }
    }
    else // if we're not supposed to be checking for this pair
    {
      if (mod & modLR[i]) // make sure this pair's keys are not held
      {
        match = false;
        break;
      }
    }
  }

  return match;
}
