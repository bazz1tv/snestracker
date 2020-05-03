#pragma once
#include "SDL.h"

struct Voice_Control
{
  static Uint8 muted_toggle_protect; // for toggle protection during MouseMoition event
  static Uint8 muted;

	// clickable function
	static int mute_solo_voice(void *data);

  static Uint8 is_muted(int i);
  static void toggle_mute(uint8_t m);  // channel num
  static void toggle_mute_all();
  static void mute_all();
  static void unmute_all();
  static void mute(uint8_t i);
  static void solo(uint8_t i);
  static void solo_bits(uint8_t i);
  static void toggle_solo(uint8_t i);
};
