/*
// Changelog
//12-21-2014
Goals: 
Add support for ID666
Create API functions

//12-20-2014
// first example without ID666 info


*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <stdint.h>

uint32_t pc;

#define ERR -1

int craft_skeleton();
extern char *optarg;
extern int optind;
short sample_startaddr=0;

#define SPC_HEADER_STR "SNES-SPC700 Sound File Data v0.30"

struct org
{
  FILE *fp;
  char *fname;
  struct stat stats;

  int size() { return stats.st_size; }
  void load() 
  { 
    fp = fopen(fname, "rb");
    if (fp == NULL)
    {
      perror("fopen");
      exit (2);
    }
  }
};

struct outfile
{
  char *fname;
  FILE *fp;

  outfile() { fname = NULL; fp = NULL; }
  outfile(char *fn) { strcpy(fname, fn); }

  void load() 
  { 
    fp = fopen(fname, "wb");
    if (fp == NULL)
    {
      perror("fopen");
      exit (2);
    }
  }

  void write(char *str)
  {
    fputs(str, fp);
  }
  void write (char c)
  {
    fputc(c, fp);
  }
  void write(uint32_t i)
  {
    // PC is 16-bit
    char c = (i & 0xff);
    write(c);
    c = ((i >> 8) & 0xff);
    write(c);
    /*c = (i >> 8) & 0xff;
    write(c);
    c = (i >> 8) & 0xff;
    write(c);*/
  }
  void writeorg(org o)
  {
    int i = o.size();
    while (i--)
    {
      char c = fgetc(o.fp);
      fputc(c, fp);
    }
  }

  void close()
  {
    fclose(fp);
  }
};

org sample, prog;
outfile out;
//char *sample_name, *prog_name;
//struct stat sample_stat, prog_stat;



