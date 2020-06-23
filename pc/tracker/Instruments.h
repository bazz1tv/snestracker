#pragma once
#include <stdint.h>
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Button.h"
#include "shared/Render.h"
#include "shared/dsptypes.h"
#include "ChunkLoader.h"
/* This number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_INSTR 0x40
#define INSTR_NAME_MAXLEN 22
#define APU_MAXVOL 0x7f

// TODO : take out meta info from Instrument into a InstrumentMeta
// datatype that inherits from Instrument. that should allow simplifying
// render_to_apu into memcpy on Instrument type into APU RAM
struct Instrument
{
  Instrument();
  ~Instrument();
  /* TODO: Move this metadata outside of the core instrument */
  char name[INSTR_NAME_MAXLEN]; // the name of the instrument
  /* the app-level sample data. How that gets exported into the snes
   * driver data is a different story */
	uint8_t srcn; // like a DIR offset
	Adsr adsr; // The volume envelope that will be used for this instrument
	/* Aside from the ADSR hardware volume envelope, the voice stereo volume
	 * may be adjusted real-time for additional effects.*/
	/* Note on volume envelopes as I learned from Milky Tracker. The graph
	 * view X-axis is specified in ticks, up to 0x100 ticks. So each point
	 * of the envelope is also specified in ticks */
	//VolumeEnvelope ve;
	//PanEnvelope pe;
	//  bool phaseflip=false;
  uint8_t vol;
  /* 0x80 = dead center, 0xFF = hard right, 0x00 = hard left */
  uint8_t pan;
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  int8_t semitone_offset;
  /* used to pitch the sample for range of +/- one semitone. Tracker
   * software must impose retraints on the allowed range of values. Will
   * be used by SNES Driver */
  int8_t finetune;

  bool operator==(const Instrument& rhs);
  inline bool operator!=(const Instrument& rhs) { return !(*this == rhs); }

	static void inc_srcn(Instrument *i);
	static void dec_srcn(Instrument *i);
  static void inc_vol(Instrument *i);
  static void dec_vol(Instrument *i);
  static void inc_pan(Instrument *i);
  static void dec_pan(Instrument *i);
  static void inc_finetune(Instrument *i);
  static void dec_finetune(Instrument *i);
};

class InstrumentChunkLoader : public ChunkLoader
{
public:
  InstrumentChunkLoader(struct Instrument *instruments, bool ignoreSongMeta=false);
  size_t load(SDL_RWops *file, size_t chunksize);
  // save specific instrument to file handle. This does not include a File Header
  size_t save(SDL_RWops *file, int instNumber);
  size_t save(SDL_RWops *file); // Save all instruments to file handle
  // Helper for non-Song loading
  size_t save(SDL_RWops *file, struct Instrument *instr);

  inline void setIdx(uint8_t idx) { this->idx = idx; idx_loaded = true; }

  enum SubChunkID {
    coreinfo=0,
    songmeta,
    name,
    tune,
    NUM_SUBCHUNKIDS
  };
private:
  struct Instrument *instruments;
  bool ignoreSongMeta;
  uint8_t idx = 0; //instrument index
  bool idx_loaded = false;
};

////////// STI INSTRUMENTS //////////////////////////////////////

// Instrument File info
//////////////////////////////////////////////////////////////////
#define INSTRFILE_EXT "sti"

#define INSTRFILE_VER_MAJOR 0
#define INSTRFILE_VER_MINOR 0
#define INSTRFILE_VER_MICRO 1
// concats the 3 version values into a string
#define INSTRFILE_VER_STRING(maj, min, mic) #maj "." #min "." #mic
// allows calling the above macro with macro arguments
#define INSTRFILE_VER_STRING2(maj, min, mic) INSTRFILE_VER_STRING(maj, min, mic)
// The final actual "public" macro for a version string
#define INSTRFILE_VERSION INSTRFILE_VER_STRING2(INSTRFILE_VER_MAJOR, INSTRFILE_VER_MINOR, INSTRFILE_VER_MICRO)
//////////////////////////////////////////////////////////////////

struct Sample;
class SampleChunkLoader;
class InstrumentFileLoader
{
public:
  static const char constexpr HeaderStr[] = "STInst";
  STATIC_HEADER_CHECK(HeaderStr, FILE_HEADER_LEN);

  InstrumentFileLoader(Instrument *instrument, Sample *sample);
  ~InstrumentFileLoader();

  enum ret_t {
    HEADER_OK=0,
//    HEADER_OLD,
    HEADER_BAD,
    FILE_LOADED,
    FILE_NOT_LOADED,
  };

  size_t save(SDL_RWops *file);
  ret_t load(SDL_RWops *file);

private:
  ret_t readHeader(SDL_RWops *file);

/* Here lies a collection of subclassed FileLoaders that are associated with
 * this file type. */
  VersionChunkLoader *vcl;
  SampleChunkLoader *scl;
  InstrumentChunkLoader *icl;

  Instrument *instrument;
};


struct Sample_Panel;
/* That defined the Data model above. Now time to get that into a view */
struct Instrument_Panel
{
  /* Initialize the panel view from an X/Y coordinate. Additionally, we
   * need a reference to the instruments */
  Instrument_Panel(Instrument *instruments, Sample_Panel *sp);
  ~Instrument_Panel();

  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void set_coords(int x, int y);

	void set_currow(int c);
	void inc_currow();
	void dec_currow();

  // callback funcs for the buttons
  static int load(void *ipanel);
  static int save(void *ipanel);
  static int zap(void *ipanel);

  Text title;
  Button loadbtn, savebtn, zapbtn;
  Text instr_indices[NUM_INSTR];

  // the number of instrument rows in GUI
  static const int NUM_ROWS = 8;
  int rows_scrolled = 0;
  // the current selected row
  int currow = 0;

  // 4 is for eg. "01|\0"
  char instr_index_strings[NUM_ROWS][4];
  Text_Edit_Rect instr_names[NUM_ROWS];
  /* a direct handle on the data, rather than accessing through an API */
  Instrument *instruments;
	// handle on the sample panel to update its currow when selecting an
	// instr
	Sample_Panel *samplepanel;
  /* Todo, calculate the panel rect */
  SDL_Rect rect; // define the boundaries of the entire panel
  SDL_Rect highlight_r; // the highlight rect of current select instr
};
