#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <cstdlib>
/* Each BRR block is a 1 byte header followed by 16 nibbles, or 8 bytes.
 * The nibble themselves are signed -8 to +7. The manual has a typo
 * stating -7 to +8; ignore it. But my machine is showing
 * these as -8 to +7, so I must re-interpret the units.
 * */
#define BRR_LEN 0x9
#define BRR_HDR_OFS 0x00
// Bit field defs for the header byte
#define BRR_HDR_BLKEND 0x01
#define BRR_HDR_LOOP 0x02
#define BRR_HDR_FILTER 0x0c
#define BRR_HDR_RANGE 0xf0

struct BrrFilter
{
  double a, b;
};
BrrFilter brrfilter[4] =
{
  {0,         0},
  {0.9375,    0},
  {1.90625,  -0.9375},
  {1.796875, -0.8125 },
};
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
  int8_t s0 :4;
  int8_t s1 :4;
  int8_t s2 :4;
  int8_t s3 :4;
  int8_t s4 :4;
  int8_t s5 :4;
  int8_t s6 :4;
  int8_t s7 :4;
  int8_t s8 :4;
  int8_t s9 :4;
  int8_t sa :4;
  int8_t sb :4;
  int8_t sc :4;
  int8_t sd :4;
  int8_t se :4;
  int8_t sf :4;
} __attribute__((packed));

inline bool is_block_end(Brr *brr)
{
  return brr->hdr & BRR_HDR_BLKEND ? true : false;
}
// This is only found set in the block_end block
inline bool is_loop(Brr *brr)
{
  return brr->hdr & BRR_HDR_LOOP ? true : false;
}

inline uint8_t get_filter(Brr *brr)
{
  return (brr->hdr & BRR_HDR_FILTER) >> 2;
}

inline uint8_t get_range(Brr *brr)
{
  return (brr->hdr & BRR_HDR_RANGE) >> 4;
}

struct WavFmtSubchunk
{
  char id[4]; // "fmt "
  uint32_t size;
  uint16_t audioformat;
  uint16_t numchannels;
  uint32_t samplerate;
  uint32_t byterate;
  uint16_t blockalign;
  uint16_t bitspersample;
} __attribute__((packed));

struct WavDataSubchunk
{
  char id[4]; // "data"
  uint32_t size;
} __attribute__((packed));

struct RiffHdr
{
  char riff[4]; //RIFF
  uint32_t size;
  char wave[4]; //WAVE

  WavFmtSubchunk fmt;
  WavDataSubchunk data;
} __attribute__((packed));

#define WAV_OFFSET1 0x4
#define WAV_OFFSET2 40
/*"RIFF0000WAVE"
"fmt \x10\x00\x00\x00" "\x01\x00" "\x01\x00"
"\x00\x7d\x00\x00" "\x00\xf4\x01\x00" "\x04\x00" "\x10\x00"
"data0000"*/
// "RIFF" + LE4-filesize-8 + "WAVE"
// "fmt " + 0x00000010 + 0x0001 + 0x0002
// 0x00007d00 + 0x0001f400 + 0x0004 + 0x0010
// "data" LE4size then ll the data
void init_Riff(RiffHdr *hdr)
{
  char *c = hdr->riff;
  *(c++) = 'R';
  *(c++) = 'I';
  *(c++) = 'F';
  *(c++) = 'F';

  // skipping chunk size

  c = hdr->wave;
  *(c++) = 'W';
  *(c++) = 'A';
  *(c++) = 'V';
  *(c++) = 'E';

  c = hdr->fmt.id;
  *(c++) = 'f';
  *(c++) = 'm';
  *(c++) = 't';
  *(c++) = ' ';

  c = hdr->data.id;
  *(c++) = 'd';
  *(c++) = 'a';
  *(c++) = 't';
  *(c++) = 'a';
}

void write_wav_header(off_t size, int wfd)
{
  RiffHdr hdr;
  WavFmtSubchunk *fmt = &hdr.fmt;
  init_Riff(&hdr);
  
  hdr.size = size + 36;
  fmt->size = 0x0010;
  fmt->audioformat = 1; //PCM
  fmt->numchannels = 1; //mono
  fmt->samplerate = 32000;
  fmt->byterate = 32000 * 1 * 16 / 8;
  fmt->blockalign = 1 * 16 / 8;
  fmt->bitspersample = 16;
  hdr.data.size = size;
  
  write(wfd, &hdr, sizeof(RiffHdr));
}
static int16_t clamp_16(int32_t n)
{
  if ((int16_t)n != n)
    n =(int16_t)(0x7FFF -(n>>24));
  return (int16_t)n;
}
static inline void calc(Brr *brr, int8_t s, int wfd)
{
  static double l1 = 0, l2 = 0;

  double dx;

  dx = (((s) << (brr->range)))
    + (brrfilter[brr->filter].a * l1)
    + (brrfilter[brr->filter].b * l2);

  l2 = l1;
  l1 = dx; //clamp_16(dx);
  
  int16_t x16 = (int16_t)dx;
  
  write(wfd, &x16, 2);
}

int decode_packet(Brr *brr, int wfd)
{
  /* x = R + ax1 + ax2
   *     (int16_t) R = d << range
   *
   */
  
  calc(brr, brr->s1, wfd);
  calc(brr, brr->s0, wfd);
  calc(brr, brr->s3, wfd);
  calc(brr, brr->s2, wfd);
  calc(brr, brr->s5, wfd);
  calc(brr, brr->s4, wfd);
  calc(brr, brr->s7, wfd);
  calc(brr, brr->s6, wfd);
  calc(brr, brr->s9, wfd);
  calc(brr, brr->s8, wfd);
  calc(brr, brr->sb, wfd);
  calc(brr, brr->sa, wfd);
  calc(brr, brr->sd, wfd);
  calc(brr, brr->sc, wfd);
  calc(brr, brr->sf, wfd);
  calc(brr, brr->se, wfd);

  printf("blkend: %d, loop: %d, filter: %d, range: %d\n",
         brr->end, brr->loop, brr->filter, brr->range);
}

int main(int argc, char **argv)
{
  // Input file comes in on argv[1]
  char newpath[100];
  int16_t *buf;
  int8_t in_buf[9];
  int fin = open(argv[1], O_RDONLY);

  struct stat statbuf;
  fstat(fin, &statbuf);
  off_t size = statbuf.st_size;
  off_t uncompressed_size = (size / 9) * 16 * 2;
  // for every nine bytes of BRR packet, we get out 8*2 = 16 bytes of WAV
  // data

  buf = (int16_t *) malloc(sizeof(int16_t) * uncompressed_size / 2);

  strcpy(newpath, argv[1]);
  strcat(newpath, ".wav");
  int fout = open(newpath, O_CREAT | O_WRONLY, S_IRWXU);

  write_wav_header(uncompressed_size, fout);

  do
  {
    read (fin, in_buf, 0x9);
    Brr *brr = (Brr *)in_buf;
    decode_packet(brr, fout);
  } while (size -= 9);


  return 0;
}
