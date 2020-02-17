#include "Pattern.h"

const int PatternSequencer_Panel::NUM_ROWS;
//#define SAMPLE_NAME_GUI_CHAR_WIDTH 22

PatternSequencer_Panel::PatternSequencer_Panel() :
  loadbtn("Load", Sample_Panel::load, this),
  savebtn("Save", Sample_Panel::save, this),
  clearbtn("Clear", Sample_Panel::clear, this),
  instrpanel(ip)
{
  // 4 is for eg. "01|\0"
  sample_index_strings = (char *) malloc( (sizeof(char) * 4) * NUM_SAMPLES );
  if (sample_index_strings == NULL)
  {
    fprintf(stderr, "Could not allocate string index memory. "
        "This is not normal so we're quitting out");
    exit(1);
  }
}

Sample_Panel::~Sample_Panel()
{
  free(sample_index_strings);
}
