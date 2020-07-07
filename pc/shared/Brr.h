#pragma once
#include <stdint.h>
/* Here is defined, most importantly, the raw Brr data block format. These
 * can be used to iterate through a BRR sample one block at a time until
 * the END block is reached (eg. brr->end is true).
 *
 * Also included are all the brr filter coefficients for use in encoding
 * and decoding BRR samples. The brr filter can be indexed directly by the
 * BRR block's filter subcomponent (brr->filter). */
struct BrrFilter
{
  double a, b;
};

extern const BrrFilter brrfilter[4];

struct Brr
{
  union
  {
    uint8_t hdr;
    struct
    {
      uint8_t end    :1;
      uint8_t loop   :1;
      uint8_t filter :2;
      uint8_t range  :4;
    } __attribute__((packed));
  };
  union
  {
    int8_t samples[8];
 
    struct
    {
      int8_t s1 :4;
      int8_t s0 :4;
    } __attribute__((packed)) samples2[8];

    struct
    {
      int8_t s1 :4;
      int8_t s0 :4;
      int8_t s3 :4;
      int8_t s2 :4;
      int8_t s5 :4;
      int8_t s4 :4;
      int8_t s7 :4;
      int8_t s6 :4;
      int8_t s9 :4;
      int8_t s8 :4;
      int8_t sb :4;
      int8_t sa :4;
      int8_t sd :4;
      int8_t sc :4;
      int8_t sf :4;
      int8_t se :4;
    } __attribute__((packed));
  };
} __attribute__((packed));

// The following definitions were never used but are included for
// posterity.
/* Each BRR block is a 1 byte header followed by 16 nibbles, or 8 bytes.
 * (9 bytes total)
 * The nibble themselves are signed -8 to +7. The manual has a typo
 * stating -7 to +8; ignore it.
 *
 * Oddly enough, the first sample is placed in the high nibble of the byte*/
#define BRR_LEN 0x9
#define BRR_HDR_OFS 0x00
// Bit field defs for the header byte
#define BRR_HDR_BLKEND 0x01
#define BRR_HDR_LOOP 0x02
#define BRR_HDR_FILTER 0x0c
#define BRR_HDR_RANGE 0xf0

int write_brr_to_file(const Brr *brr);
uint16_t get_brr_size(const Brr *brr);
Brr * getBrrEndBlock(Brr *brr);
bool doesBrrLoop(Brr *brr);
// SORT OF SEPARATE SNES TRACKER BRR RELATED STUFF
#define BRRP_MAGIC_STR "ST-BRRP"  // the S has the low 2 bits set,
//an unlikely situation for any BRR Sample
// 8-byte aligned
#define BRR_FILE_EXTENSION "brr"
#define BRRP_FILE_EXTENSION "brrp"
#define BRRI_MAGIC_STR "ST-BRRI"  // 8 byte aligned
#define BRRI_FILE_EXTENSION "brri"
