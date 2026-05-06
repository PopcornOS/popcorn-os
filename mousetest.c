#include "popcorn.h"

int pop_API pop_main(pop_Services* svc, int argc, CHAR16** argv) {
    popg_GraphicsServices* sgfx = svc->sgfx;
    int code;
    svc->println(svc, L"initing graphics");
    if ((code = sgfx->init(sgfx)) != pop_SUCCESS) {
        svc->perrno(svc, code);
        return 1;
    }
    
    svc->println(svc, L"initing mouse");
    popm_MouseServices* msvc = svc->msvc;
    if ((code = msvc->init(msvc)) != pop_SUCCESS) {
        svc->perrno(svc, code);
        return 1;
    }
    
    unsigned int size = 20;

    while (TRUE) {
        // Wait for mouse
        msvc->query(msvc);
    
        // Clear screen to black
        for (unsigned int i = 0; i < sgfx->w; i++) {
            for (unsigned int j = 0; j < sgfx->h; j++) {
                sgfx->putpixel(sgfx, i, j, 0, 0, 0);
            }
        }

        // Draw the square in current color
        for (unsigned int i = 0; i < size; i++) {
            for (unsigned int j = 0; j < size; j++) {
                sgfx->putpixel(sgfx, msvc->x + i, msvc->y + j, 255, 0, 0);
            }
        }

        sgfx->blit(sgfx);
    }

    sgfx->deinit(sgfx);
    return pop_SUCCESS;
}