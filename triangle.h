#pragma once


#include "vector.h"
#include <stdint.h>
#include "texture.h"
class face_t {
public:
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color = 0xFFFFFFFF;
	face_t() : a(0), b(0), c(0) {}
	face_t(int a, int b, int c) : a(a), b(b), c(c) {}
	face_t(int a, int b, int c, uint32_t color) : a(a), b(b), c(c), color(color) {}

	face_t(int a, int b, int c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, uint32_t color) : a(a), b(b), c(c),a_uv(a_uv), b_uv(b_uv), c_uv(c_uv), color(color) {}

};

class triangle_t {
public:
	vec4_t points[3];
	tex2_t texcoords[3];

	uint32_t color = 0xFFFFFFFF;

	float avg_depth = 0.0f;
	triangle_t() {}
	triangle_t(vec4_t p1, vec4_t p2, vec4_t p3) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}

	triangle_t(vec4_t p1, vec4_t p2, vec4_t p3,  tex2_t texcoord0, tex2_t texcoord1, tex2_t texcoord2, uint32_t color, float avg_depth = 0.0f): color(color), avg_depth(avg_depth) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		texcoords[0] = texcoord0;
		texcoords[1] = texcoord1;
		texcoords[2] = texcoord2;

	}
	triangle_t(vec4_t p1, vec4_t p2, vec4_t p3, uint32_t color, float avg_depth = 0.0f) : color(color), avg_depth(avg_depth) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}
	
	void draw_filled_triangle(uint32_t color);
	void draw_triangle(uint32_t color);

	void draw_triangle();
	void draw_filled_triangle();

	void  draw_textured_triangle(uint32_t* texture);
};


