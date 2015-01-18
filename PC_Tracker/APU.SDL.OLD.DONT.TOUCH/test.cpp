#include <iostream>
#include <cstdio>
#include <stdint.h>

int16_t freq_table[] = 
{
  0, // STOP                 
  2048,                 
  1536,                 
  1280,                 
  1024, 
  768,  
  640,  
  512,  
  384,
  320,
  256,
  192,
  160,
  128,
  96, 
  80, 
  64,
  48,
  40,
  32,
  24, 
  20, 
  16, 
  12, 
  10, 
  8,  
  6,  
  5,  
  4,
  3,
  2,
  1
};

int main()
{
  double freq = 0.03125;
  int16_t decay=0;
  
  int16_t level = 0x7ff;
  //;Rate=N*2+16, Step=-(((Level-1) SAR 8)+1)
  
  double freq_iter=0;

  for (int16_t decay_param=0; decay_param < 8; decay_param++)
  {
    
    
    int16_t decay_rate = (decay_param*2) + 16;
    for (int sustain_level_param=0; sustain_level_param < 8; sustain_level_param++)
    {
      freq_iter = 0;
      level = 0x7ff;
      int16_t sustain_level = (sustain_level_param+1)*0x100;
      while (level > sustain_level)
      {
        for (int wait=freq_table[decay_rate]; wait > 0; wait--)
        {
          freq_iter++;
        }
        int16_t step = -(((level-1) >> 8)+1);
        level += step;
        freq_iter++;
      }
      printf("[decay: %d][sustain_level: %d] %f ms\n", decay_param, sustain_level_param, (double)(freq_iter*freq));
    }
  }
  
}