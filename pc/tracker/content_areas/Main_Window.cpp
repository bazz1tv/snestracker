#include "Main_Window.h"
#include <getopt.h>
#include "utility.h"
#include "Render.h"
#include "Menu_Bar.h"
#include "Screen.h"
#include "Tracker.h"

#define L_FLAG 0
#define R_FLAG 1


/*

                     _.-*'"      "`*-._
                _.-*'                  `*-._
             .-'                            `-.
  /`-.    .-'                  _.              `-.
 :    `..'                  .-'_ .                `.
 |    .'                 .-'_.' \ .                 \
 |   /                 .' .*     ;               .-'"
 :   L                    `.     | ;          .-'
  \.' `*.          .-*"*-.  `.   ; |        .'
  /      \        '       `.  `-'  ;      .'
 : .'"`.  .       .-*'`*-.  \     .      (_
 |              .'        \  .             `*-.
 |.     .      /           ;                   `-.
 :    db      '       d$b  |                      `-.
 .   :PT;.   '       :P"T; :                         `.
 :   :bd;   '        :b_d; :                           \
 |   :$$; `'         :$$$; |                            \
 |    TP              T$P  '                             ;
 :                        /.-*'"`.                       |
.sdP^T$bs.               /'       \
$$$._.$$$$b.--._      _.'   .--.   ;
`*$$$$$$P*'     `*--*'     '  / \  :
   \                        .'   ; ;   [bug]
    `.                  _.-'    ' /
      `*-.                      .'
          `*-._            _.-*'
               `*=--..--=*'


Sonic invaded the SNES Tracker Code!!
Shoutz to BigMerl, Ravancloak, and Rowan from Twitch stream
9/28/2020 <3 Much Love <3

*/

