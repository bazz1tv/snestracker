#include "Brr.h"
#include "utility.h"

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
	SDL_RWops *file;
	nfdchar_t *outPath=NULL;

	if (Utility::get_file_write_handle(&outPath, &file, BRR_FILE_EXTENSION) == NFD_OKAY)
	{
		if (outPath != NULL)
			fprintf(stderr, "outPath: %s\n", outPath);
		/* TODO: should probaby have code that checks return and recalls
		 * appropriately .. ? */
		SDL_RWwrite(file, brr, brrsize, 1);
		SDL_RWclose(file);
		free(outPath);
	}
	return 0;
}

