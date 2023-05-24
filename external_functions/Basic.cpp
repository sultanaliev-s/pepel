#include <cstdio>
#include <cstdlib>
#include <stdexcept>

extern "C" void exitWithError(const char *msg, const int index) {
    printf("%s%d\n", msg, index);
    exit(1);
}