Main_Window::Main_Window(int &argc, char **argv, Tracker *tracker) :
  song_label("Song"),
  song_title_label("Title:"),
  song_title(SongSettings::SONGTITLE_SIZE, tracker->song.settings.song_title_str, sizeof(tracker->song.settings.song_title_str)),
  tracker(tracker),
	samplepanel(tracker->song.samples),
  instrpanel(tracker->song.instruments, &samplepanel),
  patseqpanel(&tracker->song.patseq),
  pateditpanel(&patseqpanel, &instrpanel),
  bsawidget(tracker, &pateditpanel),
  instreditor(&instrpanel),
  instreditor_btn("Instrument Editor", toggle_instreditor, this),
	sample_editor_btn("Sample Editor", toggle_sample_editor, this),
  songsettings_btn("Song Settings", toggle_songsettings, this)
{
  int x,y,xx,yy;
  /* Address EASY Layout improvement from issue #142
   * Applies to area, Instrument Panel, and Sample Panel */

  // BG RECT internal padding
  // TODO: rename to eg. INT_PAD_X
  constexpr int PAD_X = 10;
  constexpr int PAD_Y = 10;
  // EXTERNAL ELEMENT PADDING
  constexpr int EXT_PAD_X = 12;

  song_title.dblclick = false; // do not require dblclick to edit. single

  if (::render->screen == NULL)
  {
    fprintf(stderr, "Debugger::MainWindows::::render->screen is Null\n");
    exit(1);
  }
  
  x = xx = EXT_PAD_X + PAD_X; //(SCREEN_WIDTH / 2) - ((strlen("Song Title") * CHAR_WIDTH) / 2 );
  y = yy = 50;

  patseqpanel.set_coords(x, y);
  // Pattern Sequencer BG Rect
  patseq_rect_bg.x = patseqpanel.rect.x - PAD_X;
  patseq_rect_bg.y = patseqpanel.rect.y - PAD_Y;
  patseq_rect_bg.w = patseqpanel.rect.w + (PAD_X * 2);
  patseq_rect_bg.h = patseqpanel.rect.h + (PAD_Y * 2); // added vertical padding for instrment editor button

  // Update x coordinate for the Song panel
	x = patseq_rect_bg.x + patseq_rect_bg.w + PAD_X + EXT_PAD_X;

  song_label.rect.x = x;
  song_label.rect.y = y;

  y += (CHAR_HEIGHT * 2); // + 5;
  song_title_label.rect.x = x;
  song_title_label.rect.y = y;
  
  y += CHAR_HEIGHT + 5;
  song_title.rect.x = x;
  song_title.rect.y = y;

  bsawidget.set_coords(x, y + song_title.rect.h + (CHAR_HEIGHT + (CHAR_HEIGHT / 2) + (CHAR_HEIGHT / 4)));

///
// WHAT WE WANT:
// To get the Song Panel BG Rect defined here so that we can position the
// next element accordingly
  /* This all stems from how the Song Settings button's Y coordinate is
   * depending on the Instrument panel's Instrument editor button to be
   * positioned first */

// !HACK! Call the instrpanel.set_coords() with the proper Y value to get
// the proper instreditor_btn Y position, so that we can establish the
// Song Panel fully!!! \o/ Then, we actually positioning the instrument
// panel FOR REAL. Call it again, with the proper X coordinate in place.
  instrpanel.set_coords(x, yy); // x = dont care
  // 
  instreditor_btn.rect.y = instrpanel.rect.y + instrpanel.rect.h + (CHAR_HEIGHT * 1);

  
// Align song settings button vertically with the instrument editor and
// sample editor buttons
  songsettings_btn.rect.x = bsawidget.rect.x;
  songsettings_btn.rect.y = instreditor_btn.rect.y; // OMG. WE FINALLY GOT THE PROPER Y COORDINATE \o/

  // Song Area BG Rect Portion
  //
  song_rect_bg.x = song_label.rect.x - PAD_X;
  song_rect_bg.y = song_label.rect.y - PAD_Y;
  song_rect_bg.w = song_title.rect.w + (PAD_X * 2);
  song_rect_bg.h = ( (songsettings_btn.rect.y + songsettings_btn.rect.h) - song_rect_bg.y ) + (PAD_Y * 1);

/////////// COORDINATES FOR INSTRUMENT PANEL

  x = song_rect_bg.x + song_rect_bg.w + PAD_X + EXT_PAD_X;

  instrpanel.set_coords(x, yy);
  // #142, move instrument editor button to be with instrument panel
  instreditor_btn.rect.x = instrpanel.loadbtn.rect.x; // Use same indentation as Load Button
  instreditor_btn.rect.y = instrpanel.rect.y + instrpanel.rect.h + (CHAR_HEIGHT * 1);

  // Enlargen rect area for the background color area.
  instr_rect_bg.x = instrpanel.rect.x - PAD_X;
  instr_rect_bg.y = instrpanel.title.rect.y - PAD_Y;
  instr_rect_bg.w = instrpanel.rect.w + (PAD_X * 2);
  instr_rect_bg.h = ( ( instreditor_btn.rect.y + instreditor_btn.rect.h ) - instr_rect_bg.y) + PAD_Y;
  // (in Main_Window)

/////////// COORDINATES FOR SAMPLE PANEL
  x = instr_rect_bg.x + instr_rect_bg.w + PAD_X + EXT_PAD_X;
  samplepanel.set_coords(x, yy);
  // sample editor button directly beneath sample panel
  sample_editor_btn.rect.x = samplepanel.loadbtn.rect.x;
  sample_editor_btn.rect.y = samplepanel.rect.y + samplepanel.rect.h + (CHAR_HEIGHT);

  // Enlargen rect area for the background color area.
  sample_rect_bg.x = samplepanel.rect.x - PAD_X;
  sample_rect_bg.y = samplepanel.title.rect.y - PAD_Y;
  sample_rect_bg.w = samplepanel.rect.w + (PAD_X * 2);
  sample_rect_bg.h = ( ( sample_editor_btn.rect.y + sample_editor_btn.rect.h ) - sample_rect_bg.y ) + PAD_Y;

/////// COORDINATES FOR PATTERN EDITOR PANEL

  y += instrpanel.rect.h + (CHAR_HEIGHT / 2);
  pateditpanel.set_coords(xx, y);
  pateditpanel.set_visible_rows(0x08); // called to update rect.h
  y = pateditpanel.rect.y + pateditpanel.rect.h + (CHAR_HEIGHT*2);
  pateditpanel.set_visible_rows(PatternEditorPanel::MAX_VISIBLE_ROWS); // called to update rect.h

  // Enlargen rect area for the background color area.
  patedit_rect_bg.x = pateditpanel.rect.x - PAD_X;
  patedit_rect_bg.y = pateditpanel.rect.y - PAD_Y;
  patedit_rect_bg.w = pateditpanel.rect.w + (PAD_X * 2);
  patedit_rect_bg.h = ( ( pateditpanel.rect.y + pateditpanel.rect.h ) - patedit_rect_bg.y ) + PAD_Y;

  // These panels appear at the same coords when activated
	instreditor.set_coords(xx, y);
	sample_editor.set_coords(xx, y);
  songsettings_panel.set_coords(xx, y);
}

