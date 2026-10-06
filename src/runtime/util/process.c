#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void jocky_sleep_and_recheck(void) {
#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
}
