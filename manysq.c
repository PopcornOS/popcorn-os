#include "popcorn.h"

typedef struct {
    int x, y;
    int dx, dy;
    int size;
    int r, g, b;
} Square;

#define NUM_SQUARES 40

inline int abs(int x) {
    return (x < 0) ? -x : x;
}

int pop_API pop_main(pop_Services* svc, int argc, CHAR16** argv) {
    popg_GraphicsServices* sgfx = svc->sgfx;
    int code;
    if ((code = sgfx->init(sgfx)) != pop_SUCCESS) {
        svc->perrno(svc, code);
        return 1;
    }

    // Initialize squares
    Square squares[NUM_SQUARES];
    for (int i = 0; i < NUM_SQUARES; i++) {
        squares[i].x = (i * 15) % sgfx->w;
        squares[i].y = (i * 25) % sgfx->h;
        squares[i].dx = (i % 3) + 1;
        squares[i].dy = ((i+1) % 3) + 1;
        squares[i].size = 10 + (i % 10);
        squares[i].r = (i * 40) % 256;
        squares[i].g = (i * 80) % 256;
        squares[i].b = (i * 120) % 256;
    }

    while (TRUE) {
// Update positions
for (int i = 0; i < NUM_SQUARES; i++) {
    Square* s = &squares[i];
    s->x += s->dx;
    s->y += s->dy;

    // Bounce off edges
    if (s->x < 0 || s->x + s->size >= sgfx->w) s->dx = -s->dx;
    if (s->y < 0 || s->y + s->size >= sgfx->h) s->dy = -s->dy;
}

// Check collisions
for (int i = 0; i < NUM_SQUARES; i++) {
    for (int j = i+1; j < NUM_SQUARES; j++) {
        Square* a = &squares[i];
        Square* b = &squares[j];

        int overlapX = (a->x < b->x + b->size) && (a->x + a->size > b->x);
        int overlapY = (a->y < b->y + b->size) && (a->y + a->size > b->y);

        if (overlapX && overlapY) {
            // Decide axis of collision based on penetration depth
            int penX = (a->x + a->size) - b->x;
            int penY = (a->y + a->size) - b->y;

            if (penX < penY) {
                // Horizontal collision: flip dx
                a->dx = -a->dx;
                b->dx = -b->dx;

                // Separate horizontally
                if (a->x < b->x) {
                    a->x -= penX/2;
                    b->x += penX/2;
                } else {
                    a->x += penX/2;
                    b->x -= penX/2;
                }
            } else {
                // Vertical collision: flip dy
                a->dy = -a->dy;
                b->dy = -b->dy;

                // Separate vertically
                if (a->y < b->y) {
                    a->y -= penY/2;
                    b->y += penY/2;
                } else {
                    a->y += penY/2;
                    b->y -= penY/2;
                }
            }
        }
    }
}

        // Clear screen
        for (unsigned int i = 0; i < sgfx->w; i++) {
            for (unsigned int j = 0; j < sgfx->h; j++) {
                sgfx->putpixel(sgfx, i, j, 0, 0, 0);
            }
        }

        // Draw
        for (int i = 0; i < NUM_SQUARES; i++) {
            Square* s = &squares[i];
            for (int px = 0; px < s->size; px++) {
    for (int py = 0; py < s->size; py++) {
        int X = s->x + px;
        int Y = s->y + py;
        if (X >= 0 && X < sgfx->w && Y >= 0 && Y < sgfx->h)
            sgfx->putpixel(sgfx, X, Y, s->r, s->g, s->b);
    }
}
        }

        sgfx->blit(sgfx);
    }

    sgfx->deinit(sgfx);
    return pop_SUCCESS;
}
