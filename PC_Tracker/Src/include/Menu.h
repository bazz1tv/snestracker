#include "Globals.h"

struct MenuItem
{
  std::string label;
  double x,y;
  SDL_Rect rect;
  //DWORD dwID;
};
 
   
class CMenu;
 
class CMenuItem
{
  MenuItem m_MenuItemProps;
 
  //Possible pointers to previous and next menus
  //Enables menu items to lead to pop-ups and/or can
  //implement a PREV/NEXT setup
  CMenu *m_pNextMenu;
  CMenu *m_pPrevMenu;
   
  //Selection ID of menu item
  //Returned when this menu item has been selected
 
  SDL_bool HasFocus;
   
  public:
    //...
    //...
    SDL_bool Create(double x,double y,std::string lpszText,SDL_bool bHasFocus=SDL_FALSE);
 
    void SetText(std::string lpszText);
    //...
};
 
class CMenu
{
  std::vector<CMenuItem *> m_vMenuItems;
   
  /*....
   ....*/
  public:
   /*...
   ...*/
   SDL_bool AddItem(CMenuItem *pItem);
   SDL_bool AddItem(MenuItem item);
   /*...
   ...*/
};