#include <stdio.h>
#include <unistd.h>

static unsigned char g_cfg_filler = 0;
static unsigned char used2[256];

void applyBlockMask(char *filename);

int main(int argc, char **argv)
{
	char mask_file[2048];
	char *spc_file;
	char *t;
	
	if (argc<2) {
		printf("Usage: ./applymask32 spc_file [filler]\n");
		printf("\n");
		printf("If a .msk file is found in the same directory than the\n"
				".spc file, the mask will be applied on it. If no filler\n"
				"is specified (One byte, usually given in hex), unused\n"
				"areas will be filled with NULs\n");
		return 0;
	}

	spc_file = argv[1];

	strcpy(mask_file, spc_file);

	if (argc>=3) {
		g_cfg_filler = strtol(argv[2], NULL, 0);
	}
	
	t = strrchr(mask_file, '.');
	if (!t) {
		// no extension?
		fprintf(stderr, "Invalid filename\n");
		return 1;
	}

	sprintf(t, ".msk");
	printf("Spc file: %s\n", spc_file);
	printf("Mask file: %s\n", mask_file);

	if (access(spc_file, W_OK)) {
		perror("No write access to spc file");
		return 1;
	}

	if (access(mask_file, R_OK)) {
		perror("No read access from mask file");
		return 1;
	}
	
	if (load_mask(mask_file)==-1) {
		fprintf(stderr, "Mask load error\n");
		return 1;
	}

	printf("Applying pattern $%02X:\n", g_cfg_filler);
	applyBlockMask(spc_file);

	return 0;
}

int load_mask(char *filename)
{
	FILE *fptr;
	unsigned char packed_mask[32];
	int i;

	fptr = fopen(filename, "r");
	
	if (fread(packed_mask, 32, 1, fptr)==-1) {
		perror("fread");
		return -1;
	}

	fclose(fptr);

	for (i=0; i<256; i++) {
		used2[i] = packed_mask[i>>3] & (128>>(i%8));
	}
	
	printf("Mask: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
		packed_mask[0], packed_mask[1], packed_mask[2], packed_mask[3],
		packed_mask[4], packed_mask[5], packed_mask[6], packed_mask[7],
		packed_mask[8], packed_mask[9], packed_mask[10], packed_mask[11],
		packed_mask[12], packed_mask[13], packed_mask[14], packed_mask[15],
		packed_mask[16], packed_mask[17], packed_mask[18], packed_mask[19],
		packed_mask[20], packed_mask[21], packed_mask[22], packed_mask[23],
		packed_mask[24], packed_mask[25], packed_mask[26], packed_mask[27],
		packed_mask[28], packed_mask[29], packed_mask[30], packed_mask[31]);

	return 0;
}

void applyBlockMask(char *filename)
{
	FILE *fptr;
	unsigned char nul_arr[256];
	int i;

	memset(nul_arr, g_cfg_filler, 256);
	
	fptr = fopen(filename, "r+");
	if (!fptr) { perror("fopen"); }

	printf("[");
	for (i=0; i<256; i++)
	{
		fseek(fptr, 0x100+(i*256), SEEK_SET);
		
		if (!used2[i]) {
			printf(".");
			fwrite(nul_arr, 256, 1, fptr);
		} else {
			printf("o");
		}
		fflush(stdout);
	}
	printf("]\n");
	
	fclose(fptr);
}


