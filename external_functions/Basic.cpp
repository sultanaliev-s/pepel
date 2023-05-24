#include <cstdio>
#include <cstdlib>
#include <stdexcept>

extern "C" void exitWithError(const char* msg, const int index) {
    printf("%s%d\n", msg, index);
    exit(1);
}

extern "C" int ScanInt__() {
    int x;
    scanf("%d", &x);
    return x;
}

extern "C" float ScanFloat__() {
    float x;
    scanf("%f", &x);
    return x;
}
