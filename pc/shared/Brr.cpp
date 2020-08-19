#include "Brr.h"
#include "SdlNfd.h"

const BrrFilter brrfilter[4] =
{
  {0,         0},
  {0.9375,    0},
  {1.90625,  -0.9375},
  {1.796875, -0.8125 },
};

uint16_t get_brr_size(const Brr *brr)
{
	uint16_t numbytes = 9; // bytes  to account for end block
	while( ! ((brr++)->end) )
	{
		numbytes += 9;
	}
	return numbytes;
}

int write_brr_to_file(const Brr *brr)
{
	uint16_t brrsize = get_brr_size(brr);

	if (SdlNfd::get_file_handle("w", BRR_FILE_EXTENSION) == NFD_OKAY)
	{
		if (SdlNfd::outPath != NULL)
			fprintf(stderr, "outPath: %s\n", SdlNfd::outPath);
		/* TODO: should probaby have code that checks return and recalls
		 * appropriately .. ? */
		SDL_RWwrite(SdlNfd::file, brr, brrsize, 1);
	}
	return 0;
}

/* This expects a SANE Brr reference */
Brr * getBrrEndBlock(Brr *brr)
{
	while ( ! ((brr++)->end) );
	return --brr;
}

bool doesBrrLoop(Brr *brr)
{
	Brr *endblock = getBrrEndBlock(brr);
	return endblock->loop;
}
