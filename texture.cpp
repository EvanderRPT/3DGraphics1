#include <stdio.h>
#include "texture.h"
//#define PRINT_DEBUG
int texture_width = 64;
int texture_height = 64;

upng_t* png_texture = NULL;
uint32_t* mesh_texture = NULL;

void load_png_texture_data(const char* filename) {
#ifdef PRINT_DEBUG
    printf("--------1 --------\n");
#endif // PRINT_DEBUG


    png_texture = upng_new_from_file(filename);

#ifdef PRINT_DEBUG
    printf("--------2 --------\n");
#endif // PRINT_DEBUG
    if (png_texture != NULL) {
        upng_decode(png_texture);
#ifdef PRINT_DEBUG
        printf("--------3 --------\n");
#endif // PRINT_DEBUG
        if (upng_get_error(png_texture) == UPNG_EOK) {
#ifdef PRINT_DEBUG
            printf("--------4 --------\n");
#endif // PRINT_DEBUG
            mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
            texture_width = upng_get_width(png_texture);
            texture_height = upng_get_height(png_texture);
#ifdef PRINT_DEBUG
            printf("--------5 --------\n");
#endif // PRINT_DEBUG
        }
    }
}
