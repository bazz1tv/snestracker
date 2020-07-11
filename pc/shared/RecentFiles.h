#pragma once

#define NUM_RECENTFILES 8
namespace RecentFiles
{
  void init();
  void deinit();
	void push(const char *path);

  extern char *paths[NUM_RECENTFILES];
};