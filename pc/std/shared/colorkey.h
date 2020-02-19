#pragma once

union Colorkey {
  Uint32 rgb;
  struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
  } __attribute__((packed));
};
