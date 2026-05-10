#include "popcorn.h"
#include "pop_trig.h"

typedef struct { float x,y,z; } Vec3;
typedef struct { int x,y; } Point;

#define NUM_CORNERS 8
#define TAU 6.2831853f

int _fltused = 0;

// Cube corners
Vec3 cube[NUM_CORNERS] = {
    {-1,-1,-1}, {1,-1,-1}, {-1,-1,1}, {1,-1,1},
    {-1, 1,-1}, {1, 1,-1}, {-1, 1,1}, {1, 1,1}
};

int edges[][2] = {
    {0,1},{1,3},{3,2},{2,0},
    {0,4},{1,5},{2,6},{3,7},
    {4,5},{5,7},{7,6},{6,4}
};

inline float sin(float x) {
    // Normalize angle to [0, TAU]
    while (x < 0) x += TAU;
    while (x >= TAU) x -= TAU;

    int idx = (int)(x * (TABLE_SIZE / TAU));
    return (float)sin_table[idx] / AMPLITUDE;
}

inline float cos(float x) {
    while (x < 0) x += TAU;
    while (x >= TAU) x -= TAU;

    int idx = (int)(x * (TABLE_SIZE / TAU));
    return (float)cos_table[idx] / AMPLITUDE;
}

inline int abs(int x) {
    return (x < 0) ? -x : x;
}

inline Vec3 rotate(Vec3 v, float ax, float ay, float az) {
    // rotate around x
    float y = v.y*cos(ax) - v.z*sin(ax);
    float z = v.y*sin(ax) + v.z*cos(ax);
    v.y=y; v.z=z;
    // rotate around y
    float x = v.z*sin(ay) + v.x*cos(ay);
    z = v.z*cos(ay) - v.x*sin(ay);
    v.x=x; v.z=z;
    // rotate around z
    x = v.x*cos(az) - v.y*sin(az);
    y = v.x*sin(az) + v.y*cos(az);
    v.x=x; v.y=y;
    return v;
}

inline Point project(Vec3 v, int w, int h) {
    return (Point){ (int)(v.x*(w/4)+w/2), (int)(v.y*(h/4)+h/2) };
}

// Bresenham line
inline void drawLine(popg_GraphicsServices* g, Point a, Point b) {
    int dx = abs(b.x - a.x), sx = a.x < b.x ? 1 : -1;
    int dy = -abs(b.y - a.y), sy = a.y < b.y ? 1 : -1;
    int err = dx + dy;
    int x=a.x, y=a.y;
    while (1) {
        if (x>=0 && x<g->w && y>=0 && y<g->h)
            popg_PUTPIXEL(g,x,y,255,255,255);
        if (x==b.x && y==b.y) break;
        int e2 = 2*err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }
}

int pop_API pop_main(pop_Services* svc,int argc,CHAR16**argv) {
    popg_GraphicsServices* g = svc->sgfx;
    if (g->init(g)!=pop_SUCCESS) return 1;

    float ax=0, ay=0, az=0;
    while (TRUE) {
        // clear screen
        for (unsigned int i = 0; i < g->w * g->h; i++) {
            g->frame[i] = (popg_Pixel){0,0,0};
        }

        Vec3 rc[NUM_CORNERS];
        for (int i=0;i<NUM_CORNERS;i++)
            rc[i]=rotate(cube[i],ax,ay,az);

        for (int e=0;e<12;e++) {
            Point p1=project(rc[edges[e][0]],g->w,g->h);
            Point p2=project(rc[edges[e][1]],g->w,g->h);
            drawLine(g,p1,p2);
        }

        g->blit(g);
        ax+=0.03f; ay+=0.08f; az+=0.13f;
    }
    g->deinit(g);
    return pop_SUCCESS;
}