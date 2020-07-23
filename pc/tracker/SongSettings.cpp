#include "SongSettings.h"
#include "Tracker.h"
#include "apuram.h"

SongSettings::SongSettings()
{
  reset();
}

void SongSettings::reset()
{
  bpm = DEFAULT_BPM; spd = DEFAULT_SPD;
  setdefault_songtitle();
  setdefault_volandecho();
  setdefault_fir();
}

void SongSettings::setdefault_songtitle() { song_title_str[0] = 0; }
void SongSettings::setdefault_volandecho() {
  mvol = 0x40;
  evol = 0x06;
  edl = 0x05;
  efb = 0x40;
}
void SongSettings::setdefault_fir() {
  fir[0] = 0x7f;
  for (int i=1; i < 8; i++)
    fir[i] = 0;
}

void SongSettings::inc_bpm()
{
  if (bpm >= MAX_BPM)
    bpm = MAX_BPM;
  else bpm++;
  *metadata.changed = true;
}

void SongSettings::dec_bpm()
{
  if (bpm <= MIN_BPM)
    bpm = MIN_BPM;
  else bpm--;
  *metadata.changed = true;
}

void SongSettings::inc_spd()
{
  if (spd >= MAX_SPD)
    spd = MAX_SPD;
  else spd++;
  *metadata.changed = true;
}

void SongSettings::dec_spd()
{
  if (spd <= MIN_SPD)
    spd = MIN_SPD;
  else spd--;
  *metadata.changed = true;
}

void SongSettings::inc_vol(uint8_t *vol)
{
  *vol += (*vol < 0x7F) ? 1 : 0;
  ::tracker->song.changed = true;
}
void SongSettings::dec_vol(uint8_t *vol)
{
  *vol -= (*vol > 0) ? 1 : 0;
  ::tracker->song.changed = true;
}

void SongSettings::inc_edl()
{
  edl += (edl < 0x0F) ? 1 : 0;
  *metadata.changed = true;
}
void SongSettings::dec_edl()
{
  edl -= (edl > 0) ? 1 : 0;
  *metadata.changed = true;
}

// Allow wrapping for EFB
void SongSettings::inc_efb()
{
  efb++;
  *metadata.changed = true;
}
void SongSettings::dec_efb()
{
  efb--;
  *metadata.changed = true;
}

SongSettingsChunkLoader::SongSettingsChunkLoader(struct SongSettings *ss) :
  songsettings(ss), ChunkLoader(ChunkID::SongSettings)
{}

size_t SongSettingsChunkLoader::load(SDL_RWops *file, size_t chunksize)
{
  size_t maxread = 0;

  DEBUGLOG("SongSettingsChunkLoader::load()\n");

  while (maxread < chunksize)
  {
    uint8_t subchunkid;
    uint16_t subchunksize;
    read(file, &subchunkid, 1, 1, &maxread);
    read(file, &subchunksize, 2, 1, &maxread);

    switch (subchunkid)
    {
      case SubChunkID::songtitle:
      {
        DEBUGLOG("\tSubChunkID::songtitle");
        size_t bytesread = ChunkLoader::read_str_from_file2(file, songsettings->song_title_str, subchunksize, SongSettings::SONGTITLE_SIZE);
        subchunksize -= bytesread;
        maxread += bytesread;
      }
      break;
      case SubChunkID::bpmspd:
      {
        DEBUGLOG("\tSubChunkID::bpmspd\n");
        size_t minimum_chunksize = 2;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. Setting to default\n", subchunkid);
          songsettings->bpm = SongSettings::DEFAULT_BPM;
          songsettings->spd = SongSettings::DEFAULT_SPD;
          break;
        }

        uint16_t bpmspd;
        size_t rc = read(file, &bpmspd, 2, 1, &maxread);
        if (rc == 0)
        {
          DEBUGLOG("\t\tCould not read from file: %s\n", SDL_GetError());
          return -1;
        }
        subchunksize -= 2;

        // Check for valid BPM/SPD
        songsettings->bpm = bpmspd >> 6;
        if (songsettings->bpm < SongSettings::MIN_BPM || songsettings->bpm > SongSettings::MAX_BPM)
        {
          DEBUGLOG("\t\tInvalid BPM: %d. Setting to default %d\n", songsettings->bpm, SongSettings::DEFAULT_BPM);
          songsettings->bpm = SongSettings::DEFAULT_BPM;
        }

        songsettings->spd = (uint8_t)(bpmspd & 0b111111);
        if (songsettings->spd < SongSettings::MIN_SPD || songsettings->spd > SongSettings::MAX_SPD)
        {
          DEBUGLOG("\t\tInvalid SPD: %d. Setting to default %d\n", songsettings->spd, SongSettings::DEFAULT_SPD);
          songsettings->spd = SongSettings::DEFAULT_SPD;
        }
      }
      break;
      case SubChunkID::volandecho:
      {
        DEBUGLOG("\tSubChunkID::volandecho\n");
        size_t minimum_chunksize = 4;
        if (subchunksize > minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is bigger than expected.\n", subchunkid);
        }
        else if (subchunksize < minimum_chunksize)
        {
          DEBUGLOG("\t\tSubChunk %d is smaller than expected. Setting to default\n", subchunkid);
          songsettings->setdefault_volandecho();
          break;
        }
        uint8_t byte;
        /* mvol */
        read(file, &byte, 1, 1, &maxread);
        songsettings->mvol = byte;
        /* evol */
        read(file, &byte, 1, 1, &maxread);
        songsettings->evol = byte;
        /* edl */
        read(file, &byte, 1, 1, &maxread);
        songsettings->edl = byte;
        /* efb */
        read(file, &byte, 1, 1, &maxread);
        songsettings->efb = byte;

        subchunksize -= 4;
      }
      break;
      /* TODO: fir */
      default:
        DEBUGLOG("\tUnknown SubChunkID: %d. skipping over..\n", subchunkid);
      break;
    }

    /* Skip the unrecognized part of the chunk */
    if (subchunksize)
    {
      DEBUGLOG("\tskipping past %d unknown bytes of chunk\n", subchunksize);
      SDL_RWseek(file, subchunksize, RW_SEEK_CUR);
      maxread += subchunksize;
    }
  }
  return maxread;
}

