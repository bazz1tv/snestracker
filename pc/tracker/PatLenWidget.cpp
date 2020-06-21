#include "PatLenWidget.h"
#include "Tracker.h"
#include "DEBUGLOG.h"
extern Tracker *tracker;

PatLenWidget::PatLenWidget() :
	patlen_cbuf("$040"),
	patlen_title("Plen"),
	patlen_valtext(patlen_cbuf),
	patlen_incbtn("+", inc_patlen, this, true),
	patlen_decbtn("-", dec_patlen, this, true),

	seqlen_cbuf("$00"),
	seqlen_title("Slen"),
	seqlen_valtext(seqlen_cbuf)

{
}

/* these functions query the proper handles on that real data. */
void PatLenWidget :: update_patlen()
{
	uint8_t *len = &::tracker->song.patseq.patterns[
	    ::tracker->song.patseq.sequence[
			    ::tracker->main_window.patseqpanel.currow]].p.len;
	sprintf(patlen_cbuf, "$%03x", *len);
}

void PatLenWidget :: update_seqlen()
{
	sprintf(seqlen_cbuf, "$%02x", ::tracker->song.patseq.num_entries);
}

void PatLenWidget :: set_coords(int x, int y)
{
	rect.x = x;
	rect.y = y;
	/* (x,y)
	 * patlen  123 +-
	 * Spd   03 +-
	 * Add   02 +-*/

	patlen_title.rect.x = x;
	patlen_title.rect.y = y;
	patlen_valtext.rect.x = x + ((4 + 1) * CHAR_WIDTH);
	patlen_valtext.rect.y = y;
	patlen_incbtn.rect.x  = patlen_valtext.rect.x + (5 * CHAR_WIDTH);
	patlen_incbtn.rect.y = y;
	patlen_decbtn.rect.x = patlen_incbtn.rect.x + CHAR_WIDTH + 5;
	patlen_decbtn.rect.y = y;

	y += CHAR_HEIGHT + 2;

	seqlen_title.rect.x = x;
	seqlen_title.rect.y = y;
	seqlen_valtext.rect.x = x + ((4 + 1) * CHAR_WIDTH);
	seqlen_valtext.rect.y = y;

	rect.w = patlen_decbtn.rect.x + patlen_decbtn.rect.w - rect.x + 2;
	rect.h = seqlen_valtext.rect.y + seqlen_valtext.rect.h - rect.y + 2;
}

int PatLenWidget::handle_event(const SDL_Event &ev)
{
	patlen_incbtn.check_event(ev);
	patlen_decbtn.check_event(ev);
}

void PatLenWidget::draw(SDL_Surface *screen/*=::render->screen*/)
{
	update_patlen();
	patlen_title.draw(screen);
	patlen_valtext.draw(screen);
	patlen_incbtn.draw(screen);
	patlen_decbtn.draw(screen);

	update_seqlen();
	seqlen_title.draw(screen);
	seqlen_valtext.draw(screen);
}

int PatLenWidget::inc_patlen(void *bsaw)
{
	PatLenWidget *b = (PatLenWidget *)bsaw;
	//DEBUGLOG("inc_patlen; ");
	::tracker->inc_patlen();
	b->update_patlen();
}

int PatLenWidget::dec_patlen(void *bsaw)
{
	PatLenWidget *b = (PatLenWidget *)bsaw;
	::tracker->dec_patlen();
	b->update_patlen();
}
