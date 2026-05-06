#include "popcorn.h"

inline int wstrcmp(const CHAR16* a, const CHAR16* b) {
    while (*a && *b) {
        if (*a != *b) return (*a - *b);
        a++; b++;
    }
    return (*a - *b);
}

int pop_API pop_main(pop_Services* svc, int argc, CHAR16** argv) {
    svc->reset(svc, argc > 1 && wstrcmp(argv[0], L"--quick") == 0);
    return 0; // this will never get executed, but it's good etiquette.
}