size_t SongSettingsChunkLoader::save(SDL_RWops *file)
{
  uint8_t byte;
  uint16_t word;
  uint16_t chunklen = 0;
  Sint64 chunksize_location, chunkend_location;

  DEBUGLOG("SongSettingsChunkLoader::save()\n");

  DEBUGLOG("\tWriting top-level chunkid: %d\n", chunkid);
  byte = chunkid;
  SDL_RWwrite(file, &byte, 1, 1);
  chunksize_location = SDL_RWtell(file);
  SDL_RWwrite(file, &chunklen, 2, 1);
  // write song title
  const char *songtitle = songsettings->song_title_str;

  byte = SubChunkID::songtitle;
  word = strlen(songtitle);
  // don't even write a songtitle chunk if there's no string
  if (word > 0)
  {
    DEBUGLOG("\t\tSubChunkID::songtitle\n");
    write(file, &byte, 1, 1, &chunklen);
    write(file, &word, 2, 1, &chunklen);
    write(file, songtitle, word, 1, &chunklen); // also write null byte
  }

  DEBUGLOG("\t\tSubChunkID::bpmspd\n");
  byte = SubChunkID::bpmspd;
  word = 2;
  write(file, &byte, 1, 1, &chunklen);
  write(file, &word, 2, 1, &chunklen);

  uint16_t bpmspd = ((uint16_t)songsettings->bpm << 6) | songsettings->spd;
  write(file, &bpmspd, 2, 1, &chunklen);

  DEBUGLOG("\t\tSubChunkID::volandecho\n");
  byte = SubChunkID::volandecho;
  word = 4;
  write(file, &byte, 1, 1, &chunklen);
  write(file, &word, 2, 1, &chunklen);

  /* mvol */
  byte = songsettings->mvol;
  write(file, &byte, 1, 1, &chunklen);
  /* evol */
  byte = songsettings->evol;
  write(file, &byte, 1, 1, &chunklen);
  /* edl */
  byte = songsettings->edl;
  write(file, &byte, 1, 1, &chunklen);
  /* efb */
  byte = songsettings->efb;
  write(file, &byte, 1, 1, &chunklen);

  DEBUGLOG("\tWriting chunksize\n");
  chunkend_location = SDL_RWtell(file);
  SDL_RWseek(file, chunksize_location, RW_SEEK_SET);
  SDL_RWwrite(file, &chunklen, 2, 1);

  SDL_RWseek(file, chunkend_location, RW_SEEK_SET);
  return 0;
}

/* OF course I am aware of the repetitive nature of this code impl. But
 * let's get this thing off the ground first and maybe some day we can
 * consider how it could be refactored into something more elegant */

extern Tracker *tracker;

