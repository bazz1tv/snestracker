/*
This header was established in order to programmatically determine the max buf size needed to operate upon
file extensions used in SNES Tracker
*/

#include "Song.h"
#include "Instruments.h"
//#include "Samples.h" manually including BRR for now!

/* Whenever a new file extension is added to this project, make sure to add it to this definition below */
#define max(a,b) (a > b ? a : b)
// The max file extension buffer size used by any file extension associated with SNES Tracker (brr, sts/stp, sti)
#define MAX_ST_EXT_BUFSIZE ( max ( sizeof("brr"), max( sizeof(INSTRFILE_EXT), sizeof(SONGFILE_EXT) ) ) )