/*static inline void reset_byte_bit(uint8_t *byte, uint8_t bit)
{
  *byte &= ~(1 << bit);
}*/

static void deactivate_instreditor(Main_Window *m)
{
  // reset bit
	m->active_aux_panel &= ~(1 << Main_Window::INSTREDITOR);
  Tracker::prerenders.erase((DrawRenderer *)&m->instreditor.adsrpanel);
}

static void deactivate_sample_editor(Main_Window *m)
{
  // reset bit
  m->active_aux_panel &= ~(1 << Main_Window::SAMPLEDITOR);
}

static void deactivate_songsettings(Main_Window *m)
{
  // reset bit
  m->active_aux_panel &= ~(1 << Main_Window::SONGSETTINGS);
}

// consider adding to Utility class (utility.h)
static bool toggle_byte_bit(uint8_t &byte, uint8_t bit)
{
  byte ^= (1 << bit);
  return byte & (1 << bit);
}

/*static void deactivate_all_aux_panels(Main_Window *mw)
{
  deactivate_sample_editor(mw);
  deactivate_instreditor(mw);
  deactivate_songsettings(mw);
}*/

static void deactivate_all_other_aux_panels(Main_Window *mw, uint8_t active)
{
  if (active != Main_Window::INSTREDITOR)
    deactivate_instreditor(mw);
  if (active != Main_Window::SAMPLEDITOR)
    deactivate_sample_editor(mw);
  if (active != Main_Window::SONGSETTINGS)
    deactivate_songsettings(mw);
}

int Main_Window::toggle_instreditor(void *m)
{
  Main_Window *mw = (Main_Window *)m;
  bool active = toggle_byte_bit(mw->active_aux_panel, INSTREDITOR);

  deactivate_all_other_aux_panels(mw, INSTREDITOR);

  SDL_Rect r = mw->pateditpanel.fullsize_r;
  r.x +=1;
  r.w -=1;
  r.h -=1;
  SDL_FillRect(::render->screen, &r, Colors::transparent);

  if ( active )
  {
    mw->pateditpanel.set_visible_rows(0x08);
    if (mw->instreditor.tabs.adsr.active)
      Tracker::prerenders.insert((DrawRenderer *)&mw->instreditor.adsrpanel);
  }
  else
  {
    mw->pateditpanel.set_visible_rows(mw->pateditpanel.MAX_VISIBLE_ROWS);
    if (mw->instreditor.tabs.adsr.active)
      Tracker::prerenders.erase((DrawRenderer *)&mw->instreditor.adsrpanel);
  }
  return 0;
}

/* By and large a copy of toggle_instreditor. Maybe we can reduce code
 * size by putting the common into one function. TODO*/
int Main_Window::toggle_sample_editor(void *m)
{
	Main_Window *mw = (Main_Window *)m;
  bool active = toggle_byte_bit(mw->active_aux_panel, SAMPLEDITOR);

  deactivate_all_other_aux_panels(mw, SAMPLEDITOR);

	SDL_Rect r = mw->pateditpanel.fullsize_r;
	r.x +=1;
	r.w -=1;
	r.h -=1;
	SDL_FillRect(::render->screen, &r, Colors::transparent);

	if ( active )
		mw->pateditpanel.set_visible_rows(0x08);
	else
		mw->pateditpanel.set_visible_rows(mw->pateditpanel.MAX_VISIBLE_ROWS);
  return 0;
}

/* By and large a copy of toggle_instreditor. Maybe we can reduce code
 * size by putting the common into one function. TODO*/
