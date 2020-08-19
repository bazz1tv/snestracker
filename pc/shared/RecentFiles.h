#pragma once

#define NUM_RECENTFILES 8
namespace RecentFiles
{
  void init();
  void deinit();
  void push(const char *path);
  void remove(int idx);
  void createDisplayNames();

  extern char *paths[NUM_RECENTFILES];
  extern char *dnames[NUM_RECENTFILES];
};