/* Thread pool: unified entry point that includes platform-specific implementation */

#ifdef _WIN32
#include "threadpool_windows.c"
#else
#include "threadpool_linux.c"
#endif