SongSettingsPanel::SongSettingsPanel() :
	mvol_cbuf("00"),
	mvol_title("Main Vol"),
	mvol_valtext(mvol_cbuf),
	mvol_incbtn("+", inc_mvol, this, true),
	mvol_decbtn("-", dec_mvol, this, true),

  evol_cbuf("00"),
  evol_title("Echo Vol"),
  evol_valtext(evol_cbuf),
  evol_incbtn("+", inc_evol, this, true),
  evol_decbtn("-", dec_evol, this, true),

  edl_cbuf("00"),
  edl_title("Echo Delay"),
  edl_valtext(edl_cbuf),
  edl_incbtn("+", inc_edl, this, true),
  edl_decbtn("-", dec_edl, this, true),
  
  efb_cbuf("00"),
  efb_title("Echo Feedback"),
  efb_valtext(efb_cbuf),
  efb_incbtn("+", inc_efb, this, true),
  efb_decbtn("-", dec_efb, this, true)
{
}

void SongSettingsPanel::update_mvol()
{
	sprintf(mvol_cbuf, "$%02x", ::tracker->song.settings.mvol);
}

void SongSettingsPanel::update_evol()
{
  sprintf(evol_cbuf, "$%02x", ::tracker->song.settings.evol);
}

void SongSettingsPanel::update_edl()
{
  sprintf(edl_cbuf, "$%02x", ::tracker->song.settings.edl);
}

void SongSettingsPanel::update_efb()
{
  sprintf(efb_cbuf, "$%02x", ::tracker->song.settings.efb);
}

/* TODO: FIR */

void SongSettingsPanel :: set_coords(int x, int y)
{
  int yy = y;
  /* NOTE: I am using "Main Vol" string for all vars because it represents
   * the max length, the textual content is not important here; it was
   * easier to just copy/paste that one */

	mvol_title.rect.x = x;
	mvol_title.rect.y = y;
	mvol_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
	mvol_valtext.rect.y = y;
	mvol_decbtn.rect.x  = mvol_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
	mvol_decbtn.rect.y = y;
	mvol_incbtn.rect.x = mvol_decbtn.rect.x + CHAR_WIDTH + 5;
	mvol_incbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;

  evol_title.rect.x = x;
  evol_title.rect.y = y;
  evol_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  evol_valtext.rect.y = y;
  evol_decbtn.rect.x  = evol_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  evol_decbtn.rect.y = y;
  evol_incbtn.rect.x = evol_decbtn.rect.x + CHAR_WIDTH + 5;
  evol_incbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;
  
  edl_title.rect.x = x;
  edl_title.rect.y = y;
  edl_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  edl_valtext.rect.y = y;
  edl_decbtn.rect.x  = edl_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  edl_decbtn.rect.y = y;
  edl_incbtn.rect.x = edl_decbtn.rect.x + CHAR_WIDTH + 5;
  edl_incbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;
  
  efb_title.rect.x = x;
  efb_title.rect.y = y;
  efb_valtext.rect.x = x + ((sizeof("Main Vol      ")-1) * CHAR_WIDTH);
  efb_valtext.rect.y = y;
  efb_decbtn.rect.x  = efb_valtext.rect.x + (sizeof("$00") * CHAR_WIDTH);
  efb_decbtn.rect.y = y;
  efb_incbtn.rect.x = efb_decbtn.rect.x + CHAR_WIDTH + 5;
  efb_incbtn.rect.y = y;

  //y += CHAR_HEIGHT + 5;
}

int SongSettingsPanel::handle_event(const SDL_Event &ev)
{
	mvol_incbtn.check_event(ev);
	mvol_decbtn.check_event(ev);

  evol_incbtn.check_event(ev);
  evol_decbtn.check_event(ev);

  edl_incbtn.check_event(ev);
  edl_decbtn.check_event(ev);

  efb_incbtn.check_event(ev);
  efb_decbtn.check_event(ev);
  return 0;
}

void SongSettingsPanel::draw(SDL_Surface *screen/*=::render->screen*/)
{
	update_mvol();
  update_evol();
  update_edl();
  update_efb();

	mvol_title.draw(screen);
	mvol_valtext.draw(screen);
	mvol_incbtn.draw(screen);
	mvol_decbtn.draw(screen);

  evol_title.draw(screen);
  evol_valtext.draw(screen);
  evol_incbtn.draw(screen);
  evol_decbtn.draw(screen);

  edl_title.draw(screen);
  edl_valtext.draw(screen);
  edl_incbtn.draw(screen);
  edl_decbtn.draw(screen);
  
  efb_title.draw(screen);
  efb_valtext.draw(screen);
  efb_incbtn.draw(screen);
  efb_decbtn.draw(screen);
}

