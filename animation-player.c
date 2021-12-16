#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>

#define MAX_NUM_CIRCLES 100
#define MAX_RADIUS 50
#define MAX_POSITION 400
#define MAX_COLOR 255
#define MAX_ANIMATION_VELOCITY 10

#define RAND_(max) get_random(max)
#define C_() RAND_(MAX_COLOR)
#define W_(bytes) ((bytes) / 4)
#define V_() RAND_(MAX_ANIMATION_VELOCITY)

typedef struct {
    int r;
    int g;
    int b;
    int a;
} __attribute__((packed)) color_t;

typedef struct {
    int xv; // x-axis velocity
    int yv; // y-axis velocity
    int xd; // x-axis direction
    int yd; // y-axis direction
} __attribute__((packed)) animation_t;

typedef struct {
    int x;
    int y;
    int radius;
    color_t color;
    animation_t animation;
} __attribute__((packed)) circle_t;

circle_t circles[MAX_NUM_CIRCLES];

int get_random(int max) {
    return (rand() % max);
}

circle_t* get_circles(int canvas_width, int canvas_height) {
    // Check for boundary collision and reverse the direction
    for (int at = 0; at < MAX_NUM_CIRCLES; ++at) {
        circle_t* c = &circles[at];
        // Collision on right border
        if ((c->x + c->radius) >= canvas_width) {
            c->animation.xd = 0;
        }
        
        // Collision on left border
        if ((c->x - c->radius) <= 0) {
            c->animation.xd = 1;
        }

        // Collision on top
        if ((c->y - c->radius) <= 0) {
            c->animation.yd = 1;
        }

        // Collision on bottom
        if ((c->y + c->radius) >= canvas_height) {
            c->animation.yd = 0;
        }

        // Update x and y values for animation
        if (c->animation.xd) {
            c->x += c->animation.xv;
        } else {
            c->x -= c->animation.xv;
        }

        if (c->animation.yd) {
            c->y += c->animation.yv;
        } else {
            c->y -= c->animation.yv;
        }
    }
    return circles;
}

void populate_circles() {
    for (int at = 0; at < MAX_NUM_CIRCLES; ++at) {
        int radius = RAND_(MAX_RADIUS);
        int x = RAND_(MAX_POSITION) + radius;
        int y = RAND_(MAX_POSITION) + radius;
        color_t color = {
            .r = C_(),
            .g = C_(),
            .b = C_(),
            .a = 1
        };
        animation_t a = { .xv = V_(), .yv = V_(), .xd = 1, .yd = 1};
        circle_t c = {.radius = radius, .x = x, .y = y, .color = color, .animation = a};
        circles[at] = c;
    }
}

int main() {
    // Seed random number generator
    srandom(time(NULL));
    printf("Init animation player\n");
    populate_circles();
    int circlesDataBytes = MAX_NUM_CIRCLES * sizeof(circle_t);
    EM_ASM({ render($0, $1); }, W_(circlesDataBytes), W_(sizeof(circle_t)));
    return 0;
}