int getopts(int argc, char **argv)
{
  int c, err = 0; 
  int pflag=0, sflag=0, oflag=0, cflag=0, aflag;
  char *sname = (char *)"default_sname", *fname;
  static char usage[] = "usage: %s -s sample_name -a sample_upload_addr -p spc_prog_obj_name -c prog_pc\n";

  while ((c = getopt(argc, argv, "a:s:p:o:c:")) != -1)
    switch (c) {
    case 'p':
      pflag = 1;
      prog.fname = optarg;
      break;
    case 's':
      sflag = 1;
      sample.fname = optarg;
      break;
    case 'o':
      oflag = 1;
      out.fname = optarg;
      break;

    case 'c':
      cflag = 1;
      pc = strtoul( optarg, (char **)0, 0 );
      break;
    case 'a':
      aflag = 1;
      sample_startaddr = strtoul( optarg, (char **)0, 0 );
      break;
    case '?':
      err = 1;
      break;
    }
  if (sflag == 0) { /* -f was mandatory */
    fprintf(stderr, "%s: missing -s option\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  } 

  if (pflag == 0) { /* -f was mandatory */
    fprintf(stderr, "%s: missing -p option\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  if (oflag == 0) { /* -f was mandatory */
    fprintf(stderr, "%s: missing -o option\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }
  if (cflag == 0) { /* -f was mandatory */
    fprintf(stderr, "%s: missing -c option\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }
  if (aflag == 0) { /* -f was mandatory */
    fprintf(stderr, "%s: missing -a option\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }

  if ((optind) > argc) { 
    /* need at least one argument (change +1 to +2 for two, etc. as needeed) */

    printf("optind = %d, argc=%d\n", optind, argc);
    fprintf(stderr, "%s: missing name\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    exit(1);
  } else if (err) {
    fprintf(stderr, usage, argv[0]);
    exit(1);
  }
  /* see what we have */
  printf("pflag = %d\n", pflag);
  printf("sflag = %d\n", sflag);
  printf("oflag = %d\n", oflag);
  printf("pname = \"%s\"\n", prog.fname);
  printf("sname = \"%s\"\n", sample.fname);
  printf("oname = \"%s\"\n", out.fname);
  
  if (optind < argc)  /* these are the arguments after the command-line options */
    for (; optind < argc; optind++)
      printf("argument: \"%s\"\n", argv[optind]);
  else {
    printf("no arguments left to process\n");
  }
  return 0;
}

int get_filesize(char *filename, struct stat *sp)
{
  if (stat(filename, sp) == ERR) {
    (void) fprintf(stderr, ": stat error of %s:\n", filename);
    perror("");
    exit(1);
  };

  return sp->st_size;
}

int main(int argc, char **argv)
{
  // -s Sample filename
  // -p SPC prog OBJ filename
  getopts(argc,argv);

  //printf("sample_name = %s, prog_name = %s\n", sample.fname, prog.fname);
  // get each file size
  get_filesize(sample.fname, &sample.stats);
  get_filesize(prog.fname, &prog.stats);

  printf ("sample size = %d\n", sample.size());
  printf ("prog size = %d\n", prog.size());

  // load up the files
  sample.load();
  prog.load();
  out.load();

  // Start crafting an SPC file using spc_file_format.txt
  craft_skeleton();

  out.close();
  //
  printf("done\n");
}


/* ID666 Tag (text format):
0002Eh    32 Song title
0004Eh    32 Game title
0006Eh    16 Name of dumper
0007Eh    32 Comments
0009Eh    11 Date SPC was dumped (MM/DD/YYYY)
000A9h     3 Number of seconds to play song before fading out
000ACh     5 Length of fade in milliseconds
000B1h    32 Artist of song
000D1h     1 Default channel disables (0 = enable, 1 = disable)
000D2h     1 Emulator used to dump SPC:
             0 = unknown
       1 = ZSNES
       2 = Snes9x
000D3h    45 reserved (set to all 0's)

*/

/* 

Order of operations
1) Craft Skeleton SPC-File - craft_skeleton(bool ID666)
2) Optionally Craft ID666
3) Save file (close file)

*/
struct ID666_Text
{
  char SongTitle[32];
  char GameTitle[32];
  char DumperName[16];
  char Comments[32];
  char DateofDumpage[11]; // MM/DD/YYYY\0
  char sextoPlaySongBeforeFadingOut[3]; // 3 chars (no NULL)
  char LengthofFadeinMs[5]; // 5 chars (no NULL)
  char ArtistName[32];
  char defaultChannelDisables;
  char EmulatorUsed;
};
int craft_ID666_Text( )
{

  return 0;
}


// For either an ID666 or non ID666 SPC, this skeleton must be created: 
void craft_skeleton(bool ID666=false)
{
  // @ 0x000000, write "SNES-SPC700 Sound File Data v0.30" 
  out.write((char*)SPC_HEADER_STR);

  // write 26,26
  out.write((char)26);
  out.write((char)26);

  // write ID666 info or not
  if (ID666)
    out.write((char)26);
  else out.write((char)27);

  // write min version
  out.write((char)30);

  // should be at Addr 0x25
  assert (ftell(out.fp) == 0x25);
  // write SPC700 Regs
  // PC
  out.write((char)0);
  //out.write((uint32_t)pc);
  // A
  out.write((char)0);
  // X
  out.write((char)0);
  // Y
  out.write((char)0);
  // PSW
  out.write((char)0);
  // SP (lower byte 0x1XX)
  out.write((char)0xff);
  // 2 byte reserved
  out.write((char)0x00);
  out.write((char)0x00);
  // shold be at 0x2E
  assert (ftell(out.fp) == 0x2E);
  // skip this ID data..
}

craft_spc_ram_image()
{
  // goto 0x100
  // SPC 64 KB RAM upload
  // @ 0x200 + 0x100 = 0x300, upload sample
  fseek(out.fp, 0x100 + sample_startaddr, SEEK_SET);
  //fseek(out.fp, sample_startaddr, SEEK_CUR);

  out.writeorg(sample);

  // @ 0x100 + 0x1000 = 0x1100, upload prog
  fseek(out.fp, pc + 0x100, SEEK_SET);
  out.writeorg(prog);
  // goto 0x10100, write all 0's 0x80 times
  fseek(out.fp, 0x10100, SEEK_SET);
  for (int i=0x80; i > 0; i--)
  {
    out.write((char)0x00);
  }
  // write 128 0's
  for (int i=0x80; i > 0; i--)
  {
    out.write((char)0x00);
  }
  return 0;
}