int Main_Window::toggle_songsettings(void *m)
{
  Main_Window *mw = (Main_Window *)m;
  bool active = toggle_byte_bit(mw->active_aux_panel, SONGSETTINGS);

  deactivate_all_other_aux_panels(mw, SONGSETTINGS);

  SDL_Rect r = mw->pateditpanel.fullsize_r;
  r.x +=1;
  r.w -=1;
  r.h -=1;
  SDL_FillRect(::render->screen, &r, Colors::transparent);

  if ( active )
    mw->pateditpanel.set_visible_rows(0x08);
  else
    mw->pateditpanel.set_visible_rows(mw->pateditpanel.MAX_VISIBLE_ROWS);
  return 0;
}

int Main_Window::Gain::change(void *dblnewgain)
{
  ::player->gain_has_changed = true;
  return 0;
}

void Main_Window::draw_memory_outline()
{
  tmprect.x = MEMORY_VIEW_X-1;
  tmprect.y = MEMORY_VIEW_Y-1;
  tmprect.w = 512+2;
  tmprect.h = 512+2;
  SDL_SetRenderDrawColor(::render->sdlRenderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(::render->sdlRenderer, &tmprect);
}

void Main_Window::one_time_draw()
{
  // Draw Pattern Sequencer BG Rect
  SDL_FillRect(::render->screen, &patseq_rect_bg, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);


  // draw Song BG Rect
  SDL_FillRect(::render->screen, &song_rect_bg, Colors::Interface::color[Colors::Interface::Type::songpanelBG]);
  // "SONG"
  song_label.draw(::render->screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
      true, false, false, Colors::Interface::color[Colors::Interface::Type::songpanelBG]);
  // "Title:"
  song_title_label.draw(::render->screen, Colors::Interface::color[Colors::Interface::Type::text_fg],
               true, false, false, Colors::Interface::color[Colors::Interface::Type::songpanelBG]);

  // Instrument Panel
  // Draw the background rect
  SDL_FillRect(::render->screen, &instr_rect_bg, Colors::Interface::color[Colors::Interface::Type::instrpanelBG]);
  instrpanel.one_time_draw();
  // Sample Panel
  // Draw the background rect
  SDL_FillRect(::render->screen, &sample_rect_bg, Colors::Interface::color[Colors::Interface::Type::samplepanelBG]);
  samplepanel.one_time_draw();

  patseqpanel.one_time_draw();

  // Pattern Editor Panel
  SDL_FillRect(::render->screen, &patedit_rect_bg, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);
  pateditpanel.one_time_draw();

  ////// Draw a Rect to bridge the Patseq_rect_bg with the patedit_rect_bg
  /*SDL_Rect bridge;
  bridge.x = patseq_rect_bg.x;
  bridge.y = patseq_rect_bg.y + patseq_rect_bg.h; // start where patseq rect ends, vertically
  bridge.w = patseq_rect_bg.w;                    // same width at patseq rect
  bridge.h = patedit_rect_bg.y - bridge.y + 1;
  SDL_FillRect(::render->screen, &bridge, Colors::Interface::color[Colors::Interface::Type::patseqpanelBG]);*/
}

void Main_Window::draw()
{
  time_cur = SDL_GetTicks();
  one_time_draw(); // Draw these regularly so they don't disappear when refreshing shaders
  //sdlfont_drawString(::render->screen, MEMORY_VIEW_X, MEMORY_VIEW_Y-10, "spc memory:");

  // The following are correlated from i and tmp. DO NOT MESS WITH THAT
  // base height
  i = 32 + SCREEN_Y_OFFSET;  
  //fprintf(stderr, "HERE!\n");
  song_title.draw();
	bsawidget.draw(::render->screen, Colors::Interface::color[Colors::Interface::Type::songpanelBG]);
  songsettings_btn.draw(::render->screen);

  // BEWARE: Must draw instreditor_btn after the instrpanel
  // TODO (Maybe): Put instreditor_btn into Instrument_Panel class with
  // handle to Main_Window
  instrpanel.draw(::render->screen);
  instreditor_btn.draw(::render->screen);

  // BEWARE: Must draw sample_editor_btn after the samplepanel
  // TODO (Maybe): Put sample_editor_btn into Sample_Panel class with
  // handle to Main_Window
  samplepanel.draw(::render->screen);
  sample_editor_btn.draw(::render->screen);

  patseqpanel.draw(::render->screen);
  pateditpanel.draw(::render->screen);
  if (active_aux_panel & (1 << INSTREDITOR))
    instreditor.draw(::render->screen);
	else if (active_aux_panel & (1 << SAMPLEDITOR) )
		sample_editor.draw(::render->screen);
  else if (active_aux_panel & (1 << SONGSETTINGS) )
    songsettings_panel.draw(::render->screen);

  time_last = time_cur;
  
  is_first_run = false;
}

void Main_Window::check_quit(SDL_Event &ev)
{
  switch (ev.type)
  {
    case SDL_QUIT:
    quitting = true;
    break;

    case SDL_KEYDOWN:
    if (ev.key.keysym.sym == SDLK_ESCAPE)
    {
      //quitting = true;
    }
  }
}

int Main_Window::receive_event(SDL_Event &ev)
{
  check_quit(ev);

  if (handle_text_edit_rect_event(ev, &song_title) == 2)
    tracker->song.changed = true;
  instreditor_btn.check_event(ev);
	sample_editor_btn.check_event(ev);
  songsettings_btn.check_event(ev);
  bsawidget.handle_event(ev);
  instrpanel.event_handler(ev);
  samplepanel.event_handler(ev);
  patseqpanel.event_handler(ev);
  pateditpanel.event_handler(ev);
  if (active_aux_panel & (1 << INSTREDITOR))
    instreditor.handle_event(ev);
  else if (active_aux_panel & (1 << SAMPLEDITOR) )
		sample_editor.handle_event(ev);
  else if (active_aux_panel & (1 << SONGSETTINGS) )
    songsettings_panel.handle_event(ev);
  // DIRTY :( ITS IMPORTANT THAT WE CHECK THE DBLCLICK EVENTS AFTER THE ABOVE
  dblclick::check_event(&ev);
  /*if (gain.slider)
  {
    bool a=gain.slider->receive_event(ev);
    if (a) return;
  }*/
  
  switch (ev.type)
  {
  case SDL_QUIT:
    quitting = true;
    break;
  case SDL_MOUSEMOTION:
    {
      mouse::x = ev.motion.x; mouse::y = ev.motion.y;

      if (ev.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        //::voice_control.checkmouse_mute((Uint16&)ev.motion.x, (Uint16&)ev.motion.y);
      }
      if (mode == MODE_NAV || mode == MODE_EDIT_MOUSE_HEXDUMP)
      {
        if (  ev.motion.x >= MEMORY_VIEW_X && 
            ev.motion.x < MEMORY_VIEW_X + 512 &&
            ev.motion.y >= MEMORY_VIEW_Y &&
            ev.motion.y < MEMORY_VIEW_Y + 512)
        {
        }
      }
    
    }
    break;
  case SDL_KEYDOWN:
    {
      int scancode = ev.key.keysym.sym;
      int mod = ev.key.keysym.mod;

      switch(scancode)
      {
        case SDLK_s:
        break;
        case SDLK_UP:
          break;
        case SDLK_DOWN:
        break;
        default:break;
      }

      if (mode == MODE_NAV)
      {
        switch (scancode)
        {
          case SDLK_d:
          case SDLK_SLASH:
          break;

          case SDLK_TAB:
            if (mod & KMOD_SHIFT)
            {
            }
            else 
            {
            }
          break;
        }
      }
             
    } break;
    case SDL_USEREVENT:
    {
      if (ev.user.code == UserEvents::mouse_react)
      {
        SDL_Event *te = (SDL_Event *)ev.user.data1; // the mouse coordinates at time of double click
      }
    } break;
    case SDL_MOUSEWHEEL:
    {
    } break;
    case SDL_MOUSEBUTTONDOWN:           
    {
 
    } break;
  default:
    break;
  }
  return 0;
}

void Main_Window::run()
{
}

void Main_Window::draw_voices_pitchs()
{
  tmp = i+10; // y 
  sdlfont_drawString2(::render->screen, MEMORY_VIEW_X+520, tmp, "Voices pitches:"); // Colors::white);
  tmp += 9;
  
  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=::render->screen->w-tmprect.x;
  tmprect.h=8*8;
  SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::bg]);
  tmprect.w=5;
  tmprect.h = 5;
  for (i=0; i<8; i++)
  {
    
    unsigned short pitch = (player->spc_read_dsp(2+(i*0x10)) | (player->spc_read_dsp(3+(i*0x10))<<8)) & 0x3fff; 
    // I believe pitch is max 0x3fff but kirby is using higher values for some unknown reason...
    //Uint32 gray = Colors::gray;
    Uint32 *cur_color= &Colors::gray;

    uint8_t voice_base_addr = (i*0x10);
    uint8_t outx = player->spc_read_dsp(voice_base_addr+0x09);
    uint8_t envx = player->spc_read_dsp(voice_base_addr+0x08);

    if (outx || envx) 
    {
      cur_color = &Colors::voice[i];
    }


    if (::voice_control.is_muted(i))
    {
      /* I would use sub color here to
      get a view of the activity but the mute happens at the DSP level
      it invokes KOFF on the muted channel making it impossible to get its
      real status without hacking it at deep level */
      /* i will leave out the pointers in the color logic still ... altho
      the pointers are faster and can be report::used if the pointed-to-value is
      not changed
      :P pointers are back */
      cur_color = &Colors::nearblack;
    }

    int x =MEMORY_VIEW_X+520;
    int y = tmp+ (i*8);
    sprintf(tmpbuf,"%d:",i);
    sdlfont_drawString(::render->screen, x, y, tmpbuf, *cur_color);
    if (is_first_run && i == 0)
    {
      Screen::voice0pitch.x = x;
      Screen::voice0pitch.y = y;
    }
    
    tmprect.y= tmp+(i*8)+2;
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.x += pitch*(::render->screen->w-tmprect.x-20)/((0x10000)>>2);
    if (::voice_control.is_muted(i))
      SDL_FillRect(::render->screen, &tmprect, Colors::nearblack);
    else
      SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::text_fg]);
    
  }
}

