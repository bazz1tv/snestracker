#include "Pattern.h"

const int PatternSequencer_Panel::NUM_ROWS;
//#define SAMPLE_NAME_GUI_CHAR_WIDTH 22

PatSeqPanel::PatSeqPanel() :
  loadbtn("Load", Sample_Panel::load, this),
  savebtn("Save", Sample_Panel::save, this),
  clearbtn("Clear", Sample_Panel::clear, this),
  instrpanel(ip)
{

}

PatSeqPanel::~PatSeqPanel()
{
}
