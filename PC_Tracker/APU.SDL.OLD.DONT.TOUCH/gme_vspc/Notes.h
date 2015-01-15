#pragma once
#include <stdint.h>
// The note number of the engine is higher by 36 (i.e. 3 octaves) than MIDI note number.
// C-4 = [0x49]
namespace Notes
{
extern const uint8_t base;
extern const int midi_offset;
extern const uint16_t pitch_table[];
extern const uint16_t pitch_table_size;
}