int SongSettingsPanel::inc_mvol(void *i)
{
	SongSettingsPanel *ie = (SongSettingsPanel *)i;
	SongSettings::inc_vol(&::tracker->song.settings.mvol);
	ie->update_mvol();

  if (tracker->rendering())
  {
    auto mvol = ::tracker->song.settings.mvol;
    ::player->spc_write_dsp(dsp_reg::mvol_l, mvol);
    ::player->spc_write_dsp(dsp_reg::mvol_r, mvol);
    tracker->apuram->mvol_val = mvol;
  }
	return 0;
}

int SongSettingsPanel::dec_mvol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::dec_vol(&::tracker->song.settings.mvol);
  ie->update_mvol();

  if (tracker->rendering())
  {
    auto mvol = ::tracker->song.settings.mvol;
    ::player->spc_write_dsp(dsp_reg::mvol_l, mvol);
    ::player->spc_write_dsp(dsp_reg::mvol_r, mvol);
    tracker->apuram->mvol_val = mvol;
  }
  return 0;
}

int SongSettingsPanel::inc_evol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::inc_vol(&::tracker->song.settings.evol);
  ie->update_evol();

  if (tracker->rendering())
  {
    auto evol = ::tracker->song.settings.evol;
    ::player->spc_write_dsp(dsp_reg::evol_l, evol);
    ::player->spc_write_dsp(dsp_reg::evol_r, evol);
    tracker->apuram->evol_val = evol;
  }
  return 0;
}

int SongSettingsPanel::dec_evol(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  SongSettings::dec_vol(&::tracker->song.settings.evol);
  ie->update_evol();

  if (tracker->rendering())
  {
    auto evol = ::tracker->song.settings.evol;
    ::player->spc_write_dsp(dsp_reg::evol_l, evol);
    ::player->spc_write_dsp(dsp_reg::evol_r, evol);
    tracker->apuram->evol_val = evol;
  }
  return 0;
}

int SongSettingsPanel::inc_edl(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->song.settings.inc_edl();
  ie->update_edl();

  if (tracker->rendering())
  {
    auto edl = ::tracker->song.settings.edl;
    auto esa = calcESAfromEDL(edl);
    /* The order of the following two writes is CRUCIAL. Since the echo buffer
     * is mapped to the end of RAM. The ordering avoids rollover and changes
     * whether we are incrementing or decrementing the edl (growing or shrinking
     * the echo buffer and relocating its start address) */
    ::player->spc_write_dsp(dsp_reg::esa, esa);
    ::player->spc_write_dsp(dsp_reg::edl, edl);
    tracker->apuram->esa_val = esa;
    tracker->apuram->edl_val = edl;
  }
  return 0;
}

int SongSettingsPanel::dec_edl(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->song.settings.dec_edl();
  ie->update_edl();

  if (tracker->rendering())
  {
    auto edl = ::tracker->song.settings.edl;
    auto esa = calcESAfromEDL(edl);
    /* The order of the following two writes is CRUCIAL. Since the echo buffer
     * is mapped to the end of RAM. The ordering avoids rollover and changes
     * whether we are incrementing or decrementing the edl (growing or shrinking
     * the echo buffer and relocating its start address) */
    ::player->spc_write_dsp(dsp_reg::edl, edl);
    /* strange.. the SPC bugs out even with the expected correct order of dsp writes.
     * I'm commenting out the call (see below) for now. This means that the echo buffer will
     * always remain at the highest EDL setting (lowest address) you specified
     * (during song playback or the one you set before playback), which doesn't
     * pose any problem under normal circumstances (you haven't uploaded too many
     * samples, leaving no room for the desired echo buffer)
     * 
     * When I integrate the debugger into the tracker, I will be able to take a
     * better look at what might be causing this. */
    //::player->spc_write_dsp(dsp_reg::esa, esa);
    tracker->apuram->edl_val = edl;
    tracker->apuram->esa_val = esa;
  }
  return 0;
}

int SongSettingsPanel::inc_efb(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->song.settings.inc_efb();
  ie->update_efb();

  if (tracker->rendering())
  {
    auto efb = ::tracker->song.settings.efb;
    ::player->spc_write_dsp(dsp_reg::efb, efb);
    tracker->apuram->efb_val = efb;
  }
  return 0;
}

int SongSettingsPanel::dec_efb(void *i)
{
  SongSettingsPanel *ie = (SongSettingsPanel *)i;
  ::tracker->song.settings.dec_efb();
  ie->update_efb();

  if (tracker->rendering())
  {
    auto efb = ::tracker->song.settings.efb;
    ::player->spc_write_dsp(dsp_reg::efb, efb);
    tracker->apuram->efb_val = efb;
  }
  return 0;
}