void Main_Window::draw_voices_volumes()
{
  tmp += 9*8;

  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp, "Voices volumes:");
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(16*8), tmp, "Left", Colors::Interface::color[Colors::Interface::Type::voice_volume_left]);      

  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(20*8)+4, tmp, "Right", Colors::Interface::color[Colors::Interface::Type::voice_volume_right]);
  sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520+(26*8), tmp, "Gain", Colors::Interface::color[Colors::Interface::Type::voice_gain]);
  tmp += 9;

  tmprect.x=MEMORY_VIEW_X+520;
  tmprect.y=tmp;
  tmprect.w=::render->screen->w-tmprect.x;
  tmprect.h=10*11;
  SDL_FillRect(::render->screen, &tmprect, Colors::Interface::color[Colors::Interface::Type::bg]);   
  tmprect.w=2;
  tmprect.h=2;

  for (i=0; i<8; i++)
  {
    Uint32 c1 = Colors::white, c2 = Colors::gray, c3 = Colors::magenta;
    Uint32 *Color1=&c1, *Color2 = &c1;
    
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(0+(i*0x10));
    unsigned char right_vol = player->spc_read_dsp(1+(i*0x10));
    unsigned char adsr1 = player->spc_read_dsp(5+(i*0x10));
    unsigned char gain;
    if (adsr1 & 0x80) gain=0;
    else gain = player->spc_read_dsp(7+(i*0x10));

    
    
    //Value -128 can be safely report::used
    // for lefty vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
      Color1 = &c2;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
      Color2 = &c2;
    }
    sprintf(tmpbuf,"%d", i);
    int x = MEMORY_VIEW_X+520;
    int y = tmp + (i*10);
    Uint32 *color;

    if (::voice_control.is_muted(i))
      color = &Colors::nearblack;
    else 
      color = &Colors::voice[i];

    sdlfont_drawString(::render->screen, x, y, tmpbuf, *color);
    if (is_first_run && i == 0)
    {
      Screen::voice0vol.x = x;
      Screen::voice0vol.y = y;  
    }
    
    sprintf(tmpbuf,"\x1");
    if (::voice_control.is_muted(i))
    {
      Colors::subtractp(Color1,0x60);
      Colors::subtractp(Color2,0x60);
    }
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520 + 8, tmp + (i*10), tmpbuf, *Color1, *Color2);
    

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10);
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    // L volume
    if (::voice_control.is_muted(i))
      color = &Colors::dark_yellow;
    else color = &Colors::yellow;
    SDL_FillRect(::render->screen, &tmprect, *color);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+3;
    
    if (::voice_control.is_muted(i))
      color = &Colors::dark_cyan;
    else color = &Colors::cyan;
    SDL_FillRect(::render->screen, &tmprect, *color);

    // Gain needs customization
    if (!(gain & 0x80))
    {
      // direct mode
      gain = gain & 0x7f;
    }
    else
    {
      gain = gain & 0x1f;
    }


    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = gain * (::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+6;
    if (::voice_control.is_muted(i))
    {
      Uint8 r1,g1,b1;
      SDL_GetRGB(c3, ::render->screen->format, &r1, &b1, &g1);
      c3 = SDL_MapRGB(::render->screen->format,r1-0x60,g1-0x60,b1-0x60);
    }

    SDL_FillRect(::render->screen, &tmprect, c3);
  }
}

