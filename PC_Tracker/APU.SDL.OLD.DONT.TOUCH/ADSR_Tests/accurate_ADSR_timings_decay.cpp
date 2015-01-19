#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <math.h>

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

void print_result(double result)
{
  if (result > 1000)
  {
    result /= 1000;
    std::cout << result << " s\""; // << std::endl;
  }
  else
  {
    printf("%d ms\"", (int)floor(result+0.5) ); //  << " ms\"";// << std::endl;
  }

    printf("},\n");
}

int main()
{
  std::cout.precision(1);
  std::cout.setf(std::ios::fixed);
  double freq = 0.03125;
  uint16_t decay=0;
  
  uint16_t level = 0x7ff;
  //;Rate=N*2+16, Step=-(((Level-1) SAR 8)+1)
  
  double decay_freq_iter=0, attack_freq_iter=0, sustain_rate_freq_iter=0;
  for (int16_t attack_param=0; attack_param < 16; attack_param++)
  {
    attack_freq_iter=0;
    level=0;
    uint16_t attack_rate = (attack_param*2)+1; // index into freq_table

    while (level < 0x7e0)
    {
      for (int wait=freq_table[attack_rate]; wait > 0; wait--)
      {
        attack_freq_iter++;
      }
      uint16_t step;
      if (attack_rate == 31) step = 1024;
      else step = 32;

      level += step;
      attack_freq_iter++;
    }

    if ( (uint16_t)level > 0x7ff )
    {
      level = 0x7ff;
      //fprintf(stderr, "CAPPED");
    }
    //printf("[attack: %d] ", attack_param );
    double result = (double)(attack_freq_iter*freq);
    //print_result(result);
    uint16_t save_attack_level = level;
    if (attack_param == 0)
    {
      
        for (uint16_t sustain_level_param=0; sustain_level_param < 8; sustain_level_param++)
        {
          printf("{\n");
          for (uint16_t decay_param=0; decay_param < 8; decay_param++)
          {
            uint16_t decay_rate = (decay_param*2) + 16;
          decay_freq_iter = 0;
          level = save_attack_level;
          uint16_t sustain_level = (sustain_level_param+1)*0x100;
          while (level > sustain_level)
          {
            for (int wait=freq_table[decay_rate]; wait > 0; wait--)
            {
              decay_freq_iter++;
            }
            uint16_t step = (((level-1) >> 8)+1);
            level -= step;
            decay_freq_iter++;
          }
          
            printf("\t{%d, \"", decay_param, sustain_level_param);
            print_result((double)(decay_freq_iter*freq));
          
          uint16_t save_sustain_level=level;

          
          // sustain
          if (attack_param == 0 && decay_param == 0)
          {
            for (uint16_t sustain_rate_param=1; sustain_rate_param < 0x20; sustain_rate_param++)
            {
              sustain_rate_freq_iter=0;
              int level = save_sustain_level;
              //uint16_t sustain_rate_real_rate = 
              while (level > 0)
              {
                for (int wait=freq_table[sustain_rate_param]; wait > 0; wait--)
                {
                  sustain_rate_freq_iter++;
                }
                uint16_t step = (((level-1) >> 8)+1);
                level -= step;
                sustain_rate_freq_iter++;
              }
              
              //printf("\t\t[sustain_release: %d] ", sustain_rate_param );
              //print_result((double)(sustain_rate_freq_iter*freq));
            }
          
          }


        }
        printf("},\n");
      }
    }
  }  
}