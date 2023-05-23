#include <cstdio>
#include <cstdlib>
#include <stdexcept>

extern "C" void exitWithError(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}