void Main_Window::draw_main_volume()
{
  i=9;
  // 
  {
    Uint32 *Color1=&Colors::white, *Color2 = &Colors::white;
    Uint32 *thecolor = &Colors::gray;
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::mvol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::mvol_r);
    //unsigned char gain = player->spc_read_dsp(7+(i*0x10));

    //Value -128 canNOT be safely report::used
    // for left vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
    }

    if (is_inverted & (1 << L_FLAG) )
    {
      if (left_vol == 0x80)
        Color1 = &Colors::red; // this is bad value according to FullSNES
      else Color1 = thecolor;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      if (right_vol == 0x80)
        Color2 = &Colors::red; // this is bad value according to FullSNES
      Color2 = thecolor;
    }

    sprintf(tmpbuf,"M");
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, *Color1, *Color2);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    SDL_FillRect(::render->screen, &tmprect, Colors::yellow);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(::render->screen, &tmprect, Colors::cyan);
  }
}

void Main_Window::draw_echo_volume()
{
  i++;
  {
    Uint32 Color1=Colors::white;
    Uint32 Color2=Colors::white;// = &Colors::white;
    Uint32 *leftbarvol_col=&Colors::yellow;
    Uint32 *rightbarvol_col=&Colors::cyan;
    //Uint32 thecolor = &
    unsigned char is_inverted=0;
    unsigned char left_vol = player->spc_read_dsp(dsp_reg::evol_l);
    unsigned char right_vol = player->spc_read_dsp(dsp_reg::evol_r);
    
    //Value -128 can be safely report::used
    // for left vol
    if (left_vol >= 0x80)
    {
      left_vol = (left_vol ^ 0xff) + 1;
      is_inverted = 1 << L_FLAG;
    }
    // for right vol
    if (right_vol >= 0x80)
    {
      right_vol = (right_vol ^ 0xff) + 1;
      is_inverted |= 1 << R_FLAG;
    }

    if (is_inverted & (1 << L_FLAG) )
    {
      Color1 = Colors::gray;
    }
    if (is_inverted & (1 << R_FLAG) )
    {
      Color2 = Colors::gray;
    }

    //
    Uint32 *c;
    sprintf(tmpbuf,"E");
    if (is_first_run)
    {
      Screen::echoE.y = tmp+(i*10);
    }

    if (player->spc_emu()->is_echoing())
    {
      c = &Colors::white;
    }
    else
    {
      c = &Colors::nearblack;
      Colors::subtractp(&Color1, 0x60);
      Colors::subtractp(&Color2, 0x60);
      leftbarvol_col=&Colors::dark_yellow;
      rightbarvol_col=&Colors::dark_cyan;
    }
    sdlfont_drawString(::render->screen, MEMORY_VIEW_X+520, tmp + (i*10), tmpbuf, *c);
    sprintf(tmpbuf,"\x1");
    sdlfont_drawString2c(::render->screen, MEMORY_VIEW_X+520+8, tmp + (i*10), tmpbuf, Color1, Color2);
    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.y = tmp+(i*10)+1;
    tmprect.w = left_vol*(::render->screen->w-tmprect.x-20)/255;

    SDL_FillRect(::render->screen, &tmprect, *leftbarvol_col);

    tmprect.x = MEMORY_VIEW_X+520+18;
    tmprect.w = right_vol*(::render->screen->w-tmprect.x-20)/255;
    tmprect.y = tmp+(i*10)+4;
    
    SDL_FillRect(::render->screen, &tmprect, *rightbarvol_col);
  }
}

#undef L_FLAG
#undef R_FLAG
