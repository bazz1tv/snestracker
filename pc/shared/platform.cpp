#include "platform.h"
#include "SDL.h"
#include "utf8.h"
#include "DEBUGLOG.h"

#if defined(PLATFORM_LINUX)
#include <spawn.h>
#include <sys/wait.h>
#elif defined(PLATFORM_WINDOWS)
#include <shlobj.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#endif

extern "C"
{
  extern char **environ; // The environment, always available
}

int openUrl(const char *url)
{
  if (url == NULL)
    return -1;

#if defined(PLATFORM_MACOSX)

  int urlLen = strlen(url);
  int finalLen = sizeof("open ") + urlLen; // sizeof() adds null byte
  char *openUrlStr = (char *) malloc(finalLen * sizeof(char));
  strcpy(openUrlStr, "open ");
  strcat(openUrlStr, url);
  DEBUGLOG("openUrlStr: \"%s\"\n", openUrlStr);

  system(openUrlStr);

  free(openUrlStr);

  return 0;

#elif defined(PLATFORM_LINUX)

  pid_t pid;
  const char *argv[] = {"xdg-open", url, nullptr};

  // Note: at the moment this process inherits our file descriptors.
  // Note: the below const_cast is really ugly as well.
  if (posix_spawnp(&pid, "xdg-open", nullptr, nullptr, const_cast<char **>(argv), environ) != 0)
    return -1;

  // Check if xdg-open already completed (or failed.)
  int status = 0;
  if (waitpid(pid, &status, WNOHANG) > 0)
    return (status >= 0) ? 0 : -1;
  else
    // We can't tell what actually happens without waiting for
    // the process to finish, which could take forever (literally).
    return 0;

#elif defined(PLATFORM_WINDOWS)

  // Unicode-aware WinAPI functions don't accept UTF-8, so we need to convert.
  std::wstring wurl = love::to_widestr(url);

  HINSTANCE result = 0;

  result = ShellExecuteW(nullptr,
    L"open",
    wurl.c_str(),
    nullptr,
    nullptr,
    SW_SHOW);

  int retVal = (int) result;
  if (retVal < 32)
  {
    DEBUGLOG("Failed to open URL \"%s\", result code %d\n", url, retVal);
    retVal = -1;
  }
  else retVal = 0;

  return retVal;
#endif
}
