#include <stdio.h>
#include <iostream>
#include <math.h>

double vals[] = 
{ 
  1.2,
  740,
 440,
 290,
 180,
 110,
 74,
 37  };

double func(double x, double y)
{
  //let x = 1.2;
  //let y = 1 - 8;
  //x - (x*(y/8))
  return x - (x*(y/8));
}

int main(int argc, char **argv)
{

  std::cout.precision(1);
  std::cout.setf(std::ios::fixed);
  for (int i=0; i < 9; i++)
  {
    for (int j=0; j < 8; j++)
    {
      double d = func(vals[j], i);
      //d += 0.05;
      //d = floor(d);
      fprintf(stderr, "[%d] %f -> ", i, vals[j]);

      std::cout << d << std::endl;
    }
  }
  return 0;
}

/* {0x00,"1.2  s"},
  {0x01, "740  ms"},
  {0x02, "440  ms"},
  {0x03, "290  ms"},
  {0x04, "180  ms"},
  {0x05, "110  ms"},
  {0x06, "74   ms"},
  {0x07, "37   ms"},

  {0x08, "1.2  s"},
  {0x09, "740  ms"},
  {0x0A, "440  ms"},
  {0x0B, "290  ms"},
  {0x0C, "180  ms"},
  {0x0D, "110  ms"},
  {0x0E, "74   ms"},
  {0x0F, "37   ms"},

  {0x10, "1.2  s"},
  {0x11, "740  ms"},
  {0x12, "440  ms"},
  {0x13, "290  ms"},
  {0x14, "180  ms"},
  {0x15, "110  ms"},
  {0x16, "74   ms"},
  {0x17, "37   ms"},

  {0x18, "1.2  s"},
  {0x19, "740  ms"},
  {0x1A, "440  ms"},
  {0x1B, "290  ms"},
  {0x1C, "180  ms"},
  {0x1D, "110  ms"},
  {0x1E, "74   ms"},
  {0x1F, "37   ms"},

  {0x20, "1.2  s"},
  {0x21, "740  ms"},
  {0x22, "440  ms"},
  {0x23, "290  ms"},
  {0x24, "180  ms"},
  {0x25, "110  ms"},
  {0x26, "74   ms"},
  {0x27, "37   ms"},

  {0x28, "1.2  s"},
  {0x29, "740  ms"},
  {0x2A, "440  ms"},
  {0x2B, "290  ms"},
  {0x2C, "180  ms"},
  {0x2D, "110  ms"},
  {0x2E, "74   ms"},
  {0x2F, "37   ms"},

  {0x30, "1.2  s"},
  {0x31, "740  ms"},
  {0x32, "440  ms"},
  {0x33, "290  ms"},
  {0x34, "180  ms"},
  {0x35, "110  ms"},
  {0x36, "74   ms"},
  {0x37, "37   ms"},

  {0x38, "1.2  s"},
  {0x39, "740  ms"},
  {0x3A, "440  ms"},
  {0x3B, "290  ms"},
  {0x3C, "180  ms"},
  {0x3D, "110  ms"},
  {0x3E, "74   ms"},
  {0x3F, "37   ms"},

  {0x40, "1.2  s"},
  {0x41, "740  ms"},
  {0x42, "440  ms"},
  {0x43, "290  ms"},
  {0x44, "180  ms"},
  {0x45, "110  ms"},
  {0x46, "74   ms"},
  {0x47, "37   ms"}, */