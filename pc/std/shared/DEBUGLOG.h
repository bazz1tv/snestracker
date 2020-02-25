#pragma once
#define DEBUGLOG(...) fprintf (stderr, __VA_ARGS__)
#define FILE_LINE_FUNC_STR " in file %s:%d (function %s)\n"
#define FILE_LINE_FUNC_ARGS __FILE__, __LINE__, __func__
