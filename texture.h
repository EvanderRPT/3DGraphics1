#pragma once
#include <stdint.h>
#include "upng.h"
class tex2_t {
public:

    float u = 0;
    float v = 0;
    tex2_t(float u, float v) : u(u), v(v) {};
    tex2_t(){};
} ;

extern int texture_width;
extern int texture_height;
extern uint32_t* mesh_texture;
extern upng_t* png_texture;
//extern const uint8_t REDBRICK_TEXTURE[];
void load_png_texture_data(const char* filename);

